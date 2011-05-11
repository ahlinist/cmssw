class laserdata{
public:
  laserdata(){};
  bool   isset;
  float  energy;
  float  e_rms;
  float  time;
  float  t_rms;
};


class LASER_DATA{
public:
   LASER_DATA(){
        for(int i=0;i<33;i++)for(int j=0;j<72;j++)for(int k=0;k<2;k++) hb[i][j][k].isset=false; 
        for(int i=0;i<61;i++)for(int j=0;j<72;j++)for(int k=0;k<3;k++) he[i][j][k].isset=false; 
        for(int i=0;i<31;i++)for(int j=0;j<72;j++) ho[i][j].isset=false; 
        for(int i=0;i<85;i++)for(int j=0;j<72;j++)for(int k=0;k<2;k++) hf[i][j][k].isset=false; 

	ave_t_hb=ave_t_he=ave_t_ho=ave_t_hf=-1000;
   }
   LASER_DATA(char *filename){
      double amp,rms,time,rms_time;
      int Eta,Phi,Depth,rbx,chan,run;
      bool isHFP=false,isHFM=false,isHBP=false,isHBM=false,isHEP=false,isHEM=false,isHO=false;
      char subdet[10];
        for(int i=0;i<33;i++)for(int j=0;j<72;j++)for(int k=0;k<2;k++) hb[i][j][k].isset=false; 
        for(int i=0;i<61;i++)for(int j=0;j<72;j++)for(int k=0;k<3;k++) he[i][j][k].isset=false; 
        for(int i=0;i<31;i++)for(int j=0;j<72;j++) ho[i][j].isset=false; 
        for(int i=0;i<85;i++)for(int j=0;j<72;j++)for(int k=0;k<2;k++) hf[i][j][k].isset=false; 

	ave_t_hb=ave_t_he=ave_t_ho=ave_t_hf=-1;
	ave_e_hb=ave_e_he=ave_e_ho=ave_e_hf=-1;
	
	TFile* f = new TFile(filename,"READ");
      	TObjString *STR=(TObjString *)f->Get("run number");
      	sscanf(STR->String().Data(),"%i",&run); RUN=run;
      	if(!f->IsOpen()){ printf("Enable to load laser data...\n"); return; }
      	
        TTree*  t=(TTree*)f->Get("HCAL Laser data");
        if(!t) return false;
        t->SetBranchAddress("Subdet",    subdet);
        t->SetBranchAddress("eta",       &Eta);
        t->SetBranchAddress("phi",       &Phi);
        t->SetBranchAddress("depth",     &Depth);
        t->SetBranchAddress("amp",       &amp);
        t->SetBranchAddress("rms",       &rms);
        t->SetBranchAddress("time",      &time);
        t->SetBranchAddress("time_rms",  &rms_time);
    
	double thb=0,ehb=0,nhb=0, the=0,ehe=0,nhe=0, tho=0,eho=0,nho=0, thf=0,ehf=0,nhf=0;
        for(int ievt=0;ievt<t->GetEntries();ievt++){
           t->GetEntry(ievt);
           if(strcmp(subdet,"HB")==0){
                if(amp<10) continue;
	        hb[Eta+16][Phi-1][Depth-1].isset=true;
	        hb[Eta+16][Phi-1][Depth-1].energy=amp;
	        hb[Eta+16][Phi-1][Depth-1].e_rms=rms;
	        hb[Eta+16][Phi-1][Depth-1].time=time;
	        hb[Eta+16][Phi-1][Depth-1].t_rms=rms_time;   
		if(Eta>0) isHBP=true;         
		if(Eta<0) isHBM=true; 
		thb+=time;
		ehb+=amp;
		nhb++;        
	   }
	   if(strcmp(subdet,"HE")==0){
                if(amp<10) continue;
	        he[Eta+30][Phi-1][Depth-1].isset=true;
	        he[Eta+30][Phi-1][Depth-1].energy=amp;
	        he[Eta+30][Phi-1][Depth-1].e_rms=rms;
	        he[Eta+30][Phi-1][Depth-1].time=time;
	        he[Eta+30][Phi-1][Depth-1].t_rms=rms_time;   
		if(Eta>0) isHEP=true;         
		if(Eta<0) isHEM=true;         
		the+=time;
		ehe+=amp;
		nhe++;        
	   }
	   if(strcmp(subdet,"HO")==0){
                if(amp<10) continue;
	        ho[Eta+15][Phi-1].isset=true;
	        ho[Eta+15][Phi-1].energy=amp;
	        ho[Eta+15][Phi-1].e_rms=rms;
	        ho[Eta+15][Phi-1].time=time;
	        ho[Eta+15][Phi-1].t_rms=rms_time; 
		isHO=true;   
                if(abs(Eta)<=4){
		   tho+=time;
		   eho+=amp;
		   nho++;
                }
	   } 
           if(strcmp(subdet,"HF")==0){
                if(amp<10) continue;
	        hf[Eta+42][Phi-1][Depth-1].isset=true;
	        hf[Eta+42][Phi-1][Depth-1].energy=amp;
	        hf[Eta+42][Phi-1][Depth-1].e_rms=rms;
	        hf[Eta+42][Phi-1][Depth-1].time=time;
	        hf[Eta+42][Phi-1][Depth-1].t_rms=rms_time; 
		if(Eta>0) isHFP=true;         
		if(Eta<0) isHFM=true;         
		thf+=time;
		ehf+=amp;
		nhf++;        
	   }
	}
        f->Close(); 
	if(nhb>1000.0){ave_t_hb=thb/nhb;ave_e_hb=ehb/nhb;}
	if(nhe>1000.0){ave_t_he=the/nhe;ave_e_he=ehe/nhe;}
	if(nho>100.0) {ave_t_ho=tho/nho;ave_e_ho=eho/nho;}
	if(nhf>1000.0){ave_t_hf=thf/nhf;ave_e_hf=ehf/nhf;}
	printf("run %i: laser data loaded... ",RUN);
	if(isHBM) printf(" HBM");  
	if(isHBP) printf(" HBP");  
	if(isHEM) printf(" HEM");  
	if(isHEP) printf(" HEP");  
	if(isHFM) printf(" HFM");  
	if(isHFP) printf(" HFP");  
	if(isHO)  printf(" HO"); 
	printf("\n"); 
   }
   int  get_run_number(){ return RUN; }
   bool get_laser_value(char *SUBDET,int ETA,int PHI,int DEPTH,laserdata **VAL){
           *VAL=-1;
          if(strcmp(SUBDET,"HB")==0 && abs(ETA)<17 && DEPTH<=2) *VAL=&hb[ETA+16][PHI-1][DEPTH-1];
          if(strcmp(SUBDET,"HE")==0 && abs(ETA)<30 && DEPTH<=3) *VAL=&he[ETA+30][PHI-1][DEPTH-1];
          if(strcmp(SUBDET,"HO")==0 && abs(ETA)<15 && DEPTH==4) *VAL=&ho[ETA+15][PHI-1];
          if(strcmp(SUBDET,"HF")==0 && abs(ETA)<42 && DEPTH<=2) *VAL=&hf[ETA+42][PHI-1][DEPTH-1];
	  if(*VAL==-1) return false;
	  if(((laserdata *)*VAL)->isset==false) return false;
	  return true;
   }
   bool set_laser_value(char *SUBDET,int ETA,int PHI,int DEPTH,double energy,double time){
          if(strcmp(SUBDET,"HB")==0){ hb[ETA+16][PHI-1][DEPTH-1].energy=energy; hb[ETA+16][PHI-1][DEPTH-1].time=time;
                                      hb[ETA+16][PHI-1][DEPTH-1].isset=true;}
          if(strcmp(SUBDET,"HE")==0){ he[ETA+30][PHI-1][DEPTH-1].energy=energy; he[ETA+30][PHI-1][DEPTH-1].time=time; 
                                      he[ETA+30][PHI-1][DEPTH-1].isset=true;}
          if(strcmp(SUBDET,"HO")==0){ ho[ETA+15][PHI-1].energy=energy; ho[ETA+15][PHI-1].time=time; 
                                      ho[ETA+15][PHI-1].isset=true;}
          if(strcmp(SUBDET,"HF")==0){ hf[ETA+42][PHI-1][DEPTH-1].energy=energy; hf[ETA+42][PHI-1][DEPTH-1].time=time; 
                                      hf[ETA+42][PHI-1][DEPTH-1].isset=true;}
	  return true;
   }
  bool get_ave_time_value(char *SUBDET,double *VAL){
          if(strcmp(SUBDET,"HB")==0) *VAL=ave_t_hb;
          if(strcmp(SUBDET,"HE")==0) *VAL=ave_t_he;
          if(strcmp(SUBDET,"HO")==0) *VAL=ave_t_ho;
          if(strcmp(SUBDET,"HF")==0) *VAL=ave_t_hf;
	  if(*VAL<0) return false;
	  return true;
   }   
  bool get_ave_amp_value(char *SUBDET,double *VAL){
          if(strcmp(SUBDET,"HB")==0) *VAL=ave_e_hb;
          if(strcmp(SUBDET,"HE")==0) *VAL=ave_e_he;
          if(strcmp(SUBDET,"HO")==0) *VAL=ave_e_ho;
          if(strcmp(SUBDET,"HF")==0) *VAL=ave_e_hf;
	  if(*VAL<0) return false;
	  return true;
   }   
private:
   laserdata hb[33][72][2];
   laserdata he[61][72][3];
   laserdata ho[31][72];
   laserdata hf[85][72][2];
   double ave_t_hb;
   double ave_t_he;
   double ave_t_ho;
   double ave_t_hf;
   double ave_e_hb;
   double ave_e_he;
   double ave_e_ho;
   double ave_e_hf;
   
   int RUN;
};

LASER_DATA *DATA[10000];
int     NumOfRuns;
int     RUN_MIN=1000000;
int     RUN_MAX=0;

int get_run_index(int run){
int ind;  
  for(ind=0;ind<NumOfRuns;ind++) if(DATA[ind]->get_run_number()==run) break;
  return ind;
}
void LoadLaserData(char *ListOfDatasets=""){
char   str[500]; 
  if(strlen(ListOfDatasets)==0){
     printf("Usage: .x LoadLaserData.C(ListOfDatasets_Filename)\n");
     return;
  }
  gDirectory->DeleteAll(); 
  //////////////////////////////////////////////////////////////////////
  NumOfRuns=0;
  FILE *f=fopen(ListOfDatasets,"rt");
  for(;;){
    fscanf(f,"%s",str);
    if(feof(f)) break;
    DATA[NumOfRuns]=new LASER_DATA(str);
    NumOfRuns++;
  }
  fclose(f);  
  for(int i=0;i<NumOfRuns;i++){
     if(DATA[i]->get_run_number()>RUN_MAX) RUN_MAX=DATA[i]->get_run_number();
     if(DATA[i]->get_run_number()<RUN_MIN) RUN_MIN=DATA[i]->get_run_number();
  }
}
