//Authors: Nicole Ippolito


#ifndef RootWriterCSCMonitor_h
#define RootWriterCSCMonitor_h

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

#include "TFile.h"
#include "TDirectory.h"
#include <iostream>
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include <cmath>

class RootWriterCSCMonitor {
  
 public:
  RootWriterCSCMonitor(){
    std::cout <<"constructing writer" << std::endl;
    init_coint=0;
  }
  
  ~RootWriterCSCMonitor(){
    std::cout <<"destructing writer" << std::endl;
  }
  
  void done(){
    
    if (myFile!=0) {
      myFile->Write();
      delete myFile;
      // myFile=0;
      std::cout<<"closing file" << std::endl;
    }  
    
  }
  
  void setup(const CSCReadoutMappingFromFile & theMapping){
    int dmb_val[9]={1,2,3,4,5,7,8,9,10};
    myFile = new TFile("CSCMonitoring.root", "RECREATE");
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

        if(init_coint==0){
          printf(" init_coint entered \n");
	  init_coint=1;
	  coint = new TH1F("coincidence","Station Coincidence Trigger", 8,0.0,8.0);
          ncoint = new TH1F("n chambers","n chambers  Trigger", 8,0.0,8.0);
          me11 = new TH1F(" ME11 Triggered","ME11",6,27.,33.);
          me12 = new TH1F(" ME12 Triggered","ME12",6,27.,33.);
          me13 = new TH1F(" ME13 Triggered","ME13",6,27.,33.);
          me21 = new TH1F(" ME21 Triggered","ME21",3,14.,17.);
          me22 = new TH1F(" ME22 Triggered","ME22",6,27.,33.);
          me31 = new TH1F(" ME31 Triggered","ME31",3,14.,17.);
          me32 = new TH1F(" ME32 Triggered","ME32",6,27.,33.);
          printf(" leave init_coint \n");
        }
        char buf[20];
        sprintf(buf,"chamber%d",sid);
        TDirectory *dir= myFile->mkdir(buf);
        dir->cd();

        char buf2[60];
        sprintf(buf2,"04 Number of Anode Tracks%d",sid);
        hnatrks[histid]= new TH1F("04 Number of Anode Tracks",buf2,20,0.0,20.0);
	sprintf(buf2,"05 Number of Cathode Tracks%d",sid);
        hnctrks[histid]= new TH1F("05 Number of Cathode Tracks",buf2,20,0.0,20.0);
        sprintf(buf2,"06 Number of 3D Tracks%d",sid); 
        hntrks[histid]= new TH1F("06 Number of 3D Tracks",buf2,20,0.0,20.0);
	sprintf(buf2, "18 Cathode  Landua Peaks%04d", sid);
	landau[histid] = new TH1F("18 Cathode Landau peak", buf2, 100, 0.0, 1000.0);
	sprintf(buf2, "19 Correlation Between Anode and Cathode Misses%d", sid);
	miss_correlation[histid] = new TH1F("19 Correlation Between Anode and Cathode Misses", buf2,4, 0.0, 4.0); 
	sprintf(buf2, "20 Missing Cathode vs. Missing Anode%d", sid);
	anode_cathode_missing[histid] = new TH2F("20 Missing Cathode vs. Missing Anode", buf2,80,0.0,80.0,64,0.0,64.0);
	anode_cathode_missing[histid] ->SetMarkerStyle(7);
	sprintf(buf2, "23 If Missing Anode, Q on Cathode%d", sid);
	cathode_missing_anode[histid] = new TH1F("23 If Missing Anode, Q on Cathode", buf2, 500, 0.0, 500.0);
	sprintf(buf2, "24 Cathode Track vs Cathode Trigger Efficiency %d", sid);
	comp_use_hist[histid] = new TH1F("24 Cathode Track vs Cathode Trigger Efficiency", buf2, 5, 0.0, 5.0);
	sprintf(buf2, "25 Hit Cathode Track and Miss Trigger Position%d", sid);
	missed_comp[histid] = new TH1F("25 Hit Cathode and Miss Trigger Position", buf2, 80, 0.0, 80.0);
	sprintf(buf2, "13 Cathode Track Peak Time%d",sid);
	cathode_pulse_peak_time[histid] = new TH1F("13 Cathode Track Peak Time", buf2, 100, 100.0, 400.0);
	sprintf(buf2, "14 Cathode Track Peak Time Bin%d", sid);
	max_time_bin[histid] = new TH1F("14 Cathode Track Peak Time Bin", buf2, 8, 0.0,8.0);
	sprintf(buf2, "15 Cathode Track Pedestal Check %d",sid);
	pedmon_chamber[histid] = new TH1F("15 Cathode Track Pedestal Check", buf2,60,-30.0,30.0);
	sprintf(buf2, "24 If Missing Comparator Hit, Q on Cathode%d",sid);
	missed_comp_charge[histid] = new TH1F("24 If Missing Comparator Hit, Q on Cathode",buf2,500,0.0,500.0); 
	sprintf(buf2, "10 Anode Track Misses%d",sid);
	ano_trk_misses[histid] = new TH1F("10 Anode Track Misses", buf2,64,0.0,64.0);
	sprintf(buf2, "17 Cathode Track Misses%d",sid);
	cat_trk_misses[histid] = new TH1F("17 Cathode Track Misses", buf2,80,0.0,80.0);
	sprintf(buf2, "16 Cathode Track Hits%d",sid);
	cat_trk_hits[histid] = new TH1F("16 Cathode Track Hits",buf2,80,0.0,80.0);
	sprintf(buf2, "11 Anode Track Angle%d",sid);
	ano_trk_angle[histid] = new TH1F("11 Anode Track Angle", buf2, 100, -10.0, 10.0);
	sprintf(buf2, "21 If Cathode Track but no Anode, Position of Missing Anode%d", sid);
	hitc_missa[histid] = new TH1F("21 If Cathode Track but no Anode, Position of Missing Anode",buf2, 64, 0.0, 64.0);
	sprintf(buf2, "22 If Anode Track but no Cathode, Position of Missing Cathode%d", sid);
	hita_missc[histid] = new TH1F("22 If Anode Track but no Cathode, Position of Missing Cathode",buf2,80, 0.0, 80.0); 
	sprintf(buf2, "07 Anode Track Hit Efficiency%d", sid);
	ano_trk_hit_eff[histid] = new TH1F("07 Anode Track Hit Efficiency",buf2, 4, -1.0, 2.0);
	sprintf(buf2, "12 Cathode Track Hit Efficiency%d",sid);
	cat_trk_hit_eff[histid] = new TH1F("12 Cathode Track Efficiency",buf2,4,-1.0,2.0);
	sprintf(buf2, "01 Anode Raw Hits Position%d",sid);
	ano_raw_hits_pos[histid] = new TH1F("01 Anode Raw Hits Position", buf2,64, 0.0, 64.0);
	sprintf(buf2, "03 Cathode Raw Hits Position%d",sid);
	cat_raw_hits_pos[histid]= new TH1F("03 Cathode Raw Hits Position",buf2,80,0.0,80.0);
	sprintf(buf2, "00 Anode Raw Hits Number%d",sid);
	ano_raw_hits_number[histid] = new TH1F("00 Anode Raw Hits Number",buf2,20,0.0,20.0);
	sprintf(buf2, "02 Cathode Raw Hits Number%d",sid);
	cat_raw_hits_number[histid] = new TH1F("02 Cathode Raw Hits Number",buf2,20,0.0,20.0);
 
	sprintf(buf2, "Anode Layer%04d", sid);
	anode_layer[histid] = new TH1F("Anode Layer", buf2, 6, 0.0, 6.0);
	sprintf(buf2, "09 Anode Track Hits%04d", sid);
	anode_wire[histid] = new TH1F("09 Anode Track Hits", buf2, 64, 0.0, 64.0);
	sprintf(buf2, "08 Anode Track Timing%04d", sid);
	anode_time[histid] = new TH1F("08 Anode Track Timing", buf2,8, 0.0,8.0);
	
	
        char buf3[20];
        sprintf(buf3,"16 Cathode Track Hits by Layer %d",sid);
        TDirectory *dir2 = dir->mkdir(buf3);
        dir2->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  hhits[j][histid] = new TH1F(buf4,buf3,80,0.,80.);
	}

