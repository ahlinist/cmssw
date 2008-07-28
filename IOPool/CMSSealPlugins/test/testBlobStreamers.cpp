#include "POOLCore/Exception.h"
#include "CondFormats/Calibration/interface/BlobPedestals.h"
#include "CondFormats/Calibration/interface/BlobComplex.h"
//#include "ObjectRelationalAccess/ObjectRelationalMappingUtilities.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ObjectRelationalMappingUtility.h"
#include "CondCore/DBCommon/interface/AuthenticationMethod.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/DBCommon/interface/SessionConfiguration.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/TypedRef.h"
#include "CondCore/DBCommon/interface/ContainerIterator.h"
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ITransaction.h"

// template class to instantiate test routines for different payload classes.

template<class Content>
class BlobStreamerTest {
  public:
    BlobStreamerTest(const char *blobStreamer) : blobStreamer(blobStreamer) {}

    void test();
 
  private:
    const char *blobStreamer;

    // the following members must be specialized

    Content *createObject() const;
    void testObject(Content *read, Content *orig) const;

    static const char *mapping;
    static const char *mappingFile;
    static const char *className;
};

// template implementations for BlobPedestals

template<> const char *
BlobStreamerTest<BlobPedestals>::mapping ="basic";
template<> const char *
BlobStreamerTest<BlobPedestals>::mappingFile = "BlobPedestals_basic_0.xml";
template<> const char *
BlobStreamerTest<BlobPedestals>::className = "BlobPedestals";

template<> BlobPedestals *
BlobStreamerTest<BlobPedestals>::createObject() const
{
  BlobPedestals* bped=new BlobPedestals;
  const int numberOfChannels = 10;
  for ( int i = 0; i < numberOfChannels; ++i ) {
    bped->m_pedestals.push_back(i+1000);
  }
  return bped;
}

template<> void
BlobStreamerTest<BlobPedestals>::testObject(BlobPedestals *read,
                                            BlobPedestals *orig) const
{
  if(read->m_pedestals.size()!=orig->m_pedestals.size()){
    std::cout<<"TEST ERROR: BlobPedestals objects read back differently for vector content."<<std::endl;
    std::cout <<"Size of BlobPedestals  readout="<< read->m_pedestals.size()<<std::endl;
    std::cout <<"Size of BlobPedestals  expected="<< orig->m_pedestals.size()<<std::endl;
  }

  for(std::vector<unsigned int>::iterator it=read->m_pedestals.begin(); it!=read->m_pedestals.end();++it){
    std::cout<<"read back "<<*it<<std::endl;
  }
}

// template implementations for BlobComplex

template<> const char *
BlobStreamerTest<BlobComplex>::mapping ="basic";
template<> const char *
BlobStreamerTest<BlobComplex>::mappingFile ="BlobComplex_basic_0.xml";
template<> const char *
BlobStreamerTest<BlobComplex>::className = "BlobComplex";

template<> BlobComplex *
BlobStreamerTest<BlobComplex>::createObject() const
{
  BlobComplex* obj = new BlobComplex;
  unsigned int serial = 111;
  obj->fill(serial);
  return obj;
}

template<> void
BlobStreamerTest<BlobComplex>::testObject(BlobComplex *read,
                                          BlobComplex *orig) const
{
  if (*read != *orig)
    std::cout<<"TEST ERROR: BlobComplex objects read back differently."<<std::endl;
  read->print();
}

// templated method for BlobStreamer tests on template argument payload

template<class Content>
void BlobStreamerTest<Content>::test()
{
  std::cout << "testing blobStreamer \"" << blobStreamer << "\"" << std::endl;
  std::string connect("sqlite_file:mydb.db");
  cond::DBSession* session=new cond::DBSession;
  try{
    std::cout<<"loading XMLauthenticationService"<<std::endl;
    session->configuration().setAuthenticationMethod(cond::XML);
    std::cout<<"loading MessageService"<<std::endl;
    session->configuration().setMessageLevel(cond::Error);
    std::cout<<"done"<<std::endl;
    std::cout<<"loading BlobStreamingService"<<std::endl;
    session->configuration().setBlobStreamer(blobStreamer);
    std::cout<<"done"<<std::endl;
    /// The reference data
    Content *obj = createObject();
    cond::Connection myconnection(connect,100);
    session->open();
    myconnection.connect(session);
    //std::cout << "##### processing custom mapping " << std::endl;
    //cond::CoralTransaction& coraldb=myconnection.coralTransaction();
    //std::cout<<1<<std::endl;
    /*coraldb.start(false); 
    std::cout<<2<<std::endl;
    cond::ObjectRelationalMappingUtility* mappingUtil=new cond::ObjectRelationalMappingUtility(&(coraldb.coralSessionProxy()) );
    std::cout<<3<<std::endl;
    if( !mappingUtil->existsMapping(BlobStreamerTest<Content>::mapping) ){
      std::cout<<"inside "<<std::string(BlobStreamerTest<Content>::mappingFile)<<std::endl;
      mappingUtil->buildAndStoreMappingFromFile(BlobStreamerTest<Content>::mappingFile);
      std::cout<<"stored"<<std::endl;
    }
    std::cout<<"about to commit"<<std::endl;
    coraldb.commit(); 
    delete mappingUtil;
    */
    cond::PoolTransaction& pooldb=myconnection.poolTransaction();
    pooldb.start(false);
    cond::TypedRef<Content> myref(pooldb,obj);
    myref.markWrite(className);
    pooldb.commit();
    pooldb.start(true);
    cond::ContainerIterator<Content> objs(pooldb,className);
    unsigned int numberOfObj = 0;
    while ( objs.next() ) {
      testObject(&*objs.dataRef(), obj);
      numberOfObj++;
    }
    std::cout << "Read back " << numberOfObj
              << " objects of type " << className << std::endl;
    // commit the transaction
    pooldb.commit();
    myconnection.disconnect();
    delete session;
  }catch(const cond::Exception& er){
    std::cout<<er.what()<<std::endl;
  }catch(const pool::Exception& er){
    std::cout<<er.what()<<std::endl;
  }
}

int main()
{
  // test the default BlobStreamingService that can stream
  // STL vectors of simple data types (fixed-size, no further nesting)
  BlobStreamerTest<BlobPedestals>(
   "COND/Services/DefaultBlobStreamingService").test();

  // test the ROOT-based TBuffer BlobStreamer that can stream
  // virtually any payload that has a dictionary, test on complex object
  //BlobStreamerTest<BlobComplex>(
  //		"COND/Services/TBufferBlobStreamingService").test();

  return 0;
}
