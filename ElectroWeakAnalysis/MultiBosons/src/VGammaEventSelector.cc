#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"

#include <iostream>

using namespace std;

VGammaEventSelector::VGammaEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  trigTag_         (params.getParameter<edm::InputTag>("trigSrc") ),
  photonTag_       (params.getParameter<edm::InputTag>("photonSrc") ),
  zeegTag_          (params.getParameter<edm::InputTag>("zeegSrc") ),  
  zmumugTag_        (params.getParameter<edm::InputTag>("zmumugSrc")),  
  wenugTag_         (params.getParameter<edm::InputTag>("wenugSrc") ),  
  wmunugTag_        (params.getParameter<edm::InputTag>("wmunugSrc")), 
  photonMETTag_    (params.getParameter<edm::InputTag>("photonMETSrc")),
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  photonTrig_      (params.getParameter<std::string>("photonTrig")),
  metTrig_         (params.getParameter<std::string>("metTrig")),
  diLeptonId_      (params.getParameter<edm::ParameterSet>("diLeptonIdCfg"))
  //photonId_(params.getParameter<edm::ParameterSet>("photonIdCfg")),
  //leptonPlusMETId_(params.getParameter<edm::ParameterSet>("leptonPlusMETIdCfg")),
  //photonPlusMETId_(params.getParameter<edm::ParameterSet>("photonPlusMETIdCfg")),    
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Photon"  );
  push_back( "== 1 Tight Photon" );  
  push_back( "Pass DiLepton Id"  );
  push_back( "Pass Lepton+MET Id" );
  push_back( "Pass Photon+MET Id" );

  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 Photon"  );
  set( "== 1 Tight Photon" );  
  set( "Pass DiLepton Id"  );
  set( "Pass Lepton+MET Id" );
  set( "Pass Photon+MET Id" );

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );  

  retInternal_ = getBitTemplate();
}

bool VGammaEventSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  ret.set(false);

  selectedPhotons_.clear();
  selectedZEEGammaCandidates_.clear();
  selectedZMuMuGammaCandidates_.clear();
  selectedWENuGammaCandidates_.clear();
  selectedWMuNuGammaCandidates_.clear();

  passCut( ret, "Inclusive");

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
    
        
    edm::Handle< vector< reco::CompositeCandidate > > zeegHandle;
    event.getByLabel (zeegTag_, zeegHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > zmumugHandle;
    event.getByLabel (zmumugTag_, zmumugHandle);

    edm::Handle< vector< reco::CompositeCandidate > > wenugHandle;
    event.getByLabel (wenugTag_, wenugHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > wmunugHandle;
    event.getByLabel (wmunugTag_, wmunugHandle);

    edm::Handle< vector< pat::Photon > > photonHandle;
    event.getByLabel (photonTag_, photonHandle);    

    edm::Handle< vector< reco::CompositeCandidate > > photonMETHandle;
    event.getByLabel (photonMETTag_, photonMETHandle);

    if(ignoreCut(">= 1 Photon") || photonHandle->size()) passCut(ret,">= 1 Photon");

    /*
    for ( std::vector<pat::Photon>::const_iterator photonBegin = photonHandle->begin(),
	    photonEnd = photonHandle->end(), iphoton = photonBegin;
	  iphoton != photonEnd; ++iphoton ) 
      if ( photonId_(*iphoton,event) ) 
	selectedPhotons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Photon>( photonHandle, iphoton - photonBegin ) ) );
    */
    if(ignoreCut("== 1 Tight Photon") || selectedPhotons_.size() == 1) passCut(ret,"== 1 Tight Photon");

    for ( std::vector<reco::CompositeCandidate>::const_iterator zmumugBegin = zmumugHandle->begin(),
	    zmumugEnd = zeegHandle->end(), izmumug = zmumugBegin;
	  izmumug != zmumugEnd; ++izmumug ) {
      const reco::CompositeCandidate *zmumu = dynamic_cast<const reco::CompositeCandidate*>(izmumug->daughter(0)->masterClonePtr().get());
      const pat::Photon *photon = dynamic_cast<const pat::Photon*>(izmumug->daughter(1)->masterClonePtr().get());

      if( diLeptonId_(*zmumu,event) )//&& photonId_(*photon,event) ) 
	selectedZMuMuGammaCandidates_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( zmumugHandle, 
														     izmumug - zmumugBegin ) ) );      
    }
    
    //write Zee, WEnu, WMuNu, Photon + MET

    if(ignoreCut("Pass DiLepton Id") || diLeptonId_(event)) passCut(ret,"Pass DiLepton Id");

    if(ignoreCut("Pass Lepton+MET Id") || true) passCut(ret,"Pass Lepton+MET Id");

    if(ignoreCut("Pass Photon+MET Id") || true) passCut(ret,"Pass Photon+MET Id");      

  } // end if trigger
  

  setIgnored(ret);
  
  return (bool)ret;
}
