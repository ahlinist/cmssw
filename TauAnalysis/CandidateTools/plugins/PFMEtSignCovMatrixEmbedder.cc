/*
 * =====================================================================================
 *
 *       Filename:  PFMEtSignCovMatrixEmbedder.cc
 *
 *    Description:  Embeds covariance matrices into reco/pat mets.
 *
 *         Author:  Evan Friis (), evan.friis@cern.ch
 *        Company:  UW Madison
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PFMEtSignCovMatrix.h"

template<class T>
class PFMEtSignCovMatrixEmbedder : public edm::EDProducer {
  public:
    typedef std::vector<T> Collection;
    PFMEtSignCovMatrixEmbedder(const edm::ParameterSet& pset);
    virtual ~PFMEtSignCovMatrixEmbedder(){}
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::InputTag src_;
    edm::InputTag srcCov_;
};

template<class T>
PFMEtSignCovMatrixEmbedder<T>::PFMEtSignCovMatrixEmbedder(const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  srcCov_ = pset.getParameter<edm::InputTag>("srcCov");
  produces<Collection>();
}

template<class T>
void PFMEtSignCovMatrixEmbedder<T>::produce(edm::Event& evt, const edm::EventSetup& es) {
  std::auto_ptr<Collection> output(new Collection);

  edm::Handle<edm::View<T> > met;
  evt.getByLabel(src_, met);

  edm::Handle<PFMEtSignCovMatrix> cov;
  evt.getByLabel(srcCov_, cov);

  output->reserve(met->size());

  for (size_t i = 0; i < met->size(); ++i) {
    // Make a local copy
    const T& theMET = met->at(i);
    T myMET = theMET;
    myMET.setSignificanceMatrix(*cov);
    output->push_back(myMET);
  }

  evt.put(output);
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/METReco/interface/MET.h"
typedef PFMEtSignCovMatrixEmbedder<reco::MET> RecoPFMEtSignCovMatrixEmbedder;
DEFINE_FWK_MODULE(RecoPFMEtSignCovMatrixEmbedder);

#include "DataFormats/PatCandidates/interface/MET.h"
typedef PFMEtSignCovMatrixEmbedder<pat::MET> PatPFMEtSignCovMatrixEmbedder;
DEFINE_FWK_MODULE(PatPFMEtSignCovMatrixEmbedder);
