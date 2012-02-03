class PEDESTALS{
public:
   PEDESTALS(){
     RUN=-1;
   }
   PEDESTALS(char *filename){
      double ped[4],rms[4],VAL;
      int Eta,Phi,Depth,run,Statistic,Status;
      char subdet[10];
        for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++)
             shb[i][j][k]=she[i][j][k]=sho[i][j][k]=shf[i][j][k]=0; 
        for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++)for(int l=0;l<5;l++)
             hb[i][j][k][l]=he[i][j][k][l]=ho[i][j][k][l]=hf[i][j][k][l]=-1; 
        for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++)for(int l=0;l<5;l++)
             hb_rms[i][j][k][l]=he_rms[i][j][k][l]=ho_rms[i][j][k][l]=hf_rms[i][j][k][l]=-1; 
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
	  VAL=(ped[0]+ped[1]+ped[2]+ped[3])/4.0;
          for(int i=0;i<4;i++){
            if(strcmp(subdet,"HB")==0){ 
              hb[Eta+50][Phi][Depth][i]=ped[i];hb_rms[Eta+50][Phi][Depth][i]=rms[i]; shb[Eta+50][Phi][Depth]=Statistic; }
            if(strcmp(subdet,"HE")==0){ 
              he[Eta+50][Phi][Depth][i]=ped[i];he_rms[Eta+50][Phi][Depth][i]=rms[i]; she[Eta+50][Phi][Depth]=Statistic; }
            if(strcmp(subdet,"HO")==0){  
              ho[Eta+50][Phi][Depth][i]=ped[i];ho_rms[Eta+50][Phi][Depth][i]=rms[i]; sho[Eta+50][Phi][Depth]=Statistic; }
            if(strcmp(subdet,"HF")==0){ 
              hf[Eta+50][Phi][Depth][i]=ped[i];hf_rms[Eta+50][Phi][Depth][i]=rms[i]; shf[Eta+50][Phi][Depth]=Statistic; }	   
          }
        }
        f->Close(); 
	printf("run %i: pedestal data loaded...\n",RUN);  
   }
   int  get_run_number(){ return RUN; }
   bool get_ped_value(char *SUBDET,int ETA,int PHI,int DEPTH,float *VAL){
        for(int i=0;i<4;i++){
          if(strcmp(SUBDET,"HB")==0) VAL[i]=hb[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HE")==0) VAL[i]=he[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HO")==0) VAL[i]=ho[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HF")==0) VAL[i]=hf[ETA+50][PHI][DEPTH][i];
        }  
	return true;
   }
   bool get_rms_value(char *SUBDET,int ETA,int PHI,int DEPTH,float *VAL){
        for(int i=0;i<4;i++){
          if(strcmp(SUBDET,"HB")==0) VAL[i]=hb_rms[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HE")==0) VAL[i]=he_rms[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HO")==0) VAL[i]=ho_rms[ETA+50][PHI][DEPTH][i];
          if(strcmp(SUBDET,"HF")==0) VAL[i]=hf_rms[ETA+50][PHI][DEPTH][i];
        }  
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
private:
   float hb[100][73][5][4];
   float he[100][73][5][4];
   float ho[100][73][5][4];
   float hf[100][73][5][4];
   float hb_rms[100][73][5][4];
   float he_rms[100][73][5][4];
   float ho_rms[100][73][5][4];
   float hf_rms[100][73][5][4];
   int shb[100][73][5];
   int she[100][73][5];
   int sho[100][73][5];
   int shf[100][73][5];
   int RUN;
};

void PreparePedestalTable(char *DetDiagFile,char *TableFile,float TH=0.2){
char str[500],sd[10],id[20];
int eta,phi,depth;
float PED[4],RMS[4],PEDdd[4],RMSdd[4];
   TH1F *hb=new TH1F("diff hb","diff hb",1500,-3,3);
   TH1F *he=new TH1F("diff he","diff he",1500,-3,3);
   TH1F *ho=new TH1F("diff ho","diff ho",1500,-3,3);
   TH1F *hf=new TH1F("diff hf","diff hf",1500,-3,3);

   PEDESTALS peds(DetDiagFile);
   FILE *tab=fopen(TableFile,"rt");
   sprintf(str,"new_pedestals_run_%i.txt",peds.get_run_number());
   FILE *out=fopen(str,"wt");
   fgets(str,500,tab); fprintf(out,"%s",str);
   fgets(str,500,tab); fprintf(out,"%s",str);
   for(;;){
       fgets(str,500,tab); 
       sscanf(str,"%d %d %d",&eta,&phi,&depth); 
       sscanf(&str[54],"%s %f %f %f %f %f %f %f %f %s",sd,&PED[0],&PED[1],&PED[2],&PED[3],&RMS[0],&RMS[1],&RMS[2],&RMS[3],&id);
       if(feof(tab)) break;
       if(strcmp(sd,"HB")==0 || strcmp(sd,"HE")==0 || strcmp(sd,"HO")==0 || strcmp(sd,"HF")==0){
          int stat;  peds.get_ped_stat(sd,eta,phi,depth,&stat); 
	  
	  if(stat>100){  // ignore missing/bad channel
             peds.get_ped_value(sd,eta,phi,depth,PEDdd); 
             peds.get_rms_value(sd,eta,phi,depth,RMSdd); 
             double VALdd=(PEDdd[0]+PEDdd[1]+PEDdd[2]+PEDdd[3])/4.0; 
             double VALtab=(PED[0]+PED[1]+PED[2]+PED[3])/4.0;
             double diff=VALdd-VALtab;
             if(strcmp(sd,"HB")==0)hb->Fill(VALdd-VALtab);
             if(strcmp(sd,"HE")==0)he->Fill(VALdd-VALtab);
             if(strcmp(sd,"HO")==0)ho->Fill(VALdd-VALtab);
             if(strcmp(sd,"HF")==0)hf->Fill(VALdd-VALtab);
             if(fabs(diff)>TH){
                sprintf(str,"  %15d %15d %15d %15s %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f %8.5f   %s\n",
                   eta,phi,depth,sd,PEDdd[0],PEDdd[1],PEDdd[2],PEDdd[3],RMSdd[0],RMSdd[1],RMSdd[2],RMSdd[3],id);
		   printf("%s\n",str);
	     }
          }
       }
       fprintf(out,"%s",str);
   }
   fclose(tab);fclose(out);
   TCanvas *c=new TCanvas("c","c");
   c->Divide(2,2,0.00001,0.000001);
   c->cd(1); c->cd(1)->SetLogy(); hb->Draw();
   c->cd(2); c->cd(2)->SetLogy(); he->Draw();
   c->cd(3); c->cd(3)->SetLogy(); ho->Draw();
   c->cd(4); c->cd(4)->SetLogy(); hf->Draw();
}


