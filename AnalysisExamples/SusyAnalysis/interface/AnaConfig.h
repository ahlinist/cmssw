#ifndef AnaConfig_h
#define AnaConfig_h

// system include files
#include <memory>
#include <string>
#include <iostream>

#include "FWCore/ParameterSet/interface/ParameterSet.h"

struct Config_t {
     edm::ParameterSet acceptance_cuts;
     edm::ParameterSet cleaner_params;
     edm::ParameterSet isolator_params;
     edm::ParameterSet objectmatch_params;
    
};

#endif
