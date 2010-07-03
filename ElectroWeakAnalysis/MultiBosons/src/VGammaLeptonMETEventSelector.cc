#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaLeptonMETEventSelector.h"

#include <iostream>

using namespace std;

VGammaLeptonMETEventSelector::VGammaLeptonMETEventSelector( edm::ParameterSet const & params ) :
  EventSelector(),
  trigTag_            (params.getParameter<edm::InputTag>("trigSrc") ),
  muonTag_            (params.getParameter<edm::InputTag>("muonSrc") ),
  electronTag_        (params.getParameter<edm::InputTag>("electronSrc") ),  
  muonPlusMETTag_     (params.getParameter<edm::InputTag>("muonPlusMETSrc")),  
  electronPlusMETTag_ (params.getParameter<edm::InputTag>("electronPlusMETSrc")), 
  muTrig_             (params.getParameter<std::string>("muTrig")),
  eleTrig_            (params.getParameter<std::string>("eleTrig")),
  muonId_             (params.getParameter<edm::ParameterSet>("muonId") ),
  electronId_         (params.getParameter<edm::ParameterSet>("electronId") ),
  metId_              (params.getParameter<edm::ParameterSet>("metId"))  
{
  // make the bitset
  push_back( "Inclusive"      );
  push_back( "Trigger"        );
  push_back( ">= 1 Lepton"  );
  push_back( ">= 1 Tight Lepton"  );
  push_back( ">= 1 Lepton+MET" );
  push_back( "== 1 Selected Lepton+MET" );
  push_back( "minMT" , params.getParameter<double>("minMT") );
  push_back( "minAcoplanarity", params.getParameter<double>("minAcoplanarity") );

  // turn (almost) everything on by default
  set( "Inclusive"      );
  set( "Trigger"        );
  set( ">= 1 Lepton"    );
  set( ">= 1 Tight Lepton"    );
  set( ">= 1 Lepton+MET" );
  set( "== 1 Selected Lepton+MET" );
  set("minMT");
  set("minAcoplanarity");  

  ignoreCut("minMT");
  ignoreCut("minAcoplanarity");

  if ( params.exists("cutsToIgnore") )
    setIgnoredCuts( params.getParameter<std::vector<std::string> >("cutsToIgnore") );  

  retInternal_ = getBitTemplate();
}

