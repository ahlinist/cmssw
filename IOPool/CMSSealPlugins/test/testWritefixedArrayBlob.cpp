#include "ObjectRelationalAccess/ObjectRelationalMappingUtilities.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondFormats/Calibration/interface/mypt.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/TypedRef.h"
#include <boost/program_options.hpp>
#include <stdexcept>
#include <string>
#include <iostream>

int main(int argc, char *argv[]){
  boost::program_options::options_description desc("options");
  boost::program_options::options_description visible("Usage: testWritefixedArrayBlob [options] \n");
  visible.add_options()
    ("connect,c",boost::program_options::value<std::string>(),"connection string(required)")
    //("mappingfile,m",boost::program_options::value<std::string>(),"mapping file(required)")
    ("nobj,n",boost::program_options::value<size_t>(),"number of objects to write(required)") 
    //("objsize,s",boost::program_options::value<unsigned int>(),"size of the blob vector(required)")
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
  /*if(!vm.count("mappingfile")){
    std::cerr <<"[Error] no mappingfile[m] option given \n";
    std::cerr<<" please do "<<argv[0]<<" --help \n";
    return 1;
    }*/
  if(!vm.count("nobj")){
    std::cerr <<"[Error] no nobj[n] option given \n";
    std::cerr<<" please do "<<argv[0]<<" --help \n";
    return 1;
  } 
  /*if(!vm.count("objsize")){
    std::cerr <<"[Error] no objsize[s] option given \n";
    std::cerr<<" please do "<<argv[0]<<" --help \n";
    return 1;
  }
  */
  std::string connect=vm["connect"].as<std::string>();
  //std::string mappingFile=vm["mappingfile"].as<std::string>();
  size_t numberOfObj=vm["nobj"].as<size_t>();
  //unsigned int bsize=vm["objsize"].as<unsigned int>();
  //  pool::ObjectRelationalMappingUtilities utility;
  //utility.setProperty(pool::ObjectRelationalMappingUtilities::connectionStringProperty(),connect);
  try{
    cond::DBSession* session=new cond::DBSession;
    std::cout<<"loading XMLauthenticationService"<<std::endl;
    session->configuration().setAuthenticationMethod(cond::XML);
    std::cout<<"loading MessageService"<<std::endl;
    session->configuration().setMessageLevel(cond::Error);
    std::cout<<"done"<<std::endl;
    std::cout<<"loading BlobStreamingService"<<std::endl;
    session->configuration().setBlobStreamer("COND/Services/TBufferBlobStreamingService");
    std::cout<<"done"<<std::endl;
    cond::Connection myconnection(connect,0);
    session->open();
    myconnection.connect(session);
    cond::PoolTransaction& pooldb=myconnection.poolTransaction();
    pooldb.start(false);
    //std::cout << "##### processing custom mapping " << std::endl;
    //if(utility.existsMapping(mappingFile)){
    //  utility.removeMapping(mappingFile);
    //}
    //utility.buildAndMaterializeMapping(mappingFile,"",false,false);
    unsigned int i;
    pooldb.start(false);
    std::cout<<"numberOfObj "<<numberOfObj<<std::endl;
    for (i = 0; i <numberOfObj ; ++i ){
      std::cout<<"i "<<i<<std::endl;
      mypt* me = new mypt;      
      cond::TypedRef< mypt > myref(pooldb,me);
      std::cout<<"filling "<<std::endl;
      me->fill();
      std::cout<<"filled "<<std::endl;
      myref.markWrite("mypt");
    }
    pooldb.commit();
    myconnection.disconnect();
    delete session;
  }catch (const cond::Exception& e){
    std::cout<<e.what()<<std::endl;
  }
}
