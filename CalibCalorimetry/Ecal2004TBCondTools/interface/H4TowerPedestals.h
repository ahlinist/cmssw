// -*- C++ -*-
// 
// H4TowerPedestals.h 
// 
// Class which holds the Pedestals from the Pedestal Files. 
// Contains pointers to instances of the original TowerPedestals class
// by J. Bourotte et al. 
// 
// It would be great if the pedestal values could be stored in the SQL Database
// and accessed through the H4DBManager. I'll modify the code as soon as 
// this is the case. For the moment, H4TowerPedestals.h is only an interface to
// the TTree provided by J. Bourotte et al. 
// 
// I also try to manage the crystal numbering as soon as I understand it...
// 
// last change : $Date: 2003/08/28 11:15:51 $
// by          : $Author: koblitz $
// 
#ifndef H4TOWERPEDESTALS_H
#define H4TOWERPEDESTALS_H

#include <vector>
#include <string>
#include <map>
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalMgr.h"

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/TTowerPeds.h"


class H4TowerPedestals : public H4PedestalMgr {
public:
  enum { NoSuchRunNumber = -100 } ; 
  
  //! Constructor: 
  //  H4TowerPedestals(TTree* tree, vector<string> const& branchNames) ;
  
  //! Constructor w/ its own copy of the TTree 
  H4TowerPedestals(std::string const& filename, std::string const& treeName, 
		   std::vector<int> const& smTowerNumbers,
		   std::string const& branchNamePrefix="Tower_Peds_",
		   std::string const& branchNameSuffix=".");
  
  //! Destructor: does nothing. 
  virtual ~H4TowerPedestals() ;
  
  //! Select the current pedestal run
  bool            selectPedestalRun(int run) ; 
  
  //! Get the mean pedestal, determined on the current pedestal run
  double          getMean(int smTower, int crystal, int gain) const ;
  
  //! Get the RMS, determined on the current pedestal run
  double          getRMS(int smTower, int  crystal, int gain) const ;
  
  //! Get the low frequency RMS, determined on the current? pedestal run 
  double          getLowFrequencyRMS(int smTower, int crystal, int gain) const ;
  
  //! Get the number of "peds used for computation" (?) --I'll ask J.Bourotte what this is exactly
  int             getNEntries(int smTower, int crystal, int gain) const ;
  
  //! Get the current run number 
  int             getCurrentRunNumber() const { return runNumber ; }
  
  //! Number of towers 
  int             getNTowers() const { return nTowers ; } 
  
  //! Number of crystals / tower 
  int             getNCrystals() const { return nCrystals ; }
  
  //! return the Supermodule tower number of the tower number i
  int             getSMTowerNumber(int i) const;
  
  //! return the tower index of the tower number smTower
  int             getTowerNumber(int smTower) const;

  //  Copy the mapping form a branch number on pedestal file to SM tower
  //  number to another SM number
  int             copySMTowerMapping(int smTower, int smTowerCopy);

  //! Print out information 
  virtual void    Print(const char *opt=0) const ; 
  
private:
  int    curr_idx ;
  int    runNumber ;
  int    nCrystals ;
  int    nTowers ;
  
  //  TTree* pedestalTree ; 
  //  bool   ownTree ;
  //  TFile* tf ;
  std::vector<TBranch*>         branches ;
  std::vector<TTowerPeds*>      tpedestals ;
  std::vector<std::string>      branchNames ;
  std::vector<int>              smTowerNumber; // smTowerNumbers[towerIndex]
  std::map<int,int>             towerIndex;    // towerIndex[smTower]
  
  ClassDef(H4TowerPedestals,0)  
} ;


#endif

