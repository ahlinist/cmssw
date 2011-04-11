#ifndef ElectroWeakAnalysis_MultiBosons_interface_VerboseEDFilterObjectWrapper_h
#define ElectroWeakAnalysis_MultiBosons_interface_VerboseEDFilterObjectWrapper_h

/**
  \class    VerboseFilterWrapper VerboseFilterWrapper.h "ElectroWeakAnalysis/MultiBosons/interface/FilterWrapper.h"
  \brief    Adding configurable verbose output to the EDFilterObjectWrapper

*/

#include <iostream>
#include <sstream>

#include "PhysicsTools/UtilAlgos/interface/EDFilterObjectWrapper.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace vgamma {

  template<class T, class C>
  class VerboseFilterObjectWrapper : public edm::FilterObjectWrapper<T, C>{

  public:

    VerboseFilterObjectWrapper(const edm::ParameterSet& cfg) :
      edm::FilterObjectWrapper<T, C>(cfg),
      verbosity_(cfg.getUntrackedParameter<unsigned>("verbosity", 0))
    { }
    /// default destructor
    virtual ~VerboseFilterObjectWrapper(){}
    /// done after the end of event loop
    virtual void endJob() {
      std::ostringstream cutFlow;

      switch (verbosity_) {
        case 0:
          break;
        case 1:
          this->filter_->printActiveCuts(cutFlow);
          edm::LogInfo("ActiveCuts") << cutFlow.str();
          break;
        default:
          this->filter_->print(cutFlow);
          edm::LogInfo("CutFlow") << cutFlow.str();
      } // end of switch(verbosity_)
    } // end of virtual void endJob()

  protected:
    /// Flag to control the verbosity of output
    unsigned verbosity_;
  };

}

#endif
