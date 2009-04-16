#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <Math/VectorUtil.h>
#include <TMath.h>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpTrackJets.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/JetReco/interface/BasicJetCollection.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/getRef.h"


// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;





// ----------------------------------------------------------------------
HFDumpTrackJets::HFDumpTrackJets(const edm::ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fJetsLabel(iConfig.getUntrackedParameter<string>("jetsLabel", string("ic5TrackJets"))),
  fTracksLabel(iConfig.getUntrackedParameter<string>("tracksLabel", string("trackCandidates")))
  
 
{
  
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpTrackJets constructor" << endl;
  cout << "--- " << fJetsLabel.c_str() << endl;
  cout << "----------------------------------------------------------------------" << endl;
  
}


// ----------------------------------------------------------------------
HFDumpTrackJets::~HFDumpTrackJets() {
  
}


// ----------------------------------------------------------------------
void HFDumpTrackJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  
  nevt++;

  // handle to 0.5 cone ctf track jets  
  Handle<BasicJetCollection> jetsH;
  iEvent.getByLabel(fJetsLabel.c_str(),jetsH);
  const BasicJetCollection *jets   = jetsH.product(); 

  //tracks (jet constituents)
  Handle<reco::CandidateView> candidates1Handle;
  iEvent.getByLabel(fTracksLabel.c_str(), candidates1Handle); 

  std::vector<const Candidate *> cands;
  cands.clear();
  vector<const Candidate *>::const_iterator found = cands.begin();
 

  //get tracks 
  for (unsigned int i = 0; i < candidates1Handle->size(); ++ i ) {
    const Candidate &p = (*candidates1Handle)[i];
    const Candidate *  p1 = &p;
    cands.push_back( & * p1 );
  }

  int jetIndex=0; 
  TAnaJet *pTrackJet;
  
  for(BasicJetCollection::const_iterator it= jets->begin();it != jets->end(); it++){
    
    pTrackJet = gHFEvent->addTrackJet();
    pTrackJet->fIndex            = jetIndex;  

    pTrackJet->fPlab.SetPtEtaPhi(it->pt(),
				it->eta(),
				it->phi()
				);
 
    pTrackJet->fQ                = it->charge();
    pTrackJet->fE                = it->energy();
    pTrackJet->fEt               = it->et();
    pTrackJet->fM                = it->mass();
    pTrackJet->fMt               = it->mt();

    pTrackJet->fEMEnergy         = -1;//not used
    pTrackJet->fHADEnergy        = -1;//not used
    pTrackJet->finvisibleEnergy  = -1;//not used

    pTrackJet->fn60              = -9999;//not used
    pTrackJet->fn90              = -9999;//not used
  
    pTrackJet->fJetFlavorHeavy   = -9999;//not used
    pTrackJet->fJetFlavorNear2   = -9999;//not used
    pTrackJet->fJetFlavorNear3   = -9999;//not used 
    pTrackJet->fJetFlavorAlgo    = -9999;//not used
    pTrackJet->fJetFlavorPhys    = -9999;//not used
    pTrackJet->fJetFlavorEne     = -9999;//not used
  
    pTrackJet->fD1               = -9999;//not used
    pTrackJet->fD2               = -9999;//not used
    pTrackJet->fD4               = -9999;//not used
    pTrackJet->fD5               = -9999;//not used
    pTrackJet->fD6               = -9999;//not used
    pTrackJet->fD7               = -9999;//not used  


    //jet constituents
    std::vector< const reco::Candidate * > Constituent = it->getJetConstituentsQuick();
    for (unsigned int i=0; i< Constituent.size(); i++) {

      int index  = -1;
      const reco::Candidate * consti = Constituent[i];
      if (consti) {
	found = find( cands.begin(), cands.end(), consti );
	if ( found != cands.end() ) index = found - cands.begin() ;
	pTrackJet->addTrack(index);
	//if (fVerbose) cout << index << " pt " << consti->pt() << " phi " << consti->phi() << " eta " << consti->eta() << endl;
      } 
 
    }

    if (fVerbose > 0) pTrackJet->dump();
    jetIndex++;
  }

 



}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpTrackJets::beginJob(const edm::EventSetup& setup) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpTrackJets::endJob() { 
  cout << "HFDumpJet>    Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpTrackJets);
