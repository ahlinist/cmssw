// How to run:
//root -b -q -l RemoteMonitoringMAP.C+
//root -b -q -l 'RemoteMonitoringMAP.C+("test.root")'

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

void RemoteMonitoringMAP(const char* fname = "test.root")
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

  TH2F *Map2maxTStoAllTSRatioHB[4];
  TH2F *Map2maxTStoAllTSRatioHE[4];
  TCanvas *cHB = new TCanvas("cHB","cHB",1000,400);
  TCanvas *cHE = new TCanvas("cHE","cHE",1500,400);
  
  char *str = (char*)alloca(10000);
      
  Map2maxTStoAllTSRatioHB[1] = (TH2F*)hfile->Get("h_mapDepth1Ampl_HB");
  Map2maxTStoAllTSRatioHB[2] = (TH2F*)hfile->Get("h_mapDepth2Ampl_HB"); 
  Map2maxTStoAllTSRatioHB[3] = (TH2F*)hfile->Get("h_mapDepth3Ampl_HB"); 
  
  Map2maxTStoAllTSRatioHE[1] = (TH2F*)hfile->Get("h_mapDepth1Ampl_HE");
  Map2maxTStoAllTSRatioHE[2] = (TH2F*)hfile->Get("h_mapDepth2Ampl_HE"); 
  Map2maxTStoAllTSRatioHE[3] = (TH2F*)hfile->Get("h_mapDepth3Ampl_HE");   

//HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i);  
     gPad->SetGridy();
     gPad->SetGridx();
//     gPad->SetLogz();
     sprintf(str,"HB, Depth%d \b", i); 
     Map2maxTStoAllTSRatioHB[i]->SetTitle(str);
     Map2maxTStoAllTSRatioHB[i]->SetXTitle("#eta \b");
     Map2maxTStoAllTSRatioHB[i]->SetYTitle("#phi \b");
     Map2maxTStoAllTSRatioHB[i]->SetZTitle("Ratio \b");
     Map2maxTStoAllTSRatioHB[i]->Draw("COLZ");
     Map2maxTStoAllTSRatioHB[i]->GetYaxis()->SetRangeUser(0, 72.);
     Map2maxTStoAllTSRatioHB[i]->GetZaxis()->SetRangeUser(0., 1.);
     cHB->Modified(); 
     cHB->Update(); 
     
  }    
  cHB->Print("Map2maxTStoAllTSRatioHB.png"); 
  cHB->Clear();

//HE
  cHE->Divide(3,1);
  for (int i=1;i<=3;i++) {
     cHE->cd(i);  
     gPad->SetGridy();
     gPad->SetGridx();
//     gPad->SetLogz();
     sprintf(str,"HE, Depth%d \b", i); 
     Map2maxTStoAllTSRatioHE[i]->SetTitle(str);
     Map2maxTStoAllTSRatioHE[i]->SetXTitle("#eta \b");
     Map2maxTStoAllTSRatioHE[i]->SetYTitle("#phi \b");
     Map2maxTStoAllTSRatioHE[i]->SetZTitle("Ratio \b");
     Map2maxTStoAllTSRatioHE[i]->Draw("COLZ");
     Map2maxTStoAllTSRatioHE[i]->GetYaxis()->SetRangeUser(0, 72.);
     Map2maxTStoAllTSRatioHE[i]->GetZaxis()->SetRangeUser(0., 1.);
     cHE->Modified(); 
     cHE->Update(); 
     
  }    
  cHE->Print("Map2maxTStoAllTSRatioHE.png"); 
  cHE->Clear();

//======================================================================




