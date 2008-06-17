//*****************************************************************************
// File:      ElectronRefToValueProducer.cc
// ----------------------------------------------------------------------------


// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include "EgammaAnalysis/ElectronIDProducers/interface/ElectronRefToValueProducer.h"

ElectronRefToValueProducer::ElectronRefToValueProducer(const edm::ParameterSet& config) : conf_(config)
{
  // use configuration file to setup input/output collection names
  electronProducer_               = conf_.getParameter<edm::InputTag>("electronProducer");
 
  //register your products
  produces < reco::GsfElectronCollection>("byValue");

}

ElectronRefToValueProducer::~ElectronRefToValueProducer(){}


								       
void ElectronRefToValueProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Get the input objects
  edm::Handle<edm::RefVector<reco::GsfElectronCollection> > electronHandle;;  
  iEvent.getByLabel(electronProducer_,electronHandle);

  std::auto_ptr<reco::GsfElectronCollection> pOutEle(new reco::GsfElectronCollection);

  for(unsigned int i = 0 ; i < electronHandle->size(); ++i ){
    pOutEle->push_back(*electronHandle->at(i));
  }
  
  iEvent.put(pOutEle,"byValue");

}
