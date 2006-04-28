#include "PhysicsTools/ttHAnalysis/interface/ElectronSelector.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronVariables.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

ElectronSelector::ElectronSelector( const ParameterSet & p ) :
  source ( p.getParameter<string>( "src" ) ) { 
  produces<Candidates>();
}

ElectronSelector::~ElectronSelector() {
}

void ElectronSelector::produce( Event& evt, const EventSetup& ) {
  const float ptCut = 10.0;
  const float ptIsoMax = 5.0;
  Handle<Candidates> inputElectrons;
  Handle<vector<ElectronVariables> > elVars;
  try {
    evt.getByLabel( source, inputElectrons );
    evt.getByLabel( source, elVars );
    int size = inputElectrons->size();
    assert( size == int (elVars->size()) );
    auto_ptr<Candidates> outputElectrons( new Candidates );
    for( int i = 0; i < size; ++i ) {
      const Candidate & eCand = ( *inputElectrons )[ i ];
      const ElectronVariables & elVar = ( *elVars )[ i ];
      //      cout << "electron pt = " << eCand->pt() << endl;
      //      cout << "electron iso = " << elVar.ptIso << endl;
      if( eCand.pt() > ptCut )
	if( elVar.ptIso < ptIsoMax )
	  outputElectrons->push_back( eCand.clone() ); 
    }
    evt.put( outputElectrons );
  } catch ( exception e ) {
    cerr << ">>> can't access " << source << ": " << e.what() << endl;
  }
  
}