//======================================================================
/// Prepare maps of good/bad channels:

    TH2F *Map = new TH2F("Map","Map", 82, -41, 41, 72, 0, 72);    
    TH2F *MapHB[4];
    TH2F *MapHE[4];         
    int nx = Map->GetXaxis()->GetNbins();
    int ny = Map->GetYaxis()->GetNbins();
    int NBad = 0;
    int NWarn = 0;
    int Eta[3][10000]={0};
    int Phi[3][10000]={0};
    int Depth[3][10000]={0};
    string Comment[3][10000]={""};
  
    for (int k=1;k<=3;k++) { 
       sprintf(str,"MapHBDepth%d \b", k);
       MapHB[k]=(TH2F*) Map2maxTStoAllTSRatioHB[k]->Clone(str) ;
       sprintf(str,"MapHEDepth%d \b", k);
       MapHE[k]=(TH2F*) Map2maxTStoAllTSRatioHE[k]->Clone(str) ;
    }
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {	  
//HB
	  for (int k=1;k<=2;k++) {
	     if (MapHB[k]->GetBinContent(i,j)!=0) {MapHB[k]->SetBinContent(i,j,0.5);Map->SetBinContent(i,j,0.5);}
	     if((Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) > 0.2 )&&(Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) < 0.3) )  {
	         Map->SetBinContent(i,j,0.8);
		 MapHB[k]->SetBinContent(i,j,0.8);
		 NWarn +=1; 
		 Eta[1][NWarn]=i-41;
		 Phi[1][NWarn]=j-1;
		 Depth[1][NWarn]=k;
		 Comment[1][NWarn]="Test 2.A.: Ratio less 0.3";		 
             }	     
//	     cout<<"Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) "<<Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j)<<endl;
             if((Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) != 0. )&& (Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) < 0.2 ))  {
	        Map->SetBinContent(i,j,1.0);
		MapHB[k]->SetBinContent(i,j,1.0);
	        NBad +=1; 
		Eta[2][NBad]=i-41;
		Phi[2][NBad]=j-1;
		Depth[2][NBad]=k;
		Comment[2][NBad]="Test 2.A.: Ratio less 0.2";
		cout<<"Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) "<<Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j)<<endl;		
	     }	     	  
          }
//HE
	  for (int k=1;k<=3;k++) {
	     if (MapHE[k]->GetBinContent(i,j)!=0) {MapHE[k]->SetBinContent(i,j,0.5);Map->SetBinContent(i,j,0.5);}
	     if((Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j) > 0.2 )&&(Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j) < 0.3) )  {
	         Map->SetBinContent(i,j,0.8);
		 MapHE[k]->SetBinContent(i,j,0.8);
		 NWarn +=1; 
		 Eta[1][NWarn]=i-41;
		 Phi[1][NWarn]=j-1;
		 Depth[1][NWarn]=k;
		 Comment[1][NWarn]="Test 2.A.: Ratio less 0.3";		 
             }	     
//	     cout<<"Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j) "<<Map2maxTStoAllTSRatioHB[k]->GetBinContent(i,j)<<endl;
             if((Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j) != 0. )&& (Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j) < 0.2 ))  {
	        Map->SetBinContent(i,j,1.0);
		MapHE[k]->SetBinContent(i,j,1.0);
	        NBad +=1; 
		Eta[2][NBad]=i-41;
		Phi[2][NBad]=j-1;
		Depth[2][NBad]=k;
		Comment[2][NBad]="Test 2.A.: Ratio less 0.2";
		cout<<"Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j) "<<Map2maxTStoAllTSRatioHE[k]->GetBinContent(i,j)<<endl;		
	     }	     	  
          }
//HF
//HO	  	  
      }
    }
//++++++++++++++++++++++++++++++++++++    
// TEST    
      Map->SetBinContent(1,4,0.7);
      NWarn += 1;
      Eta[1][NWarn]=1-41; Phi[1][NWarn]=7-1;  Depth[1][NWarn]=1;
      Comment[1][NWarn]="TEST";
      Map->SetBinContent(80,56,0.7);
      NWarn += 1;
      Eta[1][NWarn]=80-41; Phi[1][NWarn]=57-1; Depth[1][NWarn]=2;
      Comment[1][NWarn]="TEST";
       
      Map->SetBinContent(15,10,1.);
      NBad += 1;
      Eta[2][NBad]=15-41; Phi[2][NBad]=10-1; Depth[2][NBad]=2;
      Comment[2][NBad]="TEST";
      Map->SetBinContent(56,67,1.);
      NBad += 1;
      Eta[2][NBad]=56-41; Phi[2][NBad]=67-1; Depth[2][NBad]=4;
      Comment[2][NBad]="TEST";
      Map->SetBinContent(30,30,1.);
      NBad += 1;
      Eta[2][NBad]=30-41; Phi[2][NBad]=30-1; Depth[2][NBad]=1;
      Comment[2][NBad]="TEST";
      
