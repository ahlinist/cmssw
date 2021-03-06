#include "TauAnalysis/RecoTools/plugins/PATMuonIdSelector.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/eventAuxFunctions.h"

#include <TMath.h>

#include <string>

enum { kInnerTrack, kGlobalTrack };
enum { kBeamSpot, kVertex };

//--- define default muon selection criteria used by Higgs --> tau+ tau- analysis,
//    as documented in CMS AN-11/390
//   (to be used in case selection criteria are not specified explicitely)
bool     defaultApplyGlobalMuonPromptTight_ = false;
bool     defaultApplyAllArbitrated_         = false;
double   defaultMaxIPxy_                    =  0.045; // cm
double   defaultMaxIPz_                     =  1.e+3;
int      defaultIPtrackType_                = kInnerTrack;
int      defaultIPrefType_                  = kVertex;
double   defaultMaxChi2red_                 = 10.;   // chi^2/nDoF
double   defaultMaxDptOverPt_               =  0.10;
unsigned defaultMinTrackerHits_             = 10;
unsigned defaultMinPixelHits_               =  1; 
unsigned defaultMinMuonStations_            =  1;
unsigned defaultMinMatchedSegments_         =  2;

template<typename T>
T getConfigurationParameter(const edm::ParameterSet& cfg, const std::string& cfgParName, T* cfgParDefaultValue = 0)
{
  if ( cfg.exists(cfgParName) || !cfgParDefaultValue ) return cfg.getParameter<T>(cfgParName);
  else return (*cfgParDefaultValue);
}

PATMuonIdSelectorImp::PATMuonIdSelectorImp(const edm::ParameterSet& cfg) 
  : IPtrackType_(defaultIPtrackType_),
    IPrefType_(defaultIPrefType_)
{
  applyGlobalMuonPromptTight_ = getConfigurationParameter<bool>    (cfg, "applyGlobalMuonPromptTight", &defaultApplyGlobalMuonPromptTight_);
  applyAllArbitrated_         = getConfigurationParameter<bool>    (cfg, "applyAllArbitrated",         &defaultApplyAllArbitrated_);
  maxIPxy_                    = getConfigurationParameter<double>  (cfg, "maxIPxy",                    &defaultMaxIPxy_);
  maxIPz_                     = getConfigurationParameter<double>  (cfg, "maxIPz",                     &defaultMaxIPz_);
  maxChi2red_                 = getConfigurationParameter<double>  (cfg, "maxChi2red",                 &defaultMaxChi2red_);
  maxDptOverPt_               = getConfigurationParameter<double>  (cfg, "maxDptOverPt",               &defaultMaxDptOverPt_);
  minTrackerHits_             = getConfigurationParameter<unsigned>(cfg, "minTrackerHits",             &defaultMinTrackerHits_);
  minPixelHits_               = getConfigurationParameter<unsigned>(cfg, "minPixelHits",               &defaultMinPixelHits_);
  minMuonStations_            = getConfigurationParameter<unsigned>(cfg, "minMuonStations",            &defaultMinMuonStations_);
  minMatchedSegments_         = getConfigurationParameter<unsigned>(cfg, "minMatchedSegments",         &defaultMinMatchedSegments_);
  
  if ( cfg.exists("IPtrackType") ) {
    std::string IPtrackType_string = cfg.getParameter<std::string>("IPtrackType");
    if      ( IPtrackType_string == "innerTrack"  ) IPtrackType_ = kInnerTrack;
    else if ( IPtrackType_string == "globalTrack" ) IPtrackType_ = kGlobalTrack;
    else {
      edm::LogError ("PATMuonIdSelector") 
	<< "Invalid Configuration Parameter 'IPtrackType' = " << IPtrackType_string << " --> using default !!";
    }
  }

  if ( cfg.exists("IPrefType") ) {
    std::string IPrefType_string = cfg.getParameter<std::string>("IPrefType");
    if      ( IPrefType_string == "beamSpot"  ) IPrefType_ = kBeamSpot;
    else if ( IPrefType_string == "vertex"    ) IPrefType_ = kVertex;
    else {
      edm::LogError ("PATMuonIdSelector") 
	<< "Invalid Configuration Parameter 'IPrefType' = " << IPrefType_string << " --> using default !!";
    }
  }
  
  srcBeamSpot_ = cfg.getParameter<edm::InputTag>("beamSpotSource");
  if ( IPrefType_ == kVertex ) srcVertex_ = cfg.getParameter<edm::InputTag>("vertexSource");

  //print();
}

PATMuonIdSelectorImp::~PATMuonIdSelectorImp() 
{
//--- nothing to be done yet...
}

void PATMuonIdSelectorImp::print()
{
  std::cout << "<PATMuonIdSelectorImp::print>:" <<  std::endl;
  std::cout << " srcBeamSpot = " << srcBeamSpot_.label() << std::endl;
  std::cout << " srcVertex = " << srcVertex_.label() << std::endl;
  std::cout << " applyGlobalMuonPromptTight = " << applyGlobalMuonPromptTight_ << std::endl;
  std::cout << " applyAllArbitrated = " << applyAllArbitrated_ << std::endl;
  std::cout << " maxIPxy = " << maxIPxy_ << std::endl;
  std::cout << " maxIPz = " << maxIPz_ << std::endl;
  std::cout << " maxChi2red = " << maxChi2red_ << std::endl;
  std::cout << " maxDptOverPt = " << maxDptOverPt_ << std::endl;
  std::cout << " minTrackerHits = " << minTrackerHits_ << std::endl;
  std::cout << " minPixelHits = " << minPixelHits_ << std::endl;
  std::cout << " minMuonStations = " << minMuonStations_ << std::endl;
  std::cout << " minMatchedSegments = " << minMatchedSegments_ << std::endl;
  std::cout << " IPtrackType = ";
  if ( IPtrackType_ == kInnerTrack  ) std::cout << "innerTrack"  << std::endl;
  if ( IPtrackType_ == kGlobalTrack ) std::cout << "globalTrack" << std::endl;
  std::cout << " IPrefType = ";
  if ( IPrefType_   == kBeamSpot    ) std::cout << "beamSpot"    << std::endl;
  if ( IPrefType_   == kVertex      ) std::cout << "vertex"      << std::endl;
}

void PATMuonIdSelectorImp::select(const edm::Handle<collection>& patMuonCollection,
				    edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  reco::TrackBase::Point IPrefPoint;
  bool IPrefPoint_initialized = false;

  if ( IPrefType_ == kVertex ) {
    edm::Handle<reco::VertexCollection> recoVertices;
    evt.getByLabel(srcVertex_, recoVertices);
    for ( reco::VertexCollection::const_iterator recoVertex = recoVertices->begin();
	  recoVertex != recoVertices->end(); ++recoVertex ) {
      if ( recoVertex->tracksSize() > 0 ) {
	IPrefPoint = recoVertex->position();
	IPrefPoint_initialized = true;
	break;
      }
    }
  }
  
  if ( !IPrefPoint_initialized ) {
    edm::Handle<reco::BeamSpot> beamSpot;
    evt.getByLabel(srcBeamSpot_, beamSpot);
    IPrefPoint = beamSpot->position();
  }

  for ( collection::const_iterator patMuon = patMuonCollection->begin(); 
	patMuon != patMuonCollection->end(); ++patMuon ) {

    if ( !patMuon->isGlobalMuon()                                                                ) continue;
    if ( !patMuon->isTrackerMuon()                                                               ) continue;
    if ( !isValidRef(patMuon->globalTrack())                                                     ) continue;
    if ( !isValidRef(patMuon->innerTrack())                                                      ) continue;

    if ( applyGlobalMuonPromptTight_ && !muon::isGoodMuon(*patMuon, muon::GlobalMuonPromptTight) ) continue;
    if ( applyAllArbitrated_         && !muon::isGoodMuon(*patMuon, muon::AllArbitrated)         ) continue;

    reco::TrackRef muonTrack;
    if      ( IPtrackType_ == kInnerTrack  ) muonTrack = patMuon->innerTrack();
    else if ( IPtrackType_ == kGlobalTrack ) muonTrack = patMuon->globalTrack();
    if ( !isValidRef(muonTrack)                                                                  ) continue;
    if ( !(TMath::Abs(muonTrack->dxy(IPrefPoint)) < maxIPxy_)                                    ) continue;
    if ( !(TMath::Abs(muonTrack->dz(IPrefPoint)) < maxIPz_)                                      ) continue;

    if ( !(patMuon->globalTrack()->normalizedChi2() < maxChi2red_)                               ) continue;
    if ( !(patMuon->innerTrack()->ptError() < (maxDptOverPt_*patMuon->innerTrack()->pt()))       ) continue;

    const reco::HitPattern& innerTrackHitPattern = patMuon->innerTrack()->hitPattern();
    if ( !(innerTrackHitPattern.numberOfValidTrackerHits() >= (int)minTrackerHits_)              ) continue;
    if ( !(innerTrackHitPattern.numberOfValidPixelHits() >= (int)minPixelHits_)                  ) continue;

    //---------------------------------------------------------------------------
    // compute numbers of muon stations with matched segments
    //
    // CV: code copied from version 1.41 of DataFormats/MuonReco/src/Muon.cc
    //    (not included in CMSSW_3_8_x yet)
    //
    int numMuonStations = 0;

    unsigned int theStationMask = (unsigned int)patMuon->stationMask(reco::Muon::SegmentAndTrackArbitration);
    for ( int i = 0; i < 8; ++i ) { // eight stations, eight bits
      if ( theStationMask & (1<<i) ) ++numMuonStations;
    } 
    //---------------------------------------------------------------------------

    if ( !(numMuonStations >= (int)minMuonStations_)                                             ) continue;
    if ( !(patMuon->numberOfMatches() >= (int)minMatchedSegments_)                               ) continue;

    selected_.push_back(&(*patMuon));
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATMuonIdSelectorImp> PATMuonIdSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATMuonIdSelector);
