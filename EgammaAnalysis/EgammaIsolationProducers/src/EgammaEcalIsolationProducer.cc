//*****************************************************************************
// File:      EgammaEcalIsolationProducer.cc
// ----------------------------------------------------------------------------
// OrigAuth:  Matthias Mozer
// Institute: IIHE-VUB
//=============================================================================
//*****************************************************************************


#include "EgammaAnalysis/EgammaIsolationProducers/interface/EgammaEcalIsolationProducer.h"

// Framework
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"


EgammaEcalIsolationProducer::EgammaEcalIsolationProducer(const edm::ParameterSet& config) : conf_(config)
{
 // use configuration file to setup input/output collection names
  emObjectProducer_               = conf_.getParameter<edm::InputTag>("emObjectProducer");

  basicClusterProducer_           = conf_.getParameter<edm::InputTag>("basicClusterProducer");
  superClusterProducer_           = conf_.getParameter<edm::InputTag>("superClusterProducer");

  etLow_               = conf_.getParameter<double>("etMin");
  extRadius_            = conf_.getParameter<double>("extRadius");

  absolut_ = conf_.getParameter<bool>("absolut");

  //register your products
  produces < reco::CandViewDoubleAssociations >();
}


EgammaEcalIsolationProducer::~EgammaEcalIsolationProducer(){}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EgammaEcalIsolationProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  // Get the  filtered objects
  edm::Handle< edm::View<reco::Candidate> > emObjectHandle;
  iEvent.getByLabel(emObjectProducer_,emObjectHandle);
  
  // Get the basicClusters hcal hits
  edm::Handle<reco::BasicClusterCollection> basicClusterHandle;
  iEvent.getByLabel(basicClusterProducer_, basicClusterHandle);
  const reco::BasicClusterCollection* basicClusters = basicClusterHandle.product();
  
  // Get the superClusters hcal hits
  edm::Handle<reco::SuperClusterCollection> superClusterHandle;
  iEvent.getByLabel(superClusterProducer_, superClusterHandle);
  const reco::SuperClusterCollection* superClusters = superClusterHandle.product();
  

  reco::CandViewDoubleAssociations* isoMap = new reco::CandViewDoubleAssociations( reco::CandidateBaseRefProd( emObjectHandle ) );


  EgammaEcalIsolation myEcalIsolation(extRadius_,
				      etLow_,
				      basicClusters,
				      superClusters) ;

  
  for( size_t i = 0 ; i < emObjectHandle->size(); ++i) {
    double isoValue = myEcalIsolation.getEcalEtSum(&(emObjectHandle->at(i)));
    if(absolut_==true){
      isoMap->setValue(i,isoValue);
    }
    else{
      reco::SuperClusterRef sc = (emObjectHandle->at(i)).get<reco::SuperClusterRef>();
      double et = sc.get()->energy()*sin(2*atan(exp(-sc.get()->eta())));
      isoMap->setValue(i,isoValue/et);
    }
  }



  std::auto_ptr<reco::CandViewDoubleAssociations> isolMap(isoMap);
  iEvent.put(isolMap);

}

//define this as a plug-in
//DEFINE_FWK_MODULE(EgammaEcalIsolationProducer);
