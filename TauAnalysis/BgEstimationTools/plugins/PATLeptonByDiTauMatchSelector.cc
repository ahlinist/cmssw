#include "TauAnalysis/BgEstimationTools/plugins/PATLeptonByDiTauMatchSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Math/interface/deltaR.h"

template<typename T1, typename T2>
PATLeptonByDiTauMatchSelector<T1,T2>::PATLeptonByDiTauMatchSelector(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    produceLeg1_(false),
    produceLeg2_(false)
{
  srcDiTau_ = cfg.getParameter<edm::InputTag>("srcDiTau");

  if ( cfg.exists("srcLeg1") ) {
    srcLeg1_ = cfg.getParameter<edm::InputTag>("srcLeg1");
    dRmatchLeg1_ = cfg.getParameter<double>("dRmatchLeg1");
    produceLeg1_ = true;
  }

  if ( cfg.exists("srcLeg2") ) {
    srcLeg2_ = cfg.getParameter<edm::InputTag>("srcLeg2");
    dRmatchLeg2_ = cfg.getParameter<double>("dRmatchLeg2");
    produceLeg2_ = true;
  }

  if ( produceLeg1_ ) produces<leg1Collection>("");
  if ( produceLeg2_ ) produces<leg2Collection>("");
}

template<typename T1, typename T2>
PATLeptonByDiTauMatchSelector<T1,T2>::~PATLeptonByDiTauMatchSelector() 
{
// nothing to be done yet...
}

template<typename T1, typename T2, typename T3>
void produceLeg(edm::Event& evt, const edm::EventSetup& es, 
		const std::vector<CompositePtrCandidateT1T2MEt<T1,T2> >& diTaus, int legIdx,
		const edm::InputTag& srcLeg, double dRmatchLeg)
{
  typedef std::vector<T3> legCollection;
  std::auto_ptr<legCollection> selectedLegObjects(new legCollection());

  edm::Handle<legCollection> legObjects;
  evt.getByLabel(srcLeg, legObjects);

  for ( typename legCollection::const_iterator legObject = legObjects->begin();
	legObject != legObjects->end(); ++legObject ) {
    bool isMatched = false;
    for ( typename std::vector<CompositePtrCandidateT1T2MEt<T1,T2> >::const_iterator diTau = diTaus.begin();
	  diTau != diTaus.end(); ++diTau ) {
      reco::Candidate::LorentzVector p4Ref;
      if      ( legIdx == 1 ) p4Ref = diTau->leg1()->p4();
      else if ( legIdx == 2 ) p4Ref = diTau->leg2()->p4();
      else assert(0);

      if ( deltaR(legObject->p4(), p4Ref) < dRmatchLeg ) isMatched = true;
    }

    if ( isMatched ) selectedLegObjects->push_back(*legObject);
  }

  evt.put(selectedLegObjects);
}

template<typename T1, typename T2>
void PATLeptonByDiTauMatchSelector<T1,T2>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<PATLeptonByDiTauMatchSelector::produce>:" << std::endl;
  //std::cout << " moduleLabel = " << moduleLabel_ << std::endl;

  edm::Handle<CompositePtrCandidateCollection> diTauCollection;
  evt.getByLabel(srcDiTau_, diTauCollection);

  if ( produceLeg1_ ) produceLeg<T1,T2,T1>(evt, es, *diTauCollection, 1, srcLeg1_, dRmatchLeg1_);
  if ( produceLeg2_ ) produceLeg<T1,T2,T2>(evt, es, *diTauCollection, 2, srcLeg2_, dRmatchLeg2_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATLeptonByDiTauMatchSelector<pat::Electron, pat::Tau> PATLeptonByElecTauPairMatchSelector;
typedef PATLeptonByDiTauMatchSelector<pat::Muon, pat::Tau> PATLeptonByMuTauPairMatchSelector;
typedef PATLeptonByDiTauMatchSelector<pat::Tau, pat::Tau> PATLeptonByDiTauPairMatchSelector;
typedef PATLeptonByDiTauMatchSelector<pat::Electron, pat::Muon> PATLeptonByElecMuPairMatchSelector;

DEFINE_FWK_MODULE(PATLeptonByElecTauPairMatchSelector);
DEFINE_FWK_MODULE(PATLeptonByMuTauPairMatchSelector);
DEFINE_FWK_MODULE(PATLeptonByDiTauPairMatchSelector);
DEFINE_FWK_MODULE(PATLeptonByElecMuPairMatchSelector);



