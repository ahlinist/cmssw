#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"

#include <iostream>

using namespace std;

VGammaEventSelector::VGammaEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  verbose_         (params.getParameter<bool>("verbose_output") ),
  trigTag_         (params.getParameter<edm::InputTag>("trigSrc") ),
  muonTag_         (params.getParameter<edm::InputTag>("muonSrc") ),
  electronTag_     (params.getParameter<edm::InputTag>("electronSrc") ),
  photonTag_       (params.getParameter<edm::InputTag>("photonSrc") ),
  metTag_          (params.getParameter<edm::InputTag>("metSrc") ),
  dimuonTag_       (params.getParameter<edm::InputTag>("dimuonSrc") ),
  dielectronTag_   (params.getParameter<edm::InputTag>("dielectronSrc") ),
  electronMETTag_  (params.getParameter<edm::InputTag>("electronMETSrc") ),
  muonMETTag_      (params.getParameter<edm::InputTag>("muonMETSrc") ),
  zeegTag_         (params.getParameter<edm::InputTag>("zeegSrc") ),  
  zmumugTag_       (params.getParameter<edm::InputTag>("zmumugSrc")),  
  wenugTag_        (params.getParameter<edm::InputTag>("wenugSrc") ),  
  wmunugTag_       (params.getParameter<edm::InputTag>("wmunugSrc")), 
  znunugTag_       (params.getParameter<edm::InputTag>("znunugSrc")),
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  photonTrig_      (params.getParameter<std::string>("photonTrig")),
  metTrig_         (params.getParameter<std::string>("metTrig")),
  diLeptonId_      (params.getParameter<edm::ParameterSet>("diLeptonIdCfg")),
  leptonPlusMETId_ (params.getParameter<edm::ParameterSet>("leptonPlusMETIdCfg")),
  //photonPlusMETId_(params.getParameter<edm::ParameterSet>("photonPlusMETIdCfg")),
  zgphotonId_      (params.getParameter<edm::ParameterSet>("zGammaPhotonIdCfg")),
  wgphotonId_      (params.getParameter<edm::ParameterSet>("wGammaPhotonIdCfg")),
  minLeptonPhotonDR_ (params.getParameter<double>("minLeptonPhotonDeltaR"))
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Photon"  );
  push_back( "== 1 Tight Photon" );  
  push_back( "Pass DiLepton Id" );
  push_back( "Pass Lepton+MET Id" );
  push_back( "Pass Photon+MET Id" );
  push_back( "ZMuMuGamma" );
  push_back( "ZEEGamma" );
  push_back( "WMuNuGamma" );
  push_back( "WENuGamma" );
  push_back( "ZInvisibleGamma" );
  
  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 Photon"  );
  set( "== 1 Tight Photon" );  
  set( "Pass DiLepton Id"  );
  set( "Pass Lepton+MET Id" );
  set( "Pass Photon+MET Id" );
  set( "ZMuMuGamma" );
  set( "ZEEGamma" );
  set( "WMuNuGamma" );
  set( "WENuGamma" );
  set( "ZInvisibleGamma" );

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );  

  retInternal_ = getBitTemplate();
}

