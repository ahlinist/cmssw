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
// $Id: JPsiAnalyzerPAT.cc,v 1.33 2010/06/24 10:01:05 covarell Exp $
//
//

// system include files
#include <memory>
#include <fstream>

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
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include <DataFormats/BeamSpot/interface/BeamSpot.h>

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
      void makeCuts(int sign) ;
      pair< unsigned int, const pat::CompositeCandidate* > theBestQQ(int sign);
      void fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aCand, const edm::Event&);
      bool isMuonInAccept(const pat::Muon* aMuon);
      bool selGlobalMuon(const pat::Muon* aMuon);
      bool selTrackerMuon(const pat::Muon* aMuon);
      bool selCaloMuon(const pat::Muon* aMuon);
      int getJpsiVarType(const double jpsivar, vector<double> vectbin);

      // histos
      TH1F *QQMass2Glob_passmu3;
      TH1F *QQMass1Glob1Trk_passmu3;
      TH1F *QQMass1Glob1Cal_passmu3;
      TH1F *QQMass2GlobPT6_passmu3;
      TH1F *QQMass1Glob1TrkPT6_passmu3;
      TH1F *QQMass1Glob1CalPT6_passmu3;
  // TH1F *WSMass2Glob_passmu3;
  // TH1F *WSMass1Glob1Trk_passmu3;
  // TH1F *WSMass1Glob1Cal_passmu3;
      TH1F *QQMass2Glob_passmu5;
      TH1F *QQMass1Glob1Trk_passmu5;
      TH1F *QQMass1Glob1Cal_passmu5;
  // TH1F *QQMass2Glob_passmu9;
  // TH1F *QQMass1Glob1Trk_passmu9;
  // TH1F *QQMass1Glob1Cal_passmu9;
      TH1F *QQMass2GlobPT6_passmu5;
      TH1F *QQMass1Glob1TrkPT6_passmu5;
      TH1F *QQMass1Glob1CalPT6_passmu5;
      TH1F *QQMass2Glob_pass2muOpen;
      TH1F *QQMass1Glob1Trk_pass2muOpen;
      TH1F *QQMass1Glob1Cal_pass2muOpen;
  // TH1F *WSMass2Glob_pass2muOpen;
  // TH1F *WSMass1Glob1Trk_pass2muOpen;
  // TH1F *WSMass1Glob1Cal_pass2muOpen;
      TH1F *QQMass2Glob_pass2mu0;             
      TH1F *QQMass1Glob1Trk_pass2mu0; 
      TH1F *QQMass1Glob1Cal_pass2mu0;
      
      TH1F *hMcRecoGlobMuDeltaR;        
      TH1F *hMcRecoTrkMuDeltaR;  
      TH1F *hMcRecoCalMuDeltaR;  
      
      TH1F *hMcRightMunPixHits; 
      TH1F *hMcWrongMunPixHits; 
      TH1F *hMcRightMud0;       
      TH1F *hMcWrongMud0;       
      TH1F *hMcRightMudz;       
      TH1F *hMcWrongMudz;       
      TH1F *hMcRightGlbMuGlobalchi2;
      TH1F *hMcWrongGlbMuGlobalchi2;
      TH1F *hMcRightGlbMunMuHits;
      TH1F *hMcWrongGlbMunMuHits;

      TH1F *hMcRightGlbGlbMuLife;              
      TH1F *hMcWrongGlbGlbMuLife;
      TH1F *hMcRightGlbGlbMuMass;              
      TH1F *hMcWrongGlbGlbMuMass;
      TH1F *hMcPPGlbGlbMuMass;
      TH1F *hMcMMGlbGlbMuMass;
      TH1F *hMcSqrtGlbGlbMuMass;
      TH1F *hMcRightGlbGlbMuPt;              
      TH1F *hMcWrongGlbGlbMuPt;
      TH1F *hMcRightGlbGlbMuEta;              
      TH1F *hMcWrongGlbGlbMuEta; 
      TH1F *hMcRightGlbGlbMuVtxProb;
      TH1F *hMcWrongGlbGlbMuVtxProb;

      TH1F *hMcRightMuPt;
      TH1F *hMcWrongMuPt;
      TH1F *hMcRightMuP;
      TH1F *hMcWrongMuP;
      TH1F *hMcRightTrkBit4;
      TH1F *hMcWrongTrkBit4;
      TH1F *hMcRightTrkBit5;
      TH1F *hMcWrongTrkBit5;
      TH1F *hMcRightTrkBit8;
      TH1F *hMcWrongTrkBit8;
      TH1F *hMcRightTrkBit9;
      TH1F *hMcWrongTrkBit9;
      TH1F *hMcRightTrkBitNew;
      TH1F *hMcWrongTrkBitNew;
      TH1F *hMcRightTrkMuChi2;   
      TH1F *hMcWrongTrkMuChi2; 
      TH1F *hMcRightMuNhits;
      TH1F *hMcWrongMuNhits;
      TH1F *hMcRightTrkMuCaloComp;         
      TH1F *hMcWrongTrkMuCaloComp;

      TH1F *hMcRightGlbTrkMuLife;              
      TH1F *hMcWrongGlbTrkMuLife;
      TH1F *hMcRightGlbTrkMuMass;              
      TH1F *hMcWrongGlbTrkMuMass;
      TH1F *hMcPPGlbTrkMuMass;
      TH1F *hMcMMGlbTrkMuMass;
      TH1F *hMcSqrtGlbTrkMuMass;
      TH1F *hMcRightGlbTrkMuPt;              
      TH1F *hMcWrongGlbTrkMuPt;
      TH1F *hMcRightGlbTrkMuEta;              
      TH1F *hMcWrongGlbTrkMuEta;
      TH1F *hMcRightGlbTrkMuVtxProb;  
      TH1F *hMcWrongGlbTrkMuVtxProb;

      TH1F *hMcRightTrkTrkMuLife;              
      TH1F *hMcWrongTrkTrkMuLife;
      TH1F *hMcRightTrkTrkMuMass;              
      TH1F *hMcWrongTrkTrkMuMass;
      TH1F *hMcPPTrkTrkMuMass;
      TH1F *hMcMMTrkTrkMuMass;
      TH1F *hMcSqrtTrkTrkMuMass;
      TH1F *hMcRightTrkTrkMuPt;              
      TH1F *hMcWrongTrkTrkMuPt;
      TH1F *hMcRightTrkTrkMuEta;              
      TH1F *hMcWrongTrkTrkMuEta;
      TH1F *hMcRightTrkTrkMuVtxProb;  
      TH1F *hMcWrongTrkTrkMuVtxProb;
      
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
      RooRealVar* JpsictErr;
      RooRealVar* JpsictTrue;
      RooRealVar* TNPeff;
      RooRealVar* TNPefferr;       			
      RooCategory* JpsiType;
      RooCategory* JpsiPtType;
      RooCategory* JpsiEtaType;
      RooCategory* matchType;
      RooCategory* trigger0;
      RooCategory* trigger1;

      // handles
      // Handle<pat::CompositeCandidateCollection > collGG;
      // Handle<pat::CompositeCandidateCollection > collGT;

      Handle<pat::CompositeCandidateCollection > collAll;
      Handle<pat::CompositeCandidateCollection > collCalo;
      // Handle<TriggerResults> trigger;

      // data members
      InputTag       _patJpsi;
      InputTag       _patJpsiWithCalo;
      string         _histfilename;      
      string         _datasetname;
      vector<double> _ptbinranges;
      vector<double> _etabinranges;
      bool           _onlythebest;
      bool           _applycuts;
      bool           _storeefficiency;
      bool           _useBS;
      bool           _useRapidity;
      bool           _useCalo;
      bool           _removeSignal;
      bool           _removeMuons;
      bool           _storeWs;
      bool           _writeOutCands;
      bool           _inclPsiP;
      // InputTag       _triggerresults;
      vector<unsigned int>                     _thePassedCats[3];
      vector<const pat::CompositeCandidate*>   _thePassedCands[3];

      // number of events
      unsigned int nEvents;
      unsigned int passedCandidates;
      unsigned int matchOldCandidates;
      unsigned int matchNewCandidates;

      // limits 
      float JpsiMassMin;
      float JpsiMassMax;
      float JpsiMassMinSide;
      float JpsiMassMaxSide;
      float JpsiCtMin;
      float JpsiCtMax;
      float JpsiPtMin;           // SET BY 
      float JpsiPtMax;           // DEFINITION
      float JpsiEtaMin;          // OF BIN
      float JpsiEtaMax;          // LIMITS 

      math::XYZPoint RefVtx;
      ofstream* theTextFile;

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
  _patJpsi(iConfig.getParameter<InputTag>("src")),
  _patJpsiWithCalo(iConfig.getParameter<InputTag>("srcWithCaloMuons")),
  _histfilename(iConfig.getParameter<string>("histFileName")),		
  _datasetname(iConfig.getParameter<string>("dataSetName")),		
  _ptbinranges(iConfig.getParameter< vector<double> >("pTBinRanges")),	
  _etabinranges(iConfig.getParameter< vector<double> >("etaBinRanges")),
  _onlythebest(iConfig.getParameter<bool>("onlyTheBest")),		
  _applycuts(iConfig.getParameter<bool>("applyCuts")),			
  _storeefficiency(iConfig.getParameter<bool>("storeEfficiency")),	
  _useBS(iConfig.getParameter<bool>("useBeamSpot")),
  _useRapidity(iConfig.getParameter<bool>("useRapidity")),
  _useCalo(iConfig.getUntrackedParameter<bool>("useCaloMuons",false)),
  _removeSignal(iConfig.getUntrackedParameter<bool>("removeSignalEvents",false)),
  _removeMuons(iConfig.getUntrackedParameter<bool>("removeTrueMuons",false)),
  _storeWs(iConfig.getUntrackedParameter<bool>("storeWrongSign",false)),
  _writeOutCands(iConfig.getUntrackedParameter<bool>("writeOutCandidates",false)),
  _inclPsiP(iConfig.getUntrackedParameter<bool>("includePsiPrime",false))
  // _triggerresults(iConfig.getParameter<InputTag>("TriggerResultsLabel"))
{
   //now do what ever initialization is needed
  nEvents = 0;
  passedCandidates = 0;
  matchOldCandidates = 0;
  matchNewCandidates = 0;

  JpsiMassMin = 2.6;
  JpsiMassMax = 3.5;
  if (_inclPsiP) JpsiMassMax = 4.2;
  JpsiMassMinSide = 0.;
  JpsiMassMaxSide = 12.0;
  JpsiCtMin = -1.0;
  JpsiCtMax = 3.5;

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
  if (_useCalo) {
    JpsiType->defineType("GC",3);  
    JpsiType->defineType("TC",4);  
    JpsiType->defineType("CC",5);  
  }

  matchType->defineType("unmatched",0);
  matchType->defineType("matched",1);

  trigger0 = new RooCategory("trigger0","Match with trigger bit 0");
  trigger1 = new RooCategory("trigger1","Match with trigger bit 1"); 

  trigger0->defineType("unmatched",0);
  trigger0->defineType("matched",1);
  trigger1->defineType("unmatched",0);
  trigger1->defineType("matched",1);

  JpsiMass = new RooRealVar("JpsiMass","J/psi mass",JpsiMassMin,JpsiMassMax,"GeV/c^{2}");
  JpsiPt = new RooRealVar("JpsiPt","J/psi pt",JpsiPtMin,JpsiPtMax,"GeV/c");
  JpsiEta = new RooRealVar("JpsiEta","J/psi eta",-JpsiEtaMax,JpsiEtaMax);
  Jpsict = new RooRealVar("Jpsict","J/psi ctau",JpsiCtMin,JpsiCtMax,"mm");
  JpsictErr = new RooRealVar("JpsictErr","J/psi ctau error",-1.,1.,"mm");
  JpsictTrue = new RooRealVar("JpsictTrue","J/psi ctau true",-100.,JpsiCtMax,"mm");
  TNPeff = new RooRealVar("TNPeff","Tag and probe efficiency",0.,1.);
  TNPefferr = new RooRealVar("TNPefferr","Tag and probe efficiency uncertainty",0.,1.);  		

  RooArgList varlist(*JpsiMass,*Jpsict,*JpsiPt,*JpsiEta,*TNPeff,*TNPefferr,*JpsiType,*matchType);
  varlist.add(*JpsictTrue);   varlist.add(*JpsiPtType);
  varlist.add(*JpsiEtaType);  varlist.add(*JpsictErr);
  varlist.add(*trigger0);     varlist.add(*trigger1);

  data = new RooDataSet("data","A sample",varlist);
  if (_writeOutCands) theTextFile = new ofstream("passedCandidates.txt");

}


