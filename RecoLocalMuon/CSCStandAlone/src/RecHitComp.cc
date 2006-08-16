//Authors: S Durkin, N Ippolito

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
#include "RecoLocalMuon/CSCStandAlone/interface/rechit_pass.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
                                                                                
using namespace edm;
using namespace std;


namespace test{
  
  class RecHitComp : public edm::EDProducer {
  public:
    
    RecHitComp( const edm::ParameterSet& ps ) : iev( 0 ) {
      
      //theMapping=CSCReadoutMappingFromFile("csc_slice_test_map.txt");
      //string theMappingFile = pset.getParameter<std::string>("theMappingFile");
      //theMapping=CSCReadoutMappingFromFile(theMappingFile);
      theMapping=CSCReadoutMappingFromFile(ps);
      
      writer.setup(theMapping);
      
      // register all chambers
      int dmb_val[9]={1,2,3,4,5,7,8,9,10};
      for(int vmecrate=0;vmecrate<4;vmecrate++){
	for(int i=0;i<9;i++){
	  int dmb=dmb_val[i];
	  chamb_const.Register(theMapping,vmecrate,dmb);
	}
      }
    

  
  // Find names of algorithms
  std::vector<std::string> algoNames = ps.getParameter<std::vector<std::string> >("algo_types");

  // Find appropriate ParameterSets
  std::vector<edm::ParameterSet> algoPSets = ps.getParameter<std::vector<edm::ParameterSet> >("algo_psets");

  // Find allocation of algorithm to chamber type
  std::vector<int> algoToType = ps.getParameter<std::vector<int> >("algo_per_chamber_type");

  // How many chamber types do we have? This seems hard-wirable, but what the heck
  int ntypes = ps.getParameter<int>("no_of_chamber_types");
  LogDebug("CSC") << "no. of chamber types = " << ntypes;
  algos_.resize(ntypes);

  // Instantiate the requested algorithm(s) and buffer them
  std::vector<CSCRecHit2DAlgo*> algobuf;

  // Ask factory to build this algorithm, giving it appropriate ParameterSet
  for ( size_t i = 0; i < algoNames.size(); ++i ) {
    CSCRecHit2DAlgo* pAlgo = CSCRecHit2DBuilderPluginFactory::get()-> create( algoNames[i], algoPSets[i] );
    algobuf.push_back( pAlgo );
    
  }
  
  // Register appropriate algorithm for each chamber type
  for ( size_t i = 0; i < algoToType.size(); ++i ) {
    algos_[i] = algobuf[ algoToType[i] - 1 ]; // Care! std::vector index is type-1
    
  }

  //fp=fopen("rechitcomp.dat","w");
  // register what this produces
  //printf(" leaving producer \n");
  produces<CSCRecHit2DCollection>();
}

~RecHitComp()
{
  LogDebug("CSC") << "deleting recHitComp after " << iev << " events.";
  // delete recHitBuilder_;
  //fclose(fp);
  writer.done();
}

void produce( edm::Event& e, const edm::EventSetup& setup )
{
  LogDebug("CSC") << "start producing rechits for event " << ++iev;
  
  // find the geometry (& conditions?) for this event & cache it in the builder
  edm::ESHandle<CSCGeometry> h;
  setup.get<MuonGeometryRecord>().get( h );
  const CSCGeometry* pgeom = &*h;
  Handle<FEDRawDataCollection> rawdata;
  //e.getByLabel("DaqSource", rawdata);
  e.getByType(rawdata);
  for (int id=FEDNumbering::getCSCFEDIds().first;id<=FEDNumbering::getCSCFEDIds().second; ++id){ //for each of our DCCs
    const FEDRawData& data = rawdata->FEDData(id);
    if(size_t size=data.size()) {
      if(id>=750&&id<=757){
	unsigned short * buf = (unsigned short *)data.data();
	CSCDCCEventData dccEvent(buf);
        std::vector<CSCDDUEventData> & ddudata = dccEvent.dduData();
        if(ddudata.size()==0)continue;
	for(unsigned iddu=0;iddu<ddudata.size();iddu++){
	  const std::vector<CSCEventData> & cscData = ddudata[iddu].cscData();
	  //int lvl1num=ddudata[iddu].header().lvl1num();
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
            
            int ec=tcid.endcap();
            int st=tcid.station();
            int rg=tcid.ring();
            int ch=tcid.chamber();
	
	     rechit_pass rhp; //and added this!
	  
            for(int layer=1;layer<=6;layer++){
              CSCDetId cid(ec,st,rg,ch,layer);
	      const CSCLayer* glayer = pgeom->layer(cid);
	      CSCStripDigiCollection sdc;
              CSCWireDigiCollection wdc;
              std::vector <CSCStripDigi> digis = cscData[k].stripDigis(layer);
	      std::vector<CSCWireDigi> wires = cscData[k].wireDigis(layer);
              
              sdc.put(std::make_pair(digis.begin(),digis.end()),cid);
              const CSCStripDigiCollection::Range tsdc = sdc.get(cid);
              wdc.put(std::make_pair(wires.begin(),wires.end()),cid);
              const CSCWireDigiCollection::Range twdc = wdc.get(cid);
              if(digis.size()>0&&wires.size()>0){
		CSCRecHit2DAlgo* alg = algos_[2];
		
                std::vector<CSCRecHit2D> rhv = alg->run(cid,glayer,tsdc,twdc);
                
                float xo=-99.0;
                float yo=-99.0;
                for(unsigned int rh=0;rh<rhv.size();rh++){
		  LocalPoint rh_tmp=rhv[rh].localPosition();
                  float xrh=rh_tmp.x();
                  float yrh=rh_tmp.y();
                  float dist=(xrh-xo)*(xrh-xo)+(yrh-yo)*(yrh-yo);
                  xo=xrh;
                  yo=yrh;
                  if(dist>0.0001){  
		    rhp.rechit_fill(layer,xrh,yrh);   //and added this one
                  }
	        } 
              }
            }
                        
	    // start comparison between rechit and stan's fitting variables
            //Nicole moved this stuff into a .h file for histogramming!
            // end comparison rechit with stan's tracking variables 
	    writer.comparison(theMapping,cscData[k],ctrk,atrk,fit,pgeom,rhp);
          }
        }
      }
    }
  } 
}


private:

//FILE *fp;

RootWriterRechit writer;

CSCReadoutMappingFromFile theMapping;
Strip_Fit_Constants chamb_const;

int iev; // events through
std::vector<CSCRecHit2DAlgo*> algos_;

};
                                                                                
DEFINE_FWK_MODULE(RecHitComp)
  }
