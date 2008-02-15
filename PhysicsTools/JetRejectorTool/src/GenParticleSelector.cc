//#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/JetRejectorTool/interface/GenParticleSelector.h"

GenParticleSelector::GenParticleSelector( const edm::ParameterSet & iConfig) {
  selgenjetsrc_ = iConfig.getParameter<edm::InputTag>( "selgenjetsrc" );
 switchSignalDefinition = iConfig.getParameter< unsigned int > ("signalDefinition");
  produces<CandidateCollection>();
}

GenParticleSelector::~GenParticleSelector() {
}

void GenParticleSelector::produce( Event& evt, const EventSetup& ) {
  Handle<CandidateCollection> particles;
  evt.getByLabel( "genParticleCandidates", particles );

  edm::Handle<CandidateCollection> genjet;
  evt.getByLabel (selgenjetsrc_ ,genjet);
  // Handle<vector<reco::GenJet> > genjets;
  // if (switchSignalDefinition == 2) evt.getByLabel( selgenjetsrc_, genjets); 
  
  auto_ptr<CandidateCollection> cands( new CandidateCollection );
  cands->reserve( particles->size() );

  size_t idx = 0;
  if (switchSignalDefinition == 1) {
    for( CandidateCollection::const_iterator p = particles->begin();
	 p != particles->end(); ++ p, ++ idx ) {
      //    if (idx < 6 ) continue ;
      int status = p->status();
      if ( status == 3 ) {
	if (switchSignalDefinition == 1) {
	  int id = abs( p->pdgId() );
	  if( id < 6 &&    // id == 21 || id == 22 
	      p->mother()->pdgId() != 2212 && id != abs(p->mother()->pdgId())    ) {
	    //cout << "Parton Status:" << status << " Id:" << id << endl;
	    CandidateBaseRef ref( CandidateRef( particles, idx ) );
	    cands->push_back( new ShallowCloneCandidate( ref ) );
	  }
	} 
      }
    }
  }
  /*  else if(switchSignalDefinition == 2) {
    //   vector<reco::GenJet> genjet;
    // typedef ShallowCloneProducer<reco::GenJetCollection> GenJetShallowCloneProducer;
    //    CandidateCollection genjet fare il cast di reco::GenJet;
    for(CandidateCollection::const_iterator i = genjet->begin();
	 i != particles->end(); ++ i, ++idx) {
      // const reco::Candidate & cand = genjet[i];
      // candColl->push_back(& cand );
      CandidateBaseRef ref( CandidateRef( genjet, idx ) );
      cands->push_back( new ShallowCloneCandidate( ref ) );
    }    
    //    } else if (switchSignalDefinition == 3) {  // to be uncommented when available (CMSSW > 1_5_0)
    //      genjet= (*partonjets);  
  } else {
    cout << " !!ERROR!! no such option!" << endl;
    return;
    }*/
  evt.put( cands );
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GenParticleSelector);