bool VGammaEventSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  ret.set(false);
  
  selectedZGammaPhotons_.clear();
  selectedWGammaPhotons_.clear();
  selectedZGammaMuons_.clear();
  selectedWGammaMuons_.clear();
  selectedZGammaElectrons_.clear();
  selectedWGammaElectrons_.clear();
  selectedZGammaMETs_.clear();
  selectedWGammaMETs_.clear();
  selectedZGammaDiMuons_.clear();
  selectedZGammaDiElectrons_.clear();
  selectedWGammaMuonPlusMETs_.clear();
  selectedWGammaElectronPlusMETs_.clear();

  selectedZEEGammaCands_.clear();
  selectedZMuMuGammaCands_.clear();
  selectedWENuGammaCands_.clear();
  selectedWMuNuGammaCands_.clear();

  passCut( ret, "Inclusive");

  // fill all*
  fillAll( event );

  bool passTrig = false;
  if (!ignoreCut("Trigger") ) {

    edm::Handle<pat::TriggerEvent> triggerEvent;
    event.getByLabel(trigTag_, triggerEvent);

    pat::TriggerEvent const * trig = &*triggerEvent;

    if ( trig->wasRun() && trig->wasAccept() ) {

      pat::TriggerPath const * muPath = trig->path(muTrig_);

      pat::TriggerPath const * elePath = trig->path(eleTrig_);

      if ( muPath != 0 && muPath->wasAccept() ) {
	passTrig = true;    
      }
      
      if ( elePath != 0 && elePath->wasAccept() ) {
	passTrig = true;
      }
    }
  }

  if ( ignoreCut("Trigger") || 
       passTrig ) {
    passCut(ret, "Trigger");    

    unsigned len(0);

    if(!ignoreCut("ZEEGamma")) {
      selectedZGammaElectrons_.reserve(allZEEGammaCands_->size()*2);
      selectedZGammaDiElectrons_.reserve(allZEEGammaCands_->size());
      len += allZEEGammaCands_->size();
    }

    if(!ignoreCut("ZMuMuGamma")) {
      selectedZGammaMuons_.reserve(allZMuMuGammaCands_->size()*2);
      selectedZGammaDiMuons_.reserve(allZMuMuGammaCands_->size());
      len += allZMuMuGammaCands_->size();
    }

    selectedZGammaPhotons_.reserve(len);

    // initialize selected object vector max sizes
    len = 0;

    if(!ignoreCut("WENuGamma")) {
      selectedWGammaElectrons_.reserve(len += allWENuGammaCands_->size());
      selectedWGammaMuonPlusMETs_.reserve(len);
    }

    if(!ignoreCut("WMuNuGamma")) {
      selectedWGammaMuons_.reserve(len += allWMuNuGammaCands_->size());
      selectedWGammaMuonPlusMETs_.reserve(len);
    }

    selectedWGammaMETs_.reserve(len);
    selectedWGammaPhotons_.reserve(len);

    edm::Handle< vector< reco::CompositeCandidate > > photonMETHandle(allZNuNuGammaCands_);
    
    if(ignoreCut(">= 1 Photon") || allPhotons_->size()) passCut(ret,">= 1 Photon");
    
    if(!ignoreCut("ZMuMuGamma")) {
      //std::cout << "Selecting ZMuMuGamma events!" << std::endl;
      for ( unsigned i = 0; i < allZMuMuGammaCands_->size(); ++i ) {	
	const reco::CompositeCandidate* zmumu = (const reco::CompositeCandidate*)allZMuMuGammaCands_->at(i).daughter(0);	
	const pat::Photon* photon = (const pat::Photon*)allZMuMuGammaCands_->at(i).daughter(1)->masterClonePtr().get();
	
	if( diLeptonId_(*zmumu,event) && zgphotonId_(*photon,event) ) {
	  if ( deltaR(zmumu->daughter(0),photon) > minLeptonPhotonDR_ && 
	       deltaR(zmumu->daughter(1),photon) > minLeptonPhotonDR_ ) {	  
	    
	    selectedZMuMuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( allZMuMuGammaCands_, 
														    i ) ) ); 
	    selectedZGammaMuons_.push_back( reco::ShallowClonePtrCandidate( zmumu->daughter(0)->masterClonePtr() ) );
	    selectedZGammaMuons_.push_back( reco::ShallowClonePtrCandidate( zmumu->daughter(1)->masterClonePtr() ) );
	    //selectedZGammaDiMuons_.push_back( reco::ShallowClonePtrCandidate( izmumug->daughter(0)->masterClonePtr() ) );
	    selectedZGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( allZMuMuGammaCands_->at(i).daughter(1)->masterClonePtr() ) );
	  }
	}	
      }
    }
    
    if(!ignoreCut("ZEEGamma")) {
      for ( unsigned i = 0; i < allZEEGammaCands_->size(); ++i ) {	
	const reco::CompositeCandidate *zee = (const reco::CompositeCandidate*)allZMuMuGammaCands_->at(i).daughter(0);
	const pat::Photon *photon = (const pat::Photon*)allZEEGammaCands_->at(i).daughter(1)->masterClonePtr().get();

	if( diLeptonId_(*zee,event) && zgphotonId_(*photon,event) ) {
	  if( deltaR(zee->daughter(0),photon) > minLeptonPhotonDR_ && 
	      deltaR(zee->daughter(1),photon) > minLeptonPhotonDR_ ) {
	    
	    selectedZEEGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( allZEEGammaCands_, 
														  i ) ) );
	    selectedZGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( zee->daughter(0)->masterClonePtr() ) );
	    selectedZGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( zee->daughter(1)->masterClonePtr() ) );
	    //selectedZGammaDiElectrons_.push_back( reco::ShallowClonePtrCandidate( izeeg->daughter(0)->masterClonePtr() ) );
	    selectedZGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( allZEEGammaCands_->at(i).daughter(1)->masterClonePtr() ) );
	  }
	}      
      }
    }

    if(!ignoreCut("WMuNuGamma")) {
      //std::cout << "Selecting WMuNuGamma events!" << std::endl;
      for ( unsigned i = 0; i < allWMuNuGammaCands_->size(); ++i ) {
	const reco::CompositeCandidate *wmunu = (const reco::CompositeCandidate*)allWMuNuGammaCands_->at(i).daughter(0);
	const pat::Photon *photon = (const pat::Photon*)allWMuNuGammaCands_->at(i).daughter(1)->masterClonePtr().get();
	
	if( leptonPlusMETId_(*wmunu,event) && wgphotonId_(*photon,event) ) {
	  if( deltaR(wmunu->daughter(0),photon) > minLeptonPhotonDR_ ) {
	    
	    selectedWMuNuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( allWMuNuGammaCands_, 
														    i ) ) );
	    selectedWGammaMuons_.push_back( reco::ShallowClonePtrCandidate( wmunu->daughter(0)->masterClonePtr() ) );
	    selectedWGammaMETs_.push_back( reco::ShallowClonePtrCandidate( wmunu->daughter(1)->masterClonePtr() ) );
	    //selectedWGammaMuonPlusMETs_.push_back( reco::ShallowClonePtrCandidate( iwmunug->daughter(0)->masterClonePtr() ) );
	    selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( allWMuNuGammaCands_->at(i).daughter(1)->masterClonePtr() ) );
	  }
	}
      }
    }
    
    if(!ignoreCut("WENuGamma")) {
      for ( unsigned i = 0; i < allWENuGammaCands_->size(); ++i ) {	
	const reco::CompositeCandidate *wenu = (const reco::CompositeCandidate*)allWENuGammaCands_->at(i).daughter(0);
	const pat::Photon *photon = (const pat::Photon*)allWENuGammaCands_->at(i).daughter(1)->masterClonePtr().get();
		
	if( leptonPlusMETId_(*wenu,event) && wgphotonId_(*photon,event) ) {
	  if( deltaR(wenu->daughter(0),photon) > minLeptonPhotonDR_ ) {
	    
	    selectedWENuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( allWENuGammaCands_, 
														   i ) ) );
	    selectedWGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(0)->masterClonePtr() ) );
	    selectedWGammaMETs_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(1)->masterClonePtr() ) );
	    //selectedWGammaElectronPlusMETs_.push_back( reco::ShallowClonePtrCandidate( iwenug->daughter(0)->masterClonePtr() ) );
	    selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( allWENuGammaCands_->at(i).daughter(1)->masterClonePtr() ) );
	  }
	}
      }
    }
    // when it's ready...
    if(!ignoreCut("ZInvisibleGamma")) {      
      for ( unsigned i = 0; i < allWENuGammaCands_->size(); ++i ) {
	const reco::CompositeCandidate *wenu = (const reco::CompositeCandidate*)allWENuGammaCands_->at(i).daughter(0);
	const pat::Photon *photon = (const pat::Photon*)allWENuGammaCands_->at(i).daughter(1)->masterClonePtr().get();
		
	if( leptonPlusMETId_(*wenu,event) && wgphotonId_(*photon,event) ) {
	  if( deltaR(wenu->daughter(0),photon) > minLeptonPhotonDR_ ) {
	    
	    selectedWENuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( allWENuGammaCands_, 
														   i ) ) );
	    selectedWGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(0)->masterClonePtr() ) );
	    selectedWGammaMETs_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(1)->masterClonePtr() ) );
	    //selectedWGammaElectronPlusMETs_.push_back( reco::ShallowClonePtrCandidate( iwenug->daughter(0)->masterClonePtr() ) );
	    selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( allWENuGammaCands_->at(i).daughter(1)->masterClonePtr() ) );
	  }
	}
      }
    }

    if(ignoreCut("== 1 Tight Photon") || 
       selectedZGammaPhotons_.size() == 1 ||
       selectedWGammaPhotons_.size() == 1) passCut(ret,"== 1 Tight Photon");

    //if(ignoreCut("Pass DiLepton Id") || diLeptonId_(event)) passCut(ret,"Pass DiLepton Id");
       
    //if(ignoreCut("Pass Lepton+MET Id") || leptonPlusMETId_(event)) passCut(ret,"Pass Lepton+MET Id");
    
    if(ignoreCut("Pass Photon+MET Id") || false) passCut(ret,"Pass Photon+MET Id");
       
    if(ignoreCut("ZMuMuGamma") || selectedZMuMuGammaCands_.size() == 1) passCut(ret,"ZMuMuGamma");    
    if(ignoreCut("ZEEGamma")   || selectedZEEGammaCands_.size() == 1)   passCut(ret,"ZEEGamma");    
    if(ignoreCut("WMuNuGamma") || selectedWMuNuGammaCands_.size() == 1) passCut(ret,"WMuNuGamma");
    if(ignoreCut("WENuGamma")  || selectedWENuGammaCands_.size() == 1)  passCut(ret,"WENuGamma");
    //if(ignoreCut("ZInvisibleGamma") || selectedZNuNuGammaCands().size() == 1) passCut(ret,"ZInvisibleGamma");
    
  } // end if trigger

  setIgnored(ret);
  
  return (bool)ret;
}

