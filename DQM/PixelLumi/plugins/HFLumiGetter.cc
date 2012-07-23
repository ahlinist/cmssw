// -*- C++ -*-

// Package:    HFLumiGetter
// Class:      HFLumiGetter

#include "HFLumiGetter.h"

#include <memory>
#include <string>
#include <sstream>

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//DIP lumi stuff
#include "RecoLuminosity/LumiProducer/interface/DIPLumiSummary.h"
#include "RecoLuminosity/LumiProducer/interface/DIPLumiDetail.h"
#include "RecoLuminosity/LumiProducer/interface/DIPLuminosityRcd.h"

#include "FWCore/Framework/interface/NoRecordException.h"


// Constructors and destructor.
HFLumiGetter::HFLumiGetter(const edm::ParameterSet& iConfig) :
  intHFLumiRecorded(0.),
  intHFLumiDelivered(0.),
  intHFLumiByBx(lastBunchCrossing+1,0.),
  fHistHFRecordedByLS((MonitorElement*)0),
  fHistHFDeliveredByLS((MonitorElement*)0),
  fHistHFRecordedByBxLastLS((MonitorElement*)0),
  fHistHFRecordedByBxCumulative((MonitorElement*)0),
  fHistHFRecordedToDeliveredRatioByLS((MonitorElement*)0)
{
}

HFLumiGetter::~HFLumiGetter()
{
  // Do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.).
}


// Member functions.

// ------------ Method that fills 'descriptions' with the allowed parameters for the module.  ------------
void
HFLumiGetter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters.
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}


// ------------ Method called for each event.  ------------
void
HFLumiGetter::analyze(const edm::Event& iEvent,
                         const edm::EventSetup& iSetup)
{


} // analyze()


// ------------ Method called once per job just before starting event loop.  ------------
void
HFLumiGetter::beginJob()
{
  edm::LogInfo("Status") << "Starting new job";
}


// ------------ Method called once per job just after ending the event loop.  ------------
void
HFLumiGetter::endJob()
{
  edm::LogInfo("Status") << "Successfully finished job";
}


// ------------ Method called when starting to process a run.  ------------
void
HFLumiGetter::beginRun(edm::Run const& run, edm::EventSetup const&)
{
  // book or get HF lumi histograms

  DQMStore *dbe_ = edm::Service<DQMStore>().operator->();

  std::string folder = "PixelLumi/HF";
  dbe_->setCurrentFolder(folder);
  fHistHFRecordedByLS  = dbe_->get(folder+"/totalHFRecordedLumiByLS");
  if(fHistHFRecordedByLS==0)
    fHistHFRecordedByLS=dbe_->book1D("totalHFRecordedLumiByLS","HF Recorded Lumi vs LS",8000,0.5,8000.5);

  fHistHFDeliveredByLS  = dbe_->get(folder+"/totalHFDeliveredLumiByLS");
  if(fHistHFDeliveredByLS==0)
    fHistHFDeliveredByLS=dbe_->book1D("totalHFDeliveredLumiByLS","HF Delivered Lumi vs LS",8000,0.5,8000.5);

  fHistHFRecordedToDeliveredRatioByLS = dbe_->book1D("HFRecordedToDeliveredRatioByLS","HF Recorded to Delivered Lumi Ratio vs LS",
						     8000,0.5,8000.5);
}


// ------------ Method called when ending the processing of a run.  ------------
void
HFLumiGetter::endRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ Method called when starting to process a luminosity block.  ------------
void
HFLumiGetter::beginLuminosityBlock(edm::LuminosityBlock const&lumiBlock,
                                      edm::EventSetup const&)
{
}


// ------------ Method called when ending the processing of a luminosity block.  ------------
void
HFLumiGetter::endLuminosityBlock(edm::LuminosityBlock const& lumiBlock,
				 edm::EventSetup const&es)
{

  unsigned int ls = lumiBlock.luminosityBlockAuxiliary().luminosityBlock();
  // accumulate HF data at every LS as it is closed. 
  // note: lumi unit from DIPLumiSummary and Detail is microbarns
  try{
    edm::ESHandle<DIPLumiSummary> datahandle;
    unsigned long long cache_id=es.get<DIPLuminosityRcd>().cacheIdentifier();
    es.getData(datahandle);
    if(datahandle.isValid()){
      const DIPLumiSummary* lumiSummary_=datahandle.product();
      intHFLumiRecorded += (lumiSummary_->intgRecLumiByLS()/1000.); //in nb
      intHFLumiDelivered += (lumiSummary_->intgDelLumiByLS()/1000.); //in nb
    }else{
      std::cout<<"DIPLumiSummary:: no valid data found"<<std::endl;
    }
  }catch(const edm::eventsetup::NoRecordException<DIPLuminosityRcd>& er){
    std::cout<<"DIPLumiSummary:: no data found"<<std::endl;
  }

  printf("Lumi Block = %d\n",ls);

  //HF stuff first
  fHistHFRecordedByLS->setBinContent(ls,intHFLumiRecorded);
  fHistHFDeliveredByLS->setBinContent(ls,intHFLumiDelivered);
  if(intHFLumiDelivered != 0.)
    fHistHFRecordedToDeliveredRatioByLS->setBinContent(ls,intHFLumiRecorded/intHFLumiDelivered);

  // now the HF counters can be reset (this is especially important if fResetIntervalInLumiSections != 1)
  intHFLumiRecorded = 0.;
  intHFLumiDelivered = 0.;
  for(unsigned int i = 0; i<= lastBunchCrossing; i++) intHFLumiByBx[i] = 0.;

}

// Define this as a CMSSW plug-in.
DEFINE_FWK_MODULE(HFLumiGetter);
