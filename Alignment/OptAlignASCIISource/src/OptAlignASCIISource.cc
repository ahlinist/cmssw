#include "OpticalAlignment/OptAlignASCIISource/interface/OptAlignASCIISource.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

OptAlignASCIISource::OptAlignASCIISource ( edm::ParameterSet const& ps
					   , edm::InputSourceDescription const& desc) 
  : edm::ExternalInputSource::ExternalInputSource(ps, desc)
{
  // ... read any configuration parameters specific to your input source
  // ... do any other initialization specific to your input source.
  // note: no argument in the call to produces
  // the standard module label is assumed
  produces<OpticalAlignments>();
}
  
OptAlignASCIISource::~OptAlignASCIISource () { }


bool OptAlignASCIISource::produce ( edm::Event& e ) {
  // filenames come from super class.

  // confirm the file(s) exists and is/are readable.
  std::vector<std::string> fnames(edm::ExternalInputSource::fileNames());
  std::auto_ptr<OpticalAlignments> result(new OpticalAlignments);
  for ( std::vector<std::string>::const_iterator it = fnames.begin();
	it != fnames.end(); it++ ) {
    firstFound_ = false;

    currentType_.clear();
    size_t fit = it->rfind("/");
    size_t eit = it->rfind(".");
    size_t j = 0;
    for ( size_t i = fit; i < eit; i++ ) {
      currentType_[j++] = (*it)[i];
    }
    std::cout << currentType_ << std::endl;
    //    std::copy(fit, eit, currentType_);
    tf_ = new std::ifstream( it->c_str() );
    if ( tf_ ) {
      //       while ( nextRecord(tf_) ) {
      // 	int getNextRecord(1);
      // 	std::vector<float> getNextRecord(6,9);
      //       }
    }
    delete tf_;
    
    
    // fill OpticalAlignments based on the parsing.
    
  }
  e.put(result);
  return true;
}

// some sort of method to get the next record
// which will contain: int,char,char,char,char and 9 floats.
// ASSUMPTION: they will be in the same order in the text file always.
// reader/interpreter and writer/author will need to talk to know
// what variables they are talking about.
//
// the int == OpticalInfo::objectID_
// the char,char,char,char,char will be concatinated using the | (pipe)
// symbol as a divider and put into OpticalInfo::objectType_
// 
// since none of the floats are x, y, z or angx, angy, angz they
// will be put into extraEntries and the transformation will be 
// 0,0,0 and 0,0,0.
//
// from the text file I know to ignore everything before "Type,"
// from there on it is a matter of parsing starting with the next
// line, and ignoring the first entry of the record. i.e. because 
// of the way they Excel exports and the titles put in the 
//
// NOTE: if there is more than one file then the file name will be used
// to name the type, instead of concatinating the char variables.

// bool OptAlignASCIISource::nextRecord( std::fstream* tf ) {


// }


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(OptAlignASCIISource)
