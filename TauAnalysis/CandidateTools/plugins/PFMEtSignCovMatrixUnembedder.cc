#include "TauAnalysis/CandidateTools/plugins/PFMEtSignCovMatrixUnembedder.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <list>

PFMEtSignCovMatrixUnembedder::PFMEtSignCovMatrixUnembedder(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  produces<PFMEtSignCovMatrix>();
}

PFMEtSignCovMatrixUnembedder::~PFMEtSignCovMatrixUnembedder()
{
// nothing to be done yet...
}

void PFMEtSignCovMatrixUnembedder::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  typedef edm::View<reco::MET> METView;
  edm::Handle<METView> met;
  evt.getByLabel(src_, met);
  if ( met->size() != 1 ) 
    throw cms::Exception("PFMEtSignCovMatrixUnembedder::produce") 
      << "Failed to find unique MET object !!\n";

  std::auto_ptr<PFMEtSignCovMatrix> covMatrix_ptr(new PFMEtSignCovMatrix(met->front().getSignificanceMatrix()));
  evt.put(covMatrix_ptr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFMEtSignCovMatrixUnembedder);
