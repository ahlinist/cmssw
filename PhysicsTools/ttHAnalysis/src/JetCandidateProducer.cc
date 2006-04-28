#include "PhysicsTools/ttHAnalysis/interface/JetCandidateProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/NtupleData.h"
#include "PhysicsTools/ttHAnalysis/interface/JetVariables.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

JetCandidateProducer::JetCandidateProducer( const ParameterSet & p ) { 
  produces<Candidates>();
  produces<vector<JetVariables> >();
}

JetCandidateProducer::~JetCandidateProducer() {
}

void JetCandidateProducer::produce( Event& evt, const EventSetup& ) {
  Handle<NtupleData> h;
  try {
    evt.getByLabel( source, h );
  } catch ( ... ) {
    cerr << ">>> getByLabel failed with source: " << source 
	 << ". Try to recover using getManyByType." << endl;
    vector<Handle<NtupleData> > vv;
    evt.getManyByType( vv ); 
    std::cerr << ">>> getManyByType selected: " << vv.size() << std::endl;
    for( std::vector<Handle<NtupleData> >::const_iterator
	   j = vv.begin(); j != vv.end(); ++j ) {
      const edm::Provenance * prov = j->provenance();
      std::cerr << ">>> provenance: " << prov->product.module.moduleLabel_ << std::endl;
    }
    h = vv.front();
    assert( h.isValid() );
    std::cerr << ">>> getManyByType succeeded: recovering." << std::endl;
  }
  auto_ptr<Candidates> jets( new Candidates );
  auto_ptr<vector<JetVariables> > jetVars( new vector<JetVariables>( h->NumbJet ) );
  for( int i = 0; i < h->NumbJet; ++i ) {
    float px = h->JetEt[i] * cos(h->JetPhi[i]);
    float py = h->JetEt[i] * sin(h->JetPhi[i]);
    float pz = h->JetE[i]  * cos(h->JetTheta[i]);
    
    Candidate::LorentzVector p( px, py, pz, h->JetE[i] );
    Candidate::Charge q = 0;    
    jets->push_back( new LeafCandidate( q, p ) ); 
    JetVariables & jv = (*jetVars)[ i ];
    jv.ecalFraction = h->JetEcalFraction[i];
    jv.jetFlag = h->BJetFlag[i];
    jv.bDiscriminator = h->BDiscriminator[i];
  }
  evt.put( jets );
  evt.put( jetVars );
} 
 
