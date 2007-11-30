#ifndef FWCore_Framework_OutputModuleDescription_h
#define FWCore_Framework_OutputModuleDescription_h

/*----------------------------------------------------------------------

OutputModuleDescription : the stuff that is needed to configure an
output module that does not come in through the ParameterSet  

$Id$
----------------------------------------------------------------------*/
namespace edm {

  struct OutputModuleDescription {
    OutputModuleDescription() : maxEvents_(-1), maxLumis_(-1) {}
    OutputModuleDescription(int maxEvents, int maxLumis) :
      maxEvents_(maxEvents),
      maxLumis_(maxLumis)
    {}
    int maxEvents_;
    int maxLumis_;
  };
}

#endif
