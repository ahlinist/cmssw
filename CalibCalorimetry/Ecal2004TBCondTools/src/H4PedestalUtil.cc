/*! File H4PedestalUtil.C
 \class H4PedestalUtil
 \brief A small utility class to find pedestal runs in the rootdb.

 $Date: 2004/11/09 11:00:20 $
 $Author: brunelie $

 This class locates the nearest pedestal run within 24 hours into the
 past or future of a run in the run-database. If no run is found, a
 default is used which is defined by defaultPedestalRun.

 Pedestal runs are runs with at least 100 pedestal trigger events and
 a VfeMode of 10.
*/

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PedestalUtil.h"
#include <iostream>

ClassImp(H4PedestalUtil)

#define DEFAULT_PEDESTAL_RUN 72974

H4PedestalUtil::H4PedestalUtil(){
  myDBManager=0; 
  myRunEntry=0;
  pedRun = 0 ;
  prevRqstRunNumber = -100 ;
  defaultPedestalRun=DEFAULT_PEDESTAL_RUN;
}

H4PedestalUtil::H4PedestalUtil(H4DBManager *manager, H4Run *runEntry){
  myDBManager=manager;
  myRunEntry=runEntry;
  pedRun = 0 ;
  prevRqstRunNumber = -100 ;
  defaultPedestalRun = DEFAULT_PEDESTAL_RUN;
}

int H4PedestalUtil::getPedestalRun(int run) const{
  
  if( (prevRqstRunNumber!=-100) && (run==prevRqstRunNumber) ) 
    return pedRun ;
    
  // This is a new request 
  prevRqstRunNumber = run ;
    
  // We save the contents of the run-object 
  H4Run savedRun(*myRunEntry);

  // We did not find a run, yet, so we start with a good default
  int bestRun=defaultPedestalRun;
  int n = myDBManager->update(bestRun, H4DBEntry::NoSuchSubindex, myRunEntry);
  if(n<0 || n>1){
    std::cout << "H4PedestalUtil: Default run not valid!" << std::endl;
    abort();
  }
  int bestTime = myRunEntry->getStartTime();


  // We will look backwards and forwards
  for (int direction=-1; direction<2; direction+=2){
       
    // Select run given in arguments, if not unique, take first subrun
    int n = myDBManager->update(run, H4DBEntry::NoSuchSubindex, myRunEntry);
    if(n>1)
      myDBManager->update(run,myRunEntry->getMinSubIndex(), myRunEntry);
    // Check whether we actually found a run at all
    if(n<0){
      std::cout <<"H4PedestalUtil::getPedestalRun: Could not find run " 
		<< run << std::endl;
      *myRunEntry = savedRun;
      pedRun = 0 ;
      return 0;
    }
    int timeOfRun=myRunEntry->getStartTime();

#ifdef DEBUG
    std::cout << "Time of run: " << timeOfRun << std::endl;
#endif

    int d=0;  // Distance to desired run
    bool otherDirection = false;

    while (true){

      if(run+d*direction < 0 || d>500) break;
      
      int minSubRun=0,maxSubRun=0;
      if(myDBManager->update(run+d*direction,
			     H4DBEntry::NoSuchSubindex, myRunEntry) > 1){
	myRunEntry->print();
	minSubRun=myRunEntry->getMinSubIndex();
	maxSubRun=myRunEntry->getMinSubIndex();
      }

      // Loop over all subruns
      for(int subrun = minSubRun; subrun <=maxSubRun; subrun++){

	// If there are subruns, select the respective subrun
	if(minSubRun!=maxSubRun)
	  myDBManager->update(run+d*direction, subrun, myRunEntry);

#ifdef DEBUG
	std::cout << "run: " << run+d*direction << "   time: " 
		  << myRunEntry->getStartTime() << std::endl;
#endif

	// In plus/minus 24 hour time window?
	if(abs(myRunEntry->getStartTime()-timeOfRun)>86400){
	  otherDirection=true;
	  break;
	}	

	// Check quality of pedestal run
	if (myRunEntry->getNPedTrg() > 600 && myRunEntry->getVfeMode() == 10) {
#ifdef DEBUG
	  std::cout << "A possible trigger run!" << std::endl;
#endif

	  // Is the time difference larger than for the existing best run
	  if(abs(bestTime-timeOfRun) 
	     < abs(myRunEntry->getStartTime()-timeOfRun)){
	    otherDirection=true;
	    break;
	  }

	  bestRun=run+d*direction;
	  bestTime=myRunEntry->getStartTime();
	}
      } // Loop over all subruns
      if(otherDirection) break;
      d++;
    } // while (true)
  }  // direction -1 and +1
  *myRunEntry = savedRun;
  pedRun = bestRun ;
  return bestRun;
}
