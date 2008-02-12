// -*- C++ -*-
//
// Package:    HLTMuonDQMClient
// Class:      HLTMuonDQMClient
// 
/**\class HLTMuonDQMClient HLTMuonDQMClient.cc HLTriggerOffline/HLTMuonDQMClient/src/HLTMuonDQMClient.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Muriel VANDER DONCKT *:0
//         Created:  Wed Dec 12 09:55:42 CET 2007
// $Id: HLTMuonDQMClient.cc,v 1.3 2008/02/11 17:55:33 muriel Exp $
//
//



#include "DQM/HLTObjects/interface/HLTMuonDQMClient.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TStyle.h"

using namespace std;
using namespace edm;
using namespace reco;
#include "TApplication.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
//
// constructors and destructor
//
HLTMuonDQMClient::HLTMuonDQMClient( const edm::ParameterSet& ps )
{
  writeHisto_ = ps.getUntrackedParameter<bool>("writeHisto", true);
  writeHTML_  = ps.getUntrackedParameter<bool>("writeHTML", true);
  dumpRate_   = ps.getUntrackedParameter<int>("dumpRate", 10);
  outputFileName_ = ps.getUntrackedParameter<string>("outputFileName", "HLTMonMuon");
  rootFolder_ = ps.getUntrackedParameter<string>("rootFolder", "HLT/HLTMonMuon/");
  htmlDir_    = ps.getUntrackedParameter<string>("htmlDir","HLT/HLTMonMuon/");
  htmlName_   = ps.getUntrackedParameter<string>("htmlName","muonHLT.html");  
  sta_        = ps.getUntrackedParameter<bool>("RunStandalone", false);
  x11_        = ps.getUntrackedParameter<bool>("InitializeX11", false);

  count_ = 0;
  run_ = -1;
  init_ = false;

  dbe_ = Service<DaqMonitorBEInterface>().operator->();

  if (x11_) {
    // If this block is executed, the x11 interface is initialized
    // and the created canvases are displayed at run time.
    int argc=0;
    char *argv[1];
    theApp_= new TApplication("App", &argc, argv);
  }
  else
    theApp_= 0;

  
}


HLTMuonDQMClient::~HLTMuonDQMClient()
{
  if (theApp_) delete theApp_;
  
}


//--------------------------------------------------------
void HLTMuonDQMClient::beginJob(const EventSetup& context){
  if (dbe_) {
    dbe_->setVerbose(1);
    dbe_->setCurrentFolder("HLT/HLTMonMuon");
  }

}
void HLTMuonDQMClient::setup(){
  init_ = true;
}

void HLTMuonDQMClient::cleanup() {

  if (sta_) return;

  if (dbe_) {
    dbe_->setCurrentFolder("HLT/HLTMonMuon");
  }

  init_ = false;

}

// ----------------------------------------------------------
void HLTMuonDQMClient::analyze(const Event& e, 
			 const EventSetup& context )
{  
 if (! init_) this->setup();

  int runNum = e.id().run();
  Char_t runNum_s[50];

  if (runNum != run_) { 

    if (run_ > 0) {

      sprintf(runNum_s, "%08d", run_);
      outputFile_ = htmlDir_+"/"+runNum_s+"/"+outputFileName_+"_"+runNum_s+".root";

      if (writeHTML_) {
	doQT();
	htmlOutput(run_, htmlDir_, htmlName_);
      }

      if (writeHisto_) dbe_->save(outputFile_);
    }

    run_ = runNum; 
    count_ = 0;

    sprintf(runNum_s, "%08d", run_);
    outputFile_ = htmlDir_+"/"+runNum_s+"/"+outputFileName_+"_"+runNum_s+".root";
  }

  count_++;

  if ((count_ % dumpRate_) == 0) {
    if (writeHTML_) {
      doQT();
      htmlOutput(runNum, htmlDir_, htmlName_);
    }
    if (writeHisto_) dbe_->save(outputFile_);
  }
 
}

void HLTMuonDQMClient::doQT(){
  Char_t tit[512];
  MonitorElementT<TNamed>* meT;
  if (!dbe_) {
    return;
  }
  for ( unsigned level=2; level < 4 ; level++){
    sprintf(tit, "%sLevel%i/HLTMuonL%i_NMu",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hNMu_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_pt",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hPt_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_ptlx",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hPtlx_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_eta",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hEta_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_phi",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hPhi_[level-2]= dynamic_cast<TH1F*> (meT->operator->());
    
    sprintf(tit, "%sLevel%i/HLTMuonL%i_drphi",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hDrphi_[level-2]= dynamic_cast<TProfile*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_dzeta",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hDzeta_[level-2]= dynamic_cast<TProfile*> (meT->operator->());    

    sprintf(tit, "%sLevel%i/HLTMuonL%i_etaphi",rootFolder_.c_str(),level,level); 
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hEtaphi_[level-2]= dynamic_cast<TH2F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_ptphi",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hPtphi_[level-2]= dynamic_cast<TH2F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_pteta",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hPteta_[level-2]= dynamic_cast<TH2F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_dr",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hDr_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_dz",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hDz_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_err0",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hErr0_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_nhit",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hNhit_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_iso",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hIso_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_DiMuMass",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hDimumass_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/HLTMuonL%i_charge",rootFolder_.c_str(),level,level);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
    hQ_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    if (level == 2){
      sprintf(tit, "%sLevel%i/HLTMuonL%i_ptres",rootFolder_.c_str(),level,level);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
      hPtres_= dynamic_cast<TH1F*> (meT->operator->());
      
      sprintf(tit, "%sLevel%i/HLTMuonL%i_etares",rootFolder_.c_str(),level,level);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
      hEtares_= dynamic_cast<TH1F*> (meT->operator->());
      
      sprintf(tit, "%sLevel%i/HLTMuonL%i_phires",rootFolder_.c_str(),level,level);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
      hPhires_= dynamic_cast<TH1F*> (meT->operator->());

      sprintf(tit, "%sLevel%i/HLTMuonL%i_etareseta",rootFolder_.c_str(),level,level);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
      hEtareseta_= dynamic_cast<TProfile*> (meT->operator->());
      
      sprintf(tit, "%sLevel%i/HLTMuonL%i_phiresphi",rootFolder_.c_str(),level,level);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(dbe_->get(tit));
      hPhiresphi_= dynamic_cast<TProfile*> (meT->operator->());
    }
  } 
}

void HLTMuonDQMClient::htmlOutput(int run, string htmlDir, string htmlName) {
   cout<<"Going to output HLTMuonDQMClient html ..."<<endl;

  Char_t run_s[50];
  sprintf(run_s, "%08d", run); 
  htmlDir = htmlDir+"/"+run_s;
  system(("/bin/mkdir -m 777 -p " + htmlDir).c_str());
  cout<<"opening to html"<<endl;
  ofstream htmlFile;   
  htmlFile.open((htmlDir+"/"+htmlName).c_str()); 
  cout<<"file open"<<endl;
  // html page header
  htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
  htmlFile << "<html>  " << endl;
  htmlFile << "<head>  " << endl;
  htmlFile << "  <meta content=\"text/html; charset=UTF-8\"  " << endl;
  htmlFile << " http-equiv=\"content-type\">  " << endl;
  htmlFile << "  <title>HLT Muon DQM </title> " << endl;
  htmlFile << "</head>  " << endl;
  htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
  htmlFile << "<body>  " << endl;
  htmlFile << "<a name=""top""></a>" << endl;
  htmlFile << "<h2>Run Number / Num of Analyzed events :&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">" << run << "&nbsp;&nbsp;&nbsp;/&nbsp;&nbsp;&nbsp;"<< count_ <<"</span></h2>" << endl;
  htmlFile << "<h2>Monitoring task :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
  htmlFile << " style=\"color: rgb(0, 0, 153);\">Level 2 and 3 Track Parameters</span></h2> " << endl;
  htmlFile << "<hr>" << endl;
  cout <<"html part done"<<endl;
  // Plot Occupancy
  string histName;
  gROOT->SetStyle("Plain");
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.15);
  gStyle->SetPalette(1, 0);
  gStyle->SetGridStyle(1);
  cout<<"About to draw canvas 2"<<endl;
  TCanvas *cMuL2 = new TCanvas("cMuL2", "cMuL2", 1200, 1800);
  gStyle->SetOptStat(111110);
  cMuL2->Divide(4,6);
  unsigned pad=0;
  if (hNMu_[0] == 0)return;
  cMuL2->cd(++pad); 
  gPad->SetLogy();
  hNMu_[0]->Draw();
  cMuL2->cd(++pad);
  gPad->SetLogy();
  hQ_[0]->Draw();
  gPad->Update();  
  cMuL2->cd(++pad);
  gPad->SetLogy(0);
  hPt_[0]->Draw();
  cMuL2->cd(++pad);
  hPtlx_[0]->Draw();
  cMuL2->cd(++pad);
  hPtres_->Draw();
  cMuL2->cd(++pad);
  hPtphi_[0]->Draw("colz");
  cMuL2->cd(++pad);
  hPteta_[0]->Draw("colz");
   cMuL2->cd(++pad);
  hEta_[0]->Draw();
  cMuL2->cd(++pad);
  hEtares_->Draw();
  cMuL2->cd(++pad);
  hEtareseta_->Draw();
  cMuL2->cd(++pad);
  hPhi_[0]->Draw();
  cMuL2->cd(++pad);
  hPhires_->Draw();
  cMuL2->cd(++pad);
  hPhiresphi_->Draw();
  cMuL2->cd(++pad);
  gStyle->SetOptStat(0);
  hEtaphi_[0]->Draw("colz");
  gStyle->SetOptStat(111110);
  cMuL2->cd(++pad);
  hDr_[0]->Draw();
  cMuL2->cd(++pad);
  hDrphi_[0]->Draw();
  cMuL2->cd(++pad);
  hDz_[0]->Draw();
  cMuL2->cd(++pad);
  hDzeta_[0]->Draw();
  cMuL2->cd(++pad);
  hErr0_[0]->Draw();
  cMuL2->cd(++pad);
  hNhit_[0]->Draw();
  cMuL2->cd(++pad);
  gPad->SetLogy();
  hIso_[0]->Draw();
  cMuL2->cd(++pad);
  gPad->SetLogy();
  hDimumass_[0]->Draw();
  gPad->Update();
  histName = htmlDir+"/Level2.png";
  cMuL2->SaveAs(histName.c_str()); 
  cout << "saved file "<<endl;
  TCanvas *cMuL3 = new TCanvas("cMuL3", "cMuL3", 1200, 1500);
  cMuL3->Divide(4,5);
  pad=0;
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hNMu_[1]->Draw();
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hQ_[1]->Draw();
  gPad->Update();
  cMuL3->cd(++pad);
  gPad->SetLogy(0);
  hPt_[1]->Draw();
  cMuL3->cd(++pad);
  hPtlx_[1]->Draw();
  cMuL3->cd(++pad);
  hPtphi_[1]->Draw("colz");
  cMuL3->cd(++pad);
  hPteta_[1]->Draw("colz");
  cMuL3->cd(++pad);
  hEta_[1]->Draw();
  cMuL3->cd(++pad);
  hPhi_[1]->Draw();
  cMuL3->cd(++pad);
  gStyle->SetOptStat(0);
  hEtaphi_[1]->Draw("colz");
  gStyle->SetOptStat(111110);
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hDr_[1]->Draw();
  cMuL3->cd(++pad);
  hDrphi_[1]->Draw();
  /*
  double dphiMax=hDrphi_[1]->GetMaximumStored();
  int nbins=hDrphi_[1]->GetNbinsX();
  double phiMax=-3.15+(hDrphi_[1]->GetMaximumBin())*(6.283/nbins);
  double beamSpotX=dphiMax*cos(phiMax);
  double beamSpotY=dphiMax*sin(phiMax);
  TPaveText *pav=new TPaveText(0.11,0.8,0.41,0.89,"tl");
  char text[512];
  sprintf(text,"BeamSpot x=%f  y=%f",beamSpotX, beamSpotY);
  pav->SetFillColor(0);
  pav->AddText(0,0,text);
  pav->AppendPad();
  pav->Draw(); */
  gPad->Update();
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hDz_[1]->Draw();
  cMuL3->cd(++pad);
  hDzeta_[1]->Draw();
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hErr0_[1]->Draw();
  cMuL3->cd(++pad);
  hNhit_[1]->Draw();
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hIso_[1]->Draw();
  cMuL3->cd(++pad);
  gPad->SetLogy();
  hDimumass_[1]->Draw();
  histName = htmlDir+"/Level3.png";
  gPad->Update();
  cMuL3->SaveAs(histName.c_str()); 
  delete cMuL2;
  delete cMuL3; 
  delete pav;
  htmlFile << "<h2>Level 2</h2>" << endl;
  htmlFile << "<img src=\"Level2.png\"></img>" << endl;
  htmlFile << "<hr>" << endl;
  htmlFile << "<h2>Level 3</h2>" << endl;
  htmlFile << "<img src=\"Level3.png\"></img>" << endl;
 // html page footer
  htmlFile << "</body> " << endl;
  htmlFile << "</html> " << endl;

  
}


//--------------------------------------------------------
void HLTMuonDQMClient::endJob(){
  Char_t runNum_s[50];
  sprintf(runNum_s, "%08d", run_);
  outputFile_ = htmlDir_+"/"+runNum_s+"/"+outputFileName_+"_"+runNum_s+".root";

   if (writeHTML_) {
    doQT();
    htmlOutput(run_, htmlDir_, htmlName_);
   }

  if (writeHisto_) dbe_->save(outputFile_);
  dbe_->rmdir("HLT/HLTMonMuon");  

  if ( init_ ) this->cleanup();
}
