#ifndef ttHAnalysis_TTHNuReco_h
#define ttHAnalysis_TTHNuReco_h

#include "DataFormats/Candidate/interface/Candidate.h"

// Utility class
// Reconstruct the transverse missing energy from the calorimetric towers
// and the longitudinal missing energy from the transverse component, the 
// W mass constraint and the lepton momentum

namespace tth {

  struct TTHNuReco {
    TTHNuReco(){}
    ~TTHNuReco(){}
    
    void momentum( float missEtX, float missEtY, float pxLep,float pyLep,
		   float pzLep, reco::Candidate::LorentzVector & pNuP, 
		   reco::Candidate::LorentzVector & pNuM );
    
  };
}
#endif
