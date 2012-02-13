#include "TauAnalysis/CandidateTools/plugins/PFMEtSignCovMatrixProducer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

#include <list>

typedef edm::View<reco::Candidate> CandidateView;

PFMEtSignCovMatrixProducer::PFMEtSignCovMatrixProducer(const edm::ParameterSet& cfg) 
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    pfMEtSignInterface_(cfg)
{
  src_ = cfg.getParameter<vInputTag>("src");

  produces<PFMEtSignCovMatrix>();
}

PFMEtSignCovMatrixProducer::~PFMEtSignCovMatrixProducer()
{
// nothing to be done yet...
}

void PFMEtSignCovMatrixProducer::produce(edm::Event& evt, const edm::EventSetup& es) 
{
  std::list<const reco::Candidate*> particles;
  for ( vInputTag::const_iterator src_i = src_.begin();
	src_i != src_.end(); ++src_i ) {
    edm::Handle<CandidateView> particles_i;
    evt.getByLabel(*src_i, particles_i);
    for ( CandidateView::const_iterator particle = particles_i->begin();
	  particle != particles_i->end(); ++particle ) {
      particles.push_back(&(*particle));
    }
  }

  std::auto_ptr<PFMEtSignCovMatrix> covMatrix_ptr(new PFMEtSignCovMatrix(pfMEtSignInterface_(particles)));
  evt.put(covMatrix_ptr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PFMEtSignCovMatrixProducer);
