// -*- C++ -*-
// 
// H4PNPedestals.h
// 
// last change : $Date: 2003/08/08 08:56:04 $
// by          : $Author: koblitz $
// 
#ifndef H4PNPEDESTALS_H
#define H4PNPEDESTALS_H

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalMgr.h"

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/TPNsPeds.h"

class H4PNPedestals : public H4PedestalMgr {
 public:
  H4PNPedestals(std::string const& filename, std::string const& treeName,
		std::string const& branchName="PN_Peds.");
  
  //! Destructor
  ~H4PNPedestals();
  
  //! Select the current pedestal run
  bool    selectPedestalRun(int run);
  
  //! Number of PN diodes read out 
  int     getNPNDiodes() const { return nPNDiodes_; }
  
  //! Number of gain channels
  int     getNGains() const { return nGains_; }
  
  //! Get the mean pedestal, determined on the current pedestal run
  double  getMean(int pn, int gain) const;
  
  //! Get the RMS, determined on the current pedestal run
  double  getRMS(int pn, int gain) const;
  
  //! Get the low frequency RMS
  double  getLowFrequencyRMS(int pn, int gain) const;
  
  //! Get the number "number of peds used for computation"
  int     getNEntries(int pn, int gain) const;
  
  //! get the current pedestal run number
  int     getCurrentRunNumber() const { return runNumber_ ; }
  
  //! Print out data
  void    print() const;
  
private:
  int         runNumber_;
  int         curr_idx_;
  int         nPNDiodes_;
  int         nGains_;
  std::string branchName_;
  TBranch*   branch_;
  TPNsPeds*  tpnpedestals_;
  
  ClassDef(H4PNPedestals,0)
};


#endif


