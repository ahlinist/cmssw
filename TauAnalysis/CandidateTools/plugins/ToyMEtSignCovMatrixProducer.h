#ifndef TauAnalysis_CandidateTools_ToyMEtSignCovMatrixProducer_h  
#define TauAnalysis_CandidateTools_ToyMEtSignCovMatrixProducer_h

/** \class ToyMEtSignCovMatrixProducer
 *
 * Produce covariance matrix representing resolution of "toy MC" pat::MET objects 
 * obtained by smearing Monte Carlo "truth" information 
 *
 * \author Christian Veelken, LLR
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ToyMEtSignCovMatrixProducer.h,v 1.2 2012/08/28 15:00:24 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "JetMETCorrections/METPUSubtraction/interface/PFMEtSignInterfaceBase.h"

class ToyMEtSignCovMatrixProducer : public edm::EDProducer 
{
 public:
  
  ToyMEtSignCovMatrixProducer(const edm::ParameterSet&);
  ~ToyMEtSignCovMatrixProducer();

  void produce(edm::Event&, const edm::EventSetup&);

 private:

  std::string moduleLabel_;

  double resolutionX_;
  double resolutionY_;
};

#endif


