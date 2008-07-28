#include <bitset>
#include <iostream>
#include <stdexcept>
#include "POOLCore/POOLContext.h"
#include "StorageSvc/DbType.h"
#include "StorageSvc/poolDb.h"
#include "DataSvc/IDataSvc.h"
#include "DataSvc/DataSvcFactory.h"
#include "FileCatalog/IFileCatalog.h"
#include "PersistencySvc/DatabaseConnectionPolicy.h"
#include "PersistencySvc/ISession.h"
#include "PersistencySvc/Placement.h"
#include "PersistencySvc/ITransaction.h"
#include "PersistencySvc/ISession.h"
#include "DataSvc/Ref.h"
#include "DataSvc/RefException.h"
#include "FileCatalog/IFileCatalog.h"
#include "CondFormats/Calibration/interface/mySiStripNoises.h"
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <cassert>
//#include "Cintex/Cintex.h"

typedef boost::minstd_rand base_generator_type;
int main(){
  //ROOT::Cintex::Cintex::Enable();
  
  pool::POOLContext::loadComponent( "SEAL/Services/MessageService" );
  pool::POOLContext::loadComponent("COND/Services/TBufferBlobStreamingService");
  pool::POOLContext::setMessageVerbosityLevel( seal::Msg::Error );  
  pool::Placement place;
  place.setDatabase("sqlite_file:strip.db", pool::DatabaseSpecification::PFN );
  place.setContainerName("mySiStripNoisesRcd");
  place.setTechnology(pool::POOL_RDBMS_HOMOGENEOUS_StorageType.type());
  pool::IFileCatalog* fileCatalog = new pool::IFileCatalog;
  fileCatalog->setWriteCatalog( "file:me.xml" );
  fileCatalog->connect();
  fileCatalog->start();
  pool::IDataSvc* datasvc = pool::DataSvcFactory::instance( fileCatalog );
  pool::DatabaseConnectionPolicy policy;
  policy.setWriteModeForNonExisting( pool::DatabaseConnectionPolicy::CREATE );
  policy.setWriteModeForExisting( pool::DatabaseConnectionPolicy::OVERWRITE);
  datasvc->session().setDefaultConnectionPolicy( policy );
  datasvc->transaction().start(pool::ITransaction::UPDATE);
  
  mySiStripNoises* myobj = new mySiStripNoises;
  unsigned int detidseed=1234;
  unsigned int bsize=100;
  unsigned int nAPV=2;
  base_generator_type rng(42u);
  boost::uniform_real<> uni_dist(0,1);
  boost::variate_generator<base_generator_type&, boost::uniform_real<> > uni(rng, uni_dist);
  for(unsigned int detid=detidseed; detid<(detidseed+bsize);detid++){
    std::vector<short> theSiStripVector;
    for(unsigned int strip=0; strip<128*nAPV; ++strip){
      std::cout<<strip<<std::endl;
      float noise = uni();;      
      myobj->setData(noise,theSiStripVector);
    }
    myobj->put(detid,theSiStripVector);
  }    
  std::cout<<"about to build ref"<<std::endl;
  pool::Ref< mySiStripNoises > simple(datasvc,myobj);
  std::cout<<"about to mark write"<<std::endl;
  simple.markWrite(place);
  std::cout<<"about to commit"<<std::endl;
  std::string t=simple.toString();
  std::cout<<"token "<<t<<std::endl;
  datasvc->transaction().commit();
  std::cout<<"committed"<<std::endl;
  datasvc->transaction().start(pool::ITransaction::READ);
  std::cout<<"about to read back"<<std::endl;
  pool::Ref< mySiStripNoises > p(datasvc,t);
  unsigned int a=p->v_noises.size();
  std::cout<<"size a "<<a<<std::endl;
  unsigned int b=p->indexes.size();
  std::cout<<"size b "<<b<<std::endl;
  datasvc->transaction().commit();
  datasvc->session().disconnectAll();
  fileCatalog->commit();
  fileCatalog->disconnect();
  return 0;
}
