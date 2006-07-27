#ifndef CondTools_InclinometersDataConverter_h
#define CondTools_InclinometersDataConverter_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondTools/Utilities/interface/CSVFieldMap.h"
#include <string>
#include <vector>
namespace edm{
  class ParameterSet;
  class Event;
  class EventSetup;
}
class InclinometersDataConverter : public edm::EDAnalyzer {
 public:
  explicit InclinometersDataConverter(const edm::ParameterSet& iConfig );
  ~InclinometersDataConverter(){}
  virtual void analyze( const edm::Event&, const edm::EventSetup& ){}
  virtual void endJob();
 private:
  CSVFieldMap m_fieldMap;
  std::string m_inFileName;
};
#endif
