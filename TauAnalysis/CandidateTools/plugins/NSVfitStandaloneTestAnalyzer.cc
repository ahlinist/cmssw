#include "TauAnalysis/CandidateTools/plugins/NSVfitStandaloneTestAnalyzer.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"

/// default constructor
NSVfitStandaloneTestAnalyzer::NSVfitStandaloneTestAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    metSign_(0),
    timer_(0),
    numSVfitCalls_(0)
{
  met_   = cfg.getParameter<edm::InputTag>("met");
  leps1_ = cfg.getParameter<edm::InputTag>("leps1");
  leps2_ = cfg.getParameter<edm::InputTag>("leps2");
  type1_ = cfg.getParameter<std::string>("type1");
  type2_ = cfg.getParameter<std::string>("type2");

  // initialize MET significance calculation 
  metSign_ = new PFMEtSignInterface(cfg.getParameter<edm::ParameterSet>("metSignificance"));

  dqmDirectory_ = cfg.exists("dqmDirectory") ?
    cfg.getParameter<std::string>("dqmDirectory") : "";

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
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogWarning ("NSVfitStandaloneTestAnalyzer::beginJob")
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
    fillHistograms_ = false;
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  leg1Pt_     = dqmStore.book1D("leg1Pt",     "leg1Pt",      500, 0.,  500.);
  leg2Pt_     = dqmStore.book1D("leg2Pt",     "leg2Pt",      500, 0.,  500.);
  metPt_      = dqmStore.book1D("metPt",      "metPt",       500, 0.,  500.);
  svFitMass_  = dqmStore.book1D("svFitMass",  "svFitMass",  1000, 0., 1000.);
  visMass_    = dqmStore.book1D("visMass",    "visMass",    1000, 0., 1000.);

  fillHistograms_ = true;
}

/// everything that needs to be done during the event loop
void 
NSVfitStandaloneTestAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
  // fetch MET
  typedef edm::View<reco::MET> METView;
  edm::Handle<METView> met;
  event.getByLabel(met_, met);
  // fetch selected muon
  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<CandidateView> leps1;
  event.getByLabel(leps1_, leps1);
  // fetch selected electron
  edm::Handle<CandidateView> leps2;
  event.getByLabel(leps2_, leps2);
  // ...
  metSign_->beginEvent(event, eventSetup);
  // list of reco::Candidates to be subtracted from the list
  // of particle flow candidates, when calculating the MET
  // significance 
  std::list<const reco::Candidate*> candList;

  // make sure to prevent ambiguities
  if ( leps1->size() > 1 || leps2->size() > 1 ) {
    std::cout << "Sorry got mixed up: too many e/mu/tau candidates --> skipping !!" << std::endl;
    return;
  }

  // loop leptons
  for ( edm::View<reco::Candidate>::const_iterator lep1 = leps1->begin(); lep1 != leps1->end(); ++lep1 ) {
    for ( edm::View<reco::Candidate>::const_iterator lep2 = leps2->begin(); lep2 != leps2->end(); ++lep2 ) {
      // determine met significance
      candList.push_back(&*lep1);
      candList.push_back(&*lep2);

      TMatrixD covMET(2,2);
      covMET = (*metSign_)(candList);

      // setup measure tau lepton vectors 
      std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(type1_==std::string("lep") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, lep1->p4()));
      measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(type2_==std::string("lep") ? NSVfitStandalone::kLepDecay : NSVfitStandalone::kHadDecay, lep2->p4()));
      
      // construct the class object from the minimal necesarry information
      NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met->front().momentum(), covMET, 0);
      // apply customized configurations if wanted (examples are given below)
      algo.addLogM(false);
      timer_->Start(false);
      algo.integrate();
      timer_->Stop();
      ++numSVfitCalls_;

      // retrieve the results 
      if ( verbosity_ ) {
	std::cout << "<NSVfitStandaloneTestAnalyzer::endJob>:" << std::endl;
	std::cout << " moduleLabel = " << moduleLabel_ << std::endl;
	std::cout << "--> mass (standalone version) = " << algo.getMass() 
		  << " + " << algo.massUncert() << " - " << algo.massUncert() << " [" << algo.fitStatus() << "]" << std::endl;
      }
      if ( fillHistograms_ ) {
	leg1Pt_->Fill(lep1->pt());
	leg2Pt_->Fill(lep2->pt());
	metPt_->Fill(met->front().pt()); 
	svFitMass_->Fill(algo.getMass() );
	visMass_->Fill((lep1->p4() + lep2->p4()).mass()); 
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
