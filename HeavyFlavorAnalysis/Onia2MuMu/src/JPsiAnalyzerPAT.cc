// -*- C++ -*-
//
// Package:    JPsiAnalyzerPAT
// Class:      JPsiAnalyzerPAT
// 
/**\class JPsiAnalyzerPAT JPsiAnalyzerPAT.cc OctoberXTracking/JPsiAnalyzerPAT/src/JPsiAnalyzerPAT.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author: Roberto Covarelli 
//         Created:  Fri Oct  9 04:59:40 PDT 2009
// $Id: JPsiAnalyzerPAT.cc,v 1.1 2009/10/09 13:02:15 mangano Exp $
//
//

// system include files
#include <memory>

// ROOT/Roofit include files
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1.h>

#include "RooFit.h"
#include "RooGlobalFunc.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooCategory.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>
#include <DataFormats/PatCandidates/interface/Muon.h>

using namespace std;
using namespace edm;
using namespace reco;
using namespace RooFit;

//
// class declaration
//
class JPsiAnalyzerPAT : public edm::EDAnalyzer {
   public:
      explicit JPsiAnalyzerPAT(const edm::ParameterSet&);
      ~JPsiAnalyzerPAT();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // histos
      TH1F *QQMass2Glob_passmu3;
      TH1F *QQMass1Glob1Trk_passmu3;
      TH1F *QQMass1Glob1Cal_passmu3;
      TH1F *QQMass2GlobPT6_passmu3;
      TH1F *QQMass1Glob1TrkPT6_passmu3;
      TH1F *QQMass1Glob1CalPT6_passmu3;
      TH1F *WSMass2Glob_passmu3;
      TH1F *WSMass1Glob1Trk_passmu3;
      TH1F *WSMass1Glob1Cal_passmu3;
      TH1F *QQMass2Glob_passmu5;
      TH1F *QQMass1Glob1Trk_passmu5;
      TH1F *QQMass1Glob1Cal_passmu5;
  // TH1F *QQMass2Glob_passmu9;
  // TH1F *QQMass1Glob1Trk_passmu9;
  // TH1F *QQMass1Glob1Cal_passmu9;
      TH1F *QQMass2GlobPT6_passmu5;
      TH1F *QQMass1Glob1TrkPT6_passmu5;
      TH1F *QQMass1Glob1CalPT6_passmu5;
      TH1F *QQMass2Glob_pass2mu3;
      TH1F *QQMass1Glob1Trk_pass2mu3;
      TH1F *QQMass1Glob1Cal_pass2mu3;
      TH1F *WSMass2Glob_pass2mu3;
      TH1F *WSMass1Glob1Trk_pass2mu3;
      TH1F *WSMass1Glob1Cal_pass2mu3;
      TH1F *QQMass2Glob_pass2mu0;             
      TH1F *QQMass1Glob1Trk_pass2mu0; 
      TH1F *QQMass1Glob1Cal_pass2mu0;
      
      TH1F *hMcRecoGlobMuDeltaR;        
      TH1F *hMcRecoTrkMuDeltaR;  
      TH1F *hMcRecoCalMuDeltaR;  
      
      TH1F *hMcRightGlbMunPixHits; 
      TH1F *hMcWrongGlbMunPixHits; 
      TH1F *hMcRightGlbMud0;       
      TH1F *hMcWrongGlbMud0;       
      TH1F *hMcRightGlbMudz;       
      TH1F *hMcWrongGlbMudz;       
      TH1F *hMcRightGlbMuFirstLayer;
      TH1F *hMcWrongGlbMuFirstLayer;

      TH1F *hMcRightGlbGlbMuLife;              
      TH1F *hMcWrongGlbGlbMuLife;
      TH1F *hMcRightGlbGlbMuMass;              
      TH1F *hMcWrongGlbGlbMuMass;
      TH1F *hMcRightGlbGlbMuVtxProb;
      TH1F *hMcWrongGlbGlbMuVtxProb;

      TH1F *hMcRightTrkMuPt;
      TH1F *hMcWrongTrkMuPt;
      TH1F *hMcRightTrkBit4;
      TH1F *hMcWrongTrkBit4;
      TH1F *hMcRightTrkBit5;
      TH1F *hMcWrongTrkBit5;
      TH1F *hMcRightTrkBit8;
      TH1F *hMcWrongTrkBit8;
      TH1F *hMcRightTrkBit9;
      TH1F *hMcWrongTrkBit9;
      TH1F *hMcRightTrkMuChi2;   
      TH1F *hMcWrongTrkMuChi2; 
      TH1F *hMcRightTrkMuNhits;
      TH1F *hMcWrongTrkMuNhits;
      TH1F *hMcRightTrkMud0;       
      TH1F *hMcWrongTrkMud0;       
      TH1F *hMcRightTrkMudz;       
      TH1F *hMcWrongTrkMudz;

      TH1F *hMcRightGlbTrkMuLife;              
      TH1F *hMcWrongGlbTrkMuLife;
      TH1F *hMcRightGlbTrkMuMass;              
      TH1F *hMcWrongGlbTrkMuMass;
      TH1F *hMcRightGlbTrkMuVtxProb;  
      TH1F *hMcWrongGlbTrkMuVtxProb;
      
      TH1F *hMcRightCalMuChi2; 
      TH1F *hMcWrongCalMuChi2; 
      TH1F *hMcRightCalMuNhits;
      TH1F *hMcWrongCalMuNhits;
      TH1F *hMcRightCalMuCaloComp;         
      TH1F *hMcWrongCalMuCaloComp;      
      TH1F *hMcRightCalMuPt;            
      TH1F *hMcWrongCalMuPt;   
 
      TH1F *hMcRightCalGlbMuDeltaR;        
      TH1F *hMcWrongCalGlbMuDeltaR;     
      TH1F *hMcRightCalGlbMuMass;              
      TH1F *hMcWrongCalGlbMuMass;
      TH1F *hMcRightCalGlbMuVtxChi2; 
      TH1F *hMcWrongCalGlbMuVtxChi2; 
      TH1F *hMcRightCalGlbMuS; 
      TH1F *hMcWrongCalGlbMuS;
      TH1F *hMcRightCalGlbMucosAlpha; 
      TH1F *hMcWrongCalGlbMucosAlpha;
          
      TH1F *hMcRightAllMuIso;
      
      TH1F* gammaFactor_GGnonprompt;
      TH1F* gammaFactor_GTnonprompt;
      
      TH2F *heffTrk;
      TH2F *heffMuGlb;
      TH2F *heffMuTrk;
      TH2F *heffMuHLT;

      // dataset
      RooDataSet* data;

      // data members
      string _histfilename;      
      string _datasetname;
      bool _onlythebest;
      bool _storeefficiency;

};    
      
//    
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
JPsiAnalyzerPAT::JPsiAnalyzerPAT(const edm::ParameterSet& iConfig):
  _histfilename(iConfig.getParameter<string>("histFileName")),
  _datasetname(iConfig.getParameter<string>("dataSetName")),
  _onlythebest(iConfig.getParameter<bool>("onlyTheBest")),
  _storeefficiency(iConfig.getParameter<bool>("storeEfficiency"))
{
   //now do what ever initialization is needed
}


JPsiAnalyzerPAT::~JPsiAnalyzerPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
JPsiAnalyzerPAT::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   Handle<pat::CompositeCandidateCollection > collH;
   iEvent.getByLabel("myJPsiAnalysisPAT",collH);

   cout << "analyze event" << endl;
   for(vector<pat::CompositeCandidate>::const_iterator it=collH->begin();
       it!=collH->end();++it){
     
     bool step1(true),step2(true),step3(true),step4(true),step5(true),step6(true);


     const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(it->daughter("muon1"));
     const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(it->daughter("muon2"));
     
     if(muon1==0 || muon2==0) {cout << "ERROR: dynamic cast failed" << endl; continue;}
     
     if(muon1->charge() == muon2->charge()) step1=false;
     if(!(muon1->isGlobalMuon() && muon2->isGlobalMuon())) step2=false;
     if(!(muon1->innerTrack()->found() >11  && muon2->innerTrack()->found() >=11 )) step3=false;
     if( muon1->innerTrack()->pt()< 2.5 || muon2->innerTrack()->pt()<2.5 ) step4=false;
     if( it->userFloat("vNChi2") > 4.) step5=false;
     if( it->mass()<2.6 || it->mass()>3.5) step6=false;
     
     cout << "analyze candidate" << endl;
     
   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
JPsiAnalyzerPAT::beginJob()
{
  // book histos

  // trigger passed 
  QQMass2Glob_passmu3              = new TH1F("QQMass2Glob_passmu3",  "Invariant mass (2 global muons)", 100, 2.,4.);
  QQMass1Glob1Trk_passmu3          = new TH1F("QQMass1Glob1Trk_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 100, 2.,4.);
  QQMass1Glob1Cal_passmu3          = new TH1F("QQMass1Glob1Cal_passmu3",  "Invariant mass (1 global + 1 calo muon)", 100, 2.,4.);
  QQMass2GlobPT6_passmu3           = new TH1F("QQMass2GlobPT6_passmu3",  "Invariant mass (2 global muons)", 100, 0.,15.);
  QQMass1Glob1TrkPT6_passmu3       = new TH1F("QQMass1Glob1TrkPT6_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  QQMass1Glob1CalPT6_passmu3       = new TH1F("QQMass1Glob1CalPT6_passmu3",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  WSMass2Glob_passmu3              = new TH1F("WSMass2Glob_passmu3",  "Invariant mass (2 global muons)", 100, 0.,15.);
  WSMass1Glob1Trk_passmu3          = new TH1F("WSMass1Glob1Trk_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  WSMass1Glob1Cal_passmu3          = new TH1F("WSMass1Glob1Cal_passmu3",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  QQMass2Glob_passmu5              = new TH1F("QQMass2Glob_passmu5",  "Invariant mass (2 global muons)", 100, 0.,15.);
  QQMass1Glob1Trk_passmu5          = new TH1F("QQMass1Glob1Trk_passmu5",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  QQMass1Glob1Cal_passmu5          = new TH1F("QQMass1Glob1Cal_passmu5",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.); 
  // QQMass2Glob_passmu9              = new TH1F("QQMass2Glob_passmu9",  "Invariant mass (2 global muons)", 100, 0.,15.);
  // QQMass1Glob1Trk_passmu9          = new TH1F("QQMass1Glob1Trk_passmu9",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  // QQMass1Glob1Cal_passmu9          = new TH1F("QQMass1Glob1Cal_passmu9",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  QQMass2GlobPT6_passmu5           = new TH1F("QQMass2GlobPT6_passmu5",  "Invariant mass (2 global muons)", 100, 0.,15.);
  QQMass1Glob1TrkPT6_passmu5       = new TH1F("QQMass1Glob1TrkPT6_passmu5",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  QQMass1Glob1CalPT6_passmu5       = new TH1F("QQMass1Glob1CalPT6_passmu5",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  QQMass2Glob_pass2mu3              = new TH1F("QQMass2Glob_pass2mu3",  "Invariant mass (2 global muons)", 100, 0.,15.);
  QQMass1Glob1Trk_pass2mu3          = new TH1F("QQMass1Glob1Trk_pass2mu3",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  QQMass1Glob1Cal_pass2mu3          = new TH1F("QQMass1Glob1Cal_pass2mu3",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  WSMass2Glob_pass2mu3              = new TH1F("WSMass2Glob_pass2mu3",  "Invariant mass (2 global muons)", 100, 0.,15.);
  WSMass1Glob1Trk_pass2mu3          = new TH1F("WSMass1Glob1Trk_pass2mu3",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  WSMass1Glob1Cal_pass2mu3          = new TH1F("WSMass1Glob1Cal_pass2mu3",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);
  QQMass2Glob_pass2mu0              = new TH1F("QQMass2Glob_pass2mu0",  "Invariant mass (2 global muons)", 100, 0.,15.);
  QQMass1Glob1Trk_pass2mu0          = new TH1F("QQMass1Glob1Trk_pass2mu0",  "Invariant mass (1 global + 1 tracker muon)", 100, 0.,15.);
  QQMass1Glob1Cal_pass2mu0          = new TH1F("QQMass1Glob1Cal_pass2mu0",  "Invariant mass (1 global + 1 calo muon)", 100, 0.,15.);

  // deltaR
  hMcRecoGlobMuDeltaR               = new TH1F("hMcRecoGlobMuDeltaR",  "MC-reco matching #Delta R (global muons)", 100, 0.,0.5);
  hMcRecoTrkMuDeltaR                = new TH1F("hMcRecoTrkMuDeltaR",  "MC-reco matching #Delta R (tracker muons)", 100, 0.,0.5);
  hMcRecoCalMuDeltaR                = new TH1F("hMcRecoCalMuDeltaR",  "MC-reco matching #Delta R (calo muons)", 100, 0.,0.5);

  // mc-truth matching (global)
  hMcRightGlbMunPixHits            = new TH1F("hMcRightGlbMunPixHits",  "number of pixel hits - MC matched (global muons)", 8, -0.5, 7.5);
  hMcWrongGlbMunPixHits            = new TH1F("hMcWrongGlbMunPixHits",  "number of pixel hits - MC unmatched (global muons)", 8, -0.5, 7.5);
  hMcRightGlbMud0                  = new TH1F("hMcRightGlbMud0",  "d0 - MC matched (global muons)", 100, 0., 30.);
  hMcWrongGlbMud0                  = new TH1F("hMcWrongGlbMud0",  "d0 - MC unmatched (global muons)", 100, 0., 30.);
  hMcRightGlbMudz                  = new TH1F("hMcRightGlbMudz",  "dz - MC matched (global muons)", 100, 0., 50.);
  hMcWrongGlbMudz                  = new TH1F("hMcWrongGlbMudz",  "dz - MC unmatched (global muons)", 100, 0., 50.);
  hMcRightGlbMuFirstLayer          = new TH1F("hMcRightGlbMuFirstLayer",  "first pixel layer hit - MC matched (global muons)", 4, -0.5, 3.5);
  hMcWrongGlbMuFirstLayer          = new TH1F("hMcWrongGlbMuFirstLayer",  "first pixel layer hit - MC unmatched (global muons)", 4, -0.5, 3.5);

  // mc truth matching - global + global
  hMcRightGlbGlbMuMass             = new TH1F("hMcRightGlbGlbMuMass",  "Inv. mass - MC matched (global+global muons)", 100, 2.6,3.6);
  hMcWrongGlbGlbMuMass             = new TH1F("hMcWrongGlbGlbMuMass",  "Inv. mass - MC matched (global+global muons)", 100, 2.6,3.6);
  hMcRightGlbGlbMuLife             = new TH1F("hMcRightGlbGlbMuLife",  "c #tau - MC matched (global+global muons)", 90, -1.0,3.5);
  hMcWrongGlbGlbMuLife             = new TH1F("hMcWrongGlbGlbMuLife",  "c #tau - MC matched (global+global muons)", 90, -1.0,3.5);
  hMcRightGlbGlbMuVtxProb          = new TH1F("hMcRightGlbGlbMuVtxProb",  "Vertex probability - MC matched (global+global muons)", 1000, 0.0,1.0);
  hMcWrongGlbGlbMuVtxProb          = new TH1F("hMcWrongGlbGlbMuVtxProb",  "Vertex probability - MC unmatched (global+global muons)", 1000, 0.0,1.0);
 
  // mc truth matching - trk
  hMcRightTrkMuPt                  = new TH1F("hMcRightTrkMuPt",  "pT  - MC matched (tracker muons)", 50, 0.,5.0);
  hMcWrongTrkMuPt                  = new TH1F("hMcWrongTrkMuPt",  "pT - MC unmatched (tracker muons)", 50, 0.,5.0);
  hMcRightTrkMuChi2                = new TH1F("hMcRightTrkMuChi2",  "chi2  - MC matched (tracker muons)", 50, -0.5, 6.5);
  hMcWrongTrkMuChi2                = new TH1F("hMcWrongTrkMuChi2",  "chi2 - MC unmatched (tracker muons)", 50, -0.5,6.5);
  hMcRightTrkMuNhits               = new TH1F("hMcRightTrkMuNhits",  "chi2  - MC matched (tracker muons)", 30, 0.5, 30.5);
  hMcWrongTrkMuNhits               = new TH1F("hMcWrongTrkMuNhits",  "chi2 - MC unmatched (tracker muons)", 30, 0.5,30.5);
  hMcRightTrkBit4                  = new TH1F("hMcRightTrkBit4",  "2DCompatibilityLoose bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit4                  = new TH1F("hMcWrongTrkBit4",  "2DCompatibilityLoose bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit5                  = new TH1F("hMcRightTrkBit5",  "2DCompatibilityTight bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit5                  = new TH1F("hMcWrongTrkBit5",  "2DCompatibilityTight bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit8                  = new TH1F("hMcRightTrkBit8",  "StationOptimizedLowPtLoose bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit8                  = new TH1F("hMcWrongTrkBit8",  "StationOptimizedLowPtLoose bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit9                  = new TH1F("hMcRightTrkBit9",  "StationOptimizedLowPtTight bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit9                  = new TH1F("hMcWrongTrkBit9",  "StationOptimizedLowPtTight bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkMud0                  = new TH1F("hMcRightTrkMud0",  "d0 - MC matched (global muons)", 100, 0., 30.);
  hMcWrongTrkMud0                  = new TH1F("hMcWrongTrkMud0",  "d0 - MC unmatched (global muons)", 100, 0., 30.);
  hMcRightTrkMudz                  = new TH1F("hMcRightTrkMudz",  "dz - MC matched (global muons)", 100, 0., 50.);
  hMcWrongTrkMudz                  = new TH1F("hMcWrongTrkMudz",  "dz - MC unmatched (global muons)", 100, 0., 50.);

  // mc truth matching - global + trk
  hMcRightGlbTrkMuMass             = new TH1F("hMcRightGlbTrkMuMass",  "Inv. mass - MC matched (global+global muons)", 100, 2.6,3.6);
  hMcWrongGlbTrkMuMass             = new TH1F("hMcWrongGlbTrkMuMass",  "Inv. mass - MC matched (global+global muons)", 100, 2.6,3.6);
  hMcRightGlbTrkMuLife             = new TH1F("hMcRightGlbTrkMuLife",  "c #tau - MC matched (global+global muons)", 90, -1.0,3.5);
  hMcWrongGlbTrkMuLife             = new TH1F("hMcWrongGlbTrkMuLife",  "c #tau - MC matched (global+global muons)", 90, -1.0,3.5);
  hMcRightGlbTrkMuVtxProb          = new TH1F("hMcRightGlbTrkMuVtxProb",  "Vertex probability - MC matched (global+tracker muons)", 1000, 0.0,1.0);
  hMcWrongGlbTrkMuVtxProb          = new TH1F("hMcWrongGlbTrkMuVtxProb",  "Vertex probability - MC unmatched (global+tracker muons)", 1000, 0.0,1.0);

  // mc truth matching - calo
  hMcRightCalMuPt                  = new TH1F("hMcRightCalMuPt",  "pT  - MC matched (calo muons)", 50, 0.,5.0);
  hMcWrongCalMuPt                  = new TH1F("hMcWrongCalMuPt",  "pT - MC unmatched (calo muons)", 50, 0.,5.0);
  hMcRightCalMuCaloComp            = new TH1F("hMcRightCalMuCaloComp",  "pT  - MC matched (calo muons)", 60, 0.5,1.1);
  hMcWrongCalMuCaloComp            = new TH1F("hMcWrongCalMuCaloComp",  "pT - MC unmatched (calo muons)", 60, 0.5,1.1);
  hMcRightCalMuChi2                = new TH1F("hMcRightCalMuChi2",  "chi2  - MC matched (calo muons)", 50, -0.5, 6.5);
  hMcWrongCalMuChi2                = new TH1F("hMcWrongCalMuChi2",  "chi2 - MC unmatched (calo muons)", 50, -0.5,6.5);
  hMcRightCalMuNhits               = new TH1F("hMcRightCalMuNhits",  "chi2  - MC matched (calo muons)", 30, 0.5, 30.5);
  hMcWrongCalMuNhits               = new TH1F("hMcWrongCalMuNhits",  "chi2 - MC unmatched (calo muons)", 30, 0.5,30.5);

  // mc truth matching - global + calo
  hMcRightCalGlbMuDeltaR           = new TH1F("hMcRightCalGlbMuDeltaR",  " DeltaR - MC matched (calo+global muons)", 80, 0.,5.0);
  hMcWrongCalGlbMuDeltaR           = new TH1F("hMcWrongCalGlbMuDeltaR",  " DeltaR - MC unmatched (calo+global muons)", 80, 0.,5.0);
  hMcRightCalGlbMuMass             = new TH1F("hMcRightCalGlbMuMass",  "Inv. mass - MC matched (calo+global muons)", 80, 1.0,5.2);
  hMcWrongCalGlbMuMass             = new TH1F("hMcWrongCalGlbMuMass",  "Inv. Mass - MC unmatched (calo+global muons)", 80, 1.0,5.2);
  hMcRightCalGlbMuVtxChi2          = new TH1F("hMcRightCalGlbMuVtxChi2",  "Vertex norm. #chi^{2} - MC matched (calo+global muons)", 80, 0.0,10.0);
  hMcWrongCalGlbMuVtxChi2          = new TH1F("hMcWrongCalGlbMuVtxChi2",  "Vertex norm. #chi^{2} - MC unmatched (calo+global muons)", 80, 0.0,10.0);
  hMcRightCalGlbMuS                = new TH1F("hMcRightCalGlbMuS",  "Significance of muon IPs - MC matched (calo+global muons)", 80, 0.0,100.0);
  hMcWrongCalGlbMuS                = new TH1F("hMcWrongCalGlbMuS",  "Significance of muon IPs - MC unmatched (calo+global muons)", 80, 0.0,100.0);
  hMcRightCalGlbMucosAlpha         = new TH1F("hMcRightCalGlbMucosAlpha",  "cos #alpha - MC matched (calo+global muons)", 100, -1.0,1.0);
  hMcWrongCalGlbMucosAlpha         = new TH1F("hMcWrongCalGlbMucosAlpha",  "cos #alpha - MC unmatched (calo+global muons)", 100, -1.0,1.0);

  // all
  hMcRightAllMuIso                 = new TH1F("hMcRightAllMuIso",  "Isolation - MC matched (all muons)", 50, -1.0,15.0);
 
  // correction factors
  gammaFactor_GGnonprompt          = new TH1F("gammaFactorGG_nonprompt",  "MC correction for non-prompt decays", 100, 0.,3.);
  gammaFactor_GTnonprompt          = new TH1F("gammaFactorGT_nonprompt",  "MC correction for non-prompt decays", 100, 0.,3.);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
JPsiAnalyzerPAT::endJob() {
  
  TFile fOut(_histfilename.c_str(), "RECREATE");
  fOut.cd();
  
  QQMass2Glob_passmu3           -> Write();        
  QQMass1Glob1Trk_passmu3       -> Write(); 
  QQMass1Glob1Cal_passmu3       -> Write();
  QQMass2GlobPT6_passmu3        -> Write();        
  QQMass1Glob1TrkPT6_passmu3    -> Write(); 
  QQMass1Glob1CalPT6_passmu3    -> Write();
  WSMass2Glob_passmu3           -> Write();        
  WSMass1Glob1Trk_passmu3       -> Write(); 
  WSMass1Glob1Cal_passmu3       -> Write();
  QQMass2Glob_passmu5           -> Write();
  QQMass1Glob1Trk_passmu5       -> Write();
  QQMass1Glob1Cal_passmu5       -> Write();
  QQMass2GlobPT6_passmu5        -> Write();
  QQMass1Glob1TrkPT6_passmu5    -> Write();
  QQMass1Glob1CalPT6_passmu5    -> Write();
  // QQMass2Glob_passmu9        -> Write();
  // QQMass1Glob1Trk_passmu9    -> Write();
  // QQMass1Glob1Cal_passmu9    -> Write();
  QQMass2Glob_pass2mu3          -> Write();
  QQMass1Glob1Trk_pass2mu3      -> Write();
  QQMass1Glob1Cal_pass2mu3      -> Write();
  WSMass2Glob_pass2mu3          -> Write();
  WSMass1Glob1Trk_pass2mu3      -> Write();
  WSMass1Glob1Cal_pass2mu3      -> Write();
  QQMass2Glob_pass2mu0          -> Write();  
  QQMass1Glob1Trk_pass2mu0      -> Write();  
  QQMass1Glob1Cal_pass2mu0      -> Write();  

  hMcRecoGlobMuDeltaR           -> Write();   
  hMcRecoTrkMuDeltaR            -> Write();   
  hMcRecoCalMuDeltaR            -> Write();

  hMcRightGlbMunPixHits         -> Write(); 
  hMcWrongGlbMunPixHits         -> Write(); 
  hMcRightGlbMud0               -> Write(); 
  hMcWrongGlbMud0               -> Write(); 
  hMcRightGlbMudz               -> Write(); 
  hMcWrongGlbMudz               -> Write(); 
  hMcRightGlbMuFirstLayer       -> Write(); 
  hMcWrongGlbMuFirstLayer       -> Write();
 
  hMcRightGlbGlbMuMass          -> Write(); 
  hMcWrongGlbGlbMuMass          -> Write(); 
  hMcRightGlbGlbMuLife          -> Write(); 
  hMcWrongGlbGlbMuLife          -> Write();
  hMcRightGlbGlbMuVtxProb       -> Write(); 
  hMcWrongGlbGlbMuVtxProb       -> Write(); 

  hMcRightTrkMuPt               -> Write(); 
  hMcWrongTrkMuPt               -> Write(); 
  hMcRightTrkBit4               -> Write(); 
  hMcWrongTrkBit4               -> Write(); 
  hMcRightTrkBit5               -> Write(); 
  hMcWrongTrkBit5               -> Write(); 
  hMcRightTrkBit8               -> Write(); 
  hMcWrongTrkBit8               -> Write(); 
  hMcRightTrkBit9               -> Write(); 
  hMcWrongTrkBit9               -> Write();
  hMcRightTrkMuChi2             -> Write();   
  hMcWrongTrkMuChi2             -> Write(); 
  hMcRightTrkMuNhits            -> Write(); 
  hMcWrongTrkMuNhits            -> Write(); 
  hMcRightGlbTrkMuVtxProb       -> Write();   
  hMcWrongGlbTrkMuVtxProb       -> Write(); 

  hMcRightCalMuChi2             -> Write(); 
  hMcWrongCalMuChi2             -> Write(); 
  hMcRightCalMuNhits            -> Write(); 
  hMcWrongCalMuNhits            -> Write(); 
  hMcRightTrkMud0               -> Write(); 
  hMcWrongTrkMud0               -> Write(); 
  hMcRightTrkMudz               -> Write(); 
  hMcWrongTrkMudz               -> Write(); 
  hMcRightCalMuCaloComp         -> Write();   
  hMcWrongCalMuCaloComp         -> Write(); 
  hMcRightCalMuPt               -> Write(); 
  hMcWrongCalMuPt               -> Write();
  hMcRightAllMuIso              -> Write();
  hMcRightCalGlbMuDeltaR        -> Write();   
  hMcWrongCalGlbMuDeltaR        -> Write();   
  hMcRightCalGlbMuMass          -> Write();   
  hMcWrongCalGlbMuMass          -> Write(); 
  hMcRightCalGlbMuVtxChi2       -> Write(); 
  hMcWrongCalGlbMuVtxChi2       -> Write();
  hMcRightCalGlbMuS             -> Write(); 
  hMcWrongCalGlbMuS             -> Write();
  hMcRightCalGlbMucosAlpha      -> Write(); 
  hMcWrongCalGlbMucosAlpha      -> Write();
   
  fOut.Close();

  TFile fOut2(_datasetname.c_str(), "RECREATE");
  fOut2.cd();
  data->Write();
  fOut2.Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(JPsiAnalyzerPAT);
