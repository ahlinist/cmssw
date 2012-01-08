// CMSDAS_PileupReweight.h
// Description: A basic analyzer for pileup reweighting studies
// Author: Mike Hildreth
// Date: January 8, 2012

#ifndef __CMSDAS_PILEUPREWEIGHT_H__
#define __CMSDAS_PILEUPREWEIGHT_H__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


#include <string>

class TH1D;

class CMSDAS_PileupReweight : public edm::EDAnalyzer {
 public:
  CMSDAS_PileupReweight(const edm::ParameterSet &);
  void analyze( const edm::Event& , const edm::EventSetup& );
  virtual ~CMSDAS_PileupReweight() {}
  virtual void beginJob() {}
  virtual void endJob(void);
  
  
 private:

  // Parameters
  edm::InputTag vertexSrc_;

  // histograms

  TH1D* TNPUInTime_;
  TH1D* TNPUTrue_;
  TH1D* TNVTX_;

};


#endif
