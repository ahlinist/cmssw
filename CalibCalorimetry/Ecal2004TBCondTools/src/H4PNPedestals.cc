// -*- C++ -*-
// 
// H4PNPedestals.C
// 
// last change : $Date: 2003/09/24 09:10:18 $
// by          : $Author: nrl $
// 
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PNPedestals.h"
#include <iostream>

ClassImp(H4PNPedestals)


H4PNPedestals::H4PNPedestals(std::string const& filename,
			     std::string const& treeName,
			     std::string const& branchName)
  : H4PedestalMgr(filename, treeName), runNumber_(-100), curr_idx_(-100),
    nPNDiodes_(N_PN), // FIXME: hardcoded in JB's code
    nGains_(2),       // FIXME: hardcoded
    branchName_(branchName), tpnpedestals_(new TPNsPeds)
{
  if(!pedestalTree_) return;
  branch_ = pedestalTree_->GetBranch(branchName_.c_str());
  if(!branch_){
    std::cout << "H4PNPedestals::H4PNPedestals ERROR loading branch "
	      << branchName_ << " from tree " << treeName_ << std::endl;
    return;
  }
  branch_->SetAddress(&tpnpedestals_);
}


H4PNPedestals::~H4PNPedestals()
{
  // delete branch_ (?)
  // delete tpnpedestals_; (?)
}


bool H4PNPedestals::selectPedestalRun(int run)
{
  curr_idx_ = pedestalTree_->GetEntryNumberWithIndex(run,run);
  if(curr_idx_ == -1) {
    runNumber_ = -100;
    return false;
  }
  runNumber_ = run;
  pedestalTree_->GetEntry(curr_idx_);
  
  tpnpedestals_->SwapGain0and1();

  return true;
}


double H4PNPedestals::getMean(int pn, int gain) const
{
#ifdef H4_BOUNDS_CHECK
  if(runNumber_ == -100) {
    std::cout << "H4PNPedestals::getMean: no run selected." << std::endl;
    abort();
  }
  if(pn<0 || pn>=nPNDiodes_) {
    std::cout << "H4PNPedestals::getMean: Invalid PNDiode number="
	      << pn << std::endl;
    return 0;
  }
  if(gain<0 || gain>=nGains_) {
    std::cout << "H4PNPedestals::getMean: Invalid Gain number="
	      << gain << std::endl;
    return 0;
  }
#endif
  return tpnpedestals_->fMeans[pn][gain];
}


double H4PNPedestals::getRMS(int pn, int gain) const
{
#ifdef H4_BOUNDS_CHECK
  if(runNumber_ == -100) {
    std::cout << "H4PNPedestals::getRMS: no run selected." << std::endl;
    abort();
  }
  if(pn<0 || pn>=nPNDiodes_) {
    std::cout << "H4PNPedestals::getRMS: Invalid PNDiode number="
	      << pn << std::endl;
    return 0;
  }
  if(gain<0 || gain>=nGains_) {
    std::cout << "H4PNPedestals::getRMS: Invalid Gain number="
	      << gain << std::endl;
    return 0;
  }
#endif
  return tpnpedestals_->fSigTot[pn][gain];
}


double H4PNPedestals::getLowFrequencyRMS(int pn, int gain) const
{
#ifdef H4_BOUNDS_CHECK
  if(runNumber_ == -100) {
    std::cout << "H4PNPedestals::getLowFrequencyRMS: no run selected." 
	      << std::endl;
    abort();
  }
  if(pn<0 || pn>=nPNDiodes_) {
    std::cout << "H4PNPedestals::getLowFrequencyRMS: Invalid PNDiode number="
	      << pn << std::endl;
    return 0;
  }
  if(gain<0 || gain>=nGains_) {
    std::cout << "H4PNPedestals::getLowFrequencyRMS: Invalid Gain number="
	      << gain << std::endl;
    return 0;
  }
#endif
  return tpnpedestals_->fSigLF[pn][gain];
}


int H4PNPedestals::getNEntries(int pn, int gain) const
{
#ifdef H4_BOUNDS_CHECK
  if(runNumber_ == -100) {
    std::cout << "H4PNPedestals::getNEntries: no run selected." << std::endl;
    abort();
  }
  if(pn<0 || pn>=nPNDiodes_) {
    std::cout << "H4PNPedestals::getNEntries: Invalid PNDiode number="
	      << pn << std::endl;
    return 0;
  }
  if(gain<0 || gain>=nGains_) {
    std::cout << "H4PNPedestals::getNEntries: Invalid Gain number="
	      << gain << std::endl;
    return 0;
  }
#endif
  return tpnpedestals_->fEntries[pn][gain];
}


void H4PNPedestals::print() const
{
  std::cout << "H4PNPedestals::print " << std::endl;
  std::cout << " runNumber = " << runNumber_ 
            << ", nPNDiodes = " << nPNDiodes_
            << ", nGains = " << nGains_ << std::endl;
}

