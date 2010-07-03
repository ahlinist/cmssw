#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaDiLeptonEventSelector.h"

#include <iostream>

using namespace std;

VGammaDiLeptonEventSelector::VGammaDiLeptonEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  trigTag_         (params.getParameter<edm::InputTag>("trigSrc") ),
  muonTag_         (params.getParameter<edm::InputTag>("muonSrc") ),
  electronTag_     (params.getParameter<edm::InputTag>("electronSrc") ),  
  diMuonTag_     (params.getParameter<edm::InputTag>("dimuonSrc")),  
  diElectronTag_     (params.getParameter<edm::InputTag>("dielectronSrc")), 
  muTrig_          (params.getParameter<std::string>("muTrig")),
  eleTrig_         (params.getParameter<std::string>("eleTrig")),
  muonId1_     (params.getParameter<edm::ParameterSet>("muonId1") ),
  muonId2_        (params.getParameter<edm::ParameterSet>("muonId2") ),
  electronId1_ (params.getParameter<edm::ParameterSet>("electronId1") ),
  electronId2_    (params.getParameter<edm::ParameterSet>("electronId2") )
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 DiLepton"  );
  push_back( "== 1 Tight DiLepton" );
  push_back( "minMass" , params.getParameter<double>("minMass"));

  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 DiLepton"    );
  set( "== 1 Tight DiLepton" );
  set( "minMass" );

  ignoreCut( "minMass" );

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

      pat::TriggerPath const * muPath(NULL);
      pat::TriggerPath const * elePath(NULL);

      if(muTrig_.size())
	muPath = trig->path(muTrig_);      

      if(eleTrig_.size())
	elePath = trig->path(eleTrig_);

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
    
    if(ignoreCut(">= 1 DiLepton") || diMuonHandle->size() + diElectronHandle->size()) passCut(ret,">= 1 DiLepton");
    
    for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
	    muonEnd = muonHandle->end(), imuon = muonBegin;
	  imuon != muonEnd; ++imuon ) 
      if ( muonId1_(*imuon,event) || muonId2_(*imuon,event) ) 
	selectedMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, imuon - muonBegin ) ) );
        
    for ( std::vector<reco::CompositeCandidate>::const_iterator diMuonBegin = diMuonHandle->begin(),
	    diMuonEnd = diMuonHandle->end(), idimuon = diMuonBegin;
	  idimuon != diMuonEnd; ++idimuon ) {
      const pat::Muon *mu1 = dynamic_cast<const pat::Muon*>(idimuon->daughter(0)->masterClonePtr().get());
      const pat::Muon *mu2 = dynamic_cast<const pat::Muon*>(idimuon->daughter(1)->masterClonePtr().get());

      if( ((muonId1_(*mu1,event) && muonId2_(*mu2,event)) || 
	   (muonId2_(*mu1,event) && muonId1_(*mu2,event))) &&
	  idimuon->mass() > cut("minMass",double()) ) 
	selectedDiMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( diMuonHandle, idimuon - diMuonBegin ) ) );      
    }
    
    for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(),
	    electronEnd = electronHandle->end(), ielectron = electronBegin;
	  ielectron != electronEnd; ++ielectron ) 
      if ( electronId1_(*ielectron,event) || electronId2_(*ielectron,event) ) 
	selectedElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, ielectron - electronBegin ) ) );
    
    for ( std::vector<reco::CompositeCandidate>::const_iterator diElectronBegin = diElectronHandle->begin(),
	    diElectronEnd = diElectronHandle->end(), idielectron = diElectronBegin;
	  idielectron != diElectronEnd; ++idielectron ) {
      const pat::Electron * e1 = dynamic_cast<const pat::Electron*>(idielectron->daughter(0)->masterClonePtr().get());
      const pat::Electron * e2 = dynamic_cast<const pat::Electron*>(idielectron->daughter(1)->masterClonePtr().get());

      if( ((electronId1_(*e1,event) && electronId2_(*e2,event)) || 
	   (electronId2_(*e1,event) && electronId1_(*e2,event))) &&
	  idielectron->mass() > cut("minMass",double())) 
	selectedDiElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( diElectronHandle, idielectron - diElectronBegin ) ) );      
    }
    

    if(ignoreCut("== 1 Tight DiLepton") || 
       selectedDiMuons_.size() == 1 || 
       selectedDiElectrons_.size() == 1 ) passCut(ret,"== 1 Tight DiLepton");

  } // end if trigger
  

  setIgnored(ret);
  
  return (bool)ret;
}

bool VGammaDiLeptonEventSelector::operator() ( reco::CompositeCandidate const& diLepton , edm::EventBase const& evt) const
{
  bool ret = false;
  VGammaMuonSelector muid1(muonId1_), muid2(muonId2_);
  VGammaElectronSelector eid1(electronId1_), eid2(electronId2_);
 
  const pat::Muon *mu1 = dynamic_cast<const pat::Muon*>(diLepton.daughter(0)->masterClonePtr().get());
  const pat::Muon *mu2 = dynamic_cast<const pat::Muon*>(diLepton.daughter(1)->masterClonePtr().get());
 
  if(mu1 && mu2) {
    ret = ((bool)muid1(*mu1,evt) && (bool)muid2(*mu2,evt)) || ((bool)muid2(*mu1,evt) && (bool)muid1(*mu2,evt));
    ret = diLepton.mass() > cut("minMass",double());
  }
  
  const pat::Electron *e1 = dynamic_cast<const pat::Electron*>(diLepton.daughter(0)->masterClonePtr().get());
  const pat::Electron *e2 = dynamic_cast<const pat::Electron*>(diLepton.daughter(1)->masterClonePtr().get());

  if(e1 && e2) {
    ret = ((bool)eid1(*e1,evt) && (bool)eid2(*e2,evt)) || ((bool)eid2(*e1,evt) && (bool)eid1(*e2,evt));
    ret = diLepton.mass() > cut("minMass",double());
  }  

  return ret;
}
