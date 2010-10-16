#include "TauAnalysis/Core/plugins/LeptonPFIsolationHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <iostream>
#include <iomanip>

const unsigned precision_dRiso = 2;
const unsigned precision_ptThreshold = 2;

template<typename T>
LeptonPFIsolationHistManager<T>::pfIsoHistogramEntryType::pfIsoHistogramEntryType(double dRisoCone)
  : dRisoCone_(dRisoCone)
{}

template<typename T>
LeptonPFIsolationHistManager<T>::pfIsoHistogramEntryType::~pfIsoHistogramEntryType()
{
// nothing to be done yet...
}

std::string getStringEncodedValue(double value, unsigned precision)
{
  std::ostringstream value_stringstream;
  value_stringstream << std::fixed << std::setprecision(precision) << value;

  int errorFlag;
  std::string value_string = replace_string(value_stringstream.str(), ".", "_", 1, 1, errorFlag);

  return value_string;
}

MonitorElement* bookIsoHistogram(
		  DQMStore& dqmStore, const std::string& pfIsoType, double dRsig, 
		  const std::string& ptThresholdLabel_name, const std::string& ptThresholdLabel_title, 
		  const std::string& matchType)
{
  std::string dRsig_string = getStringEncodedValue(dRsig, precision_dRiso);

  std::string isoHistogramName = std::string(pfIsoType).append(dRsig_string).append("dRsig").append(ptThresholdLabel_name).append("GeV");
  if ( matchType != "" ) isoHistogramName.append("_").append(matchType);
  std::ostringstream isoHistogramTitle;
  isoHistogramTitle << pfIsoType << " ("
		    << "dR_{sig} = " << std::fixed << std::setprecision(precision_dRiso) << dRsig << ","
		    << " " << ptThresholdLabel_title;
  if ( matchType != "" ) isoHistogramTitle << ", " << matchType;
  isoHistogramTitle << ")";  
  MonitorElement* isoHistogram = dqmStore.book1D(isoHistogramName.data(), isoHistogramTitle.str().data(), 100, -0.01, 25.);
  
  return isoHistogram;
}

template<typename T>
void LeptonPFIsolationHistManager<T>::pfIsoHistogramEntryType::bookHistograms(
       DQMStore& dqmStore, const std::string& dqmDirectory, double ptThreshold, 
       const std::string& type)
{
  dqmStore.setCurrentFolder(dqmDirectory);

  std::string ptThresholdLabel_name = getStringEncodedValue(ptThreshold, precision_ptThreshold);

  std::ostringstream ptThresholdLabel_title;
  ptThresholdLabel_title << "P_{T} > " << std::fixed << std::setprecision(precision_ptThreshold) << ptThreshold << " GeV"; 

  hPFCandIso0_05dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.05, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
  hPFCandIso0_10dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.10, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
  hPFCandIso0_15dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.15, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
  hPFCandIso0_20dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.20, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
  hPFCandIso0_25dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.25, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
  hPFCandIso0_30dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.30, ptThresholdLabel_name, ptThresholdLabel_title.str(), type);
}

template<typename T>
void LeptonPFIsolationHistManager<T>::pfIsoHistogramEntryType::bookHistograms(
       DQMStore& dqmStore, const std::string& dqmDirectory, 
       double ptThresholdChargedHadron, double ptThresholdPhoton, 
       const std::string& type)
{
  dqmStore.setCurrentFolder(dqmDirectory);

  std::ostringstream ptThresholdLabel_name;
  ptThresholdLabel_name << getStringEncodedValue(ptThresholdChargedHadron, precision_ptThreshold)
			<< "_"
			<< getStringEncodedValue(ptThresholdPhoton, precision_ptThreshold);

  std::ostringstream ptThresholdLabel_title;
  ptThresholdLabel_title << "P_{T}^{h} > " << std::fixed << std::setprecision(precision_ptThreshold) << ptThresholdChargedHadron << ","
			 << " P_{T}^{gamma} > " << std::fixed << std::setprecision(precision_ptThreshold) << ptThresholdPhoton << " GeV"; 
  
  hPFCandIso0_05dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.05, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
  hPFCandIso0_10dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.10, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
  hPFCandIso0_15dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.15, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
  hPFCandIso0_20dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.20, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
  hPFCandIso0_25dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.25, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
  hPFCandIso0_30dRsigCone_ = bookIsoHistogram(dqmStore, "PFCandIso", 0.30, ptThresholdLabel_name.str(), ptThresholdLabel_title.str(), type);
}

