#ifndef RecoBTag_JetFilter
#define RecoBTag_JetFilter

#include "DataFormats/Candidate/interface/Particle.h"

class JetFilter {
  /**
   *  Class that determines if jet is being considered for btagging.
   */

   public:
     JetFilter ( float eta_min=0., float eta_max=2.5, float pt_min=10., float p_min=15. );

     /**
      *  Returns true for an acceptable jet,
      *  false for a veto!
      */
     bool operator() ( const reco::Particle & ) const;
   private:
     float etaMin_;
     float etaMax_;
     float pTMin_;
     float pMin_;
};

#endif
