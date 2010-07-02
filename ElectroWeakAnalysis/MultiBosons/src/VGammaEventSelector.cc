#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"

#include <iostream>

using namespace std;

VGammaEventSelector::VGammaEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
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
  znunugTag_       (params.getParameter<edm::InputTag>("photonMETSrc")),
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  photonTrig_      (params.getParameter<std::string>("photonTrig")),
  metTrig_         (params.getParameter<std::string>("metTrig")),
  diLeptonId_      (params.getParameter<edm::ParameterSet>("diLeptonIdCfg")),
  zgphotonId_      (params.getParameter<edm::ParameterSet>("zGammaPhotonIdCfg")),
  wgphotonId_      (params.getParameter<edm::ParameterSet>("wGammaPhotonIdCfg"))
  //leptonPlusMETId_(params.getParameter<edm::ParameterSet>("leptonPlusMETIdCfg")),
  //photonPlusMETId_(params.getParameter<edm::ParameterSet>("photonPlusMETIdCfg")),    
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
  push_back( "ZNuNuGamma" );
  
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
  set( "ZNuNuGamma" );
  

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );  

  retInternal_ = getBitTemplate();
}

bool VGammaEventSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  ret.set(false);

  allPhotons_.clear();
  allMuons_.clear();
  allElectrons_.clear();
  allMETs_.clear();

  allDiMuons_.clear();
  allDiElectrons_.clear();
  allMuonPlusMETs_.clear();
  allElectronPlusMETs_.clear();

  allZEEGammaCands_.clear();
  allZMuMuGammaCands_.clear();
  allWENuGammaCands_.clear();
  allWMuNuGammaCands_.clear();
  allZNuNuGammaCands_.clear();

  selectedZGammaPhotons_.clear();
  selectedWGammaPhotons_.clear();
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
    event.getByLabel (znunugTag_, photonMETHandle);

    if(ignoreCut(">= 1 Photon") || photonHandle->size()) passCut(ret,">= 1 Photon");

    
    for ( std::vector<pat::Photon>::const_iterator photonBegin = photonHandle->begin(),
	    photonEnd = photonHandle->end(), iphoton = photonBegin;
	  iphoton != photonEnd; ++iphoton ) {
      if ( zgphotonId_(*iphoton,event) ) 
	selectedZGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Photon>( photonHandle, iphoton - photonBegin ) ) );
      if ( wgphotonId_(*iphoton,event) ) 
	selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Photon>( photonHandle, iphoton - photonBegin ) ) );      
    }
    
    if(ignoreCut("== 1 Tight Photon") || 
       selectedZGammaPhotons_.size() == 1 ||
       selectedWGammaPhotons_.size() == 1) passCut(ret,"== 1 Tight Photon");


    for ( std::vector<reco::CompositeCandidate>::const_iterator zmumugBegin = zmumugHandle->begin(),
	    zmumugEnd = zeegHandle->end(), izmumug = zmumugBegin;
	  izmumug != zmumugEnd; ++izmumug ) {
      const reco::CompositeCandidate *zmumu = dynamic_cast<const reco::CompositeCandidate*>(izmumug->daughter(0)->masterClonePtr().get());
      const pat::Photon *photon = dynamic_cast<const pat::Photon*>(izmumug->daughter(1)->masterClonePtr().get());

      if( diLeptonId_(*zmumu,event) && zgphotonId_(*photon,event) ) 
	selectedZMuMuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( zmumugHandle, 
														izmumug - zmumugBegin ) ) );      
    }
    
    //write Zee, WEnu, WMuNu, Photon + MET

    if(ignoreCut("Pass DiLepton Id") || diLeptonId_(event)) passCut(ret,"Pass DiLepton Id");
       
    if(ignoreCut("Pass Lepton+MET Id") || false) passCut(ret,"Pass Lepton+MET Id");
    
    if(ignoreCut("Pass Photon+MET Id") || false) passCut(ret,"Pass Photon+MET Id");
       
    if(ignoreCut("ZMuMuGamma") || selectedZMuMuGammaCands_.size() == 1) passCut(ret,"ZMuMuGamma");    
    if(ignoreCut("ZEEGamma")   || selectedZEEGammaCands_.size() == 1)   passCut(ret,"ZEEGamma");    
    if(ignoreCut("WMuNuGamma") || selectedWMuNuGammaCands_.size() == 1) passCut(ret,"WMuNuGamma");
    if(ignoreCut("WENuGamma")  || selectedWENuGammaCands_.size() == 1)  passCut(ret,"WENuGamma");
    //if(ignoreCut("ZNuNuGamma") || selectedZNuNuGammaCands().size() == 1) passCut(ret,"ZNuNuGamma");
    
  } // end if trigger

  setIgnored(ret);
  
  return (bool)ret;
}

void VGammaEventSelector::fillAll( edm::EventBase const & event ) 
{
  edm::Handle<std::vector<pat::Photon> > allpho;
  event.getByLabel (photonTag_, allpho);  
  allPhotons_.insert(allPhotons_.begin(),allpho->begin(),allpho->end());

  edm::Handle<std::vector<pat::Muon> > allmu;
  event.getByLabel (muonTag_, allmu);  
  allMuons_.insert(allMuons_.begin(),allmu->begin(),allmu->end());

  edm::Handle<std::vector<pat::Electron> > alle;
  event.getByLabel (electronTag_, alle);  
  allElectrons_.insert(allElectrons_.begin(),alle->begin(),alle->end());
  
  edm::Handle<std::vector<pat::MET> > allmet;
  event.getByLabel (metTag_, allmet);  
  allMETs_.insert(allMETs_.begin(),allmet->begin(),allmet->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > alldimu;
  event.getByLabel (dimuonTag_, alldimu);  
  allDiMuons_.insert(allDiMuons_.begin(),alldimu->begin(),alldimu->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > alldie;
  event.getByLabel (dielectronTag_, allmet);  
  allDiElectrons_.insert(allDiElectrons_.begin(),alldie->begin(),alldie->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allmumet;
  event.getByLabel (muonMETTag_, allmet);  
  allMuonPlusMETs_.insert(allMuonPlusMETs_.begin(),allmumet->begin(),allmumet->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allemet;
  event.getByLabel (electronMETTag_, allmet);  
  allElectronPlusMETs_.insert(allElectronPlusMETs_.begin(),allemet->begin(),allemet->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allzeeg;
  event.getByLabel (zeegTag_, allzeeg);  
  allZEEGammaCands_.insert(allZEEGammaCands_.begin(),allzeeg->begin(),allzeeg->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allzmumug;
  event.getByLabel (zmumugTag_, allmet);  
  allZMuMuGammaCands_.insert(allZMuMuGammaCands_.begin(),allzmumug->begin(),allzmumug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allwenug;
  event.getByLabel (wenugTag_, allmet);  
  allWENuGammaCands_.insert(allWENuGammaCands_.begin(),allwenug->begin(),allwenug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allwmunug;
  event.getByLabel (wmunugTag_, allmet);  
  allWMuNuGammaCands_.insert(allWMuNuGammaCands_.begin(),allwmunug->begin(),allwmunug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allznunug;
  event.getByLabel (znunugTag_, allmet);  
  allZNuNuGammaCands_.insert(allZNuNuGammaCands_.begin(),allznunug->begin(),allznunug->end());
}
