#include "EgammaAnalysis/ElectronIDAlgos/interface/CutBasedElectronID.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

void CutBasedElectronID::setup(const edm::ParameterSet& conf) {

  baseSetup(conf);

  quality_ = conf.getParameter<std::string>("electronQuality");
  if (quality_=="tight") {
    cuts_ = conf.getParameter<edm::ParameterSet>("tightEleIDCuts");
  } else if (quality_=="medium") {
    cuts_ = conf.getParameter<edm::ParameterSet>("mediumEleIDCuts");
  } else if (quality_=="loose") {
    cuts_ = conf.getParameter<edm::ParameterSet>("looseEleIDCuts");
  } else {
    throw(std::runtime_error("\n\nElectronIDProducer: Invalid electronQuality parameter: must be tight, medium or loose.\n\n"));
  }

}

bool CutBasedElectronID::result(const reco::PixelMatchGsfElectron* electron,
				const edm::Event& e) {

  if (electron->classification()==40) return true;

  //determine which element of the cut arrays in electronId.cfi to read
  //depending on the electron classification
  int icut=0;
  switch (electron->classification()) {
  case 0: icut=0; break;
  case 10: icut=0; break;
  case 20: icut=0; break;
  case 30: icut=1; break;
  case 31: icut=1; break;
  case 32: icut=1; break;
  case 33: icut=1; break;
  case 34: icut=1; break;
  case 100: icut=2; break;
  case 110: icut=2; break;
  case 120: icut=2; break;
  case 130: icut=3; break;
  case 131: icut=3; break;
  case 132: icut=3; break;
  case 133: icut=3; break;
  case 134: icut=3; break;
  default:
    edm::LogError("CutBasedElectronID") << "Error: unrecognized electron classification ";
    break;
  }

  if (useEoverPIn_) {
    double value = electron->eSuperClusterOverP();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("EoverPInMax");
    std::vector<double> mincut = cuts_.getParameter<std::vector<double> >("EoverPInMin");
    if (value<mincut[icut] || value>maxcut[icut]) return false;
  }

  if (useDeltaEtaIn_) {
    double value = electron->deltaEtaSuperClusterTrackAtVtx();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("deltaEtaIn");
    if (fabs(value>maxcut[icut])) return false;
  }

  if (useDeltaPhiIn_) {
    double value = electron->deltaPhiSuperClusterTrackAtVtx();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("deltaPhiIn");
    if (fabs(value>maxcut[icut])) return false;
  }

  if (useHoverE_) {
    double value = electron->hadronicOverEm();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("HoverE");
    if (fabs(value>maxcut[icut])) return false;
  }

  if (useEoverPOut_) {
    double value = electron->eSeedClusterOverPout();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("EoverPOutMax");
    std::vector<double> mincut = cuts_.getParameter<std::vector<double> >("EoverPOutMin");
    if (value<mincut[icut] || value>maxcut[icut]) return false;
  }

  if (useDeltaPhiOut_) {
    double value = electron->deltaPhiSuperClusterTrackAtVtx();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("deltaPhiOut");
    if (fabs(value>maxcut[icut])) return false;
  }

  if (useInvEMinusInvP_) {
    double value = (1./electron->caloEnergy())-(1./electron->trackMomentumAtVtx().R());
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("invEMinusInvP");
    if (value>maxcut[icut]) return false;
  }

  if (useBremFraction_) {
    double value = electron->trackMomentumAtVtx().R()-electron->trackMomentumOut().R();
    std::vector<double> mincut = cuts_.getParameter<std::vector<double> >("bremFraction");
    if (value<mincut[icut]) return false;
  }

  const reco::ClusterShapeRef& shapeRef = getClusterShape(electron,e);

  if (useE9overE25_) {
    double value = shapeRef->e3x3()/shapeRef->e5x5();
    std::vector<double> mincut = cuts_.getParameter<std::vector<double> >("E9overE25");
    if (fabs(value<mincut[icut])) return false;
  }

  if (useSigmaEtaEta_) {
    double value = shapeRef->covEtaEta();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("sigmaEtaEtaMax");
    std::vector<double> mincut = cuts_.getParameter<std::vector<double> >("sigmaEtaEtaMin");
    if (value<mincut[icut] || value>maxcut[icut]) return false;
  }

  if (useSigmaPhiPhi_) {
    double value = shapeRef->covPhiPhi();
    std::vector<double> maxcut = cuts_.getParameter<std::vector<double> >("sigmaPhiPhi");
    if (value>maxcut[icut]) return false;
  }

  return true;

}
