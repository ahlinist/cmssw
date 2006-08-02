#ifndef CondTools_PXsensorsDataConverter_h
#define CondTools_PXsensorsDataConverter_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondTools/Utilities/interface/CSVFieldMap.h"
#include <string>
#include <vector>
namespace edm{
  class ParameterSet;
  class Event;
  class EventSetup;
}
class PXsensorsDataConverter : public edm::EDAnalyzer {
 public:
  explicit PXsensorsDataConverter(const edm::ParameterSet& iConfig );
  ~PXsensorsDataConverter(){}
  virtual void analyze( const edm::Event&, const edm::EventSetup& ){}
  virtual void endJob();
 private:
  CSVFieldMap m_fieldMap;
  std::string m_inFileName;
};
#endif
