// -*- C++ -*-
// 
// H4TowerPedestals.cxx 
// 
// last change : $Date: 2005/06/17 17:15:02 $
// by          : $Author: brunelie $
// 
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TowerPedestals.h"

ClassImp(H4TowerPedestals) 

using namespace std ; 

H4TowerPedestals::H4TowerPedestals(string const& filename, 
				   string const& treeName,
				   std::vector<int> const& smTowerNumbers,
				   string const& branchNamePrefix, 
				   string const& branchNameSuffix)
  : H4PedestalMgr(filename, treeName), curr_idx(0),
    runNumber(NoSuchRunNumber), nCrystals(0), nTowers(0)
{
  if(!pedestalTree_) return;
  
  unsigned int i;
  nTowers = (int)smTowerNumbers.size();
  for(i=0;i<smTowerNumbers.size();i++){
    stringstream sstrm;
    // sstrm.fill('0');
    // sstrm << branchNamePrefix
    //	  << setw(2)
    //	  << smTowerNumbers[i] 
    //	  << branchNameSuffix;
    sstrm << branchNamePrefix
	  << smTowerNumbers[i] 
	  << branchNameSuffix;
    branchNames.push_back(sstrm.str());
    std::cout << " *** " << sstrm.str() << std::endl;
  }
  
  if(nTowers == 0) {
    std::cout << "H4TowerPedestals::H4TowerPedestals WARNING: No input names !" << std::endl ;
    return;
  }
  
  copy(smTowerNumbers.begin(), smTowerNumbers.end(), back_inserter(smTowerNumber));
  for(i=0;i<smTowerNumbers.size();i++)
    towerIndex[smTowerNumbers[i]] = i;
  
  branches.resize(nTowers) ;
  tpedestals.resize(nTowers) ;
  for(i=0;i<tpedestals.size();i++)
    tpedestals[i] = 0 ;
  
  i = 0 ;
  std::vector<string>::iterator it = branchNames.begin() ; 
  while( it != branchNames.end() ) {
    branches[i] = pedestalTree_->GetBranch( it->c_str() ) ;
    if(!branches[i]) {
      std::cout << "H4TowerPedestals::H4TowerPedestals WARNING: TBranch "
		<< branchNames[i] << " does not exist." << std::endl ;
      it++ ; i++ ;
      continue;
    }
    branches[i]->SetAddress(&(tpedestals[i])) ;
    it++ ; i++ ;
  }
  nCrystals = tpedestals[0]->GetSize() ;
}


H4TowerPedestals::~H4TowerPedestals()
{
  // Should we delete this ourselves ? 
  // What will happen then, if pedestalTree->GetEntry() is called ? 
  //  unsigned int i ;
  //  for(i=0;i<tpedestals.size();i++) 
  //    delete tpedestals[i] ;
}


bool H4TowerPedestals::selectPedestalRun(int run) 
{
  if(run==runNumber) return true;
  curr_idx = pedestalTree_->GetEntryNumberWithIndex(run, run) ;
  if(curr_idx == -1) {
    runNumber = -100 ;
    return false ;
  }
  runNumber = run ;
  pedestalTree_->GetEntry(curr_idx) ;
  return true ;
}


double H4TowerPedestals::getMean(int smTower, int crystal, int gain) const
{
  int tower = getTowerNumber(smTower);
  // Maybe a assert() + exception handler would be better... 
#ifdef H4_BOUNDS_CHECK
  if(runNumber == -100) {
    std::cout << "H4TowerPedestals::getMean: no run selected." << std::endl ;
    return 0;
    //    abort() ;
  }
  if(tower<0 || tower>=nTowers) {
    std::cout << "H4TowerPedestals::getMean: Invalid Tower number=" 
	      << tower << std::endl ;
    return 0;
  }
  if(crystal<0 || crystal>=nCrystals) {
    std::cout << "H4TowerPedestals::getMean: Invalid crystal number=" 
	      << crystal << std::endl ;
    return 0;
  }
  if(gain<0 || gain>=4) { // $Change this$
    std::cout << "H4TowerPedestals::getMean: Invalid gain number=" 
	      << gain << std::endl ;
    return 0;
  }
#endif
  if(! tpedestals[tower] ) return 0;
//   if (!tpedestals[tower] || tpedestals[tower]->fMeans[crystal][gain] <= 0.) {
//     float pedestalMean = 0.;
//     int entry = curr_idx - 1;
//     while (pedestalMean <= 0. && entry >= 0) {
//       pedestalTree_->GetEntry(entry);
//       entry--;
//       if (!tpedestals[tower]) continue;
//       pedestalMean = tpedestals[tower]->fMeans[crystal][gain];
//     }      
//     pedestalTree_->GetEntry(curr_idx);
//     return pedestalMean;
//   }      
  return tpedestals[tower]->fMeans[crystal][gain] ;
}


