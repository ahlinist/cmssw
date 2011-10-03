void CheckLaserTiming(){
   char *subdet[4]={"HB","HE"};
   float RMS[2][60][72][3];

   TH1D *hb_rms=new TH1D("hb_rms","hb run to run rms",1000,0,1); 
   TH1D *he_rms=new TH1D("he_rms","he run to run rms",1000,0,1); 

   TH1D *HBP=new TH1D("HBP","HBP run-to-run timing RMS",36,0.5,36.5);
   TH1D *HBM=new TH1D("HBM","HBM run-to-run timing RMS",36,0.5,36.5);
   TH1D *HEP=new TH1D("HEP","HEP run-to-run timing RMS",36,0.5,36.5);
   TH1D *HEM=new TH1D("HEM","HEM run-to-run timing RMS",36,0.5,36.5);
   char str[20];
   for(int i=1;i<=72;i+=2){ sprintf(str,"HBM %02i-%02i",i,i+1); HBM->GetXaxis()->SetBinLabel(i/2+1,str);}
   HBM->GetXaxis()->SetBit(TAxis::kLabelsVert); HBM->GetXaxis()->SetLabelSize(0.05); HBM->SetMarkerStyle(20);
   for(int i=1;i<=72;i+=2){ sprintf(str,"HBP %02i-%02i",i,i+1); HBP->GetXaxis()->SetBinLabel(i/2+1,str);}
   HBP->GetXaxis()->SetBit(TAxis::kLabelsVert); HBP->GetXaxis()->SetLabelSize(0.05); HBP->SetMarkerStyle(20);
   for(int i=1;i<=72;i+=2){ sprintf(str,"HEM %02i-%02i",i,i+1); HEM->GetXaxis()->SetBinLabel(i/2+1,str);}
   HEM->GetXaxis()->SetBit(TAxis::kLabelsVert); HEM->GetXaxis()->SetLabelSize(0.05); HEM->SetMarkerStyle(20);
   for(int i=1;i<=72;i+=2){ sprintf(str,"HEP %02i-%02i",i,i+1); HEP->GetXaxis()->SetBinLabel(i/2+1,str);}
   HEP->GetXaxis()->SetBit(TAxis::kLabelsVert); HEP->GetXaxis()->SetLabelSize(0.05); HEP->SetMarkerStyle(20);

   TH2F *MAP=new TH2F("map","Laser timing problems map",60,-29.5,29.5,72,0.5,72.5);
   //timing loop
   float N=0,SUM=0;
   for(int sd=0;sd<=1;sd++) for(int eta=-29;eta<=29;eta++) for(int phi=1;phi<=72;phi++) for(int depth=1;depth<=3;depth++){
         float xt=0,xxt=0,nt=0;
         RMS[sd][eta+30][phi-1][depth-1]=-1;
         laserdata *data;
         for(int i=0;i<NumOfRuns;i++){
             double ave=-1;
             if(DATA[i]->get_ave_time_value(subdet[sd],&ave) && DATA[i]->get_laser_value(subdet[sd],eta,phi,depth,&data)){
                if(data->energy<100) continue;
                xt+=data->time-ave; xxt+=(data->time-ave)*(data->time-ave); nt+=1.0;
             }
         }
         ////////////////
         if(nt>3 && (xt/nt)!=0){
             float rms=sqrt(xxt/nt-(xt*xt)/(nt*nt));
             RMS[sd][eta+30][phi-1][depth-1]=rms;
             SUM+=rms; N+=1.0;
             if(sd==0) hb_rms->Fill(rms);
             if(sd==1) he_rms->Fill(rms);
         }
         ////////////////
   }
   for(int phi=1;phi<=72;phi+=2){
     float n=0,sum=0; 
     for(int eta=15;eta<=29;eta++) for(int depth=1;depth<=3;depth++){
       if(RMS[1][eta+30][phi-1][depth-1]>0){ n+=1.0; sum+=RMS[1][eta+30][phi-1][depth-1];}
       if(RMS[1][eta+30][phi][depth-1]>0){   n+=1.0; sum+=RMS[1][eta+30][phi][depth-1];}
     }
     if(n>10) HEP->SetBinContent(phi/2+1,sum/n); 
   }
   for(int phi=1;phi<=72;phi+=2){
     float n=0,sum=0; 
     for(int eta=-29;eta<=-15;eta++) for(int depth=1;depth<=3;depth++){
       if(RMS[1][eta+30][phi-1][depth-1]>0){ n+=1.0; sum+=RMS[1][eta+30][phi-1][depth-1];}
       if(RMS[1][eta+30][phi][depth-1]>0){   n+=1.0; sum+=RMS[1][eta+30][phi][depth-1];}
     }
     if(n>10) HEM->SetBinContent(phi/2+1,sum/n); 
   }
   for(int phi=1;phi<=72;phi+=2){
     float n=0,sum=0; 
     for(int eta=0;eta<=16;eta++) for(int depth=1;depth<=2;depth++){
       if(RMS[0][eta+30][phi-1][depth-1]>0){ n+=1.0; sum+=RMS[0][eta+30][phi-1][depth-1];}
       if(RMS[0][eta+30][phi][depth-1]>0){   n+=1.0; sum+=RMS[0][eta+30][phi][depth-1];}
     }
     if(n>10) HBP->SetBinContent(phi/2+1,sum/n); 
   }
   for(int phi=1;phi<=72;phi+=2){
     float n=0,sum=0; 
     for(int eta=-16;eta<=0;eta++) for(int depth=1;depth<=2;depth++){
       if(RMS[0][eta+30][phi-1][depth-1]>0){ n+=1.0; sum+=RMS[0][eta+30][phi-1][depth-1];}
       if(RMS[0][eta+30][phi][depth-1]>0){   n+=1.0; sum+=RMS[0][eta+30][phi][depth-1];}
     }
     if(n>10) HBM->SetBinContent(phi/2+1,sum/n); 
   }

   if(N>0) for(int sd=0;sd<=1;sd++)for(int phi=1;phi<=72;phi++) for(int eta=-29;eta<=29;eta++) for(int depth=1;depth<=3;depth++){
     if(RMS[sd][eta+30][phi-1][depth-1]>(SUM/N*3.0)) MAP->Fill(eta,phi);
   }

   gStyle->SetPalette(1);
   gStyle->SetOptStat(10);
   gStyle->SetMarkerColor(kRed);

   TCanvas *c1=new TCanvas("c1","c1");
   c1->Divide(1,2,0.000001,0.000001);
   c1->cd(1)->SetLogy(); c1->cd(1); hb_rms->Draw();
   c1->cd(2)->SetLogy(); c1->cd(2); he_rms->Draw();
 
   TCanvas *c2=new TCanvas("c2","c2");
   c2->Divide(2,2,0.000001,0.000001);
   float max;
   max=HEM->GetMaximum(); if(max<0.1) max=0.1; HEM->GetYaxis()->SetRangeUser(0,max);
   max=HEP->GetMaximum(); if(max<0.1) max=0.1; HEP->GetYaxis()->SetRangeUser(0,max);
   max=HBM->GetMaximum(); if(max<0.1) max=0.1; HBM->GetYaxis()->SetRangeUser(0,max);
   max=HBP->GetMaximum(); if(max<0.1) max=0.1; HBP->GetYaxis()->SetRangeUser(0,max);
   c2->cd(1); c2->cd(1)->SetBottomMargin(0.2); HBP->Draw("P");
   c2->cd(2); c2->cd(2)->SetBottomMargin(0.2); HBM->Draw("P");
   c2->cd(3); c2->cd(3)->SetBottomMargin(0.2); HEP->Draw("P");
   c2->cd(4); c2->cd(4)->SetBottomMargin(0.2); HEM->Draw("P");

   MAP->GetXaxis()->SetTitle("i#eta");
   MAP->GetYaxis()->SetTitle("i#phi");
   TCanvas *c3=new TCanvas("c3","c3");
   c3->cd(); MAP->Draw("COLZ");

}