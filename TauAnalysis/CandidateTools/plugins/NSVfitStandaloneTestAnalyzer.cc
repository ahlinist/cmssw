#include "TauAnalysis/CandidateTools/plugins/NSVfitStandaloneTestAnalyzer.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"
#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

typedef edm::View<reco::Candidate> CandidateView;
typedef edm::View<reco::MET> METView;

/// default constructor
NSVfitStandaloneTestAnalyzer::NSVfitStandaloneTestAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    metSign_(0),
    timer_(0),
    numSVfitCalls_(0)
{
  //std::cout << "<NSVfitStandaloneTestAnalyzer::NSVfitStandaloneTestAnalyzer>:" << std::endl;
  //std::cout << " moduleLabel = " << moduleLabel_ << std::endl;

  doGenPlots_ = cfg.getParameter<bool>("doGenPlots");
  //std::cout << " doGenPlots = " << doGenPlots_ << std::endl;
  if ( doGenPlots_ ) {
    srcGenTauPairs_ = cfg.getParameter<edm::InputTag>("srcGenTauPairs");
    srcGenLeg1_     = cfg.getParameter<edm::InputTag>("srcGenLeg1");
    srcGenLeg2_     = cfg.getParameter<edm::InputTag>("srcGenLeg2");
    srcGenMEt_      = cfg.getParameter<edm::InputTag>("srcGenMEt");
  }

  doRecPlots_ = cfg.getParameter<bool>("doRecPlots");
  //std::cout << " doRecPlots = " << doRecPlots_ << std::endl;

  if ( doGenPlots_ || doRecPlots_ ) {
    dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");
  }

  srcRecLeg1_       = cfg.getParameter<edm::InputTag>("srcRecLeg1");
  srcRecLeg2_       = cfg.getParameter<edm::InputTag>("srcRecLeg2");
  srcRecMEt_        = cfg.getParameter<edm::InputTag>("srcRecMEt");
  srcRecMEtCov_     = ( cfg.exists("srcRecMEtCov") ) ? 
    cfg.getParameter<edm::InputTag>("srcRecMEtCov") : edm::InputTag();

  srcWeights_ = ( cfg.exists("srcWeights") ) ?
    cfg.getParameter<vInputTag>("srcWeights") : vInputTag();

  typeLeg1_ = cfg.getParameter<std::string>("typeLeg1");
  typeLeg2_ = cfg.getParameter<std::string>("typeLeg2");
  std::string mode_string = cfg.getParameter<std::string>("mode");
  if      ( mode_string == "fit"  ) mode_ = kFit;
  else if ( mode_string == "int"  ) mode_ = kInt;
  else if ( mode_string == "int2" ) mode_ = kInt2;
  else throw cms::Exception("InvalidParameter")
    << "Invalid Configuration Parameter 'mode' = " << mode_string << " !!\n";

  redoMEtCov_ = cfg.getParameter<bool>("redoMEtCov");
  if ( redoMEtCov_ ) {
    metSign_ = new PFMEtSignInterface(cfg.getParameter<edm::ParameterSet>("paramsMEtCov"));
  }
  
  timer_ = new TStopwatch();
  timer_->Stop();
  
  verbosity_ = cfg.exists("verbosity") ?
    cfg.getParameter<int>("verbosity") : 0;
}

/// default destructor
NSVfitStandaloneTestAnalyzer::~NSVfitStandaloneTestAnalyzer()
{
  delete metSign_;
  delete timer_;
}

