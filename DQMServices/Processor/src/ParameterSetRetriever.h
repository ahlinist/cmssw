#ifndef DQMEVF_PARAMETERSETRETRIEVER_H
#define DQMEVF_PARAMETERSETRETRIEVER_H

#include <string>

namespace dqmevf {
  
  class ParameterSetRetriever
  {
  public:
    ParameterSetRetriever(const std::string& in);
    std::string getAsString() const; 
    std::string getPathTableAsString() const; 
    std::string getModuleTableAsString() const; 
    std::string getHostString(const std::string &in, std::string modifier="") const; 
  private:
    std::string pset;
    std::string pathIndexTable;
    static const std::string fileheading;
    static const std::string dbheading;  
    static const std::string webheading;

  };

} // dqmevf

#endif
