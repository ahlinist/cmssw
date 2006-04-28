#include "PhysicsTools/ttHAnalysis/interface/SignalElectronSelector.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

SignalElectronSelector::SignalElectronSelector( const ParameterSet & p ) :
  source ( p.getParameter<string>( "src" ) ) { 
  produces<Candidates>();
}

SignalElectronSelector::~SignalElectronSelector() {
}

void SignalElectronSelector::produce( Event& evt, const EventSetup& ) {
  Handle<Candidates> goodEl;
  try {
    evt.getByLabel( source, goodEl );
    int size = goodEl->size();
    auto_ptr<Candidates> signalElectrons( new Candidates );

    float ptMax = 0;
    int sigElInd = -1;
    for( int i = 0; i < size; ++i ) {
      const Candidate & tempCand = ( *goodEl )[ i ];
      if( tempCand.pt() > ptMax ) {
	ptMax = tempCand.pt();
	sigElInd = i;
      }
    }
    if( sigElInd > -1 )
      signalElectrons->push_back( ( *goodEl )[ sigElInd ].clone() ); 
    evt.put( signalElectrons );

  } catch ( exception e ) {
    cerr << ">>> can't access " << source << ": " << e.what() << endl;
  }
  
}
