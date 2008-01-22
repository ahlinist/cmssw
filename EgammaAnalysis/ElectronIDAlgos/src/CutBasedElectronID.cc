#include "EgammaAnalysis/ElectronIDAlgos/interface/CutBasedElectronID.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

void CutBasedElectronID::setup(const edm::ParameterSet& conf) {
  
  // Get all the parameters
  baseSetup(conf);
  
  // Select algo dependent set of parameters
  std::vector<edm::ParameterSet> algoPSets = conf.getParameter<std::vector<edm::ParameterSet> >("algo_psets");
  
  quality_ = algoPSets[1].getParameter<std::string>("electronQuality");
  
  if (quality_ == "tight") {
    cuts_ = algoPSets[1].getParameter<edm::ParameterSet>("tightEleIDCuts");
  } else if (quality_=="robust") {
    cuts_ = algoPSets[1].getParameter<edm::ParameterSet>("robustEleIDCuts");
  } else if (quality_=="loose") {
    cuts_ = algoPSets[1].getParameter<edm::ParameterSet>("looseEleIDCuts");
  } else {
    throw(std::runtime_error("\n\nElectronIDProducer: Invalid electronQuality parameter: must be tight, loose or robust.\n\n"));
  }
}

int CutBasedElectronID::classify(const reco::GsfElectron* electron) {
  
  double eta = electron->p4().Eta();
  double eOverP = electron->eSuperClusterOverP();
  double pin  = electron->trackMomentumAtVtx().R(); 
  double pout = electron->trackMomentumOut().R(); 
  double fBrem = (pin-pout)/pin;
  
  int cat;
  
  if((fabs(eta)<1.479 && fBrem<0.06) || (fabs(eta)>1.479 && fBrem<0.1)) 
    cat=1;
  else if (eOverP < 1.2 && eOverP > 0.8) 
    cat=0;
  else 
    cat=2;
  
  return cat;
}

bool CutBasedElectronID::result(const reco::GsfElectron* electron,
                                const edm::Event& e) { 
  
  double eta = fabs(electron->p4().Eta());
  const reco::ClusterShapeRef& shapeRef = getClusterShape(electron, e);
  
  double eOverP = electron->eSuperClusterOverP();
  double eSeed = electron->superCluster()->seed()->energy();
  double pin  = electron->trackMomentumAtVtx().R();   
  double eSeedOverPin = eSeed/pin; 
  double pout = electron->trackMomentumOut().R(); 
  double fBrem = (pin-pout)/pin;
  
  double hOverE = electron->hadronicOverEm();
  double sigmaee = sqrt(shapeRef->covEtaEta());
  double deltaPhiIn = electron->deltaPhiSuperClusterTrackAtVtx();
  double deltaEtaIn = electron->deltaEtaSuperClusterTrackAtVtx();
  
  int eb;
  if (eta < 1.479) 
    eb = 0;
  else {
    eb = 1; 
    sigmaee = sigmaee - 0.02*(fabs(eta) - 2.3);   //correct sigmaetaeta dependence on eta in endcap
  }

  std::vector<double> cut;
    
  // ROBUST Selection
  if (quality_ == "robust") {

    // hoe, sigmaEtaEta, dPhiIn, dEtaIn
    if (eta < 1.479)
      cut = cuts_.getParameter<std::vector<double> >("barrel");
    else
      cut = cuts_.getParameter<std::vector<double> >("endcap");

    if (hOverE > cut[0]) 
      return false;    

    if (sigmaee > cut[1]) 
      return false;    

    if (fabs(deltaPhiIn) > cut[2]) 
      return false;    

    if (fabs(deltaEtaIn) > cut[3]) 
      return false;    
    
    return true;
  }
  
  int cat = classify(electron);

  // TIGHT Selection
  if (quality_ == "tight") {
    
    if ((eOverP < 0.8) && (fBrem < 0.2)) 
      return false;
    
    if (eOverP < 0.9*(1-fBrem))
      return false;
    
    cut = cuts_.getParameter<std::vector<double> >("hOverE");
    if (hOverE > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("sigmaEtaEta");
    if (sigmaee > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("deltaPhiIn");
    if (eOverP < 1.5) {
      if (fabs(deltaPhiIn) > cut[cat+4*eb]) 
        return false;    
    } else {
      if (fabs(deltaPhiIn) > cut[3+4*eb])
        return false;
    }
    
    cut = cuts_.getParameter<std::vector<double> >("deltaEtaIn");
    if (fabs(deltaEtaIn) > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("eSeedOverPinMin");
    if (eSeedOverPin < cut[cat+4*eb]) 
      return false;  

    cut = cuts_.getParameter<std::vector<double> >("eSeedOverPinMax");
    if (eSeedOverPin > cut[cat+4*eb]) 
      return false;  

    return true;
  }
  
    // LOOSE Selection
  if (quality_ == "loose") {
    
    if ((eOverP < 0.8) && (fBrem < 0.2)) 
      return false;
    
    cut = cuts_.getParameter<std::vector<double> >("hOverE");
    if (hOverE > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("sigmaEtaEta");
    if (sigmaee > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("deltaPhiIn");
    if (eOverP < 1.5) {
      if (fabs(deltaPhiIn) > cut[cat+4*eb]) 
        return false;    
    } else {
      if (fabs(deltaPhiIn) > cut[3+4*eb])
        return false;
    }
    
    cut = cuts_.getParameter<std::vector<double> >("deltaEtaIn");
    if (fabs(deltaEtaIn) > cut[cat+4*eb]) 
      return false;    
    
    cut = cuts_.getParameter<std::vector<double> >("eSeedOverPin");
    if (eSeedOverPin < cut[cat+4*eb]) 
      return false;  
    
    return true; 
  }
  
  return false;
}
