#include "TauAnalysis/Core/plugins/PFCandidateHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

//
//-----------------------------------------------------------------------------------------------------------------------
//

PFCandidateHistManager::PFCandidateHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<PFCandidateHistManager::PFCandidateHistManager>:" << std::endl;

  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");
  //std::cout << " pfCandidateSrc = " << pfCandidateSrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
  
  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "pfCandidates");
}

PFCandidateHistManager::~PFCandidateHistManager()
{
//--- nothing to be done yet...
}

void PFCandidateHistManager::bookHistograms()
{
  //std::cout << "<PFCandidateHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  bookPFCandidateHistograms(dqmStore, hPFCandidatePt_, hPFCandidateEta_, hPFCandidatePhi_, "PFCandidate");
  hPFCandidatePtVsEta_ = dqmStore.book2D("PFCandidatePtVsEta", "PFCandidatePtVsEta", 24, -3., +3., 30, 0., 150.);
}

void PFCandidateHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<PFCandidateHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  edm::Handle<std::vector<reco::PFCandidate> > pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  double pfCandidateWeightSum = pfCandidates->size();

  for ( std::vector<reco::PFCandidate>::const_iterator pfCandidate = pfCandidates->begin(); 
	pfCandidate != pfCandidates->end(); ++pfCandidate ) {

    double pfCandidateWeight = 1.;
    double weight = getWeight(evtWeight, pfCandidateWeight, pfCandidateWeightSum);
  
    fillPFCandidateHistograms(*pfCandidate, hPFCandidatePt_, hPFCandidateEta_, hPFCandidatePhi_, weight);
    hPFCandidatePtVsEta_->Fill(pfCandidate->eta(), pfCandidate->pt(), weight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void PFCandidateHistManager::bookPFCandidateHistograms(DQMStore& dqmStore, MonitorElement*& hPFCandidatePt, MonitorElement*& hPFCandidateEta, MonitorElement*& hPFCandidatePhi, const char* histoSetName)
{
  std::string hPFCandidatePtName = std::string(histoSetName).append("Pt");
  hPFCandidatePt = dqmStore.book1D(hPFCandidatePtName, hPFCandidatePtName, 75, 0., 150.);
  
  std::string hPFCandidateEtaName = std::string(histoSetName).append("Eta");
  hPFCandidateEta = dqmStore.book1D(hPFCandidateEtaName, hPFCandidateEtaName, 60, -3., +3.);
  
  std::string hPFCandidatePhiName = std::string(histoSetName).append("Phi");
  hPFCandidatePhi = dqmStore.book1D(hPFCandidatePhiName, hPFCandidatePhiName, 36, -TMath::Pi(), +TMath::Pi());
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void PFCandidateHistManager::fillPFCandidateHistograms(const reco::PFCandidate& pfCandidate, 
						       MonitorElement* hPFCandidatePt, MonitorElement* hPFCandidateEta, MonitorElement* hPFCandidatePhi, 
						       double evtWeight)
{
  //std::cout << "<PFCandidateHistManager::fillPFCandidateHistograms>:" << std::endl;

  hPFCandidatePt->Fill(pfCandidate.pt(), evtWeight);
  hPFCandidateEta->Fill(pfCandidate.eta(), evtWeight);
  hPFCandidatePhi->Fill(pfCandidate.phi(), evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PFCandidateHistManager, "PFCandidateHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PFCandidateHistManager, "PFCandidateHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PFCandidateHistManager> PFCandidateAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(PFCandidateAnalyzer);
