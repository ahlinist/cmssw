#ifndef CondTools_DQM_DQMXMLFileSourceHandler_h
#define CondTools_DQM_DQMXMLFileSourceHandler_h

#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
//#include "CondFormats/Common/interface/Time.h"
#include "CondFormats/GeometryObjects/interface/GeometryFile.h"
#include <string>

namespace popcon {
  class DQMXMLFileSourceHandler : public popcon::PopConSourceHandler<GeometryFile> {
   public:
    DQMXMLFileSourceHandler(const edm::ParameterSet & pset);
    ~DQMXMLFileSourceHandler();
    void getNewObjects();
    std::string id() const;
   private:
    std::string m_name;
    std::string m_file;
    bool m_zip;
    //cond::Time_t m_since;
    boost::uint64_t m_since;
    bool m_debugMode;
  };
}

#endif
