#include "TFile.h"
#include "TTree.h"
#include "stdlib.h"

class leddata{
public:
  leddata(){};
  bool   isset;
  double energy;
  double e_rms;
  double time;
  double t_rms;
};


class LED_RUN{
public:
   LED_RUN(char *filename){
      double led,rms,time,rms_time;
      int Eta,Phi,Depth,rbx,chan,run;
      bool isHFP=false,isHFM=false,isHBP=false,isHBM=false,isHEP=false,isHEM=false;
      char subdet[10];
        for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++){
           hb[i][j][k].isset=he[i][j][k].isset=ho[i][j][k].isset=hf[i][j][k].isset=false; 
        }
        for(int i=0;i<5;i++)for(int j=0;j<5;j++)for(int k=0;k<72;k++){
             calib_data[i][j][k].isset=false;
        }
        TFile* f = new TFile(filename,"READ");
        TObjString *STR=(TObjString *)f->Get("run number");
        sscanf(STR->String().Data(),"%i",&run); RUN=run;
        if(!f->IsOpen()){ printf("Enable to load led data...\n"); return; }

        TTree*  t=(TTree*)f->Get("HCAL LED data");
        if(!t) return;
        t->SetBranchAddress("Subdet",    subdet);
        t->SetBranchAddress("eta",       &Eta);
        t->SetBranchAddress("eta",       &Eta);
        t->SetBranchAddress("phi",       &Phi);
        t->SetBranchAddress("depth",     &Depth);
        t->SetBranchAddress("led",       &led);
        t->SetBranchAddress("rms",       &rms);
        t->SetBranchAddress("time",      &time);
        t->SetBranchAddress("time_rms",  &rms_time);
    
        for(int ievt=0;ievt<t->GetEntries();ievt++){
           t->GetEntry(ievt);
           if(strcmp(subdet,"HB")==0){
	        hb[Eta+50][Phi][Depth].isset=true;
	        hb[Eta+50][Phi][Depth].energy=led;
	        hb[Eta+50][Phi][Depth].e_rms=rms;
	        hb[Eta+50][Phi][Depth].time=time;
	        hb[Eta+50][Phi][Depth].t_rms=rms_time;   
		if(Eta>0) isHBP=true;         
		if(Eta<0) isHBM=true;         
	   }
	   if(strcmp(subdet,"HE")==0){
	        he[Eta+50][Phi][Depth].isset=true;
	        he[Eta+50][Phi][Depth].energy=led;
	        he[Eta+50][Phi][Depth].e_rms=rms;
	        he[Eta+50][Phi][Depth].time=time;
	        he[Eta+50][Phi][Depth].t_rms=rms_time;   
		if(Eta>0) isHEP=true;         
		if(Eta<0) isHEM=true;         
	   }
	   if(strcmp(subdet,"HO")==0){
	        ho[Eta+50][Phi][Depth].isset=true;
	        ho[Eta+50][Phi][Depth].energy=led;
	        ho[Eta+50][Phi][Depth].e_rms=rms;
	        ho[Eta+50][Phi][Depth].time=time;
	        ho[Eta+50][Phi][Depth].t_rms=rms_time;    
	   } 
           if(strcmp(subdet,"HF")==0){
	        hf[Eta+50][Phi][Depth].isset=true;
	        hf[Eta+50][Phi][Depth].energy=led;
	        hf[Eta+50][Phi][Depth].e_rms=rms;
	        hf[Eta+50][Phi][Depth].time=time;
	        hf[Eta+50][Phi][Depth].t_rms=rms_time; 
		if(Eta>0) isHFP=true;         
		if(Eta<0) isHFM=true;         
	   }
           if(strcmp(subdet,"CALIB_HB")==0 && Depth==0){
	        calib_data[1][Eta+2][Phi].isset=true; 
	        calib_data[1][Eta+2][Phi].energy=led;
	        calib_data[1][Eta+2][Phi].e_rms=rms;
	        calib_data[1][Eta+2][Phi].time=time;
	        calib_data[1][Eta+2][Phi].t_rms=rms_time;           
	   }
           if(strcmp(subdet,"CALIB_HE")==0 && Depth==0){
	        calib_data[2][Eta+2][Phi].isset=true;
	        calib_data[2][Eta+2][Phi].energy=led;
	        calib_data[2][Eta+2][Phi].e_rms=rms;
	        calib_data[2][Eta+2][Phi].time=time;
	        calib_data[2][Eta+2][Phi].t_rms=rms_time;           
	   }
           if(strcmp(subdet,"CALIB_HO")==0){
	        calib_data[3][Eta+2][Phi].isset=true;
	        calib_data[3][Eta+2][Phi].energy=led;
	        calib_data[3][Eta+2][Phi].e_rms=rms;
	        calib_data[3][Eta+2][Phi].time=time;
	        calib_data[3][Eta+2][Phi].t_rms=rms_time;           
	   }
           if(strcmp(subdet,"CALIB_HF")==0){
	        calib_data[4][Eta+2][Phi].isset=true;
	        calib_data[4][Eta+2][Phi].energy=led;
	        calib_data[4][Eta+2][Phi].e_rms=rms;
	        calib_data[4][Eta+2][Phi].time=time;
	        calib_data[4][Eta+2][Phi].t_rms=rms_time;           
	   }
	}
        f->Close(); 
	char *hfdata="";
	if(isHFP)  hfdata="HFP";       
	if(isHFM)  hfdata="HFM";       
	if(isHFP && isHFM)  hfdata="HFP+HFM";       