void fillIsoHistogram(
       MonitorElement* h, const reco::Particle::LorentzVector& particleP4, 
       std::vector<const reco::PFCandidate*> pfCandidates, double dRsigCone, double dRisoCone, double weight)
{
  //std::cout << "<LeptonPFIsolationHistManager::fillIsoHistogram>:" << std::endl;
  //std::cout << " particle: Pt = " << particleP4.pt() << ", eta = " << particleP4.eta() << ", phi = " << particleP4.phi() << std::endl;
  //std::cout << " dRsigCone = " << dRsigCone << std::endl;
  //std::cout << " dRisoCone = " << dRisoCone << std::endl;
  
  double isoPtSum = 0.;

  for ( std::vector<const reco::PFCandidate*>::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    double dR = deltaR(particleP4, (*pfCandidate)->p4());
    if ( dR > dRsigCone && dR < dRisoCone ) isoPtSum += (*pfCandidate)->pt();
  }

  //std::cout << "--> isoPtSum = " << isoPtSum << " (weight = " << weight << ")" << std::endl;

  h->Fill(isoPtSum, weight);
}

template<typename T>	      
void LeptonPFIsolationHistManager<T>::pfIsoHistogramEntryType::fillHistograms(
       const reco::Particle::LorentzVector& particleP4, const std::vector<const reco::PFCandidate*>& pfCandidates, double weight)
{
  fillIsoHistogram(hPFCandIso0_05dRsigCone_, particleP4, pfCandidates, 0.05, dRisoCone_, weight);
  fillIsoHistogram(hPFCandIso0_10dRsigCone_, particleP4, pfCandidates, 0.10, dRisoCone_, weight);
  fillIsoHistogram(hPFCandIso0_15dRsigCone_, particleP4, pfCandidates, 0.15, dRisoCone_, weight);
  fillIsoHistogram(hPFCandIso0_20dRsigCone_, particleP4, pfCandidates, 0.20, dRisoCone_, weight);
  fillIsoHistogram(hPFCandIso0_25dRsigCone_, particleP4, pfCandidates, 0.25, dRisoCone_, weight);
  fillIsoHistogram(hPFCandIso0_30dRsigCone_, particleP4, pfCandidates, 0.30, dRisoCone_, weight);
}

//
//-------------------------------------------------------------------------------
//

template<typename T>
LeptonPFIsolationHistManager<T>::LeptonPFIsolationHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<LeptonPFIsolationHistManager>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("LeptonPFIsolationHistManager") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!";
    dqmError_ = 1;
    return;
  }

  dqmStore_ = &(*edm::Service<DQMStore>());

  leptonSrc_ = cfg.getParameter<edm::InputTag>("leptonSource");
  leptonPtMin_ = cfg.getParameter<double>("ptMin");
  leptonPtMax_ = cfg.getParameter<double>("ptMax");
  leptonEtaMin_ = cfg.getParameter<double>("etaMin");
  leptonEtaMax_ = cfg.getParameter<double>("etaMax");

  //std::cout << " leptonPtMin = " << leptonPtMin_ << std::endl;
  //std::cout << " leptonPtMax = " << leptonPtMax_ << std::endl;
  //std::cout << " leptonEtaMin = " << leptonEtaMin_ << std::endl;
  //std::cout << " leptonEtaMax = " << leptonEtaMax_ << std::endl;
  
  dRisoCone_ = cfg.getParameter<double>("dRisoCone");

  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");

  leptonWeightExtractors_ = getTauJetWeightExtractors<T>(cfg, "leptonWeightSource");
  
  std::string normalization_string = cfg.getParameter<std::string>("normalization");
  normMethod_ = getNormMethod(normalization_string, "leptons");
  
  edm::ParameterSet cfgGenLeptonMatch = cfg.getParameter<edm::ParameterSet>("genLeptonMatch");
  genLeptonMatch_ = new PATLeptonGenMatcher<T>(cfgGenLeptonMatch);

  pfIsoHistogramsPtThreshold0_5GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold0_5GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold1_0GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold1_0GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold1_5GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold1_5GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold2_0GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold2_0GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold2_5GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold2_5GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold3_0GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPtThreshold3_0GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);

  pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVmatched_   = new pfIsoHistogramEntryType(dRisoCone_);
  pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVunmatched_ = new pfIsoHistogramEntryType(dRisoCone_);
}

template<typename T>
LeptonPFIsolationHistManager<T>::~LeptonPFIsolationHistManager()
{
  for ( typename std::vector<FakeRateJetWeightExtractor<T>*>::iterator it = leptonWeightExtractors_.begin();
	it != leptonWeightExtractors_.end(); ++it ) {
    delete (*it);
  }

  delete pfIsoHistogramsPtThreshold0_5GeVmatched_;
  delete pfIsoHistogramsPtThreshold0_5GeVunmatched_;
  delete pfIsoHistogramsPtThreshold1_0GeVmatched_;
  delete pfIsoHistogramsPtThreshold1_0GeVunmatched_;
  delete pfIsoHistogramsPtThreshold1_5GeVmatched_;
  delete pfIsoHistogramsPtThreshold1_5GeVunmatched_;
  delete pfIsoHistogramsPtThreshold2_0GeVmatched_;
  delete pfIsoHistogramsPtThreshold2_0GeVunmatched_;
  delete pfIsoHistogramsPtThreshold2_5GeVmatched_;
  delete pfIsoHistogramsPtThreshold2_5GeVunmatched_;
  delete pfIsoHistogramsPtThreshold3_0GeVmatched_;
  delete pfIsoHistogramsPtThreshold3_0GeVunmatched_;

  delete pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVmatched_;
  delete pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVunmatched_;
}

