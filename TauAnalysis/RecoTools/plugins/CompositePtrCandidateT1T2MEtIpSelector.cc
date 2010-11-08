#include "TauAnalysis/RecoTools/plugins/CompositePtrCandidateT1T2MEtIpSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include <TMath.h>

template <typename T1, typename T2>
CompositePtrCandidateT1T2MEtIpSelector<T1,T2>::CompositePtrCandidateT1T2MEtIpSelector(const edm::ParameterSet& cfg) 
{
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");

  if ( cfg.exists("IpSigMin") ) {
    ipSigMin_ = cfg.getParameter<double>("IpSigMin");
    applyIpSigMin_ = true;
  } else {
    applyIpSigMin_ = false;
  }

  if ( cfg.exists("IpSigMax") ) {
    ipSigMax_ = cfg.getParameter<double>("IpSigMax");
    applyIpSigMax_ = true;
  } else {
    applyIpSigMax_ = false;
  }
}

template <typename T1, typename T2>
CompositePtrCandidateT1T2MEtIpSelector<T1,T2>::~CompositePtrCandidateT1T2MEtIpSelector() 
{
//--- nothing to be done yet...
}

template <typename T1, typename T2>
void CompositePtrCandidateT1T2MEtIpSelector<T1,T2>::select(const edm::Handle<collection>& diTauCollection,
							   edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();
  
  edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
  evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
  if ( primaryEventVertexCollection->size() < 1 ) {
    return;
  } else if ( primaryEventVertexCollection->size() > 1 ) {
    edm::LogError ("CompositePtrCandidateT1T2MEtIpSelector::select") << " Cannot have more than one primary event vertex --> skipping !!";
    return;
  } 

  const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

  for ( typename collection::const_iterator diTauCandidate = diTauCollection->begin(); 
	diTauCandidate != diTauCollection->end(); ++diTauCandidate ) {
    
    const reco::Track* trackLeg1 = trackExtractorLeg1_(*diTauCandidate->leg1());
    const reco::Track* trackLeg2 = trackExtractorLeg2_(*diTauCandidate->leg2());
    if ( trackLeg1 && trackLeg2 ) {

      double trackLeg1IpSig = trackLeg1->dxy(thePrimaryEventVertex.position())/trackLeg1->dxyError();
      double trackLeg2IpSig = trackLeg2->dxy(thePrimaryEventVertex.position())/trackLeg2->dxyError();

      double combinedIpSig = TMath::Sqrt(trackLeg1IpSig*trackLeg1IpSig + trackLeg2IpSig*trackLeg2IpSig);

      if ( (!applyIpSigMin_ || combinedIpSig > ipSigMin_) &&
	   (!applyIpSigMax_ || combinedIpSig < ipSigMax_) ) {
	selected_.push_back(&(*diTauCandidate));
      }
    }
  }
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Electron, pat::Tau> > PATElecTauPairIpSelector;
typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Muon, pat::Tau> > PATMuTauPairIpSelector;
typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Tau, pat::Tau> > PATDiTauPairIpSelector;
typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Electron, pat::Muon> > PATElecMuPairIpSelector;
typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Electron, pat::Electron> > PATDiElecPairIpSelector;
typedef ObjectSelector<CompositePtrCandidateT1T2MEtIpSelector<pat::Muon, pat::Muon> > PATDiMuPairIpSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElecTauPairIpSelector);
DEFINE_FWK_MODULE(PATMuTauPairIpSelector);
DEFINE_FWK_MODULE(PATDiTauPairIpSelector);
DEFINE_FWK_MODULE(PATElecMuPairIpSelector);
