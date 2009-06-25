
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

typedef SingleObjectSelector<
            edm::View<reco::MET>, 
            StringCutObjectSelector<reco::MET>,
            reco::METCollection
          > CaloMEtSelector;

typedef SingleObjectSelector<
            edm::View<reco::PFMET>, 
            StringCutObjectSelector<reco::PFMET>,
            reco::PFMETCollection
          > PFMEtSelector;

#include "FWCore/Framework/interface/MakerMacros.h"
 
DEFINE_ANOTHER_FWK_MODULE(CaloMEtSelector);
DEFINE_ANOTHER_FWK_MODULE(PFMEtSelector);
