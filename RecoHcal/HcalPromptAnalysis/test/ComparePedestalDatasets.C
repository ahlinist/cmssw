void ComparePedestalDatasets(int run=0,int ref=0,float th=0.5){
int IndRun=-1,IndRef=-1;
  for(int i=0;i<NumOfRuns;i++){
      if(peds[i]->get_run_number()==run) IndRun=i;
      if(peds[i]->get_run_number()==ref) IndRef=i;
  }
  if(IndRun<0 || IndRef<0) return;
  char str[100];
  sprintf(str,"HB pedestal difference: r%i - r%i",run,ref);
  TH1F *hb=new TH1F("hb",str,500,-3,3);
  sprintf(str,"HE pedestal difference: r%i - r%i",run,ref);
  TH1F *he=new TH1F("he",str,500,-3,3);
  sprintf(str,"HO pedestal difference: r%i - r%i",run,ref);
  TH1F *ho=new TH1F("ho",str,500,-3,3);
  sprintf(str,"HF pedestal difference: r%i - r%i",run,ref);
  TH1F *hf=new TH1F("hf",str,500,-3,3);

  double ped,ped0;
  for(int eta=-16;eta<=16;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=2;depth++){
     if(peds[IndRun]->get_ped_value("HB",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HB",eta,phi,depth,&ped0)){
         hb->Fill(ped-ped0);
         if(fabs(ped-ped0)>th){
           printf("HB(%3i,%2i,%i):\tPedestal(run %i)-Pedestal(run %i) = %+.3f\n",eta,phi,depth,run,ref,ped-ped0);
         }
     }
  }
  for(int eta=-29;eta<=29;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
     if(abs(eta)<15) continue;
     if(peds[IndRun]->get_ped_value("HE",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HE",eta,phi,depth,&ped0)){
         he->Fill(ped-ped0);
         if(fabs(ped-ped0)>th){
           printf("HE(%3i,%2i,%i):\tPedestal(run %i)-Pedestal(run %i) = %+.3f\n",eta,phi,depth,run,ref,ped-ped0);
         }
     }
  }
  for(int eta=-41;eta<=41;eta++) for(int phi=1;phi<=72;phi+=2)for(int depth=1;depth<=2;depth++){
      if(abs(eta)<29) continue;
      if(peds[IndRun]->get_ped_value("HF",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HF",eta,phi,depth,&ped0)){
         hf->Fill(ped-ped0);
         if(fabs(ped-ped0)>th){
           printf("HF(%3i,%2i,%i):\tPedestal(run %i)-Pedestal(run %i) = %+.3f\n",eta,phi,depth,run,ref,ped-ped0);
         }
     }
  }
  for(int eta=-15;eta<=15;eta++) for(int phi=1;phi<=72;phi++)for(int depth=4;depth<=4;depth++){
     if(peds[IndRun]->get_ped_value("HO",eta,phi,depth,&ped) && peds[IndRef]->get_ped_value("HO",eta,phi,depth,&ped0)){
         ho->Fill(ped-ped0);
         if(fabs(ped-ped0)>th){
           printf("HO(%3i,%2i,%i):\tPedestal(run %i)-Pedestal(run %i) = %+.3f\n",eta,phi,depth,run,ref,ped-ped0);
         }
     }
  }

  TCanvas *c=new TCanvas("c","c");
  c->Divide(2,2,0.00001,0.000001);
  c->cd(1); c->cd(1)->SetLogy(); hb->Draw();
  c->cd(2); c->cd(2)->SetLogy(); he->Draw();
  c->cd(3); c->cd(3)->SetLogy(); ho->Draw();
  c->cd(4); c->cd(4)->SetLogy(); hf->Draw();
}