void DrawSingleChannelLaser(char *subdet,int eta,int phi,int depth){
char str[100];
  //////////////////////////////////////////////////////////////////////
  laserdata *data;
  float minE=9999999,maxE=0,minT=9999999,maxT=0;
  for(int i=0;i<NumOfRuns;i++){
      double aveT=-1,aveE=-1; 
      if(DATA[i]->get_ave_time_value(subdet,&aveT) && DATA[i]->get_ave_amp_value(subdet,&aveE) && DATA[i]->get_laser_value(subdet,eta,phi,depth,&data)){
	 if((data->energy)/aveE<minE) minE=(data->energy)/aveE;
	 if((data->energy)/aveE>maxE) maxE=(data->energy)/aveE;
         if((data->time-aveT)<minT) minT=data->time-aveT;
         if((data->time-aveT)>maxT) maxT=data->time-aveT;
      }
  }

  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Laser amplitude distribution (ADC counts)",subdet,eta,phi,depth); 
  TH1F *h1=new TH1F("Distribution",str,1000,0,maxE*2.0);
  h1->SetXTitle("Laser amplitude");
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Laser amplitude vs Run number",subdet,eta,phi,depth); 
  TH2F *h2=new TH2F("Distribution",str,1000,RUN_MIN-50,RUN_MAX+50,1000,0,maxE*2.0);
  h2->SetXTitle("Run number");
  h2->SetYTitle("Laser amplitude");
  h2->SetMarkerStyle(20);
  h2->SetMarkerColor(kBlue);
  h2->SetMarkerSize(0.7);
  h2->GetXaxis()->SetNoExponent(true);
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Laser timing distribution",subdet,eta,phi,depth); 
  TH1F *h11=new TH1F("Timing",str,1000,minT-1,maxT+1);
  h11->SetXTitle("Time (TS)");
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - Laser timing vs Run number",subdet,eta,phi,depth); 
  TH2F *h21=new TH2F("Timing",str,1000,RUN_MIN-50,RUN_MAX+50,1000,minT-1,maxT+1);
  h21->SetXTitle("Run number");
  h21->SetYTitle("Time (TS)");
  h21->SetMarkerStyle(20);
  h21->SetMarkerColor(kBlue);
  h21->SetMarkerSize(0.7);
  h21->GetXaxis()->SetNoExponent(true);  
  //////////////////////////////////////////////////////////////////////  
  for(int i=0;i<NumOfRuns;i++){
      double aveT=-1,aveE=-1; 
      if(DATA[i]->get_ave_time_value(subdet,&aveT) && DATA[i]->get_ave_amp_value(subdet,&aveE) && DATA[i]->get_laser_value(subdet,eta,phi,depth,&data)){
	  int run=DATA[i]->get_run_number();
          h1->Fill(data->energy/aveE);
          h2->Fill(run,data->energy/aveE);
          h11->Fill(data->time-aveT);
          h21->Fill(run,data->time-aveT);
          printf("%i\t%f\t%f\n",run,data->energy/aveE,data->time-aveT);
      }
  }
  //////////////////////////////////////////////////////////////////////
  TCanvas *c=new TCanvas("c","c");
  c->SetFillColor(0);
  c->SetWindowPosition(12,30);
  c->SetWindowSize(475,475);
  c->Divide(1,2,0.0001,0.0001);
  c->cd(1); h2->Draw();
  c->cd(2); h1->Draw();
  c->Update();

  char title1[100];
  sprintf(title1,"LaserAmplitude_%s_%i_%i_%i.jpg",subdet,eta,phi,depth);
  c->SaveAs(title1);


  TCanvas *c1=new TCanvas("c1","c1");
  c1->SetFillColor(0);
  c1->SetWindowPosition(500,30);
  c1->SetWindowSize(475,475);
  c1->Divide(1,2,0.0001,0.0001);
  c1->cd(1); h21->Draw();
  c1->cd(2); h11->Draw();
  c1->Update();

  char title2[100];
  sprintf(title2,"LaserRMS_%s_%i_%i_%i.jpg",subdet,eta,phi,depth);
  c1->SaveAs(title2);



}
