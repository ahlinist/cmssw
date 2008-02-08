#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronNeuralNet.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"


void ElectronNeuralNet::setup(const edm::ParameterSet& conf)
{
  mySet_ = conf.getParameter<std::string>("NN_set");
  weightsDir_ = conf.getParameter<std::string>("weightsDir");
  barrelClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("barrelClusterShapeAssociation");
  endcapClusterShapeAssocProducer_ = conf.getParameter<edm::InputTag>("endcapClusterShapeAssociation");

  readerBarrel = new TMVA::Reader("!V");
  readerEndcap = new TMVA::Reader("!V");

  readerBarrel->AddVariable("(1./E)-(1./p_in)",&EoP_inv);
  readerBarrel->AddVariable("HoE",&HoE);
  readerBarrel->AddVariable("covEtaEta",&covEtaEta);
  readerBarrel->AddVariable("Deta_in",&deta_in);
  readerEndcap->AddVariable("(1./E)-(1./p_in)",&EoP_inv);
  readerEndcap->AddVariable("HoE",&HoE);
  readerEndcap->AddVariable("covEtaEta",&covEtaEta);
  readerEndcap->AddVariable("Deta_in",&deta_in);

  string weightsFileBarrel    = weightsDir_+"weights_"+mySet_+"_barrel.txt";
  string weightsFileEndcap    = weightsDir_+"weights_"+mySet_+"_endcap.txt";

  readerBarrel->BookMVA("MLP method", weightsFileBarrel );
  readerEndcap->BookMVA("MLP method", weightsFileEndcap );
}


double ElectronNeuralNet::result(const reco::GsfElectron* electron,
				 const edm::Event& e) {
  E = electron->energy();
  p_in = sqrt(electron->trackMomentumAtVtx().Mag2());
  EoP_inv = (1./E) - (1./p_in);
  HoE = electron->hadronicOverEm();
  deta_in  = electron->deltaEtaSuperClusterTrackAtVtx();
  const reco::ClusterShapeRef& shapeRef = getClusterShape(electron, e);
  covEtaEta = shapeRef->covEtaEta();


  if ( fabs((*electron).eta()) < 1.440 )
    retval = readerBarrel->EvaluateMVA("MLP method");
  else
    retval = readerEndcap->EvaluateMVA("MLP method");


  return retval;

}
