#include "SusyAnalysis/EventSelector/interface/MHTEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/MHT.h"

//__________________________________________________________________________________________________
MHTEventSelector::MHTEventSelector (const edm::ParameterSet& pset) :
  SusyEventSelector(pset),
  //  jetTag_( pset.getParameter<edm::InputTag>("jetTag") ),
  mhtTag_( pset.getParameter<edm::InputTag>("mhtTag") ),
  minMHT_( pset.getParameter<double>("minMHT") ),
  //minHT_ ( pset.getParameter<double>("minHT") ),
  minPt_ ( pset.getParameter<double>("minPt") )
{ 

  // Store computed MHT
  //  defineVariable("MHT");

  // Store MHT from PAT
  defineVariable("mht");
  defineVariable("significance");
  defineVariable("error");
  defineVariable("ht");
  defineVariable("phi");
  //defineVariable("numberOfJets");
  //defineVariable("numberOfElectrons");
  //defineVariable("numberOfMuons");
  //defineVariable("METsignificance");

}


//__________________________________________________________________________________________________
bool
MHTEventSelector::select (const edm::Event& event) const
{
  // reset cached variables
  resetVariables();


  // Get the jets  // No need for the following. 
  
  /*  edm::Handle< edm::View<pat::Jet> > jetHandle;
  event.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("MHTEventSelector") << "No Jet results for InputTag " << jetTag_;
    return false;
  }

  // Sum over jet Ets (with cut on min. pt)
  float myMHT = 0.0;
  float sumpx = 0.0;
  float sumpy = 0.0;

  edm::View<pat::Jet>::const_iterator iJet = jetHandle->begin();
  while ( iJet != jetHandle->end() ) {
    if ( iJet->pt() > minPt_ ) {
      sumpx = sumpx - iJet->px();
      sumpy = sumpy - iJet->py();
    }
    ++iJet;
  }
  myMHT = sqrt(sumpx*sumpx+sumpy*sumpy);

  // setVariable("MHT",myMHT);

  */

  // get the MHT

  edm::Handle< edm::View<pat::MHT> > mhtHandle;
  event.getByLabel(mhtTag_, mhtHandle);

  if ( !mhtHandle.isValid() ) {
    edm::LogWarning("MHTEventSelector") << "No MHT results for InputTag " << mhtTag_;
    return false;
  }

  //
  // sanity check on collection
  //
  if ( mhtHandle->size()!=1 ) {
    edm::LogWarning("MHTEventSelector") << "MHT collection size is " 
					<< mhtHandle->size() << " instead of 1";
    return false;
  }


  edm::View<pat::MHT>::const_iterator iMHT = mhtHandle->begin();
  setVariable("mht", iMHT->mht());
  setVariable("significance", iMHT->significance());
  setVariable("error", iMHT->error());
  setVariable("ht", iMHT->ht() );
  setVariable("phi", iMHT->phi() );
  //setVariable("numberOfJets", iMHT->getNumberOfJets());
  //setVariable("numberOfElectrons", iMHT->getNumberOfElectrons());
  //setVariable("numberOfMuons", iMHT->getNumberOfMuons());
  //setVariable("METsignificance", iMHT->getMETsignificance());

  return iMHT->mht()>minMHT_;

}

//__________________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MHTEventSelector, "MHTEventSelector");
