void CheckPedestalShifts(int ref,float TH_Run_to_Run=0.02){
int indRef=-1,run_min=10000000,run_max=0;
   for(int i=0;i<NumOfRuns;i++){
       if(peds[i]->get_run_number()>run_max) run_max=peds[i]->get_run_number();
       if(peds[i]->get_run_number()<run_min) run_min=peds[i]->get_run_number();
       if(peds[i]->get_run_number()==ref)    indRef=i;
   }
   if(indRef<0) return;

   TH2F *map_hbef=new TH2F("HBEF","HBEF: Number of pedestal shifts |ped(n)-ped(ref)|>th",87,-43,43,74,0,73);
   TH2F *map_ho=new TH2F("HO","HO: Number of pedestal shifts |ped(n)-ped(ref)|>th",33,-16,16,74,0,73);
   TH1F *N=new TH1F("shifts","Number of pedestal shifrs per run",run_max-run_min,run_min,run_max);

   for(int IndRun=0;IndRun<NumOfRuns;IndRun++){ if(peds[IndRun]->get_set_number()>1) continue;
          double ped,ped0;
          int n=0,n_all=0,r=peds[IndRun]->get_run_number();
  
	  for(int eta=-16;eta<=16;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=2;depth++){
             if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HB",eta,phi,depth,&ped0)){
	        if(fabs(ped-ped0)>TH_Run_to_Run){ n++; map_hbef->Fill(eta,phi);}
                if(fabs(ped-ped0)>TH_Run_to_Run) n_all++;
	     }
          }
	  for(int eta=-29;eta<=29;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
             if(abs(eta)<15) continue;
  	     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HE",eta,phi,depth,&ped0)){
 	        if(fabs(ped-ped0)>TH_Run_to_Run){ n++; map_hbef->Fill(eta,phi);}
                if(fabs(ped-ped0)>TH_Run_to_Run) n_all++;
	     }
          }
	  for(int eta=-41;eta<=41;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
             if(abs(eta)<29) continue;
	     if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HF",eta,phi,depth,&ped0)){
 	        if(fabs(ped-ped0)>TH_Run_to_Run){ n++; map_hbef->Fill(eta,phi);}
                if(fabs(ped-ped0)>TH_Run_to_Run) n_all++;
 	     }
          }
	  for(int eta=-15;eta<=15;eta++) for(int phi=1;phi<=72;phi++)for(int depth=4;depth<=4;depth++){
             if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped) && peds[indRef]->get_ped_value("HO",eta,phi,depth,&ped0)){
	        if(fabs(ped-ped0)>TH_Run_to_Run){ n++; map_ho->Fill(eta,phi);}
                if(fabs(ped-ped0)>TH_Run_to_Run) n_all++;
	     }
          }
          N->Fill(peds[IndRun]->get_run_number(),n_all);
	  printf("look at run %i: %i\n",peds[IndRun]->get_run_number(),n);
   }
   TCanvas *c=new TCanvas("c","c"); c->cd();
   map_hbef->Draw("Text");
   TCanvas *c1=new TCanvas("c1","c1"); c1->cd();
   map_ho->Draw("Text");
   TCanvas *c2=new TCanvas("c2","c2"); c2->cd();
   N->Draw();
}
