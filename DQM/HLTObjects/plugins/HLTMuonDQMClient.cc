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
// $Id: HLTMuonDQMClient.cc,v 1.1 2008/01/24 15:31:01 muriel Exp $
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
//
// constructors and destructor
//
HLTMuonDQMClient::HLTMuonDQMClient( const edm::ParameterSet& ps )
{
  writeHisto_ = ps.getUntrackedParameter<bool>("writeHisto", true);
  writeHTML_  = ps.getUntrackedParameter<bool>("writeHTML", true);
  dumpRate_   = ps.getUntrackedParameter<int>("dumpRate", 10);
  outputFileName_ = ps.getUntrackedParameter<string>("outputFileName", "HLTMuon");
  rootFolder_ = ps.getUntrackedParameter<string>("rootFolder", "HLTMuon/");
  htmlDir_    = ps.getUntrackedParameter<string>("htmlDir","HLT/Objects/Muon/");
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
    dbe_->setCurrentFolder("HLTMuon/");
    // dbe_->rmdir("HLTMuon");
  }

}
void HLTMuonDQMClient::setup(){
  init_ = true;
}

void HLTMuonDQMClient::cleanup() {

  if (sta_) return;

  if (dbe_) {
    dbe_->setCurrentFolder("HLTMuon");
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
    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_NMu",rootFolder_.c_str(),level,level);
    MonitorElement *meNMu= dbe_->get(tit);
    if (!meNMu) break;
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meNMu);
    hNMu_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_pt",rootFolder_.c_str(),level,level);
    MonitorElement *mePt= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(mePt);
    hPt_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_ptlx",rootFolder_.c_str(),level,level);
    MonitorElement *mePtlx= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(mePtlx);
    hPtlx_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_eta",rootFolder_.c_str(),level,level);
    MonitorElement *meEta= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meEta);
    hEta_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_phi",rootFolder_.c_str(),level,level);
    MonitorElement *mePhi= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(mePhi);
    hPhi_[level-2]= dynamic_cast<TH1F*> (meT->operator->());
    
    if (level == 2){
      sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_ptres",rootFolder_.c_str(),level,level);
      MonitorElement *mePtres= dbe_->get(tit);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(mePtres);
      hPtres_= dynamic_cast<TH1F*> (meT->operator->());
      
      sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_etares",rootFolder_.c_str(),level,level);
      MonitorElement *meEtares= dbe_->get(tit);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(meEtares);
      hEtares_= dynamic_cast<TH1F*> (meT->operator->());
      
      sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_phires",rootFolder_.c_str(),level,level);
      MonitorElement *mePhires= dbe_->get(tit);
      meT = dynamic_cast<MonitorElementT<TNamed>*>(mePhires);
      hPhires_= dynamic_cast<TH1F*> (meT->operator->());
    }

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_etaphi",rootFolder_.c_str(),level,level);
    MonitorElement *meEtaphi= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meEtaphi);
    hEtaphi_[level-2]= dynamic_cast<TH2F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_dr",rootFolder_.c_str(),level,level);
    MonitorElement *meDr= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meDr);
    hDr_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_dz",rootFolder_.c_str(),level,level);
    MonitorElement *meDz= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meDz);
    hDz_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_err0",rootFolder_.c_str(),level,level);
    MonitorElement *meErr0= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meErr0);
    hErr0_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_nhit",rootFolder_.c_str(),level,level);
    MonitorElement *meNhit= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meNhit);
    hNhit_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_iso",rootFolder_.c_str(),level,level);
    MonitorElement *meIso= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meIso);
    hIso_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_DiMuMass",rootFolder_.c_str(),level,level);
    MonitorElement *meDimumass= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meDimumass);
    hDimumass_[level-2]= dynamic_cast<TH1F*> (meT->operator->());

    sprintf(tit, "%sLevel%i/Objects/HLTMuonL%i_charge",rootFolder_.c_str(),level,level);
    MonitorElement *meQ= dbe_->get(tit);
    meT = dynamic_cast<MonitorElementT<TNamed>*>(meQ);
    hQ_[level-2]= dynamic_cast<TH1F*> (meT->operator->());
  } 
}

