#include "EgammaAnalysis/ElectronIDProducers/interface/ElectronIDProducer.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

void ElectronIDProducer::beginJob(edm::EventSetup const& iSetup) {
  if (doCutBased_) cutBasedAlgo_->setup(conf_);
  if (doLikelihood_) likelihoodAlgo_->setup(conf_);
  if (doNeuralNet_) neuralNetAlgo_->setup(conf_);
}

ElectronIDProducer::ElectronIDProducer(const edm::ParameterSet& conf) : conf_(conf) {

  electronProducer_ = conf_.getParameter<std::string>("electronProducer");
  electronLabel_ = conf_.getParameter<std::string>("electronLabel");
  electronIDLabel_ = conf_.getParameter<std::string>("electronIDLabel");
  electronIDAssociation_ = conf_.getParameter<std::string>("electronIDAssociationLabel");

  doCutBased_ = conf_.getParameter<bool>("doCutBased");
  doLikelihood_ = conf_.getParameter<bool>("doLikelihood");
  doNeuralNet_ = conf_.getParameter<bool>("doNeuralNet");

  cutBasedAlgo_ = new CutBasedElectronID();
  likelihoodAlgo_ = new ElectronLikelihood();
  neuralNetAlgo_ = new ElectronNeuralNet();

  produces<reco::ElectronIDCollection>(electronIDLabel_);
  produces<reco::ElectronIDAssociationCollection>(electronIDAssociation_);

}

ElectronIDProducer::~ElectronIDProducer() {
  delete cutBasedAlgo_;
  delete likelihoodAlgo_;
  delete neuralNetAlgo_;
}

void ElectronIDProducer::produce(edm::Event& e, const edm::EventSetup& c) {

  // Read in electrons
  edm::Handle<reco::PixelMatchGsfElectronCollection> electrons;
  e.getByLabel(electronProducer_,electronLabel_,electrons);

  // Initialize output electron ID collection
  reco::ElectronIDCollection electronIDCollection;
  std::auto_ptr<reco::ElectronIDCollection> electronIDCollection_p(new reco::ElectronIDCollection);

  // Loop over electrons and calculate electron ID using specified technique(s)
  reco::PixelMatchGsfElectronCollection::const_iterator electron;
  for (electron = (*electrons).begin();
       electron != (*electrons).end(); ++electron) {
    bool cutBasedDecision = -1;
    double likelihood = -1.;
    double neuralNetOutput = -1.;
    if (doCutBased_) cutBasedDecision = cutBasedAlgo_->result(&(*electron),e);
    if (doLikelihood_) likelihood = likelihoodAlgo_->result(&(*electron),e);
    if (doNeuralNet_) neuralNetOutput = neuralNetAlgo_->result(&(*electron),e);
    electronIDCollection.push_back(reco::ElectronID(cutBasedDecision,
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
    electronIDAssocs_p->insert(edm::Ref<reco::PixelMatchGsfElectronCollection>(electrons,i),edm::Ref<reco::ElectronIDCollection>(electronIDHandle,i));
  }  
  e.put(electronIDAssocs_p,electronIDAssociation_);

}
