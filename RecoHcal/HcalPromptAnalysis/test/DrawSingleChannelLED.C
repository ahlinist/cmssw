void DrawSingleChannelLED(char *sd,int eta,int phi,int depth){
char str[100];
 //////////////////////////////////////////////////////////////////////
  double max=0,max1=0,ave;
  leddata *data,*calib;
  for(int i=0;i<NumOfRuns;i++){
     if(leds[i]->get_led_value(sd,eta,phi,depth,&data)){
	double val=data->energy;
	if(val>max) max=val;  
	val=data->time; 
	if(val>max1) max1=val;  
     }
  }	


  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - LED values distribution (ADC counts)",sd,eta,phi,depth); 
  TH1F *h1=new TH1F("Distribution",str,1000,0,max*2);
  h1->SetXTitle("LED amplitude value (ADC counts)");
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - LED values vs Run number",sd,eta,phi,depth); 
  TH2F *h2=new TH2F("Distribution",str,RUN_MAX-RUN_MIN+100,RUN_MIN-50,RUN_MAX+50,1000,0,max*2);
  h2->SetXTitle("Run number");
  h2->SetYTitle("LED amplitude (ADC counts)");
  h2->SetMarkerStyle(20);
  h2->SetMarkerColor(kBlue);
  h2->SetMarkerSize(0.7);
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - LED timing distribution (TS)",sd,eta,phi,depth); 
  TH1F *h11=new TH1F("Distribution",str,1000,0,max1*2);
  h11->SetXTitle("LED timing (TS)");
  sprintf(str,"%s: Eta=%i,Phi=%i,Depth=%i - LED timing vs Run number",sd,eta,phi,depth); 
  TH2F *h22=new TH2F("Distribution",str,RUN_MAX-RUN_MIN+100,RUN_MIN-50,RUN_MAX+50,1000,0,max1*2);
  h22->SetXTitle("Run number");
  h22->SetYTitle("LED timing (TS)");
  h22->SetMarkerStyle(20);
  h22->SetMarkerColor(kBlue);
  h22->SetMarkerSize(0.7);
  
  //////////////////////////////////////////////////////////////////////  
  for(int i=0;i<NumOfRuns;i++){
    if(leds[i]->get_led_value(sd,eta,phi,depth,&data) /*&& leds[i]->get_calib_value(sd,eta,phi,&calib)*/){
	  int run=leds[i]->get_run_number();
	  double val=data->energy;
	  printf("%i %f\n",run,val);
          h1->Fill(val);
          h2->Fill(run,val);
	  val=data->time;
          h11->Fill(val);
          h22->Fill(run,val);
    }
  }
  //////////////////////////////////////////////////////////////////////
  TCanvas *c=new TCanvas("c","c");
  c->Divide(1,2,0.0001,0.0001);
  c->cd(1); h2->Draw();
  c->cd(2); h1->Draw();
  TCanvas *c1=new TCanvas("c1","c1");
  c1->Divide(1,2,0.0001,0.0001);
  c1->cd(1); h22->Draw();
  c1->cd(2); h11->Draw();
}
