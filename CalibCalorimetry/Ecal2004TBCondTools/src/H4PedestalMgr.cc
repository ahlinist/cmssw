// -*- C++ -*-
// 
// H4PedestalMgr.C
// 
// 
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalMgr.h"
#include <iostream>

ClassImp(H4PedestalMgr)

H4PedestalMgr::H4PedestalMgr()
  : TObject()
{
  count_++;
}


H4PedestalMgr::H4PedestalMgr(std::string const& fileName,
			     std::string const& treeName)
  : TObject()
{
  count_++;
  if(pedestalFile_) return;
  fileName_ = fileName;
  treeName_ = treeName;
  pedestalFile_ = new TFile(fileName_.c_str(), "READ");
  if(!pedestalFile_) 
    std::cout << "H4PedestalMgr::H4PedestalMgr: ERROR, unable to open pedestal file "
	      << fileName_ << " for reading" << std::endl;
  
  pedestalTree_ = (TTree*)pedestalFile_->Get(treeName_.c_str());
  
  // FIXME: hardcoded 
  if(!pedestalTree_) 
    std::cout << "H4PedestalMgr::H4PedestalMgr: ERROR, unable to load pedestal Tree: "
	      << treeName_ << " from file " << fileName_ << std::endl;
  else 
    pedestalTree_->BuildIndex("PN_Peds.fRunNumber", "PN_Peds.fRunNumber");
}


H4PedestalMgr::~H4PedestalMgr()
{
  count_--;
  if(count_==0 && pedestalFile_){
    pedestalFile_->Close();
    delete pedestalFile_;
    pedestalFile_=0;
    // pedestalTree automagically deleted, I guess
  }
}

std::string  H4PedestalMgr::fileName_="";
std::string  H4PedestalMgr::treeName_="";
int          H4PedestalMgr::count_ = 0;
TFile*       H4PedestalMgr::pedestalFile_=0;
TTree*       H4PedestalMgr::pedestalTree_=0;

