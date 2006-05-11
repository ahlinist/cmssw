#include "PhysicsTools/ttHAnalysis/interface/NeutrinoCandidateProducer.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/ttHAnalysis/interface/MissingEtVariables.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "PhysicsTools/ttHAnalysis/interface/TTHNuReco.h"

using namespace std;
using namespace tth;
using namespace edm;
using namespace reco;

NeutrinoCandidateProducer::NeutrinoCandidateProducer( const ParameterSet & p ) :
  sourceEl ( p.getParameter<string>( "srcEl" ) ), 
  sourceMet ( p.getParameter<string>( "srcMet" ) ) { 
  produces<Candidates>();
}

NeutrinoCandidateProducer::~NeutrinoCandidateProducer() {
}

void NeutrinoCandidateProducer::produce( Event& evt, const EventSetup& ) {
  Handle<Candidates> sigEl;
  Handle<vector<MissingEtVariables> > metVars;
  try {
    evt.getByLabel( sourceEl, sigEl );
    evt.getByLabel( sourceMet, metVars );
    auto_ptr<Candidates> neutrinos( new Candidates );
    if( (sigEl->size() > 0) &&(metVars->size() > 0) ) {
      const Candidate & eCand = ( *sigEl )[ 0 ];
      const MissingEtVariables & metVar = ( *metVars )[ 0 ];
    
      //neutrino reconstruction
      TTHNuReco * nuReconstructor = new TTHNuReco();
      float pxLep = eCand.px();
      float pyLep = eCand.py();
      float pzLep = eCand.pz();
      float metX = metVar.mEtX;
      float metY = metVar.mEtY;
      Candidate::LorentzVector pNuP( 0,0,0,0 );
      Candidate::LorentzVector pNuM( 0,0,0,0 );
      nuReconstructor->momentum( metX, metY, pxLep, pyLep, pzLep, pNuP, pNuM );
      Candidate::Charge q = 0;
      neutrinos->push_back( new LeafCandidate( q, pNuP ) ); 
      neutrinos->push_back( new LeafCandidate( q, pNuM ) ); 
      
      delete nuReconstructor;
    }

    evt.put( neutrinos );

  } catch ( exception e ) {
    cerr << ">>> can't access " << sourceEl << ": " << e.what() << endl;
  }
  
}
