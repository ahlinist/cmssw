#include "TauAnalysis/RecoTools/plugins/ZtautauVisMassHypothesisT1T2Producer.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "DataFormats/Common/interface/Handle.h"

#include <TMath.h>

template<typename T1, typename T2>
ZtautauVisMassHypothesisT1T2Producer<T1,T2>::ZllHypothesisEntry::ZllHypothesisEntry(const edm::ParameterSet& cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");
  minZllMass_ = cfg.getParameter<double>("minZllMass");
  maxZllMass_ = cfg.getParameter<double>("maxZllMass");
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<typename T1, typename T2>
ZtautauVisMassHypothesisT1T2Producer<T1,T2>::ZtautauVisMassHypothesisT1T2Producer(const edm::ParameterSet& cfg)
{
  srcDiCandidatePairs_ = cfg.getParameter<edm::InputTag>("diCandidatePairSource");
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgZllHypotheses = cfg.getParameter<vParameterSet>("ZllHypotheses");
  for ( vParameterSet::const_iterator cfgZllHypothesis = cfgZllHypotheses.begin(); 
	cfgZllHypothesis != cfgZllHypotheses.end(); ++cfgZllHypothesis ) {
    ZllHypothesisEntries_.push_back(ZllHypothesisEntry(*cfgZllHypothesis));
  }
  
  produces<ZtautauVisMassHypothesisCollection>();
}

template<typename T1, typename T2>
ZtautauVisMassHypothesisT1T2Producer<T1,T2>::~ZtautauVisMassHypothesisT1T2Producer()
{
//--- nothing to be done yet...
}

template<typename T1, typename T2>
void ZtautauVisMassHypothesisT1T2Producer<T1,T2>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<DiCandidatePairCollection> DiCandidatePairs;
  evt.getByLabel(srcDiCandidatePairs_, DiCandidatePairs);

  std::auto_ptr<ZtautauVisMassHypothesisCollection> ZtautauVisMassHypotheses(new ZtautauVisMassHypothesisCollection(DiCandidatePairRefProd(DiCandidatePairs)));

  unsigned numDiCandidatePairs = DiCandidatePairs->size();
  for ( unsigned iDiCandidatePair = 0; iDiCandidatePair < numDiCandidatePairs; ++iDiCandidatePair ) {
    DiCandidatePairRef DiCandidatePair(DiCandidatePairs, iDiCandidatePair);

    bool isZllZllHypothesis = false;
    double mZllBestMatch = -1.;

    for ( typename vZllHypothesisEntry::const_iterator cfgZllHypothesisEntry = ZllHypothesisEntries_.begin();
	  cfgZllHypothesisEntry != ZllHypothesisEntries_.end(); ++cfgZllHypothesisEntry ) {
      edm::Handle<ZllHypothesisCollection> ZllHypotheses;
      evt.getByLabel(cfgZllHypothesisEntry->src_, ZllHypotheses);
      
      for ( typename ZllHypothesisCollection::const_iterator ZllHypothesis = ZllHypotheses->begin();
	    ZllHypothesis != ZllHypotheses->end(); ++ZllHypothesis ) {
	
	if ( ZllHypothesis->diCandidatePair().get() != DiCandidatePair.get() ) continue;

	double mZllHypothesis = ZllHypothesis->p4ZbestMatch().mass();
	if ( mZllHypothesis > cfgZllHypothesisEntry->minZllMass_ && 
	     mZllHypothesis < cfgZllHypothesisEntry->maxZllMass_ ) {
	  
	  //edm::ESHandle<HepPDT::ParticleDataTable> pdt;
	  //es.getData(pdt);
	  //const double nominalZmass = pdt->particle(23)->mass();
	  const double nominalZmass = 91.188;
	  
	  if ( TMath::Abs(mZllHypothesis - nominalZmass) < TMath::Abs(mZllHypothesis - mZllBestMatch) ) mZllBestMatch = mZllHypothesis;
	  isZllZllHypothesis = true;	  
	}
      }
    }

    double visMass = DiCandidatePair->p4Vis().mass();
    double mHypothesis = ( isZllZllHypothesis ) ? mZllBestMatch : visMass;
    
    ZtautauVisMassHypotheses->setValue(iDiCandidatePair, mHypothesis);
  }
  
  evt.put(ZtautauVisMassHypotheses);
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ZtautauVisMassHypothesisT1T2Producer<pat::Electron, pat::Tau> ZtautauVisMassHypothesisElecTauProducer;
typedef ZtautauVisMassHypothesisT1T2Producer<pat::Muon, pat::Tau> ZtautauVisMassHypothesisMuTauProducer;
typedef ZtautauVisMassHypothesisT1T2Producer<pat::Tau, pat::Tau> ZtautauVisMassHypothesisDiTauProducer;
typedef ZtautauVisMassHypothesisT1T2Producer<pat::Electron, pat::Muon> ZtautauVisMassHypothesisElecMuProducer;

DEFINE_ANOTHER_FWK_MODULE(ZtautauVisMassHypothesisElecTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZtautauVisMassHypothesisMuTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZtautauVisMassHypothesisDiTauProducer);
DEFINE_ANOTHER_FWK_MODULE(ZtautauVisMassHypothesisElecMuProducer);





