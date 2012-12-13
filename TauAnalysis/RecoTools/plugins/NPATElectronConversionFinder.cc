//
// Implement photon conversion veto for electron identification
// $Id: NPATElectronConversionFinder.cc,v 1.3 2011/07/21 09:34:24 jkolb Exp $
//
//
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "TauAnalysis/RecoTools/plugins/NPATElectronConversionFinder.h"

NPATElectronConversionFinderImp::NPATElectronConversionFinderImp(const edm::ParameterSet& iConfig)
  : maxMissingInnerHits_(0),
    minMissingInnerHits_(0),
    minRxy_(2),
    minFitProb_(1e-6),
    maxHitsBeforeVertex_(0),
    invertConversionVeto_(0)
{
  //  set minimum/maximum number of inner missing hits
  if ( iConfig.exists("maxMissingInnerHits") ) 
    maxMissingInnerHits_ = iConfig.getParameter<int>("maxMissingInnerHits");
  if ( iConfig.exists("minMissingInnerHits") ) 
    minMissingInnerHits_ = iConfig.getParameter<int>("minMissingInnerHits");
	
  //  change default values for conversion compatibility test
  if ( iConfig.exists("minRxy") ) 
    minRxy_ = iConfig.getParameter<double>("minRxy");
  if ( iConfig.exists("minFitProb") ) 
    minFitProb_ = iConfig.getParameter<double>("minFitProb");
  if ( iConfig.exists("maxHitsBeforeVertex") ) 
    maxHitsBeforeVertex_ = iConfig.getParameter<int>("maxHitsBeforeVertex");
  
  //  switch to *select* conversions identified with minRxy, minFitProb, and maxHitsBeforeVertex parameters
  if ( iConfig.exists("invertConversionVeto") ) 
    invertConversionVeto_ = iConfig.getParameter<bool>("invertConversionVeto");
}

NPATElectronConversionFinderImp::~NPATElectronConversionFinderImp() {}

void NPATElectronConversionFinderImp::select(const edm::Handle<collection>& electrons,
		edm::Event& event, const edm::EventSetup& setup)
{
  selected_.clear();

  edm::Handle<reco::BeamSpot> bsHandle;
  event.getByLabel("offlineBeamSpot", bsHandle);
  const reco::BeamSpot& theBS = *bsHandle;

  edm::Handle<reco::ConversionCollection> hConversions;
  event.getByLabel("allConversions", hConversions);

  // iterate over PAT electrons in event
  for ( std::vector<pat::Electron>::const_iterator elec = electrons->begin(); elec != electrons->end(); ++elec ){
    //  calculate compatibility with conversion
    bool passConversionVeto = !ConversionTools::hasMatchedConversion(*elec, hConversions, theBS.position(), true, minRxy_, minFitProb_, maxHitsBeforeVertex_);
        
    // invert conversion veto if we want to select conversions
    if (invertConversionVeto_)
      passConversionVeto = !passConversionVeto;

    //  calculate number of missing inner pixel hits
    const reco::Track* elec_track = elec->gsfTrack().get();  
    const reco::HitPattern& p_inner = elec_track->trackerExpectedHitsInner(); 
    
    //  save electrons that pass inner hits cut and conversion veto
    if ( p_inner.numberOfHits() <= maxMissingInnerHits_ && p_inner.numberOfHits() >= minMissingInnerHits_ && passConversionVeto ) 
      selected_.push_back(&(*elec));
    
  } // end PAT electron loop
}

typedef ObjectSelector<NPATElectronConversionFinderImp> NPATElectronConversionFinder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NPATElectronConversionFinder);