/// everything that needs to be done before the event loop
void 
NSVfitStandaloneTestAnalyzer::beginJob()
{
  //std::cout << "<NSVfitStandaloneTestAnalyzer::beginJob>:" << std::endl;
  //std::cout << " dqmDirectory = " << dqmDirectory_ << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogWarning ("NSVfitStandaloneTestAnalyzer::beginJob")
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
    fillHistograms_ = false;
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  if ( doGenPlots_ ) {
    genDiTauPt_     = dqmStore.book1D("genDiTauPt",     "genDiTauPt",      250,           0.,         250.);
    genDiTauEta_    = dqmStore.book1D("genDiTauEta",    "genDiTauEta",     198,         -9.9,         +9.9);
    genDiTauMass_   = dqmStore.book1D("genDiTauMass",   "genDiTauMass",   1000,           0.,        1000.);
    genLeg1Pt_      = dqmStore.book1D("genLeg1Pt",      "genLeg1Pt",       250,           0.,         250.);
    genLeg1Eta_     = dqmStore.book1D("genLeg1Eta",     "genLeg1Eta",      198,         -9.9,         +9.9);
    genLeg2Pt_      = dqmStore.book1D("genLeg2Pt",      "genLeg2Pt",       250,           0.,         250.);
    genLeg2Eta_     = dqmStore.book1D("genLeg2Eta",     "genLeg2Eta",      198,         -9.9,         +9.9);
    genMEtPt_       = dqmStore.book1D("genMEtPt",       "genMEtPt",        500,           0.,         500.);
  }

  if ( doRecPlots_ ) {
    recDiTauPt_     = dqmStore.book1D("recDiTauPt",     "recDiTauPt",      250,           0.,         250.);
    recDiTauEta_    = dqmStore.book1D("recDiTauEta",    "recDiTauEta",     198,         -9.9,         +9.9);
    svFitMass_      = dqmStore.book1D("svFitMass",      "svFitMass",      1000,           0.,        1000.);
    svFitStatus_    = dqmStore.book1D("svFitStatus",    "svFitStatus",      11,         -1.5,         +9.5);
    visMass_        = dqmStore.book1D("visMass",        "visMass",        1000,           0.,        1000.);
    recLeg1Pt_      = dqmStore.book1D("recLeg1Pt",      "recLeg1Pt",       250,           0.,         250.);
    recLeg1Eta_     = dqmStore.book1D("recLeg1Eta",     "recLeg1Eta",      198,         -9.9,         +9.9);
    recLeg2Pt_      = dqmStore.book1D("recLeg2Pt",      "recLeg2Pt",       250,           0.,         250.);
    recLeg2Eta_     = dqmStore.book1D("recLeg2Eta",     "recLeg2Eta",      198,         -9.9,         +9.9);
    recMEtPt_       = dqmStore.book1D("recMEtPt",       "recMEtPt",        500,           0.,         500.);
  }

  if ( doGenPlots_ && doRecPlots_ ) {
    deltaDiTauPt_   = dqmStore.book1D("deltaDiTauPt",   "deltaDiTauPt",    350,        -100.,        +250.);
    deltaDiTauEta_  = dqmStore.book1D("deltaDiTauEta",  "deltaDiTauEta",   198,         -9.9,         +9.9);
    deltaDiTauPhi_  = dqmStore.book1D("deltaDiTauPhi",  "deltaDiTauPhi",   360, -TMath::Pi(), +TMath::Pi());
    deltaDiTauMass_ = dqmStore.book1D("deltaDiTauMass", "deltaDiTauMass", 2000,       -1000.,       +1000.);
  }

  fillHistograms_ = true;
}

namespace
{
  const reco::Candidate* getObject(const edm::Event& evt, edm::InputTag& src, bool& errorFlag)
  {
    const reco::Candidate* object = 0;
    edm::Handle<CandidateView> objects;
    evt.getByLabel(src, objects);
    if ( objects->size() >= 1 ) object = &objects->front(); 
    else errorFlag = true;
    return object;
  }

  reco::Candidate::LorentzVector getObjectP4(const edm::Event& evt, edm::InputTag& src, bool& errorFlag)
  {
    reco::Candidate::LorentzVector objectP4;
    const reco::Candidate* object = getObject(evt, src, errorFlag);
    if ( object ) objectP4 = object->p4();
    return objectP4;
  }
}