JPsiAnalyzerPAT::~JPsiAnalyzerPAT()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  if (_writeOutCands) theTextFile->close();
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
JPsiAnalyzerPAT::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nEvents++;

   // iEvent.getByLabel(_triggerresults,trigger);

   // try {iEvent.getByLabel("onia2MuMuPatGlbGlb",collGG);} 
   // catch (...) {cout << "Global-global J/psi not present in event!" << endl;}
   
   // try {iEvent.getByLabel("onia2MuMuPatGlbTrk",collGT);}
   // catch (...) {cout << "Global-tracker J/psi not present in event!" << endl;}
   
   // edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
   // iEvent.getByLabel("offlineBeamSpot",recoBeamSpotHandle);
   // reco::BeamSpot bs = *recoBeamSpotHandle;

   // if (_useBS) {
   //  RefVtx = bs.position();
   // } else {

   Handle<reco::VertexCollection> privtxs;
   iEvent.getByLabel("offlinePrimaryVertices", privtxs);
   VertexCollection::const_iterator privtx;

   if ( privtxs->begin() != privtxs->end() ) {
     privtx=privtxs->begin();
     RefVtx = privtx->position();
   } else {
     RefVtx.SetXYZ(0.,0.,0.);
   }
   // }

   try {iEvent.getByLabel(_patJpsi,collAll);} 
   catch (...) {cout << "J/psi not present in event!" << endl;}

   if (_useCalo) {
     try {iEvent.getByLabel(_patJpsiWithCalo,collCalo);} 
     catch (...) {cout << "J/psi to calomuons not present in event!" << endl;}
   }

   _thePassedCats[0].clear();      _thePassedCands[0].clear();
   _thePassedCats[1].clear();      _thePassedCands[1].clear();
   _thePassedCats[2].clear();      _thePassedCands[2].clear();

   // APPLY CUTS
   int lastSign = 0;
   this->makeCuts(0);
   if (_storeWs) {
     this->makeCuts(1);
     this->makeCuts(2);
     lastSign = 2;
   }

   // BEST J/PSI? 

   if (_onlythebest) {  // yes, fill simply the best (possibly wrong-sign)

     for (int iSign = 0; iSign <= lastSign; iSign++) {
       pair< unsigned int, const pat::CompositeCandidate* > theBest = theBestQQ(iSign);
       if (theBest.first < 10) fillHistosAndDS(theBest.first, theBest.second, iEvent);
     }

   } else {   // no, fill all candidates passing cuts (possibly wrong-sign)

     for (int iSign = 0; iSign <= lastSign; iSign++) {
       for( unsigned int count = 0; count < _thePassedCands[iSign].size(); count++) { 
	 fillHistosAndDS(_thePassedCats[iSign].at(count), _thePassedCands[iSign].at(count),iEvent); 
       }
     }

   }
}


