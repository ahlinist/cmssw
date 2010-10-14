/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
/// Written by:                          ///
/// Andrew W. Rose                       ///
/// 2008                                 ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, May                            ///
///                                      ///
/// Added features:                      ///
/// Possibility to have a flag telling   ///
/// if the Stub is compatible with a     ///
/// higher Pt threshold than the default ///
/// one. Just performs the cross check   ///
/// twice.                               ///
/// ////////////////////////////////////////

#ifndef HIT_MATCHING_ALGO_RECORD_H
#define HIT_MATCHING_ALGO_RECORD_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometryRecord.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "boost/mpl/vector.hpp"

namespace cmsUpgrades{
class HitMatchingAlgorithmRecord : public edm::eventsetup::DependentRecordImplementation< cmsUpgrades::HitMatchingAlgorithmRecord , boost::mpl::vector<StackedTrackerGeometryRecord , IdealMagneticFieldRecord> > {};
}

#endif

