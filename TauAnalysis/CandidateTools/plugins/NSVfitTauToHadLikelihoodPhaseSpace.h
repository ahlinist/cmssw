#ifndef TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodPhaseSpace_h
#define TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodPhaseSpace_h

/** \class NSVfitTauToLeptonLikelihoodPhaseSpace
 *
 * Plugin to compute likelihood for system of hadrons to be compatible 
 * with tau --> tau-jet + nu two-body decay,
 * assuming constant matrix element, so that energy and angular distribution 
 * of decay products are solely determined by phase-space
 * 
 * \author Evan Friis, Christian Veelken; UC Davis
 *
 * \version $Revision: 1.7 $
 *
 * $Id: NSVfitTauToHadLikelihoodPhaseSpace.h,v 1.7 2012/03/14 16:34:47 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include <TFile.h>
#include <TGraph.h>

class NSVfitTauToHadLikelihoodPhaseSpace : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauToHadLikelihoodPhaseSpace(const edm::ParameterSet&);
  ~NSVfitTauToHadLikelihoodPhaseSpace();

  void beginJob(NSVfitAlgorithmBase*);

  double operator()(const NSVfitSingleParticleHypothesis*, int) const;

 private:
  bool applySinThetaFactor_; 
  edm::FileInPath VMshapeFileName_;
  TFile* VMshapeFile_;
  TGraph* rhoLPlus_;
  TGraph* rhoNormLPlus_; 
  TGraph* rhoLMinus_;
  TGraph* rhoNormLMinus_; 
  TGraph* rhoTPlus_;
  TGraph* rhoNormTPlus_;
  TGraph* rhoTMinus_;
  TGraph* rhoNormTMinus_;
};

#endif
