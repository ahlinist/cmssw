#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/Framework/interface/EventProcessor.h>
#include <FWCore/Utilities/interface/ProblemTracker.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DBuilder.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DAlgo.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DBuilderPluginFactory.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DFromStripsAndWires.h>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 
#include <Geometry/Records/interface/MuonGeometryRecord.h>
// #include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigiCollection.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDCCHeader.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDCCEventData.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDDUHeader.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include "EventFilter/CSCRawToDigi/interface/CSCEventData.h"
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include "EventFilter/CSCRawToDigi/interface/CSCEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCAnodeData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCALCTHeader.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCLCTData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCTMBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDDUTrailer.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDMBHeader.h"
//#include "condbc.h"

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/ParameterSet/interface/FileInPath.h>

#include "RecoLocalMuon/CSCStandAlone/interface/Strip_Fit_Constants.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/AnoTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkFit3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkLink3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDump.h"
#include "RecoLocalMuon/CSCStandAlone/interface/RootWriterRechit.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace edm;
using namespace std;


namespace test{
  
  class RecFitterComp : public edm::EDProducer {
  public:
    
    RecFitterComp( const edm::ParameterSet& ps ) : iev( 0 ) {
      
      //theMapping=CSCReadoutMappingFromFile("csc_slice_test_map.txt");
      theMapping=CSCReadoutMappingFromFile(ps);
      // register all chambers
      int dmb_val[9]={1,2,3,4,5,7,8,9,10};
      for(int vmecrate=0;vmecrate<4;vmecrate++){
	for(int i=0;i<9;i++){
	  int dmb=dmb_val[i];
	  chamb_const.Register(theMapping,vmecrate,dmb);
	}
      }
      
      
      //printf(" find the names of algorithms \n");
      // Find names of algorithms
      std::vector<std::string> algoNames = ps.getParameter<std::vector<std::string> >("algo_types");
      
      // Find appropriate ParameterSets
      std::vector<edm::ParameterSet> algoPSets = ps.getParameter<std::vector<edm::ParameterSet> >("algo_psets");
      
      // Find allocation of algorithm to chamber type
      std::vector<int> algoToType = ps.getParameter<std::vector<int> >("algo_per_chamber_type");
      
      // How many chamber types do we have? This seems hard-wirable, but what the heck
      int ntypes = ps.getParameter<int>("no_of_chamber_types");
      LogDebug("CSC") << "no. of chamber types = " << ntypes;
      //printf(" NO OF CHAMBER TYPES %d \n",ntypes);
      algos_.resize(ntypes);
      
      // Instantiate the requested algorithm(s) and buffer them
      std::vector<CSCRecHit2DAlgo*> algobuf;
      
      // Ask factory to build this algorithm, giving it appropriate ParameterSet
      for ( size_t i = 0; i < algoNames.size(); ++i ) {
	CSCRecHit2DAlgo* pAlgo = CSCRecHit2DBuilderPluginFactory::get()-> create( algoNames[i], algoPSets[i] );
	algobuf.push_back( pAlgo );
	std::cout << "algorithm [" << i << "] named " << algoNames[i] << " has address " << std::endl;
  }
      //printf(" algototype size %d \n",algoToType.size()); 
      // Register appropriate algorithm for each chamber type
      for ( size_t i = 0; i < algoToType.size(); ++i ) {
	//printf(" algototype %d %d \n",i,algoToType[i]);
	algos_[i] = algobuf[ algoToType[i] - 1 ]; // Care! std::vector index is type-1
	//std::cout << "address of algorithm for chamber type " << i << " is " << algos_[i] << std::endl;
      }
      
      fp=fopen("RecFitterComp.dat","w");
      // register what this produces
      //printf(" leaving producer \n");
      produces<CSCRecHit2DCollection>();
    }
    
    ~RecFitterComp()
    {
      LogDebug("CSC") << "deleting recHitComp after " << iev << " events.";
      // delete recHitBuilder_;
      fclose(fp);
    }
    
