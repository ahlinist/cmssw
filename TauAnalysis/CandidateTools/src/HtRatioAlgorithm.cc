#include "TauAnalysis/CandidateTools/interface/HtRatioAlgorithm.h"

HtRatioAlgorithm::HtRatioAlgorithm(const edm::ParameterSet& cfg)
{
  verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);
}

HtRatioAlgorithm::~HtRatioAlgorithm() 
{
//--- nothing to be done HtRatio...
}

HtRatio HtRatioAlgorithm::buildHtRatio(const edm::Ptr<pat::Tau>& tauPtr, const jetCollectionType& jets)
{
  HtRatio htRatio;

  computeHtRatio(htRatio, tauPtr, jets);

  return htRatio;
}

void HtRatioAlgorithm::computeHtRatio(HtRatio& htRatio,
					const edm::Ptr<pat::Tau>& tauPtr, const jetCollectionType& jets)
{
  double tauPt = tauPtr->pt();
  double sumJetPt = 0;

  for ( jetCollectionType::const_iterator iJet = jets.begin(); iJet != jets.end(); ++iJet ) 
    sumJetPt += iJet->pt();
  
  double Ratio = tauPt/sumJetPt;
  
  if ( verbosity_ ) {
	std::cout << "<HtRatioAlgorithm::computeHtRatio>:" << std::endl;
	std::cout << " tau Pt = " << tauPt << std::endl;
	std::cout << " tau Pt + sum jets Pt = " << sumJetPt+tauPt << std::endl;
	std::cout << "HT-ratio = " <<Ratio<<std::endl;
  }
  
  htRatio.setRatio(Ratio);
}

