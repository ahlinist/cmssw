//#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/OptAlignObjects/interface/CSCZSensors.h"
#include "CondFormats/DataRecord/interface/CSCZSensorsRcd.h"

using namespace std;

namespace condtooltest
{
  class CSCZSensorsDataPrinter : public edm::EDAnalyzer
  {
  public:
    explicit  CSCZSensorsDataPrinter(edm::ParameterSet const& p) 
    { }
    virtual ~CSCZSensorsDataPrinter() {}
    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  private:
  };
  
  void
   CSCZSensorsDataPrinter::analyze(const edm::Event& e, const edm::EventSetup& context)
  {
    using namespace edm::eventsetup;
    edm::ESHandle<CSCZSensors> pPeds;
    context.get<CSCZSensorsRcd>().get(pPeds);

    const CSCZSensors* myped=pPeds.product();
    std::vector<CSCZSensorData>::const_iterator it;
    for( it=myped->cscZSens_.begin();it!=myped->cscZSens_.end(); ++it ){
      std::cout<<"--------------------------------\t"<<'\n';
      std::cout<<"sensor type "<<it->sensorType_<<'\n';
      std::cout<<"sensor no "<<it->sensorNo_<<'\n';
      std::cout<<"ME layer "<<it->meLayer_<<'\n';
      std::cout<<"logical AlignmentName "<<it->logicalAlignmentName_<<'\n';
      std::cout<<"cern Designator "<<it->cernDesignator_<<'\n';
      std::cout<<"cern Barcode "<<it->cernBarcode_<<'\n';
      std::cout<<"abs slope "<<it->absSlope_<<'\n';
      std::cout<<"abs slope error "<<it->absSlopeError_<<'\n';
      std::cout<<"norm slope "<<it->normSlope_<<'\n';
      std::cout<<"norm slope error "<<it->normSlopeError_<<'\n';
      std::cout<<"abs Intercept "<<it->absIntercept_<<'\n';
      std::cout<<"abs Intercept error "<<it->absInterceptError_<<'\n';
      std::cout<<"norm Intercept "<<it->normIntercept_<<'\n';
      std::cout<<"norm Intercept error "<<it->normInterceptError_<<'\n';
      std::cout<<"shifts "<<it->shifts_<<'\n';
    }
    std::cout<<std::endl;
  }
  DEFINE_FWK_MODULE(CSCZSensorsDataPrinter);
}
