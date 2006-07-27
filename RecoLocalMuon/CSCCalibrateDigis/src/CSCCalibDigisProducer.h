#ifndef CSCCalibrateDigis_CSCCalibDigisProducer_h
#define CSCCalibrateDigis_CSCCalibDigisProducer_h

/** \class CSCCalibDigisProducer
 * 
 * Reads in calibration database and computes the average strip gain G for whole CSC System.
 * Then, it loops over all CSCStripDigis, and it scales the ADC counts of each channel i by
 * a factor G/g_i, where g_i is the gain for strip i.
 *
 * A new CSCStripDigi is then formed and added to a new CSCStripDigiCollection
 *
 * \author Dominique Fortin - UCR
 *
*/

#include <memory>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CSCMakeStripDigiCollections;
class CSCMakeWireDigiCollections;

class CSCCalibDigisProducer : public edm::EDProducer {
 public:

  explicit CSCCalibDigisProducer(const edm::ParameterSet&);

  ~CSCCalibDigisProducer();

  virtual void produce(edm::Event&, const edm::EventSetup&);


 private:
 
  // Keep index of event processed
  int iev; 

  // Input parameters from .cfg file:
  bool debug;
  std::string RawDataLabel;

// Classe nicknames:
  CSCMakeStripDigiCollections* mkStripCollect_;
  CSCMakeWireDigiCollections* mkWireCollect_;


};
#endif
