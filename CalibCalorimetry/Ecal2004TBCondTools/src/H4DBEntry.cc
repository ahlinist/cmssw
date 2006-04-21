// -*- C++ -*-
// 
// File H4DBEntry.C
//
/*! \class H4DBEntry 
 * \brief Interface class for classes which would 
 * like to be handled by the H4DBManager class.
 *
 * last change : $Date: 2003/09/24 09:10:18 $
 * by          : $Author: nrl $
 *
 */

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include <iostream>
using namespace std;

ClassImp(H4DBEntry)


H4DBEntry::H4DBEntry(){
  matchMethod=indexOnly;
  valid=false;
}

int H4DBEntry::isValid() const{
  return valid;
}

int H4DBEntry::getMinSubIndex() const{
  return minSubIndex;
}

int H4DBEntry::getMaxSubIndex() const{
  return maxSubIndex;
}

void H4DBEntry::print(Option_t *option) const{
  std::cout << "H4DBEntry::print: This should not have happend, you are "<< std::endl
	    << "trying to print a badly initialized H4DBEntry object!" << std::endl;
}
