#ifndef Guard_LTDRootSource_h
#define Guard_LTDRootSource_h
#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include <iosfwd>

// -*- C++ -*-
//
// Package:    AlignmentTools/LTDRootSource
// Class:      LTDRootSource.h
// 
/**\class LTDRootSource LTDRootSource.cc AlignmentTools/LTDRootSource/interface/LTDRootSource.h

 Description: test access to the OpticalAlignments as read from the file provided
    by Marcus Hohlmann

 Implementation:

*/
//
// Original Author:  Mike Case and Noemi Beni
//         Created:  //Mon Jan 17 11:47:40 CET 2006
//                   Tue Feb 21 17:02 CET 2006
// $Id: LTDRootSource.h,v 1.2 2006/02/16 10:38:56 case Exp $
//
//

class LTDRootSource : public edm::ExternalInputSource
{

 public:

  LTDRootSource ( edm::ParameterSet const& ps
			, edm::InputSourceDescription const& desc);

  virtual ~LTDRootSource();

  bool produce( edm::Event& e );

 private:

};

#endif