void HLTMuonDQMClient::htmlOutput(int run, string htmlDir, string htmlName) {
   cout<<"Going to output HLTMuonDQMClient html ..."<<endl;

  Char_t run_s[50];
  sprintf(run_s, "%08d", run); 
  htmlDir = htmlDir+"/"+run_s;
  system(("/bin/mkdir -m 777 -p " + htmlDir).c_str());

  ofstream htmlFile;   
  htmlFile.open((htmlDir+"/"+htmlName).c_str()); 

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

  // Plot Occupancy
  string histName;
  gROOT->SetStyle("Plain");
  gStyle->SetStatW(0.3);
  gStyle->SetStatH(0.15);
  gStyle->SetPalette(1, 0);
  gStyle->SetGridStyle(1);

  TCanvas *cMuL2 = new TCanvas("cMuL2", "cMuL2", 1200, 1200);
  gStyle->SetOptStat(111110);
  cMuL2->Divide(4,4);
  unsigned pad=0;
  cMuL2->cd(++pad);
  hNMu_[0]->Draw();
  cMuL2->cd(++pad);
  hQ_[0]->Draw();
  cMuL2->cd(++pad);
  hPt_[0]->Draw();
  cMuL2->cd(++pad);
  hPtlx_[0]->Draw();
  cMuL2->cd(++pad);
  hPtres_->Draw();
  cMuL2->cd(++pad);
  hEta_[0]->Draw();
  cMuL2->cd(++pad);
  hEtares_->Draw();
  cMuL2->cd(++pad);
  hPhi_[0]->Draw();
  cMuL2->cd(++pad);
  hPhires_->Draw();
  cMuL2->cd(++pad);
  gStyle->SetOptStat(0);
  hEtaphi_[0]->Draw("colz");
  gStyle->SetOptStat(111110);
  cMuL2->cd(++pad);
  hDr_[0]->Draw();
  cMuL2->cd(++pad);
  hDz_[0]->Draw();
  cMuL2->cd(++pad);
  hErr0_[0]->Draw();
  cMuL2->cd(++pad);
  hNhit_[0]->Draw();
  cMuL2->cd(++pad);
  hIso_[0]->Draw();
  cMuL2->cd(++pad);
  hDimumass_[0]->Draw();
  histName = htmlDir+"/Level2.png";
  cMuL2->SaveAs(histName.c_str());  

  TCanvas *cMuL3 = new TCanvas("cMuL3", "cMuL3", 1200, 1200);
  cMuL3->Divide(4,4);
  pad=0;
  cMuL3->cd(++pad);
  hNMu_[1]->Draw();
  cMuL3->cd(++pad);
  hQ_[1]->Draw();
  cMuL3->cd(++pad);
  hPt_[1]->Draw();
  cMuL3->cd(++pad);
  hPtlx_[1]->Draw();
  cMuL3->cd(++pad);
  hEta_[1]->Draw();
  cMuL3->cd(++pad);
  hPhi_[1]->Draw();
  cMuL3->cd(++pad);
  gStyle->SetOptStat(0);
  hEtaphi_[1]->Draw("colz");
  gStyle->SetOptStat(111110);
  cMuL3->cd(++pad);
  hDr_[1]->Draw();
  cMuL3->cd(++pad);
  hDz_[1]->Draw();
  cMuL3->cd(++pad);
  hErr0_[1]->Draw();
  cMuL3->cd(++pad);
  hNhit_[1]->Draw();
  cMuL3->cd(++pad);
  hIso_[1]->Draw();
  cMuL3->cd(++pad);
  hDimumass_[1]->Draw();
  histName = htmlDir+"/Level3.png";
  cMuL3->SaveAs(histName.c_str()); 
  delete cMuL2;
  delete cMuL3; 

  htmlFile << "<img src=\"Level2.png\"></img>" << endl;
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
  dbe_->rmdir("HLTMuon");  

  if ( init_ ) this->cleanup();
}
