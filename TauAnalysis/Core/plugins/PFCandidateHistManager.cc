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
  : HistManagerBase(cfg)
{
  //std::cout << "<PFCandidateHistManager::PFCandidateHistManager>:" << std::endl;

  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");
  //std::cout << " pfCandidateSrc = " << pfCandidateSrc_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "pfCandidates");
}

PFCandidateHistManager::~PFCandidateHistManager()
{
//--- nothing to be done yet...
}

void PFCandidateHistManager::bookHistogramsImp()
{
  //std::cout << "<PFCandidateHistManager::bookHistogramsImp>:" << std::endl;
  
  bookPFCandidateHistograms(hPFCandidatePt_, hPFCandidateEta_, hPFCandidatePhi_, "PFCandidate");
  hPFCandidatePtVsEta_ = book2D("PFCandidatePtVsEta", "PFCandidatePtVsEta", 24, -3., +3., 30, 0., 150.);
}

void PFCandidateHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<PFCandidateHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
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

void PFCandidateHistManager::bookPFCandidateHistograms(MonitorElement*& hPFCandidatePt, MonitorElement*& hPFCandidateEta, MonitorElement*& hPFCandidatePhi, const char* histoSetName)
{
  std::string hPFCandidatePtName = std::string(histoSetName).append("Pt");
  hPFCandidatePt = book1D(hPFCandidatePtName, hPFCandidatePtName, 75, 0., 150.);
  
  std::string hPFCandidateEtaName = std::string(histoSetName).append("Eta");
  hPFCandidateEta = book1D(hPFCandidateEtaName, hPFCandidateEtaName, 60, -3., +3.);
  
  std::string hPFCandidatePhiName = std::string(histoSetName).append("Phi");
  hPFCandidatePhi = book1D(hPFCandidatePhiName, hPFCandidatePhiName, 36, -TMath::Pi(), +TMath::Pi());
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

DEFINE_FWK_MODULE(PFCandidateAnalyzer);
