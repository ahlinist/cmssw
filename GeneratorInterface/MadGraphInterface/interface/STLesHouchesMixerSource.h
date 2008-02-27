#ifndef STLesHouchesMixerSource_h
#define STLesHouchesMixerSource_h
// -*- C++ -*-
// Package:    MadGraphInterface
// Class:      STLesHouchesMixerSource
/**\class STLesHouchesMixerSource STLesHouchesMixerSource.cc GeneratorInterface/MadGraphInterface/src/STLesHouchesMixerSource.cc
 Description:        Mix two Les Houches Files of 22 and 23 processes for single top production. 23 process is filtered. It gives meaningful results only if run together with STFilter. ST = Single Top
 Implementation: implemented like an empty (it produces nothing) input source
*/
// Original Author:  Julia Weinelt, Dorian Kcira
//         Created:  Tue Feb 26 17:48:50 CET 2008
// $Id$
#include "FWCore/Sources/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
namespace edm {
  class STLesHouchesMixerSource : public ExternalInputSource {
    public:
      STLesHouchesMixerSource(const ParameterSet &, const InputSourceDescription &);
      virtual ~STLesHouchesMixerSource();
    private:
      virtual bool produce(Event & e);
    private:
      std::string inputLHEFile22_;
      std::string inputLHEFile23_;
  };
} 
#endif
