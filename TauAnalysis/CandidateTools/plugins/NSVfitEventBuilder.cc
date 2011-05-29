#include "TauAnalysis/CandidateTools/plugins/NSVfitEventBuilder.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include <TMath.h>

void NSVfitEventBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  NSVfitEventBuilderBase::beginJob(algorithm);
}

void setFitParameterStepSize(NSVfitAlgorithmBase* algorithm, int fitParameterIdx, double stepSize)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = algorithm->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setStepSize(stepSize);
  }
}

NSVfitEventHypothesis* NSVfitEventBuilder::build(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  NSVfitEventHypothesis* event = NSVfitEventBuilderBase::build(inputParticles, eventVertex);

//--- set fitParameter step-size according to estimated uncertainty on vertex position
  double pvPositionXerr, pvPositionYerr, pvPositionZerr;
  if ( event->eventVertexSVrefittedIsValid() ) {
    pvPositionXerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(0));
    pvPositionYerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(1));
    pvPositionZerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(2));
  } else {
    pvPositionXerr = 0.0050; // 50 microns
    pvPositionYerr = 0.0050; // 50 microns
    pvPositionZerr = 0.1;    //  1 mm
  }

  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftX_, 0.25*pvPositionXerr);
  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftY_, 0.25*pvPositionYerr);
  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftZ_, 0.25*pvPositionZerr);
  
  return event;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventBuilderPluginFactory, NSVfitEventBuilder, "NSVfitEventBuilder");
