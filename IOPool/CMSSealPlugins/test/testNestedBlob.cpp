#include "POOLCore/Exception.h"
#include "CondFormats/Calibration/interface/CalibHistograms.h"
#include "ObjectRelationalAccess/ObjectRelationalMappingUtilities.h"
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ITransaction.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Connection.h"
//#include "CondCore/DBCommon/interface/Exception.h"
//#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/TypedRef.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
int main(){
  std::string connect("sqlite_file:myhistos.db");
  std::cout << "##### processing custom mapping " << std::endl;
  pool::ObjectRelationalMappingUtilities utility;
  utility.initialize();
  utility.connect(connect,coral::Update);
  utility.session()->transaction().start(false);
  //blob version
  if(utility.existsMapping("CalibHistograms-2.0")){
    utility.removeMapping("CalibHistograms-2.0");
  }
  std::string inputFile("CalibHistograms-mapping-custom-2.0.xml");
  
  //default version
  //if(utility.existsMapping("CalibHistograms-1.0")){
  //  utility.removeMapping("CalibHistograms-1.0");
  //}
  //std::string inputFile("CalibHistograms-mapping-custom-1.0.xml");
  utility.buildAndMaterializeMapping(inputFile,"",false,false);
  utility.session()->transaction().commit();
  std::cout <<"Mapping built and stored." <<std::endl; 
  cond::DBSession* session=new cond::DBSession;
  try{
    std::cout<<"loading XMLAuthenticationService"<<std::endl;
    session->configuration().setAuthenticationMethod(cond::XML);
    std::cout<<"loading MessageService"<<std::endl;
    session->configuration().setMessageLevel(cond::Error);
    std::cout<<"loading BlobStreamingService"<<std::endl;
    session->configuration().setBlobStreamer("");
    std::cout<<"done"<<std::endl;
    /// The reference data
    const int numberOfHist = 10;
    CalibHistograms* histos=new CalibHistograms;
    for( int h = 0; h < numberOfHist; ++h){
      CalibHistogram* histo=new CalibHistogram;
      const int numberOfBins = 10;
      for ( int i = 0; i < numberOfBins; ++i ) {
	histo->m_histo.push_back(i+i*20/100);
      }
      histos->m_data.push_back(*histo);
    }
    session->open();
    cond::Connection myconnection(connect,0);
    myconnection.connect(session);
    cond::PoolTransaction& pooldb=myconnection.poolTransaction();
    std::cout << "##### reading back data from POOL database " << std::endl;
    pooldb.start(false);
    cond::TypedRef<CalibHistograms> myref(pooldb,histos);
    myref.markWrite("CalibHistograms");
    pooldb.commit();
    std::cout<<"written"<<std::endl;
    pooldb.start(true);
    cond::ContainerIterator<CalibHistograms> histosIt(pooldb,"CalibHistograms");
    unsigned int numberOfObj = 0;
    while ( histosIt.next() ) {
      if(histosIt.dataRef()->m_data.size()!=histos->m_data.size()){
	std::cout<<"TEST ERROR: number of histo read back from CalibHistograms is not as expected"<<std::endl;
	std::cout <<"Size of object readout="<< histosIt.dataRef()->m_data.size()<<std::endl;
	std::cout <<"Size of object expected="<< histos->m_data.size()<<std::endl;
      }
      for(std::vector<CalibHistogram>::iterator it=histosIt.dataRef()->m_data.begin(); it!=histosIt.dataRef()->m_data.end();++it){
	std::cout<<"histo size "<<it->m_histo.size()<<std::endl;
      }
      numberOfObj++;
    }
    std::cout << "Read back " <<numberOfObj<< " CalibHistograms in total" <<std::endl;
    // commit the transaction
    pooldb.commit();
    myconnection.disconnect();
    delete session;
  }catch(const cond::Exception& er){
    std::cout<<er.what()<<std::endl;
  }catch(const pool::Exception& er){
    std::cout<<er.what()<<std::endl;
  }catch(const std::exception& er){
    std::cout<<er.what()<<std::endl;
  }
}
