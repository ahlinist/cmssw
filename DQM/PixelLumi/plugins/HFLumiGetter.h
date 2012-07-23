// -*- C++ -*-

// Package:    HFLumiGetter
// Class:      HFLumiGetter


// Original author:   Amita Raval


#ifndef __PixelLumi_PixelLumiDQM_HFLumiGetter_h__
#define __PixelLumi_PixelLumiDQM_HFLumiGetter_h__

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Provenance/interface/RunID.h"
#include "TObject.h"

class MonitorElement;

class HFLumiGetter : public edm::EDAnalyzer {
public:
  explicit HFLumiGetter(const edm::ParameterSet&);
  ~HFLumiGetter();
  // the highest bunch crossing id with 25 ns bunch spacing (only one out of two can be filled with 50ns)
  // constants
  // static const unsigned int NUM_BX = 3564;
  static constexpr double FREQ_ORBIT = 11246.;
  static constexpr double SECONDS_PER_LS = double(0x40000)/double(FREQ_ORBIT);
  static constexpr double CM2_TO_NANOBARN = 1.d/1.e-33d;
  static const unsigned int lastBunchCrossing = 3564;


  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob();
  virtual void endJob();
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&,
                                    edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&,
                                  edm::EventSetup const&);


  float intHFLumiRecorded;
  float intHFLumiDelivered;
  std::vector<float> intHFLumiByBx;

  //HF stuff
  MonitorElement *fHistHFRecordedByLS;
  MonitorElement *fHistHFDeliveredByLS;
  MonitorElement *fHistHFRecordedToDeliveredRatioByLS;
  MonitorElement *fHistHFRecordedByBxLastLS;
  MonitorElement *fHistHFRecordedByBxCumulative;

};

#endif
