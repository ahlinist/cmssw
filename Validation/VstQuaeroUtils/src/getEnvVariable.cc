#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include "Validation/VstQuaeroUtils/interface/getEnvVariable.hh"


std::string getEnvVariable(std::string nameOfVariable) {

  std::string envVariable="_unknown_";
  char* tmpName = (char*)nameOfVariable.c_str();
  if(getenv(tmpName)) {
    envVariable = getenv(tmpName);
  }
  else {
    std::cerr << "Can't find Environment Variable " << nameOfVariable << std::endl;
    //    throw std::runtime_error("Can't find environment variable");
  }

  return envVariable;
}
