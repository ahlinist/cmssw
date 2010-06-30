#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonEventSelector.h"

#include <iostream>

using namespace std;

VGammaDiLeptonEventSelector::VGammaDiLeptonEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  trigTag_         (params.getParameter<edm::InputTag>("trigSrc") ),
  muonTag_         (params.getParameter<edm::InputTag>("muonSrc") ),
  electronTag_     (params.getParameter<edm::InputTag>("electronSrc") ),  
  diMuonTag_     (params.getParameter<edm::InputTag>("diMuonSrc")),  
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  muonId_     (params.getParameter<edm::ParameterSet>("muonId") ),
  diMuonId_        (params.getParameter<edm::ParameterSet>("muonId") ),
  //  electronId_ (params.getParameter<edm::ParameterSet>("electronId") ),
  //  diElectronId_    (params.getParameter<edm::ParameterSet>("electronId") ),  
  doee             (params.getParameter<bool>("selectDiElectrons") ),
  domumu           (params.getParameter<bool>("selectDiMuons") )
  
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 DiLepton"  );
  push_back( "== 1 Tight DiLepton" );

  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 DiLepton"    );
  set( "== 1 Tight DiLepton" );

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );  

  retInternal_ = getBitTemplate();
}

bool VGammaDiLeptonEventSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  ret.set(false);

  selectedMuons_.clear();
  selectedElectrons_.clear();
  selectedDiMuons_.clear();
  selectedDiElectrons_.clear();

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
    
    
    edm::Handle< vector< pat::Electron > > electronHandle;
    event.getByLabel (electronTag_, electronHandle);
    
    edm::Handle< vector< pat::Muon > > muonHandle;
    event.getByLabel (muonTag_, muonHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > diMuonHandle;
    event.getByLabel (diMuonTag_, diMuonHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > diElectronHandle;
    event.getByLabel (diElectronTag_, diElectronHandle);
    
    if((domumu && diMuonHandle->size()) || (doee && diElectronHandle->size())) passCut(ret,">= 1 DiLepton");

    for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
	    muonEnd = muonHandle->end(), imuon = muonBegin;
	  imuon != muonEnd; ++imuon ) 
      if ( muonId_(*imuon,event) ) 
	selectedMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
        
    for ( std::vector<reco::CompositeCandidate>::const_iterator diMuonBegin = diMuonHandle->begin(),
	    diMuonEnd = diMuonHandle->end(), idimuon = diMuonBegin;
	  idimuon != diMuonEnd; ++idimuon ) {
      const pat::Muon *mu1 = dynamic_cast<const pat::Muon*>(idimuon->daughter(0)->masterClonePtr().get());
      const pat::Muon *mu2 = dynamic_cast<const pat::Muon*>(idimuon->daughter(1)->masterClonePtr().get());

      if( diMuonId_(*mu1,event) && diMuonId_(*mu2,event) ) 
	selectedDiMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( diMuonHandle, idimuon - diMuonBegin ) ) );      
    }
    /*
    for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(),
	    electronEnd = electronHandle->end(), ielectron = electronBegin;
	  ielectron != electronEnd; ++ielectron ) 
      if ( electronId_(*ielectron,event) ) 
	selectedElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, ielectron - electronBegin ) ) );
        
    for ( std::vector<reco::CompositeCandidate>::const_iterator diElectronBegin = diElectronHandle->begin(),
	    diElectronEnd = electronHandle->diElectronHandle->end(), idielectron = diElectronBegin;
	  idielectron != diElectronEnd; ++idielectron ) {
      const pat::Electron * mu1 = dynamic_cast<const pat::Electron*>(idielectron->daughter(0)->masterClonePtr().get());
      const pat::Electron * mu2 = dynamic_cast<const pat::Electron*>(idielectron->daughter(1)->masterClonePtr().get());

      if( diElectronId_(*mu1,event) && diElectronId_(*mu2,event) ) 
	selectedDiElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( diElectronHandle, idielectron - diElectronBegin ) ) );      
    }
    */ 

    if((domumu && selectedDiMuons_.size()) || (doee && selectedDiElectrons_.size() == 1)) passCut(ret,"== 1 Tight DiLepton");

  } // end if trigger
  

  setIgnored(ret);
  
  return (bool)ret;
}
