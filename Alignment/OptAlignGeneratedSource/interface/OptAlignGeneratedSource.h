#ifndef Guard_OptAlignGeneratedSource_h
#define Guard_OptAlignGeneratedSource_h
#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

class OptAlignGeneratedSource : public edm::GeneratedInputSource
{

 public:

  OptAlignGeneratedSource ( edm::ParameterSet const& ps
			, edm::InputSourceDescription const& desc);

  virtual ~OptAlignGeneratedSource();

  bool produce( edm::Event& e );

 private:
  unsigned numObjects_;
};

#endif
