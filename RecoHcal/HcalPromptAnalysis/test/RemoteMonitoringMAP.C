// How to run:
//root -b -q -l RemoteMonitoringMAP.C+
//root -b -q -l 'RemoteMonitoringMAP.C+("test7runs.root")'

#include "LogEleMapdb.h"

#include <iostream>
#include <fstream>

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <TMath.h>
#include "TStyle.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TText.h"
#include "TAxis.h"
#include "TFile.h"

using namespace std;

void RemoteMonitoringMAP(const char* fname = "test7runs.root")
{
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);  
    gStyle->SetOptTitle(1);  

//======================================================================
// Connect the input file and get the 2-d histogram in memory
//    TFile *hfile= new TFile("GlobalHist.root", "READ");
    string runnumber;
    runnumber = "LED "+ (string) fname;
    TFile *hfile= new TFile( fname, "READ");
//    TFile *hfile= new TFile("testNZS.root", "READ");            
//======================================================================



//======================================================================
// Prepare histograms and plot them to .png files

 
  TCanvas *cHB = new TCanvas("cHB","cHB",1000,400);
  TCanvas *cHE = new TCanvas("cHE","cHE",1500,400);
  TCanvas *cONE = new TCanvas("cONE","cONE",500,500);
  
  char *str = (char*)alloca(10000);


  TH2F *Map_Ampl[10][5][5]; // 2D histogramm for test,subdet,depth
  TH2F *Map_SUB[5][5];      // 2d histogramm for subdet, depth
  TH1F *HistAmpl[10][5];    // 1d histogramm for test,subdet

  Map_SUB[1][1] = (TH2F*)hfile->Get("h_mapDepth1_HB");
  Map_SUB[1][2] = (TH2F*)hfile->Get("h_mapDepth2_HB");   
  Map_SUB[2][1] = (TH2F*)hfile->Get("h_mapDepth1_HE");
  Map_SUB[2][2] = (TH2F*)hfile->Get("h_mapDepth2_HE"); 
  Map_SUB[2][3] = (TH2F*)hfile->Get("h_mapDepth3_HE"); 

//+++++++++++++++++++++++++++++  
//Test 1 (2.A) Rate of Cap ID errors   
//+++++++++++++++++++++++++++++ 
 
  Map_Ampl[1][1][1] = (TH2F*)hfile->Get("h_mapDepth1Error_HB");
  Map_Ampl[1][1][2] = (TH2F*)hfile->Get("h_mapDepth2Error_HB");   
  Map_Ampl[1][2][1] = (TH2F*)hfile->Get("h_mapDepth1Error_HE");
  Map_Ampl[1][2][2] = (TH2F*)hfile->Get("h_mapDepth2Error_HE"); 
  Map_Ampl[1][2][3] = (TH2F*)hfile->Get("h_mapDepth3Error_HE");    

