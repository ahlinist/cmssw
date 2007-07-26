// Author : Samvel Khalatian (samvel at fnal dot gov)
// Created: 07/25/07
// License: GPL

#include <ostream>

#include "interface/FlagTxt.h"

#include "interface/SerializeBase.h"

// --[ BASE ARCHIVE MANAGER ]--------------------------------------------------
//                                                  --[ PUBLIC ]--
SerializeBase::SerializeBase() {
  poFlagTxt_ = 0;
}

SerializeBase::~SerializeBase() {
  if( poFlagTxt_) {
    delete poFlagTxt_;
  }
}

bool SerializeBase::write( std::ostream &roOut,
                           const FlagTxt &roFLAGTXT) const
{
  bool bResult = false;

  try {
    writeFlag( roOut, roFLAGTXT);

    bResult = true;
  } catch( ...) {
    // Failed to write Flag into output stream
  }

  return bResult;
}
