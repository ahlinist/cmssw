#ifndef TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodMC_h
#define TauAnalysis_CandidateTools_NSVfitTauDecayLikelihoodMC_h

/** \class NSVfitTauDecayLikelihoodMC
 *
 * Plugin for computing likelihood for tau lepton to decay
 * with given opening angle between visible and "invisible" (neutrinos) decay products.
 *
 * The parametrization of the decay angles is described in the (MMC) paper
 *   "A New Mass Reconstruction Technique for Resonances Decaying to tau tau",
 *   by A. Elagin, P.Murat, A.Pranko and A. Safonov
 * ( http://arxiv.org/pdf/1012.4686 )
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitTauDecayLikelihoodMC.h,v 1.4 2011/05/29 17:58:22 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitCachingPdfWrapper.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

#include "RooAbsPdf.h"
#include "RooRealVar.h"

template<typename T>
class NSVfitTauDecayLikelihoodMC : public NSVfitSingleParticleLikelihood
{
 public:
  NSVfitTauDecayLikelihoodMC(const edm::ParameterSet&);
  ~NSVfitTauDecayLikelihoodMC();

  void beginJob(NSVfitAlgorithmBase*);
  void beginCandidate(const NSVfitSingleParticleHypothesis*);
  
  double operator()(const NSVfitSingleParticleHypothesis*, int) const;

 private:
  int getDecayMode(const T*) const;

  enum { kElectron, kMuon, kOneProng0Pi0, kOneProngGt0Pi0s, kThreeProng0Pi0, kOther };
  enum { kPt, kEnergy };
  enum { kEtaPhi, kAngle };

  struct decayModeEntryType
  {
    NSVfitCachingPdfWrapper decayPdf_;
    int momType_;
    int sepType_;
  };

  std::map<int, decayModeEntryType*> decayModeParameters_;

  decayModeEntryType* currentDecayModeParameter_;
};

#endif
