#ifndef TauAnalysis_CandidateTools_CompositePtrCandidateTMEtAlgorithm_h
#define TauAnalysis_CandidateTools_CompositePtrCandidateTMEtAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/Utilities/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 

#include <TMath.h>

template<typename T>
class CompositePtrCandidateTMEtAlgorithm 
{
  typedef edm::Ptr<T> TPtr;

 public:

  CompositePtrCandidateTMEtAlgorithm(const edm::ParameterSet& cfg)
  {
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
  }
  ~CompositePtrCandidateTMEtAlgorithm() {}

  CompositePtrCandidateTMEt<T> buildCompositePtrCandidate(const TPtr visDecayProducts, 
							  const reco::CandidatePtr met)
  {
    CompositePtrCandidateTMEt<T> compositePtrCandidate(visDecayProducts, met);
  
    if ( visDecayProducts.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to visible Decay products invalid !!";
      return compositePtrCandidate;
    }

    if ( met.isNull() ) {
      edm::LogError ("CompositePtrCandidateTMEtAlgorithm") << " Pointer to missing transverse momentum invalid !!";
      return compositePtrCandidate;
    }

    compositePtrCandidate.setCharge(visDecayProducts->charge());
    compositePtrCandidate.setMt(compMt(visDecayProducts->p4(), met->px(), met->py()));
    compositePtrCandidate.setDPhi(TMath::Abs(normalizedPhi(visDecayProducts->phi() - met->phi())));
    
    return compositePtrCandidate;
  }

 private: 

  double compMt(const reco::Candidate::LorentzVector& visParticle, 
		double metPx, double metPy)
  {
    double px = visParticle.px() + metPx;
    double py = visParticle.py() + metPy;
    double et = visParticle.Et() + TMath::Sqrt(metPx*metPx + metPy*metPy);
    double mt2 = et*et - (px*px + py*py);
    if ( mt2 < 0 ) {
      edm::LogWarning ("compMt") << " mt2 = " << mt2 << " must not be negative !!";
      return 0.;
    }
    return TMath::Sqrt(mt2);
  }

  int verbosity_;
};

#endif 

