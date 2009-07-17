#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpJets.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "PhysicsTools/JetMCUtils/interface/JetMCTag.h"
#include "PhysicsTools/JetMCUtils/interface/CandMCTag.h"

//#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"
//#include "PhysicsTools/JetMCAlgos/plugins/JetFlavourIdentifier.cc"
#include "SimDataFormats/JetMatching/interface/JetMatchedPartons.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"

// -- Yikes!
extern TAna01Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
using namespace JetMCTagUtils;
using namespace CandMCTagUtils;

// ----------------------------------------------------------------------
HFDumpJets::HFDumpJets(const edm::ParameterSet& iConfig):
  fJetsLabel(iConfig.getUntrackedParameter<string>("jetsLabel", string("iterativeCone5CaloJets"))),
  fGenJetsLabel(iConfig.getUntrackedParameter<string>("genjetsLabel", string("iterativeCone5GenJets"))),
  fDisc1(iConfig.getUntrackedParameter<string>("discriminator1", string("softMuonBJetTags"))),
  fDisc2(iConfig.getUntrackedParameter<string>("discriminator2", string("softMuonNoIPBJetTags"))),
  fDisc3(iConfig.getUntrackedParameter<string>("discriminator3", string("softElectronBJetTags"))),
  fDisc4(iConfig.getUntrackedParameter<string>("discriminator4", string("trackCountingHighEffBJetTags"))),
  fDisc5(iConfig.getUntrackedParameter<string>("discriminator5", string("trackCountingHighPurBJetTags"))),
  fDisc6(iConfig.getUntrackedParameter<string>("discriminator6", string("jetProbabilityBJetTags"))),
  fDisc7(iConfig.getUntrackedParameter<string>("discriminator7", string("combinedSecondaryVertexBJetTags"))),
  fJetPartonMapAlgo(iConfig.getParameter<edm::InputTag>("JetPartonMapAlgo")),
  fJetPartonMapPhys(iConfig.getParameter<edm::InputTag>("JetPartonMapPhys")),
  fGenJetPartonMapAlgo(iConfig.getParameter<edm::InputTag>("GenJetPartonMapAlgo")),
  fGenJetPartonMapPhys(iConfig.getParameter<edm::InputTag>("GenJetPartonMapPhys"))
 
{
  
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpJets constructor" << endl;
  cout << "--- " << fJetsLabel.c_str() << endl;
  cout << "--- " << fGenJetsLabel.c_str() << endl;
  cout << "--- " << fDisc1.c_str() << endl;
  cout << "--- " << fDisc2.c_str() << endl;
  cout << "--- " << fDisc3.c_str() << endl;
  cout << "--- " << fDisc4.c_str() << endl;
  cout << "--- " << fDisc5.c_str() << endl;
  cout << "--- " << fDisc6.c_str() << endl;
  cout << "--- " << fDisc7.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
  
}


// ----------------------------------------------------------------------
HFDumpJets::~HFDumpJets() {
  
}


