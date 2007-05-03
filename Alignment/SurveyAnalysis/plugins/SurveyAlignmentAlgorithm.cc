#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/SurveyAnalysis/interface/SurveyAlignmentSensor.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Alignment/SurveyAnalysis/plugins/SurveyAlignmentAlgorithm.h"

SurveyAlignmentAlgorithm::SurveyAlignmentAlgorithm(const edm::ParameterSet& cfg):
  AlignmentAlgorithmBase(cfg),
  theOutfile(cfg.getParameter<std::string>("outfile")),
  theIterations(cfg.getParameter<unsigned int>("nIteration")),
  theLevels(cfg.getParameter< std::vector<std::string> >("levels"))
{
}

void SurveyAlignmentAlgorithm::initialize(const edm::EventSetup&,
					  AlignableTracker*,
					  AlignableMuon*,
					  AlignmentParameterStore* store)
{
  const std::vector<Alignable*>& alignables = store->alignables();

  unsigned int nAlignable = alignables.size();

  std::vector<Alignable*> sensors(nAlignable, 0);

  for (unsigned int i = 0; i < nAlignable; ++i)
  {
    sensors[i] = alignables[i]->components().front();
  }

  AlignableObjectId dummy;

  std::vector<Alignable::AlignableObjectIdType> levels;

  for (unsigned int l = 0; l < theLevels.size(); ++l)
  {
    levels.push_back(dummy.nameToType(theLevels[l]));
  }

  SurveyAlignmentSensor align(sensors, levels);

  align.iterate(theIterations, theOutfile);
}

// Plug in to framework

#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentAlgorithmPluginFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AlignmentAlgorithmPluginFactory, SurveyAlignmentAlgorithm, "SurveyAlignmentAlgorithm");
