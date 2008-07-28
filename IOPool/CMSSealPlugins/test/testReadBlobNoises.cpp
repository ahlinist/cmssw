#include "POOLCore/Exception.h"
#include "CondFormats/Calibration/interface/BlobNoises.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/Connection.h"
//#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/TypedRef.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include <boost/program_options.hpp>
//#include <stdexcept.h>
int main(int argc, char *argv[]){
  boost::program_options::options_description desc("options");
  boost::program_options::options_description visible("Usage: testReadBlobNoises [options] \n");
  visible.add_options()
    ("connect,c",boost::program_options::value<std::string>(),"connection string(required)")
    ("help,h", "help message")
    ;
  desc.add(visible);
  boost::program_options::variables_map vm;
  try{
    boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).run(), vm);
    boost::program_options::notify(vm);
  }catch(const boost::program_options::error& er) {
    std::cerr << er.what()<<std::endl;
    return 1;
  }
  if (vm.count("help")) {
    std::cout << visible <<std::endl;;
    return 0;
  }
  if(!vm.count("connect")){
    std::cerr <<"[Error] no connect[c] option given \n";
    std::cerr<<" please do "<<argv[0]<<" --help \n";
    return 1;
  }
  std::string connect=vm["connect"].as<std::string>();
  cond::DBSession* session=new cond::DBSession;
  try{
    std::cout<<"loading XMLAuthenticationService"<<std::endl;
    session->configuration().setAuthenticationMethod(cond::XML);
    std::cout<<"loading MessageService"<<std::endl;
    session->configuration().setMessageLevel(cond::Error);
    std::cout<<"loading BlobStreamingService"<<std::endl;
    session->configuration().setBlobStreamer("COND/Services/DefaultBlobStreamingService");
    cond::Connection myconnection(connect,0);
    session->open();
    myconnection.connect(session);
    cond::PoolTransaction& pooldb=myconnection.poolTransaction();
    pooldb.start(false);
    cond::ContainerIterator<BlobNoises> data(pooldb,"BlobNoises");
    unsigned int counter = 0;
    while ( data.next() ) {
      int s=data.dataRef()->v_noises.size();
      std::cout<<"datasize "<<s<<std::endl;
      counter++;
    }
    std::cout<<"Read back "<<counter<<" object of type BlobNoises"<<std::endl;
    pooldb.commit();
    myconnection.disconnect();
    delete session;
  }catch(const cond::Exception& er){
    std::cout<<er.what()<<std::endl;
  }catch(const pool::Exception& er){
    std::cout<<er.what()<<std::endl;
  }
}
