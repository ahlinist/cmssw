#include "OpticalAlignment/OptAlignASCIISource/interface/OptAlignASCIISource.h"
#include "OpticalAlignment/OptAlignASCIISource/interface/OptAlignCSCFileReader.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

// Boost parser, spirit, for parsing the std::vector elements.
#include "boost/spirit/core.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

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
    OptAlignCSCFileReader oacfr(*it);
    std::vector<std::string> names, types;
    oacfr.next();
    if ( oacfr.getData(names) ) {
      oacfr.next();
      if ( oacfr.getData(types) ) {
	if ( names.size() != types.size() ) { 
	  std::cout << "Invalid file header.  Need names and types." << std::endl;
	  std::cout << "names.size() = " << names.size() << "  types.size() = " << types.size() << std::endl;
	} else {
	  std::cout << "name-type: " ;
	  for ( size_t i = 0 ; i < names.size() ; i++ ) {
	    std::cout << names[i] << "-" << types[i] << ", ";
	  }
	  std::cout << std::endl << std::endl;
	  while ( oacfr.next() ) {
	    std::vector<std::string> stringData;
	    //	  std::vector<int> intData;
	    std::vector<double> dblData;
	    oacfr.getData(dblData);
	    oacfr.getData(stringData);
	    //	  std::vector<float> fltData;
	    std::cout << "========= Doubles ===========" << std::endl;
	    for ( size_t i = 0; i < dblData.size(); i++ ) {
	      std::cout << dblData[i] << ",  ";
	    }
	    std::cout << std::endl;
	    std::cout << "========= Strings ===========" << std::endl;
	    for ( size_t i = 0; i < stringData.size(); i++ ) {
	      std::cout << stringData[i] << ",  ";
	    }
	    std::cout << std::endl;
	    // fill OpticalAlignments based on the parsing.
	  } // end while
	} // end else
      } // end if got names 
    } // if got  types
  } // end for each file name.
  e.put(result);
  return true;
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(OptAlignASCIISource)
