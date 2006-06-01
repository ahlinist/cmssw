//Authors: Nicole Ippolito

#ifndef RootWriterRechit_h
#define RootWriterRechit_h

#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "EventFilter/CSCRawToDigi/interface/CSCCLCTData.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigi.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCFEBTimeSlice.h"
#include "RecoLocalMuon/CSCStandAlone/interface/rechit_pass.h"
#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>

#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TCanvas.h>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TFile.h>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TDirectory.h>
#include <iostream>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TTree.h>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TH1F.h>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TH2F.h>

#include <cmath>


class RootWriterRechit {
  
 public:
  RootWriterRechit(){
    counter1=0;
    counter2=0;
    counter3=0;
    ratio=0;
  }
  
  ~RootWriterRechit(){
  }
  
  void done(){
    if (myFile!=0) {
      myFile->Write();
      delete myFile;
    }      
  }
  
  void setup(const CSCReadoutMappingFromFile & theMapping){
    int dmb_val[9]={1,2,3,4,5,7,8,9,10};
    myFile = new TFile("RechitComparison.root", "RECREATE");
    for(int vmecrate=0;vmecrate<4;vmecrate++){
      for(int i=0;i<9;i++){
        int dmb=dmb_val[i];
        int tmb = -1;
        int endcap = -1;
        int station = -1;
        int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
        CSCDetId cid(id);
	
        int sid=cid.station()*1000+cid.ring()*100+cid.chamber();
        int histid=vmecrate*9+i;

	//efficiency_by_event = new TH1F(" Efficiency", "Ratio of Efficiencies", 5, 0.0, 5.0);
	
	char buf[20];
        sprintf(buf,"chamber%d",sid);
        TDirectory *dir= myFile->mkdir(buf);
        dir->cd();

	char buf2[60];
	sprintf(buf2, "TrackFind Cathode Hits vs Rechits%d", sid);
	stan_vs_tim_cathode[histid] = new TH2F("TrackFind Cathode Hits vs. Rechits", buf2,50,-50.0,50.0,50,-50.0,50.0);
	stan_vs_tim_cathode[histid] ->SetMarkerStyle(7);

	sprintf(buf2, "TrackFind Anode Hits vs Rechits%d", sid);
	stan_vs_tim_anode[histid] = new TH2F("TrackFind Anode Hits vs. Rechits", buf2,100,-100.0,100.0,100,-100.0,100.0);
	stan_vs_tim_anode[histid] ->SetMarkerStyle(7);

	sprintf(buf2,"TrackFind Cathode Hit Minus Rechit%d",sid);
        rechit_diff_cat[histid]= new TH1F("TrackFind Cathode Hit Minus Rechit",buf2,100,-10.0,10.0);

	sprintf(buf2,"TrackFind Anode Hit Minus Rechit%d",sid);
        rechit_diff_ano[histid]= new TH1F("TrackFind Anode Hit Minus Rechit",buf2,100,-10.0,10.0);

	
	sprintf(buf2,"TrackFind to Rechit Efficiency%d",sid);
        missing_plot[histid]= new TH1F("TrackFind to Rechit Efficiency",buf2,5,0.0,5.0);

	sprintf(buf2,"Raw Rechit Hits%d",sid);
        rechit_hits_raw[histid]= new TH1F("Raw Rechit Hits",buf2,100,-50.0,50.0);

	char buf3[20];
	sprintf(buf3, "TrackFind to Rechit Efficiency by Layer %d", sid);
	TDirectory *dir2 = dir->mkdir(buf3);
	dir2->cd();
	for(int j=0;j<6;j++){
	  char buf4[20];
	  sprintf(buf4, "layer%d", j);
	  missing_plot_by_layers[j][histid] = new TH1F(buf4,buf3,5,0.0,5.0);
	}

	
      }
    }
  }
  
