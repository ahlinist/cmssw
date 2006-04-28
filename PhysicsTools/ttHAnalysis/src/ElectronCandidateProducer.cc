#include "PhysicsTools/ttHAnalysis/interface/ElectronCandidateProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/NtupleData.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronVariables.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

ElectronCandidateProducer::ElectronCandidateProducer( const ParameterSet & p ) :
  source ( p.getParameter<string>( "src" ) ) { 
  produces<Candidates>();
  produces<vector<ElectronVariables> >();
}

ElectronCandidateProducer::~ElectronCandidateProducer() {
}

void ElectronCandidateProducer::produce( Event& evt, const EventSetup& ) {
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
  auto_ptr<Candidates> electrons( new Candidates );
  auto_ptr<vector<ElectronVariables> > elVars( new vector<ElectronVariables>( h->NumbLep ) );
  const float me = 0.000511;
  for( int i = 0; i < h->NumbLep; ++i ) {
    float px = h->LeptonPx[ i ];
    float py = h->LeptonPy[ i ];
    float pz = h->LeptonPz[ i ];
    float en = sqrt(px * px + py * py + pz * pz + me * me);
    Candidate::LorentzVector p( px, py, pz, en );
    Candidate::Charge q = + 1;
    electrons->push_back( new LeafCandidate( q, p ) ); 
    ElectronVariables & ev = (*elVars)[ i ];
    //    cout << "LeptonPtIso["<<i<<"]= " << h->LeptonPtIso[i] << endl;
    //    cout << "LeptonPx["<<i<<"]= " << h->LeptonPx[i] << endl;
    ev.ptIso = h->LeptonPtIso[i];
  }
  evt.put( electrons );
  evt.put( elVars );
}
