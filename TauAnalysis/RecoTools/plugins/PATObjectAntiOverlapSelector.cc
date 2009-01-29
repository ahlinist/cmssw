#include "TauAnalysis/RecoTools/plugins/PATObjectAntiOverlapSelector.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h" 
#include "DataFormats/Candidate/interface/Candidate.h" 

#include "PhysicsTools/Utilities/interface/deltaR.h"

template <class T>
PATObjectAntiOverlapSelector<T>::PATObjectAntiOverlapSelector(const edm::ParameterSet& cfg)
{
  srcNotToBeFiltered_ = cfg.getParameter<edm::InputTag>("srcNotToBeFiltered");
  dRmin_ = cfg.getParameter<double>("dRmin");
}

template <class T>
PATObjectAntiOverlapSelector<T>::~PATObjectAntiOverlapSelector()
{
// nothing to be done yet...
}

template <class T>
void PATObjectAntiOverlapSelector<T>::select(const edm::Handle<collection>& particlesToBeFiltered, 
					     const edm::Event& evt, const edm::EventSetup& es)
{
  selected_.clear();
  
  edm::Handle<reco::CandidateView> particlesNotToBeFiltered;
  evt.getByLabel(srcNotToBeFiltered_, particlesNotToBeFiltered);
  std::cout << "srcNotToBeFiltered_ = " << srcNotToBeFiltered_ << std::endl;

  for ( typename collection::const_iterator particleToBeFiltered = particlesToBeFiltered->begin();
	particleToBeFiltered != particlesToBeFiltered->end(); ++particleToBeFiltered ) {
    bool isOverlap = false;
    for ( reco::CandidateView::const_iterator particleNotToBeFiltered = particlesNotToBeFiltered->begin();
	  particleNotToBeFiltered != particlesNotToBeFiltered->end(); ++particleNotToBeFiltered ) {
      double dR = reco::deltaR(particleToBeFiltered->p4(), particleNotToBeFiltered->p4());
      if ( dR < dRmin_ ) isOverlap = true;
    }
    
    if ( !isOverlap) selected_.push_back(&(*particleToBeFiltered)); 
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