//+++++++++++++++++++++++++++++++++++      
            
//HB
  cHB->Divide(2,1);
  for (int i=1;i<=2;i++) {
     cHB->cd(i);  
     gPad->SetGridy();
     gPad->SetGridx();
//     gPad->SetLogz();
     sprintf(str,"HB, Depth%d \b", i); 
     MapHB[i]->SetTitle(str);
     MapHB[i]->SetXTitle("#eta \b");
     MapHB[i]->SetYTitle("#phi \b");
     MapHB[i]->Draw("COL");
     MapHB[i]->GetYaxis()->SetRangeUser(0, 72.);
     MapHB[i]->GetZaxis()->SetRangeUser(0., 1.);
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
     MapHE[i]->SetTitle(str);
     MapHE[i]->SetXTitle("#eta \b");
     MapHE[i]->SetYTitle("#phi \b");
     MapHE[i]->Draw("COL");
     MapHE[i]->GetYaxis()->SetRangeUser(0, 72.);
     MapHE[i]->GetZaxis()->SetRangeUser(0., 1.);
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
  Map->SetXTitle("#eta \b");
  Map->SetYTitle("#phi \b");
  Map->Draw("COL");
  Map->GetYaxis()->SetRangeUser(0, 72.);
  Map->GetZaxis()->SetRangeUser(0, 1.);
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
     htmlFileHB << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
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
	
	if ((ce.size()>=1)&&(abs(Eta[2][i])<=16)) {
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
           htmlFileHB << raw_class<< Comment[2][i]<<"</td>"<< std::endl;
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
     htmlFileHB << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
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
	
	if ((ce.size()>=1&&(abs(Eta[1][i])<=16))) {
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
           htmlFileHB << raw_class<< Comment[1][i]<<"</td>"<< std::endl;
	   htmlFileHB << "</tr>" << std::endl;

           ind+=1;
	}
     } 
     htmlFileHB << "</table>" << std::endl;
     htmlFileHB << "<br>"<< std::endl;

//HB Test 2A 
         
     htmlFileHB << "<h2> 2.Tests</h2>"<< std::endl;
     htmlFileHB << "<h3> 2.A. Test: ADC value sum over Two maximum Timeslises (TS) devide to ADC value sum over All maximum Timeslises (TS) > 0.3  for each channel </h3>"<< std::endl;
     htmlFileHB << " <img src=\"Map2maxTStoAllTSRatioHB.png\" />" << std::endl; 
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
     htmlFileHE << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
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
	
	if ((ce.size()>=1)&&(abs(Eta[2][i])<=29)&&(abs(Eta[2][i])>=17)) {
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
           htmlFileHE << raw_class<< Comment[2][i]<<"</td>"<< std::endl;
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
     htmlFileHE << "<td class=\"s1\" align=\"center\">Comment</td>"   << std::endl;
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
	
	if ((ce.size()>=1)&&(abs(Eta[2][i])<=29)&&(abs(Eta[1][i])>=17)) {
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
           htmlFileHE << raw_class<< Comment[1][i]<<"</td>"<< std::endl;
	   htmlFileHE << "</tr>" << std::endl;

           ind+=1;
	}
     } 
     htmlFileHE << "</table>" << std::endl;
     htmlFileHE << "<br>"<< std::endl;
//HE Test 2A      

     htmlFileHE << "<h2> 2.Tests</h2>"<< std::endl;;
     htmlFileHE << "<h3> 2.A. Test: ADC value sum over Two maximum Timeslises (TS) devide to ADC value sum over All maximum Timeslises (TS) > 0.3 for each channel</h3>"<< std::endl;
     htmlFileHE << " <img src=\"Map2maxTStoAllTSRatioHE.png\" />" << std::endl; 
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
     htmlFile << "  <td><a href=\"http://HF.html\">HF</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://HO.html\">HO</a></td>"<< std::endl;
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
