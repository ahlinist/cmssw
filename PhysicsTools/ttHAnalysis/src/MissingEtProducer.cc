#include "PhysicsTools/ttHAnalysis/interface/MissingEtProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/NtupleData.h"
#include "PhysicsTools/ttHAnalysis/interface/MissingEtVariables.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace tth;
using namespace edm;

MissingEtProducer::MissingEtProducer( const ParameterSet & p ) :
  source ( p.getParameter<string>( "src" ) ) { 
  produces<vector<MissingEtVariables> >();
}

MissingEtProducer::~MissingEtProducer() {
}

void MissingEtProducer::produce( Event& evt, const EventSetup& ) {
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

  auto_ptr<vector<MissingEtVariables> > metVars( new vector<MissingEtVariables>(  ) );

  MissingEtVariables met;
  met.mEtX = - ( h->TotalMuonX + h->TowerEtX );
  met.mEtY = - ( h->TotalMuonY + h->TowerEtY );

  metVars->push_back( met );
  evt.put( metVars );

}
