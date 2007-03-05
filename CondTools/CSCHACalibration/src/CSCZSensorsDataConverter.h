#ifndef CSCZSensorsDataConverter_h
#define CSCZSensorsDataConverter_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondCore/Utilities/interface/CSVFieldMap.h"
#include <string>
#include <vector>
namespace edm{
  class ParameterSet;
  class Event;
  class EventSetup;
}
class CSCZSensorsDataConverter : public edm::EDAnalyzer {
 public:
  explicit CSCZSensorsDataConverter(const edm::ParameterSet& iConfig );
  ~CSCZSensorsDataConverter(){}
  virtual void analyze( const edm::Event&, const edm::EventSetup& ){}
  virtual void endJob();
 private:
  CSVFieldMap m_fieldMap;
  std::string m_inFileName;
};
#endif