// ----------------------------------------------------------------------
void HFDumpJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // -- get the collection of RecoJets 
  edm::Handle<reco::CaloJetCollection> caloJets;
  iEvent.getByLabel( fJetsLabel.c_str(), caloJets );

  // -- get the collection of GenJets 
  edm::Handle<reco::GenJetCollection> genJets;
  iEvent.getByLabel( fGenJetsLabel.c_str(), genJets );


  // -- get the maps for flavor matching
  edm::Handle<reco::CandMatchMap>        theJetPartonMapAlgo;
  iEvent.getByLabel( fJetPartonMapAlgo,theJetPartonMapAlgo);
  edm::Handle<reco::CandMatchMap>        theJetPartonMapPhys;
  iEvent.getByLabel( fJetPartonMapPhys,theJetPartonMapPhys);
  edm::Handle<reco::CandMatchMap>        theGenJetPartonMapAlgo;
  iEvent.getByLabel( fGenJetPartonMapAlgo,theGenJetPartonMapAlgo);
  edm::Handle<reco::CandMatchMap>        theGenJetPartonMapPhys;
  iEvent.getByLabel( fGenJetPartonMapPhys,theGenJetPartonMapPhys);

  // -- get the b tagging jettags
  edm::Handle<reco::JetTagCollection> bTagHandle1;
  iEvent.getByLabel(fDisc1.c_str(), bTagHandle1);
  const reco::JetTagCollection & bTags1 = *(bTagHandle1.product());
  edm::Handle<reco::JetTagCollection> bTagHandle2;
  iEvent.getByLabel(fDisc2.c_str(), bTagHandle2);
  const reco::JetTagCollection & bTags2 = *(bTagHandle2.product());
  edm::Handle<reco::JetTagCollection> bTagHandle3;
  iEvent.getByLabel(fDisc3.c_str(), bTagHandle3);
  const reco::JetTagCollection & bTags3 = *(bTagHandle3.product());
  edm::Handle<reco::JetTagCollection> bTagHandle4;
  iEvent.getByLabel(fDisc4.c_str(), bTagHandle4);
  const reco::JetTagCollection & bTags4 = *(bTagHandle4.product());
  edm::Handle<reco::JetTagCollection> bTagHandle5;
  iEvent.getByLabel(fDisc5.c_str(), bTagHandle5);
  const reco::JetTagCollection & bTags5 = *(bTagHandle5.product());
  edm::Handle<reco::JetTagCollection> bTagHandle6;
  iEvent.getByLabel(fDisc6.c_str(), bTagHandle6);
  const reco::JetTagCollection & bTags6 = *(bTagHandle6.product());
  edm::Handle<reco::JetTagCollection> bTagHandle7;
  iEvent.getByLabel(fDisc7.c_str(), bTagHandle7);
  const reco::JetTagCollection & bTags7 = *(bTagHandle7.product());
  

  TAnaJet *pCaloJet;
  TAnaJet *pGenJet;
  
 //Loop over CaloJets 
 int jetIndex=0;
 for( reco::CaloJetCollection::const_iterator cal = caloJets->begin(); cal != caloJets->end(); ++ cal ) {

   pCaloJet = gHFEvent->addCaloJet();

   pCaloJet->fIndex = jetIndex;

   pCaloJet->fQ  = cal->charge();
   pCaloJet->fPlab.SetPtEtaPhi(cal->pt(),
				cal->eta(),
				cal->phi()
				);

   pCaloJet->fE  = cal->energy();
   pCaloJet->fEt = cal->et();
   pCaloJet->fM  = cal->mass();
   pCaloJet->fMt = cal->mt();

   pCaloJet->fEMEnergy         = cal->emEnergyFraction();
   pCaloJet->fHADEnergy        = cal->energyFractionHadronic();
   pCaloJet->finvisibleEnergy  = -1;

   pCaloJet->fn60  = cal->n60();
   pCaloJet->fn90  = cal->n90();

   pCaloJet->fJetFlavorAlgo = -9999;
   pCaloJet->fJetFlavorPhys = -9999;
   pCaloJet->fJetFlavorEne  = -9999;
   pCaloJet->fD1 = -9999;
   pCaloJet->fD2 = -9999;
   pCaloJet->fD4 = -9999;
   pCaloJet->fD5 = -9999;
   pCaloJet->fD6 = -9999;
   pCaloJet->fD7 = -9999;
   //JetFlavour: Algorithmic definition
   for( CandMatchMap::const_iterator f  = theJetPartonMapAlgo->begin(); f != theJetPartonMapAlgo->end(); f++) {
     const Candidate *theJet     = &*(f->key);
     const Candidate *theParton  = &*(f->val);
     if (cal->et()==theJet->et()) {
	pCaloJet->fJetFlavorAlgo = theParton->pdgId();
	break;
     }
   }
    //JetFlavour: Physics definition
    for( CandMatchMap::const_iterator f  = theJetPartonMapPhys->begin(); f != theJetPartonMapPhys->end(); f++) {
      const Candidate *theJet     = &*(f->key);
      const Candidate *theParton  = &*(f->val);
      if (cal->et()==theJet->et()) {
	pCaloJet->fJetFlavorPhys = theParton->pdgId();
	break;
      }
    }

//     //BTagging: discriminator 1
//     for (unsigned int i = 0; i != bTags1.size(); ++i) { 
//       if (cal->et() == bTags1[i].jet()->et()) {

// 	pCaloJet->fD1 = bTags1[i].discriminator();
//       }
//     }
//     //BTagging: discriminator 2
//     for (unsigned int i = 0; i != bTags2.size(); ++i) { 
//       if (cal->et() == bTags2[i].jet()->et())
// 	pCaloJet->fD2 = bTags2[i].discriminator(); 
//     }
//     //BTagging: discriminator 3
//     for (unsigned int i = 0; i != bTags3.size(); ++i) { 
//       if (cal->et() == bTags3[i].jet()->et())
// 	pCaloJet->fD3 = bTags3[i].discriminator(); 
//     }
//     //BTagging: discriminator 4 
//     for (unsigned int i = 0; i != bTags4.size(); ++i) { 
//       if (cal->et() == bTags4[i].jet()->et()) {
// 	pCaloJet->fD4 = bTags4[i].discriminator();
	
// 	//fill tracks belonging to jet
// 	const edm::RefVector<reco::TrackCollection> tc = bTags4[i].tracks();
// 	for (unsigned int j=0; j<tc.size(); j++) {
// 	  pCaloJet->addTrack(tc[j].index());
// 	}
//       }
//     }
//     //BTagging: discriminator 5 
//     for (unsigned int i = 0; i != bTags5.size(); ++i) { 
//       if (cal->et() == bTags5[i].jet()->et()) 
// 	pCaloJet->fD5 = bTags5[i].discriminator();
//     }
//     //BTagging: discriminator 6
//     for (unsigned int i = 0; i != bTags6.size(); ++i) { 
//       if (cal->et() == bTags6[i].jet()->et())
// 	pCaloJet->fD6 = bTags6[i].discriminator();
//     }
//     //BTagging: discriminator 7
//     for (unsigned int i = 0; i != bTags7.size(); ++i) { 
//       if (cal->et() == bTags7[i].jet()->et())
// 	pCaloJet->fD7 = bTags7[i].discriminator(); 
//     }
  
    if (jetIndex==0) cout << "===> Calo Jets " << endl;
    pCaloJet->dump();
    cout << "JetFlavor (algorithmic  physical): " << pCaloJet->fJetFlavorAlgo << " " << pCaloJet->fJetFlavorPhys << endl;
    cout << "discriminator: " << pCaloJet->fD1 << " " << pCaloJet->fD2 << " " << pCaloJet->fD3 << " " << pCaloJet->fD4 << " " << pCaloJet->fD5 << " " << pCaloJet->fD6 << " " << pCaloJet->fD7 << endl;
     
    jetIndex++;
  }

  //Loop over GenJets
  jetIndex=0; 
  for( reco::GenJetCollection::const_iterator gen = genJets->begin(); gen != genJets->end(); ++ gen ) { 

    pGenJet = gHFEvent->addGenJet();
 
    pGenJet->fIndex = jetIndex;

    pGenJet->fQ  = gen->charge();
    pGenJet->fPlab.SetPtEtaPhi(gen->pt(),
			       gen->eta(),
			       gen->phi()
			       );

    pGenJet->fE  = gen->energy();
    pGenJet->fEt = gen->et();
    pGenJet->fM  = gen->mass();
    pGenJet->fMt = gen->mt();

    pGenJet->fEMEnergy         = gen->emEnergy();
    pGenJet->fHADEnergy        = gen->hadEnergy();
    pGenJet->finvisibleEnergy  = gen->invisibleEnergy();

    pGenJet->fn60  = -1;
    pGenJet->fn90  = -1; 
    pGenJet->fJetFlavorAlgo = -9999;
    pGenJet->fJetFlavorPhys = -9999;
    pGenJet->fJetFlavorEne  = -9999;
    pGenJet->fD1 = -9999;
    pGenJet->fD2 = -9999;
    pGenJet->fD4 = -9999;
    pGenJet->fD5 = -9999;
    pGenJet->fD6 = -9999;
    pGenJet->fD7 = -9999;
    //JetFlavour: Algorithmic definition
    for( CandMatchMap::const_iterator f  = theGenJetPartonMapAlgo->begin(); f != theGenJetPartonMapAlgo->end(); f++) {
      const Candidate *theJet     = &*(f->key);
      const Candidate *theParton  = &*(f->val);
      if (gen->et()==theJet->et()) {
	pGenJet->fJetFlavorAlgo = theParton->pdgId();
	break;
      }
    }
    //JetFlavour: Physics definition
    for( CandMatchMap::const_iterator f  = theGenJetPartonMapPhys->begin(); f != theGenJetPartonMapPhys->end(); f++) {
      const Candidate *theJet     = &*(f->key);
      const Candidate *theParton  = &*(f->val);	
      if (gen->et()==theJet->et()) {
	pGenJet->fJetFlavorPhys = theParton->pdgId();
	break;
      }
    }

    if (jetIndex==0) cout << "===> Gen Jets " << endl;
    pGenJet->dump(); 
    cout << "JetFlavor (algorithmic  physical): " << pGenJet->fJetFlavorAlgo << " " << pGenJet->fJetFlavorPhys << endl;

    jetIndex++;
  } 
  

}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpJets::beginJob(const edm::EventSetup& setup) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpJets::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpJets);
