#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TauAnalysis/CandidateTools/plugins/NSVfitStandaloneTestAnalyzer.h"
#include "TauAnalysis/CandidateTools/interface/PFMEtSignInterface.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

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

  timer_ = new TStopwatch();
  timer_->Stop();
}

/// default destructor
NSVfitStandaloneTestAnalyzer::~NSVfitStandaloneTestAnalyzer()
{
  delete metSign_;
  delete timer_;
}

/// everything that needs to be done during the event loop
void 
NSVfitStandaloneTestAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
  // fetch MET
  edm::Handle<std::vector<pat::MET> > met;
  event.getByLabel(met_, met);
  // fetch selected muon
  edm::Handle<edm::View<reco::Candidate> > leps1;
  event.getByLabel(leps1_, leps1);
  // fetch selected electron
  edm::Handle<edm::View<reco::Candidate> > leps2;
  event.getByLabel(leps2_, leps2);
  // ...
  metSign_->beginEvent(event, eventSetup);
  // list of reco::Candidates to be subtracted from the list
  // of particle flow candidates, when calculating the MET
  // significance 
  std::list<const reco::Candidate*> candList;

  // make sure to prevent ambiguities
  if ( leps1->size() > 1 || leps2->size() > 1 ) {
    std::cout << "sorry got mixed up, too many e and mu candidates" << std::endl;
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
      std::cout << "<NSVfitStandaloneTestAnalyzer::endJob>:" << std::endl;
      std::cout << " moduleLabel = " << moduleLabel_ << std::endl;
      std::cout << "--> mass (standalone version) = " << algo.getMass() 
		<< " + " << algo.massUncert() << " - " << algo.massUncert() << " [" << algo.fitStatus() << "]" << std::endl;
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