//2 HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i); 
     Map_Ampl[1][1][i]->Divide(Map_Ampl[1][1][i],Map_SUB[1][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     sprintf(str,"HB, Depth%d \b", i); 
     Map_Ampl[1][1][i]->SetTitle(str);
     Map_Ampl[1][1][i]->SetXTitle("#eta \b");
     Map_Ampl[1][1][i]->SetYTitle("#phi \b");
     Map_Ampl[1][1][i]->SetZTitle("Rate \b");
     Map_Ampl[1][1][i]->Draw("COLZ");
     Map_Ampl[1][1][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[1][1][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHB->Modified(); 
     cHB->Update();
  }    
  cHB->Print("MapRateCapIDHB.png"); 
  cHB->Clear();    
     
//3 HE
  cHE->Divide(3,1);
  for (int i=1;i<=3;i++) {
     cHE->cd(i); 
     Map_Ampl[1][2][i]->Divide(Map_Ampl[1][2][i],Map_SUB[2][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     sprintf(str,"HE, Depth%d \b", i); 
     Map_Ampl[1][2][i]->SetTitle(str);
     Map_Ampl[1][2][i]->SetXTitle("#eta \b");
     Map_Ampl[1][2][i]->SetYTitle("#phi \b");
     Map_Ampl[1][2][i]->SetZTitle("Rate \b");
     Map_Ampl[1][2][i]->Draw("COLZ");
     Map_Ampl[1][2][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[1][2][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHE->Modified(); 
     cHE->Update(); 
     
  }    
  cHE->Print("MapRateCapIDHE.png");
  cHE->Clear();

//+++++++++++++++++++++++++++++  
//Test 2 (2.B) Rate of RMS   
//+++++++++++++++++++++++++++++ 
 
  Map_Ampl[2][1][1] = (TH2F*)hfile->Get("h_mapDepth1Amplitude225_HB");
  Map_Ampl[2][1][2] = (TH2F*)hfile->Get("h_mapDepth2Amplitude225_HB");   
  Map_Ampl[2][2][1] = (TH2F*)hfile->Get("h_mapDepth1Amplitude225_HE");
  Map_Ampl[2][2][2] = (TH2F*)hfile->Get("h_mapDepth2Amplitude225_HE"); 
  Map_Ampl[2][2][3] = (TH2F*)hfile->Get("h_mapDepth3Amplitude225_HE");    
  
  HistAmpl[2][1] = (TH1F*)hfile->Get("h_Amplitude_HB");
  HistAmpl[2][2] = (TH1F*)hfile->Get("h_Amplitude_HE");

//2 HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i); 
     Map_Ampl[2][1][i]->Divide(Map_Ampl[2][1][i],Map_SUB[1][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     sprintf(str,"HB, Depth%d \b", i); 
     Map_Ampl[2][1][i]->SetTitle(str);
     Map_Ampl[2][1][i]->SetXTitle("#eta \b");
     Map_Ampl[2][1][i]->SetYTitle("#phi \b");
     Map_Ampl[2][1][i]->SetZTitle("Rate \b");
     Map_Ampl[2][1][i]->Draw("COLZ");
     Map_Ampl[2][1][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[2][1][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHB->Modified(); 
     cHB->Update();
  }    
  cHB->Print("MapRateRMSHB.png"); 
  cHB->Clear();
  
  cONE->Divide(1,1);
  cONE->cd(1);
  gPad->SetGridy();
  gPad->SetGridx(); 
  gPad->SetLogy();
  HistAmpl[2][1]->SetTitleOffset(1.3,"Y"); 
  HistAmpl[2][1]->SetTitle("HB, All Depth");
  HistAmpl[2][1]->SetXTitle("TS RMS in each event & cell \b");
  HistAmpl[2][1]->SetYTitle("Number of cell-events \b");
  HistAmpl[2][1]->Draw("");
//  HistAmplitudeHB->GetYaxis()->SetRangeUser(1., 100.);
  HistAmpl[2][1]->GetXaxis()->SetRangeUser(0.0, 5.);
  cONE->Modified(); 
  cONE->Update();      
  cONE->Print("HistRMSHB.png"); 
  cONE->Clear();     
     
//3 HE
  cHE->Divide(3,1);
  for (int i=1;i<=3;i++) {
     cHE->cd(i); 
     Map_Ampl[2][2][i]->Divide(Map_Ampl[2][2][i],Map_SUB[2][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     sprintf(str,"HE, Depth%d \b", i); 
     Map_Ampl[2][2][i]->SetTitle(str);
     Map_Ampl[2][2][i]->SetXTitle("#eta \b");
     Map_Ampl[2][2][i]->SetYTitle("#phi \b");
     Map_Ampl[2][2][i]->SetZTitle("Rate \b");
     Map_Ampl[2][2][i]->Draw("COLZ");
     Map_Ampl[2][2][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[2][2][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHE->Modified(); 
     cHE->Update(); 
     
  }    
  cHE->Print("MapRateRMSHE.png");
  cHE->Clear();
  
  cONE->Divide(1,1);
  cONE->cd(1);
  gPad->SetGridy();
  gPad->SetGridx();
  gPad->SetLogy(); 
  HistAmpl[2][2]->SetTitleOffset(1.3,"Y");
  HistAmpl[2][2]->SetTitle("HE, All Depth");
  HistAmpl[2][2]->SetXTitle("TS RMS in each event & cell \b");
  HistAmpl[2][2]->SetYTitle("Number of cell-events \b");
  HistAmpl[2][2]->Draw("");
//  HistAmpl[2][2]->GetYaxis()->SetRangeUser(1., 100.);
  HistAmpl[2][2]->GetXaxis()->SetRangeUser(0., 5.);
  cONE->Modified(); 
  cONE->Update();      
  cONE->Print("HistRMSHE.png"); 
  cONE->Clear();  

//+++++++++++++++++++++++++++++  
//Test 3 (2.C) Rate of 34 TS/ All TS   
//+++++++++++++++++++++++++++++
 
  Map_Ampl[3][1][1] = (TH2F*)hfile->Get("h_mapDepth1Ampl047_HB");
  Map_Ampl[3][1][2] = (TH2F*)hfile->Get("h_mapDepth2Ampl047_HB");   
  Map_Ampl[3][2][1] = (TH2F*)hfile->Get("h_mapDepth1Ampl047_HE");
  Map_Ampl[3][2][2] = (TH2F*)hfile->Get("h_mapDepth2Ampl047_HE"); 
  Map_Ampl[3][2][3] = (TH2F*)hfile->Get("h_mapDepth3Ampl047_HE"); 
  
  
  HistAmpl[3][1] = (TH1F*)hfile->Get("h_Ampl_HB");
  HistAmpl[3][2] = (TH1F*)hfile->Get("h_Ampl_HE");

//3 HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i); 
     Map_Ampl[3][1][i]->Divide(Map_Ampl[3][1][i], Map_SUB[1][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     gStyle->SetTitleOffset(0.5, "Y");  
     sprintf(str,"HB, Depth%d \b", i); 
     Map_Ampl[3][1][i]->SetTitle(str);
     Map_Ampl[3][1][i]->SetXTitle("#eta \b");
     Map_Ampl[3][1][i]->SetYTitle("#phi \b");
     Map_Ampl[3][1][i]->SetZTitle("Rate \b");
     Map_Ampl[3][1][i]->Draw("COLZ");
     Map_Ampl[3][1][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[3][1][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHB->Modified(); 
     cHB->Update();
  }    
  cHB->Print("MapRate43TStoAllTSHB.png"); 
  cHB->Clear();
  
  cONE->Divide(1,1);
  cONE->cd(1);
  gPad->SetGridy();
  gPad->SetGridx(); 
  gPad->SetLogy(); 
  HistAmpl[3][1]->SetTitleOffset(1.3,"Y");
  HistAmpl[3][1]->SetTitle("HB, All Depth");
  HistAmpl[3][1]->SetXTitle("Ratio in each event & cell \b");
  HistAmpl[3][1]->SetYTitle("Number of cell-events \b");
  HistAmpl[3][1]->Draw("");
//  HistAmpl[3][1]->GetYaxis()->SetRangeUser(1., 10000000.);
  HistAmpl[3][1]->GetXaxis()->SetRangeUser(0.0, 1.);
  cONE->Modified(); 
  cONE->Update();      
  cONE->Print("Hist43TStoAllTSHB.png"); 
  cONE->Clear();     
     
//2 HE
  cHE->Divide(3,1);
  for (int i=1;i<=3;i++) {
     cHE->cd(i); 
     Map_Ampl[3][2][i]->Divide(Map_Ampl[3][2][i],Map_SUB[2][i], 1, 1, "B"); 
     gPad->SetGridy();
     gPad->SetGridx();
     gPad->SetLogz();
     sprintf(str,"HE, Depth%d \b", i); 
     Map_Ampl[3][2][i]->SetTitle(str);
     Map_Ampl[3][2][i]->SetXTitle("#eta \b");
     Map_Ampl[3][2][i]->SetYTitle("#phi \b");
     Map_Ampl[3][2][i]->SetZTitle("Rate \b");
     Map_Ampl[3][2][i]->Draw("COLZ");
     Map_Ampl[3][2][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_Ampl[3][2][i]->GetZaxis()->SetRangeUser(0.0001, 1.);
     cHE->Modified(); 
     cHE->Update(); 
     
  }    
  cHE->Print("MapRate43TStoAllTSHE.png");
  cHE->Clear();
  
  cONE->Divide(1,1);
  cONE->cd(1);
  gPad->SetGridy();
  gPad->SetGridx();
  gPad->SetLogy(); 
  HistAmpl[3][2]->SetTitleOffset(1.3,"Y");
  HistAmpl[3][2]->SetTitle("HE, All Depth");
  HistAmpl[3][2]->SetXTitle("Ratio in each event & cell \b");
  HistAmpl[3][2]->SetYTitle("Number of cell-events \b");
  HistAmpl[3][2]->Draw("");
//  HistAmpl[3][2]->GetYaxis()->SetRangeUser(1., 10000000.);
  HistAmpl[3][2]->GetXaxis()->SetRangeUser(0., 1.);
  cONE->Modified(); 
  cONE->Update();      
  cONE->Print("Hist43TStoAllTSHE.png"); 
  cONE->Clear();   
  


//======================================================================




//======================================================================
/// Prepare maps of good/bad channels:

    TH2F *Map_ALL = new TH2F("Map","Map", 82, -41, 41, 72, 0, 72);             
    int nx = Map_ALL->GetXaxis()->GetNbins();
    int ny = Map_ALL->GetYaxis()->GetNbins();
    int NBad = 0;
    int NWarn = 0;
    int Eta[3][10000]={0};
    int Phi[3][10000]={0};
    int Sub[3][10000]={0};
    int Depth[3][10000]={0};
    string Comment[3][10000]={""};
    string Text[6]={"","2.A","2.B","2.C","2.D","2.E"};
    int flag_W = 0;
    int flag_B = 0;
    
    for (int i=1;i<=nx;i++) {  //Eta
       for (int j=1;j<=ny;j++) {	// Phi  
          for (int sub=1;sub<=2;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	     int k_min[5]={0,1,1,1,4}; // minimum depth for each subdet
	     int k_max[5]={0,2,3,3,4}; // maximum depth for each subdet	
	     for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
	        if (Map_SUB[sub][k]->GetBinContent(i,j)!=0) {Map_SUB[sub][k]->SetBinContent(i,j,0.5);Map_ALL->SetBinContent(i,j,0.5);}
	     }	
          }
       }
    }   	  
    for (int i=1;i<=nx;i++) {  //Eta
       for (int j=1;j<=ny;j++) {	// Phi  
          for (int sub=1;sub<=2;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	     int k_min[5]={0,1,1,1,4}; // minimum depth for each subdet
	     int k_max[5]={0,2,3,3,4}; // maximum depth for each subdet	
	     for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
	        flag_W = 0;
	        flag_B = 0;		
	        for (int test=1;test<=3;test++) { //Test: 1-2.A, 2-2.B, etc
//Warning		
                   if ((Map_Ampl[test][sub][k]->GetBinContent(i,j) != 0. )&&(Map_Ampl[test][sub][k]->GetBinContent(i,j) < 0.5) )  {
	              Map_ALL->SetBinContent(i,j,0.8);
		      Map_SUB[sub][k]->SetBinContent(i,j,0.8);
		      if (flag_W == 0) {
		         NWarn +=1; 
		         Eta[1][NWarn]=i-41;
		         Phi[1][NWarn]=j-1;
			 Sub[1][NWarn]=sub;
		         Depth[1][NWarn]=k;
		         Comment[1][NWarn]=Text[test]; 
		      } 
		      else Comment[1][NWarn]+=", "+Text[test];
		      flag_W = 1;		      		 
		      cout<<"Map_Ampl["<<test<<"]["<<sub<<"]["<<k<<"]->GetBinContent("<<i<<","<<j<<")= "<<Map_Ampl[test][sub][k]->GetBinContent(i,j)<<endl;
                   }
//Bad			     	                 
                   if (Map_Ampl[test][sub][k]->GetBinContent(i,j) >= 0.5 )  {
	              Map_ALL->SetBinContent(i,j,1.);
		      Map_SUB[sub][k]->SetBinContent(i,j,1.);
		      if (flag_B == 0) {
		         NBad +=1; 
		         Eta[2][NBad]=i-41;
		         Phi[2][NBad]=j-1;
			 Sub[2][NBad]=sub;
		         Depth[2][NBad]=k;
		         Comment[2][NBad]=Text[test];
		      } 
		      else Comment[2][NBad]+=", "+Text[test];
		      flag_B = 1;		 
		      cout<<"Map_Ampl["<<test<<"]["<<sub<<"]["<<k<<"]->GetBinContent("<<i<<","<<j<<")= "<<Map_Ampl[test][sub][k]->GetBinContent(i,j)<<endl;
                   }
		} //end test
             }//end Depth
	  }//end Sub	  	  
       }//end Phi
    }//end Eta            
//HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i);  
     gPad->SetGridy();
     gPad->SetGridx();
//     gPad->SetLogz();
     sprintf(str,"HB, Depth%d \b", i); 
     Map_SUB[1][i]->SetTitle(str);
     Map_SUB[1][i]->SetXTitle("#eta \b");
     Map_SUB[1][i]->SetYTitle("#phi \b");
     Map_SUB[1][i]->Draw("COL");
     Map_SUB[1][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_SUB[1][i]->GetZaxis()->SetRangeUser(0., 1.);
     cHB->Modified(); 
     cHB->Update();      
  }    
  cHB->Print("MAPHB.png"); 
  cHB->Clear();

//HE
  cHE->Divide(3,1);
  for (int i=1;i<=3;i++) {
     cHE->cd(i);  
     gPad->SetGridy();
     gPad->SetGridx();
//     gPad->SetLogz();
     sprintf(str,"HE, Depth%d \b", i); 
     Map_SUB[2][i]->SetTitle(str);
     Map_SUB[2][i]->SetXTitle("#eta \b");
     Map_SUB[2][i]->SetYTitle("#phi \b");
     Map_SUB[2][i]->Draw("COL");
     Map_SUB[2][i]->GetYaxis()->SetRangeUser(0, 72.);
     Map_SUB[2][i]->GetZaxis()->SetRangeUser(0., 1.);
     cHE->Modified(); 
     cHE->Update();      
  }    
  cHE->Print("MAPHE.png"); 
  cHE->Clear();


// ALL SubDet  
  gStyle->SetOptTitle(0);
  TCanvas *cmain = new TCanvas("cmain","MAP",800,800);
  gPad->SetGridy();
  gPad->SetGridx();
//   gPad->SetLogz();
  Map_ALL->SetTitleOffset(1.3,"Y");
  Map_ALL->SetXTitle("#eta \b");
  Map_ALL->SetYTitle("#phi \b");
  Map_ALL->Draw("COL");
  Map_ALL->GetYaxis()->SetRangeUser(0, 72.);
  Map_ALL->GetZaxis()->SetRangeUser(0, 1.);
  cmain->Modified(); 
  cmain->Update();   
  cmain->Print("MAP.png"); 
            
//======================================================================


//======================================================================
// Creating subdetectors  html pages:

     std::string raw_class;  
     int ind = 0;
     
//HB
     ofstream htmlFileHB;
     htmlFileHB.open("HB.html"); 
     htmlFileHB << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFileHB << "<head>"<< std::endl;
     htmlFileHB << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFileHB << "<title> Raw Data Analyser </title>"<< std::endl;
     htmlFileHB << "<style type=\"text/css\">"<< std::endl;
     htmlFileHB << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFileHB << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFileHB << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFileHB << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFileHB << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFileHB << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFileHB << "</style>"<< std::endl;
     htmlFileHB << "<body>"<< std::endl;
     
     htmlFileHB << "<h1> HCAL BARREL, RUN = "<< runnumber <<" </h1>"<< std::endl;
     htmlFileHB << "<br>"<< std::endl;
     htmlFileHB << "<h2> 1.Status HB over all test </h2>"<< std::endl;
     htmlFileHB << "<h3> 1.A.Channel map for each Depth </h3>"<< std::endl;
     htmlFileHB << "<h4> Channel legend: green - good, yelloy - warning, red - bad, white - not applicable or out of range </h4>"<< std::endl;
     htmlFileHB << " <img src=\"MAPHB.png\" />" << std::endl;      
     htmlFileHB << "<br>"<< std::endl;
     
     htmlFileHB << "<h3> 1.B.List of Bad channels </h3>"<< std::endl;
     htmlFileHB << "<table>"<< std::endl;     
     htmlFileHB << "<tr>";
     htmlFileHB << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.A</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.B</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.C</td>"   << std::endl;
     htmlFileHB << "</tr>"   << std::endl;     
   
     for (int i=1;i<=NBad;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[2][i]).find("Phi", Phi[2][i]).find("Depth", Depth[2][i]);
		    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"Warning: More than one line correspond to such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	
	if ((ce.size()>=1)&&(Sub[2][i]==1)) {
	   htmlFileHB << "<tr>"<< std::endl;
           htmlFileHB << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Eta[2][i]<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Phi[2][i]<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Depth[2][i] <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
	   htmlFileHB << raw_class<< Map_Ampl[1][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Map_Ampl[2][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
	   htmlFileHB << raw_class<< Map_Ampl[3][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
	   htmlFileHB << "</tr>" << std::endl;

        ind+=1;
	}
     } 
     htmlFileHB << "</table>" << std::endl;
     htmlFileHB << "<br>"<< std::endl;
     
     htmlFileHB << "<h3> 1.C.List of Warning channels </h3>"<< std::endl;
     htmlFileHB << "<table>"<< std::endl;     
     
     htmlFileHB << "<tr>";
     htmlFileHB << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.A</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.B</td>"   << std::endl;
     htmlFileHB << "<td class=\"s1\" align=\"center\">2.C</td>"   << std::endl;
     htmlFileHB << "</tr>"   << std::endl;     
   
     for (int i=1;i<=NWarn;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[1][i]).find("Phi", Phi[1][i]).find("Depth", Depth[1][i]);
	    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	else if (ce.size()>1) {cout<<"Warning: More than one line correspond to such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	
	if ((ce.size()>=1)&&(Sub[1][i]==1)) {
	   htmlFileHB << "<tr>"<< std::endl;
           htmlFileHB << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Eta[1][i]<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Phi[1][i]<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Depth[1][i] <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFileHB << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
	   htmlFileHB << raw_class<< Map_Ampl[1][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
           htmlFileHB << raw_class<< Map_Ampl[2][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
	   htmlFileHB << raw_class<< Map_Ampl[3][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
	   htmlFileHB << "</tr>" << std::endl;

           ind+=1;
	}
     } 
     htmlFileHB << "</table>" << std::endl;
     htmlFileHB << "<br>"<< std::endl;
   
     htmlFileHB << "<h2> 2.Tests of HB </h2>"<< std::endl;

//HB Test 2A 
         
     htmlFileHB << "<h3> 2.B. CapID errors </h3>"<< std::endl;
     htmlFileHB << "<h4> Rate of CapId failure in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHB << " <img src=\"MapRateCapIDHB.png\" />" << std::endl; 
     htmlFileHB << "<br>"<< std::endl;

//HB Test 2B 
         
     htmlFileHB << "<h3> 2.B. Maximum timeslice position RMS </h3>"<< std::endl;
     htmlFileHB << "<h4> Combined RMS distribution over all events, channel and depth. Legend: Bins more 2.25 correpond to bad RMS </h4>"<< std::endl;
     htmlFileHB << " <img src=\"HistRMSHB.png\" />" << std::endl; 
     htmlFileHB << "<br>"<< std::endl; 
     htmlFileHB << "<h4> Rate of bad RMS (>2.25) in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHB << " <img src=\"MapRateRMSHB.png\" />" << std::endl; 
     htmlFileHB << "<br>"<< std::endl;


//HB Test 2C 
         
     htmlFileHB << "<h3> 2.C. Ratio ADC value sum over Two third and fourth Timeslises (TS) to ADC value sum over All TS for each channel. </h3>"<< std::endl;
     htmlFileHB << "<h4> Combined ratio distribution over all events, channel and depth. Legend: Bins less 0.5 correpond to bad ratio </h4>"<< std::endl;
     htmlFileHB << " <img src=\"Hist43TStoAllTSHB.png\" />" << std::endl; 
     htmlFileHB << "<br>"<< std::endl; 
     htmlFileHB << "<h4> Rate of bad ratio (<0.5) in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHB << " <img src=\"MapRate43TStoAllTSHB.png\" />" << std::endl; 
     htmlFileHB << "<br>"<< std::endl;
    

     htmlFileHB << "</body> " << std::endl;
     htmlFileHB << "</html> " << std::endl;
     htmlFileHB.close();
     

//HE
     ofstream htmlFileHE;
     htmlFileHE.open("HE.html"); 
     htmlFileHE << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFileHE << "<head>"<< std::endl;
     htmlFileHE << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFileHE << "<title> Raw Data Analyser </title>"<< std::endl;
     htmlFileHE << "<style type=\"text/css\">"<< std::endl;
     htmlFileHE << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFileHE << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFileHE << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFileHE << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFileHE << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFileHE << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFileHE << "</style>"<< std::endl;
     htmlFileHE << "<body>"<< std::endl;
     
     htmlFileHE << "<h1> HCAL ENDCAP, RUN = "<< runnumber <<" </h1>"<< std::endl; 
     htmlFileHE << "<br>"<< std::endl;
     htmlFileHE << "<h2> 1. Status HE over all test </h2>"<< std::endl;
     htmlFileHE << "<h3> 1.A. Channel map for each Depth </h3>"<< std::endl;
     htmlFileHE << "<h4> Channel legend: green - good, yelloy - warning, red - bad, white - not applicable or out of range </h4>"<< std::endl;
     htmlFileHE << " <img src=\"MAPHE.png\" />" << std::endl;      
     htmlFileHE << "<br>"<< std::endl;
     
     htmlFileHE << "<h3> 1.B. List of Bad channels </h3>"<< std::endl;
     htmlFileHE << "<table>"<< std::endl;     
     htmlFileHE << "<tr>";
     htmlFileHE << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.A</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.B</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.C</td>"   << std::endl;
     htmlFileHE << "</tr>"   << std::endl;     
     
     ind = 0;
   
     for (int i=1;i<=NBad;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[2][i]).find("Phi", Phi[2][i]).find("Depth", Depth[2][i]);
		    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"Warning: More than one line correspond to such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	
	if ((ce.size()>=1)&&(Sub[2][i]==2)) {
	   htmlFileHE << "<tr>"<< std::endl;
           htmlFileHE << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Eta[2][i]<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Phi[2][i]<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Depth[2][i] <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
	   htmlFileHE << raw_class<< Map_Ampl[1][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Map_Ampl[2][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
	   htmlFileHE << raw_class<< Map_Ampl[3][Sub[2][i]][Depth[2][i]]->GetBinContent(Eta[2][i]+41,Phi[2][i]+1)<<"</td>"<< std::endl;
	   htmlFileHE << "</tr>" << std::endl;

        ind+=1;
	}
     } 
     htmlFileHE << "</table>" << std::endl;
     htmlFileHE << "<br>"<< std::endl;
     
     htmlFileHE << "<h3> 1.C. List of Warning channels </h3>"<< std::endl;
     htmlFileHE << "<table>"<< std::endl;     
     
     htmlFileHE << "<tr>";
     htmlFileHE << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.A</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.B</td>"   << std::endl;
     htmlFileHE << "<td class=\"s1\" align=\"center\">2.C</td>"   << std::endl;
     htmlFileHE << "</tr>"   << std::endl;     
   
     for (int i=1;i<=NWarn;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[1][i]).find("Phi", Phi[1][i]).find("Depth", Depth[1][i]);
	    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"Warning: More than one line correspond to such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	
	if ((ce.size()>=1)&&(Sub[1][i]==2)) {
	   htmlFileHE << "<tr>"<< std::endl;
           htmlFileHE << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Eta[1][i]<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Phi[1][i]<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Depth[1][i] <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFileHE << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
	   htmlFileHE << raw_class<< Map_Ampl[1][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
           htmlFileHE << raw_class<< Map_Ampl[2][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
	   htmlFileHE << raw_class<< Map_Ampl[3][Sub[1][i]][Depth[1][i]]->GetBinContent(Eta[1][i]+41,Phi[1][i]+1)<<"</td>"<< std::endl;
	   htmlFileHE << "</tr>" << std::endl;

           ind+=1;
	}
     } 
     htmlFileHE << "</table>" << std::endl;
     htmlFileHE << "<br>"<< std::endl;

//HE Test 2A 
         
     htmlFileHE << "<h3> 2.B. CapID errors </h3>"<< std::endl;
     htmlFileHE << "<h4> Rate of CapId failure in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHE << " <img src=\"MapRateCapIDHE.png\" />" << std::endl; 
     htmlFileHE << "<br>"<< std::endl;

//HE Test 2B 
         
     htmlFileHE << "<h3> 2.B. Maximum timeslice position RMS </h3>"<< std::endl;
     htmlFileHE << "<h4> Combined RMS distribution aver all events, channel and depth. Legend: Bins more 2.25 correpond to bad RMS </h4>"<< std::endl;
     htmlFileHE << " <img src=\"HistRMSHE.png\" />" << std::endl; 
     htmlFileHE << "<br>"<< std::endl; 
     htmlFileHE << "<h4> Rate of bad RMS (>2.25) in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHE << " <img src=\"MapRateRMSHE.png\" />" << std::endl; 
     htmlFileHE << "<br>"<< std::endl;

//HE Test 2A 
         
     htmlFileHE << "<h3> 2.A. Ratio ADC value sum over Two third and fourth Timeslises (TS) to ADC value sum over All TS for each channel. </h3>"<< std::endl;
     htmlFileHE << "<h4> Combined ratio distribution aver all events, channel and depth. Legend: Bins less 0.5 correpond to bad ratio </h4>"<< std::endl;
     htmlFileHE << " <img src=\"Hist43TStoAllTSHE.png\" />" << std::endl; 
     htmlFileHE << "<br>"<< std::endl; 
     htmlFileHE << "<h4> Rate of bad ratio (<0.5) in each channel for each depth.Channel legend: wthite - good, other colour - bad. </h4>"<< std::endl;
     htmlFileHE << " <img src=\"MapRate43TStoAllTSHE.png\" />" << std::endl; 
     htmlFileHE << "<br>"<< std::endl;
    
    
     htmlFileHE << "</body> " << std::endl;
     htmlFileHE << "</html> " << std::endl; 
     htmlFileHE.close();
     
     
     
//======================================================================





//======================================================================
// Creating main html file:     
     ofstream htmlFile;
     htmlFile.open("MAP.html");  
     htmlFile << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFile << "<head>"<< std::endl;
     htmlFile << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFile << "<title> Raw Data Analyser </title>"<< std::endl;
     htmlFile << "<style type=\"text/css\">"<< std::endl;
     htmlFile << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFile << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFile << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFile << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFile << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFile << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFile << "</style>"<< std::endl;
     htmlFile << "<body>"<< std::endl;
     
     htmlFile << "<h1> Raw Data Analyser, RUN = "<< runnumber <<" </h1>"<< std::endl;
     htmlFile << "<br>"<< std::endl;
     
     htmlFile << "<h2> 1. Status of each subdetector separately </h2>"<< std::endl;   
     htmlFile << "<table width=\"400\">"<< std::endl;
     htmlFile << "<tr>"<< std::endl;
     htmlFile << "  <td><a href=\"file:///afs/cern.ch/work/d/dtlisov/private/Monitoring/CMSSW_5_3_8/src/RecoHcal/HcalPromptAnalysis/test/HB.html\">HB</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"file:///afs/cern.ch/work/d/dtlisov/private/Monitoring/CMSSW_5_3_8/src/RecoHcal/HcalPromptAnalysis/test/HE.html\">HE</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"file:///afs/cern.ch/work/d/dtlisov/private/Monitoring/CMSSW_5_3_8/src/RecoHcal/HcalPromptAnalysis/test/HF.html\">HF</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"file:///afs/cern.ch/work/d/dtlisov/private/Monitoring/CMSSW_5_3_8/src/RecoHcal/HcalPromptAnalysis/test/HO.html\">HO</a></td>"<< std::endl;
     htmlFile << "</tr>"<< std::endl;
     htmlFile << "</table>"<< std::endl;
     htmlFile << "<br>"<< std::endl;    
    
    
     htmlFile << "<h2> 2. HCAL status over all test and subdetectors </h2>"<< std::endl;   
     htmlFile << "<h3> 2.A. Channel map </h3>"<< std::endl;
     htmlFile << "<h4> Channel legend: green - good, yelloy - warning, red - bad, white - not applicable or out of range </h4>"<< std::endl;
     htmlFile << " <img src=\"MAP.png\" />" << std::endl;      
     htmlFile << "<br>"<< std::endl;
     
     htmlFile << "<h3> 2.B. List of Bad channels </h3>"<< std::endl;
     htmlFile << "<table>"<< std::endl;     
     htmlFile << "<tr>";
     htmlFile << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
     htmlFile << "</tr>"   << std::endl;     
   
     ind = 0;
     
     for (int i=1;i<=NBad;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[2][i]).find("Phi", Phi[2][i]).find("Depth", Depth[2][i]);
		    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"Warning: More than one line correspond to such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	
	if (ce.size()>=1) {
	   htmlFile << "<tr>"<< std::endl;
           htmlFile << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFile << raw_class<< Eta[2][i]<<"</td>"<< std::endl;
           htmlFile << raw_class<< Phi[2][i]<<"</td>"<< std::endl;
           htmlFile << raw_class<< Depth[2][i] <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
           htmlFile << raw_class<< Comment[2][i]<<"</td>"<< std::endl;
	   htmlFile << "</tr>" << std::endl;

        ind+=1;
	}
     } 
     htmlFile << "</table>" << std::endl;
     htmlFile << "<br>"<< std::endl;
     
     htmlFile << "<h3> 2.C.List of Warning channels </h3>"<< std::endl;
     htmlFile << "<table>"<< std::endl;     
     htmlFile << "<tr>";
     htmlFile << "<td class=\"s4\" align=\"center\">#</td>"    << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">ETA</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">PHI</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">DEPTH</td>"<< std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RBX</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RM</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">PIXEL</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">RM_FIBER</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">FIBER_CH</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">QIE</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">ADC</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">CRATE</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">DCC</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">SPIGOT</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_FIBER</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_SLOT</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HTR_FPGA</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
     htmlFile << "</tr>"   << std::endl;     
   
     for (int i=1;i<=NWarn;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[1][i]).find("Phi", Phi[1][i]).find("Depth", Depth[1][i]);
	    if (ce.size()==0) {cout<<"Error: No such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"Warning: More than one line correspond to such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	
	if (ce.size()>=1) {
	   htmlFile << "<tr>"<< std::endl;
           htmlFile << "<td class=\"s4\" align=\"center\">" << ind+1 <<"</td>"<< std::endl;
           htmlFile << raw_class<< Eta[1][i]<<"</td>"<< std::endl;
           htmlFile << raw_class<< Phi[1][i]<<"</td>"<< std::endl;
           htmlFile << raw_class<< Depth[1][i] <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RBX <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RM <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].Pixel <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].RMfiber <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].FiberCh <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].QIE <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].ADC<<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].VMECardID <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].dccID <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].Spigot <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].FiberIndex <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].HtrSlot <<"</td>"<< std::endl;
           htmlFile << raw_class<< ce[0].HtrTB <<"</td>"<< std::endl;
           htmlFile << raw_class<< Comment[1][i]<<"</td>"<< std::endl;
	   htmlFile << "</tr>" << std::endl;

           ind+=1;
	}
     } 
    
   
     htmlFile << "</table>" << std::endl;
     htmlFile << "</body> " << std::endl;
     htmlFile << "</html> " << std::endl; 
     htmlFile.close();  
//======================================================================



      
//======================================================================
// Close and delete all possible things:
    hfile->Close();    
//  hfile->Delete();
//  Exit Root
    gSystem->Exit(0);
//======================================================================

}