bool VGammaLeptonMETEventSelector::operator() ( edm::EventBase const & event, pat::strbitset & ret)
{
  ret.set(false);

  selectedMuons_.clear();
  selectedElectrons_.clear();
  selectedMuonPlusMETs_.clear();
  selectedElectronPlusMETs_.clear();

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
    
    edm::Handle< vector< pat::MET > > metHandle;
    event.getByLabel (metTag_, metHandle);

    edm::Handle< vector< reco::CompositeCandidate > > muonPlusMETHandle;
    event.getByLabel (muonPlusMETTag_, muonPlusMETHandle);
    
    edm::Handle< vector< reco::CompositeCandidate > > electronPlusMETHandle;
    event.getByLabel (electronPlusMETTag_, electronPlusMETHandle);
    
    if(ignoreCut(">= 1 Lepton") || muonHandle->size() + electronHandle->size()) passCut(ret,">= 1 Lepton");

    if(ignoreCut( ">= 1 Lepton+MET" ) || muonPlusMETHandle->size() + electronPlusMETHandle->size()) passCut(ret,">= 1 Lepton+MET");

    for ( std::vector<pat::Muon>::const_iterator muonBegin = muonHandle->begin(),
	    muonEnd = muonHandle->end(), imuon = muonBegin;
	  imuon != muonEnd; ++imuon ) 
      if ( muonId_(*imuon,event) ) 
	selectedMuons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Muon>( muonHandle, 
										       imuon - muonBegin ) ) );
        
    for ( std::vector<reco::CompositeCandidate>::const_iterator muonMETBegin = muonPlusMETHandle->begin(),
	    muonMETEnd = muonPlusMETHandle->end(), imuonMET = muonMETBegin;
	  imuonMET != muonMETEnd; ++imuonMET ) {
      const pat::Muon *mu = dynamic_cast<const pat::Muon*>(imuonMET->daughter(0)->masterClonePtr().get());
      const pat::MET *met = dynamic_cast<const pat::MET*>(imuonMET->daughter(1)->masterClonePtr().get());

      if( muonId_(*mu,event) && 
	  metId_(*met,event) &&
	  acop(*imuonMET) > cut("minAcoplanarity",double()) &&
	  mt(*imuonMET) > cut("minMT",double())) 
	selectedMuonPlusMETs_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( muonPlusMETHandle, 
													     imuonMET - muonMETBegin ) ) );      
    }
    
    for ( std::vector<pat::Electron>::const_iterator electronBegin = electronHandle->begin(),
	    electronEnd = electronHandle->end(), ielectron = electronBegin;
	  ielectron != electronEnd; ++ielectron ) 
      if ( electronId_(*ielectron,event) ) 
	selectedElectrons_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<pat::Electron>( electronHandle, 
											       ielectron - electronBegin ) ) );
        
    for ( std::vector<reco::CompositeCandidate>::const_iterator electronMETBegin = electronPlusMETHandle->begin(),
	    electronMETEnd = electronPlusMETHandle->end(), ielectronMET = electronMETBegin;
	  ielectronMET != electronMETEnd; ++ielectronMET ) {
      const pat::Electron * ele = dynamic_cast<const pat::Electron*>(ielectronMET->daughter(0)->masterClonePtr().get());
      const pat::MET * met = dynamic_cast<const pat::MET*>(ielectronMET->daughter(1)->masterClonePtr().get());

      if( electronId_(*ele,event) && 
	  metId_(*met,event) && 
	  acop(*ielectronMET) > cut("minAcoplanarity",double()) &&
	  mt(*ielectronMET) > cut("minMT",double())
	  ) 
	selectedElectronPlusMETs_.push_back( reco::ShallowClonePtrCandidate( edm::Ptr<reco::CompositeCandidate>( electronPlusMETHandle, 
														 ielectronMET - electronMETBegin ) ) );
    }
    
    if(ignoreCut(">= 1 Tight Lepton") || 
       selectedMuons_.size() + selectedElectrons_.size() >= 1) passCut(ret,">= 1 Tight Lepton");

    if(ignoreCut("== 1 Selected Lepton+MET") || 
       selectedElectronPlusMETs_.size() == 1 ||
       selectedMuonPlusMETs_.size() == 1) passCut(ret,"== 1 Selected Lepton+MET");

  } // end if trigger
  

  setIgnored(ret);
  
  return (bool)ret;
}

bool VGammaLeptonMETEventSelector::operator() ( reco::CompositeCandidate const& leptonMET , edm::EventBase const& evt) const
{
  bool ret = false;
  VGammaMuonSelector muid(muonId_);
  VGammaElectronSelector eid(electronId_);
  VGammaMETSelector metid(metId_);
 
  const pat::Muon *mu = dynamic_cast<const pat::Muon*>(leptonMET.daughter(0)->masterClonePtr().get());
  const pat::Electron *ele = dynamic_cast<const pat::Electron*>(leptonMET.daughter(0)->masterClonePtr().get());
  const pat::MET *met = dynamic_cast<const pat::MET*>(leptonMET.daughter(1)->masterClonePtr().get());
   
  if(mu && met) {
    ret = (bool)muid(*mu,evt) && (bool)metid(*met,evt);
    ret = (acop(leptonMET) > cut("minAcoplanarity",double()));
    ret = (mt(leptonMET)   > cut("minMT",double()));
  }
						      
  
  if(ele && met) {
    ret = (bool)eid(*ele,evt) && (bool)metid(*met,evt);
    ret = (acop(leptonMET) > cut("minAcoplanarity",double()));
    ret = (mt(leptonMET)   > cut("minMT",double()));
  }
  

  return ret;
}

double VGammaLeptonMETEventSelector::acop(const reco::CompositeCandidate& c) const {
  double acoplanarity(0.0);

  const reco::Candidate* d1 = c.daughter(0);
  const reco::Candidate* d2 = c.daughter(1);

  acoplanarity = fabs(d1->phi() - d2->phi() - M_PI);

  return acoplanarity;
}

double VGammaLeptonMETEventSelector::mt(const reco::CompositeCandidate& c) const {
  double transversemass(0.0);

  const reco::Candidate* d1 = c.daughter(0);
  const reco::Candidate* d2 = c.daughter(1);

  transversemass = std::sqrt( pow(d1->pt()+d2->pt(),2.) - pow(d1->px() + d2->px(),2.) - pow(d1->py() + d2->py(),2.) );
  
  return transversemass;
}
