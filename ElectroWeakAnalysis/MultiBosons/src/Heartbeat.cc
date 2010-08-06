#include "ElectroWeakAnalysis/MultiBosons/interface/Heartbeat.h"
#include <iomanip>
#include <cmath>

Heartbeat::Heartbeat(const unsigned long long& m,const double & p):
  precision(p),
  max(m),  
  current(1),
  last(1)
{
  outprec=0;
  while(p*std::pow(10,outprec-1) < 1) ++outprec;
  std::cout << "There are " << max << " events to process!" << std::endl;
}

void Heartbeat::update(std:: ostream& o) {

  if( ((double)current/max*100.0 - (double)last/max*100) >= precision || current == max  ) {
    
    o.setf(std::ios_base::fixed,std::ios_base::floatfield);
    o << std::setprecision(outprec) << "\rProcessing is " << ((double)current/max)*100.0 << "% complete..." << std::flush;
    o.unsetf(std::ios_base::fixed);
    o.unsetf(std::ios_base::floatfield);
    
    last = current;
    
    if(current == max) finish(o);  
  }  

  ++current;
}

void Heartbeat::finish(std::ostream& o) const {  
  o << std::endl << "-->Done processing!             " << std::endl; 
}
