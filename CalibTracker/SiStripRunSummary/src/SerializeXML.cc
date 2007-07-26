// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/15/07
// License: GPL

#include <istream>
#include <ostream>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

// Next include is essential: this is the right place where all calsses are
// registered for serialization: include all anchestors of FlagTxt.
#include "CalibTracker/SiStripRunSummary/interface/DQMFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/FlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TIBFlagXML.h"
#include "CalibTracker/SiStripRunSummary/interface/TOBFlagXML.h"

#include "CalibTracker/SiStripRunSummary/interface/SerializeXML.h"

// --[ TEXT ARCHIVE MANAGER ]--------------------------------------------------
//                                                  --[ PUBLIC ]--
SerializeXML::SerializeXML() {
  poFlagXML_ = 0;
}

SerializeXML::~SerializeXML() {
  if( poFlagXML_) {
    delete poFlagXML_;
  }
}

bool SerializeXML::write( std::ostream  &roOut,
                          const FlagXML &roFLAGXML) const
{
  bool bResult = false;

  boost::archive::xml_oarchive oAOut( roOut);

  try {
    oAOut << BOOST_SERIALIZATION_NVP( roFLAGXML);

    bResult = true;
  } catch( const boost::archive::archive_exception &roEX) {
    // Failed to serialize Object
  }

  return bResult;
}

//                                                  --[ PRIVATE ]--
void SerializeXML::readFlag( FlagXML *poFlagXML, std::istream &roIn) {
  boost::archive::xml_iarchive oAIn( roIn);

  oAIn >> BOOST_SERIALIZATION_NVP( *poFlagXML);
}