void VGammaEventSelector::fillAll( edm::EventBase const & event ) 
{
  event.getByLabel (photonTag_, allPhotons_);

  if(!(ignoreCut("ZMuMuGamma") && ignoreCut("WMuNuGamma")))
    event.getByLabel(muonTag_, allMuons_);
   
  if(!(ignoreCut("ZEEGamma") && ignoreCut("WENuGamma")))
    event.getByLabel(electronTag_, allElectrons_);

  if(!(ignoreCut("WENuGamma") && ignoreCut("WMuNuGamma") && ignoreCut("ZInvisibleGamma")))  
    event.getByLabel(metTag_, allMETs_);

  if(!ignoreCut("ZMuMuGamma"))
    event.getByLabel(dimuonTag_, allDiMuons_);
  
  if(!ignoreCut("ZEEGamma"))
    event.getByLabel(dielectronTag_, allDiElectrons_);

  if(!ignoreCut("WMuNuGamma"))
    event.getByLabel(muonMETTag_, allMuonPlusMETs_);

  if(!ignoreCut("WENuGamma"))
    event.getByLabel(electronMETTag_, allElectronPlusMETs_);

  if(!ignoreCut("ZEEGamma"))
    event.getByLabel(zeegTag_, allZEEGammaCands_);

  if(!ignoreCut("ZMuMuGamma"))
    event.getByLabel(zmumugTag_, allZMuMuGammaCands_);

  if(!ignoreCut("WENuGamma"))
    event.getByLabel(wenugTag_, allWENuGammaCands_);

  if(!ignoreCut("WMuNuGamma"))
    event.getByLabel(wmunugTag_, allWMuNuGammaCands_);

  if(!ignoreCut("ZInvisibleGamma"))
    event.getByLabel(znunugTag_, allZNuNuGammaCands_);
}

double VGammaEventSelector::deltaR(const reco::Candidate* one, const reco::Candidate* two) const {

  double dphi = fabs(one->phi() - two->phi());
  double deta = fabs(one->eta() - two->eta());

  while( dphi > 2*M_PI ) dphi -= 2*M_PI;
  while( dphi < 0 ) dphi += 2*M_PI;

  return std::sqrt(pow(dphi,2.)+pow(deta,2.));
}
