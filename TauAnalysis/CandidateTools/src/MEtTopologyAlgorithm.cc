#include "TauAnalysis/CandidateTools/interface/MEtTopologyAlgorithm.h"

MEtTopologyAlgorithm::MEtTopologyAlgorithm(const edm::ParameterSet& cfg)
{
  globalThreshold_ = cfg.getParameter<double>("globalThreshold");
  verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
}

MEtTopologyAlgorithm::~MEtTopologyAlgorithm() 
{
//--- nothing to be done yet...
}

MEtTopology MEtTopologyAlgorithm::buildMEtTopology(const edm::Ptr<reco::MET>& metPtr, const energyDepositCollectionType& energyDeposits)
{
  MEtTopology metTopology;

  computeVproj(metTopology, metPtr, energyDeposits);

  return metTopology;
}

void MEtTopologyAlgorithm::computeVproj(MEtTopology& metTopology,
					const edm::Ptr<reco::MET>& metPtr, const energyDepositCollectionType& energyDeposits)
{
  double metPhi = metPtr->phi();

  double projAxisX = -cos(metPhi);
  double projAxisY = -sin(metPhi);

  double Vparallel = 0.;
  double Vanti = 0.;

  for ( energyDepositCollectionType::const_iterator energyDeposit = energyDeposits.begin();
	energyDeposit != energyDeposits.end(); ++energyDeposit ) {

    if ( energyDeposit->et() < globalThreshold_ ) continue;

    double projection = energyDeposit->px()*projAxisX + energyDeposit->py()*projAxisY;

    if ( projection > 0. )
      Vparallel += projection;
    else
      Vanti += fabs(projection);
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

