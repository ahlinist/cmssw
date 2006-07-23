#ifndef CSCCalibDigis_CSCCalibDigisProducer_h
#define CSCCalibDigis_CSCCalibDigisProducer_h

/*
//
// Original Authors: Dominique Fortin - UCR
// $Id: CSCCalibDigisProducer.h,v 1.1 2006/07/19 21:12:41 dfortin Exp $
//
*/

#include <memory>
#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class MakeStripDigiCollections;
class MakeWireDigiCollections;

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
  MakeStripDigiCollections* mkStripCollect_;
  MakeWireDigiCollections* mkWireCollect_;


};
#endif
