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


#include "CondFormats/OptAlignObjects/interface/Inclinometers.h"
#include "CondFormats/DataRecord/interface/InclinometersRcd.h"

using namespace std;

namespace condtooltest
{
  class InclinometersDataPrinter : public edm::EDAnalyzer
  {
  public:
    explicit  InclinometersDataPrinter(edm::ParameterSet const& p) 
    { }
    virtual ~InclinometersDataPrinter() {}
    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  private:
  };
  
  void
   InclinometersDataPrinter::analyze(const edm::Event& e, const edm::EventSetup& context)
  {
    using namespace edm::eventsetup;
    edm::ESHandle<Inclinometers> pPeds;
    context.get<InclinometersRcd>().get(pPeds);
    const Inclinometers* myped=pPeds.product();
    std::vector<Inclinometers::Item>::const_iterator it;
    for( it=myped->m_inclinometers.begin();it!=myped->m_inclinometers.end(); ++it ){
      std::cout<<"--------------------------------\t"<<'\n';
      std::cout<<"sensor type "<<it->Sensor_type<<'\n';
      std::cout<<"sensor no "<<it->Sensor_number<<'\n';
      std::cout<<"ME layer "<<it->ME_layer<<'\n';
      std::cout<<"logical AlignmentName "<<it->Logical_Alignment_Name<<'\n';
      std::cout<<"cern Designator "<<it->CERN_Designator<<'\n';
      std::cout<<"cern Barcode "<<it->CERN_Barcode<<'\n';
      std::cout<<"inclination Direction "<< it->Inclination_Direction<<'\n';
      std::cout<<"abs slope "<<it->Abs_Slope<<'\n';
      std::cout<<"abs slope error "<<it->Abs_Slope_Error<<'\n';
      std::cout<<"norm slope "<<it->Norm_Slope<<'\n';
      std::cout<<"norm slope error "<<it->Norm_Slope_Error<<'\n';
      std::cout<<"abs Intercept "<<it->Abs_Intercept<<'\n';
      std::cout<<"abs Intercept error "<<it->Abs_Intercept_Error<<'\n';
      std::cout<<"norm Intercept "<<it->Norm_Intercept<<'\n';
      std::cout<<"norm Intercept error "<<it->Norm_Intercept_Error<<'\n';
      std::cout<<"shifts "<<it->Shifts_due_to_shims_etc<<'\n';
    }
    std::cout<<std::endl;
  }
  DEFINE_FWK_MODULE(InclinometersDataPrinter);
}
