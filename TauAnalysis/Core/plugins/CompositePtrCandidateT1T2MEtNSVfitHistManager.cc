#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtNSVfitHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisFwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TMath.h>
#include <TH1.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::CompositePtrCandidateT1T2MEtNSVfitHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::CompositePtrCandidateT1T2MEtNSVfitHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_.label() << std::endl;

  genParticleSrc_ = cfg.getParameter<edm::InputTag>("genParticleSource");
  //std::cout << " genParticleSrc = " << genParticleSrc_.label() << std::endl;

  nSVfitEventHypothesisSrc_ = cfg.getParameter<edm::InputTag>("nSVfitEventHypothesisSource");
  //std::cout << " nSVfitEventHypothesisSrc_ = " << nSVfitEventHypothesisSrc.label() << std::endl;

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");

  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  pdgIdsElectron_.push_back(+11);
  pdgIdsElectron_.push_back(-11);
  pdgIdsMuon_.push_back(+13);
  pdgIdsMuon_.push_back(-13);
  pdgIdsPhoton_.push_back(22);
  for ( int iQuarkType = 1; iQuarkType <= 6; ++iQuarkType ) {
    pdgIdsJet_.push_back(+iQuarkType);
    pdgIdsJet_.push_back(-iQuarkType);
  }
  pdgIdsJet_.push_back(21);

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtNSVfitHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T1>*>::iterator it = diTauLeg1WeightExtractors_.begin();
	it != diTauLeg1WeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  for ( typename std::vector<FakeRateJetWeightExtractor<T2>*>::iterator it = diTauLeg2WeightExtractors_.begin();
	it != diTauLeg2WeightExtractors_.end(); ++it ) {
    delete (*it);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::bookHistogramsImp>:" << std::endl;

  hMass_   = book1D("Mass",   "Mass",    50, 0., 250.);
  hMassL_  = book1D("MassL",  "MassL",  100, 0., 500.);
  hMassXL_ = book1D("MassXL", "MassXL", 150, 0., 750.);

  hMassGenLeg2Electron_ = book1D("MassGenLeg2Electron", "MassGenLeg2Electron", 50, 0., 250.);
  hMassGenLeg2Muon_     = book1D("MassGenLeg2Muon",     "MassGenLeg2Muon",     50, 0., 250.);
  hMassGenLeg2Photon_   = book1D("MassGenLeg2Photon",   "MassGenLeg2Photon",   50, 0., 250.);
  hMassGenLeg2Jet_      = book1D("MassGenLeg2Jet",      "MassGenLeg2Jet",      50, 0., 250.);

  hMassMedian_ = book1D("Mass_median", "Mass_median",  50,  0., 250.);
  hMassMean_   = book1D("Mass_mean",   "Mass_mean",    50,  0., 250.);

  hMassPull_   = book1D("MassPull",    "MassPull",    100, -5.,  +5.);

  hMassSum_           = 0; // book histogram when filling first value
  hMassSumNormalized_ = 0;

  bookWeightHistograms(*dqmStore_, "DiTauCandidateWeight", "Composite Weight",
		       hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
		       hDiTauCandidateWeightLinear_);
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtNSVfitHistManager::fillHistogramsImp>:" << std::endl;

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  getCollection(evt, diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_.label() << ":"
  //	      << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  edm::Handle<reco::GenParticleCollection> genParticles;
  if ( genParticleSrc_.label() != "" ) evt.getByLabel(genParticleSrc_, genParticles);

  edm::Handle<NSVfitEventHypothesisCollection> nSVfitEventHypotheses;
  evt.getByLabel(nSVfitEventHypothesisSrc_, nSVfitEventHypotheses);

  double diTauCandidateWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin();
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*diTauCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin();
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {

    //bool isGenMatched = matchesGenCandidatePair(*diTauCandidate);
    //std::cout << " Pt = " << diTauCandidate->pt() << ", phi = " << diTauCandidate->phi() << ","
    //          << " visMass = " << diTauCandidate->p4Vis().mass() << std::endl;
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;

    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*diTauCandidate);
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    const NSVfitEventHypothesis* matchedNSVfitEventHypothesis = 0;
    for ( NSVfitEventHypothesisCollection::const_iterator nSVfitEventHypothesis = nSVfitEventHypotheses->begin();
	  nSVfitEventHypothesis != nSVfitEventHypotheses->end(); ++nSVfitEventHypothesis ) {
      const std::vector<NSVfitResonanceHypothesis*>& resonances = nSVfitEventHypothesis->resonances();     
      assert(resonances.size() == 1);
      const std::vector<NSVfitSingleParticleHypothesisBase*>& daughters = resonances[0]->daughters();
      assert(daughters.size() == 2);
      if ( deltaR(diTauCandidate->leg1()->p4(), daughters[0]->p4()) < epsilon &&
	   deltaR(diTauCandidate->leg2()->p4(), daughters[1]->p4()) < epsilon ) matchedNSVfitEventHypothesis = &(*nSVfitEventHypothesis);
    }

    if ( !matchedNSVfitEventHypothesis ) {
      edm::LogWarning("CompositePtrCandidateT1T2MEtNSVfitHistManager::fillHistogramsImp") 
        << " Failed to match NSVfitEventHypothesis object to diTauCandidate --> skipping !!";
      continue;
    }

    const NSVfitResonanceHypothesis* matchedNSVfitResonanceHypothesis = matchedNSVfitEventHypothesis->resonances()[0];

    double recMass = matchedNSVfitResonanceHypothesis->mass();
    double genMass = diTauCandidate->p4gen().mass();

    hMass_->Fill(recMass, weight);
    hMassL_->Fill(recMass, weight);
    hMassXL_->Fill(recMass, weight);
    
    if ( genParticles.isValid() ) {
      fillHistogramGenMatch(hMassGenLeg2Electron_, recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsElectron_, weight);
      fillHistogramGenMatch(hMassGenLeg2Muon_,     recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsMuon_,     weight);
      fillHistogramGenMatch(hMassGenLeg2Photon_,   recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsPhoton_,   weight);
      fillHistogramGenMatch(hMassGenLeg2Jet_,      recMass, diTauCandidate->leg2()->p4(), *genParticles, pdgIdsJet_,      weight);
    }

    hMassMedian_->Fill(matchedNSVfitResonanceHypothesis->mass_median(), weight);
    hMassMean_->Fill(matchedNSVfitResonanceHypothesis->mass_mean(), weight);
    
    double sigma = ( recMass > genMass ) ? 
      matchedNSVfitResonanceHypothesis->massErrUp() : matchedNSVfitResonanceHypothesis->massErrDown();
    if ( sigma != 0. ) hMassPull_->Fill((recMass - genMass)/sigma, weight);
    
    if ( matchedNSVfitEventHypothesis->histMassResults() ) {
      const TH1* histMassResults = matchedNSVfitEventHypothesis->histMassResults();
      double integral = histMassResults->Integral();
      if ( integral > 0. ) fillHistogramMassResult(hMassSum_, "MassSum", histMassResults, weight/integral);
      fillHistogramMassResult(hMassSumNormalized_, "MassSumNormalized", histMassResults, weight);
    }

    fillWeightHistograms(hDiTauCandidateWeightPosLog_, hDiTauCandidateWeightNegLog_, hDiTauCandidateWeightZero_,
			 hDiTauCandidateWeightLinear_, diTauCandidateWeight);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtNSVfitHistManager<T1,T2>::fillHistogramMassResult(
       MonitorElement*& me, const std::string& meName, const TH1* histMassResults, double weight)
{
  if ( !me ) {
//--- check that no other MonitorElement with name given as function argument exists
    if ( checkExistence(*dqmStore_, dqmDirectory_store_, meName) ) return;

//--- book MonitorElement
    dqmStore_->setCurrentFolder(dqmDirectory_store_);
    
    std::string meName_full = dqmDirectoryName(dqmDirectory_store_).append(meName);
    TH1* histogram = (TH1*)histMassResults->Clone(meName.data());
    histogram->SetTitle(meName.data());
    histogram->Reset();
    dqmRegisterHistogram(*dqmStore_, histogram, meName_full);
  }

  if ( weight > 0. ) me->getTH1()->Add(histMassResults, weight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Tau> PATElecTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Muon, pat::Tau> PATMuTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Tau, pat::Tau> PATDiTauPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Muon> PATElecMuPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Electron, pat::Electron> PATDiElecPairNSVfitHistManager;
typedef CompositePtrCandidateT1T2MEtNSVfitHistManager<pat::Muon, pat::Muon> PATDiMuPairNSVfitHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairNSVfitHistManager, "PATElecTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairNSVfitHistManager, "PATElecTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairNSVfitHistManager, "PATMuTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairNSVfitHistManager, "PATMuTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairNSVfitHistManager, "PATDiTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairNSVfitHistManager, "PATDiTauPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairNSVfitHistManager, "PATElecMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairNSVfitHistManager, "PATElecMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiElecPairNSVfitHistManager, "PATDiElecPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiElecPairNSVfitHistManager, "PATDiElecPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiMuPairNSVfitHistManager, "PATDiMuPairNSVfitHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiMuPairNSVfitHistManager, "PATDiMuPairNSVfitHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<PATElecTauPairNSVfitHistManager> PATElecTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATMuTauPairNSVfitHistManager> PATMuTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiTauPairNSVfitHistManager> PATDiTauPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATElecMuPairNSVfitHistManager> PATElecMuPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiElecPairNSVfitHistManager> PATDiElecPairNSVfitAnalyzer;
typedef HistManagerAdapter<PATDiMuPairNSVfitHistManager> PATDiMuPairNSVfitAnalyzer;

DEFINE_FWK_MODULE(PATElecTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiElecPairNSVfitAnalyzer);
DEFINE_FWK_MODULE(PATDiMuPairNSVfitAnalyzer);