template<typename T>
void LeptonPFIsolationHistManager<T>::bookHistogramsImp()
{
  pfIsoHistogramsPtThreshold0_5GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 0.5, "matched");
  pfIsoHistogramsPtThreshold0_5GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 0.5, "unmatched");
  pfIsoHistogramsPtThreshold1_0GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.0, "matched");
  pfIsoHistogramsPtThreshold1_0GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.0, "unmatched");
  pfIsoHistogramsPtThreshold1_5GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.5, "matched");
  pfIsoHistogramsPtThreshold1_5GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.5, "unmatched");
  pfIsoHistogramsPtThreshold2_0GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 2.0, "matched");
  pfIsoHistogramsPtThreshold2_0GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 2.0, "unmatched");
  pfIsoHistogramsPtThreshold2_5GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 2.5, "matched");
  pfIsoHistogramsPtThreshold2_5GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 2.5, "unmatched");
  pfIsoHistogramsPtThreshold3_0GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 3.0, "matched");
  pfIsoHistogramsPtThreshold3_0GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 3.0, "unmatched");

  pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.00, 1.50, "matched");
  pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVunmatched_->bookHistograms(*dqmStore_, dqmDirectory_store_, 1.00, 1.50, "unmatched");

  bookWeightHistograms(*dqmStore_, "LeptonWeight", "Lepton Weight", 
		       hLeptonWeightPosLog_, hLeptonWeightNegLog_, hLeptonWeightZero_,
		       hLeptonWeightLinear_);
}

template<typename T>
double LeptonPFIsolationHistManager<T>::getLeptonWeight(const T& lepton)
{
  return getTauJetWeight<T>(lepton, leptonWeightExtractors_);
}

std::vector<const reco::PFCandidate*> filterPFCandidates(const reco::PFCandidateCollection& pfCandidates, double ptThreshold)
{
  std::vector<const reco::PFCandidate*> filteredPFCandidates;
  
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    if ( pfCandidate->pt() > ptThreshold ) filteredPFCandidates.push_back(&(*pfCandidate));
  }
  
  return filteredPFCandidates;
}

void filterSelPFCandidateType(
       const reco::PFCandidateCollection& pfCandidates, reco::PFCandidate::ParticleType pfParticleType, double ptThreshold, 
       std::vector<const reco::PFCandidate*>& filteredPFCandidates)
{
  for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	pfCandidate != pfCandidates.end(); ++pfCandidate ) {
    if ( pfCandidate->particleId() == pfParticleType && pfCandidate->pt() > ptThreshold ) 
      filteredPFCandidates.push_back(&(*pfCandidate));
  }
}

