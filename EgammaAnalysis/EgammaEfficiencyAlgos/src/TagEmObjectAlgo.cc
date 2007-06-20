#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagEmObjectAlgo.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

//#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

void TagEmObjectAlgo::initialise(const edm::ParameterSet &params)
{
   _inputProducer = params.getParameter<std::string>("InputProducer");
   _electronIDAssocProducer = params.getParameter<std::string>("ElectronIDAssociationProducer");
}

void TagEmObjectAlgo::run(const edm::Event &event, EgEff::EmObjectCollection &outCol)
{
   // Get electrons
   edm::Handle<reco::PixelMatchGsfElectronCollection> eleCandidatesHandle;
   try
   {
      event.getByLabel(_inputProducer, eleCandidatesHandle);
   }
   catch(cms::Exception &ex)
   {
      edm::LogError("TagEmObjectAlgo") << "Error! Can't get collection " << _inputProducer;
      throw ex;
   }

   // Get electronId association map
   edm::Handle<reco::ElectronIDAssociationCollection> electronIDAssocHandle;
   try
   {
      event.getByLabel(_electronIDAssocProducer, electronIDAssocHandle);
   }
   catch(cms::Exception &ex)
   {
      edm::LogError("TagEmObjectAlgo") << "Error! Can't get collection " << _electronIDAssocProducer;
      throw ex;
   }

   // Loop over electrons
   for(unsigned int i = 0; i < eleCandidatesHandle->size(); ++i)
   {
      // Get cut decision for each electron
      edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(eleCandidatesHandle, i);
      reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
      electronIDAssocItr = electronIDAssocHandle->find(electronRef);
      const reco::ElectronIDRef &id = electronIDAssocItr->val;
      bool cutDecision = id->cutBasedDecision();
      if(cutDecision)
      {
         // Create EM Object for gsf electron which passed all cuts
         EgEff::EmObject newEle(electronRef->charge(), electronRef->p4(), electronRef->vertex());
         newEle.setGsfTrack(electronRef->gsfTrack());
         newEle.setSuperCluster(electronRef->superCluster());
         outCol.push_back(newEle);
      }
   } 
}

