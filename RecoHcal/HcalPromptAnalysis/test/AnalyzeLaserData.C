#include <stdio.h>
void AnalyzeLaserData(float AmpTH=0.06,float TimeTH=0.06){
   char *subdet[4]={"HB","HE","HO","HF"};
   TH1D *h_data=new TH1D("h","h",1000,0,2000); 
   TH1D *h_data1=new TH1D("h1","h1",1000,-10,10); 
 
   TH1D *LASER_ENERGY     =new TH1D("LASER ENERGY","LASER ENERGY",1000,0,4);
   TH1D *LASER_ENERGY_RMS =new TH1D("Ampl RMS","Run to Run Laser amplitude rms",1000,0,0.5);
   TH1D *LASER_TIMING     =new TH1D("LASER TIMING","LASER TIMING",1000,-2,2);
   TH1D *LASER_TIMING_RMS =new TH1D("Time RMS","Run to Run Laser timing rms",1000,0,0.35);
   TH2F *LASER_TIMING_MAPHBHEHF =new TH2F("timing problems map","timing problems map",61,-30,30,74,0,73);
   LASER_TIMING_MAPHBHEHF->SetXTitle("i#eta");
   LASER_TIMING_MAPHBHEHF->SetYTitle("i#phi");
   gStyle->SetOptStat(111111);
   //gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
 
   laserdata *data;
  
   FILE * pFile;
   pFile = fopen("LaserSummary1.txt","w");

  int n=0;
   // energy loop
   for(int sd=0;sd<=3;sd++) for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++) for(int depth=1;depth<=4;depth++){
         h_data->Reset();
	 for(int i=0;i<NumOfRuns;i++){
             double ave=-1;
	     if(DATA[i]->get_ave_amp_value(subdet[sd],&ave) && DATA[i]->get_laser_value(subdet[sd],eta,phi,depth,&data)){
 	         h_data->Fill(data->energy/ave);
	     }
         } 
	 ////////////////
	 if(h_data->GetEntries()>1 && h_data->GetMean()!=0){
	    if(h_data->GetRMS()/h_data->GetMean()>AmpTH){
	      printf("ENERGY %s: (%i,%i,%i)->%f [%i]\n",subdet[sd],eta,phi,depth,h_data->GetRMS()/h_data->GetMean(),h_data->GetEntries());
	      fprintf(pFile,"ENERGY %s: (%i,%i,%i)->%f [%i]\n",subdet[sd],eta,phi,depth,h_data->GetRMS()/h_data->GetMean(),h_data->GetEntries());

	    }
	    LASER_ENERGY_RMS->Fill(h_data->GetRMS()/h_data->GetMean());
	    LASER_ENERGY->Fill(h_data->GetMean());
	 }
	 ////////////////
   }  
   //timing loop
   for(int sd=0;sd<=3;sd++) for(int eta=-42;eta<=42;eta++) for(int phi=1;phi<=72;phi++) for(int depth=1;depth<=4;depth++){
         h_data1->Reset();
	 laserdata *data;
	 for(int i=0;i<NumOfRuns;i++){
	     double ave=-1;
  	    
	     if(DATA[i]->get_ave_time_value(subdet[sd],&ave) && DATA[i]->get_laser_value(subdet[sd],eta,phi,depth,&data)){
	        h_data1->Fill(data->time-ave);
	     }
         }
	 ////////////////
	 if(h_data1->GetEntries()>1 && h_data1->GetMean()!=0){
	    if(h_data1->GetRMS()>TimeTH){
	      printf("TIMING %s: (%i,%i,%i)->%f [%i]\n",subdet[sd],eta,phi,depth,h_data1->GetRMS(),h_data1->GetEntries());
               fprintf(pFile,"TIMING %s: (%i,%i,%i)->%f [%i]\n",subdet[sd],eta,phi,depth,h_data1->GetRMS(),h_data1->GetEntries());

	       LASER_TIMING_MAPHBHEHF->Fill(eta,phi);
	       if(abs(eta)>20)LASER_TIMING_MAPHBHEHF->Fill(eta,phi+1);
	    }
	    LASER_TIMING_RMS->Fill(h_data1->GetRMS());
	    LASER_TIMING->Fill(h_data1->GetMean());
	    
	 }
	 ////////////////
   }  
   
   TCanvas *c=new TCanvas("c","c");
   c->SetFillColor(0);
   c->cd(); 
   c->SetWindowPosition(12,30);
   c->SetWindowSize(475,475);

   c->SetLogy(1);
   LASER_ENERGY_RMS->Draw();
   c->Update();
   printf("Writing canvas c\n");
   c->SaveAs("LaserEnergyRMS.jpg");

   TCanvas *c1=new TCanvas("c1","c1");
   c1->SetFillColor(0);
 
   c1->cd(); 
   c1->SetWindowPosition(500,30);
   c1->SetWindowSize(475,475);

   c1->SetLogy(1);

   LASER_ENERGY->Draw();
   c1->Update();
   printf("Writing canvas c1\n");
 
   c1->SaveAs("LaserEnergy.jpg");

   // MAP->Draw();
   TCanvas *c2=new TCanvas("c2","c2");
   c2->SetFillColor(0);
   c2->cd(); 
   c2->SetWindowPosition(12,534);
   c2->SetWindowSize(475,475);

   c2->SetLogy(1);

   LASER_TIMING_RMS->Draw();
   c2->Update();
   printf("Writing canvas c2\n");
 
   c2->SaveAs("LaserTimingRMS.jpg");
   TCanvas *c3=new TCanvas("c3","c3");
   c3->SetFillColor(0);
   c3->cd(); 
   c3->SetWindowPosition(500,534);
   c3->SetWindowSize(475,475);

   c3->SetLogy(1);
   LASER_TIMING->Draw(); //LASER_TIMING_MAPHBHEHF->Draw();
   c3->Update();
   printf("Writing canvas c3\n");
    
   c3->SaveAs("LaserTiming.jpg");

   printf("========================================================\n"); 
   fprintf(pFile,"========================================================\n");
   fclose(pFile);
}
