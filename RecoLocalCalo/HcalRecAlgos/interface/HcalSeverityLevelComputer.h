// -*- C++ -*-
//
// Package:    HcalRecAlgos
// Class:      HcalSeverityLevelComputer
// 
/*
 Description: delivers the severity level for HCAL cells
*/
//
// Original Author:  Radek Ofierzynski
//
//

#ifndef HCALSEVERITYLEVELCOMPUTER_H
#define HCALSEVERITYLEVELCOMPUTER_H

#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"


class HcalSeverityLevelComputer
{

 public:
  HcalSeverityLevelComputer( const edm::ParameterSet& );
  ~HcalSeverityLevelComputer();
  
  // gives back severity level based on evaluation of the RecHit flag and cell's channel status
  int getSeverityLevel(const DetId& myid, const uint32_t& myflag, const uint32_t& mystatus) const;
  
 private:
  class HcalSeverityDefinition
  {
  public:
    int sevLevel;
    uint32_t chStatusMask;
    uint32_t HBHEFlagMask, HOFlagMask, HFFlagMask, ZDCFlagMask, CalibFlagMask;
  HcalSeverityDefinition():
    sevLevel(0), chStatusMask(0),
      HBHEFlagMask(0), HOFlagMask(0), HFFlagMask(0), ZDCFlagMask(0), CalibFlagMask(0)
      {}
    
  };

  std::vector<HcalSeverityDefinition> SevDef;

  void setBit (const unsigned bitnumber, uint32_t& where);

  friend std::ostream& operator<<(std::ostream& s, const HcalSeverityLevelComputer::HcalSeverityDefinition& def);


};


#endif
