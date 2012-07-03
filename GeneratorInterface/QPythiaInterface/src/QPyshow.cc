
#include "GeneratorInterface/QPythiaInterface/interface/QPythiaWrapper.h"

extern "C" {
  void pyshow_(int& ip1, int& ip2, double& qmax)
  {
    return QPYSHOW(ip1,ip2,qmax);
  }
}

