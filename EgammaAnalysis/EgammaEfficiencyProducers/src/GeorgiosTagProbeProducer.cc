#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "EgammaAnalysis/EgammaEfficiencyProducers/interface/GeorgiosTagProbeProducer.h"
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagProbeAlgo.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
#include <string>
#include <memory>

GeorgiosTagProbeProducer::GeorgiosTagProbeProducer(const edm::ParameterSet &params) : conf_(params)
{
   // Register product
   produces<EgEff::TagProbeAssociationCollection>();
}

void GeorgiosTagProbeProducer::beginJob(const edm::EventSetup &setup)
{
   algo_.initialise(conf_);
}

void GeorgiosTagProbeProducer::produce(edm::Event &event, const edm::EventSetup &eventSetup)
{
   // Create the output collection
   std::auto_ptr<EgEff::TagProbeAssociationCollection> outCol(new EgEff::TagProbeAssociationCollection);

   // Run the algoritm and store in the event
   algo_.run(event, *outCol);
   event.put(outCol);
}