double H4TowerPedestals::getRMS(int smTower, int crystal, int gain) const
{
  int tower = getTowerNumber(smTower);
#ifdef H4_BOUNDS_CHECK
  if(runNumber == -100) {
    std::cout << "H4TowerPedestals::getRMS: no run selected." << std::endl ;
    return 0;
    //    abort();
  }
  if(tower<0 || tower>=nTowers) {
    std::cout << "H4TowerPedestals::getRMS: Invalid Tower number=" 
	      << tower << std::endl ;
    return 0;
  }
  if(crystal<0 || crystal>=nCrystals) {
    std::cout << "H4TowerPedestals::getRMS: Invalid crystal number=" 
	      << crystal << std::endl ;
    return 0;
  }
  if(gain<0 || gain>=4) { // $Change this$
    std::cout << "H4TowerPedestals::getRMS: Invalid gain number=" 
	      << gain << std::endl ;
    return 0;
  }
#endif
  if(! tpedestals[tower] ) return 0;
  return tpedestals[tower]->fSigTot[crystal][gain] ;
}


double H4TowerPedestals::getLowFrequencyRMS(int smTower, int crystal, int gain) const
{
  int tower = getTowerNumber(smTower);
#ifdef H4_BOUNDS_CHECK
  if(runNumber == -100) {
    std::cout << "H4TowerPedestals::getLowFrequencyRMS: no run selected." << std::endl ;
    return 0;
    //    abort() ;
  }
  if(tower<0 || tower>=nTowers) {
    std::cout << "H4TowerPedestals::getLowFrequencyRMS: Invalid Tower number=" 
	      << tower << std::endl ;
    return 0;
  }
  if(crystal<0 || crystal>=nCrystals) {
    std::cout << "H4TowerPedestals::getLowFrequencyRMS: Invalid crystal number=" 
	      << crystal << std::endl ;
    return 0;
  }
  if(gain<0 || gain>=4) { // $Change this$
    std::cout << "H4TowerPedestals::getLowFrequencyRMS: Invalid gain number=" 
	      << gain << std::endl ;
    return 0;
  }
#endif
  if(! tpedestals[tower] ) return 0;
  return tpedestals[tower]->fSigLF[crystal][gain] ;
}


int H4TowerPedestals::getNEntries(int smTower, int crystal, int gain) const
{
  int tower = getTowerNumber(smTower);
#ifdef H4_BOUNDS_CHECK
  if(runNumber == -100) {
    std::cout << "H4TowerPedestals::getNEntries: no run selected." << std::endl ;
    return 0;
    //    abort() ;
  }
  if(tower<0 || tower>=nTowers) {
    std::cout << "H4TowerPedestals::getNEntries: Invalid Tower number=" 
	      << tower << std::endl ;
    return 0;
  }
  if(crystal<0 || crystal>=nCrystals) {
    std::cout << "H4TowerPedestals::getNEntries: Invalid crystal number=" 
	      << crystal << std::endl ;
    return 0;
  }
  if(gain<0 || gain>=4) { // $Change this$
    std::cout << "H4TowerPedestals::getNEntries: Invalid gain number=" 
	      << gain << std::endl ;
    return 0;
  }
#endif
  if(! tpedestals[tower] ) return 0;
  return tpedestals[tower]->fEntries[crystal][gain] ;
}


int  H4TowerPedestals::getSMTowerNumber(int i) const
{
#ifdef H4_BOUNDS_CHECK
  if(i<0 || i>=nTowers){
    std::cout << "H4TowerPedestals::getSMTowerNumber: index out of range!"
	      << " i=" << i << ", nTowers=" << nTowers << std::endl;
    return -1;
  }
#endif
  return smTowerNumber[i];
}


int  H4TowerPedestals::getTowerNumber(int smTower) const
{
  map<int,int>::const_iterator it;
  it = towerIndex.find(smTower);
  if (it == towerIndex.end()) {
    std::cout << "H4TowerPedestals::getTowerNumber: no such SM tower number: "
	      << smTower << std::endl;
    return -1;
  }
  return it->second;
}


//! Copy the mapping form a branch number on pedestal file to SM tower
//! number to another SM number
/** This allows the user to apply the pedestals from @smTower also to
    the tower @smTowerCopy. Returns @smTowerCopy on success, otherwise -1.
*/
int H4TowerPedestals::copySMTowerMapping(int smTower, int smTowerCopy){
  map<int,int>::const_iterator it;
  it = towerIndex.find(smTower);
  if(it==towerIndex.end()){
    std::cout << "H4TowerPedestals::copySMTowerMapping: no such SM tower number: "
	      << smTower << std::endl;
    return -1;
  }
  towerIndex[smTowerCopy]=it->second;
  return smTowerCopy;
}

void H4TowerPedestals::Print(const char *opt) const 
{
  std::cout << "[H4TowerPedestals::Print]" << std::endl ;
  std::cout << " current runNumber = " << runNumber 
	    << "  (current index = " << curr_idx << ")" << std::endl 
	    << " nTowers = " << nTowers << std::endl 
	    << " nCrystals/Tower = " << nCrystals << std::endl ;
  std::cout << " Tower Names:" << std::endl ;
  int i ;
  for(i=0;i<nTowers;i++) 
    std::cout << " (*) tower[" << i << "]: " << branchNames[i] << std::endl ;
}