  void comparison(CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit,const CSCGeometry * &pgeom, rechit_pass & rhp ){
    int inv_dmb[10]={0,1,2,3,4,0,5,6,7,8};    
    int vmecrate = data.dmbHeader().crateID();
    int dmb = data.dmbHeader().dmbID();
    int tmb = -1;
    int endcap = -1;
    int station = -1;
    int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
    CSCDetId tcid(id);
    int histid=9*vmecrate+inv_dmb[dmb-1];
    int ec=tcid.endcap();
    int st=tcid.station();
    int rg=tcid.ring();
    int ch=tcid.chamber();
    int sid=st*1000+rg*100+ch;

   
    
    if(fit.size()==1){
      for(int i3d=0;i3d<fit.size();i3d++){
	std::vector<int> trk_lay=fit.trk3D_layer(i3d);
	std::vector<int> trk_strip=fit.trk3D_strip(i3d);
	std::vector<float> trk_spos=fit.trk3D_spos(i3d);
	std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
	std::vector<float> trk_swidth=fit.trk3D_swidth(i3d);
	std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
	std::vector<int> trk_use=fit.trk3D_use(i3d);
	//float cslp=fit.trk3D_slp(i3d);
	//float cintr=fit.trk3D_intr(i3d);
	int at=fit.trk3D_apnt(i3d);
	int ct=fit.trk3D_cpnt(i3d);
	std::vector<int> clay = ctrk.ctrk_layer(ct);
	std::vector<int> alay=atrk.atrk_layer(at);
	float aslp=atrk.atrk_slp(at);
	float aintr=atrk.atrk_intr(at);
	int trkfnd_use[6]={0,0,0,0,0,0};
	//int trkfnd_use_ano[6]={0,0,0,0,0,0};
	int recfnd_use_cat[6]={0,0,0,0,0,0};
	int recfnd_use_ano[6]={0,0,0,0,0,0};
	int rechit_hits=0;

	if(trk_lay.size()>5){
	  
	  for(unsigned int ihit=0;ihit<trk_lay.size();ihit++){
	    
	    trkfnd_use[trk_lay[ihit]]=1;   //is this right?
	    //trkfnd_use_ano[alay[ihit]]=1;
	    
 	    CSCDetId cid(ec,st,rg,ch,trk_lay[ihit]+1);
	    const CSCLayer* glayer = pgeom->layer(cid);
	    const CSCLayerGeometry * layerg = glayer->geometry();
            std::vector<float> arechit = rhp.rechita(trk_lay[ihit]);
	    std::vector<float> srechit = rhp.rechitb(trk_lay[ihit]);
	    // work out trkfnd coodinates
	    //float sta = aslp*trk_lay[ihit]+aintr;
	    int isrp=(trk_spos[ihit]+0.5*((trk_lay[ihit])%2));
	    int iwir=aslp*trk_lay[ihit]+aintr;
	    LocalPoint lp = layerg->stripWireGroupIntersection(isrp,iwir);
	    float xx = lp.x();
	    float yy = lp.y();
	    int bstrp=isrp;
	    if(isrp>79)bstrp=79;
	    LocalPoint lpp = layerg->stripWireGroupIntersection(bstrp+1,iwir);
	    LocalPoint lpm = layerg->stripWireGroupIntersection(bstrp,iwir);
	    float strp_wid = lpp.x()-lpm.x();
	    float ds = trk_sgatti[ihit]*strp_wid;
	    float x_trkfnd=xx-ds;
            float y_trkfnd=yy; 
	    
	    rechit_hits=rechit_hits+srechit.size(); 
	    
	    if(trk_lay.size()>5){
	      for(unsigned int rhit=0;rhit<srechit.size();rhit++){
		//recfnd_use[trk_lay[ihit]]=1; 
		//printf(" %d %f %f %f %f \n",trk_lay[ihit],x_trkfnd,y_trkfnd,srechit[rhit],arechit[rhit]);
		stan_vs_tim_cathode[histid]->Fill(x_trkfnd,srechit[rhit]);
		
		float cat_difference=x_trkfnd-srechit[rhit];
		rechit_diff_cat[histid]->Fill(cat_difference);
		float ano_diff=y_trkfnd-arechit[rhit];
		rechit_diff_ano[histid]->Fill(ano_diff);
		
		if(abs(x_trkfnd-srechit[rhit])<2.0){recfnd_use_cat[trk_lay[ihit]]=1;}
		if(abs(y_trkfnd-arechit[rhit])<15.0){recfnd_use_ano[trk_lay[ihit]]=1;}
		
		// nicole - this is the only correct code down here				
	      }
            }  
	  } 
	}
	// at this point recfnd_use and trkfnd_use are filled. Do your histogramming here!
	if(sid==2227){
	  for(int it=0; it<6; it++){
	    float eff=-99.0;
	    
	    if(trkfnd_use[it]==1){
	      if(trkfnd_use[it]==1 && recfnd_use_cat[it]==1 && recfnd_use_ano[it]==1){eff=1.1;}else{eff=2.1;}
	      if(trkfnd_use[it]==1){counter1=counter1+1;}
	      if(trkfnd_use[it]==1 && recfnd_use_cat[it]==1 && recfnd_use_ano[it]==1){counter2=counter2+1;}
	      if(recfnd_use_cat[it]==1 && recfnd_use_ano[it]==1){counter3=counter3+1;}
	      ratio=(float)counter2/(float)counter1;
	      std::cout << " counter1 " << counter1 << std::endl;
	      std::cout << " counter2 " << counter2 << std::endl;
	      std::cout << " counter3 " << counter3 << std::endl;
	      std::cout << " ratio " << ratio << std::endl;
	      missing_plot[histid]->Fill(eff);
	      //efficiency_by_event->Fill(eff);
	      rechit_hits_raw[histid]->Fill(rechit_hits);
	      missing_plot_by_layers[it][histid]->Fill(eff);
	    }
	  }
	}
      }
    }  
  }
  
 private:
  
  TFile *myFile;
  TDirectory *dir;
  TH2F *stan_vs_tim_cathode[36];
  TH2F *stan_vs_tim_anode[36];

  TH1F *rechit_diff_cat[36];
  TH1F *rechit_diff_ano[36];
  TH1F *missing_plot[36];
  TH1F *missing_plot_by_layers[6][36];
  TH1F *rechit_hits_raw[36];
  //TH1F *efficiency_by_event;
  int counter1;
  int counter2;
  int counter3;
  float ratio;
  
};

#endif
