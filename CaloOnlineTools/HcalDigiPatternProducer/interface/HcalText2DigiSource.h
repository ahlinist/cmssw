#ifndef HCAL_TEXT_2_DIGI_SOURCE_H
#define HCAL_TEXT_2_DIGI_SOURCE_H

#include "FWCore/Sources/interface/ExternalInputSource.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

class HcalText2DigiSource : public edm::ExternalInputSource 
{

 public:
  HcalText2DigiSource(const edm::ParameterSet &ps, edm::InputSourceDescription const& desc);
  ~HcalText2DigiSource();    
  void ConstructDigi(HBHEDataFrame* frame, int adc);
  void ConstructDigi(HFDataFrame* frame, int adc);
  int FindEnergy(HcalDetId id);
  bool ReadEvent();
  
 protected:
  virtual bool produce(edm::Event & e);
  virtual void beginJob(edm::EventSetup const& eventSetup);

 private:
  HcalTrigTowerGeometry theTrigTowerGeometry;
  //std::string &file;
  std::ifstream input;
  std::map<HcalTrigTowerDetId,int> eventMap;
  std::vector<DetId> hbCells;
  std::vector<DetId> heCells;
  std::vector<DetId> hfCells;
  bool zs;
};

#endif
