#include "TauAnalysis/CandidateTools/plugins/ToyMEtSignCovMatrixProducer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <TMatrixD.h>

ToyMEtSignCovMatrixProducer::ToyMEtSignCovMatrixProducer(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  resolutionX_ = cfg.getParameter<double>("resolutionX");
  resolutionY_ = cfg.getParameter<double>("resolutionY");

  produces<PFMEtSignCovMatrix>();
}

ToyMEtSignCovMatrixProducer::~ToyMEtSignCovMatrixProducer()
{
// nothing to be done yet...
}

void ToyMEtSignCovMatrixProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  TMatrixD covMatrix(2,2);
  covMatrix(0,0) = resolutionX_*resolutionX_;
  covMatrix(0,1) = 0.;
  covMatrix(1,0) = 0.;
  covMatrix(1,1) = resolutionY_*resolutionY_;
  std::auto_ptr<PFMEtSignCovMatrix> covMatrix_ptr(new PFMEtSignCovMatrix(covMatrix));
  evt.put(covMatrix_ptr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ToyMEtSignCovMatrixProducer);
