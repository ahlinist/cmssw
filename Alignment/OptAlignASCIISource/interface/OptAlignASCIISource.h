#ifndef Guard_OptAlignASCIISource_h
#define Guard_OptAlignASCIISource_h
#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include <iosfwd>

// -*- C++ -*-
//
// Package:    AlignmentTools/OptAlignASCIISource
// Class:      OptAlignASCIISource.h
// 
/**\class OptAlignASCIISource OptAlignASCIISource.cc AlignmentTools/OptAlignASCIISource/interface/OptAlignASCIISource.h

 Description: test access to the OpticalAlignments as read from the file provided
    by Marcus Hohlmann

 Implementation:

*/
//
// Original Author:  Mike Case
//         Created:  Mon Jan 17 11:47:40 CET 2006
// $Id: OptAlignASCIISource.h,v 1.1 2006/01/26 15:03:48 case Exp $
//
//

class OptAlignASCIISource : public edm::ExternalInputSource
{

 public:

  OptAlignASCIISource ( edm::ParameterSet const& ps
			, edm::InputSourceDescription const& desc);

  virtual ~OptAlignASCIISource();

  bool produce( edm::Event& e );

 private:

};

#endif
