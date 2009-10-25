#include "TauAnalysis/RecoTools/plugins/PATObjectAntiOverlapSelector.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 

#include "DataFormats/Math/interface/deltaR.h"

template <class T>
PATObjectAntiOverlapSelector<T>::PATObjectAntiOverlapSelector(const edm::ParameterSet& cfg)
{
  srcNotToBeFiltered_ = cfg.getParameter<vInputTag>("srcNotToBeFiltered");
  dRmin_ = cfg.getParameter<double>("dRmin");
  invert_ = cfg.getUntrackedParameter<bool>("invert",false);
}

template <class T>
void PATObjectAntiOverlapSelector<T>::select(const edm::Handle<collection>& particlesToBeFiltered, 
					     const edm::Event& evt, const edm::EventSetup& es)
{
  selected_.clear();

  std::vector<bool> isOverlap(particlesToBeFiltered->size());

  for ( vInputTag::const_iterator it = srcNotToBeFiltered_.begin();
	it != srcNotToBeFiltered_.end(); ++it ) {

    edm::Handle<reco::CandidateView> particlesNotToBeFiltered;
    evt.getByLabel(*it, particlesNotToBeFiltered);

    for ( reco::CandidateView::const_iterator particleNotToBeFiltered = particlesNotToBeFiltered->begin();
	  particleNotToBeFiltered != particlesNotToBeFiltered->end(); ++particleNotToBeFiltered ) {

      int particleToBeFilteredIndex = 0;
      for ( typename collection::const_iterator particleToBeFiltered = particlesToBeFiltered->begin();
	    particleToBeFiltered != particlesToBeFiltered->end(); ++particleToBeFiltered, ++particleToBeFilteredIndex ) {

	double dR = reco::deltaR(particleToBeFiltered->p4(), particleNotToBeFiltered->p4());

	if ( dR < dRmin_ ) isOverlap[particleToBeFilteredIndex] = true;
      }
    }
  }
  
  int particleToBeFilteredIndex = 0;
  for ( typename collection::const_iterator particleToBeFiltered = particlesToBeFiltered->begin();
	particleToBeFiltered != particlesToBeFiltered->end(); ++particleToBeFiltered, ++particleToBeFilteredIndex ) {

    if(!invert_){
      if ( !isOverlap[particleToBeFilteredIndex] ) selected_.push_back(&(*particleToBeFiltered)); 
    }
    else {
      if ( isOverlap[particleToBeFilteredIndex] ) selected_.push_back(&(*particleToBeFiltered)); 
    }

  }
}

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Electron> > PATElectronAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Muon> > PATMuonAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Tau> > PATTauAntiOverlapSelector;
typedef ObjectSelector<PATObjectAntiOverlapSelector<pat::Jet> > PATJetAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATElectronAntiOverlapSelector);
DEFINE_ANOTHER_FWK_MODULE(PATMuonAntiOverlapSelector);
DEFINE_ANOTHER_FWK_MODULE(PATTauAntiOverlapSelector);
DEFINE_ANOTHER_FWK_MODULE(PATJetAntiOverlapSelector);
