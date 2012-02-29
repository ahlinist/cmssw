void CompareLEDDatasets(int run=0,int ref=0,float th=0.5){
int IndRun=-1,IndRef=-1;
  for(int i=0;i<NumOfRuns;i++){
      if(leds[i]->get_run_number()==run) IndRun=i;
      if(leds[i]->get_run_number()==ref) IndRef=i;
  }
  if(IndRun<0 || IndRef<0) return;
  char str[100];
  sprintf(str,"HB: LED ratio (r%i -r%i)/r%i",run,ref,ref); TH1F *hb=new TH1F("diff hb",str,500,-3,3);
  sprintf(str,"HE: LED ratio (r%i -r%i)/r%i",run,ref,ref); TH1F *he=new TH1F("diff he",str,500,-3,3);
  sprintf(str,"HO: LED ratio (r%i -r%i)/r%i",run,ref,ref); TH1F *ho=new TH1F("diff ho",str,500,-3,3);
  sprintf(str,"HF: LED ratio (r%i -r%i)/r%i",run,ref,ref); TH1F *hf=new TH1F("diff hf",str,500,-3,3);

  leddata *data,*data0;
  for(int eta=-16;eta<=16;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=2;depth++){
     if(leds[IndRun]->get_led_value("HB",eta,phi,depth,&data) && leds[IndRef]->get_led_value("HB",eta,phi,depth,&data0)){
       if(data0->energy>20 && data->energy>20){
         hb->Fill(data->energy/data0->energy-1);
         if(fabs(data->energy/data0->energy-1)>th){
           printf("HB(%3i,%2i,%i):\tLED ratio (r%i-r%i)/r%i = %+.3f\n",eta,phi,depth,run,ref,ref,data->energy/data0->energy-1);
         }}
     }
  }
  for(int eta=-29;eta<=29;eta++) for(int phi=1;phi<=72;phi++)for(int depth=1;depth<=3;depth++){
     if(abs(eta)<15) continue;
     if(leds[IndRun]->get_led_value("HE",eta,phi,depth,&data) && leds[IndRef]->get_led_value("HE",eta,phi,depth,&data0)){
       if(data0->energy>20 && data->energy>20){
         he->Fill(data->energy/data0->energy-1);
         if(fabs(data->energy/data0->energy-1)>th){
           printf("HE(%3i,%2i,%i):\tLED ratio (r%i-r%i)/r%i = %+.3f\n",eta,phi,depth,run,ref,ref,data->energy/data0->energy-1);
         }}
     }
  }
  for(int eta=-41;eta<=41;eta++) for(int phi=1;phi<=72;phi+=2)for(int depth=1;depth<=2;depth++){
      if(abs(eta)<29) continue;
     if(leds[IndRun]->get_led_value("HF",eta,phi,depth,&data) && leds[IndRef]->get_led_value("HF",eta,phi,depth,&data0)){
       if(data0->energy>20 && data->energy>20){
         hf->Fill(data->energy/data0->energy-1);
         if(fabs(data->energy/data0->energy-1)>th){
           printf("HF(%3i,%2i,%i):\tLED ratio (r%i-r%i)/r%i = %+.3f\n",eta,phi,depth,run,ref,ref,data->energy/data0->energy-1);
         }}
     }
  }
  for(int eta=-15;eta<=15;eta++) for(int phi=1;phi<=72;phi++)for(int depth=4;depth<=4;depth++){
     if(leds[IndRun]->get_led_value("HO",eta,phi,depth,&data) && leds[IndRef]->get_led_value("HO",eta,phi,depth,&data0)){
       if(data0->energy>20 && data->energy>20){
         ho->Fill(data->energy/data0->energy-1);
         if(fabs(data->energy/data0->energy-1)>th){
           printf("HO(%3i,%2i,%i):\tLED ratio (r%i-r%i)/r%i = %+.3f\n",eta,phi,depth,run,ref,ref,data->energy/data0->energy-1);
         }}
     }
  }

  TCanvas *c=new TCanvas("c","c");
  c->Divide(2,2,0.00001,0.000001);
  c->cd(1); c->cd(1)->SetLogy(); hb->Draw();
  c->cd(2); c->cd(2)->SetLogy(); he->Draw();
  c->cd(3); c->cd(3)->SetLogy(); ho->Draw();
  c->cd(4); c->cd(4)->SetLogy(); hf->Draw();
}
