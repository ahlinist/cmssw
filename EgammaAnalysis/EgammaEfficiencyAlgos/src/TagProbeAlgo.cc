#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagProbeAlgo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

void TagProbeAlgo::initialise(const edm::ParameterSet &params)
{
   tagProducer_ = params.getParameter<std::string>("TagProducer");
   probeProducer_ = params.getParameter<std::string>("ProbeProducer");
   massCutMin_ = params.getParameter<double>("MassCutMin");
   massCutMax_ = params.getParameter<double>("MassCutMax");
   singleProbe_ = params.getParameter<bool>("SingleProbe");
}

void TagProbeAlgo::run(const edm::Event &event, EgEff::TagProbeAssociationCollection &tagProbeCol)
{
   // Get input collections
   edm::Handle<EgEff::EmObjectCollection> tagCollection;
   edm::Handle<EgEff::EmObjectCollection> probeCollection;

   try
   {
      event.getByLabel(tagProducer_, tagCollection);
   }
   catch(cms::Exception &ex)
   {
      edm::LogError("TagProbeAlgo") << "Error! Can't get collection " << tagProducer_;
      throw ex;
   }
   try
   {
      event.getByLabel(probeProducer_, probeCollection);
   }
   catch(cms::Exception &ex)
   {
      edm::LogError("TagProbeAlgo") << "Error! Can't get collection " << probeProducer_;
      throw ex;
   }

   // Iterate over all combinations of tags + probes, identifying candidates
   std::vector<int> probeIds;
   for(unsigned int i = 0; i < tagCollection->size(); ++i)
   {
      // Get tag object EDM ref
      const EgEff::EmObjectRef tag = edm::Ref<EgEff::EmObjectCollection>(tagCollection, i);
      for(unsigned int j = 0; j < probeCollection->size(); ++j)
      {
         // Get probe object EDM ref
         const EgEff::EmObjectRef probe = edm::Ref<EgEff::EmObjectCollection>(probeCollection, j);
         double invMass = invariantMass(tag, probe);
         if((invMass > massCutMin_) && (invMass < massCutMax_))
         {
            probeIds.push_back(j);
         }
      }
      // Add all probes depending on conditions
      if(singleProbe_ && (probeIds.size() == 1))
      {
         const EgEff::EmObjectRef probe = edm::Ref<EgEff::EmObjectCollection>(probeCollection, 0);
      }
      else if(!singleProbe_)
      {
         for(unsigned int k = 0; k < probeIds.size(); ++k)
         {
            const EgEff::EmObjectRef probe = edm::Ref<EgEff::EmObjectCollection>(probeCollection, k);
            tagProbeCol.insert(tag, probe);
         }
      }
   }
}

double TagProbeAlgo::invariantMass(const EgEff::EmObjectRef &obj1, const EgEff::EmObjectRef &obj2)
{
   double retVal = pow(obj1->p() + obj2->p(), 2);
   retVal -= pow(obj1->px() + obj2->px(), 2);
   retVal -= pow(obj1->py() + obj2->py(), 2);
   retVal -= pow(obj1->pz() + obj2->pz(), 2);
   return sqrt(retVal);
}

