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

#ifndef HIT_MATCHING_ALGO_BASE_H
#define HIT_MATCHING_ALGO_BASE_H

#include "SimDataFormats/SLHC/interface/LocalStub.h"
#include "SLHCUpgradeSimulations/Utilities/interface/StackedTrackerGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include <sstream>
#include <map>

//using namespace std;

namespace cmsUpgrades{

template<  typename T  >
class HitMatchingAlgorithm {
  public:

    HitMatchingAlgorithm( const cmsUpgrades::StackedTrackerGeometry *i ) : theStackedTracker(i){}

    virtual ~HitMatchingAlgorithm(){}

    virtual std::pair<bool,bool> CheckTwoMemberHitsForCompatibility( const cmsUpgrades::LocalStub<T> &aLocalStub ) const {
      std::pair<bool,bool> a(false,false);
      return a;
    }

    virtual std::string AlgorithmName() const { return ""; }


  protected:
    const cmsUpgrades::StackedTrackerGeometry *theStackedTracker;
};


}

#endif

