#include "TauAnalysis/CandidateTools/interface/MEtTopologyAlgorithm.h"

#include <TMath.h>

MEtTopologyAlgorithm::MEtTopologyAlgorithm(const edm::ParameterSet& cfg)
{
  globalThreshold_ = cfg.getParameter<double>("globalThreshold");
  verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
}

MEtTopologyAlgorithm::~MEtTopologyAlgorithm() 
{
//--- nothing to be done yet...
}

MEtTopology MEtTopologyAlgorithm::buildMEtTopology(const std::vector<energyDepositCollectionType>& energyDepositCollections)
{
  MEtTopology metTopology;

  computeVproj(metTopology, energyDepositCollections);

  return metTopology;
}

void MEtTopologyAlgorithm::computeVproj(MEtTopology& metTopology,
					const std::vector<energyDepositCollectionType>& energyDepositCollections)
{
  reco::Candidate::LorentzVector metP4;
  for ( std::vector<energyDepositCollectionType>::const_iterator energyDepositCollection = energyDepositCollections.begin();
	energyDepositCollection != energyDepositCollections.end(); ++energyDepositCollection ) {
    for ( energyDepositCollectionType::const_iterator energyDeposit = energyDepositCollection->begin();
	  energyDeposit != energyDepositCollection->end(); ++energyDeposit ) {
      if ( energyDeposit->et() > globalThreshold_ ) metP4 += energyDeposit->p4();
    }
  }
  
  double metPhi = metP4.phi();

  double projAxisX = -TMath::Cos(metPhi);
  double projAxisY = -TMath::Sin(metPhi);

  double Vparallel = 0.;
  double Vanti = 0.;

  for ( std::vector<energyDepositCollectionType>::const_iterator energyDepositCollection = energyDepositCollections.begin();
	energyDepositCollection != energyDepositCollections.end(); ++energyDepositCollection ) {
    for ( energyDepositCollectionType::const_iterator energyDeposit = energyDepositCollection->begin();
	  energyDeposit != energyDepositCollection->end(); ++energyDeposit ) {
      if ( energyDeposit->et() > globalThreshold_ ) {	
	double projection = energyDeposit->px()*projAxisX + energyDeposit->py()*projAxisY;     
	if ( projection > 0. )
	  Vparallel += projection;
	else
	  Vanti += fabs(projection);
      }
    }
  }

  if ( verbosity_ ) {
    std::cout << "<MEtTopologyAlgorithm::computeVproj>:" << std::endl;
    std::cout << " Vparallel = " << Vparallel << std::endl;
    std::cout << " Vanti = " << Vanti << std::endl;
    std::cout << " Vanti/Vparallel = " << Vanti/Vparallel << std::endl;
  }

  metTopology.setVparallel(Vparallel);
  metTopology.setVanti(Vanti);
}

