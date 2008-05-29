#// Module to filter events with particular trigger bits
// can dump trigger table as well
// F.Ratnikov UMd, May 29, 2008
// $Id:$

#include "L1GTFilter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"


L1GTFilter::L1GTFilter(const edm::ParameterSet& fParameters)
  : mTriggerName (fParameters.getParameter <std::string> ("trigger")),
    mTriggerBit (-1),
    mCacheId (0),
    mDumpTriggerTable (fParameters.getUntrackedParameter<bool>("dumpTriggerTable", true)),
    mMenu (0)
{}

bool L1GTFilter::filter (edm::Event& fEvent, const edm::EventSetup& fEventSetup){
  // get trigger menu
  const L1GtTriggerMenuRcd& record = fEventSetup.get <L1GtTriggerMenuRcd> ();
  if (!mMenu || record.cacheIdentifier() != mCacheId) { // need to renew cache
    mCacheId = record.cacheIdentifier();
    edm::ESHandle  <L1GtTriggerMenu> l1GtMenu_h;
    record.get (l1GtMenu_h);
    mMenu = &*l1GtMenu_h;
    // Dump new trigger table
    if (mDumpTriggerTable) {
      std::cout << "NEW TRIGGER TABLE for RUN " << fEvent.id().run() << std::endl;
      int printLevel = 0;
      mMenu->print (std::cout, printLevel);
    }
    // Search for trigger bit
    mTriggerBit = -1;
    // have to search for all 128 bits (lack of interface in L1GtTriggerMenu)
    for (size_t ibit = 0; ibit < 128; ibit++) {
      DecisionWord oneBit (128, false); // assume 128 bits here
      oneBit [ibit] = true;
      if (mMenu->gtAlgorithmResult (mTriggerName, oneBit)) { // match!
	mTriggerBit = ibit;
	break;
      }  
    }
    if (mTriggerBit < 0) {
      edm::LogError ("L1GTFilter") << "Can not find trigger '" << mTriggerName << "' in L1 GT menu " << mMenu->gtTriggerMenuName();
    }
  }

  edm::Handle <L1GlobalTriggerReadoutRecord> gtRecord;
  fEvent.getByType (gtRecord); // assume only one L1 trigger record here
  // filter business itself 
  if (mTriggerBit >= 0 && gtRecord->decisionWord() [mTriggerBit]) return true;
  return false;
}
