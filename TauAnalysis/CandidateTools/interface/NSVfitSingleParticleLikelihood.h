#ifndef TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h
#define TauAnalysis_CandidateTools_NSVfitSingleParticleLikelihood_h

/** \class NSVfitSingleParticleLikelihoodBase
 *
 * Abstract base-class for plugins computing likelihood of single particle kinematics;
 * used by nSVfit algorithm
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.13 $
 *
 * $Id: NSVfitSingleParticleLikelihood.h,v 1.13 2012/03/14 16:34:43 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitLikelihoodBase.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TF1.h>
#include <TFormula.h>
#include <TString.h>

#include <string>
#include <iostream>

class NSVfitSingleParticleLikelihood : public NSVfitLikelihoodBase
{
 public:
  NSVfitSingleParticleLikelihood(const edm::ParameterSet& cfg)
    : NSVfitLikelihoodBase(cfg),
      prodParticleLabel_(cfg.getParameter<std::string>("prodParticleLabel")),
      applyVisPtCutCorrection_(false),
      visPtCutThreshold_(0.)
  {
    if ( cfg.exists("applyVisPtCutCorrection") ) {
      applyVisPtCutCorrection_ = cfg.getParameter<bool>("applyVisPtCutCorrection");
      if ( applyVisPtCutCorrection_ ) {
	visPtCutThreshold_ = cfg.getParameter<double>("visPtCutThreshold");
	if ( visPtCutThreshold_ < 0. ) visPtCutThreshold_ = 0.;
      }
    }
  }

  virtual ~NSVfitSingleParticleLikelihood() {}

  virtual void beginCandidate(const NSVfitSingleParticleHypothesis*) {}

  virtual double operator()(const NSVfitSingleParticleHypothesis*, int) const = 0;

 protected:
  std::string prodParticleLabel_;

  bool applyVisPtCutCorrection_;
  double visPtCutThreshold_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<NSVfitSingleParticleLikelihood* (const edm::ParameterSet&)> NSVfitSingleParticleLikelihoodPluginFactory;

#endif
