#include "PhysicsTools/ttHAnalysis/interface/QJetDiscriminator.h"
#include "PhysicsTools/ttHAnalysis/interface/JetVariables.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

QJetDiscriminator::QJetDiscriminator( const ParameterSet & p ) :
  source ( p.getParameter<string>( "src" ) ) { 
  produces<Candidates>();
}

QJetDiscriminator::~QJetDiscriminator() {
}

void QJetDiscriminator::produce( Event& evt, const EventSetup& ) {
  //  const float ptCut = 30.0;
  const float ecalFractionMax = 0.9;
  const float bDiscriminatorCut = 0.5;
  Handle<Candidates> allJets;
  Handle<vector<JetVariables> > jetVars;
  try {
    evt.getByLabel( source, allJets );
    evt.getByLabel( source, jetVars );
    int size = allJets->size();
    assert( size == int (jetVars->size()) );
    auto_ptr<Candidates> qJets( new Candidates );
    for( int i = 0; i < size; ++i ) {
      const Candidate & jetCand = ( *allJets )[ i ];
      const JetVariables & jetVar = ( *jetVars )[ i ];
      if( jetVar.ecalFraction > ecalFractionMax ) continue;
      if( jetVar.jetFlag == -1 ) continue;
      //      if( jetCand->pt() < ptCut ) continue;
      if( jetVar.bDiscriminator <= bDiscriminatorCut ) {
	qJets->push_back( jetCand.clone() ); 
      }
    }
    evt.put( qJets );
  } catch ( exception e ) {
    cerr << ">>> can't access " << source << ": " << e.what() << endl;
  }
  
}
