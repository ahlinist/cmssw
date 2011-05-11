class PEDESTALS{
public:
   PEDESTALS(){
     for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++) hb[i][j][k]=he[i][j][k]=ho[i][j][k]=hf[i][j][k]=-1; 
     for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++) shb[i][j][k]=she[i][j][k]=sho[i][j][k]=shf[i][j][k]=0; 
     RUN=-1;
   }
   PEDESTALS(char *filename){
      double ped[4],rms[4],VAL;
      int Eta,Phi,Depth,run,Statistic,Status,stat=0;
      char subdet[10];
        for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++) hb[i][j][k]=he[i][j][k]=ho[i][j][k]=hf[i][j][k]=-1; 
   	TFile* f = new TFile(filename,"READ");
      	TObjString *STR=(TObjString *)f->Get("run number");
      	sscanf(STR->String().Data(),"%i",&run); RUN=run;
	sscanf(&filename[27],"%d",&run); RUN=run;
      	if(!f->IsOpen()){ printf("Enable to load reference pedestal data...\n"); return; }
      	TTree*  t=(TTree*)f->Get("HCAL Pedestal data");
      	if(!t) return;
      	t->SetBranchAddress("Subdet",   subdet);
      	t->SetBranchAddress("eta",      &Eta);
      	t->SetBranchAddress("phi",      &Phi);
      	t->SetBranchAddress("depth",    &Depth);
      	t->SetBranchAddress("cap0_ped", &ped[0]);
      	t->SetBranchAddress("cap0_rms", &rms[0]);
      	t->SetBranchAddress("cap1_ped", &ped[1]);
      	t->SetBranchAddress("cap1_rms", &rms[1]);
      	t->SetBranchAddress("cap2_ped", &ped[2]);
      	t->SetBranchAddress("cap2_rms", &rms[2]);
      	t->SetBranchAddress("cap3_ped", &ped[3]);
      	t->SetBranchAddress("cap3_rms", &rms[3]);
	t->SetBranchAddress("statistic",&Statistic);
        t->SetBranchAddress("status",   &Status);

      	int ievt=0;
      	for(ievt=0;ievt<t->GetEntries();ievt++){
          t->GetEntry(ievt);
	  if(Statistic>stat) stat=Statistic; 
	  VAL=(ped[0]+ped[1]+ped[2]+ped[3])/4.0;
          if(strcmp(subdet,"HB")==0){ hb[Eta+50][Phi][Depth]=VAL; shb[Eta+50][Phi][Depth]=Statistic; }
          if(strcmp(subdet,"HE")==0){ he[Eta+50][Phi][Depth]=VAL; she[Eta+50][Phi][Depth]=Statistic; }
          if(strcmp(subdet,"HO")==0){ ho[Eta+50][Phi][Depth]=VAL; sho[Eta+50][Phi][Depth]=Statistic; }
          if(strcmp(subdet,"HF")==0){ hf[Eta+50][Phi][Depth]=VAL; shf[Eta+50][Phi][Depth]=Statistic; }
	   
        }
        f->Close(); 
	printf("run %i: pedestal data loaded...(stat: %i)\n",RUN,stat);  
   }
   int  get_run_number(){ return RUN; }
   bool get_ped_value(char *SUBDET,int ETA,int PHI,int DEPTH,double *VAL){
          int stat;
	  if(!get_ped_stat(SUBDET,ETA,PHI,DEPTH,&stat)) return false;
	  if(stat<500) return false;
          if(strcmp(SUBDET,"HB")==0) *VAL=hb[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HE")==0) *VAL=he[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HO")==0) *VAL=ho[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HF")==0) *VAL=hf[ETA+50][PHI][DEPTH];
          if(*VAL==-1) return false;
	  return true;
   }
   bool get_ped_stat(char *SUBDET,int ETA,int PHI,int DEPTH,int *VAL){
          if(strcmp(SUBDET,"HB")==0) *VAL=shb[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HE")==0) *VAL=she[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HO")==0) *VAL=sho[ETA+50][PHI][DEPTH];
          if(strcmp(SUBDET,"HF")==0) *VAL=shf[ETA+50][PHI][DEPTH];
          if(*VAL==-1) return false;
	  return true;
   }
   bool set_ped_value(char *SUBDET,int ETA,int PHI,int DEPTH,double VAL,int stat){
          if(strcmp(SUBDET,"HB")==0){ hb[ETA+50][PHI][DEPTH]=VAL; shb[ETA+50][PHI][DEPTH]=stat;}
          if(strcmp(SUBDET,"HE")==0){ he[ETA+50][PHI][DEPTH]=VAL; she[ETA+50][PHI][DEPTH]=stat;}
          if(strcmp(SUBDET,"HO")==0){ ho[ETA+50][PHI][DEPTH]=VAL; sho[ETA+50][PHI][DEPTH]=stat;}
          if(strcmp(SUBDET,"HF")==0){ hf[ETA+50][PHI][DEPTH]=VAL; shf[ETA+50][PHI][DEPTH]=stat;}
	  return true;
   }
private:
   float hb[100][73][5];
   float he[100][73][5];
   float ho[100][73][5];
   float hf[100][73][5];
   int shb[100][73][5];
   int she[100][73][5];
   int sho[100][73][5];
   int shf[100][73][5];
   int RUN;
};
int get_run_index(int run){
int ind;  
  for(ind=0;ind<NumOfRuns;ind++) if(peds[ind]->get_run_number()==run) break;
  return ind;
}

PEDESTALS *peds[10000];
int       NumOfRuns;


LoadPedestalData(char *ListOfDatasets=""){
char str[500]; 
  if(strlen(ListOfDatasets)==0){
     printf("Usage: .x LoadPedestalData.C(ListOfDatasets_Filename)\n");
     return;
  }
  //////////////////////////////////////////////////////////////////////
  NumOfRuns=0;
  FILE *f=fopen(ListOfDatasets,"rt");
  for(;;){
    fscanf(f,"%s",str);
    if(feof(f)) break;
    peds[NumOfRuns]=new PEDESTALS(str);
    NumOfRuns++;
  }
}
