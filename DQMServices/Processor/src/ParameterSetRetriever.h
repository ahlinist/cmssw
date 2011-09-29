#ifndef DQMEVF_PARAMETERSETRETRIEVER_H
#define DQMEVF_PARAMETERSETRETRIEVER_H

#include <string>
#include <iostream>

namespace dqmevf {

  class PyLineSimpleModifier;

  class ParameterSetRetriever
  {
  public:
    ParameterSetRetriever(const std::string& in, PyLineSimpleModifier *modifier = 0);
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

  class PyLineSimpleModifier {
  private:
    std::string varName_;
    std::string value_;
    bool applied;
    bool applyOnce;
  public:
    PyLineSimpleModifier(std::string varName,std::string value,bool once=true) {
      applied=false;
      varName_=varName;
      value_=value;
      applyOnce=once;
    }
    bool modifyRunType(std::string * line) {
      if (applied && applyOnce) return true;
      //size_t pos=line->find(varName_);
      size_t pos=line->find("runtype");
      if (pos==std::string::npos) return false;
      size_t repbegin = line->find("'");
      size_t repend   = line->rfind("'");
      if (repbegin==std::string::npos || repend==std::string::npos) return false;
      std::string ret = line->substr(0,repbegin+1) + value_ + line->substr(repend);
      *line = ret;
      return true;
    }
  };
} // dqmevf

#endif
