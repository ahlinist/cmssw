// -*- C++ -*-
// 
// H4PedestalMgr.h
// 
// The crystal pedestals and PNDiode pedestals happen to be stored
// in the same TTree. This class keeps handles to the TFile and TTree
// containing the Pedestals, so that H4TowerPedestals and H4PNPedestals
// can smoothly share access to the PedestalTree.
// 
// last modified : $Date: 2003/08/08 08:56:04 $
// by            : $Author: koblitz $
// 
#ifndef H4PEDESTALMGR_H
#define H4PEDESTALMGR_H

#include <string>

#include <TObject.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>


class H4PedestalMgr : public TObject {
 public:
  //! Constructor (does nothing)
  H4PedestalMgr();
  
  //! Destructor
  ~H4PedestalMgr();
  
 protected:
  //! Constructor 
  H4PedestalMgr(std::string const& fileName, std::string const& treeName);
  
  static std::string fileName_;
  static std::string treeName_;
  static int count_;
  static TFile* pedestalFile_;
  static TTree* pedestalTree_;
  
  ClassDef(H4PedestalMgr,1)
};

#endif