	char buf5[20];
        sprintf(buf5,"18 Cathode Landau Peaks by Layer%d",sid);
        TDirectory *dir3 = dir->mkdir(buf5);
        dir3->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  charge[j][histid] = new TH1F(buf4,buf5,1000,0.,1000.);   
	}

	char buf6[20];
        sprintf(buf6,"16 Cathode Track Misses by Layer%d",sid);
        TDirectory *dir4 = dir->mkdir(buf6);
        dir4->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  cineff[j][histid] = new TH1F(buf4,buf6,81,0.,81.);   
	}

	char buf7[20];
        sprintf(buf7,"10 Anode Track Misses by Layer%d",sid);
        TDirectory *dir5 = dir->mkdir(buf7);
        dir5->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  aineff[j][histid] = new TH1F(buf4,buf7,65,0.,65.);
	}

	char anode_layer[20];
        sprintf(anode_layer,"09 Anode Track Hits by layer %d",sid);
        TDirectory *dir_4 = dir->mkdir(anode_layer);
        dir_4->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  anode_wire_layer [j][histid] = new TH1F(buf4,anode_layer,64,0.,64.);   
	}

	char anode_layer_time[20];
        sprintf(anode_layer_time,"08 Anode Track Hit Timing by Layer %d",sid);
        TDirectory *dir_5 = dir->mkdir(anode_layer_time);
        dir_5->cd(); 
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  anode_time_layer [j][histid] = new TH1F(buf4,anode_layer_time,8,0.,8.); 
	}

	
	char comp_use_layer[40];
        sprintf(comp_use_layer,"24 Cathode Track vs Cathode Trigger Efficiency by Layer%d",sid);
        TDirectory *dir_7 = dir->mkdir(comp_use_layer);
        dir_7->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  comp_use_by_layer[j][histid] = new TH1F(buf4,comp_use_layer,5,0.0,5.0);
	  
	}

	char layer_missed_comp[40];
        sprintf(layer_missed_comp,"25 Hit Cathode Track and Miss Trigger Position by Layer %d",sid);
        TDirectory *dir_8 = dir->mkdir(layer_missed_comp);
        dir_8->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  missed_comp_by_layer[j][histid] = new TH1F(buf4,layer_missed_comp,80,0.0,80.0);	  
	}
	char pulse_peak_time[40];
        sprintf(pulse_peak_time,"13 Cathode Track Peak Time by Layer %d",sid);
        TDirectory *dir_9 = dir->mkdir(pulse_peak_time);
        dir_9->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  cathode_pulse_peak_time_layer[j][histid] = new TH1F(buf4,pulse_peak_time,100,100.0,400.0);	  
	}

	char max_time_bin[40];
        sprintf(max_time_bin,"14 Cathode Track Peak Timebin by Layers %d",sid);
        TDirectory *dir_10 = dir->mkdir(max_time_bin);
        dir_10->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  max_time_bin_layer[j][histid] = new TH1F(buf4,max_time_bin,8,0.0,8.0);	  
	}
	char peak_time_check[40];
        sprintf(peak_time_check,"15 Cathode Track Pedestal Check by Layers %d",sid);
        TDirectory *dir_11 = dir->mkdir(peak_time_check);
        dir_11->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  pedmon_layer[j][histid] = new TH1F(buf4,peak_time_check,10,-30.0,30.0);	  
	}

	char layer_comp_charge[40];
        sprintf(layer_comp_charge,"26 If Missing Comparator Hit, Q on Cathode by Layers %d",sid);
        TDirectory *dir_12 = dir->mkdir(layer_comp_charge);
        dir_12->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  missed_comp_charge_by_layer[j][histid] = new TH1F(buf4,layer_comp_charge,500,0.0,500.0);	  
	}
	char raw_hits_anode[40];
        sprintf(raw_hits_anode,"01 Anode Raw Hit Position by Layers %d",sid);
        TDirectory *dir_13 = dir->mkdir(raw_hits_anode);
        dir_13->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  ano_raw_hits_pos_lay[j][histid] = new TH1F(buf4,raw_hits_anode,64,0.0,64.0);
	}
	
	char raw_hits_cathode[40];
        sprintf(raw_hits_cathode,"03 Cathode Raw Hits Position by Layers %d",sid);
        TDirectory *dir_14 = dir->mkdir(raw_hits_cathode);
        dir_14->cd();
        for(int j=0;j<6;j++){
          char buf4[20];
          sprintf(buf4,"layer%d",j);
	  cat_raw_hits_pos_lay[j][histid] = new TH1F(buf4,raw_hits_cathode,80,0.0,80.0);
	}
	char charge_bits[50];

        sprintf(charge_bits,"27 Cathode Landau Peaks by Sector%d",sid);
        printf("charge_bits %s \n",charge_bits); 

	TDirectory *dir_15 = dir->mkdir(charge_bits);
	// dir_15->cd();

        for(int j=0;j<6;j++){ //layer
          char buf4[20];
          sprintf(buf4,"layer%d",j);
          //dir_15->cd();
	  TDirectory *dir_layer = dir_15->mkdir(buf4);
	  dir_layer->cd(); 

	  for(int k=0; k<5; k++){ //first graph loop
	    char buf6[20];
	    sprintf(buf6, "CFEB%d",k);
	    for(int m=0; m<5; m++){ //second graph loop
	      char buf7[40];
	      sprintf(buf7, "%dchip %d cfeb %d wb %d ",histid,j,k,m);
              charge_sec[j][k][m][histid] = new TH1F(buf7,charge_bits,200,0.,1000.);
	    }//second loop
	  }//first graph loops
	}//layer
	
      }
      gROOT->cd();
    }
  }
  void dump3_ROOT(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {
    int inv_dmb[10]={0,1,2,3,4,0,5,6,7,8};    
    int vmecrate = data.dmbHeader().crateID();
    int dmb = data.dmbHeader().dmbID();
    int tmb = -1;
    int endcap = -1;
    int station = -1;
    int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
    CSCDetId cid(id);
    int histid=9*vmecrate+inv_dmb[dmb-1];
  
                                                               
    printf(" dump3_Root \n");
    hnatrks[histid]->Fill(atrk.size());
    hnctrks[histid]->Fill(ctrk.size());
    hntrks[histid]->Fill(fit.size());


    printf(" ** Anode Raw Hits Found \n");
    int nahits_raw=0;
    for(unsigned int l=0;l<6;l++){
      std::vector<int> raw_wire=atrk.araw_wire(l);
      std::vector<int> raw_twire=atrk.araw_twire(l);
      printf("   Layer %d Hits %d: ",l,raw_wire.size());
      nahits_raw=nahits_raw+raw_wire.size();
      for(unsigned int i=0;i<raw_wire.size();i++){
	ano_raw_hits_pos[histid]->Fill(raw_wire[i]);
	ano_raw_hits_pos_lay[l][histid]->Fill(raw_wire[i]);
      }     
    }
    ano_raw_hits_number[histid]->Fill(nahits_raw);
    printf(" ** Cathode Raw Hits Found \n");
    int nchits_raw=0;
    for(unsigned int l=0;l<6;l++){
      std::vector<int> raw_strip=ctrk.craw_strip(l);
      std::vector<float> raw_half_strip=ctrk.craw_hstrip(l);
      std::vector<int> raw_twire=ctrk.craw_use(l);
      std::vector<int> raw_pstrip=ctrk.craw_pstrip(l);
      printf("   Layer %d Hits %d: ",l,raw_strip.size());
      nchits_raw=nchits_raw+raw_strip.size();
      for(unsigned int i=0;i<raw_strip.size();i++){
	cat_raw_hits_pos[histid]->Fill(raw_strip[i]);
	cat_raw_hits_pos_lay[l][histid]->Fill(raw_strip[i]);
	
      }
    }
    cat_raw_hits_number[histid]->Fill(nchits_raw);
    /////////////////////////////Raw Information Must Go Above here, else it's below the cuts!
    if(fit.size()>1)return;
        
    for(int i3d=0;i3d<fit.size();i3d++){
      std::vector<int> trk_lay=fit.trk3D_layer(i3d);
      std::vector<int> trk_strip=fit.trk3D_strip(i3d);
      std::vector<float> trk_lpos=fit.trk3D_lpos(i3d);
      std::vector<float> trk_spos=fit.trk3D_spos(i3d);
      std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
      std::vector<int> trk_use=fit.trk3D_use(i3d);
      std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
      std::vector<float> trk_gatti_chi2=fit.trk3D_gatti_chi2(i3d);
      std::vector<int> trk_ambig=fit.trk3D_ambig(i3d);
      int ct=fit.trk3D_cpnt(i3d);
      std::vector<ADCmat> cadcmat=ctrk.ctrk_adcmat(ct);
      std::vector<int> clay = ctrk.ctrk_layer(ct);
      std::vector<int> cstrip = ctrk.ctrk_strip(ct);
      std::vector<float> chstrip=ctrk.ctrk_hstrip(ct);
      
      if(clay.size()<5)return;  

      float d[3][3];
      float cslp=fit.trk3D_slp(i3d);
      float cintr=fit.trk3D_intr(i3d);
      int at=fit.trk3D_apnt(i3d);
      std::vector<int> alay=atrk.atrk_layer(at);
      std::vector<int> awire=atrk.atrk_wire(at);
      float aslp=atrk.atrk_slp(at);
      float aintr=atrk.atrk_intr(at);
      float xtan=sqrt(cslp*cslp+aslp*aslp);
      float angle = atan(aslp);

      ano_trk_angle[histid]->Fill(angle);

      int clay_used[6] = {0,0,0,0,0,0};
      int clay_hit[6];
      int alay_used[6] = {0,0,0,0,0,0};
      for(unsigned int i=0; i<clay.size();i++){
	clay_used[clay[i]]=1;
        clay_hit[clay[i]]=i;
      }

      for (unsigned int i=0; i<alay.size(); i++){
	alay_used[alay[i]]=1;
      }

      for(int layer=0; layer<6;layer++){
	ano_trk_hit_eff[histid]->Fill(1-alay_used[layer]);
	cat_trk_hit_eff[histid]->Fill(1-clay_used[layer]);
      }      


      for(int i=0; i<6; i++){
	float xfill = clay_used[i] + 2*alay_used[i];
	miss_correlation[histid]->Fill(xfill);
	float amisspos = aslp*i + aintr;
	float cmisspos = cslp*i + cintr;
	if(clay_used[i]==0){
	  float cmisspos = cslp*i + cintr;
	  cineff[i][histid]->Fill(cmisspos);
	  cat_trk_misses[histid]->Fill(cmisspos);
	}
	if(alay_used[i]==0){
	  aineff[i][histid]->Fill(amisspos);
	  ano_trk_misses[histid]->Fill(amisspos);
        }
        if(alay_used[i]==0||clay_used[i]==0){
	  anode_cathode_missing[histid]->Fill(cmisspos,amisspos);
	}
	if(clay_used[i]==1&&alay_used[i]==0){
	  // plot  c hit and a missed stuff
	  hitc_missa[histid]->Fill(amisspos);
	}
	if(clay_used[i]==0&&alay_used[i]==1){
	  // plot  a hit and c missed stuff
	  hita_missc[histid]->Fill(cmisspos);
	}
      }
      
			      
      //////////////////I am for no good reason putting the new comparator stuff between these lines
      
      int nclct = data.dmbHeader().nclct();
      if(nclct) {
        if(data.clctData().check()) {
          float strip_stagger=0.5;
          if(cid.station()==1)strip_stagger=0.0;
          for(int layer=1;layer<=6;layer++){
            int tri_strp[2],comp2,comp_time;
	    
            std::vector <CSCComparatorDigi> digis =  data.clctData().comparatorDigis(layer);
	    // now we have comparator digis for a given layer
	    //  see if any comparator hits match our track hits
	    int comp_use=0;
	    if(clay_used[layer-1]==1){
	      for(unsigned sid=0;sid<digis.size();sid++){
		tri_strp[sid%2]=digis[sid].getStrip();
		comp2=digis[sid].getComparator();
		comp_time=digis[sid].getTimeBin();
		if(sid%2==1){
		  float comp_hstrip = -99.0;
		  if(comp2==4)comp_hstrip=tri_strp[0]-0.25;
		  if(comp2==5)comp_hstrip=tri_strp[0]+0.25;
		  if(comp2==6)comp_hstrip=tri_strp[1]-0.25;
		  if(comp2==7)comp_hstrip=tri_strp[1]+0.25;
		  comp_hstrip=comp_hstrip+1.0+(layer%2)*strip_stagger;
                  //printf(" layer comp_hstrip chstrip %d %f %f \n",layer,comp_hstrip,chstrip[clay_hit[layer-1]]);
		  if(abs(comp_hstrip-chstrip[clay_hit[layer-1]])<0.01) {comp_use=1;}
                  if(comp_use==0){
		    if(abs(comp_hstrip-chstrip[clay_hit[layer-1]])<0.51){comp_use=2;}
                  }
		  //When comp_use=1 then the hits occur on the SAME 1/2 strip, when comp_use=2 the hits are within one 
		  //1/2 strip. When they are zero there are NO HITS!!!		    	
		}
	      }
	      comp_use_hist[histid]->Fill(comp_use);
              comp_use_by_layer[layer-1][histid]->Fill(comp_use);
	      if(comp_use==0){
		  float halfstripmissedposition=chstrip[clay_hit[layer-1]]; 
		  missed_comp[histid]->Fill(halfstripmissedposition);
		  missed_comp_by_layer[layer-1][histid]->Fill(halfstripmissedposition);
		  for(unsigned k=0;k<trk_spos.size();k++){ 
		  if(trk_lay[k]+1==layer){
                    float *tmp=cadcmat[k].array();
		    int unpk=0;
		    for(int iu=0;iu<3;iu++){
		      for(int ju=0;ju<4;ju++){
			if(ju!=3)d[iu][ju]=tmp[unpk];
			unpk=unpk+1;
		      }
		    }
		    float qsum=d[0][1]+d[1][1]+d[2][1];
		      missed_comp_charge[histid]->Fill(qsum);
		      missed_comp_charge_by_layer[layer-1][histid]->Fill(qsum);
		  }
		  }      
	      }
	    } 
	  }	  
	}
      }
      
      ////////////////////////////////////End new comparator stuff 
      ////////////////////////////////////Peaking Time Stuff goes here!
      int phase,phase2;
      phase2=0;
      
      int scablock,trigtime,lctphase,l1aphase;
      for (int icfeb=0; icfeb<5;icfeb++) { //loop over cfebs in a given chamber
	const CSCCFEBData * mycfebData =  data.cfebData(icfeb);
	if (mycfebData){
	  const unsigned nt = mycfebData->nTimeSamples(); 
	  for (unsigned int itime=0; itime<nt; itime++) {//loop over time samples
	    CSCCFEBTimeSlice * mytimeSlice = (CSCCFEBTimeSlice *)mycfebData->timeSlice(itime);
	    
	    if (mytimeSlice){
	      for(unsigned i_layer = 1; i_layer <= 6; i_layer++) {//loop over all layers in chambers
		scablock = mytimeSlice->scaControllerWord(i_layer).sca_blk;
		trigtime = mytimeSlice->scaControllerWord(i_layer).trig_time;
		lctphase = mytimeSlice->scaControllerWord(i_layer).lct_phase;
		l1aphase= mytimeSlice->scaControllerWord(i_layer).l1a_phase;
		if(itime>0)phase=lctphase;
		if(itime>0)phase2=l1aphase;
	      }
	    }
	  }
	}
      }
      float ctim[6]={-99.,-99.,-99,-99.,-99.,-99.};
      //int atim[6]={0,0,0,0,0,0};
      int ttmax[6];
      if(fit.size()>1)return;
      for(int i3d=0;i3d<fit.size();i3d++){
	int ct=fit.trk3D_cpnt(i3d);
	//int at=fit.trk3D_apnt(i3d);
	std::vector<int> clay=ctrk.ctrk_layer(ct);
	std::vector<int> tmax=ctrk.ctrk_tmax(ct);
	std::vector<float> t_start=ctrk.ctrk_t_start(ct);
	std::vector<float> t_peak=ctrk.ctrk_t_peak(ct);
	std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
	std::vector<float> pedmon=ctrk.ctrk_pedmon(ct);   
	for(unsigned cl=0;cl<clay.size();cl++){
	  ctim[clay[cl]]=t_start[cl]+t_peak[cl];
          ttmax[clay[cl]]=tmax[cl];
	  
	    cathode_pulse_peak_time[histid]->Fill(ctim[clay[cl]]);
	    cathode_pulse_peak_time_layer[clay[cl]][histid]->Fill(ctim[clay[cl]]);
	    max_time_bin[histid]->Fill(ttmax[clay[cl]]);
	    max_time_bin_layer[clay[cl]][histid]->Fill(ttmax[clay[cl]]); 
	    pedmon_chamber[histid]->Fill(pedmon[cl]);
	    pedmon_layer[cl][histid]->Fill(pedmon[cl]);
	 
	}
      }
      ///////////////////////////////////End Peaking Time stuff
      
      float th=atan(xtan);
      float xcos=cos(th);
      if((trk_spos.size()>=5)&&(fit.size()==1)){
	for(unsigned k=0;k<trk_spos.size();k++){
	  
	  float *tmp=cadcmat[k].array();
	  int unpk=0;
	  for(int iu=0;iu<3;iu++){
	    for(int ju=0;ju<4;ju++){
	      if(ju!=3)d[iu][ju]=tmp[unpk];
	      unpk=unpk+1;
	    }
	  }
	  float qsum=d[0][1]+d[1][1]+d[2][1];
	  
	  if(alay_used[trk_lay[k]]==0){
	    cathode_missing_anode[histid]->Fill(qsum);
	  }
	  hhits[trk_lay[k]][histid]->Fill(trk_strip[k]);
	  cat_trk_hits[histid]->Fill(trk_strip[k]);
	  charge[trk_lay[k]][histid]->Fill(qsum*xcos);
	  landau[histid]->Fill(qsum*xcos);

	}
      }
    }

    float d[3][3];
    for(int at=0;at<atrk.size();at++){
      std::vector<int> alay=atrk.atrk_layer(at);
      std::vector<int> awire=atrk.atrk_wire(at);
      for(int ct=0;ct<ctrk.size();ct++){
	std::vector<int> clay=ctrk.ctrk_layer(ct);
	std::vector<int> cstrip=ctrk.ctrk_strip(ct);
	std::vector<ADCmat> cadcmat=ctrk.ctrk_adcmat(ct);
	for(unsigned ia=0;ia<alay.size();ia++){
	  for(unsigned ic=0;ic<clay.size();ic++){
	    if(alay[ia]==clay[ic]){
	      float *tmp=cadcmat[ic].array();
	      int unpk=0;
	      for(int iu=0;iu<3;iu++){
		for(int ju=0;ju<4;ju++){
		  if(ju!=3)d[iu][ju]=tmp[unpk];
		  unpk=unpk+1;
		}
	      }
	      float qsum=d[0][1]+d[1][1]+d[2][1];
	      int gwire_set[5]={0,16,28,30,52};
	      int igwire=0;
	      for(int i=0;i<5;i++){
		if(awire[ia]>gwire_set[i])igwire=i;
	      }
              //printf(" awire[ia] igwire %d %d \n",awire[ia],igwire);
              int icfeb=(cstrip[ic]-1)/16; //does layer count from 1 or 0?
	      int layer=alay[ia]; //does layer count from 1 or 0?
	      //printf(" layer %d icfeb %d igwire %d histid %d qsum %f\n",layer,icfeb,igwire,histid,qsum);
	      charge_sec[layer][icfeb][igwire][histid]->Fill(qsum); 
	    }
	  }
	}  
      }
    }   
    
    
  
  }
  
  
  
  
  void print_raw_tracks_ROOT(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {
    int inv_dmb[10]={0,1,2,3,4,0,5,6,7,8};    
    int vmecrate = data.dmbHeader().crateID();
    int dmb = data.dmbHeader().dmbID();
    int histid=9*vmecrate+inv_dmb[dmb-1];
    
    for(int at=0;at<atrk.size();at++){
      
      std::vector<int> alay=atrk.atrk_layer(at);
      std::vector<int> awire=atrk.atrk_wire(at);
      std::vector<int> twire=atrk.atrk_twire(at);
      for(unsigned i=0;i<alay.size();i++){
	
	anode_layer[histid]->Fill(alay[i]);
	anode_wire[histid]->Fill(awire[i]);
	anode_time[histid]->Fill(twire[i]);
	anode_wire_layer[alay[i]][histid]->Fill(awire[i]);
	anode_time_layer[alay[i]][histid]->Fill(twire[i]);
	
      }
    }
    
    for(int ct=0;ct<ctrk.size();ct++){
      
      std::vector<int> clay=ctrk.ctrk_layer(ct);
      std::vector<int> cstrip=ctrk.ctrk_strip(ct);
      std::vector<float> chstrip=ctrk.ctrk_hstrip(ct);
      for(unsigned i=0;i<clay.size();i++){
      }
      
    }
    
  }
  
  
  void ZeroTrigID(){ntrigid=0;}
  
  
  void FillTrigID(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data )
    {
      printf(" Fill TrigID called \n");
      int vmecrate = data.dmbHeader().crateID();
      int dmb = data.dmbHeader().dmbID();
      int tmb = -1;
      int endcap = -1;
      int station = -1;
      int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
      CSCDetId cid(id);
      printf(" station ring chamber %d %d %d \n",cid.station(),cid.ring(),cid.chamber());
      station=cid.station();
      int ring=cid.ring();
      int chamber=cid.chamber();
      if(station==1&&ring==1)me11->Fill(chamber);
      if(station==1&&ring==2)me12->Fill(chamber);
      if(station==1&&ring==3)me13->Fill(chamber);
      if(station==2&&ring==1)me21->Fill(chamber);
      if(station==2&&ring==2)me22->Fill(chamber);
      if(station==3&&ring==1)me31->Fill(chamber);
      if(station==3&&ring==2)me32->Fill(chamber);
      
      if(ntrigid<30){
        trigid[ntrigid]=cid.station();
        ntrigid=ntrigid+1;
      }
    }

  void PlotTrigID()
    {
      //printf(" ntrigid %d \n",ntrigid);
      //for(int i=0;i<ntrigid;i++)printf(" trigid %d station%d \n",i,trigid[i]);
      int j[3]={0,0,0};  
      for(int i=0; i<ntrigid; i++){
	if(trigid[i]==1){j[0]=1;}
	if(trigid[i]==2){j[1]=1;}
	if(trigid[i]==3){j[2]=1;}

      }
      int coin=0;
      if(j[0]==1 && j[1]==1 && j[2]==1){coin=1;}
      if(j[0]==1 && j[1]==1){coin=2;}
      if(j[0]==1 && j[2]==1){coin=3;}
      if(j[1]==1 && j[2]==1){coin=4;}
      if(j[0]==1){coin=5;}
      if(j[1]==1){coin=6;}
      if(j[2]==1){coin=7;}
      coint->Fill(coin);
      ncoint->Fill(ntrigid);
    }
  
  
  
 private:

  int init_coint;
  
  int ntrigid;
  int trigid[30];
  
  TFile *myFile;
  TTree *tree;
  TDirectory *dir;
  
  TH1F *hhits[6][36];
  TH1F *hnatrks[36];
  TH1F *hnctrks[36];
  TH1F *hntrks[36];
  TH1F *landau[36];
  TH1F *charge[6][36];
  TH1F *cineff[6][36];
  TH1F *aineff[6][36];
  TH1F *anode_layer[36];
  TH1F *anode_wire[36];
  TH1F *anode_time[36];
  TH1F *anode_wire_layer[6][36];
  TH1F *anode_time_layer[6][36];
  TH1F *miss_correlation[36];
  TH2F *anode_cathode_missing[36];
  TH1F *cathode_missing_anode[36];
  TH1F *comp_hstrip_chamber[36];
  TH1F *hits_found_same_half_strip[36];
  TH1F *comp_use_hist[36];
  TH1F *comp_use_by_layer[6][36];
  TH1F *missed_comp[36];
  TH1F *missed_comp_by_layer[6][36];
  TH1F *cathode_pulse_peak_time[36];
  TH1F *cathode_pulse_peak_time_layer[6][36];
  TH1F *max_time_bin[36];
  TH1F *max_time_bin_layer[6][36];
  TH1F *pedmon_chamber[36];
  TH1F *pedmon_layer[6][36];
  TH1F *missed_comp_charge[36];
  TH1F *missed_comp_charge_by_layer[6][36];
  TH1F *coint;
  TH1F *ncoint;
  TH1F *me11;
  TH1F *me12;
  TH1F *me13;
  TH1F *me21;
  TH1F *me22;
  TH1F *me31;
  TH1F *me32; 
  TH1F *ano_trk_misses[36];
  TH1F *cat_trk_misses[36];
  TH1F *cat_trk_hits[36];
  TH1F *ano_trk_angle[36];
  TH1F *hitc_missa[36];
  TH1F *hita_missc[36];
  TH1F *ano_trk_hit_eff[36];
  TH1F *cat_trk_hit_eff[36];
  TH1F *ano_raw_hits_pos[36];
  TH1F *ano_raw_hits_pos_lay[6][36];
  TH1F *cat_raw_hits_pos[36];
  TH1F *cat_raw_hits_pos_lay[6][36];
  TH1F *ano_raw_hits_number[36];
  TH1F *cat_raw_hits_number[36];
  TH1F *charge_sec[6][5][5][36];
   
};

#endif
