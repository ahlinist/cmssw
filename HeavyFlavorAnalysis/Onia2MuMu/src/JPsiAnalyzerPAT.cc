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
// $Id: JPsiAnalyzerPAT.cc,v 1.4 2009/11/25 16:34:46 covarell Exp $
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

#include <DataFormats/PatCandidates/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>
#include <DataFormats/Common/interface/TriggerResults.h>
#include <DataFormats/Math/interface/deltaR.h>

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

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
      pair< unsigned int, pat::CompositeCandidate > theBestQQ();
      void fillHistosAndDS(unsigned int theCat, pat::CompositeCandidate aCand);
      bool selGlobalMuon(const pat::Muon* aMuon);
      bool selTrackerMuon(const pat::Muon* aMuon);
      bool selCaloMuon(const pat::Muon* aMuon);

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

      // dataset and RooRealVars
      RooDataSet* data;
      RooRealVar* JpsiMass;      
      RooRealVar* JpsiPt;
      RooRealVar* JpsiEta; 
      RooRealVar* Jpsict;
      RooRealVar* JpsictTrue;
      RooRealVar* TNPeff;
      RooRealVar* TNPefferr;       			
      RooCategory* JpsiType;
      RooCategory* JpsiPtType;
      RooCategory* JpsiEtaType;
      RooCategory* matchType;

      // handles
      Handle<pat::CompositeCandidateCollection > collGG;
      Handle<pat::CompositeCandidateCollection > collGT;
      Handle<pat::CompositeCandidateCollection > collTT;
      Handle<pat::CompositeCandidateCollection > collGC;
      Handle<TriggerResults> trigger;

      // data members
      string _histfilename;      
      string _datasetname;
      vector<double> _ptbinranges;
      vector<double> _etabinranges;
      bool _onlythebest;
      bool _applycuts;
      bool _storeefficiency;
      InputTag _triggerresults;

      unsigned int nEvents;

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
  _ptbinranges(iConfig.getParameter< vector<double> >("pTBinRanges")),
  _etabinranges(iConfig.getParameter< vector<double> >("etaBinRanges")),
  _onlythebest(iConfig.getParameter<bool>("onlyTheBest")),
  _applycuts(iConfig.getParameter<bool>("applyCuts")),
  _storeefficiency(iConfig.getParameter<bool>("storeEfficiency")),
  _triggerresults(iConfig.getParameter<InputTag>("TriggerResultsLabel"))
{
   //now do what ever initialization is needed
  nEvents = 0;

  // mass-lifetime limits
  const float JpsiMassMin = 2.6;
  const float JpsiMassMax = 3.6;
  const float JpsiCtMin = -1.0;
  const float JpsiCtMax = 3.5;
  float JpsiPtMin = 0.0;           // SET BY 
  float JpsiPtMax = 0.0;           // DEFINITION
  float JpsiEtaMin = 0.0;          // OF BIN
  float JpsiEtaMax = 0.0;          // LIMITS NOW
  
  JpsiPtType = new RooCategory("JpsiPtType","Category of Pt");
  JpsiEtaType = new RooCategory("JpsiEtaType","Category of Eta");

  JpsiPtMin = _ptbinranges[0];  cout << "Pt min = " << JpsiPtMin << endl;
  JpsiPtMax = _ptbinranges[_ptbinranges.size()-1];  cout << "Pt max = " << JpsiPtMax << endl;

  for(unsigned int i=0;i<_ptbinranges.size()-1;i++){
    char catname[100];
    sprintf(catname,"P%d",i+1);
    JpsiPtType->defineType(catname,i+1); 
    cout << "Pt bin " << i+1 << ": Min = " << _ptbinranges[i] << " Max = " << _ptbinranges[i+1] << endl;   
  }

  JpsiEtaMin = _etabinranges[0];  cout << "Eta min = " << JpsiEtaMin << endl;
  JpsiEtaMax = _etabinranges[_etabinranges.size()-1];  cout << "Eta max = " << JpsiEtaMax << endl;

  for(unsigned int i=0;i<_etabinranges.size()-1;i++){
    char catname[100];
    sprintf(catname,"E%d",i+1);
    JpsiEtaType->defineType(catname,i+1); 
    cout << "Eta bin " << i+1 << ": Min = " << _etabinranges[i] << " Max = " << _etabinranges[i+1] << endl;   
  }

  JpsiType = new RooCategory("JpsiType","Category of Jpsi");
  matchType = new RooCategory("matchType","Category of matching");

  JpsiType->defineType("GG",0);
  JpsiType->defineType("GT",1);
  JpsiType->defineType("TT",2);
  JpsiType->defineType("GC",3);

  matchType->defineType("matched",0);
  matchType->defineType("unmatched",1);

  JpsiMass = new RooRealVar("JpsiMass","J/psi mass",JpsiMassMin,JpsiMassMax,"GeV/c^{2}");
  JpsiPt = new RooRealVar("JpsiPt","J/psi pt",JpsiPtMin,JpsiPtMax,"GeV/c");
  JpsiEta = new RooRealVar("JpsiEta","J/psi eta",-JpsiEtaMax,JpsiEtaMax);
  Jpsict = new RooRealVar("Jpsict","J/psi ctau",JpsiCtMin,JpsiCtMax,"mm");
  JpsictTrue = new RooRealVar("JpsictTrue","J/psi ctau true",JpsiCtMin,JpsiCtMax,"mm");
  TNPeff = new RooRealVar("TNPeff","Tag and probe efficiency",0.,1.);
  TNPefferr = new RooRealVar("TNPefferr","Tag and probe efficiency uncertainty",0.,1.);  		

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
   nEvents++;

   iEvent.getByLabel(_triggerresults,trigger);

   if (!iEvent.getByLabel("onia2MuMuPatGlbGlb",collGG)) cout << "Global-global J/psi not present in event!" << endl;
   
   if (!iEvent.getByLabel("onia2MuMuPatGlbTrk",collGT)) cout << "Global-tracker J/psi not present in event!" << endl;

   if (!iEvent.getByLabel("onia2MuMuPatTrkTrk",collTT)) cout << "Tracker-tracker J/psi not present in event!" << endl;

   if (!iEvent.getByLabel("onia2MuMuPatGlbCal",collGC)) cout << "Global-calo J/psi not present in event!" << endl;

   if (nEvents%10000 == 0) cout << "analyze event # " << nEvents << endl;

   // BEST J/PSI?

   if (_onlythebest) {  // yes, fill simply the best

     pair< unsigned int, pat::CompositeCandidate > theBest = theBestQQ();
     if (theBest.first < 10) fillHistosAndDS(theBest.first, theBest.second);

   } else {   // no, fill all

     for(vector<pat::CompositeCandidate>::const_iterator it=collGG->begin();
	 it!=collGG->end();++it) { fillHistosAndDS(0, *it); }

     for(vector<pat::CompositeCandidate>::const_iterator it=collGT->begin();
	 it!=collGT->end();++it) { fillHistosAndDS(1, *it); }

     for(vector<pat::CompositeCandidate>::const_iterator it=collTT->begin();
	 it!=collTT->end();++it) { fillHistosAndDS(2, *it); }

     for(vector<pat::CompositeCandidate>::const_iterator it=collGC->begin();
	 it!=collGC->end();++it) { fillHistosAndDS(3, *it); }
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
  hMcRightGlbTrkMuMass            = new TH1F("hMcRightGlbTrkMuMass",  "Inv. mass - MC matched (global+tracker muons)", 100, 2.6,3.6);
  hMcWrongGlbTrkMuMass            = new TH1F("hMcWrongGlbTrkMuMass",  "Inv. mass - MC matched (global+tracker muons)", 100, 2.6,3.6);
  hMcRightGlbTrkMuLife            = new TH1F("hMcRightGlbTrkMuLife",  "c #tau - MC matched (global+tracker muons)", 90, -1.0,3.5);
  hMcWrongGlbTrkMuLife            = new TH1F("hMcWrongGlbTrkMuLife",  "c #tau - MC matched (global+tracker muons)", 90, -1.0,3.5);
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

  hMcRightGlbTrkMuMass          -> Write();
  hMcWrongGlbTrkMuMass        	-> Write();
  hMcRightGlbTrkMuLife        	-> Write();
  hMcWrongGlbTrkMuLife        	-> Write();
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

void 
JPsiAnalyzerPAT::fillHistosAndDS(unsigned int theCat, pat::CompositeCandidate aCand){
  
  const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aCand.daughter("muon1"));
  const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aCand.daughter("muon2"));

  // Trigger Results inspection
  
  static const unsigned int NTRIGGERS = 5;
  string HLTbitNames[NTRIGGERS] = {"HLT_L1MuOpen", "HLT_Mu3", "HLT_Mu5", "HLT_DoubleMu0", "HLT_DoubleMu3"};
  unsigned int hltBits[NTRIGGERS];

  HLTConfigProvider hltConfig;
  if (hltConfig.init(_triggerresults.process())) {
    // check if trigger name in config
    const unsigned int n(hltConfig.size());
    for (unsigned int ihlt = 0; ihlt < NTRIGGERS; ihlt++) {
      hltBits[ihlt] = 0;
      unsigned int triggerIndex( hltConfig.triggerIndex(HLTbitNames[ihlt]) );
      if (triggerIndex>=n) {
	cout << "TriggerName " << HLTbitNames[ihlt] << " not available in config!" << endl;
      } else {
	hltBits[ihlt] = triggerIndex;
      }
    }
  }

  // MC matching
  reco::GenParticleRef genJpsi = aCand.genParticleRef();
  bool isMatched = (genJpsi.isNonnull() && genJpsi->pdgId() == 443);

  // Trigger passed

  if (trigger.isValid()) {
    if (trigger->accept(hltBits[1])) { // pass Mu3
      if (muon1->charge()*muon2->charge() < 0) {
	if (theCat == 0) QQMass2Glob_passmu3->Fill(aCand.mass());          
	if (theCat == 1) QQMass1Glob1Trk_passmu3->Fill(aCand.mass());
	if (theCat == 3) QQMass1Glob1Cal_passmu3->Fill(aCand.mass());    
	if (theCat == 0 && aCand.pt() < 6.0) QQMass2GlobPT6_passmu3->Fill(aCand.mass());     
	if (theCat == 1 && aCand.pt() < 6.0) QQMass1Glob1TrkPT6_passmu3->Fill(aCand.mass());
	if (theCat == 3 && aCand.pt() < 6.0) QQMass1Glob1CalPT6_passmu3->Fill(aCand.mass());
      } else {
	if (theCat == 0) WSMass2Glob_passmu3->Fill(aCand.mass());          
	if (theCat == 1) WSMass1Glob1Trk_passmu3->Fill(aCand.mass());
	if (theCat == 3) WSMass1Glob1Cal_passmu3->Fill(aCand.mass());       
      }
    }
    if (trigger->accept(hltBits[2])) { // pass Mu5
      if (muon1->charge()*muon2->charge() < 0) {
	if (theCat == 0) QQMass2Glob_passmu5->Fill(aCand.mass());          
	if (theCat == 1) QQMass1Glob1Trk_passmu5->Fill(aCand.mass());
	if (theCat == 3) QQMass1Glob1Cal_passmu5->Fill(aCand.mass());    
	if (theCat == 0 && aCand.pt() < 6.0) QQMass2GlobPT6_passmu5->Fill(aCand.mass());     
	if (theCat == 1 && aCand.pt() < 6.0) QQMass1Glob1TrkPT6_passmu5->Fill(aCand.mass());
	if (theCat == 3 && aCand.pt() < 6.0) QQMass1Glob1CalPT6_passmu5->Fill(aCand.mass());
      } 
    }
    if (trigger->accept(hltBits[3])) { // pass 2Mu3
      if (muon1->charge()*muon2->charge() < 0) {
	if (theCat == 0) QQMass2Glob_pass2mu3->Fill(aCand.mass());          
	if (theCat == 1) QQMass1Glob1Trk_pass2mu3->Fill(aCand.mass());
	if (theCat == 3) QQMass1Glob1Cal_pass2mu3->Fill(aCand.mass());    
      } else {
	if (theCat == 0) WSMass2Glob_pass2mu3->Fill(aCand.mass());          
	if (theCat == 1) WSMass1Glob1Trk_pass2mu3->Fill(aCand.mass());
	if (theCat == 3) WSMass1Glob1Cal_pass2mu3->Fill(aCand.mass());       
      }
    }
    if (trigger->accept(hltBits[4])) { // pass 2Mu0
      if (muon1->charge()*muon2->charge() < 0) {
	if (theCat == 0) QQMass2Glob_pass2mu0->Fill(aCand.mass());          
	if (theCat == 1) QQMass1Glob1Trk_pass2mu0->Fill(aCand.mass());
	if (theCat == 3) QQMass1Glob1Cal_pass2mu0->Fill(aCand.mass());    
      }
    }  
  } 	
	
  // Signal / background J/psi 	
  if (muon1->charge()*muon2->charge() < 0) {   
    if (isMatched) {

      if (theCat == 0) {
	hMcRightGlbGlbMuMass->Fill(aCand.mass());       
	hMcRightGlbGlbMuLife->Fill(aCand.userFloat("ppdlPV"));            
	hMcRightGlbGlbMuVtxProb->Fill(aCand.userFloat("vProb")); 
      } else if (theCat == 1) {
        hMcRightGlbTrkMuMass->Fill(aCand.mass());       
	hMcRightGlbTrkMuLife->Fill(aCand.userFloat("ppdlPV"));            
	hMcRightGlbTrkMuVtxProb->Fill(aCand.userFloat("vProb"));   
      } else if (theCat == 3) {
        hMcRightCalGlbMuDeltaR->Fill(deltaR(muon1->eta(),muon1->phi(),muon2->eta(),muon2->phi()));     
	hMcRightCalGlbMuMass->Fill(aCand.mass());           
	hMcRightCalGlbMuVtxChi2->Fill(aCand.userFloat("vNChi2"));  
	hMcRightCalGlbMuS->Fill(sqrt(pow(muon1->track()->d0()/muon1->track()->d0Error(),2) + pow(muon2->track()->d0()/muon2->track()->d0Error(),2)));
	hMcRightCalGlbMucosAlpha->Fill(aCand.userFloat("cosAlpha"));
      }   

    } else {
    
      if (theCat == 0) {
	hMcWrongGlbGlbMuMass->Fill(aCand.mass());       
	hMcWrongGlbGlbMuLife->Fill(aCand.userFloat("ppdlPV"));            
	hMcWrongGlbGlbMuVtxProb->Fill(aCand.userFloat("vProb")); 
      } else if (theCat == 1) {
        hMcWrongGlbTrkMuMass->Fill(aCand.mass());       
	hMcWrongGlbTrkMuLife->Fill(aCand.userFloat("ppdlPV"));            
	hMcWrongGlbTrkMuVtxProb->Fill(aCand.userFloat("vProb"));   
      } else if (theCat == 3) {
        hMcWrongCalGlbMuDeltaR->Fill(deltaR(muon1->eta(),muon1->phi(),muon2->eta(),muon2->phi()));     
	hMcWrongCalGlbMuMass->Fill(aCand.mass());           
	hMcWrongCalGlbMuVtxChi2->Fill(aCand.userFloat("vNChi2"));  
	hMcWrongCalGlbMuS->Fill(sqrt(pow(muon1->track()->d0()/muon1->track()->d0Error(),2) + pow(muon2->track()->d0()/muon2->track()->d0Error(),2)));
	hMcWrongCalGlbMucosAlpha->Fill(aCand.userFloat("cosAlpha"));
      }  

    }
  
    // Signal / background muons
    char whichMuon[6];
    for(unsigned int i = 1; i<=2; i++) {
      
      sprintf(whichMuon,"muon%d",i);
      const pat::Muon* thisMuon = dynamic_cast<const pat::Muon*>(aCand.daughter(whichMuon));
 	
      reco::GenParticleRef genMu = thisMuon->genParticleRef();
      TrackRef iTrack = thisMuon->innerTrack();
      const reco::HitPattern& p = iTrack->hitPattern();

      if (isMatched) {
	if (thisMuon->isGlobalMuon()) { 
	  if (genMu.isNonnull()) hMcRecoGlobMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
	  hMcRightGlbMunPixHits->Fill(p.numberOfValidPixelHits());
	  hMcRightGlbMud0->Fill(iTrack->d0());
	  hMcRightGlbMudz->Fill(iTrack->dz());
	  hMcRightGlbMuFirstLayer->Fill(p.getLayer(p.getHitPattern(0)));
	} else if (thisMuon->isTrackerMuon()) {     // notice exclusivity!  
	  if (genMu.isNonnull()) hMcRecoTrkMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
          hMcRightTrkMuPt->Fill(thisMuon->pt());
          hMcRightTrkBit4->Fill((int)thisMuon->muonID("TM2DCompatibilityLoose"));
          hMcRightTrkBit5->Fill((int)thisMuon->muonID("TM2DCompatibilityTight"));
	  hMcRightTrkBit8->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtLoose"));
	  hMcRightTrkBit9->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtTight"));
          hMcRightTrkMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcRightTrkMuNhits->Fill(iTrack->found());
          hMcRightTrkMud0->Fill(iTrack->d0());
	  hMcRightTrkMudz->Fill(iTrack->dz());
	}         
	if (thisMuon->isCaloMuon()) {
	  if (genMu.isNonnull()) hMcRecoCalMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
	  hMcRightCalMuPt->Fill(thisMuon->pt());
          hMcRightCalMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcRightCalMuNhits->Fill(iTrack->found());    
	  hMcRightCalMuCaloComp->Fill(thisMuon->caloCompatibility());                    
	}
      } else {
	if (thisMuon->isGlobalMuon()) { 
	  hMcWrongGlbMunPixHits->Fill(p.numberOfValidPixelHits());
	  hMcWrongGlbMud0->Fill(iTrack->d0());
	  hMcWrongGlbMudz->Fill(iTrack->dz());
	  hMcWrongGlbMuFirstLayer->Fill(p.getLayer(p.getHitPattern(0)));
	} else if (thisMuon->isTrackerMuon()) {     // notice exclusivity!  
          hMcWrongTrkMuPt->Fill(thisMuon->pt());
          hMcWrongTrkBit4->Fill((int)thisMuon->muonID("TM2DCompatibilityLoose"));
          hMcWrongTrkBit5->Fill((int)thisMuon->muonID("TM2DCompatibilityTight"));
	  hMcWrongTrkBit8->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtLoose"));
	  hMcWrongTrkBit9->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtTight"));
          hMcWrongTrkMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcWrongTrkMuNhits->Fill(iTrack->found());
          hMcWrongTrkMud0->Fill(iTrack->d0());
	  hMcWrongTrkMudz->Fill(iTrack->dz());
	}         
	if (thisMuon->isCaloMuon()) {
	  hMcWrongCalMuPt->Fill(thisMuon->pt());
          hMcWrongCalMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcWrongCalMuNhits->Fill(iTrack->found());    
	  hMcWrongCalMuCaloComp->Fill(thisMuon->caloCompatibility());                    
	}
      }
    }
  }

  // Now the RooDataSet
}

