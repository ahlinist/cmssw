// How to run:
//root -b -q -l RemoteMonitoringMAP.C+

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

void RemoteMonitoringMAP()
{
    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);  
    gStyle->SetOptTitle(0);  

//======================================================================
// Connect the input file and get the 2-d histogram in memory
//    TFile *hfile= new TFile("GlobalHist.root", "READ");
//    TFile *hfile= new TFile("test.root", "READ");
    TFile *hfile= new TFile("testNZS.root", "READ");            
//======================================================================

//======================================================================
/// Prepare map of good/bad channels:
    TCanvas *c = new TCanvas("c","MAP",400,400);
    TH2F *Map = new TH2F("Map"," ", 82, -41, 41, 72, 0, 72);
    int nx = Map->GetXaxis()->GetNbins();
    int ny = Map->GetYaxis()->GetNbins();
    int NBad = 0;
    int NWarn = 0;
    int Eta[3][10000]={0};
    int Phi[3][10000]={0};
    int Depth[3][10000]={0};
    for (int i=1;i<=nx;i++) {
      for (int j=1;j<=ny;j++) {
	  Map->SetBinContent(i,j,0.5);
//	  if(ccc1 > 1.0 )  {Diffe_Depth1_HB->SetBinContent(i,j,0.8); NWern +=1; Eta[1][NWern]=i-41; Phi[1][NWern]=j-1; }	  
//	  if(ccc1 > 2.0 )  {Diffe_Depth1_HB->SetBinContent(i,j,1.); NBad +=1; Eta[2][NBad]=i-41; Phi[2][NBad]=j-1;}
      }
    }
      Map->SetBinContent(1,4,0.7);
      Map->SetBinContent(80,56,0.7);
      Map->SetBinContent(15,10,1.);
      Map->SetBinContent(56,67,1.);
      Map->SetBinContent(30,30,1.);
      NWarn = 2;
      NBad = 3;
      Eta[1][1]=1-41; Phi[1][1]=4-1;  Depth[1][1]=1; 
      Eta[1][2]=80-41; Phi[1][2]=56-1; Depth[1][2]=2; 
      
      Eta[2][1]=15-41; Phi[2][1]=10-1; Depth[2][1]=2; 
      Eta[2][2]=56-41; Phi[2][2]=67-1; Depth[2][2]=4; 
      Eta[2][3]=30-41; Phi[2][3]=30-1; Depth[2][3]=1; 
      
      gPad->SetGridy();
      gPad->SetGridx();
//      gPad->SetLogz();
      Map->SetXTitle("#eta \b");
      Map->SetYTitle("#phi \b");
      Map->Draw("COL");
      Map->GetYaxis()->SetRangeUser(0, 72.);
      Map->GetZaxis()->SetRangeUser(0, 1.);
      c->Modified(); 
      c->Update();     
      c->Print("MAP.png");

//======================================================================
// Creating html file:     
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
     
     htmlFile << "<h3> Raw Data Analyser </h3>"<< std::endl;
     htmlFile << " <img src=\"MAP.png\" />" << std::endl; 
     
     htmlFile << "<h3> Bad channels </h3>"<< std::endl;
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

     std::string raw_class;
  
     int ind = 0;
   
     for (int i=1;i<=NBad;i++) {
        if((ind%2)==1){
           raw_class="<td class=\"s2\" align=\"center\">";
        }else{
           raw_class="<td class=\"s3\" align=\"center\">";
        }
        const CellDB db;
        const CellDB ce = db.find("Eta", Eta[2][i]).find("Phi", Phi[2][i]).find("Depth", Depth[2][i]);
		    if (ce.size()==0) {cout<<"No such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"More than one line correspond to such Eta="<< Eta[2][i] <<", Phi="<< Phi[2][i] <<", Depth="<< Depth[2][i] <<" in database"<<endl;}
	
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
           htmlFile << raw_class<< "TEST"<<"</td>"<< std::endl;
	   htmlFile << "</tr>" << std::endl;

        ind+=1;
	}
     } 
 
     htmlFile << "</table>" << std::endl;
     
     htmlFile << "<h3> Warning channels </h3>"<< std::endl;
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
	    if (ce.size()==0) {cout<<"No such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	else if (ce.size()>1) { cout<<"More than one line correspond to such Eta="<< Eta[1][i] <<", Phi="<< Phi[1][i] <<", Depth="<< Depth[1][i] <<" in database"<<endl;}
	
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
           htmlFile << raw_class<< "TEST"<<"</td>"<< std::endl;
	   htmlFile << "</tr>" << std::endl;

           ind+=1;
	}
     } 
    
   
     htmlFile << "</table>" << std::endl;
     htmlFile << "</body> " << std::endl;
     htmlFile << "</html> " << std::endl;    
//======================================================================
// Close and delete all possible things:
    hfile->Close();
    htmlFile.close();
//  hfile->Delete();
//  Exit Root
    gSystem->Exit(0);
//======================================================================

}
