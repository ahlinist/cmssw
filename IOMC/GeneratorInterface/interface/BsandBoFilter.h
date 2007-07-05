#ifndef _BsandBoFilter_h_
#define _BsandBoFilter_h_


// system include files
#include <memory>
#include <iostream>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"


class BsandBoFilter : public edm::EDFilter 
{

public:

  explicit BsandBoFilter(const edm::ParameterSet&);
  ~BsandBoFilter();
  
  virtual bool filter(edm::Event&, const edm::EventSetup&);

private:

  std::string label_;
  int noAccepted;
};


#endif