pair< unsigned int, pat::CompositeCandidate > 
JPsiAnalyzerPAT::theBestQQ() {

  for(vector<pat::CompositeCandidate>::const_iterator it=collGG->begin();
	 it!=collGG->end();++it) {

    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(it->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(it->daughter("muon2"));
     
    if(muon1->charge()*muon2->charge() < 0) {
      if (!_applycuts || (selGlobalMuon(muon1) &&
			  selGlobalMuon(muon2) &&
			  it->userFloat("vProb") > 0.001 )) {
	pair< unsigned int, pat::CompositeCandidate > result = make_pair(0,*it);
	return result;
      }
    }
  } 

  for(vector<pat::CompositeCandidate>::const_iterator it=collGT->begin();
	 it!=collGT->end();++it) {

    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(it->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(it->daughter("muon2"));
     
    if(muon1->charge()*muon2->charge() < 0) {
      if (!_applycuts || (selGlobalMuon(muon1) &&
			  selTrackerMuon(muon2) &&
			  it->userFloat("vProb") > 0.001 )) {
	pair< unsigned int, pat::CompositeCandidate > result = make_pair(1,*it);
	return result;
      }
    }
  } 

  for(vector<pat::CompositeCandidate>::const_iterator it=collTT->begin();
	 it!=collTT->end();++it) {

    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(it->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(it->daughter("muon2"));
     
    if(muon1->charge()*muon2->charge() < 0) {
      if (!_applycuts || (selTrackerMuon(muon1) &&
			  selTrackerMuon(muon2) &&
			  it->userFloat("vProb") > 0.001 )) {
	pair< unsigned int, pat::CompositeCandidate > result = make_pair(2,*it);
	return result;
      }
    }
  } 

  for(vector<pat::CompositeCandidate>::const_iterator it=collGC->begin();
	 it!=collGC->end();++it) {

    const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(it->daughter("muon1"));
    const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(it->daughter("muon2"));
     
    if(muon1->charge()*muon2->charge() < 0) {
      if (!_applycuts || (selGlobalMuon(muon1) &&
			  selCaloMuon(muon2) &&
			  it->userFloat("vProb") > 0.001 )) {
	pair< unsigned int, pat::CompositeCandidate > result = make_pair(3,*it);
	return result;
      }
    }
  }

  pair< unsigned int, pat::CompositeCandidate > result = make_pair(99, pat::CompositeCandidate() );
  return result;	  

}

bool
JPsiAnalyzerPAT::selGlobalMuon(const pat::Muon* aMuon) {

  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (iTrack->found() > 11 &&
	  aMuon->globalTrack()->chi2()/aMuon->globalTrack()->ndof() < 20.0 &&
	  (p.numberOfValidPixelHits() > 2 || 
	   (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
	  iTrack->d0() < 5.0 &&
          iTrack->dz() < 20.0 );
}

bool 
JPsiAnalyzerPAT::selTrackerMuon(const pat::Muon* aMuon) {
  
  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (iTrack->found() > 11 &&
	  iTrack->chi2()/iTrack->ndof() < 5.0 &&
          aMuon->muonID("TrackerMuonArbitrated") &&
          (aMuon->muonID("TMLastStationOptimizedLowPtLoose") ||
	  aMuon->muonID("TM2DCompatibilityTight")) &&
	  (p.numberOfValidPixelHits() > 2 || 
	   (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
	  iTrack->d0() < 5.0 &&
          iTrack->dz() < 20.0 );
}

bool 
JPsiAnalyzerPAT::selCaloMuon(const pat::Muon* aMuon) {
  
  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (aMuon->caloCompatibility() > 0.89 &&
	  iTrack->found() > 11 &&
	  iTrack->chi2()/iTrack->ndof() < 5.0 &&
	  (p.numberOfValidPixelHits() > 2 || 
	   (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
	  iTrack->d0() < 5.0 &&
          iTrack->dz() < 20.0 );
}

//define this as a plug-in
DEFINE_FWK_MODULE(JPsiAnalyzerPAT);
