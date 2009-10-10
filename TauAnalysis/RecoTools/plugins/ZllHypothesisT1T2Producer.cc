#include "TauAnalysis/RecoTools/plugins/ZllHypothesisT1T2Producer.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"

#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2.h"
#include "AnalysisDataFormats/TauAnalysis/interface/ZllHypothesisT1T2Fwd.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

template<typename T1, typename T2>
ZllHypothesisT1T2Producer<T1,T2>::ZllHypothesisT1T2Producer(const edm::ParameterSet& cfg)
  : algorithm_(cfg)
{
  srcDiCandidatePairs_ = cfg.getParameter<edm::InputTag>("diCandidatePairSource");

  typedef std::vector<ZllHypothesisT1T2<T1,T2> > ZllHypothesisCollection;
  produces<ZllHypothesisCollection>("");
}

template<typename T1, typename T2>
ZllHypothesisT1T2Producer<T1,T2>::~ZllHypothesisT1T2Producer()
{
//--- nothing to be done yet...
}

template<typename T1, typename T2>
void ZllHypothesisT1T2Producer<T1,T2>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<edm::View<CompositePtrCandidateT1T2MEt<T1,T2> > > diCandidatePairs;
  evt.getByLabel(srcDiCandidatePairs_, diCandidatePairs);

  typedef std::vector<ZllHypothesisT1T2<T1,T2> > ZllHypothesisCollection;
  std::auto_ptr<ZllHypothesisCollection> ZllHypotheses(new ZllHypothesisCollection());
  
  for ( unsigned idxDiCandidatePair = 0, numDiCandidatePairs = diCandidatePairs->size(); 
	idxDiCandidatePair < numDiCandidatePairs; ++idxDiCandidatePair ) {
    edm::Ptr<CompositePtrCandidateT1T2MEt<T1,T2> > diCandidatePair = diCandidatePairs->ptrAt(idxDiCandidatePair);

    ZllHypothesisT1T2<T1,T2> ZllHypothesis = algorithm_.buildZllHypothesis(diCandidatePair, evt, es);
    ZllHypotheses->push_back(ZllHypothesis);
  }

  evt.put(ZllHypotheses);
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ZllHypothesisT1T2Producer<pat::Electron, pat::Tau> ZllHypothesisElecTauProducer;
typedef ZllHypothesisT1T2Producer<pat::Muon, pat::Tau> ZllHypothesisMuTauProducer;
typedef ZllHypothesisT1T2Producer<pat::Tau, pat::Tau> ZllHypothesisDiTauProducer;
typedef ZllHypothesisT1T2Producer<pat::Electron, pat::Muon> ZllHypothesisElecMuProducer;

DEFINE_ANOTHER_FWK_MODULE(ZllHypothesisElecTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZllHypothesisMuTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZllHypothesisDiTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZllHypothesisElecMuProducer);




