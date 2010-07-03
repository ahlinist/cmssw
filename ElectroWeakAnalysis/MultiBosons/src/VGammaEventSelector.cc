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
  znunugTag_       (params.getParameter<edm::InputTag>("znunugSrc")),
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  photonTrig_      (params.getParameter<std::string>("photonTrig")),
  metTrig_         (params.getParameter<std::string>("metTrig")),
  diLeptonId_      (params.getParameter<edm::ParameterSet>("diLeptonIdCfg")),
  leptonPlusMETId_ (params.getParameter<edm::ParameterSet>("leptonPlusMETIdCfg")),
  //photonPlusMETId_(params.getParameter<edm::ParameterSet>("photonPlusMETIdCfg")),
  zgphotonId_      (params.getParameter<edm::ParameterSet>("zGammaPhotonIdCfg")),
  wgphotonId_      (params.getParameter<edm::ParameterSet>("wGammaPhotonIdCfg"))        
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
    
    
    edm::Handle< vector< reco::CompositeCandidate > > zeegHandle;
    event.getByLabel (zeegTag_, zeegHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > zmumugHandle;
    event.getByLabel (zmumugTag_, zmumugHandle);

    edm::Handle< vector< reco::CompositeCandidate > > wenugHandle;
    event.getByLabel (wenugTag_, wenugHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > wmunugHandle;
    event.getByLabel (wmunugTag_, wmunugHandle);

    edm::Handle< vector< reco::CompositeCandidate > > photonMETHandle;
    event.getByLabel (znunugTag_, photonMETHandle);
    
    edm::Handle< vector< pat::Photon > > photonHandle;
    event.getByLabel (photonTag_, photonHandle);    
    
    if(ignoreCut(">= 1 Photon") || photonHandle->size()) passCut(ret,">= 1 Photon");
    
    if(!ignoreCut("ZMuMuGamma")) {
      //std::cout << "Selecting ZMuMuGamma events!" << std::endl;
      for ( std::vector<reco::CompositeCandidate>::const_iterator zmumugBegin = zmumugHandle->begin(),
	      zmumugEnd = zmumugHandle->end(), izmumug = zmumugBegin;
	    izmumug != zmumugEnd; ++izmumug ) {
	
	const reco::CompositeCandidate *zmumu = dynamic_cast<const reco::CompositeCandidate*>(izmumug->daughter(0));	
	const pat::Photon *photon = dynamic_cast<const pat::Photon*>(izmumug->daughter(1)->masterClonePtr().get());

	if( diLeptonId_(*zmumu,event) && zgphotonId_(*photon,event) ) {

	  std::cout << izmumug - zmumugBegin <<  "th ZMuMu Passed Selection" << std::endl;

	  selectedZMuMuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( zmumugHandle, 
	  													  izmumug - zmumugBegin ) ) ); 
	  selectedZGammaMuons_.push_back( reco::ShallowClonePtrCandidate( zmumu->daughter(0)->masterClonePtr() ) );
	  selectedZGammaMuons_.push_back( reco::ShallowClonePtrCandidate( zmumu->daughter(1)->masterClonePtr() ) );
	  //selectedZGammaDiMuons_.push_back( reco::ShallowClonePtrCandidate( izmumug->daughter(0)->masterClonePtr() ) );
	  selectedZGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( izmumug->daughter(1)->masterClonePtr() ) );
	}
      }
    }
    
    if(!ignoreCut("ZEEGamma")) {
      std::cout << "Selecting ZEEGamma events!" << std::endl;
      for ( std::vector<reco::CompositeCandidate>::const_iterator zeegBegin = zeegHandle->begin(),
	      zeegEnd = zeegHandle->end(), izeeg = zeegBegin;
	    izeeg != zeegEnd; ++izeeg ) {
	const reco::CompositeCandidate *zee = dynamic_cast<const reco::CompositeCandidate*>(izeeg->daughter(0));
	const pat::Photon *photon = dynamic_cast<const pat::Photon*>(izeeg->daughter(1)->masterClonePtr().get());

	if( diLeptonId_(*zee,event) && zgphotonId_(*photon,event) ) {
	  
	  std::cout << "Passed Selection" << std::endl;

	  selectedZEEGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( zeegHandle, 
														izeeg - zeegBegin ) ) );
	  selectedZGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( zee->daughter(0)->masterClonePtr() ) );
	  selectedZGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( zee->daughter(1)->masterClonePtr() ) );
	  //selectedZGammaDiElectrons_.push_back( reco::ShallowClonePtrCandidate( izeeg->daughter(0)->masterClonePtr() ) );
	  selectedZGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( izeeg->daughter(1)->masterClonePtr() ) );
	}      
      }
    }

    if(!ignoreCut("WMuNuGamma")) {
      //std::cout << "Selecting WMuNuGamma events!" << std::endl;
      for ( std::vector<reco::CompositeCandidate>::const_iterator wmunugBegin = wmunugHandle->begin(),
	      wmunugEnd = wmunugHandle->end(), iwmunug = wmunugBegin;
	    iwmunug != wmunugEnd; ++iwmunug ) {
	const reco::CompositeCandidate *wmunu = dynamic_cast<const reco::CompositeCandidate*>(iwmunug->daughter(0));
	const pat::Photon *photon = dynamic_cast<const pat::Photon*>(iwmunug->daughter(1)->masterClonePtr().get());
	
	if(  leptonPlusMETId_(*wmunu,event) && wgphotonId_(*photon,event) ) {

	  std::cout << iwmunug - wmunugBegin <<  "th WMuNuGamma Passed Selection" << std::endl;
	  
	  selectedWMuNuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( wmunugHandle, 
														  iwmunug - wmunugBegin ) ) );
	  selectedWGammaMuons_.push_back( reco::ShallowClonePtrCandidate( wmunu->daughter(0)->masterClonePtr() ) );
	  selectedWGammaMETs_.push_back( reco::ShallowClonePtrCandidate( wmunu->daughter(1)->masterClonePtr() ) );
	  //selectedWGammaMuonPlusMETs_.push_back( reco::ShallowClonePtrCandidate( iwmunug->daughter(0)->masterClonePtr() ) );
	  selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( iwmunug->daughter(1)->masterClonePtr() ) );
	}
      }
    }
    
    if(!ignoreCut("WENuGamma")) {
      std::cout << "Selecting WENuGamma events!" << std::endl;
      for ( std::vector<reco::CompositeCandidate>::const_iterator wenugBegin = wenugHandle->begin(),
	      wenugEnd = wenugHandle->end(), iwenug = wenugBegin;
	    iwenug != wenugEnd; ++iwenug ) {
	
	const reco::CompositeCandidate *wenu = dynamic_cast<const reco::CompositeCandidate*>(iwenug->daughter(0));
	const pat::Photon *photon = dynamic_cast<const pat::Photon*>(iwenug->daughter(1)->masterClonePtr().get());
		
	if( leptonPlusMETId_(*wenu,event) && wgphotonId_(*photon,event) ) {

	  std::cout << "Passed Selection" << std::endl;

	  selectedWENuGammaCands_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( wenugHandle, 
														 iwenug - wenugBegin ) ) );
	  selectedWGammaElectrons_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(0)->masterClonePtr() ) );
	  selectedWGammaMETs_.push_back( reco::ShallowClonePtrCandidate( wenu->daughter(1)->masterClonePtr() ) );
	  //selectedWGammaElectronPlusMETs_.push_back( reco::ShallowClonePtrCandidate( iwenug->daughter(0)->masterClonePtr() ) );
	  selectedWGammaPhotons_.push_back( reco::ShallowClonePtrCandidate( iwenug->daughter(1)->masterClonePtr() ) );
	}
      }
    }

    if(ignoreCut("== 1 Tight Photon") || 
       selectedZGammaPhotons_.size() == 1 ||
       selectedWGammaPhotons_.size() == 1) passCut(ret,"== 1 Tight Photon");

    if(ignoreCut("Pass DiLepton Id") || diLeptonId_(event)) passCut(ret,"Pass DiLepton Id");
       
    if(ignoreCut("Pass Lepton+MET Id") || leptonPlusMETId_(event)) passCut(ret,"Pass Lepton+MET Id");
    
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
  event.getByLabel (dielectronTag_, alldie);  
  allDiElectrons_.insert(allDiElectrons_.begin(),alldie->begin(),alldie->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allmumet;
  event.getByLabel (muonMETTag_, allmumet);  
  allMuonPlusMETs_.insert(allMuonPlusMETs_.begin(),allmumet->begin(),allmumet->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allemet;
  event.getByLabel (electronMETTag_, allemet);  
  allElectronPlusMETs_.insert(allElectronPlusMETs_.begin(),allemet->begin(),allemet->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allzeeg;
  event.getByLabel (zeegTag_, allzeeg);  
  allZEEGammaCands_.insert(allZEEGammaCands_.begin(),allzeeg->begin(),allzeeg->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allzmumug;
  event.getByLabel (zmumugTag_, allzmumug);  
  allZMuMuGammaCands_.insert(allZMuMuGammaCands_.begin(),allzmumug->begin(),allzmumug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allwenug;
  event.getByLabel (wenugTag_, allwenug);  
  allWENuGammaCands_.insert(allWENuGammaCands_.begin(),allwenug->begin(),allwenug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allwmunug;
  event.getByLabel (wmunugTag_, allwmunug);  
  allWMuNuGammaCands_.insert(allWMuNuGammaCands_.begin(),allwmunug->begin(),allwmunug->end());

  edm::Handle<std::vector<reco::CompositeCandidate> > allznunug;
  event.getByLabel (znunugTag_, allznunug);  
  allZNuNuGammaCands_.insert(allZNuNuGammaCands_.begin(),allznunug->begin(),allznunug->end());
}
