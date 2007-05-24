#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/TagEmObjectAlgo.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

void TagEmObjectAlgo::initialise(const edm::ParameterSet &params)
{
   _inputProducer = params.getParameter<std::string>("InputProducer");
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

   // Create TagEmObjects
   const reco::PixelMatchGsfElectronCollection *gsfEles = eleCandidatesHandle.product();
   for(reco::PixelMatchGsfElectronCollection::const_iterator ele = gsfEles->begin(); ele != gsfEles->end(); ++ele)
   {
      EgEff::EmObject newEle(ele->charge(), ele->p4(), ele->vertex());
      newEle.setGsfTrack(ele->gsfTrack());
      newEle.setSuperCluster(ele->superCluster());
      outCol.push_back(newEle);
   }
}