template<typename T>
void LeptonPFIsolationHistManager<T>::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{
  //std::cout << "<LeptonPFIsolationHistManager::fillHistogramsImp>:" << std::endl;
  //std::cout << " leptonSrc = " << leptonSrc_.label() << std::endl;

  typedef std::vector<T> leptonCollection;
  edm::Handle<leptonCollection> leptons;
  evt.getByLabel(leptonSrc_, leptons);
  
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  double leptonWeightSum = 0.;
  for ( typename leptonCollection::const_iterator lepton = leptons->begin();
	lepton != leptons->end(); ++lepton ) {
    leptonWeightSum += getLeptonWeight(*lepton);
  }
  
  for ( typename leptonCollection::const_iterator lepton = leptons->begin();
	lepton != leptons->end(); ++lepton ) {
    
    //std::cout << " lepton: Pt = " << lepton->pt() << ", eta = " << lepton->eta() << ", phi = " << lepton->phi() << std::endl;

    double leptonWeight = getLeptonWeight(*lepton);
    double weight = getWeight(evtWeight, leptonWeight, leptonWeightSum);

    fillWeightHistograms(hLeptonWeightPosLog_, hLeptonWeightNegLog_, hLeptonWeightZero_, 
			 hLeptonWeightLinear_, leptonWeight);

    const reco::Particle::LorentzVector& leptonP4 = lepton->p4();

    if ( leptonP4.pt()  > leptonPtMin_  && leptonP4.pt()  < leptonPtMax_ &&
	 leptonP4.eta() > leptonEtaMin_ && leptonP4.eta() < leptonEtaMax_ ) {
      bool isGenMatched = (*genLeptonMatch_)(evt, *lepton);
      //std::cout << " isGenMatched = " << isGenMatched << std::endl;

      std::vector<const reco::PFCandidate*> pfCandidates0_5GeV = filterPFCandidates(*pfCandidates, 0.5);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold0_5GeVmatched_->fillHistograms(leptonP4, pfCandidates0_5GeV, weight);
      else                pfIsoHistogramsPtThreshold0_5GeVunmatched_->fillHistograms(leptonP4, pfCandidates0_5GeV, weight);

      std::vector<const reco::PFCandidate*> pfCandidates1_0GeV = filterPFCandidates(*pfCandidates, 1.0);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold1_0GeVmatched_->fillHistograms(leptonP4, pfCandidates1_0GeV, weight);
      else                pfIsoHistogramsPtThreshold1_0GeVunmatched_->fillHistograms(leptonP4, pfCandidates1_0GeV, weight);

      std::vector<const reco::PFCandidate*> pfCandidates1_5GeV = filterPFCandidates(*pfCandidates, 1.5);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold1_5GeVmatched_->fillHistograms(leptonP4, pfCandidates1_5GeV, weight);
      else                pfIsoHistogramsPtThreshold1_5GeVunmatched_->fillHistograms(leptonP4, pfCandidates1_5GeV, weight);

      std::vector<const reco::PFCandidate*> pfCandidates2_0GeV = filterPFCandidates(*pfCandidates, 2.0);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold2_0GeVmatched_->fillHistograms(leptonP4, pfCandidates2_0GeV, weight);
      else                pfIsoHistogramsPtThreshold2_0GeVunmatched_->fillHistograms(leptonP4, pfCandidates2_0GeV, weight);

      std::vector<const reco::PFCandidate*> pfCandidates2_5GeV = filterPFCandidates(*pfCandidates, 2.5);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold2_5GeVmatched_->fillHistograms(leptonP4, pfCandidates2_5GeV, weight);
      else                pfIsoHistogramsPtThreshold2_5GeVunmatched_->fillHistograms(leptonP4, pfCandidates2_5GeV, weight);

      std::vector<const reco::PFCandidate*> pfCandidates3_0GeV = filterPFCandidates(*pfCandidates, 3.0);
      if ( isGenMatched ) pfIsoHistogramsPtThreshold3_0GeVmatched_->fillHistograms(leptonP4, pfCandidates3_0GeV, weight);
      else                pfIsoHistogramsPtThreshold3_0GeVunmatched_->fillHistograms(leptonP4, pfCandidates3_0GeV, weight);

      std::vector<const reco::PFCandidate*> selPFChargedHadronsPt1_0PFGammasPt1_5GeV;
      filterSelPFCandidateType(*pfCandidates, reco::PFCandidate::h, 1.0, selPFChargedHadronsPt1_0PFGammasPt1_5GeV);
      filterSelPFCandidateType(*pfCandidates, reco::PFCandidate::gamma, 1.5, selPFChargedHadronsPt1_0PFGammasPt1_5GeV);
      if ( isGenMatched ) pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVmatched_->fillHistograms(
                            leptonP4, selPFChargedHadronsPt1_0PFGammasPt1_5GeV, weight);
      else                pfIsoHistogramsPFChargedHadronPt1_0PFGammaPt1_5GeVunmatched_->fillHistograms(
                            leptonP4, selPFChargedHadronsPt1_0PFGammasPt1_5GeV, weight);
    }
  }
}

#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

typedef LeptonPFIsolationHistManager<pat::Tau> TauPFIsolationHistManager;
typedef LeptonPFIsolationHistManager<pat::Muon> MuonPFIsolationHistManager;
typedef LeptonPFIsolationHistManager<pat::Electron> ElectronPFIsolationHistManager;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauPFIsolationHistManager, "TauPFIsolationHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauPFIsolationHistManager, "TauPFIsolationHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, MuonPFIsolationHistManager, "MuonPFIsolationHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, MuonPFIsolationHistManager, "MuonPFIsolationHistManager");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ElectronPFIsolationHistManager, "ElectronPFIsolationHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, ElectronPFIsolationHistManager, "ElectronPFIsolationHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauPFIsolationHistManager> TauPFIsolationAnalyzer;
typedef HistManagerAdapter<MuonPFIsolationHistManager> MuonPFIsolationAnalyzer;
typedef HistManagerAdapter<ElectronPFIsolationHistManager> ElectronPFIsolationAnalyzer;

DEFINE_FWK_MODULE(TauPFIsolationAnalyzer);
DEFINE_FWK_MODULE(MuonPFIsolationAnalyzer);
DEFINE_FWK_MODULE(ElectronPFIsolationAnalyzer);
