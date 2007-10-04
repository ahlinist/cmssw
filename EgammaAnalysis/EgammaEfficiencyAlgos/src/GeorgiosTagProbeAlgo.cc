
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/GeorgiosTagProbeAlgo.h"

// framework includes
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"

// root includes
#include "Math/GenVector/VectorUtil.h"

void GeorgiosTagProbeAlgo::initialise(const edm::ParameterSet &params)
{

   std::cout << "GeorgiosTagProbeAlgo::initialise" << std::endl;
   tagProducer_ = params.getParameter<edm::InputTag>("tagProducer");
   probeProducer_ = params.getParameter<edm::InputTag>("probeProducer");

   massCutMin_ = params.getParameter<double>("massCutMin");
   massCutMax_ = params.getParameter<double>("massCutMax");
   requireOS_ = params.getParameter<bool>("requireOS");
   deltaPVCut_ = params.getParameter<double>("deltaPVCut");

   requireTagTrigger_ = params.getParameter<bool>("requireTagTrigger");
   triggerProducer_ = params.getParameter<edm::InputTag>("triggerProducer");

   std::cout << "GeorgiosTagProbeAlgo::initialise done" << std::endl;

}

void GeorgiosTagProbeAlgo::run(const edm::Event &event, EgEff::TagProbeAssociationCollection &tagProbeCol)
{
   // Get input collections
   edm::Handle<EgEff::EmObjectCollection> tagHandle;
   edm::Handle<EgEff::EmObjectCollection> probeHandle;
   const EgEff::EmObjectCollection *tagCollection = 0;
   const EgEff::EmObjectCollection *probeCollection = 0;

   // for the trigger
   edm::Handle<reco::HLTFilterObjectWithRefs> filterObjHandle;
   const reco::HLTFilterObjectWithRefs *filterObj = 0;
   if (requireTagTrigger_) {
     edm::Handle<reco::HLTFilterObjectWithRefs> filterObjHandle;
     event.getByLabel(triggerProducer_, filterObjHandle);
     filterObj = filterObjHandle.product();
   }

   try {
      event.getByLabel(tagProducer_, tagHandle);
      tagCollection = tagHandle.product();
   } 
   catch(cms::Exception &ex) {
      edm::LogError("GeorgiosTagProbeAlgo") 
        << "Error! Can't get collection " << tagProducer_;
      throw ex;
   }

   try {
      event.getByLabel(probeProducer_, probeHandle);
      probeCollection = probeHandle.product();
   }
   catch(cms::Exception &ex) {
      edm::LogError("GeorgiosTagProbeAlgo") 
        << "Error! Can't get collection " << probeProducer_;
      throw ex;
   }

   // Iterate over all combinations of tags + probes, identifying candidates
   for(unsigned int i = 0; i < tagCollection->size(); ++i)
   {

      // Get tag object EDM ref
      const EgEff::EmObjectRef tag = edm::Ref<EgEff::EmObjectCollection>(tagHandle, i);

      // if needed check if tag passed the trigger
      bool tagMatchHLT = true;
      if (requireTagTrigger_)
      {
         tagMatchHLT = false;
         unsigned int k = 0;
         while((k < filterObj->size()) && !tagMatchHLT)
         {
             tagMatchHLT = triggerPass(filterObj->at(k), tag);
             k++;
         }
      }

      if (tagMatchHLT)
      {
      for(unsigned int j = 0; j < probeCollection->size(); ++j)
      {

         // Get probe object EDM ref
         const EgEff::EmObjectRef probe = 
            edm::Ref<EgEff::EmObjectCollection>(probeHandle, j);

         bool goodCombination = true;

         // compute tag-probe invariant mass
         double invMass = ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probe->p4());
         if ((invMass < massCutMin_) || (invMass > massCutMax_)) goodCombination = false;

         // compute tag-probe delta z vertex
         double deltaPV = 0;
         if (tag->hasTrack() && probe->hasTrack()) {
            deltaPV = fabs(tag->genericTrack()->vz() - probe->genericTrack()->vz());
         }
         if (deltaPV > deltaPVCut_) goodCombination = false;

         // compute product of sign of tag-probe
         int signProduct = tag->charge() * probe->charge();
         if (requireOS_) {
            if (signProduct > 0) goodCombination = false;
         }

         // insert the tag-probe pair if they are good
         if(goodCombination)
         {
            tagProbeCol.insert(tag, probe);
         }

      }  // end loop on probes
      }  // end if tag passed HLT (or wasn't reqired to)

   }  // end loop on tags
  

}

bool GeorgiosTagProbeAlgo::triggerPass(const reco::Candidate &trigCand,  
   	 					EgEff::EmObjectRef tagCand)
{

   double dEta = trigCand.eta() - tagCand->eta();
   double dPhi = trigCand.phi() - tagCand->phi();
   double dRVal = sqrt(dEta*dEta + dPhi*dPhi);

   return (dRVal < 0.2);

}

