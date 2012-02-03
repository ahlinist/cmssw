void CheckPedestalShifts(int ref,float TH=0.5){
int SHIFTS[4][83][72][4];
char *SD[4]={"HB","HE","HF","HO"};
int indRef=-1,run_min=10000000,run_max=0;
   for(int i=0;i<NumOfRuns;i++){
       if(peds[i]->get_run_number()>run_max) run_max=peds[i]->get_run_number();
       if(peds[i]->get_run_number()<run_min) run_min=peds[i]->get_run_number();
       if(peds[i]->get_run_number()==ref)    indRef=i;
   }
   if(indRef<0) return;
   for(int i=0;i<4;i++)for(int j=0;j<83;j++)for(int k=0;k<72;k++)for(int l=0;l<4;l++) SHIFTS[i][j][k][l]=0;

   TH2F *map_hbef=new TH2F("HBEF","HBEF: Number of pedestal shifts |ped(n)-ped(ref)|>th",87,-43,43,74,0,73);
   TH2F *map_ho=new TH2F("HO","HO: Number of pedestal shifts |ped(n)-ped(ref)|>th",33,-16,16,74,0,73);
   TH1F *N=new TH1F("shifts","Number of pedestal shifrs per run",run_max-run_min,run_min,run_max);

   printf("\n\nNumber of channels with |ped(i)-ped(ref)|>%.3f\n\n",TH);
   for(int IndRun=0;IndRun<NumOfRuns;IndRun++){ if(peds[IndRun]->get_set_number()>1) continue;
          double ped,ped0;
          int n_HB=0,n_HE=0,n_HF=0,n_HO=0,r=peds[IndRun]->get_run_number();
  
	  for(int eta=-16;eta<=16;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=2;depth++){
             if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HB",eta,phi,depth,&ped0)){
	        if(fabs(ped-ped0)>TH){ n_HB++; SHIFTS[0][eta+41][phi-1][depth-1]++; map_hbef->Fill(eta,phi);}
	     }
          }
	  for(int eta=-29;eta<=29;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
             if(abs(eta)<15) continue;
  	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HE",eta,phi,depth,&ped0)){
 	        if(fabs(ped-ped0)>TH){ n_HE++; SHIFTS[1][eta+41][phi-1][depth-1]++; map_hbef->Fill(eta,phi);}
	     }
          }
	  for(int eta=-41;eta<=41;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
             if(abs(eta)<29) continue;
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HF",eta,phi,depth,&ped0)){
 	        if(fabs(ped-ped0)>TH){ n_HF++; SHIFTS[2][eta+41][phi-1][depth-1]++; map_hbef->Fill(eta,phi);}
 	     }
          }
	  for(int eta=-15;eta<=15;eta++) for(int phi=1;phi<=72;phi++)for(int depth=4;depth<=4;depth++){
             if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HO",eta,phi,depth,&ped0)){
	        if(fabs(ped-ped0)>TH){ n_HO++; SHIFTS[3][eta+41][phi-1][depth-1]++; map_ho->Fill(eta,phi);}
	     }
          }
          N->Fill(peds[IndRun]->get_run_number(),n_HB+n_HE+n_HF+n_HO);
	  printf("Run %i: HB - %i HE - %i HF - %i HO - %i\n",peds[IndRun]->get_run_number(),n_HB,n_HE,n_HF,n_HO);
   }

   printf("\n\nNumber of datasets with |ped(i)-ped(ref)|>%.3f\n\n",TH);
   for(int i=0;i<4;i++)for(int j=0;j<83;j++)for(int k=0;k<72;k++)for(int l=0;l<4;l++){
      if(SHIFTS[i][j][k][l]>0) printf("%s(%3i,%2i,%i)\t ---> %i\n",SD[i],j-41,k+1,l+1,SHIFTS[i][j][k][l]);
   }

   TCanvas *c=new TCanvas("c","c"); c->cd();
   map_hbef->Draw("Text");
   TCanvas *c1=new TCanvas("c1","c1"); c1->cd();
   map_ho->Draw("Text");
   TCanvas *c2=new TCanvas("c2","c2"); c2->cd();
   N->Draw();

}
