/***************************************************************************
                          testDoc.cpp  -  description
                             -------------------
    Author               : Michael Case
    email                : case@ucdhep.ucdavis.edu

    Last Updated         : Jan 9, 2004
 ***************************************************************************/

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "OpticalAlignment/OptAlignASCIISource/interface/OptAlignCSCFileReader.h" 

namespace std{} using namespace std;

int main(int argc, char *argv[])
{
  std::string mystring ( argv[1] );
    OptAlignCSCFileReader oacfr( mystring );
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
  return EXIT_SUCCESS;
}
