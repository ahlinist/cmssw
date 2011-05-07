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
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitTauDecayLikelihoodMC.h,v 1.1 2011/04/25 14:29:51 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitSingleParticleLikelihood.h"

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
  void beginCandidate(const NSVfitSingleParticleHypothesisBase*);
  
  double operator()(const NSVfitSingleParticleHypothesisBase*) const;

 private:
  int getDecayMode(const T*) const;

  enum { kElectron, kMuon, kOneProng0Pi0, kOneProngGt0Pi0s, kThreeProng0Pi0, kOther };
  enum { kPt, kEnergy };
  enum { kEtaPhi, kAngle };

  struct decayModeEntryType
  {
    RooAbsPdf* decayPdf_;
    RooRealVar* mom_;
    int momType_;
    RooRealVar* sepTimesMom_;
    int sepType_;
  };

  std::map<int, decayModeEntryType*> decayModeParameters_;

  decayModeEntryType* currentDecayModeParameter_;
};

#endif
