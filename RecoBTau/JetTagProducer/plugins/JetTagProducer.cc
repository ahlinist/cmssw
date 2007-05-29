// -*- C++ -*-
//
// Package:    JetTagProducer
// Class:      JetTagProducer
// 
/**\class JetTagProducer JetTagProducer.cc RecoBTag/JetTagProducer/src/JetTagProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Thu Apr  6 09:56:23 CEST 2006
// $Id: JetTagProducer.cc,v 1.4 2007/05/28 02:52:56 saout Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoBTau/JetTagProducer/interface/JetTagProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "DataFormats/BTauReco/interface/BaseTagInfoFwd.h"

#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputerRecord.h"
using namespace std;
using namespace reco;
using namespace edm;

//
// constructors and destructor
//
template <class ConcreteTagInfoCollection>
JetTagProducer<ConcreteTagInfoCollection>::JetTagProducer(const edm::ParameterSet& iConfig) : 
  m_config(iConfig) {

  m_tagInfo = iConfig.getParameter<edm::InputTag>("tagInfo");
  m_jetTagComputer = iConfig.getParameter<string>("jetTagComputer");

  produces<reco::JetTagCollection>();

}

template <class ConcreteTagInfoCollection>
JetTagProducer<ConcreteTagInfoCollection>::~JetTagProducer()
{
}

//
// member functions
//
// ------------ method called to produce the data  ------------
template <class ConcreteTagInfoCollection>
void
JetTagProducer<ConcreteTagInfoCollection>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  Handle<ConcreteTagInfoCollection> tagInfoHandle;
  iEvent.getByLabel(m_tagInfo,tagInfoHandle);
       
  edm::ESHandle<JetTagComputer> computer;
  iSetup.get<JetTagComputerRecord>().get( m_jetTagComputer, computer );
  m_computer = computer.product() ;
  m_computer->setEventSetup(iSetup);

  std::auto_ptr<reco::JetTagCollection> jetTagCollection(new reco::JetTagCollection());
   
  typename ConcreteTagInfoCollection::const_iterator it = tagInfoHandle->begin();
  for (int cc=0; it != tagInfoHandle->end(); it++, cc++)
  {
    JetTag jt(m_computer->discriminator(*it));
    jt.setTagInfo(RefToBase<BaseTagInfo>(edm::Ref<ConcreteTagInfoCollection>(tagInfoHandle,cc)));
    jetTagCollection->push_back(jt);    
  }
  
  iEvent.put(jetTagCollection);
}


#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"

// define these as plug-ins
typedef JetTagProducer<TrackCountingTagInfoCollection>  JetTagProducerTrackCounting;
DEFINE_FWK_MODULE(JetTagProducerTrackCounting);
typedef JetTagProducer<TrackIPTagInfoCollection>        JetTagProducerImpactParameter;
DEFINE_FWK_MODULE(JetTagProducerImpactParameter);
typedef JetTagProducer<CombinedSVTagInfoCollection>     JetTagProducerCombinedSV;
DEFINE_FWK_MODULE(JetTagProducerCombinedSV);
typedef JetTagProducer<SoftLeptonTagInfoCollection>     JetTagProducerSoftLepton;
DEFINE_FWK_MODULE(JetTagProducerSoftLepton);
