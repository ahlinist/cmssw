#ifndef _ElectroWeakAnalysisMultibosons_Heartbeat_h_
#define _ElectroWeakAnalysisMultibosons_Heartbeat_h_

#include <iostream>

class Heartbeat {
  
 public:
  Heartbeat(const unsigned long long&,const double&);
  
  void reset() {current=1; last=1;}

  void update(std::ostream&);

 private:
  
  const double precision;
  const unsigned long long max;
  unsigned long long current,outprec;
  unsigned long long last;

  void finish(std::ostream& ) const;
};

#endif
