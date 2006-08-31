#include "DQM/SiStripCommissioningSources/interface/FedTimingTask.h"
#include "DQM/SiStripCommon/interface/SiStripHistoNamingScheme.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "CalibFormats/SiStripObjects/interface/SiStripFecCabling.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// -----------------------------------------------------------------------------
//
FedTimingTask::FedTimingTask( DaqMonitorBEInterface* dqm,
			      const FedChannelConnection& conn ) :
  CommissioningTask( dqm, conn, "FedTimingTask" ),
  timing_(),
  nBins_(40) //@@ this should be from number of scope mode samples (mean booking in event loop and putting scope mode length in trigger fed)
{
  edm::LogInfo("Commissioning") << "[FedTimingTask::FedTimingTask] Constructing object...";
}

// -----------------------------------------------------------------------------
//
FedTimingTask::~FedTimingTask() {
  edm::LogInfo("Commissioning") << "[FedTimingTask::FedTimingTask] Destructing object...";
}

// -----------------------------------------------------------------------------
//
void FedTimingTask::book() {
  edm::LogInfo("Commissioning") << "[FedTimingTask::book]";

  uint16_t nbins = 24 * nBins_; // 24 "fine" pll skews possible

  string title;
  
  title = SiStripHistoNamingScheme::histoTitle( sistrip::FED_TIMING, 
						sistrip::COMBINED, 
						sistrip::FED, 
						fedKey(),
						sistrip::LLD_CHAN, 
						connection().lldChannel() );

  timing_.histo_ = dqm()->bookProfile( title, title, 
				       nbins, -0.5, nbins*1.-0.5,
				       1024, 0., 1024. );
  
  timing_.vNumOfEntries_.resize(nbins,0);
  timing_.vSumOfContents_.resize(nbins,0);
  timing_.vSumOfSquares_.resize(nbins,0);
  
}

// -----------------------------------------------------------------------------
//
/*
  Some notes: 
  - use all samples 
  - extract number of samples from trigger fed
  - need to book histos in event loop?
  - why only use fine skew setting when filling histos? should use coarse setting as well?
  - why do different settings every 100 events - change more freq? 
*/
void FedTimingTask::fill( const SiStripEventSummary& summary,
			  const edm::DetSet<SiStripRawDigi>& digis ) {
  LogDebug("Commissioning") << "[FedTimingTask::fill]";

  //@@ if scope mode length is in trigger fed, then 
  //@@ can add check here on number of digis
  if ( digis.data.size() < nBins_ ) {
    edm::LogError("Commissioning") << "[FedTimingTask::fill]" 
				   << " Unexpected number of digis! " 
				   << digis.data.size(); 
  } else {
    pair<uint32_t,uint32_t> skews = const_cast<SiStripEventSummary&>(summary).pll();
    for ( uint16_t coarse = 0; coarse < nBins_/*digis.data.size()*/; coarse++ ) {
      uint16_t fine = (coarse+1)*24 - (skews.second+1);
      updateHistoSet( timing_, fine, digis.data[coarse].adc() );
    }
  }

}

// -----------------------------------------------------------------------------
//
void FedTimingTask::update() {
  LogDebug("Commissioning") << "[FedTimingTask::update]";
  updateHistoSet( timing_ );
}