    void produce( edm::Event& e, const edm::EventSetup& setup )
    {
      LogDebug("CSC") << "start producing rechits for event " << ++iev;
      //printf(" start produce events \n");
      
      // find the geometry (& conditions?) for this event & cache it in the builder
      edm::ESHandle<CSCGeometry> h;
      setup.get<MuonGeometryRecord>().get( h );
      const CSCGeometry* pgeom = &*h;
      //std::cout << " pgeom address " << pgeom << std::endl; 
      
      //printf(" process some raw data \n"); 
      
      
      Handle<FEDRawDataCollection> rawdata;
      e.getByType(rawdata);
      for (int id=FEDNumbering::getCSCFEDIds().first;id<=FEDNumbering::getCSCFEDIds().second; ++id){ //for each of our DCCs
	const FEDRawData& data = rawdata->FEDData(id);
	if(size_t size=data.size()) {
	  cout << "FED# " << id << " " << size << endl;
	  if(id>=750&&id<=757){
	    unsigned short * buf = (unsigned short *)data.data();
	    CSCDCCEventData dccEvent(buf);
	    std::vector<CSCDDUEventData> & ddudata = dccEvent.dduData();
	    if(ddudata.size()==0)continue;
	    //printf(" DDU data size %d \n",ddudata.size());
	    for(unsigned iddu=0;iddu<ddudata.size();iddu++){
	      const std::vector<CSCEventData> & cscData = ddudata[iddu].cscData();
	      int lvl1num=ddudata[iddu].header().lvl1num();
	      //printf(" Level 1 Number %d \n",lvl1num);
	      //printf(" ddu number %d cscData.size() %d \n",iddu,cscData.size());
	      for (unsigned k=0; k<cscData.size(); ++k) {
		
		// stan's stuff
		AnoTrkFnd atrk(cscData[k]);              // find Anode tracks
		CatTrkFnd ctrk(theMapping,cscData[k],chamb_const);   // find Cathode tracks (1/2 strip\)
		ctrk.CatTrkTime(theMapping,cscData[k],chamb_const);             // Fit time Cathode tr\ack hits
		TrkFit3D fit(theMapping,cscData[k],ctrk,atrk,chamb_const);  // full blown gatti+
		
		// now deal with rechit 
		int vmecrate = cscData[k].dmbHeader().crateID();
		int dmb = cscData[k].dmbHeader().dmbID();
		int tmb = -1;
		int endcap = -1;
		int station = -1;
		int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
		CSCDetId tcid(id);
		//printf(" chamber %d  %d \n",tcid.chamber(),id);
		int ec=tcid.endcap();
		int st=tcid.station();
		int rg=tcid.ring();
		int ch=tcid.chamber();
		std::vector<float> arechit[6];
		std::vector<float> srechit[6];
		for(int layer=1;layer<=6;layer++){
		  CSCDetId cid(ec,st,rg,ch,layer);
		  const CSCLayer* glayer = pgeom->layer(cid);
		  CSCStripDigiCollection sdc;
		  CSCWireDigiCollection wdc;
		  std::vector <CSCStripDigi> digis = cscData[k].stripDigis(layer);
		  std::vector<CSCWireDigi> wires = cscData[k].wireDigis(layer);
		  // printf(" digi wires size %d %d \n",digis.size(),wires.size());
		  sdc.put(std::make_pair(digis.begin(),digis.end()),cid);
		  const CSCStripDigiCollection::Range tsdc = sdc.get(cid);
		  wdc.put(std::make_pair(wires.begin(),wires.end()),cid);
		  const CSCWireDigiCollection::Range twdc = wdc.get(cid);
		  if(digis.size()>0&&wires.size()>0){
		    CSCRecHit2DAlgo* alg = algos_[2];
		    // std::cout << "use the algo at address " << alg << std::endl;
		    std::vector<CSCRecHit2D> rhv = alg->run(cid,glayer,tsdc,twdc);
		    // printf("Layer %d RecHit Size %d \n",layer,rhv.size());
		    float xo=-99.0;
		    float yo=-99.0;
		    for(int rh=0;rh<rhv.size();rh++){
		      LocalPoint rh_tmp=rhv[rh].localPosition();
		      float xrh=rh_tmp.x();
		      float yrh=rh_tmp.y();
		      float dist=(xrh-xo)*(xrh-xo)+(yrh-yo)*(yrh-yo);
		      xo=xrh;
		      yo=yrh;
		      if(dist>0.0001){  
			// printf(" %d %f %f \n",rh,xrh,yrh);
			arechit[layer-1].push_back(yrh);
			srechit[layer-1].push_back(xrh);
		      }
		    } 
		  }
		}
		//printf(" end of layer loop \n");
		
		// start comparison between rechit and stan's fitting variables
		int sid=st*1000+rg*100+ch;
		if(sid==2227){
		if(fit.size()==1){
		  for(int i3d=0;i3d<fit.size();i3d++){
		    std::vector<int> trk_lay=fit.trk3D_layer(i3d);
		    std::vector<int> trk_strip=fit.trk3D_strip(i3d);
		    std::vector<float> trk_spos=fit.trk3D_spos(i3d);
		    std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
		    std::vector<float> trk_swidth=fit.trk3D_swidth(i3d);
		    std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
		    std::vector<int> trk_use=fit.trk3D_use(i3d);
		    float cslp=fit.trk3D_slp(i3d);
		    float cintr=fit.trk3D_intr(i3d);
		    int ct=fit.trk3D_cpnt(i3d);
		    std::vector<int> tmax=ctrk.ctrk_tmax(ct);
		    std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
		    int at=fit.trk3D_apnt(i3d);
		    float aslp=atrk.atrk_slp(at);
		    float aintr=atrk.atrk_intr(at);
		    std::vector<int> alay=atrk.atrk_layer(at);
		    std::vector<int> awire=atrk.atrk_wire(at);
		    if(trk_lay.size()>5){
		      for(int ihit=0;ihit<trk_lay.size();ihit++){
			for(int rhit=0;rhit<srechit[trk_lay[ihit]].size();rhit++){
			  float sta = aslp*trk_lay[ihit]+aintr;
			  float stc = cslp*trk_lay[ihit]+cintr;
			  float spac=(5.0*sta-162.3)/0.988;
			  float spcc=(trk_spos[ihit]-40.74)*trk_swidth[ihit]/1.049*(1+sta*0.0048/50);
			  for(int ahit=0;ahit<alay.size();ahit++){
			    if(alay[ahit]==trk_lay[ihit]){
			      //printf(" layer %d anode %d \n",alay[ahit],awire[ahit]);
			    }
			  }
			  int isrp=(trk_spos[ihit]+0.5*((trk_lay[ihit])%2));
			  int iwir=sta;
			  CSCDetId cid(ec,st,rg,ch,trk_lay[ihit]+1);
			  const CSCLayer* glayer = pgeom->layer(cid);
			  const CSCLayerGeometry * layerg = glayer->geometry();
			  float min=10000.;
			  int wire_rec=-99;
			  int strip_rec=-99;
			  for(int ii=-5;ii<6;ii++){
			    int jj=ii+iwir;
			    if(jj>0&&jj<64){
			      LocalPoint lp = layerg->stripWireGroupIntersection(isrp,jj);
			      float xxx = lp.x();
			      float yyy = lp.y();
			      if(abs(yyy-arechit[trk_lay[ihit]][rhit])<min){
				min=abs(yyy-arechit[trk_lay[ihit]][rhit]);
				wire_rec=jj;
			      }  
			      //  printf(" jj %d diff %f \n",jj,yyy-arechit[trk_lay[ihit]][rhit]);
			    }
			  }
			  min=1000.; 
			  for(int ii=-5;ii<6;ii++){
			    int jj=ii+isrp;
			    if(jj>0&&jj<80){
			      LocalPoint lp = layerg->stripWireGroupIntersection(jj,wire_rec);
			      float xxx = lp.x();
			      float yyy = lp.y();
			      if(abs(xxx-srechit[trk_lay[ihit]][rhit])<min){
				min=abs(xxx-srechit[trk_lay[ihit]][rhit]);
				strip_rec=jj;
			      }
			      //  printf(" jj %d diff %f \n",jj,yyy-arechit[trk_lay[ihit]][rhit]);
			    }
			  }
			  //printf(" spos gatti %f %f spos-gatti %f \n",trk_spos[ihit],trk_sgatti[ihit],trk_spos[ihit]+trk_sgatti[ihit]);
			  //printf(" strip wire %d %d %d %d \n",isrp,strip_rec,iwir,wire_rec);
			  int istc=stc;
			  float dst=stc-istc-0.5*((trk_lay[ihit]+1)%2);
			  LocalPoint lpt = layerg->stripWireGroupIntersection(istc,iwir);
			  float xxt = lpt.x();
			  LocalPoint lp = layerg->stripWireGroupIntersection(isrp,iwir);
			  float xx = lp.x();
			  float yy = lp.y();
			  LocalPoint lp2 = layerg->stripWireGroupIntersection(isrp,iwir+1);
                          float xx2 = lp2.x();
                          float yy2 = lp2.y();
                          float corr_slp=(xx2-xx)/(yy2-yy);
			  int bstrp=isrp;
			  if(isrp>79)bstrp=79;
			  LocalPoint lpp = layerg->stripWireGroupIntersection(bstrp+1,iwir);
			  LocalPoint lpm = layerg->stripWireGroupIntersection(bstrp,iwir);
			  float strp_wid = lpp.x()-lpm.x();
			  float x_trk=xxt+dst*strp_wid;
			  float ds = trk_sgatti[ihit]*strp_wid;
			  //printf(" isrp iwir width %f %d %d %f \n",trk_spos[ihit],isrp,iwir,strp_wid);     
			  //printf(" xx yy %f %f rechit %f %f \n",xx,yy,srechit[trk_lay[ihit]][rhit],arechit[trk_lay[ihit]][rhit]);
			  float diff = xx-ds-srechit[trk_lay[ihit]][rhit];
			  //printf(" diff %f %f %f \n",xx-ds,srechit[trk_lay[ihit]][rhit],diff);
			  //  fprintf(fp,"%2d %6d %6d %6d %11.5f %11.5f %11.5f %6d %11.5f %11.5f %11.5f %11.5f %11.5f \n",lvl1num,sid,trk_lay[ihit],trk_strip[ihit],trk_spos[ihit],trk_swidth[ihit],sta,srechit[trk_lay[ihit]].size(),srechit[trk_lay[ihit]][rhit],arechit[trk_lay[ihit]][rhit],stc,spac,spcc);
			  
			  if(abs(iwir-wire_rec)<2.1&&abs(strip_rec-trk_strip[ihit])<1.1){
                            float corr_srechit=srechit[trk_lay[ihit]][rhit]+(yy-arechit[trk_lay[ihit]][rhit])*corr_slp; 
			    fprintf(fp," %4d %7d %7d %7d %7d %11.5f %11.5f %11.5f %7d %11.5f %11.5f %11.5f \n",lvl1num,sid,trk_lay[ihit],trk_strip[ihit],strip_rec,xx-ds,corr_srechit,trk_sgatti[ihit],tmax[ihit],t_chg[ihit],strp_wid,x_trk);
                            printf("Y: %f %f %f %f %f \n",srechit[trk_lay[ihit]][rhit],arechit[trk_lay[ihit]][rhit],yy,corr_slp,corr_srechit);
			    printf("X: %4d %7d %7d %7d %7d %11.5f %11.5f %11.5f %7d %11.5f %11.5f %11.5f \n",lvl1num,sid,trk_lay[ihit],trk_strip[ihit],strip_rec,xx-ds,corr_srechit,trk_sgatti[ihit],tmax[ihit],t_chg[ihit],strp_wid,x_trk);

			  } 
			} 
		      }   
		    }
		  } 
		}
		// end comparison rechit with stan's tracking variables 
		
	      }
	    }
	  }
	}
      } 
    }
    } 
    
  private:
    
    FILE *fp;
    
    CSCReadoutMappingFromFile theMapping;
    Strip_Fit_Constants chamb_const;

    int iev; // events through
    std::vector<CSCRecHit2DAlgo*> algos_;
    
  };
  
  DEFINE_FWK_MODULE(RecFitterComp)
    }
