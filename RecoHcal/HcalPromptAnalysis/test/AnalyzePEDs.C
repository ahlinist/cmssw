#include <stdio.h>
class summary{
public:
   summary(){
   for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++) hb[i][j][k]=he[i][j][k]=ho[i][j][k]=hf[i][j][k]=0;
   for(int i=0;i<100;i++)for(int j=0;j<73;j++)for(int k=0;k<5;k++) nhb[i][j][k]=nhe[i][j][k]=nho[i][j][k]=nhf[i][j][k]=0;
   
   }
   int add(char *SUBDET,int ETA,int PHI,int DEPTH,double VAL){
         if(strcmp(SUBDET,"HB")==0){ hb[ETA+50][PHI][DEPTH]+=VAL; nhb[ETA+50][PHI][DEPTH]++; }
         if(strcmp(SUBDET,"HE")==0){ he[ETA+50][PHI][DEPTH]+=VAL; nhe[ETA+50][PHI][DEPTH]++; }
         if(strcmp(SUBDET,"HO")==0){ ho[ETA+50][PHI][DEPTH]+=VAL; nho[ETA+50][PHI][DEPTH]++; }
         if(strcmp(SUBDET,"HF")==0){ hf[ETA+50][PHI][DEPTH]+=VAL; nhf[ETA+50][PHI][DEPTH]++; }
   } 
   bool get_mean(char *SUBDET,int ETA,int PHI,int DEPTH,double *VAL){
      double ret=-1;
      if(strcmp(SUBDET,"HB")==0){  if(nhb[ETA+50][PHI][DEPTH]>0) ret=hb[ETA+50][PHI][DEPTH]/nhb[ETA+50][PHI][DEPTH]; }
      if(strcmp(SUBDET,"HE")==0){  if(nhe[ETA+50][PHI][DEPTH]>0) ret=he[ETA+50][PHI][DEPTH]/nhe[ETA+50][PHI][DEPTH]; }
      if(strcmp(SUBDET,"HO")==0){  if(nho[ETA+50][PHI][DEPTH]>0) ret=ho[ETA+50][PHI][DEPTH]/nho[ETA+50][PHI][DEPTH]; }
      if(strcmp(SUBDET,"HF")==0){  if(nhf[ETA+50][PHI][DEPTH]>0) ret=hf[ETA+50][PHI][DEPTH]/nhf[ETA+50][PHI][DEPTH]; } 
      if(ret>0) { *VAL=ret; return true;}
      return false;
   }
private: 
   float hb[100][73][5];
   float he[100][73][5];
   float ho[100][73][5];
   float hf[100][73][5];
   float nhb[100][73][5];
   float nhe[100][73][5];
   float nho[100][73][5];
   float nhf[100][73][5];
};

summary SUM;
PEDESTALS PED_REF;

