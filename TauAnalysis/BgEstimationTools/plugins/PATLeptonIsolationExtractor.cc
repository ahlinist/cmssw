#include "TauAnalysis/BgEstimationTools/plugins/PATLeptonIsolationExtractor.h"

#include "DataFormats/Common/interface/View.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

const double substituteValue = 1.e+3;

template <typename T>
PATLeptonIsolationExtractor<T>::PATLeptonIsolationExtractor(const edm::ParameterSet& cfg)
  : scale_(0),
    error_(0)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  typedef std::vector<std::string> vstring;
  vstring isoTypes_string;
  if (  cfg.exists("sum") && cfg.exists("count") ||
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

    // CMSSW_3_1_x
    if ( (*isoType_string) == "TrackIso" ) isoType = pat::TrackerIso;
    else if ( (*isoType_string) == "EcalIso" ) isoType = pat::ECalIso;
    else if ( (*isoType_string) == "HcalIso" ) isoType = pat::HCalIso;
    else if ( (*isoType_string) == "ParticleIso" ) isoType = pat::ParticleIso;
    else if ( (*isoType_string) == "PfChargedHadronIso" ) isoType = pat::ChargedHadronIso;
    else if ( (*isoType_string) == "PfNeutralHadronIso" ) isoType = pat::NeutralHadronIso;
    else if ( (*isoType_string) == "PfGammaIso" ) isoType = pat::PhotonIso;
/*
    // CMSSW_3_3_x 
    if ( (*isoType_string) == "TrackIso" ) isoType = pat::TrackIso;
    else if ( (*isoType_string) == "EcalIso" ) isoType = pat::EcalIso;
    else if ( (*isoType_string) == "HcalIso" ) isoType = pat::HcalIso;
    else if ( (*isoType_string) == "PfAllParticleIso" ) isoType = pat::PfAllParticleIso;
    else if ( (*isoType_string) == "PfChargedHadronIso" ) isoType = pat::PfChargedHadronIso;
    else if ( (*isoType_string) == "PfNeutralHadronIso" ) isoType = pat::PfNeutralHadronIso;
    else if ( (*isoType_string) == "PfGammaIso" ) isoType = pat::PfGammaIso;
 */    
    else {
      edm::LogError("PATLeptonIsolationExtractor") << " Configuration parameter 'isoType' = " << (*isoType_string) << " invalid !!";
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

  if ( cfg.exists("scale") ) scale_ = new StringObjectFunction<T>(cfg.getParameter<std::string>("scale"));

  index_ = ( cfg.exists("index") ) ? cfg.getParameter<unsigned>("index") : 0;
}

template <typename T>
PATLeptonIsolationExtractor<T>::~PATLeptonIsolationExtractor()
{
  delete scale_;
}

template <typename T>
double PATLeptonIsolationExtractor<T>::operator()(const edm::Event& evt) const
{
  if ( error_ ) return substituteValue;
 
  typedef edm::View<T> patLeptonCollectionType;
  edm::Handle<patLeptonCollectionType> patLeptons;
  evt.getByLabel(src_, patLeptons);

  if ( patLeptons->size() > index_ ) {
    edm::Ptr<T> patLeptonPtr = patLeptons->ptrAt(index_);

    double isoSum = 0.;
    double isoCount = 0;

    for ( vIsoKeys::const_iterator isoType = isoTypes_.begin();
	  isoType != isoTypes_.end(); ++isoType ) {
      
      const reco::IsoDeposit* isoDeposit = patLeptonPtr->isoDeposit(*isoType);

      if ( !isoDeposit) {
	edm::LogError("PATLeptonIsolationExtractor") << " No IsoDeposit of type = " << (*isoType) << " keyed to pat::Lepton !!";
	continue;
      }

      std::pair<double, int> isoValue = isoDeposit->depositAndCountWithin(dR_, vetos_);
      
      isoSum += isoValue.first;
      isoCount += isoValue.second;
    }

    double scaleFactor = ( scale_ ) ? (*scale_)(*patLeptonPtr) : 1.;

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

typedef PATLeptonIsolationExtractor<pat::Electron> PATElectronIsolationExtractor;
typedef PATLeptonIsolationExtractor<pat::Muon> PATMuonIsolationExtractor;
typedef PATLeptonIsolationExtractor<pat::Tau> PATTauIsolationExtractor;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATElectronIsolationExtractor, "PATElectronIsolationExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATMuonIsolationExtractor, "PATMuonIsolationExtractor");
DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, PATTauIsolationExtractor, "PATTauIsolationExtractor");
