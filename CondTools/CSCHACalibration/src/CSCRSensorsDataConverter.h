#ifndef CondTools_CSCRSensorsDataConverter_h
#define CondTools_CSCRSensorsDataConverter_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondCore/Utilities/interface/CSVFieldMap.h"
#include <string>
#include <vector>
namespace edm{
  class ParameterSet;
  class Event;
  class EventSetup;
}
class CSCRSensorsDataConverter : public edm::EDAnalyzer {
 public:
  explicit CSCRSensorsDataConverter(const edm::ParameterSet& iConfig );
  ~CSCRSensorsDataConverter(){}
  virtual void analyze( const edm::Event&, const edm::EventSetup& ){}
  virtual void endJob();
 private:
  CSVFieldMap m_fieldMap;
  std::string m_inFileName;
};
#endif
