#include "TauAnalysis/Core/plugins/PFCandidateHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

//
//-----------------------------------------------------------------------------------------------------------------------
//

PFCandidateHistManager::PFCandidateHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "pfCandidates");
}

PFCandidateHistManager::~PFCandidateHistManager()
{
//--- nothing to be done yet...
}

void PFCandidateHistManager::bookHistogramsImp()
{
  hNumPFCandidates_ = book1D("NumPFCandidates", "NumPFCandidates", 200, -0.5, 999.5);

  bookPFCandidateHistograms(hPFCandidatePt_, hPFCandidateEta_, hPFCandidatePhi_, "PFCandidate");
 
  hPFCandidateSumEt_ = book1D("PFCandidateSumEt", "PFCandidateSumEt", 250, -0.01, 2500.);
}

void PFCandidateHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  hNumPFCandidates_->Fill(pfCandidates->size(), evtWeight);

  double pfCandidateWeightSum = pfCandidates->size();

  double sumEt = 0.;	

  for ( std::vector<reco::PFCandidate>::const_iterator pfCandidate = pfCandidates->begin(); 
	pfCandidate != pfCandidates->end(); ++pfCandidate ) {

    double pfCandidateWeight = 1.;
    double weight = getWeight(evtWeight, pfCandidateWeight, pfCandidateWeightSum);

    fillPFCandidateHistograms(*pfCandidate, hPFCandidatePt_, hPFCandidateEta_, hPFCandidatePhi_, weight);

    sumEt += pfCandidate->et();
  }

  hPFCandidateSumEt_->Fill(sumEt, evtWeight);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void PFCandidateHistManager::bookPFCandidateHistograms(
       MonitorElement*& hPFCandidatePt, MonitorElement*& hPFCandidateEta, MonitorElement*& hPFCandidatePhi, const char* histoSetName)
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

void PFCandidateHistManager::fillPFCandidateHistograms(
       const reco::PFCandidate& pfCandidate, 
       MonitorElement* hPFCandidatePt, MonitorElement* hPFCandidateEta, MonitorElement* hPFCandidatePhi, 
       double evtWeight)
{
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
