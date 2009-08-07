#ifndef TauAnalysis_RecoTools_PATLeptonRecoilEnergyAlgorithm_h
#define TauAnalysis_RecoTools_PATLeptonRecoilEnergyAlgorithm_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include <TMath.h>

const double epsilon = 0.01;

template<typename T1, typename T2>
class PATLeptonRecoilEnergyAlgorithm 
{
 public:

  PATLeptonRecoilEnergyAlgorithm(const edm::ParameterSet& cfg)
  {
    etaMin_ = cfg.getParameter<double>("etaMin");
    etaMax_ = cfg.getParameter<double>("etaMax");
    etMin_ = ( cfg.exists("etMin") ) ? cfg.getParameter<double>("etMin") : 0.;
    dPhiMin_ = ( cfg.exists("dPhiMin") ) ? cfg.getParameter<double>("dPhiMin") : 0.;
    dPhiMax_ = ( cfg.exists("dPhiMax") ) ? cfg.getParameter<double>("dPhiMax") : TMath::Pi() + epsilon;
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
  }
  ~PATLeptonRecoilEnergyAlgorithm() {}
  
  PATLeptonRecoilEnergy<T1,T2> buildRecoilEnergy(const edm::Ptr<T1> lepton, const edm::View<T2>& energyObjects)
  {
    edm::PtrVector<T2> recoilEnergyObjects;
    double etSum = 0.;

    for ( unsigned idxEnergyObject = 0, numEnergyObjects = energyObjects.size(); 
	  idxEnergyObject < numEnergyObjects; ++idxEnergyObject ) {
      edm::Ptr<T2> energyObjectPtr = energyObjects.ptrAt(idxEnergyObject);

      if ( energyObjectPtr->eta() > etaMin_ && energyObjectPtr->eta() < etaMax_ &&
	   energyObjectPtr->et() > etMin_ ) {
	double dPhi = TMath::Abs(normalizedPhi(lepton->phi() - energyObjectPtr->phi()));
	if ( dPhi > dPhiMin_ && dPhi < dPhiMax_ ) {
	  recoilEnergyObjects.push_back(energyObjectPtr);
	  etSum += energyObjectPtr->et();
	}
      }
      
    }

    PATLeptonRecoilEnergy<T1,T2> patLeptonRecoilEnergy(lepton, recoilEnergyObjects);
    patLeptonRecoilEnergy.setEtSum(etSum);
    
    if ( verbosity_ ) {
      std::cout << "pat::Lepton phi = " << lepton->phi()*180./TMath::Pi() << std::endl;
      std::cout << "recoil Et sum = " << etSum << std::endl;
    }

    return patLeptonRecoilEnergy;
  }

 private: 

  double etaMin_;
  double etaMax_;
  double etMin_;
  double dPhiMin_;
  double dPhiMax_;
  
  int verbosity_;
};

#endif 

