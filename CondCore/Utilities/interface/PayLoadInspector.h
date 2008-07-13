#ifndef  PayLoadInspector_H
#define  PayLoadInspector_H
#include "CondCore/DBCommon/interface/TypedRef.h"
#include "CondCore/IOVService/interface/IOVProxy.h"
#include <string>
#include <vector>

namespace cond {

  template<typename T>
  class ValueExtractor {
  public:
    typedef T Class;
    ValueExtractor(){}
    ValueExtractor(std::string const &, std::vector<int> const&){}
    void compute(Class const &){}
    std::vector<float> const & values() const { return m_values;}
  private:
    std::vector<float> m_values;
  };

  template<typename T>
  class PayLoadInspector {
  public:
    typedef T Class;
    typedef ValueExtractor<T> Extractor;
        
    PayLoadInspector() {}
    PayLoadInspector(const cond::IOVElement & elem) : 
      object(*elem.db(),elem.payloadToken()){}

    std::string print() const { return ""; }

    std::string summary() const {return ""; }

    void extract(Extractor & extractor){extractor.compute(*object); }

  private:
    cond::TypedRef<Class> object;    

  };

}

#endif //   PayLoadInspector_H