void processIOVs(int REF,double TH=0.2){
int run0=get_run_index(REF);
   // fill initial values (from reference run)
   for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=4;depth++){
     double ped; int stat; 
     if(peds[run0]->get_ped_value("HB",eta,phi,depth,&ped) && peds[run0]->get_ped_stat("HB",eta,phi,depth,&stat)) PED_REF.set_ped_value("HB",eta,phi,depth,ped,stat);
     if(peds[run0]->get_ped_value("HE",eta,phi,depth,&ped) && peds[run0]->get_ped_stat("HE",eta,phi,depth,&stat)) PED_REF.set_ped_value("HE",eta,phi,depth,ped,stat);
     if(peds[run0]->get_ped_value("HO",eta,phi,depth,&ped) && peds[run0]->get_ped_stat("HO",eta,phi,depth,&stat)) PED_REF.set_ped_value("HO",eta,phi,depth,ped,stat);
     if(peds[run0]->get_ped_value("HF",eta,phi,depth,&ped) && peds[run0]->get_ped_stat("HF",eta,phi,depth,&stat)) PED_REF.set_ped_value("HF",eta,phi,depth,ped,stat);
   }
   
   FILE * pFile;
   pFile = fopen("PEDsummary_mean.txt","w");

   printf("====== IOVs ======\n");
   fprintf(pFile,"====== IOVs ======\n");



   for(int i=1;i<NumOfRuns;i++){
      int run1=peds[i]->get_run_number();
      printf("run %i:\n",run1);
      fprintf(pFile,"run %i:\n",run1);

      for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=4;depth++){
         double ped1=0,ped0=0; int stat=0;
         if(peds[i]->get_ped_value("HB",eta,phi,depth,&ped1) && peds[i]->get_ped_stat("HB",eta,phi,depth,&stat)){
	    PED_REF.get_ped_value("HB",eta,phi,depth,&ped0);
	    if(ped0<0){ PED_REF.set_ped_value("HB",eta,phi,depth,ped1,stat); ped0=ped1;
	        printf("new HB (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);

                fprintf(pFile,"new HB (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);
	    }
            double dped=ped1-ped0; if(dped<0) dped=-dped;
	    if(dped>TH){
	        printf("HB (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);
	        PED_REF.set_ped_value("HB",eta,phi,depth,ped1,stat); // modify PED_REF
	    } 
	 }
         if(peds[i]->get_ped_value("HE",eta,phi,depth,&ped1) && peds[i]->get_ped_stat("HE",eta,phi,depth,&stat)){
	    PED_REF.get_ped_value("HE",eta,phi,depth,&ped0);
	    if(ped0<0){ PED_REF.set_ped_value("HE",eta,phi,depth,ped1,stat); ped0=ped1; 
	        printf("new HE (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0); 

                fprintf(pFile,"new HE (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);
	    }
	    double dped=ped1-ped0; if(dped<0) dped=-dped;
	    if(dped>TH){
	        printf("HE (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);

                fprintf(pFile,"HE (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);

	        PED_REF.set_ped_value("HE",eta,phi,depth,ped1,stat); // modify PED_REF

	    }	 
	 }
         if(peds[i]->get_ped_value("HO",eta,phi,depth,&ped1) && peds[i]->get_ped_stat("HO",eta,phi,depth,&stat)){
	    PED_REF.get_ped_value("HO",eta,phi,depth,&ped0);
	    if(ped0<0){ PED_REF.set_ped_value("HO",eta,phi,depth,ped1,stat); ped0=ped1; 
	        printf("new HO (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);
                fprintf(pFile,"new HO (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);

	    }
	    double dped=ped1-ped0; if(dped<0) dped=-dped;
	    if(dped>TH){
	        printf("HO (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);
                fprintf(pFile,"HO (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);


	        PED_REF.set_ped_value("HO",eta,phi,depth,ped1,stat); // modify PED_REF
	    }
	 }
         if(peds[i]->get_ped_value("HF",eta,phi,depth,&ped1) && peds[i]->get_ped_stat("HF",eta,phi,depth,&stat)){
	    PED_REF.get_ped_value("HF",eta,phi,depth,&ped0);
	    if(ped0<0){ PED_REF.set_ped_value("HF",eta,phi,depth,ped1,stat); ped0=ped1; 
	        printf("new HF (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);
                fprintf(pFile,"new HF (%i,%i,%i):  \t%.3f\n",eta,phi,depth,ped0);


	    }
	    double dped=ped1-ped0; if(dped<0) dped=-dped;
	    if(dped>TH){
	        printf("HF (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);
                fprintf(pFile,"HF (%i,%i,%i):\t%.3f\t->\t%.3f\t(%.2f)\n",eta,phi,depth,ped0,ped1,dped);


	        PED_REF.set_ped_value("HF",eta,phi,depth,ped1,stat); // modify PED_REF
	    }
	 }
      }
      run0=run1;
   }
   printf("Writing IOV information to disk");
   fclose(pFile);
}

void AnalyzePEDs(float TH_Run_to_Run=0.02,float TH_IOV=0.2){
int    run,run_min=9999999,run_max=0;
double val;
int    REF;
char   str[100]; 
  gDirectory->DeleteAll(); 
  FILE * pFile1;
  pFile1 = fopen("PEDsummary_RMS.txt","w");

  printf("Usage: .x AnalyzePEDs.C(TH_Run_to_Run,TH_IOV)\n");
  fprintf(pFile1,"Usage: .x AnalyzePEDs.C(TH_Run_to_Run,TH_IOV)\n");


  //////////////////////////////////////////////////////////////////////
  REF=peds[0]->get_run_number();
  //////////////////////////////////////////////////////////////////////
  TH1F *hHB=new TH1F("HB Ped - Ref","HB Ped - Ref",10000,-1,1);
  TH1F *hHE=new TH1F("HE Ped - Ref","HE Ped - Ref",10000,-1,1);
  TH1F *hHO=new TH1F("HO Ped - Ref","HO Ped - Ref",10000,-1,1);
  TH1F *hHF=new TH1F("HF Ped - Ref","HF Ped - Ref",10000,-1,1);

  int IndRef=get_run_index(REF);
  int IndRun;
  for(int IndRun=0;IndRun<NumOfRuns;IndRun++){ 
          hHB->Reset();
          hHE->Reset();
          hHO->Reset();
          hHF->Reset();
	  for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=4;depth++){
	     double ped,ref;
	     if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped)) SUM.add("HB",eta,phi,depth,ped); 
	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped)) SUM.add("HE",eta,phi,depth,ped); 
	     if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped)) SUM.add("HO",eta,phi,depth,ped); 
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped)) SUM.add("HF",eta,phi,depth,ped); 
	     
	     if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HB",eta,phi,depth,&ref)){
	         hHB->Fill(ped-ref);
	     }
	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HE",eta,phi,depth,&ref)){
	         hHE->Fill(ped-ref);
	     }
	     if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HO",eta,phi,depth,&ref)){
	         hHO->Fill(ped-ref);
	     }
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HF",eta,phi,depth,&ref)){
	         hHF->Fill(ped-ref);
	     }
	  }
  }
   
  hHB->Reset();
  hHE->Reset();
  hHO->Reset();
  hHF->Reset();
  
  for(int IndRun=0;IndRun<NumOfRuns;IndRun++){ 
          double ped,mean;
	  printf("look at run: %i\n",peds[IndRun]->get_run_number());
          fprintf(pFile1,"look at run: %i\n",peds[IndRun]->get_run_number());
 	  for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=4;depth++){
             if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped) && SUM.get_mean("HB",eta,phi,depth,&mean)){
	         hHB->Fill(ped-mean);
	     }
	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped) && SUM.get_mean("HE",eta,phi,depth,&mean)){
	         hHE->Fill(ped-mean);
	     }         
             if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped) && SUM.get_mean("HO",eta,phi,depth,&mean)){
	         hHO->Fill(ped-mean);
	     }
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped) && SUM.get_mean("HF",eta,phi,depth,&mean)){
	         hHF->Fill(ped-mean);
	     }         	  
	  } 
  }
  gStyle->SetOptStat(111111);
 
  TCanvas *c1=new TCanvas("c1","c1"); 
  c1->SetFillColor(0);
  c1->SetWindowPosition(12,30);
  c1->SetWindowSize(475,475);
 
  c1->Divide(2,2,0.0001,0.0001);
  c1->cd(1); c1_1->SetLogy(1);hHB->Draw();
  c1->cd(2); c1_2->SetLogy(1);hHE->Draw();
  c1->cd(3); c1_3->SetLogy(1);hHO->Draw();
  c1->cd(4); c1_4->SetLogy(1);hHF->Draw();
  c1->SaveAs("PedShift.jpg");
   
  TH1F *hHBrms=new TH1F("HB RMS","HB RMS",1000,-0.001,0.15);
  TH1F *hHErms=new TH1F("HE RMS","HE RMS",1000,-0.001,0.15);
  TH1F *hHOrms=new TH1F("HO RMS","HO RMS",1000,-0.001,0.15);
  TH1F *hHFrms=new TH1F("HF RMS","HF RMS",1000,-0.001,0.15);
  TH1F *tmp=new TH1F("tmp","tmp",10000,0,25); 

  printf("List of channels with run to run RMS > %f\n",TH_Run_to_Run);
  fprintf(pFile1,"List of channels with run to run RMS > %f\n",TH_Run_to_Run);

  for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=4;depth++){
        double ped;
	tmp->Reset();
        for(int IndRun=0;IndRun<NumOfRuns;IndRun++){           
	     if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped)) tmp->Fill(ped);	 
	}
	if(tmp->GetEntries()>0){
	   hHBrms->Fill(tmp->GetRMS());
	   if(tmp->GetRMS()>TH_Run_to_Run) printf("HB: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());
           if(tmp->GetRMS()>TH_Run_to_Run) fprintf(pFile1,"HB: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());

	}
        tmp->Reset();
	for(int IndRun=0;IndRun<NumOfRuns;IndRun++){           
	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped)) tmp->Fill(ped);
	}
	if(tmp->GetEntries()>0){
	   hHErms->Fill(tmp->GetRMS());
	   if(tmp->GetRMS()>TH_Run_to_Run) printf("HE: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());
           if(tmp->GetRMS()>TH_Run_to_Run) fprintf(pFile1,"HE: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());

	}   
	tmp->Reset();
        for(int IndRun=0;IndRun<NumOfRuns;IndRun++){           
	     if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped)) tmp->Fill(ped);	 
	}
	if(tmp->GetEntries()>0){
	   hHOrms->Fill(tmp->GetRMS());
	   if(tmp->GetRMS()>TH_Run_to_Run) printf("HO: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());
           if(tmp->GetRMS()>TH_Run_to_Run) fprintf(pFile1,"HO: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());

	}
        tmp->Reset();
	for(int IndRun=0;IndRun<NumOfRuns;IndRun++){           
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped)) tmp->Fill(ped);
	}
	if(tmp->GetEntries()>0){
	   hHFrms->Fill(tmp->GetRMS());
	   if(tmp->GetRMS()>TH_Run_to_Run) printf("HF: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());
           if(tmp->GetRMS()>TH_Run_to_Run) fprintf(pFile1,"HF: (%i,%i,%i)---->%.3f\n",eta,phi,depth,tmp->GetRMS());

	}   
  }

  gStyle->SetOptStat(111111);
  printf("Writing the RMS information to disk");
  fclose(pFile1);  
  TCanvas *c2=new TCanvas("c2","c2"); 
  c2->SetFillColor(0);
  c2->SetWindowPosition(500,30);
  c2->SetWindowSize(475,475);

  c2->Divide(2,2,0.0001,0.0001);
  c2->cd(1); c2_1->SetLogy(1);hHBrms->Draw();
  c2->cd(2); c2_2->SetLogy(1);hHErms->Draw();
  c2->cd(3); c2_3->SetLogy(1);hHOrms->Draw();
  c2->cd(4); c2_4->SetLogy(1);hHFrms->Draw();
  c2->SaveAs("PedRMS.jpg");

  processIOVs(REF,TH_IOV); 
}