/// everything that needs to be done during the event loop
void 
NSVfitStandaloneTestAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<NSVfitStandaloneTestAnalyzer::analyze>:" << std::endl;
  //std::cout << " fillHistograms = " << fillHistograms_ << std::endl;

  reco::Candidate::LorentzVector genDiTauP4;
  reco::Candidate::LorentzVector genLeg1P4;
  reco::Candidate::LorentzVector genLeg2P4;
  reco::Candidate::LorentzVector genMEtP4;
  bool errorFlag_gen = false;
  if ( doGenPlots_ ) {
    genDiTauP4 = getObjectP4(evt, srcGenTauPairs_, errorFlag_gen);
    genLeg1P4 = getObjectP4(evt, srcGenLeg1_, errorFlag_gen);
    genLeg2P4 = getObjectP4(evt, srcGenLeg2_, errorFlag_gen);
    genMEtP4 = getObjectP4(evt, srcGenMEt_, errorFlag_gen);
    if ( errorFlag_gen ) return;
  }

  bool errorFlag_rec = false;
  const reco::Candidate* recLeg1 = getObject(evt, srcRecLeg1_, errorFlag_rec);
  reco::Candidate::LorentzVector recLeg1P4 = getObjectP4(evt, srcRecLeg1_, errorFlag_rec);
  const reco::Candidate* recLeg2 = getObject(evt, srcRecLeg2_, errorFlag_rec);
  reco::Candidate::LorentzVector recLeg2P4 = getObjectP4(evt, srcRecLeg2_, errorFlag_rec);
  reco::Candidate::LorentzVector recMEtP4;
  TMatrixD recMEtCov(2,2);
  edm::Handle<METView> recMETs;
  evt.getByLabel(srcRecMEt_, recMETs);
  if ( recMETs->size() >= 1 ) {
    recMEtP4 = recMETs->front().p4();
    if ( redoMEtCov_ ) {
      metSign_->beginEvent(evt, es);
      // list of reco::Candidates to be subtracted from the list
      // of particle flow candidates, when calculating the MET uncertainty matrix
      std::list<const reco::Candidate*> recTauDecayProducts;
      if ( recLeg1 ) recTauDecayProducts.push_back(recLeg1);
      if ( recLeg2 ) recTauDecayProducts.push_back(recLeg2);
      recMEtCov = (*metSign_)(recTauDecayProducts);
    } else if ( srcRecMEtCov_.label() != "" ) {
      edm::Handle<PFMEtSignCovMatrix> recMEtCov_handle;
      evt.getByLabel(srcRecMEtCov_, recMEtCov_handle);
      recMEtCov = (*recMEtCov_handle);
    } else {
      recMEtCov = recMETs->front().getSignificanceMatrix();
    }
  }
  if ( errorFlag_rec ) return;

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }
  
  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  // setup SVfit input
  std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
  measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(typeLeg1_ == std::string("lep") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, recLeg1P4));
  measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(typeLeg2_ == std::string("lep") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, recLeg2P4));
      
  NSVfitStandaloneAlgorithm algo(measuredTauLeptons, recMEtP4.Vect(), recMEtCov, 0); 
  timer_->Start(false);
  if ( mode_ == kFit ) {
    algo.addLogM(true);
    algo.fit();
  } else if ( mode_ == kInt ) {
    algo.addLogM(false);
    algo.integrate();
  } else if ( mode_ == kInt2 ) {
    algo.addLogM(false);
    algo.integrate2();
  } else assert(0);
  timer_->Stop();
  ++numSVfitCalls_;

  if ( verbosity_ ) {
    std::cout << "<NSVfitStandaloneTestAnalyzer::analyze>:" << std::endl;
    std::cout << " moduleLabel = " << moduleLabel_ << std::endl;
    std::cout << "--> mass (standalone version) = " << algo.mass() 
	      << " + " << algo.massUncert() << " - " << algo.massUncert() << " [" << algo.fitStatus() << "]" << std::endl;
  }

  // fill histograms
  if ( fillHistograms_ ) {
    if ( doGenPlots_ ) {
      genDiTauPt_->Fill(genDiTauP4.pt(), evtWeight);
      genDiTauEta_->Fill(genDiTauP4.eta(), evtWeight);
      genDiTauMass_->Fill(genDiTauP4.mass(), evtWeight);
      genLeg1Pt_->Fill(genLeg1P4.pt(), evtWeight);
      genLeg1Eta_->Fill(genLeg1P4.eta(), evtWeight);
      genLeg2Pt_->Fill(genLeg2P4.pt(), evtWeight);
      genLeg2Eta_->Fill(genLeg2P4.eta(), evtWeight);
      genMEtPt_->Fill(genMEtP4.pt(), evtWeight);
    }

    if ( doRecPlots_ ) {
      if ( mode_ == kInt2 ) {
	recDiTauPt_->Fill(algo.pt(), evtWeight);
	recDiTauEta_->Fill(algo.eta(), evtWeight);
      }
      svFitMass_->Fill(algo.mass(), evtWeight);
      svFitStatus_->Fill(algo.fitStatus(), evtWeight);
      visMass_->Fill((recLeg1P4 + recLeg2P4).mass(), evtWeight);
      recLeg1Pt_->Fill(recLeg1P4.pt(), evtWeight);
      recLeg1Eta_->Fill(recLeg1P4.eta(), evtWeight);
      recLeg2Pt_->Fill(recLeg2P4.pt(), evtWeight);
      recLeg2Eta_->Fill(recLeg2P4.eta(), evtWeight);
      recMEtPt_->Fill(recMEtP4.pt(), evtWeight);
    }
    
    if ( doGenPlots_ && doRecPlots_ ) { 
      if ( mode_ == kInt2 ) {
	deltaDiTauPt_->Fill(algo.pt() - genDiTauP4.pt(), evtWeight);
	deltaDiTauEta_->Fill(algo.eta() - genDiTauP4.eta(), evtWeight);
	deltaDiTauPhi_->Fill(algo.phi() - genDiTauP4.phi(), evtWeight);
	deltaDiTauMass_->Fill(algo.mass() - genDiTauP4.mass(), evtWeight);
      }
    }
  }
}

void 
NSVfitStandaloneTestAnalyzer::endJob()
{
  std::cout << "<NSVfitStandaloneTestAnalyzer::endJob>:" << std::endl;
  std::cout << " moduleLabel = " << moduleLabel_ << std::endl;
  std::cout << " real/CPU time = " 
	    << timer_->RealTime() << "/" 
	    << timer_->CpuTime() << " seconds" << std::endl;
  std::cout << " SVfit calls = " << numSVfitCalls_ << std::endl;
  std::cout << " real/CPU time per call = " 
	    << timer_->RealTime()/(double)numSVfitCalls_ << "/" 
	    << timer_->CpuTime()/(double)numSVfitCalls_ << " seconds" << std::endl;
  std::cout << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(NSVfitStandaloneTestAnalyzer);
