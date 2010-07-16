#include "TauAnalysis/Core/plugins/CompositePtrCandidateT1T2MEtEventActivityHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
bool matchesGenCandidatePair(const CompositePtrCandidateT1T2MEt<T1,T2>& compositePtrCandidate)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

std::string getNameParEncodedValue(const std::string& name, double parValue1, const char* parName2 = "", double* parValue2 = 0)
{
  std::ostringstream fullName;
  fullName.setf(std::ios::fixed);
  fullName.precision(1);
  fullName << name << parValue1;
  if ( parValue2 ) fullName << parName2 << parValue2;

  int errorFlag = 0;
  std::string fullName_mod = replace_string(fullName.str(), ".", "_", 0, 1, errorFlag);

  return fullName_mod;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::histogramCollectionType::histogramCollectionType(const edm::ParameterSet& cfg)
{
  dRveto_ = cfg.getParameter<double>("dRveto");

  etaGap_ = cfg.getParameter<double>("etaGap");

  chargedParticlePtThresholds_ = cfg.getParameter<vdouble>("chargedParticlePtThresholds");
  particlePtThresholds_ = cfg.getParameter<vdouble>("particlePtThresholds");
  jetPtThresholds_ = cfg.getParameter<vdouble>("jetPtThresholds");
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::histogramCollectionType::bookHistograms(
       DQMStore& dqmStore, const std::string& dqmDirectory)
{
  //std::cout << "<histogramCollectionType::bookHistograms>:" << std::endl;
  //std::cout << " etaGap = " << etaGap_ << std::endl;

  for ( vdouble::const_iterator chargedParticlePtThreshold = chargedParticlePtThresholds_.begin();
	chargedParticlePtThreshold != chargedParticlePtThresholds_.end(); ++chargedParticlePtThreshold ) {
    std::string histogramName = getNameParEncodedValue("NumChargedParticles", *chargedParticlePtThreshold, "etaGap", &etaGap_);
    MonitorElement* histogram = HistManagerBase::book1D(dqmStore, dqmDirectory, histogramName, histogramName, 100, -0.5, 99.5);
    hNumChargedParticles_.push_back(histogram);
  }

  for ( vdouble::const_iterator jetPtThreshold = jetPtThresholds_.begin();
	jetPtThreshold != jetPtThresholds_.end(); ++jetPtThreshold ) {
    std::string histogramName = getNameParEncodedValue("NumJets", *jetPtThreshold, "etaGap", &etaGap_);
    MonitorElement* histogram = HistManagerBase::book1D(dqmStore, dqmDirectory, histogramName, histogramName, 20, -0.5, 19.5);
    hNumJets_.push_back(histogram);
  }
	
  for ( vdouble::const_iterator chargedParticlePtThreshold = chargedParticlePtThresholds_.begin();
	chargedParticlePtThreshold != chargedParticlePtThresholds_.end(); ++chargedParticlePtThreshold ) {
    std::string histogramName = getNameParEncodedValue("ChargedParticleEtSum", *chargedParticlePtThreshold, "etaGap", &etaGap_);
    MonitorElement* histogram = HistManagerBase::book1D(dqmStore, dqmDirectory, histogramName, histogramName, 100, -0.5, 99.5);
    hChargedParticleEtSum_.push_back(histogram);
  }
  
  for ( vdouble::const_iterator particlePtThreshold = particlePtThresholds_.begin();
	particlePtThreshold != particlePtThresholds_.end(); ++particlePtThreshold ) {
    std::string histogramName = getNameParEncodedValue("ParticleEtSum", *particlePtThreshold, "etaGap", &etaGap_);
    MonitorElement* histogram = HistManagerBase::book1D(dqmStore, dqmDirectory, histogramName, histogramName, 100, -0.5, 99.5);
    hParticleEtSum_.push_back(histogram);
  }
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::histogramCollectionType::fillHistograms(
       const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate, 
       const reco::PFCandidateCollection& pfCandidates, const pat::JetCollection& patJets, double weight)
{
  //std::cout << "<histogramCollectionType::fillHistograms>:" << std::endl;
  //std::cout << " etaGap = " << etaGap_ << std::endl;

  double etaMin = TMath::Min(diTauCandidate.leg1()->eta(), diTauCandidate.leg2()->eta()) - etaGap_;
  double etaMax = TMath::Max(diTauCandidate.leg1()->eta(), diTauCandidate.leg2()->eta()) + etaGap_;
  
  unsigned numChargedParticlePtThresholds = chargedParticlePtThresholds_.size();
  std::vector<unsigned> numChargedParticles(numChargedParticlePtThresholds);
  std::vector<double> chargedParticleSumEt(numChargedParticlePtThresholds);
  unsigned numParticlePtThresholds = particlePtThresholds_.size();
  std::vector<double> particleSumEt(numParticlePtThresholds);
  
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {    
    if ( pfCandidate->eta() > etaMin && pfCandidate->eta() < etaMax &&
	 deltaR(pfCandidate->p4(), diTauCandidate.leg1()->p4()) > dRveto_ &&
	 deltaR(pfCandidate->p4(), diTauCandidate.leg2()->p4()) > dRveto_ ) {
      if ( pfCandidate->charge() != 0. ) {
	for ( unsigned iChargedParticlePtThreshold = 0; 
	      iChargedParticlePtThreshold < numChargedParticlePtThresholds; ++iChargedParticlePtThreshold ) {
	  if ( pfCandidate->pt() > chargedParticlePtThresholds_[iChargedParticlePtThreshold] ) {
	    ++numChargedParticles[iChargedParticlePtThreshold];
	    chargedParticleSumEt[iChargedParticlePtThreshold] += pfCandidate->et();
	  }
	}
      }

      for ( unsigned iParticlePtThreshold = 0; 
	    iParticlePtThreshold < numParticlePtThresholds; ++iParticlePtThreshold ) {
	if ( pfCandidate->pt() > particlePtThresholds_[iParticlePtThreshold] ) {
	  particleSumEt[iParticlePtThreshold] += pfCandidate->et();
	}
      }
    }
  }

  for ( unsigned iChargedParticlePtThreshold = 0; 
	iChargedParticlePtThreshold < numChargedParticlePtThresholds; ++iChargedParticlePtThreshold ) {
    hNumChargedParticles_[iChargedParticlePtThreshold]->Fill(numChargedParticles[iChargedParticlePtThreshold], weight);
    hChargedParticleEtSum_[iChargedParticlePtThreshold]->Fill(chargedParticleSumEt[iChargedParticlePtThreshold], weight);
  }

  for ( unsigned iParticlePtThreshold = 0; 
	iParticlePtThreshold < numParticlePtThresholds; ++iParticlePtThreshold ) {
    hParticleEtSum_[iParticlePtThreshold]->Fill(particleSumEt[iParticlePtThreshold], weight);
  }

  unsigned numJetPtThresholds = jetPtThresholds_.size();
  std::vector<unsigned> numJets(numJetPtThresholds);
  
  for ( pat::JetCollection::const_iterator patJet = patJets.begin();
	patJet != patJets.end(); ++patJet ) {
    if ( patJet->eta() > etaMin && patJet->eta() < etaMax &&
	 deltaR(patJet->p4(), diTauCandidate.leg1()->p4()) > dRveto_ &&
	 deltaR(patJet->p4(), diTauCandidate.leg2()->p4()) > dRveto_ ) {
      for ( unsigned iJetPtThreshold = 0; 
	    iJetPtThreshold < numJetPtThresholds; ++iJetPtThreshold ) {
	if ( patJet->pt() > jetPtThresholds_[iJetPtThreshold] ) ++numJets[iJetPtThreshold];
      }
    }
  }

  for ( unsigned iJetPtThreshold = 0; 
	iJetPtThreshold < numJetPtThresholds; ++iJetPtThreshold ) {
    hNumJets_[iJetPtThreshold]->Fill(numJets[iJetPtThreshold], weight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::CompositePtrCandidateT1T2MEtEventActivityHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<CompositePtrCandidateT1T2MEtEventActivityHistManager::CompositePtrCandidateT1T2MEtEventActivityHistManager>:" << std::endl;

  diTauCandidateSrc_ = cfg.getParameter<edm::InputTag>("diTauCandidateSource");
  //std::cout << " diTauCandidateSrc = " << diTauCandidateSrc_ << std::endl;

  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");
  patJetSrc_ = cfg.getParameter<edm::InputTag>("patJetSource");

  etaGaps_ = cfg.getParameter<vdouble>("etaGaps");
  for ( vdouble::const_iterator etaGap = etaGaps_.begin();
	etaGap != etaGaps_.end(); ++etaGap ) {
    edm::ParameterSet cfg_etaGap(cfg);
    cfg_etaGap.addParameter<double>("etaGap", *etaGap);
    
    histograms_.push_back(histogramCollectionType(cfg_etaGap));
  }

  diTauLeg1WeightExtractors_ = getTauJetWeightExtractors<T1>(cfg, "diTauLeg1WeightSource");
  diTauLeg2WeightExtractors_ = getTauJetWeightExtractors<T2>(cfg, "diTauLeg2WeightSource");
  
  requireGenMatch_ = cfg.getParameter<bool>("requireGenMatch");
  //std::cout << " requireGenMatch = " << requireGenMatch_ << std::endl;

  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "diTauCandidates");
}

template<typename T1, typename T2>
CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::~CompositePtrCandidateT1T2MEtEventActivityHistManager()
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
void CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::bookHistogramsImp()
{
  //std::cout << "<CompositePtrCandidateT1T2MEtEventActivityHistManager::bookHistogramsImp>:" << std::endl;

  unsigned numEtaGaps = etaGaps_.size();
  for ( unsigned iEtaGap = 0; iEtaGap < numEtaGaps; ++iEtaGap ) {
    std::string subDirName = getNameParEncodedValue("etaGap", etaGaps_[iEtaGap]);

    std::string dqmDirectoryName_full = dqmDirectoryName(dqmDirectory_store_).append(subDirName);

    histograms_[iEtaGap].bookHistograms(*dqmStore_, dqmDirectoryName_full);
  }
}

template<typename T1, typename T2>
double CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::getDiTauCandidateWeight(const CompositePtrCandidateT1T2MEt<T1,T2>& diTauCandidate)
{
  double diTauLeg1Weight = getTauJetWeight<T1>(*diTauCandidate.leg1(), diTauLeg1WeightExtractors_);
  double diTauLeg2Weight = getTauJetWeight<T2>(*diTauCandidate.leg2(), diTauLeg2WeightExtractors_);
  return (diTauLeg1Weight*diTauLeg2Weight);
}

template<typename T1, typename T2>
void CompositePtrCandidateT1T2MEtEventActivityHistManager<T1,T2>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<CompositePtrCandidateT1T2MEtEventActivityHistManager::fillHistogramsImp>:" << std::endl; 

  typedef std::vector<CompositePtrCandidateT1T2MEt<T1,T2> > CompositePtrCandidateCollection;
  edm::Handle<CompositePtrCandidateCollection> diTauCandidates;
  getCollection(evt, diTauCandidateSrc_, diTauCandidates);

  //std::cout << " diTauCandidates.size = " << diTauCandidates->size() << std::endl;

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  getCollection(evt, pfCandidateSrc_, pfCandidates);

  edm::Handle<pat::JetCollection> patJets;
  getCollection(evt, patJetSrc_, patJets);

  double diTauCandidateWeightSum = 0.;
  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    diTauCandidateWeightSum += getDiTauCandidateWeight(*diTauCandidate);
  }

  for ( typename CompositePtrCandidateCollection::const_iterator diTauCandidate = diTauCandidates->begin(); 
	diTauCandidate != diTauCandidates->end(); ++diTauCandidate ) {

    //bool isGenMatched = matchesGenCandidatePair(*diTauCandidate);
    //std::cout << " Pt = " << diTauCandidate->pt() << ", phi = " << diTauCandidate->phi() << ", visMass = " << diTauCandidate->p4Vis().mass() << std::endl;
    //std::cout << " isGenMatched = " << isGenMatched << std::endl;
    
    if ( requireGenMatch_ && !matchesGenCandidatePair(*diTauCandidate) ) continue;

    double diTauCandidateWeight = getDiTauCandidateWeight(*diTauCandidate);
    double weight = getWeight(evtWeight, diTauCandidateWeight, diTauCandidateWeightSum);

    for ( typename std::vector<histogramCollectionType>::iterator histogram = histograms_.begin();
	  histogram != histograms_.end(); ++histogram ) {
      histogram->fillHistograms(*diTauCandidate, *pfCandidates, *patJets, weight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h" 
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef CompositePtrCandidateT1T2MEtEventActivityHistManager<reco::Candidate, reco::Candidate> DiCandidatePairEventActivityHistManager;
typedef CompositePtrCandidateT1T2MEtEventActivityHistManager<pat::Electron, pat::Tau> PATElecTauPairEventActivityHistManager;
typedef CompositePtrCandidateT1T2MEtEventActivityHistManager<pat::Muon, pat::Tau> PATMuTauPairEventActivityHistManager;
typedef CompositePtrCandidateT1T2MEtEventActivityHistManager<pat::Tau, pat::Tau> PATDiTauPairEventActivityHistManager;
typedef CompositePtrCandidateT1T2MEtEventActivityHistManager<pat::Electron, pat::Muon> PATElecMuPairEventActivityHistManager;

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DiCandidatePairEventActivityHistManager, "DiCandidatePairEventActivityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, DiCandidatePairEventActivityHistManager, "DiCandidatePairEventActivityHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecTauPairEventActivityHistManager, "PATElecTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecTauPairEventActivityHistManager, "PATElecTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATMuTauPairEventActivityHistManager, "PATMuTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATMuTauPairEventActivityHistManager, "PATMuTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATDiTauPairEventActivityHistManager, "PATDiTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATDiTauPairEventActivityHistManager, "PATDiTauPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, PATElecMuPairEventActivityHistManager, "PATElecMuPairEventActivityHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, PATElecMuPairEventActivityHistManager, "PATElecMuPairEventActivityHistManager");
  
#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<DiCandidatePairEventActivityHistManager> DiCandidatePairEventActivityAnalyzer;
typedef HistManagerAdapter<PATElecTauPairEventActivityHistManager> PATElecTauPairEventActivityAnalyzer;
typedef HistManagerAdapter<PATMuTauPairEventActivityHistManager> PATMuTauPairEventActivityAnalyzer;
typedef HistManagerAdapter<PATDiTauPairEventActivityHistManager> PATDiTauPairEventActivityAnalyzer;
typedef HistManagerAdapter<PATElecMuPairEventActivityHistManager> PATElecMuPairEventActivityAnalyzer;

DEFINE_FWK_MODULE(DiCandidatePairEventActivityAnalyzer);
DEFINE_FWK_MODULE(PATElecTauPairEventActivityAnalyzer);
DEFINE_FWK_MODULE(PATMuTauPairEventActivityAnalyzer);
DEFINE_FWK_MODULE(PATDiTauPairEventActivityAnalyzer);
DEFINE_FWK_MODULE(PATElecMuPairEventActivityAnalyzer);

