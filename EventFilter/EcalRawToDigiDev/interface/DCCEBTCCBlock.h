#ifndef DCCEBTCCBLOCK_HH
#define DCCEBTCCBLOCK_HH

/*
 *\ Class DCCEBTCCBlock
 *
 * Class responsible for the EB Trigger Tower primitives unpacking.
 *
 * \file DCCEBTCCBlock.h
 *
 * $Date: 2009/03/11 18:17:05 $
 * $Revision: 1.8 $
 *
 * \author N. Almeida
 * \author G. Franzoni
 *
*/

#include <iostream>                  
#include <string>
#include <vector>
#include <map>
#include <utility>



#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h>
#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include <DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h>

#include "DCCTCCBlock.h"

class DCCDataUnpacker;

class DCCEBTCCBlock : public DCCTCCBlock {
	
  public :
    /**
      Class constructor
    */
    DCCEBTCCBlock( DCCDataUnpacker *  u, EcalElectronicsMapper * m, DCCEventBlock * e, bool unpack);    
	 
    void updateCollectors();
   
    void addTriggerPrimitivesToCollection();
  
  protected :

   bool checkTccIdAndNumbTTs();

};

#endif
