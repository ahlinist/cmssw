#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "EgammaAnalysis/EgammaEfficiencyProducers/interface/EmObjectProducer.h"
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagEmObjectAlgo.h"
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EcalTrackEmObjectAlgo.h"
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/McEmObjectAlgo.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObjectFwd.h"
#include <string>
#include <memory>

EmObjectProducer::EmObjectProducer(const edm::ParameterSet &params) : conf_(params)
{
   // Register product
   produces<EgEff::EmObjectCollection>();
}

void EmObjectProducer::beginJob(const edm::EventSetup &setup)
{
   // Load the required algorithm
   std::string algoName = conf_.getParameter<std::string>("AlgorithmName");
   if(algoName == "TagEmObjectAlgo")
   {
      std::auto_ptr<EmObjectAlgo> pt(new TagEmObjectAlgo());
      algo_ = pt;
   }
   else if(algoName == "EcalTrackEmObjectAlgo")
   {
      std::auto_ptr<EmObjectAlgo> pt(new EcalTrackEmObjectAlgo());
      algo_ = pt;
   }
   else if(algoName == "McEmObjectAlgo")
   {
      std::auto_ptr<EmObjectAlgo> pt(new McEmObjectAlgo());
      algo_ = pt;
   }

   // Initialise
   edm::ParameterSet algoParams = conf_.getParameter<edm::ParameterSet>("AlgoParams");
   algo_->initialise(algoParams);
}

void EmObjectProducer::produce(edm::Event &event, const edm::EventSetup &eventSetup)
{
   // Create the output collection
   std::auto_ptr<EgEff::EmObjectCollection> outCol(new EgEff::EmObjectCollection);

   // Run the algoritm and store in the event
   algo_->run(event, *outCol);
   event.put(outCol);
}
