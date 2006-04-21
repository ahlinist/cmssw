// H4PedestalUtil.h
//
// A utility class to find the best pedestal run for given run-number
// Maybe also other help with pedestals ???
//
// last change : $Date: 2003/08/26 10:53:16 $
// by          : $Author: koblitz $
//
//

#ifndef H4PEDESTAL_UTIL_H
#define H4PEDESTAL_UTIL_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBManager.h"
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"

class H4PedestalUtil: public TObject{
 public:
  H4PedestalUtil();
  H4PedestalUtil(H4DBManager *manager, H4Run *runEntry);
  virtual ~H4PedestalUtil() {};

  /// Get the pedestal run number best suited for a given beam-run
  int getPedestalRun(int run) const;

 private:
  mutable int pedRun ; 
  mutable int prevRqstRunNumber ;   //!< We keep track of the previous request
  H4DBManager *myDBManager;         //!< The DBManager we ask  
  H4Run *myRunEntry;                //!< Whe run entry necessary for searching
  int defaultPedestalRun;           //!< Default pedestal run
  ClassDef(H4PedestalUtil,0)        // A utility class to find pedestal runs
};

// H4PEDESTAL_UTIL_H
#endif

