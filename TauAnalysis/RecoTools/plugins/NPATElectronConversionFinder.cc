//
// Implement photon conversion veto for electron identification
// $Id: PATElectronConversionFinder.cc,v 1.11 2011/04/11 15:32:16 jkolb Exp $
//
//
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "TauAnalysis/RecoTools/plugins/NPATElectronConversionFinder.h"

NPATElectronConversionFinderImp::NPATElectronConversionFinderImp(const edm::ParameterSet& iConfig):
    maxMissingInnerHits_(0),
    minRxy_(2),
    minFitProb_(1e-6),
    maxHitsBeforeVertex_(0)
{
	//  set maximum number of inner missing hits
	if( iConfig.exists("maxMissingInnerHits") ) 
	    maxMissingInnerHits_ = iConfig.getParameter<int>("maxMissingInnerHits");
	
    //  change default values for conversion compatibility test
    if( iConfig.exists("minRxy") ) 
        minRxy_ = iConfig.getParameter<int>("minRxy");
    if( iConfig.exists("minFitProb") ) 
	    minFitProb_ = iConfig.getParameter<int>("minFitProb");
    if( iConfig.exists("maxHitsBeforeVertex") ) 
	    maxHitsBeforeVertex_ = iConfig.getParameter<int>("maxHitsBeforeVertex");
}


NPATElectronConversionFinderImp::~NPATElectronConversionFinderImp() {}

void NPATElectronConversionFinderImp::select(const edm::Handle<collection>& electrons,
		edm::Event& event, const edm::EventSetup& setup)
{
    selected_.clear();

    edm::Handle<reco::BeamSpot> bsHandle;
    event.getByLabel("offlineBeamSpot", bsHandle);
    const reco::BeamSpot &theBS = *bsHandle.product();

    edm::Handle<reco::ConversionCollection> hConversions;
    // for CMSSW 4_1_X
    event.getByLabel("trackerOnlyConversions", hConversions);
    // for CMSSW 4_2_X
    //event.getByLabel("allConversions", hConversions);

    // iterate over PAT electrons in event
    for( std::vector<pat::Electron>::const_iterator elec = electrons->begin(); elec != electrons->end(); ++elec)
    {
        //  calculate compatibility with conversion
        bool passConversionVeto = 
            !ConversionTools::hasMatchedConversion(*elec,hConversions,theBS.position(), true, minRxy_, minFitProb_, maxHitsBeforeVertex_);

        //  calculate number of missing inner pixel hits
        const reco::Track *elec_track = (const reco::Track*)(elec->gsfTrack().get());  
        const reco::HitPattern& p_inner = elec_track->trackerExpectedHitsInner(); 

        //  save electrons that pass inner hits cut and conversion veto
        if( p_inner.numberOfHits() <= maxMissingInnerHits_ && passConversionVeto) 
            selected_.push_back(&(*elec));

    } // end PAT electron loop
}

typedef ObjectSelector<NPATElectronConversionFinderImp> NPATElectronConversionFinder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NPATElectronConversionFinder);
