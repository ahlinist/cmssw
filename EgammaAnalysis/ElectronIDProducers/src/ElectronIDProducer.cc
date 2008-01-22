#include "EgammaAnalysis/ElectronIDProducers/interface/ElectronIDProducer.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

void ElectronIDProducer::beginJob(edm::EventSetup const& iSetup) {
  if (doPtdrId_) ptdrAlgo_->setup(conf_);
  if (doCutBased_) cutBasedAlgo_->setup(conf_);
  if (doNeuralNet_) neuralNetAlgo_->setup(conf_);
  if (doPtdrId_ && doCutBased_) 	 	     
     throw(std::runtime_error("\n\nElectronIDProducer: you cannot choose both PTDR algo and CutBased Algo.\n\n"));
}

ElectronIDProducer::ElectronIDProducer(const edm::ParameterSet& conf) : conf_(conf) {

  electronProducer_ = conf_.getParameter<std::string>("electronProducer");
  electronLabel_ = conf_.getParameter<std::string>("electronLabel");
  electronIDLabel_ = conf_.getParameter<std::string>("electronIDLabel");
  electronIDAssociation_ = conf_.getParameter<std::string>("electronIDAssociationLabel");

  doPtdrId_ = conf_.getParameter<bool>("doPtdrId");
  doCutBased_ = conf_.getParameter<bool>("doCutBased");
  doLikelihood_ = conf_.getParameter<bool>("doLikelihood");
  doNeuralNet_ = conf_.getParameter<bool>("doNeuralNet");

  ptdrAlgo_ = new PTDRElectronID();
  cutBasedAlgo_ = new CutBasedElectronID();
  neuralNetAlgo_ = new ElectronNeuralNet();

  barrelClusterShapeAssociation_ = conf_.getParameter<edm::InputTag>("barrelClusterShapeAssociation");
  endcapClusterShapeAssociation_ = conf.getParameter<edm::InputTag>("endcapClusterShapeAssociation");

  produces<reco::ElectronIDCollection>(electronIDLabel_);
  produces<reco::ElectronIDAssociationCollection>(electronIDAssociation_);

}

ElectronIDProducer::~ElectronIDProducer() {
  delete ptdrAlgo_;
  delete cutBasedAlgo_;
  delete neuralNetAlgo_;
}

void ElectronIDProducer::produce(edm::Event& e, const edm::EventSetup& c) {

  // Read in electrons
  edm::Handle<reco::GsfElectronCollection> electrons;
  e.getByLabel(electronProducer_,electronLabel_,electrons);

  // Initialize output electron ID collection
  reco::ElectronIDCollection electronIDCollection;
  std::auto_ptr<reco::ElectronIDCollection> electronIDCollection_p(new reco::ElectronIDCollection);

  // get the association of the clusters to their shapes for EB
  edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle ;
  e.getByLabel (barrelClusterShapeAssociation_, barrelClShpHandle) ;
  if (!barrelClShpHandle.isValid()) {
    edm::LogError ("ElectronIDProducer") << "Can't get ECAL barrel Cluster Shape Collection" ; 
  }

  const reco::BasicClusterShapeAssociationCollection& barrelClShpMap = *barrelClShpHandle ;

  // get the association of the clusters to their shapes for EE
  edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShpHandle ;
  e.getByLabel (endcapClusterShapeAssociation_, endcapClShpHandle) ;
  if (!endcapClShpHandle.isValid()) {
    edm::LogError ("ElectronIDLHProducer") << "Can't get ECAL endcap Cluster Shape Collection" ;
  }
  
  const reco::BasicClusterShapeAssociationCollection& endcapClShpMap = *endcapClShpHandle ;



  // Loop over electrons and calculate electron ID using specified technique(s)
  reco::GsfElectronCollection::const_iterator electron;
  for (electron = (*electrons).begin();
       electron != (*electrons).end(); ++electron) {

    
    bool boolDecision = -1; 
    bool ptdrDecision = -1;
    bool cutBasedDecision = -1;
    double likelihood = -1.;
    double neuralNetOutput = -1.;
    if (doPtdrId_) 
    	ptdrDecision = ptdrAlgo_->result(&(*electron), e);
    if (doCutBased_) 
    	cutBasedDecision = cutBasedAlgo_->result(&(*electron),e);
    if (doLikelihood_) 
      {
	bool hasBarrel=true ;
	bool hasEndcap=true ;

	reco::SuperClusterRef sclusRef = electron->get<reco::SuperClusterRef> () ;
	reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;
	seedShpItr = barrelClShpMap.find (sclusRef->seed ()) ;
	if ( seedShpItr==barrelClShpMap.end ())  {
	  hasBarrel=false ;
	  seedShpItr=endcapClShpMap.find (sclusRef->seed ()) ;
	  if ( seedShpItr==endcapClShpMap.end () ) hasEndcap=false ;
	}
	if (hasBarrel || hasEndcap) {
	  const reco::ClusterShapeRef& sClShape = seedShpItr->val ;
	  // get electron likelihood
	  edm::ESHandle<ElectronLikelihood> likelihoodAlgo ;
	  c.getData ( likelihoodAlgo ) ;
	  likelihood = likelihoodAlgo->result (*electron,*sClShape) ;
	}
      }

    if (doNeuralNet_) 
    	neuralNetOutput = neuralNetAlgo_->result(&(*electron),e);
    
    if (doPtdrId_)
    	boolDecision = ptdrDecision ; 	 
    else
    	boolDecision = cutBasedDecision ;
    
    electronIDCollection.push_back(reco::ElectronID(boolDecision,
						    likelihood,
						    neuralNetOutput));
  }

  // Add output electron ID collection to the event
  electronIDCollection_p->assign(electronIDCollection.begin(),
				 electronIDCollection.end());
  edm::OrphanHandle<reco::ElectronIDCollection> electronIDHandle = e.put(electronIDCollection_p,electronIDLabel_);

  // Add electron ID AssociationMap to the event
  std::auto_ptr<reco::ElectronIDAssociationCollection> electronIDAssocs_p(new reco::ElectronIDAssociationCollection);
  for (unsigned int i = 0; i < electrons->size(); i++){
    electronIDAssocs_p->insert(edm::Ref<reco::GsfElectronCollection>(electrons,i),edm::Ref<reco::ElectronIDCollection>(electronIDHandle,i));
  }
  e.put(electronIDAssocs_p,electronIDAssociation_);

}
