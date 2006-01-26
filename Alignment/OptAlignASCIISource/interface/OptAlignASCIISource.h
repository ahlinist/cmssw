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
// $Id$
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

/*   bool nextRecord( std::fstream* tf ); */

/*   int getRecord(size_t i, size_t j = 0); */
/*   std::vector<float> getRecord(size_t i, size_t j = 0); */

  std::string currentType_;
  std::ifstream* tf_;
  bool firstFound_;

/*   struct OARecord { */
/*     std::string tStr_; */
/*     float tFloat_; */
/*   }; */

/*   std::vector<OARecord> record_; */

};

#endif
