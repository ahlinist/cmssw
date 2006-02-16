#include "OpticalAlignment/OptAlignASCIISource/interface/OptAlignCSCFileReader.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"

// Boost parser, spirit, for parsing the std::vector elements.
#include "boost/spirit/core.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace boost { namespace spirit {} } using namespace boost::spirit;

  struct CSVMakeDouble
  {
    void operator() (char const* str, char const* end) const
    {
      oas_->do_makeDouble(str, end);
    }
    
    CSVMakeDouble() :oas_(0) {}
    explicit CSVMakeDouble ( const OptAlignCSCFileReader* oas ) : oas_(oas) {}

    OptAlignCSCFileReader * oas_;
    
  };
  
  struct CSVMakeString
  {
    void operator() (char const* str, char const* end) const
    {
      oas_->do_makeString(str, end);
    }
    
    CSVMakeString() : oas_(0) {}
    explicit CSVMakeString ( const OptAlignCSCFileReader* oas ) : oas_(oas) {}
  
    OptAlignCSCFileReader * oas_;

  };

OptAlignCSCFileReader::OptAlignCSCFileReader ( const std::string& fname ) : fileName_(fname), tf_(fname.c_str()) 

{ 
  // extract the "type" from the filename.
    size_t fit = fname.rfind("/");
    size_t eit = fname.rfind(".");
    size_t j = 0;
    for ( size_t i = fit; i < eit; i++ ) {
      type_[j++] = fname[i];
    }
    std::cout << type_ << std::endl;
}
  
OptAlignCSCFileReader::~OptAlignCSCFileReader () { }


bool OptAlignCSCFileReader::next ( ) {

  bool toReturn(false);
  toReturn = getline(tf_, line_);
  std::cout << "raw line " << line_ << std::endl;
  dblVec_.clear();
  strVec_.clear();
  parse_strings(line_.c_str());
  parse_numbers(line_.c_str());

//   //    std::copy(fit, eit, currentType_);
//   //    tf_ = new std::ifstream( it->c_str() );
//   std::string mystring("3,10, 20,50,40");
//   std::cout << mystring << " about to be parsed." << std::endl;
//   if (!parse_numbers(mystring.c_str())) { std::cout << " did not parse!" << std::endl; }
//   if ( tf_ ) {
//     //       while ( nextRecord(tf_) ) {
//     // 	int getNextRecord(1);
//     // 	std::vector<float> getNextRecord(6,9);
//     //       }
//   }
  return toReturn;
}

bool OptAlignCSCFileReader::parse_numbers(char const* str) const
{
  static CSVMakeDouble makeDouble(this);
  //  if ( makeDouble.oas_ == 0 ) { makeDouble.oas_ = this; }
  return parse(str,
	       ((+(anychar_p - ','))[makeDouble] 
		>> *(',' >> (+(anychar_p - ','))[makeDouble]))
	       , space_p).full;
}

bool OptAlignCSCFileReader::parse_strings(char const* str) const
{
  static CSVMakeString makeString(this);
  //  if ( makeString.oas_ == 0 ) { makeString.oas_ = this; }
  return parse(str,
	       ((+(anychar_p - ','))[makeString] 
		>> *(',' >> (+(anychar_p - ','))[makeString]))
	       , space_p).full;
}

void OptAlignCSCFileReader::do_makeString(char const* str, char const* end)
{
  std::string ts(str, end);
  strVec_.push_back(ts);
}

void OptAlignCSCFileReader::do_makeDouble(char const* str, char const* end)
{
  std::string ts(str, end);
  try {
    std::istringstream st( ts );
    double td;
    st >> td;
    dblVec_.push_back(td);
  }
  catch ( ... ) {
    std::string e("\n\tIn makeDouble of OptAlignCSCFileReader failed to evaluate a double");
    e+= ts;
    std::cout << e << std::endl;
    //    throw std::exception(e);
  }
}

bool OptAlignCSCFileReader::getData( std::vector<float>& vecFloat ) {
  vecFloat.clear();
  for (std::vector<double>::const_iterator it = dblVec_.begin(); it != dblVec_.end(); it++) {
    vecFloat.push_back( float (*it) );
  }
  return true;
}

bool OptAlignCSCFileReader::getData( std::vector<double>& vecDouble ) {
  vecDouble = dblVec_;
  return true;
}

bool OptAlignCSCFileReader::getData( std::vector<int>& vecInt ) {
  vecInt.clear();
  for (std::vector<double>::const_iterator it = dblVec_.begin(); it != dblVec_.end(); it++) {
    vecInt.push_back( int (*it) );
  }
  return true;
}

bool OptAlignCSCFileReader::getData( std::vector<std::string>& vecStr) {
  vecStr = strVec_;
  return true;
}
