//
// $Id: PATLeptonRecoilEnergySelector.h,v 1.1 2009/07/29 13:03:39 veelken Exp $
//

#ifndef TauAnalysis_RecoTools_METSignificanceSelector_h
#define TauAnalysis_RecoTools_METSignificanceSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "PhysicsTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"
#include "PhysicsTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "DataFormats/METReco/interface/CaloMET.h"

#include <vector>

typedef SingleObjectSelector<std::vector<reco::CaloMET>,StringCutObjectSelector<reco::CaloMET> > METSignificanceSelector;


#endif
