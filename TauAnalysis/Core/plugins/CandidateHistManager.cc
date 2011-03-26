#include "TauAnalysis/Core/plugins/CandidateHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

CandidateHistManager::CandidateHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  candidateSrc_ = cfg.getParameter<edm::InputTag>("candidateSource");

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "candidates");
}

CandidateHistManager::~CandidateHistManager()
{
//--- nothing to be done yet...
}

void CandidateHistManager::bookHistogramsImp()
{
  //std::cout << "<CandidateHistManager::bookHistogramsImp>:" << std::endl;
  
  hCandidatePt_     = book1D("CandidatePt",     "CandidatePt",      75,           0.,         150.);
  hCandidateEta_    = book1D("CandidateEta",    "CandidateEta",     60,          -3.,          +3.);
  hCandidatePhi_    = book1D("CandidatePhi",    "CandidatePhi",     36, -TMath::Pi(), +TMath::Pi());
  
  hCandidateMass_   = book1D("CandidateMass",   "CandidateMass",   125,           0.,         250.);

  hCandidateCharge_ = book1D("CandidateCharge", "CandidateCharge",   5,         -2.5,         +2.5);
}

void CandidateHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CandidateHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<reco::CandidateView> candidates;
  evt.getByLabel(candidateSrc_, candidates);

  double candidateWeightSum = candidates->size();

  for ( reco::CandidateView::const_iterator candidate = candidates->begin(); 
	candidate != candidates->end(); ++candidate ) {

    double candidateWeight = 1.;
    double weight = getWeight(evtWeight, candidateWeight, candidateWeightSum);
  
    hCandidatePt_->Fill(candidate->pt(), weight);
    hCandidateEta_->Fill(candidate->eta(), weight);
    hCandidatePhi_->Fill(candidate->phi(), weight);
    
    hCandidateMass_->Fill(candidate->mass(), weight);
    
    hCandidateCharge_->Fill(candidate->charge(), weight);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, CandidateHistManager, "CandidateHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, CandidateHistManager, "CandidateHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<CandidateHistManager> CandidateAnalyzer;

DEFINE_FWK_MODULE(CandidateAnalyzer);
