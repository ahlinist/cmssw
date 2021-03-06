#ifndef DCCTowerBLOCK_HH
#define DCCTowerBLOCK_HH

#include <iostream>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>
#include <DataFormats/EcalRawData/interface/EcalRawDataCollections.h>
#include <DataFormats/EcalDetId/interface/EcalDetIdCollections.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include "DCCFEBlock.h"

class DCCEventBlock;
class DCCDataUnpacker;

class DCCTowerBlock : public DCCFEBlock {
	
  //to implement
	
  public :

    DCCTowerBlock(DCCDataUnpacker * u,EcalElectronicsMapper * m, DCCEventBlock * e, bool unpack );
    
    void updateCollectors();
	 
  protected:
	 
    int unpackXtalData(uint stripID, uint xtalID);
    void fillEcalElectronicsError( std::auto_ptr<EcalElectronicsIdCollection> * );
    


    std::auto_ptr<EBDigiCollection>     * digis_;
    
    EBDetId                             * pDetId_;

    // to restructure as common collections to DCCSCBlock, to inherit from DCCFEBlock
    std::auto_ptr<EBDetIdCollection>    * invalidGains_;  
    std::auto_ptr<EBDetIdCollection>    * invalidGainsSwitch_ ;
    std::auto_ptr<EBDetIdCollection>    * invalidChIds_;
	 
};


#endif