	printf("run %i: led data loaded... %s\n",RUN,hfdata);  
   }
   int  get_run_number(){ return RUN; }
   bool get_led_value(char *SUBDET,int ETA,int PHI,int DEPTH,leddata **VAL){
          if(strcmp(SUBDET,"HB")==0) *VAL=&hb[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HE")==0) *VAL=&he[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HO")==0) *VAL=&ho[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HF")==0) *VAL=&hf[ETA+50][PHI][DEPTH];
	  if(((leddata *)*VAL)->isset==false) return false;
	  return true;
   }
   bool get_calib_value(char *SUBDET,int eta,int phi,leddata **VAL){
          int SD=0,ETA=0,PHI=0;
	  if(eta>0) eta=1; else eta=-1; 
          if(strcmp(SUBDET,"HB")==0)SD=1; 
          if(strcmp(SUBDET,"HE")==0)SD=2; 
          if(strcmp(SUBDET,"HO")==0)SD=3; 
          if(strcmp(SUBDET,"HF")==0)SD=4; 
          if(SD==1 || SD==2){
             if(eta>0) ETA=1; else ETA=-1;
             if(phi==71 ||phi==72 || phi==1 || phi==2) PHI=71; else PHI=((phi-3)/4)*4+3;
          }else if(SD==3){
             if(abs(eta)<=4){
                 ETA=0;
                 if(phi==71 ||phi==72 || phi==1 || phi==2 || phi==3 || phi==4) PHI=71; else PHI=((phi-5)/6)*6+5;
             }else{
                 if(abs(eta)>4  && abs(eta)<=10)  ETA=1;
                 if(abs(eta)>10 && abs(eta)<=15)  ETA=2;
                 if(eta<0) ETA=-ETA;
	         if(phi==71 ||phi==72 || (phi>=1 && phi<=10)) PHI=71; else PHI=((phi-11)/12)*12+11;
             }
	  }else if(SD==4){
             if(eta>0) ETA=1; else ETA=-1;
	     if(phi>=1  && phi<=18) PHI=1;
	     if(phi>=19 && phi<=36) PHI=19;
	     if(phi>=37 && phi<=54) PHI=37;
	     if(phi>=55 && phi<=72) PHI=55;
          }
       
	  *VAL=&calib_data[SD][ETA+2][PHI];
          if(calib_data[SD][ETA+2][PHI].isset==false) return false;
	  return true;
   }
   bool get_average_phi(char *SUBDET,int PHI,double *ave_e,double *ave_t){
      if(strcmp(SUBDET,"HBP")==0){
          double sum=0,n=0,sumt=0,nt=0;
	  for(int eta=1;eta<=14;eta++){
            if(hb[eta+50][PHI][1].isset){
	       sum+=hb[eta+50][PHI][1].energy;n++;
	       sumt+=hb[eta+50][PHI][1].time;nt++;
	    }
	  }
	  if(n>0){*ave_e=sum/n;*ave_t=sumt/nt; return true;} 
      }  
      if(strcmp(SUBDET,"HBM")==0){
          double sum=0,n=0,sumt=0,nt=0;
	  for(int eta=-14;eta<=-1;eta++){
            if(hb[eta+50][PHI][1].isset){
	       sum+=hb[eta+50][PHI][1].energy;n++;
	       sumt+=hb[eta+50][PHI][1].time;nt++;
	    
	    }
	  }
	  if(n>0){*ave_e=sum/n; *ave_t=sumt/nt; return true;} 
      }  
      if(strcmp(SUBDET,"HEP")==0){
          double sum=0,n=0,sumt=0,nt=0;
	  for(int eta=16;eta<=20;eta++) for(int d=1;d<=3;d++){
            if(he[eta+50][PHI][d].isset){
	       sum+=he[eta+50][PHI][d].energy;n++;
	       sumt+=he[eta+50][PHI][d].time;nt++;
	    }
	  }
	  if(n>0){*ave_e=sum/n;*ave_t=sumt/nt; return true;} 
      }  
      if(strcmp(SUBDET,"HEM")==0){
          double sum=0,n=0,sumt=0,nt=0;
	  for(int eta=-20;eta<=-16;eta++) for(int d=1;d<=3;d++){
            if(he[eta+50][PHI][d].isset){
	       sum+=he[eta+50][PHI][d].energy;n++;
	       sumt+=he[eta+50][PHI][d].time;nt++;
	    }
	  }
	  if(n>0){*ave_e=sum/n; *ave_t=sumt/nt; return true;} 
      }  
      return false;
   }
private:
   leddata hb[100][73][5];
   leddata he[100][73][5];
   leddata ho[100][73][5];
   leddata hf[100][73][5];
   leddata calib_data[5][5][73];
   int RUN;
 };

LED_RUN *leds[1000];
int     NumOfRuns;
int RUN_MIN=1000000;
int RUN_MAX=0;

int get_run_index(int run){
int ind;  
  for(ind=0;ind<NumOfRuns;ind++) if(leds[ind]->get_run_number()==run) break;
  return ind;
}
void LoadLEDData(char *filename="FileNames"){
  char   str[100]; 
  int t;
  gDirectory->DeleteAll(); 
  //////////////////////////////////////////////////////////////////////
  NumOfRuns=0;
  FILE *f=fopen(filename,"rt");
  for(;;){
    fscanf(f,"%s",str);
    if(feof(f)) break;
    leds[NumOfRuns]=new LED_RUN(str);
    NumOfRuns++; 
  }
  fclose(f);  
  for(int i=0;i<NumOfRuns;i++){
     if(leds[i]->get_run_number()>RUN_MAX) RUN_MAX=leds[i]->get_run_number();
     if(leds[i]->get_run_number()<RUN_MIN) RUN_MIN=leds[i]->get_run_number();
   }
}