// ------------ method called once each job just before starting event loop  ------------
void 
JPsiAnalyzerPAT::beginJob()
{
  // book histos

  // trigger passed 
  QQMass2Glob_passmu3              = new TH1F("QQMass2Glob_passmu3",  "Invariant mass (2 global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  QQMass1Glob1Trk_passmu3          = new TH1F("QQMass1Glob1Trk_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  QQMass1Glob1Cal_passmu3          = new TH1F("QQMass1Glob1Cal_passmu3",  "Invariant mass (1 global + 1 calo muon)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  QQMass2GlobPT6_passmu3           = new TH1F("QQMass2GlobPT6_passmu3",  "Invariant mass (2 global muons)", 120, 0.,15.);
  QQMass1Glob1TrkPT6_passmu3       = new TH1F("QQMass1Glob1TrkPT6_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  QQMass1Glob1CalPT6_passmu3       = new TH1F("QQMass1Glob1CalPT6_passmu3",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  // WSMass2Glob_passmu3              = new TH1F("WSMass2Glob_passmu3",  "Invariant mass (2 global muons)", 120, 0.,15.);
  // WSMass1Glob1Trk_passmu3          = new TH1F("WSMass1Glob1Trk_passmu3",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  // WSMass1Glob1Cal_passmu3          = new TH1F("WSMass1Glob1Cal_passmu3",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  QQMass2Glob_passmu5              = new TH1F("QQMass2Glob_passmu5",  "Invariant mass (2 global muons)", 120, 0.,15.);
  QQMass1Glob1Trk_passmu5          = new TH1F("QQMass1Glob1Trk_passmu5",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  QQMass1Glob1Cal_passmu5          = new TH1F("QQMass1Glob1Cal_passmu5",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.); 
  // QQMass2Glob_passmu9              = new TH1F("QQMass2Glob_passmu9",  "Invariant mass (2 global muons)", 120, 0.,15.);
  // QQMass1Glob1Trk_passmu9          = new TH1F("QQMass1Glob1Trk_passmu9",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  // QQMass1Glob1Cal_passmu9          = new TH1F("QQMass1Glob1Cal_passmu9",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  QQMass2GlobPT6_passmu5           = new TH1F("QQMass2GlobPT6_passmu5",  "Invariant mass (2 global muons)", 120, 0.,15.);
  QQMass1Glob1TrkPT6_passmu5       = new TH1F("QQMass1Glob1TrkPT6_passmu5",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  QQMass1Glob1CalPT6_passmu5       = new TH1F("QQMass1Glob1CalPT6_passmu5",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  QQMass2Glob_pass2muOpen              = new TH1F("QQMass2Glob_pass2muOpen",  "Invariant mass (2 global muons)", 120, 0.,15.);
  QQMass1Glob1Trk_pass2muOpen          = new TH1F("QQMass1Glob1Trk_pass2muOpen",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  QQMass1Glob1Cal_pass2muOpen          = new TH1F("QQMass1Glob1Cal_pass2muOpen",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  // WSMass2Glob_pass2muOpen              = new TH1F("WSMass2Glob_pass2muOpen",  "Invariant mass (2 global muons)", 120, 0.,15.);
  // WSMass1Glob1Trk_pass2muOpen          = new TH1F("WSMass1Glob1Trk_pass2muOpen",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  // WSMass1Glob1Cal_pass2muOpen          = new TH1F("WSMass1Glob1Cal_pass2muOpen",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  QQMass2Glob_pass2mu0              = new TH1F("QQMass2Glob_pass2mu0",  "Invariant mass (2 global muons)", 120, 0.,15.);
  QQMass1Glob1Trk_pass2mu0          = new TH1F("QQMass1Glob1Trk_pass2mu0",  "Invariant mass (1 global + 1 tracker muon)", 120, 0.,15.);
  QQMass1Glob1Cal_pass2mu0          = new TH1F("QQMass1Glob1Cal_pass2mu0",  "Invariant mass (1 global + 1 calo muon)", 120, 0.,15.);
  			       
  // deltaR		       
  hMcRecoGlobMuDeltaR               = new TH1F("hMcRecoGlobMuDeltaR",  "MC-reco matching #Delta R (global muons)", 100, 0.,0.5);
  hMcRecoTrkMuDeltaR                = new TH1F("hMcRecoTrkMuDeltaR",  "MC-reco matching #Delta R (tracker muons)", 100, 0.,0.5);
  hMcRecoCalMuDeltaR                = new TH1F("hMcRecoCalMuDeltaR",  "MC-reco matching #Delta R (calo muons)", 100, 0.,0.5);
  			       
  // mc-truth matching (global)
  hMcRightMunPixHits            = new TH1F("hMcRightMunPixHits",  "number of pixel hits - MC matched (muons)", 6, -0.5, 5.5);
  hMcWrongMunPixHits            = new TH1F("hMcWrongMunPixHits",  "number of pixel hits - MC unmatched (muons)", 6, -0.5, 5.5);
  hMcRightMud0                  = new TH1F("hMcRightMud0",  "d0 - MC matched (muons)", 100, 0., 10.);
  hMcWrongMud0                  = new TH1F("hMcWrongMud0",  "d0 - MC unmatched (muons)", 100, 0., 10.);
  hMcRightMudz                  = new TH1F("hMcRightMudz",  "dz - MC matched (muons)", 100, 0., 50.);
  hMcWrongMudz                  = new TH1F("hMcWrongMudz",  "dz - MC unmatched (muons)", 100, 0., 50.);
  hMcRightGlbMuGlobalchi2          = new TH1F("hMcRightGlbMuGlobalchi2",  "global chi2 - MC matched (global muons)", 100, -0.5, 30.);
  hMcWrongGlbMuGlobalchi2          = new TH1F("hMcWrongGlbMuGlobalchi2",  "global chi2 - MC unmatched (global muons)", 100, -0.5, 30.);
  hMcRightGlbMunMuHits          = new TH1F("hMcRightGlbMunMuHits",  "number of muon hits - MC matched (global muons)", 100, -0.5, 99.5);
  hMcWrongGlbMunMuHits          = new TH1F("hMcWrongGlbMunMuHits",  "number of muon hits - MC matched (global muons)", 100, -0.5, 99.5);

  // mc truth matching - global + global
  hMcRightGlbGlbMuMass             = new TH1F("hMcRightGlbGlbMuMass",  "Inv. mass - MC matched (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcWrongGlbGlbMuMass             = new TH1F("hMcWrongGlbGlbMuMass",  "Inv. mass - MC unmatched (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcPPGlbGlbMuMass                = new TH1F("hMcPPGlbGlbMuMass",  "Inv. mass - plus-plus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcMMGlbGlbMuMass                = new TH1F("hMcMMGlbGlbMuMass",  "Inv. mass - minus-minus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcSqrtGlbGlbMuMass              = new TH1F("hMcSqrtGlbGlbMuMass",  "Inv. mass - combintion of wrong sign (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcRightGlbGlbMuLife             = new TH1F("hMcRightGlbGlbMuLife",  "c #tau - MC matched (global+global muons)", 90, -1.0,3.5);
  hMcWrongGlbGlbMuLife             = new TH1F("hMcWrongGlbGlbMuLife",  "c #tau - MC unmatched (global+global muons)", 90, -1.0,3.5);
  hMcRightGlbGlbMuPt               = new TH1F("hMcRightGlbGlbMuPt",  "P_{T} - MC matched (global+global muons)", 60, 0.,60.);
  hMcWrongGlbGlbMuPt               = new TH1F("hMcWrongGlbGlbMuPt",  "P_{T} - MC unmatched (global+global muons)", 60, 0.,60.);
  hMcRightGlbGlbMuEta              = new TH1F("hMcRightGlbGlbMuEta",  "#eta - MC matched (global+global muons)", 60, -2.5,2.5);
  hMcWrongGlbGlbMuEta              = new TH1F("hMcWrongGlbGlbMuEta",  "#eta - MC unmatched (global+global muons)", 60, -2.5,2.5);
  hMcRightGlbGlbMuVtxProb          = new TH1F("hMcRightGlbGlbMuVtxProb",  "Vertex probability - MC matched (global+global muons)", 1000, 0.0,1.0);
  hMcWrongGlbGlbMuVtxProb          = new TH1F("hMcWrongGlbGlbMuVtxProb",  "Vertex probability - MC unmatched (global+global muons)", 1000, 0.0,1.0);
  			       
  // mc truth matching - trk   
  hMcRightMuPt                  = new TH1F("hMcRightMuPt",  "pT  - MC matched (muons)", 50, 0.,10.);
  hMcWrongMuPt                  = new TH1F("hMcWrongMuPt",  "pT - MC unmatched (muons)", 50, 0.,10.);
  hMcRightMuP                  = new TH1F("hMcRightMuP",  "p  - MC matched (muons)", 100, 0.,20.);
  hMcWrongMuP                  = new TH1F("hMcWrongMuP",  "p - MC unmatched (muons)", 100, 0.,20.);
  hMcRightTrkMuCaloComp            = new TH1F("hMcRightTrkMuCaloComp",  "calo compatibility - MC matched (tracker muons)", 60, 0.5,1.1);
  hMcWrongTrkMuCaloComp            = new TH1F("hMcWrongTrkMuCaloComp",  "calo compatibility - MC unmatched (tracker muons)", 60, 0.5,1.1);
  hMcRightTrkMuChi2                = new TH1F("hMcRightTrkMuChi2",  "chi2  - MC matched (tracker muons)", 50, -0.5, 6.5);
  hMcWrongTrkMuChi2                = new TH1F("hMcWrongTrkMuChi2",  "chi2 - MC unmatched (tracker muons)", 50, -0.5,6.5);
  hMcRightMuNhits               = new TH1F("hMcRightMuNhits",  "chi2  - MC matched (tracker muons)", 30, 0.5, 30.5);
  hMcWrongMuNhits               = new TH1F("hMcWrongMuNhits",  "chi2 - MC unmatched (tracker muons)", 30, 0.5,30.5);
  hMcRightTrkBit4                  = new TH1F("hMcRightTrkBit4",  "2DCompatibilityLoose bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit4                  = new TH1F("hMcWrongTrkBit4",  "2DCompatibilityLoose bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit5                  = new TH1F("hMcRightTrkBit5",  "2DCompatibilityTight bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit5                  = new TH1F("hMcWrongTrkBit5",  "2DCompatibilityTight bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit8                  = new TH1F("hMcRightTrkBit8",  "StationOptimizedLowPtLoose bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit8                  = new TH1F("hMcWrongTrkBit8",  "StationOptimizedLowPtLoose bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBit9                  = new TH1F("hMcRightTrkBit9",  "StationOptimizedLowPtTight bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBit9                  = new TH1F("hMcWrongTrkBit9",  "StationOptimizedLowPtTight bit - MC unmatched (tracker muons)", 4, -1.5,2.5);
  hMcRightTrkBitNew                = new TH1F("hMcRightTrkBitNew",  "TMLastStationAngTight bit - MC matched (tracker muons)", 4, -1.5,2.5);
  hMcWrongTrkBitNew                = new TH1F("hMcWrongTrkBitNew",  "TMLastStationAngTight - MC unmatched (tracker muons)", 4, -1.5,2.5);
  			       
  // mc truth matching - global + trk
  hMcRightGlbTrkMuMass            = new TH1F("hMcRightGlbTrkMuMass",  "Inv. mass - MC matched (global+tracker muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcWrongGlbTrkMuMass            = new TH1F("hMcWrongGlbTrkMuMass",  "Inv. mass - MC unmatched (global+tracker muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcPPGlbTrkMuMass                = new TH1F("hMcPPGlbTrkMuMass",  "Inv. mass - plus-plus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcMMGlbTrkMuMass                = new TH1F("hMcMMGlbTrkMuMass",  "Inv. mass - minus-minus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcSqrtGlbTrkMuMass              = new TH1F("hMcSqrtGlbTrkMuMass",  "Inv. mass - combintion of wrong sign (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcRightGlbTrkMuLife            = new TH1F("hMcRightGlbTrkMuLife",  "c #tau - MC matched (global+tracker muons)", 90, -1.0,3.5);
  hMcWrongGlbTrkMuLife            = new TH1F("hMcWrongGlbTrkMuLife",  "c #tau - MC unmatched (global+tracker muons)", 90, -1.0,3.5);
  hMcRightGlbTrkMuPt               = new TH1F("hMcRightGlbTrkMuPt",  "P_{T} - MC matched (global+tracker muons)", 60, 0.,60.);
  hMcWrongGlbTrkMuPt               = new TH1F("hMcWrongGlbTrkMuPt",  "P_{T} - MC unmatched (global+tracker muons)", 60, 0.,60.);
  hMcRightGlbTrkMuEta              = new TH1F("hMcRightGlbTrkMuEta",  "#eta - MC matched (global+tracker muons)", 60, -2.5,2.5);
  hMcWrongGlbTrkMuEta              = new TH1F("hMcWrongGlbTrkMuEta",  "#eta - MC unmatched (global+tracker muons)", 60, -2.5,2.5);
  hMcRightGlbTrkMuVtxProb          = new TH1F("hMcRightGlbTrkMuVtxProb",  "Vertex probability - MC matched (global+tracker muons)", 1000, 0.0,1.0);
  hMcWrongGlbTrkMuVtxProb          = new TH1F("hMcWrongGlbTrkMuVtxProb",  "Vertex probability - MC unmatched (global+tracker muons)", 1000, 0.0,1.0);
  			       
  // mc truth matching - trk + trk
  hMcRightTrkTrkMuMass            = new TH1F("hMcRightTrkTrkMuMass",  "Inv. mass - MC matched (tracker+tracker muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcWrongTrkTrkMuMass            = new TH1F("hMcWrongTrkTrkMuMass",  "Inv. mass - MC unmatched (tracker+tracker muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcPPTrkTrkMuMass                = new TH1F("hMcPPTrkTrkMuMass",  "Inv. mass - plus-plus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcMMTrkTrkMuMass                = new TH1F("hMcMMTrkTrkMuMass",  "Inv. mass - minus-minus (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcSqrtTrkTrkMuMass              = new TH1F("hMcSqrtTrkTrkMuMass",  "Inv. mass - combintion of wrong sign (global+global muons)", 120, JpsiMassMinSide,JpsiMassMaxSide);
  hMcRightTrkTrkMuLife            = new TH1F("hMcRightTrkTrkMuLife",  "c #tau - MC matched (tracker+tracker muons)", 90, -1.0,3.5);
  hMcWrongTrkTrkMuLife            = new TH1F("hMcWrongTrkTrkMuLife",  "c #tau - MC unmatched (tracker+tracker muons)", 90, -1.0,3.5);
  hMcRightTrkTrkMuPt               = new TH1F("hMcRightTrkTrkMuPt",  "P_{T} - MC matched (tracker+tracker muons)", 60, 0.,60.);
  hMcWrongTrkTrkMuPt               = new TH1F("hMcWrongTrkTrkMuPt",  "P_{T} - MC unmatched (tracker+tracker muons)", 60, 0.,60.);
  hMcRightTrkTrkMuEta              = new TH1F("hMcRightTrkTrkMuEta",  "#eta - MC matched (tracker+tracker muons)", 60, -2.5,2.5);
  hMcWrongTrkTrkMuEta              = new TH1F("hMcWrongTrkTrkMuEta",  "#eta - MC unmatched (tracker+tracker muons)", 60, -2.5,2.5);
  hMcRightTrkTrkMuVtxProb          = new TH1F("hMcRightTrkTrkMuVtxProb",  "Vertex probability - MC matched (tracker+tracker muons)", 1000, 0.0,1.0);
  hMcWrongTrkTrkMuVtxProb          = new TH1F("hMcWrongTrkTrkMuVtxProb",  "Vertex probability - MC unmatched (tracker+tracker muons)", 1000, 0.0,1.0);

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
  
  cout << "Total number of events = " << nEvents << endl;
  cout << "Total number of passed candidates = " << passedCandidates << endl;
  cout << "DoubleMuOpen-matched candidates: standard way = " << matchOldCandidates << endl;
  cout << "DoubleMuOpen-matched candidates: new way = " << matchNewCandidates << endl;

  TFile fOut(_histfilename.c_str(), "RECREATE");
  fOut.cd();
  
  QQMass2Glob_passmu3           -> Write();        
  QQMass1Glob1Trk_passmu3       -> Write(); 
  QQMass1Glob1Cal_passmu3       -> Write();
  QQMass2GlobPT6_passmu3        -> Write();        
  QQMass1Glob1TrkPT6_passmu3    -> Write(); 
  QQMass1Glob1CalPT6_passmu3    -> Write();
  // WSMass2Glob_passmu3           -> Write();        
  // WSMass1Glob1Trk_passmu3       -> Write(); 
  // WSMass1Glob1Cal_passmu3       -> Write();
  QQMass2Glob_passmu5           -> Write();
  QQMass1Glob1Trk_passmu5       -> Write();
  QQMass1Glob1Cal_passmu5       -> Write();
  QQMass2GlobPT6_passmu5        -> Write();
  QQMass1Glob1TrkPT6_passmu5    -> Write();
  QQMass1Glob1CalPT6_passmu5    -> Write();
  // QQMass2Glob_passmu9        -> Write();
  // QQMass1Glob1Trk_passmu9    -> Write();
  // QQMass1Glob1Cal_passmu9    -> Write();
  QQMass2Glob_pass2muOpen          -> Write();
  QQMass1Glob1Trk_pass2muOpen      -> Write();
  QQMass1Glob1Cal_pass2muOpen      -> Write();
  // WSMass2Glob_pass2muOpen          -> Write();
  // WSMass1Glob1Trk_pass2muOpen      -> Write();
  // WSMass1Glob1Cal_pass2muOpen      -> Write();
  QQMass2Glob_pass2mu0          -> Write();  
  QQMass1Glob1Trk_pass2mu0      -> Write();  
  QQMass1Glob1Cal_pass2mu0      -> Write();  
  			     
  hMcRecoGlobMuDeltaR           -> Write();   
  hMcRecoTrkMuDeltaR            -> Write();   
  hMcRecoCalMuDeltaR            -> Write();
  			     
  hMcRightMunPixHits         -> Write(); 
  hMcWrongMunPixHits         -> Write(); 
  hMcRightMud0               -> Write(); 
  hMcWrongMud0               -> Write(); 
  hMcRightMudz               -> Write(); 
  hMcWrongMudz               -> Write(); 
  hMcRightGlbMuGlobalchi2       -> Write(); 
  hMcWrongGlbMuGlobalchi2       -> Write();
  hMcRightGlbMunMuHits          -> Write(); 
  hMcWrongGlbMunMuHits          -> Write();
  			     
  hMcRightGlbGlbMuMass          -> Write(); 
  hMcWrongGlbGlbMuMass          -> Write(); 
  hMcPPGlbGlbMuMass             -> Write();
  hMcMMGlbGlbMuMass             -> Write();
  hMcRightGlbGlbMuLife          -> Write(); 
  hMcWrongGlbGlbMuLife          -> Write();
  hMcRightGlbGlbMuPt            -> Write(); 
  hMcWrongGlbGlbMuPt            -> Write(); 
  hMcRightGlbGlbMuEta           -> Write(); 
  hMcWrongGlbGlbMuEta           -> Write();
  hMcRightGlbGlbMuVtxProb       -> Write(); 
  hMcWrongGlbGlbMuVtxProb       -> Write(); 
  			     
  hMcRightMuPt               -> Write(); 
  hMcWrongMuPt               -> Write(); 
  hMcRightMuP               -> Write(); 
  hMcWrongMuP               -> Write(); 
  hMcRightTrkBit4               -> Write(); 
  hMcWrongTrkBit4               -> Write(); 
  hMcRightTrkBit5               -> Write(); 
  hMcWrongTrkBit5               -> Write(); 
  hMcRightTrkBit8               -> Write(); 
  hMcWrongTrkBit8               -> Write(); 
  hMcRightTrkBit9               -> Write(); 
  hMcWrongTrkBit9               -> Write();
  hMcRightTrkBitNew             -> Write(); 
  hMcWrongTrkBitNew             -> Write();
  hMcRightTrkMuChi2             -> Write();   
  hMcWrongTrkMuChi2             -> Write(); 
  hMcRightMuNhits            -> Write(); 
  hMcWrongMuNhits            -> Write();
  hMcRightTrkMuCaloComp         -> Write();   
  hMcWrongTrkMuCaloComp         -> Write(); 

  hMcRightGlbTrkMuMass          -> Write();
  hMcWrongGlbTrkMuMass        	-> Write();
  hMcPPGlbTrkMuMass             -> Write();
  hMcMMGlbTrkMuMass             -> Write();
  hMcRightGlbTrkMuLife        	-> Write();
  hMcWrongGlbTrkMuLife        	-> Write();
  hMcRightGlbTrkMuPt            -> Write(); 
  hMcWrongGlbTrkMuPt            -> Write(); 
  hMcRightGlbTrkMuEta           -> Write(); 
  hMcWrongGlbTrkMuEta           -> Write();
  hMcRightGlbTrkMuVtxProb       -> Write();   
  hMcWrongGlbTrkMuVtxProb       -> Write(); 

  hMcRightTrkTrkMuMass          -> Write();
  hMcWrongTrkTrkMuMass        	-> Write();
  hMcPPTrkTrkMuMass             -> Write();
  hMcMMTrkTrkMuMass             -> Write();
  hMcRightTrkTrkMuLife        	-> Write();
  hMcWrongTrkTrkMuLife        	-> Write();
  hMcRightTrkTrkMuPt            -> Write(); 
  hMcWrongTrkTrkMuPt            -> Write(); 
  hMcRightTrkTrkMuEta           -> Write(); 
  hMcWrongTrkTrkMuEta           -> Write();
  hMcRightTrkTrkMuVtxProb       -> Write();   
  hMcWrongTrkTrkMuVtxProb       -> Write(); 
  			     
  hMcRightCalMuChi2             -> Write(); 
  hMcWrongCalMuChi2             -> Write(); 
  hMcRightCalMuNhits            -> Write(); 
  hMcWrongCalMuNhits            -> Write(); 
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
   
  // find correct combination of ++ and -- candidates
  for (int iBin = 0; iBin <= hMcPPGlbGlbMuMass->GetNbinsX(); iBin++) {
    float pp = hMcPPGlbGlbMuMass->GetBinContent(iBin);
    float mm = hMcMMGlbGlbMuMass->GetBinContent(iBin);
    hMcSqrtGlbGlbMuMass->SetBinContent(iBin,2*sqrt(pp*mm));
    hMcSqrtGlbGlbMuMass->SetBinError(iBin,sqrt(pp + mm));  // do the calculation, if you don't believe it
  }
  for (int iBin = 0; iBin <= hMcPPGlbTrkMuMass->GetNbinsX(); iBin++) {
    float pp = hMcPPGlbTrkMuMass->GetBinContent(iBin);
    float mm = hMcMMGlbTrkMuMass->GetBinContent(iBin);
    hMcSqrtGlbTrkMuMass->SetBinContent(iBin,2*sqrt(pp*mm));
    hMcSqrtGlbTrkMuMass->SetBinError(iBin,sqrt(pp + mm));  // do the calculation, if you don't believe it
  }
  for (int iBin = 0; iBin <= hMcPPTrkTrkMuMass->GetNbinsX(); iBin++) {
    float pp = hMcPPTrkTrkMuMass->GetBinContent(iBin);
    float mm = hMcMMTrkTrkMuMass->GetBinContent(iBin);
    hMcSqrtTrkTrkMuMass->SetBinContent(iBin,2*sqrt(pp*mm));
    hMcSqrtTrkTrkMuMass->SetBinError(iBin,sqrt(pp + mm));  // do the calculation, if you don't believe it
  }

  hMcSqrtGlbGlbMuMass           -> Write();
  hMcSqrtGlbTrkMuMass           -> Write();
  hMcSqrtTrkTrkMuMass           -> Write();

  fOut.Close();

  TFile fOut2(_datasetname.c_str(), "RECREATE");
  fOut2.cd();
  data->Write();
  fOut2.Close();

}

void 
JPsiAnalyzerPAT::fillHistosAndDS(unsigned int theCat, const pat::CompositeCandidate* aCand, const edm::Event& iEvent){
  
  const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(aCand->daughter("muon1"));
  const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(aCand->daughter("muon2"));
  
  float theMass = aCand->mass();

  // Only sidebands
  /* if (aCand->mass() < 2.3 || aCand->mass() > 6.5  ||
      (aCand->mass() < 3.3 && aCand->mass() > 2.9) ||
      (aCand->mass() < 3.9 && aCand->mass() > 3.5)) 
      return;*/

  float theRapidity = aCand->rapidity();
  if (!_useRapidity) theRapidity = theRapidity;

  float theCtau; 
  if (_useBS) {theCtau = 10.*aCand->userFloat("ppdlBS");}
  else {theCtau = 10.*aCand->userFloat("ppdlPV");}

  float theCtauErr; 
  if (_useBS) {theCtauErr = 10.*aCand->userFloat("ppdlErrBS");}
  else {theCtauErr = 10.*aCand->userFloat("ppdlErrPV");}

  // MC matching
  reco::GenParticleRef genJpsi = aCand->genParticleRef();
  bool isMatched = (genJpsi.isAvailable() && genJpsi->pdgId() == 443);
  if (isMatched && _removeSignal) return;

  reco::GenParticleRef genMu1 = muon1->genParticleRef();
  reco::GenParticleRef genMu2 = muon2->genParticleRef();
  bool isMuMatched = (genMu1.isAvailable() && genMu2.isAvailable() && 
		      genMu1->pdgId()*genMu2->pdgId() == -169 && 
		      genMu1->momentum().rho() > 2.5 && genMu2->momentum().rho() > 2.5);
  if (isMuMatched && _removeMuons) return;

  // PAT trigger matches (new way)
  
  // PUT HERE THE *LAST FILTERS* OF THE BITS YOU LIKE
  static const unsigned int NTRIGGERS = 5;
  // MC 8E29
  bool isTriggerMatched[NTRIGGERS];
  bool isTriggerMatchedNew;
  string HLTLastFilters[NTRIGGERS] = {"hltDoubleMuLevel1PathL1OpenFiltered", // BIT HLT_L1DoubleMuOpen 
				      "hltSingleMu3L3Filtered3",             // BIT HLT_Mu3  
				      "hltSingleMu5L3Filtered5",             // BIT HLT_Mu5 
				      "hltDiMuonL3PreFiltered0",             // BIT HLT_DoubleMu0 
				      "hltDiMuonL3PreFiltered"};             // BIT HLT_DoubleMu3
  // early data
  /* string HLTLastFilters[NTRIGGERS] = {"hltDoubleMuLevel1PathL1OpenFiltered", // BIT HLT_L1DoubleMuOpen 
                                      "hltL2Mu0L2Filtered0",                 // BIT HLT_L2Mu0
				      "hltSingleMu3L2Filtered3",             // BIT HLT_L2Mu3  
				      "hltMu0L1MuOpenL3Filtered0",           // BIT HLT_Mu0_L1MuOpen
				      "hltMu0TrackJpsiTrackMassFiltered"};   // BIT HLT_Mu0_Track0_Jpsi */
  
  // Trigger passed

  for (unsigned int iTr = 0; iTr<NTRIGGERS; iTr++ ) {
    const pat::TriggerObjectStandAloneCollection mu1HLTMatches = muon1->triggerObjectMatchesByFilter( HLTLastFilters[iTr] ); 
    const pat::TriggerObjectStandAloneCollection mu2HLTMatches = muon2->triggerObjectMatchesByFilter( HLTLastFilters[iTr] );
    bool pass1 = mu1HLTMatches.size() > 0;
    bool pass2 = mu2HLTMatches.size() > 0;
    if (iTr == 1 || iTr == 2) {  // single triggers here
      isTriggerMatched[iTr] = pass1 || pass2;
    } else {                     // double triggers here
      isTriggerMatched[iTr] = pass1 && pass2;
    }
    if (iTr == 0) {
      if ( fabs(muon1->eta()) > 1.2 ) pass1 = (pass1 || muon1->userInt("muonL1MatchExtended") > 0);
      if ( fabs(muon2->eta()) > 1.2 ) pass2 = (pass2 || muon2->userInt("muonL1MatchExtended") > 0);              // double triggers here
      isTriggerMatchedNew = pass1 && pass2;
    }
  }

  if (isTriggerMatched[1]) { // pass Bit 1
    if (muon1->charge()*muon2->charge() < 0) {
      if (theCat == 0) QQMass2Glob_passmu3->Fill(theMass);          
      if (theCat == 1) QQMass1Glob1Trk_passmu3->Fill(theMass);
      if (theCat == 3) QQMass1Glob1Cal_passmu3->Fill(theMass);    
      if (theCat == 0 && aCand->pt() < 6.0) QQMass2GlobPT6_passmu3->Fill(theMass);     
      if (theCat == 1 && aCand->pt() < 6.0) QQMass1Glob1TrkPT6_passmu3->Fill(theMass);
      if (theCat == 3 && aCand->pt() < 6.0) QQMass1Glob1CalPT6_passmu3->Fill(theMass);
      // } else {
      // if (theCat == 0) WSMass2Glob_passmu3->Fill(theMass);          
      // if (theCat == 1) WSMass1Glob1Trk_passmu3->Fill(theMass);
      // if (theCat == 3) WSMass1Glob1Cal_passmu3->Fill(theMass);       
    }
  }
  if (isTriggerMatched[2]) { // pass Bit 2
    if (muon1->charge()*muon2->charge() < 0) {
      if (theCat == 0) QQMass2Glob_passmu5->Fill(theMass);          
      if (theCat == 1) QQMass1Glob1Trk_passmu5->Fill(theMass);
      if (theCat == 3) QQMass1Glob1Cal_passmu5->Fill(theMass);    
      if (theCat == 0 && aCand->pt() < 6.0) QQMass2GlobPT6_passmu5->Fill(theMass);     
      if (theCat == 1 && aCand->pt() < 6.0) QQMass1Glob1TrkPT6_passmu5->Fill(theMass);
      if (theCat == 3 && aCand->pt() < 6.0) QQMass1Glob1CalPT6_passmu5->Fill(theMass);
    } 
  }
  if (isTriggerMatched[0]) { // pass Bit 0
    if (muon1->charge()*muon2->charge() < 0) {
      if (theCat == 0) QQMass2Glob_pass2muOpen->Fill(theMass);          
      if (theCat == 1) QQMass1Glob1Trk_pass2muOpen->Fill(theMass);
      if (theCat == 3) QQMass1Glob1Cal_pass2muOpen->Fill(theMass);    
      // } else {
      // if (theCat == 0) WSMass2Glob_pass2muOpen->Fill(theMass);          
      // if (theCat == 1) WSMass1Glob1Trk_pass2muOpen->Fill(theMass);
      // if (theCat == 3) WSMass1Glob1Cal_pass2muOpen->Fill(theMass);       
    }
  }
  if (isTriggerMatched[3]) { // pass Bit 3
    if (muon1->charge()*muon2->charge() < 0) {
      if (theCat == 0) QQMass2Glob_pass2mu0->Fill(theMass);          
      if (theCat == 1) QQMass1Glob1Trk_pass2mu0->Fill(theMass);
      if (theCat == 3) QQMass1Glob1Cal_pass2mu0->Fill(theMass);    
    }
  }  
  
  // Signal / background J/psi 	
  if (muon1->charge()*muon2->charge() < 0) { 
    if (isMatched) {
      if (theCat == 0) {
	hMcRightGlbGlbMuMass->Fill(theMass);       
	hMcRightGlbGlbMuLife->Fill(theCtau);
        hMcRightGlbGlbMuPt->Fill(aCand->pt());
        hMcRightGlbGlbMuEta->Fill(theRapidity);             
	hMcRightGlbGlbMuVtxProb->Fill(aCand->userFloat("vProb")); 
      } else if (theCat == 1) {
        hMcRightGlbTrkMuMass->Fill(theMass);       
	hMcRightGlbTrkMuLife->Fill(theCtau);
	hMcRightGlbTrkMuPt->Fill(aCand->pt());
        hMcRightGlbTrkMuEta->Fill(theRapidity);
	hMcRightGlbTrkMuVtxProb->Fill(aCand->userFloat("vProb")); 
      } else if (theCat == 2) {
        hMcRightTrkTrkMuMass->Fill(theMass);       
	hMcRightTrkTrkMuLife->Fill(theCtau);
	hMcRightTrkTrkMuPt->Fill(aCand->pt());
        hMcRightTrkTrkMuEta->Fill(theRapidity);
	hMcRightTrkTrkMuVtxProb->Fill(aCand->userFloat("vProb"));    
      } else if (theCat == 3) {
        hMcRightCalGlbMuDeltaR->Fill(deltaR(muon1->eta(),muon1->phi(),muon2->eta(),muon2->phi()));     
	hMcRightCalGlbMuMass->Fill(theMass);           
	hMcRightCalGlbMuVtxChi2->Fill(aCand->userFloat("vNChi2"));  
	hMcRightCalGlbMuS->Fill(sqrt(pow(muon1->track()->dxy(RefVtx)/muon1->track()->d0Error(),2) + pow(muon2->track()->dxy(RefVtx)/muon2->track()->d0Error(),2)));
	hMcRightCalGlbMucosAlpha->Fill(aCand->userFloat("cosAlpha"));
      }   

    } else {
    
      if (theCat == 0) {
	hMcWrongGlbGlbMuMass->Fill(theMass);       
	hMcWrongGlbGlbMuLife->Fill(theCtau);
        hMcWrongGlbGlbMuPt->Fill(aCand->pt());
        hMcWrongGlbGlbMuEta->Fill(theRapidity);             
	hMcWrongGlbGlbMuVtxProb->Fill(aCand->userFloat("vProb")); 
      } else if (theCat == 1) {
        hMcWrongGlbTrkMuMass->Fill(theMass);       
	hMcWrongGlbTrkMuLife->Fill(theCtau);
	hMcWrongGlbTrkMuPt->Fill(aCand->pt());
        hMcWrongGlbTrkMuEta->Fill(theRapidity);
	hMcWrongGlbTrkMuVtxProb->Fill(aCand->userFloat("vProb")); 
      } else if (theCat == 2) {
        hMcWrongTrkTrkMuMass->Fill(theMass);       
	hMcWrongTrkTrkMuLife->Fill(theCtau);
	hMcWrongTrkTrkMuPt->Fill(aCand->pt());
        hMcWrongTrkTrkMuEta->Fill(theRapidity);
	hMcWrongTrkTrkMuVtxProb->Fill(aCand->userFloat("vProb"));    
      } else if (theCat == 3) {    
	hMcWrongCalGlbMuMass->Fill(theMass);           
	hMcWrongCalGlbMuVtxChi2->Fill(aCand->userFloat("vNChi2"));  
	hMcWrongCalGlbMuS->Fill(sqrt(pow(muon1->track()->dxy(RefVtx)/muon1->track()->d0Error(),2) + pow(muon2->track()->dxy(RefVtx)/muon2->track()->d0Error(),2)));
	hMcWrongCalGlbMucosAlpha->Fill(aCand->userFloat("cosAlpha"));
      }   

    }
  
    // Signal / background muons
    char whichMuon[6];
    for(unsigned int i = 1; i<=2; i++) {
      
      sprintf(whichMuon,"muon%d",i);
      const pat::Muon* thisMuon = dynamic_cast<const pat::Muon*>(aCand->daughter(whichMuon));

      reco::GenParticleRef genMu = thisMuon->genParticleRef();
      TrackRef iTrack = thisMuon->innerTrack();
      const reco::HitPattern& p = iTrack->hitPattern();

      if (isMatched) {
	hMcRightMunPixHits->Fill(p.pixelLayersWithMeasurement());
	hMcRightMud0->Fill(fabs(iTrack->dxy(RefVtx)));
	hMcRightMudz->Fill(fabs(iTrack->dz(RefVtx)));
	hMcRightMuNhits->Fill(iTrack->found());
	hMcRightMuPt->Fill(thisMuon->pt());
        hMcRightMuP->Fill(thisMuon->p());
	if (thisMuon->isGlobalMuon()) { 
          TrackRef gTrack = thisMuon->globalTrack();
          const reco::HitPattern& q = gTrack->hitPattern();
	  if (genMu.isNonnull()) hMcRecoGlobMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
	  hMcRightGlbMuGlobalchi2->Fill(gTrack->chi2()/gTrack->ndof());
          hMcRightGlbMunMuHits->Fill(q.numberOfValidMuonHits());
	} else if (thisMuon->isTrackerMuon()) {     // notice exclusiveness!  
	  if (genMu.isNonnull()) hMcRecoTrkMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
          hMcRightMunPixHits->Fill(p.pixelLayersWithMeasurement());
          hMcRightTrkBit4->Fill(int(thisMuon->muonID("TM2DCompatibilityLoose")));
          hMcRightTrkBit5->Fill(int(thisMuon->muonID("TM2DCompatibilityTight")));
	  hMcRightTrkBit8->Fill(int(thisMuon->muonID("TMLastStationOptimizedLowPtLoose")));
	  hMcRightTrkBit9->Fill(int(thisMuon->muonID("TMLastStationOptimizedLowPtTight")));
          hMcRightTrkBitNew->Fill(int(thisMuon->muonID("TMLastStationAngTight")));
          hMcRightTrkMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
	  hMcRightTrkMuCaloComp->Fill(thisMuon->caloCompatibility()); 
	} else if (thisMuon->isCaloMuon()) {
	  if (genMu.isNonnull()) hMcRecoCalMuDeltaR->Fill(deltaR(thisMuon->eta(),thisMuon->phi(),genMu->eta(),genMu->phi()));
	  hMcRightCalMuPt->Fill(thisMuon->pt());
          hMcRightCalMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcRightCalMuNhits->Fill(iTrack->found());    
	  hMcRightCalMuCaloComp->Fill(thisMuon->caloCompatibility());   
	}
      } else {
	hMcWrongMunPixHits->Fill(p.pixelLayersWithMeasurement());
        hMcWrongMud0->Fill(fabs(iTrack->dxy(RefVtx)));
	hMcWrongMudz->Fill(fabs(iTrack->dz(RefVtx)));
        hMcWrongMuNhits->Fill(iTrack->found());
        hMcWrongMuPt->Fill(thisMuon->pt());
	hMcWrongMuP->Fill(thisMuon->p());
	if (thisMuon->isGlobalMuon()) {
	  TrackRef gTrack = thisMuon->globalTrack();
          const reco::HitPattern& q = gTrack->hitPattern();
	  hMcWrongGlbMuGlobalchi2->Fill(gTrack->chi2()/gTrack->ndof());
          hMcWrongGlbMunMuHits->Fill(q.numberOfValidMuonHits());
	} else if (thisMuon->isTrackerMuon()) {     // notice exclusiveness!
          hMcWrongTrkBit4->Fill((int)thisMuon->muonID("TM2DCompatibilityLoose"));
          hMcWrongTrkBit5->Fill((int)thisMuon->muonID("TM2DCompatibilityTight"));
	  hMcWrongTrkBit8->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtLoose"));
	  hMcWrongTrkBit9->Fill((int)thisMuon->muonID("TMLastStationOptimizedLowPtTight"));
	  hMcWrongTrkBitNew->Fill(int(thisMuon->muonID("TMLastStationAngTight")));
          hMcWrongTrkMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
	  hMcWrongTrkMuCaloComp->Fill(thisMuon->caloCompatibility());
	} else if (thisMuon->isCaloMuon()) {
	  hMcWrongCalMuPt->Fill(thisMuon->pt());
          hMcWrongCalMuChi2->Fill(iTrack->chi2()/iTrack->ndof());
          hMcWrongCalMuNhits->Fill(iTrack->found());    
	  hMcWrongCalMuCaloComp->Fill(thisMuon->caloCompatibility());                    
	}
      }
    }
  // wrong-sign J/psi's
  } else if (muon1->charge() + muon2->charge() == 2) {   

    if (theCat == 0) hMcPPGlbGlbMuMass->Fill(theMass);
    if (theCat == 1) hMcPPGlbTrkMuMass->Fill(theMass);
    if (theCat == 2) hMcPPTrkTrkMuMass->Fill(theMass);

  } else if (muon1->charge() + muon2->charge() == -2) {
    
    if (theCat == 0) hMcMMGlbGlbMuMass->Fill(theMass);
    if (theCat == 1) hMcMMGlbTrkMuMass->Fill(theMass);
    if (theCat == 2) hMcMMTrkTrkMuMass->Fill(theMass);
     
  } 

  // Now the RooDataSet

  float tnpeff = 0.;
  float tnpefferr = 0.;

  if (_storeefficiency) {
    // to be done
  }

  if (muon1->charge()*muon2->charge() < 0 &&
      theMass > JpsiMassMin && theMass < JpsiMassMax && 
      theCtau > JpsiCtMin && theCtau < JpsiCtMax && 
      aCand->pt() > JpsiPtMin && aCand->pt() < JpsiPtMax && 
      fabs(theRapidity) > JpsiEtaMin && fabs(theRapidity) < JpsiEtaMax) {
	
    passedCandidates++;
    if (isTriggerMatched[0]) matchOldCandidates++;
    if (isTriggerMatchedNew) matchNewCandidates++;
    
    if (_writeOutCands) *theTextFile << iEvent.id().run() << "\t" << iEvent.luminosityBlock() << "\t" << iEvent.id().event() << "\t" << theMass << "\n";

    JpsiPt->setVal(aCand->pt()); 
    JpsiEta->setVal(theRapidity); 
    JpsiMass->setVal(theMass);
    Jpsict->setVal(theCtau);
    JpsictErr->setVal(theCtauErr);
    // cout << "Type = " << theCat << " pt = " << aCand->pt() << " eta = " << theRapidity << endl;
    // cout << " PPDL = " << theCtau << " Mother = " << aCand->userInt("momPDGId") << " PPDL true = " << 10.*aCand->userFloat("ppdlTrue") << endl;
    JpsiType->setIndex(theCat,kTRUE);
    matchType->setIndex((int)isMatched,kTRUE);
    trigger0->setIndex((int)isTriggerMatched[0],kTRUE);
    trigger1->setIndex((int)isTriggerMatched[1],kTRUE);
    JpsictTrue->setVal(10.*aCand->userFloat("ppdlTrue"));
     
    if (_storeefficiency) {
      // to be done
    }

    TNPeff->setVal(tnpeff);
    TNPefferr->setVal(tnpefferr);
    
    JpsiPtType->setIndex(getJpsiVarType(aCand->pt(),_ptbinranges),kTRUE);
    JpsiEtaType->setIndex(getJpsiVarType(fabs(theRapidity),_etabinranges),kTRUE);
    // cout << "JpsiPtType = " << getJpsiVarType(aCand->pt(),_ptbinranges) << " JpsiEtaType = " << getJpsiVarType(fabs(theRapidity),_etabinranges) << endl;
    
    // Fill RooDataSet
    //  RooArgSet varlist_tmp(*JpsiMass,*Jpsict,*JpsiPt,*JpsiEta,*TNPeff,*TNPefferr,*JpsiType,*matchType);
    RooArgSet varlist_tmp(*JpsiMass,*Jpsict,*JpsiPt,*JpsiEta,*JpsiType,*matchType);   // temporarily remove tag-and-probe weights
    varlist_tmp.add(*JpsictTrue);   varlist_tmp.add(*JpsiPtType);
    varlist_tmp.add(*JpsiEtaType);  varlist_tmp.add(*JpsictErr);
    varlist_tmp.add(*trigger0);     varlist_tmp.add(*trigger1); 

    data->add(varlist_tmp);
    
  }
}
        
void JPsiAnalyzerPAT::makeCuts(int sign) {

  if (collAll.isValid()) {

    for(vector<pat::CompositeCandidate>::const_iterator it=collAll->begin();
	it!=collAll->end();++it) {
      
      const pat::CompositeCandidate* cand = &(*it);	
      // cout << "Now checking candidate of type " << theJpsiCat << " with pt = " << cand->pt() << endl;
      const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
      const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));
 
      bool thisSign = (sign == 0 && muon1->charge() + muon2->charge() == 0) || 
	(sign == 1 && muon1->charge() + muon2->charge() == 2) || 
	(sign == 2 && muon1->charge() + muon2->charge() == -2);

      if (thisSign) {	  
	  
        // global + global?
	if (muon1->isGlobalMuon() && muon2->isGlobalMuon() ) {
	  if (!_applycuts || (selGlobalMuon(muon1) &&
			      selGlobalMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(0);  _thePassedCands[sign].push_back(cand);
            continue;
	  }
	}
	
        // global + tracker? (x2)    
	if (muon1->isGlobalMuon() && muon2->isTrackerMuon() ) {
	  if (!_applycuts || (selGlobalMuon(muon1) &&
			      selTrackerMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(1);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}

        if (muon2->isGlobalMuon() && muon1->isTrackerMuon() ) {
	  if (!_applycuts || (selGlobalMuon(muon2) &&
			      selTrackerMuon(muon1) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(1);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}

        // tracker + tracker?  
        if (muon1->isTrackerMuon() && muon2->isTrackerMuon() ) {
	  if (!_applycuts || (selTrackerMuon(muon1) &&
			      selTrackerMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(2);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}
      }
    }
  }
  
  if (_useCalo && collCalo.isValid()) {

    for(vector<pat::CompositeCandidate>::const_iterator it=collCalo->begin();
	it!=collCalo->end();++it) {
      
      const pat::CompositeCandidate* cand = &(*it);
      
      const pat::Muon* muon1 = dynamic_cast<const pat::Muon*>(cand->daughter("muon1"));
      const pat::Muon* muon2 = dynamic_cast<const pat::Muon*>(cand->daughter("muon2"));
      
      // Remove non-quarkonia region
      // if (cand->mass() < 1.5 && cand->mass() > 15.) continue;

       bool thisSign = (sign == 0 && muon1->charge() + muon2->charge() == 0) || 
	(sign == 1 && muon1->charge() + muon2->charge() == 2) || 
	(sign == 2 && muon1->charge() + muon2->charge() == -2);

      if (thisSign && !(muon1->isTrackerMuon() && muon2->isTrackerMuon()) ) {

	// global + calo? (x2)
	if (muon1->isGlobalMuon() && muon2->isCaloMuon() ) {
	  if (!_applycuts || (selGlobalMuon(muon1) &&
			      selCaloMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(3);  _thePassedCands[sign].push_back(cand);
            continue;
	  }
	}

	if (muon2->isGlobalMuon() && muon1->isCaloMuon() ) {
	  if (!_applycuts || (selGlobalMuon(muon2) &&
			      selCaloMuon(muon1) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(3);  _thePassedCands[sign].push_back(cand);
            continue;
	  }
	}
	
        // tracker + calo? (x2)    
	if (muon1->isTrackerMuon() && muon2->isCaloMuon() ) {
	  if (!_applycuts || (selTrackerMuon(muon1) &&
			      selCaloMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(4);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}

        if (muon2->isTrackerMuon() && muon1->isCaloMuon() ) {
	  if (!_applycuts || (selTrackerMuon(muon2) &&
			      selCaloMuon(muon1) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(4);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}

        // calo + calo? 
        if (muon1->isCaloMuon() && muon2->isCaloMuon() ) {
	  if (!_applycuts || (selCaloMuon(muon1) &&
			      selCaloMuon(muon2) &&
			      cand->userFloat("vProb") > 0.001 )) {
	    _thePassedCats[sign].push_back(5);  _thePassedCands[sign].push_back(cand);
	    continue;
	  }
	}
      }
    }
  }

  return;
}

pair< unsigned int, const pat::CompositeCandidate* > 
JPsiAnalyzerPAT::theBestQQ(int sign) {

  unsigned int theBestCat = 99;
  const pat::CompositeCandidate* theBestCand = new pat::CompositeCandidate();

  for( unsigned int i = 0; i < _thePassedCands[sign].size(); i++) { 
    if (_thePassedCats[sign].at(i) < theBestCat) {
      theBestCat = _thePassedCats[sign].at(i);
      theBestCand = _thePassedCands[sign].at(i);
    }
  }

  pair< unsigned int, const pat::CompositeCandidate* > result = make_pair(theBestCat, theBestCand );
  return result;

}

bool
JPsiAnalyzerPAT::isMuonInAccept(const pat::Muon* aMuon) {
  
  return (fabs(aMuon->eta()) < 2.4 &&
	  ((fabs(aMuon->eta()) < 1.3 && aMuon->pt() > 3.3) ||
	   (fabs(aMuon->eta()) > 1.3 && fabs(aMuon->eta()) < 2.2 && aMuon->p() > 2.9) ||
	   (fabs(aMuon->eta()) > 2.2 && aMuon->pt() > 0.8)));
}

bool
JPsiAnalyzerPAT::selGlobalMuon(const pat::Muon* aMuon) {

  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  TrackRef gTrack = aMuon->globalTrack();
  const reco::HitPattern& q = gTrack->hitPattern();

  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 11 &&
	  gTrack->chi2()/gTrack->ndof() < 20.0 &&
          q.numberOfValidMuonHits() > 0 &&
	  // (p.numberOfValidPixelHits() > 2 || 
	  // (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
          iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  aMuon->muonID("TrackerMuonArbitrated") &&
	  aMuon->muonID("TMLastStationAngTight") &&
          p.pixelLayersWithMeasurement() > 1 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
          fabs(iTrack->dz(RefVtx)) < 15.0 );
}

bool 
JPsiAnalyzerPAT::selTrackerMuon(const pat::Muon* aMuon) {
  
  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (isMuonInAccept(aMuon) &&
	  iTrack->found() > 11 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  aMuon->muonID("TrackerMuonArbitrated") &&
	  aMuon->muonID("TMLastStationAngTight") &&
          // aMuon->muonID("TM2DCompatibilityTight") &&
          // aMuon->muonID("TMLastStationOptimizedLowPtTight") &&
	  // (p.numberOfValidPixelHits() > 2 || 
	  // (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
          p.pixelLayersWithMeasurement() > 1 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
          fabs(iTrack->dz(RefVtx)) < 15.0 );
}

bool 
JPsiAnalyzerPAT::selCaloMuon(const pat::Muon* aMuon) {
  
  TrackRef iTrack = aMuon->innerTrack();
  const reco::HitPattern& p = iTrack->hitPattern();

  return (isMuonInAccept(aMuon) &&
	  aMuon->caloCompatibility() > 0.89 &&
	  iTrack->found() > 11 &&
	  iTrack->chi2()/iTrack->ndof() < 4.0 &&
	  // (p.numberOfValidPixelHits() > 2 || 
	  // (p.numberOfValidPixelHits() > 1 && p.getLayer(p.getHitPattern(0)) == 1)) &&
          p.pixelLayersWithMeasurement() > 1 &&
	  fabs(iTrack->dxy(RefVtx)) < 3.0 &&
          fabs(iTrack->dz(RefVtx)) < 15.0 );
}

int 
JPsiAnalyzerPAT::getJpsiVarType(const double jpsivar, vector<double> vectbin) {

  for(unsigned int i=0;i<vectbin.size()-1;i++) {
    if(jpsivar > vectbin[i] && jpsivar < vectbin[i+1]) return i+1;
  }

  return -999;
}

//define this as a plug-in
DEFINE_FWK_MODULE(JPsiAnalyzerPAT);
