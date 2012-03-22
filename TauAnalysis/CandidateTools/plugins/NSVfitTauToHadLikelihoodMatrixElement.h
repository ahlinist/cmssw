#ifndef TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodMatrixElement_h
#define TauAnalysis_CandidateTools_NSVfitTauToHadLikelihoodMatrixElement_h

/** \class NSVfitTauToHadLikelihoodMatrixElement
 *
 * Plugin for computing likelihood for tau lepton decay 
 *  tau- --> X nu
 * to be compatible with matrix element of V-A electroweak decay
 *
 * NOTE: the system of hadrons X can either be pi-, rho- --> pi- pi0,
 *       a1- --> pi- pi0 pi0 or a1- --> pi- pi+ pi-;
 *       tau decays into pi- pi+ pi- pi0 are **not** supported
 *
 * \author Christian Veelken, Lorenzo Bianchini; LLR
 *
 * \version $Revision: 1.3 $
 *
 * $Id: NSVfitTauToHadLikelihoodMatrixElement.h,v 1.3 2011/05/29 17:58:22 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include <TFile.h>
#include <TGraph.h>

class NSVfitTauToHadLikelihoodMatrixElement : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauToHadLikelihoodMatrixElement(const edm::ParameterSet&);
  ~NSVfitTauToHadLikelihoodMatrixElement();

  void beginJob(NSVfitAlgorithmBase*);

  double operator()(const NSVfitSingleParticleHypothesis*, int) const;

 private:
  bool applySinThetaFactor_; 

  edm::FileInPath inputFileName_;
  TFile* inputFile_;
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
