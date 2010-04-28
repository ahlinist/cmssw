//
// $Id: METSignificanceSelector.h,v 1.1 2010/01/29 15:16:42 liis Exp $
//

#ifndef TauAnalysis_RecoTools_METSignificanceSelector_h
#define TauAnalysis_RecoTools_METSignificanceSelector_h

#include "DataFormats/Common/interface/RefVector.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SingleElementCollectionSelector.h"

#include "DataFormats/METReco/interface/CaloMET.h"

#include <vector>

typedef SingleObjectSelector<std::vector<reco::CaloMET>,StringCutObjectSelector<reco::CaloMET> > METSignificanceSelector;


#endif
