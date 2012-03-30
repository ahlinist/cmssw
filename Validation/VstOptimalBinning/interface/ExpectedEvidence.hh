#ifndef __ExpectedEvidence
#define __ExpectedEvidence

#include <string>
#include <cassert>

class ExpectedEvidence
{
public:
  ExpectedEvidence(double _expectedEvidence_h=0, double _expectedEvidence_b=0): 
    expectedEvidence_h(_expectedEvidence_h), expectedEvidence_b(_expectedEvidence_b) {};
  ExpectedEvidence& operator+=(double x) { expectedEvidence_h += x; expectedEvidence_b += x; return(*this); }
  ExpectedEvidence operator+(double x) { return(ExpectedEvidence(this->expectedEvidence_h + x, this->expectedEvidence_b + x)); };
  ExpectedEvidence& operator-=(double x) { expectedEvidence_h -= x; expectedEvidence_b -= x; return(*this); }
  ExpectedEvidence operator-(double x) { return(ExpectedEvidence(this->expectedEvidence_h - x, this->expectedEvidence_b - x)); };
  ExpectedEvidence& operator+=(const ExpectedEvidence& rhs) { expectedEvidence_h += rhs.expectedEvidence_h; expectedEvidence_b += rhs.expectedEvidence_b; return(*this); }
 
  double getExpectedEvidence(std::string type="") 
  { 
    assert((type=="")||(type=="h")||(type=="b")||(type=="sm"));
    if(type=="h")
      return(expectedEvidence_h);
    else if((type=="b")||(type=="sm"))
      return(expectedEvidence_b);
    else
      return((expectedEvidence_h+expectedEvidence_b)/2);
  }

private:
  double expectedEvidence_h, expectedEvidence_b;
};



#endif
