
void SingleChannelPED(char *subdet="",int ETA=0,int PHI=0,int DEPTH=0){
char   str[100]; 
  gDirectory->DeleteAll(); 
  //////////////////////////////////////////////////////////////////////
  if(ETA==0 || PHI==0 || DEPTH==0){
     printf("\n\nUsage:\n\n");
     printf("ls HcalHardwarePedestalMonitor_run* > FileNames\n");
     printf("root -l\n");
     printf(".x SingleChannelPED.C(\"SubDet\",eta,phi,depth)\n\n");
     printf("Example: .x SingleChannelPED.C(\"HB\",1,1,1)\n\n");
     return;
  }
  int min_run=99999999,max_run=0;
  float max_ped=0;
  for(int i=0;i<NumOfRuns;i++){
      if(peds[i]->get_run_number()<min_run) min_run=peds[i]->get_run_number();
      if(peds[i]->get_run_number()>max_run) max_run=peds[i]->get_run_number();
      double ped; 
      if(peds[i]->get_ped_value(subdet,ETA,PHI,DEPTH,&ped)){
         if(max_ped<ped) max_ped=ped; 
      }
  }
  //////////////////////////////////////////////////////////////////////
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Pedeslal values distribution (ADC counts)",subdet,ETA,PHI,DEPTH); 
  TH1F *h1=new TH1F("Distribution",str,1000,0,max_ped*1.5);
  h1->SetXTitle("Pedestal value (ADC counts)");
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Pedeslal values vs Run number",subdet,ETA,PHI,DEPTH); 
  TH2F *h2=new TH2F("Distribution",str,1000,min_run-100,max_run+100,max_run-min_run+200,0,max_ped*1.5);
  h2->SetXTitle("Run number");
  h2->SetYTitle("Pedestal value (ADC counts)");
  h2->SetMarkerStyle(20);
  h2->SetMarkerColor(kBlue);
  h2->SetMarkerSize(0.7);
  
  //////////////////////////////////////////////////////////////////////  
  for(int i=0;i<NumOfRuns;i++){ 
    double ped; 
    if(peds[i]->get_ped_value(subdet,ETA,PHI,DEPTH,&ped)){
       printf("%i %f\n",peds[i]->get_run_number(),ped);
       h1->Fill(ped);
       h2->Fill(peds[i]->get_run_number(),ped);
    }
  }
  //////////////////////////////////////////////////////////////////////
  TCanvas *c=new TCanvas("c","c");
  c->SetFillColor(0);
  c->SetWindowPosition(12,30);
  c->SetWindowSize(475,475);
 
  c->Divide(1,2,0.0001,0.0001);
  // c->Divide(1,2,0.0001,0.0001);
  c->cd(1); h2->Draw();
  c->cd(2); 
  c_2->SetLogy(1);
  h1->Draw();
  c->Update();

  char title[100];
  sprintf(title,"PED_%s_%i_%i_%i.jpg",subdet, ETA, PHI, DEPTH);
  c->SaveAs(title);

}
