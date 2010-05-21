#include "TauAnalysis/BgEstimationTools/plugins/PATDiTauLeptonIsolationExtractor.h"

#include "DataFormats/Common/interface/View.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

const double substituteValue = 1.e+3;

template <typename T1, typename T2>
PATDiTauLeptonIsolationExtractor<T1,T2>::PATDiTauLeptonIsolationExtractor(const edm::ParameterSet& cfg)
  : scale_(0),
    error_(0)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  leg_ = cfg.getParameter<unsigned>("leg");
  if ( leg_ != 1 && leg_ != 2 ) {
    edm::LogError("PATDiTauLeptonIsolationExtractor") << " Configuration parameter 'leg' = " << leg_ << " invalid !!";
    error_ = 1;
  }

  typedef std::vector<std::string> vstring;
  vstring isoTypes_string;
  if ( (cfg.exists("sum") && cfg.exists("count")) ||
      !(cfg.exists("sum") || cfg.exists("count")) ) {
    edm::LogError("PATDiTauLeptonIsolationExtractor") << " Need to specify either 'sum' or 'count' Configuration parameters !!";
    error_ = 1;
  } else if ( cfg.exists("sum") ) {
    isoTypes_string = cfg.getParameter<vstring>("sum");
    mode_ = kSum;
  } else if ( cfg.exists("count") ) {
    isoTypes_string = cfg.getParameter<vstring>("count");
    mode_ = kCount;
  } 

  for ( vstring::const_iterator isoType_string = isoTypes_string.begin();
	isoType_string != isoTypes_string.end(); ++isoType_string ) {
    pat::IsolationKeys isoType = pat::UserBaseIso;

    if ( (*isoType_string) == "TrackIso" ) isoType = pat::TrackIso;
    else if ( (*isoType_string) == "EcalIso" ) isoType = pat::EcalIso;
    else if ( (*isoType_string) == "HcalIso" ) isoType = pat::HcalIso;
    else if ( (*isoType_string) == "PfAllParticleIso" ) isoType = pat::PfAllParticleIso;
    else if ( (*isoType_string) == "PfChargedHadronIso" ) isoType = pat::PfChargedHadronIso;
    else if ( (*isoType_string) == "PfNeutralHadronIso" ) isoType = pat::PfNeutralHadronIso;
    else if ( (*isoType_string) == "PfGammaIso" ) isoType = pat::PfGammaIso;
    else {
      edm::LogError("PATDiTauLeptonIsolationExtractor") << " Configuration parameter 'isoType' = " << (*isoType_string) << " invalid !!";
      error_ = 1;
    }
    
    if ( isoType != -1 ) isoTypes_.push_back(isoType);
  }

  dR_ = cfg.getParameter<double>("dR");

  vstring vetos_string = cfg.getParameter<vstring>("vetos");
  for ( vstring::const_iterator veto_string = vetos_string.begin();
	veto_string != vetos_string.end(); ++veto_string ) {
    vetos_.push_back(IsoDepositVetoFactory::make(veto_string->data()));
  }

  if ( cfg.exists("scale") ) scale_ = new StringObjectFunction<diTauType>(cfg.getParameter<std::string>("scale"));

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T1, typename T2>
PATDiTauLeptonIsolationExtractor<T1,T2>::~PATDiTauLeptonIsolationExtractor()
{
  delete scale_;
}

template <typename T1, typename T2>
double PATDiTauLeptonIsolationExtractor<T1,T2>::operator()(const edm::Event& evt) const
{
  if ( error_ ) return substituteValue;
 
  typedef edm::View<diTauType> diTauCollectionType;
  edm::Handle<diTauCollectionType> diTaus;
  evt.getByLabel(src_, diTaus);

  if ( diTaus->size() > index_ ) {
    edm::Ptr<diTauType> diTauPtr = diTaus->ptrAt(index_);

    double isoSum = 0.;
    double isoCount = 0;

    for ( vIsoKeys::const_iterator isoType = isoTypes_.begin();
	  isoType != isoTypes_.end(); ++isoType ) {
      
      const reco::IsoDeposit* isoDeposit = 0;
      if ( leg_ == 1 ) isoDeposit = diTauPtr->leg1()->isoDeposit(*isoType);
      else if ( leg_ == 2 ) isoDeposit = diTauPtr->leg2()->isoDeposit(*isoType);
      if ( isoDeposit) {
	std::pair<double, int> isoValue = isoDeposit->depositAndCountWithin(dR_, vetos_);
	
	isoSum += isoValue.first;
	isoCount += isoValue.second;
      } else {
	edm::LogError("PATDiTauLeptonIsolationExtractor") << " No IsoDeposit of type = " << (*isoType) << " keyed to pat::Lepton !!";
	continue;
      }
    }

    double scaleFactor = ( scale_ ) ? (*scale_)(*diTauPtr) : 1.;

    if ( mode_ == kSum )
      return isoSum*scaleFactor;
    else if ( mode_ == kCount )
      return isoCount*scaleFactor;
  }

  return substituteValue;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATDiTauLeptonIsolationExtractor<pat::Electron, pat::Tau> PATElecTauPairLeptonIsolationExtractor;
typedef PATDiTauLeptonIsolationExtractor<pat::Electron, pat::Muon> PATElecMuPairLeptonIsolationExtractor;
typedef PATDiTauLeptonIsolationExtractor<pat::Muon, pat::Tau> PATMuTauPairLeptonIsolationExtractor;
typedef PATDiTauLeptonIsolationExtractor<pat::Tau, pat::Tau> PATDiTauPairLeptonIsolationExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecTauPairLeptonIsolationExtractor, "PATElecTauPairLeptonIsolationExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElecMuPairLeptonIsolationExtractor, "PATElecMuPairLeptonIsolationExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuTauPairLeptonIsolationExtractor, "PATMuTauPairLeptonIsolationExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATDiTauPairLeptonIsolationExtractor, "PATDiTauPairLeptonIsolationExtractor");

