#include "TauAnalysis/RecoTools/plugins/PATMuonAntiPionSelector.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"

PATMuonAntiPionSelectorImp::PATMuonAntiPionSelectorImp(const edm::ParameterSet& cfg )
{ 
  coeffCaloComp_ = cfg.getParameter<double>("CaloCompCoefficient");
  coeffSegmComp_ = cfg.getParameter<double>("SegmCompCoefficient");
  cut_ = cfg.getParameter<double>("AntiPionCut");
}

void PATMuonAntiPionSelectorImp::select(const edm::Handle<collection>& patMuonCollection,
					edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  for ( collection::const_iterator patMuon = patMuonCollection->begin();
	patMuon != patMuonCollection->end(); ++patMuon ) {
    double discriminant = coeffCaloComp_*muon::caloCompatibility(*patMuon) 
                         + coeffSegmComp_*muon::segmentCompatibility(*patMuon);
    if ( discriminant > cut_ ) {
      selected_.push_back(&(*patMuon));
    }
  }
}

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<PATMuonAntiPionSelectorImp> PATMuonAntiPionSelector;

DEFINE_ANOTHER_FWK_MODULE(PATMuonAntiPionSelector);


