#ifndef TauAnalysis_CandidateTools_NSVfitResonanceBuilderW_h
#define TauAnalysis_CandidateTools_NSVfitResonanceBuilderW_h

/** \class NSVfitResonanceBuilderW
 *
 * Auxiliary class for building SVfitResonanceHypothesis objects
 * representing W -> e nu and W -> mu nu decays
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: NSVfitResonanceBuilderW.h,v 1.2 2012/03/14 16:34:43 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleBuilderBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

class NSVfitAlgorithmBase;

class NSVfitResonanceBuilderW : public NSVfitResonanceBuilderBase
{
 public:
  NSVfitResonanceBuilderW(const edm::ParameterSet&); 
  ~NSVfitResonanceBuilderW() {}

  virtual void beginJob(NSVfitAlgorithmBase*);

  virtual NSVfitResonanceHypothesis* build(const inputParticleMap&) const;

  virtual void applyFitParameter(NSVfitResonanceHypothesis*, const double*) const;	

  virtual void print(std::ostream&) const;

 private:
  std::string prodChargedLeptonLabel_;
  std::string prodNeutrinoLabel_;  

  int idxFitParameter_theta_lab_;
  int idxFitParameter_phi_lab_;
  int idxFitParameter_mass_;    

  double W_mass_;
  double W_width_;
};

#endif


