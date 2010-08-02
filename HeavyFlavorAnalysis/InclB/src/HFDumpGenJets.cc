#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpGenJets.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "PhysicsTools/JetMCUtils/interface/CandMCTag.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/JetMatching/interface/MatchedPartons.h"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaJet.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
using namespace JetMCTagUtils;
using namespace CandMCTagUtils;

// ----------------------------------------------------------------------
HFDumpGenJets::HFDumpGenJets(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fGenJetsLabel(iConfig.getUntrackedParameter<string>("genjetsLabel", string("iterativeCone5GenJets"))),
  fGenCandidatesLabel(iConfig.getUntrackedParameter<string>("genparticlesLabel", string("genParticles"))),
  fsourceByRefer(iConfig.getParameter<edm::InputTag>("sourceByRefer"))
 
{
  
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpGenJets constructor" << endl;
  cout << "--- " << fGenJetsLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
  
}


// ----------------------------------------------------------------------
HFDumpGenJets::~HFDumpGenJets() {
  
}


// ----------------------------------------------------------------------
void HFDumpGenJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  
  nevt++;
  // -- get the collection of GenJets 
  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByLabel( fGenJetsLabel.c_str(), genJets ); 

  // -- get the collection for flavor matching
  edm::Handle<reco::JetMatchedPartonsCollection> theTagByRef;
  iEvent.getByLabel (fsourceByRefer , theTagByRef);

  // -- get the collection of GenParticles 
  edm::Handle<GenParticleCollection> genParticlesH;
  genParticlesH.clear();
  iEvent.getByLabel (fGenCandidatesLabel.c_str(), genParticlesH);
 
  //GenParticles for Jet Flavor Tagging
  std::vector<const GenParticle *> cands;
  cands.clear();
  vector<const GenParticle *>::const_iterator found = cands.begin();
  for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
    cands.push_back( & * p );
  }

  if (fVerbose > 0) cout << "==>HFDumpGenJets> nGenJets =" << genJets->size() << endl;
  if (genJets->size() != theTagByRef->size()) {
    if (fVerbose > 0) cout << "==>HFDumpGenJets> ERROR: Different Size of JetCollections" << endl;
  }
  
  //Loop over GenJets (with Jet Flavor Tagging)
  int jetIndex=0; 
  TAnaJet *pGenJet;
  reco::GenJetCollection::const_iterator gen = genJets->begin();
  for ( JetMatchedPartonsCollection::const_iterator j  = theTagByRef->begin(); j != theTagByRef->end(); j ++ ) {
  
    const Jet *aJet             = (*j).first.get(); 
    const MatchedPartons aMatch = (*j).second;  

    if (gen->eta() != aJet->eta()) {
      if (fVerbose > 0) cout << "==>HFDumpGenJets> ERROR: Different jets in JetCollections" << endl;
    }

    pGenJet = gHFEvent->addGenJet(); 
 
    pGenJet->fIndex            = jetIndex;

    pGenJet->fPlab.SetPtEtaPhi(gen->pt(),
			       gen->eta(),
			       gen->phi()
			       );
  
    pGenJet->fQ                = gen->charge();
    pGenJet->fE                = gen->energy();
    pGenJet->fEt               = gen->et();
    pGenJet->fM                = gen->mass();
    pGenJet->fMt               = gen->mt();

    pGenJet->fEMEnergy         = gen->emEnergy(); 
    pGenJet->fHADEnergy        = gen->hadEnergy();
    pGenJet->finvisibleEnergy  = gen->invisibleEnergy();

    pGenJet->fn60              = -9999;
    pGenJet->fn90              = -9999;  

    pGenJet->fJetFlavorAlgo    = -9999;
    pGenJet->fJetFlavorPhys    = -9999;
    pGenJet->fJetFlavorEne     = -9999;

    pGenJet->fD1               = -9999; //
    pGenJet->fD2               = -9999; //fJetFlavorNear2
    pGenJet->fD3               = -9999; //fJetFlavorNear3
    pGenJet->fD4               = -9999; //fJetFlavorHeavy
    pGenJet->fD5               = -9999; //not used
    pGenJet->fD6               = -9999; //not used
    pGenJet->fD7               = -9999; //not used  


    //jet constituents
    std::vector <const GenParticle*> mcparts = gen->getGenConstituents(); 
    for (unsigned int i = 0; i < mcparts.size (); i++) {
      int index  = -1;
      const GenParticle* mcpart = mcparts[i];
      if (mcpart) {
	for (unsigned int j = 0; j < cands.size(); ++ j ) {
	  const GenParticle *p1 = (cands)[j];
	  
	  if (mcpart->pt() == p1->pt() && mcpart->phi() == p1->phi() && mcpart->eta() == p1->eta() ) 
	    index = j;
	  
	}
	pGenJet->addTrack(index);
      }
      
       
    }
  
    if (fVerbose > 0) pGenJet->dump();
    
    const GenParticleRef theHeaviest = aMatch.heaviest() ;
    if(theHeaviest.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theHeaviest.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pGenJet->fD4 = index;
      } 
      if (fVerbose > 0) {
	cout << "theHeaviest flav idx (p,eta,phi)= " 
	     << theHeaviest.get()->pdgId() << " " 
	     << index
	     << " (" << theHeaviest.get()->p() 
	     << ","  << theHeaviest.get()->eta() 
	     << ","  << theHeaviest.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef theNearest2 = aMatch.nearest_status2() ;
    if(theNearest2.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theNearest2.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pGenJet->fD2 = index;
      }
      if (fVerbose > 0) {
	cout << "theNearest Stat2  flav idx (p,eta,phi)= " 
	     << theNearest2.get()->pdgId() << " " 
	     << index
	     << " (" << theNearest2.get()->p() 
	     << ","  << theNearest2.get()->eta() 
	     << ","  << theNearest2.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef theNearest3 = aMatch.nearest_status3() ;
    if(theNearest3.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theNearest3.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pGenJet->fD3 = index;
      }
      if (fVerbose > 0) {
	cout << "theNearest Stat3  flav idx (p,eta,phi)= " 
	     << theNearest3.get()->pdgId() << " " 
	     << index
	     << " (" << theNearest3.get()->p() 
	     << ","  << theNearest3.get()->eta() 
	     << ","  << theNearest3.get()->phi() << ") " << endl; 
      }
    }
    const GenParticleRef thePhyDef = aMatch.physicsDefinitionParton() ;
    if(thePhyDef.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), thePhyDef.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pGenJet->fJetFlavorPhys = index;
      }
      if (fVerbose > 0) {
	cout << "thePhysDefinition flav idx (p,eta,phi)= " 
	     << thePhyDef.get()->pdgId() << " " 
	     << index 
	     << " (" << thePhyDef.get()->p() 
	     << ","  << thePhyDef.get()->eta() 
	     << ","  << thePhyDef.get()->phi() << ") " << endl; 
      }
     
    }
    const GenParticleRef theAlgDef = aMatch.algoDefinitionParton() ;
    if(theAlgDef.isNonnull()) {
      int index = -1;
      found = find(cands.begin(), cands.end(), theAlgDef.get());
      if (found != cands.end()) {
	index = found - cands.begin();
	pGenJet->fJetFlavorAlgo = index;
      }
      if (fVerbose > 0) {
	cout << "theAlgoDefinition flav idx (p,eta,phi)= " 
	     << theAlgDef.get()->pdgId() << " " 
	     << index 
	     << " (" << theAlgDef.get()->p() 
	     << ","  << theAlgDef.get()->eta() 
	     << ","  << theAlgDef.get()->phi() << ") " << endl; 
      } 
      
    }
   
    jetIndex++;
    gen++;
    
  } 



  

}

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpGenJets::beginJob(const edm::EventSetup& setup) {
void  HFDumpGenJets::beginJob() {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpGenJets::endJob() { 
  cout << "HFDumpGenJet>     Summary: Events processed: " << nevt << endl;  
 
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpGenJets);
