
/** \class L1RPCHwConfigBuilder
 *
 * Description:
 *      Class to read directly OMDS DB with OCCI and fill Offline DB
 *
 * $Date: 2008/03/05 12:25:48 $
 * $Revision: 1.3 $
 *
 */

#include "CondFormats/RPCObjects/interface/L1RPCHwConfig.h"


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <time.h>
#include "OnlineDB/Oracle/interface/Oracle.h"

using namespace std;
using namespace oracle::occi;

class L1RPCHwConfigBuilder : public edm::EDAnalyzer {
  
public:
  explicit L1RPCHwConfigBuilder(const edm::ParameterSet& iConfig);
  ~L1RPCHwConfigBuilder();
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){}
  void ConnectOnlineDB(string host, string sid, string user, string pass, int port);
  void DisconnectOnlineDB();
  void WritePoolDB();
  void readHwConfig();

private:
  Environment* env;
  Connection* conn;

  L1RPCHwConfig* disabledDevs;
  struct aSegment {
    int tower;
    int sector;
    int segment;
  };
  std::vector<aSegment> theDisabledSegments;

  string record_;
  string tag_;

  // utilities
  string IntToString(int num)
  {
    stringstream snum;
    snum << num << flush;

    return(snum.str());
  }

};

L1RPCHwConfigBuilder::L1RPCHwConfigBuilder(const edm::ParameterSet& iConfig) 
  : record_(iConfig.getParameter<std::string>("record")),
    tag_(iConfig.getUntrackedParameter<std::string>("tag",(string)"L1RPCHwConfig_v1"))
{
  cout<<"CTor called"<<endl;

  string host = iConfig.getParameter<std::string>("host"); 
  string sid = iConfig.getParameter<std::string>("sid"); 
  string user = iConfig.getParameter<std::string>("user"); 
  string pass = iConfig.getParameter<std::string>("pass"); 
  unsigned int port = iConfig.getParameter<unsigned int>("port"); 
  ConnectOnlineDB(host, sid, user, pass, port);

}

L1RPCHwConfigBuilder::~L1RPCHwConfigBuilder()
{ 
  cout<<"DTor called"<<endl;
}

void L1RPCHwConfigBuilder::endJob()
{ 
  try{
    DisconnectOnlineDB();
    WritePoolDB();
  }
  catch (SQLException &e) {
    cerr << "SQLException:  " << e.getMessage() << endl;
  } catch (std::exception &e) {
    cerr << "std::exception:  " << e.what() << endl;
  } catch (...) {
    cerr << "Unknown exception caught" << endl;
  }
}

void L1RPCHwConfigBuilder::beginJob(const edm::EventSetup& iSetup)
{ 
  try{
    readHwConfig();
  }
  catch (SQLException &e) {
    cerr << "SQLException:  " << e.getMessage() << endl;
  } catch (std::exception &e) { 
    cout <<"std::exception:  "<< e.what(); 
  } catch (...) { 
    cout << "Unknown error caught "<<endl; 
  }
}

void L1RPCHwConfigBuilder::ConnectOnlineDB(string host, string sid, string user, string pass, int port=1521)
{
  stringstream ss;
  ss << "//" << host << ":" << port << "/" << sid;
    
  cout << "Connecting to " << host << "..." << flush;
  env = Environment::createEnvironment(Environment::OBJECT);
  conn = env->createConnection(user, pass, ss.str());
  cout << "Done." << endl;
}

void L1RPCHwConfigBuilder::DisconnectOnlineDB()
{
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
}

void L1RPCHwConfigBuilder::WritePoolDB()
{
  cout << endl << "Start writing to PoolDB" << flush << endl;

  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if( !mydbservice.isAvailable() ){
    cout<<"DB service unavailable"<<endl;
    return;
  } else { 
    cout<<"DB service OK"<<endl; 
  }
  
  int iov=mydbservice->currentTime();
  std::cout<<"Current IOV is "<<iov<<std::endl;
  mydbservice->writeOne<L1RPCHwConfig>(disabledDevs, iov, record_);
  cout<<"... all done, end"<<endl;
}
  
// Method called to read Online DB and fill L1RPCHwConfig
void L1RPCHwConfigBuilder::readHwConfig()
{

  cout << "Reading map with mode 0:  Basic OCCI" << endl;

  Statement* stmt = conn->createStatement();
  string sqlQuery ="";

  cout << endl <<"Start to build L1RPC HwConfig" << flush << endl << endl;
  disabledDevs =  new L1RPCHwConfig();

  std::pair<int,int> aTowSec;
  std::vector<std::pair<int,int> > theDisabledTowSecs;
  aSegment logTowSecSeg;

// get disabled crates and translate into towers/sectors/segments
  sqlQuery = "select tb.towerto, tb.towerfrom, tb.sector ";
  sqlQuery += "from CRATEDISABLED cd, CRATE c, BOARD b, TRIGGERBOARD tb ";
  sqlQuery += "where c.crateid=cd.crate_crateid and b.crate_crateid=c.crateid and b.boardid=tb.triggerboardid and c.type='TRIGGERCRATE' ";
  sqlQuery += "order by tb.towerto, tb.sector ";
  stmt->setSQL(sqlQuery.c_str());
  ResultSet* rset = stmt->executeQuery();
  while (rset->next()) {
    std::cout<<"  found disabled crate..."<<std::endl;
    aTowSec.second=atoi((rset->getString(3)).c_str());
    logTowSecSeg.sector=atoi((rset->getString(3)).c_str());
    int first=atoi((rset->getString(1)).c_str());
    int last=atoi((rset->getString(2)).c_str());
    for (int iTower=first; iTower<=last; iTower++) {
      aTowSec.first=iTower;
      logTowSecSeg.tower=iTower;
      theDisabledTowSecs.push_back(aTowSec);
      for (int jSegment=0; jSegment<12; jSegment++) {
        logTowSecSeg.segment=jSegment;
        theDisabledSegments.push_back(logTowSecSeg);
      }
    }
  }
// get disabled triggerboards and translate into towers/sectors/segments
  sqlQuery = "select tb.towerto, tb.towerfrom, tb.sector ";
  sqlQuery += "from BOARDDISABLED bd, BOARD b, TRIGGERBOARD tb ";
  sqlQuery += "where b.boardid=bd.board_boardid and b.boardid=tb.triggerboardid ";
  sqlQuery += "order by tb.towerto, tb.sector ";
  stmt->setSQL(sqlQuery.c_str());
  rset = stmt->executeQuery();
  while (rset->next()) {
    std::cout<<"  found disabled board..."<<std::endl;
    aTowSec.second=atoi((rset->getString(3)).c_str());
    logTowSecSeg.sector=atoi((rset->getString(3)).c_str());
    int first=atoi((rset->getString(1)).c_str());
    int last=atoi((rset->getString(2)).c_str());
    for (int iTower=first; iTower<=last; iTower++) {
      aTowSec.first=iTower;
      logTowSecSeg.tower=iTower;
      theDisabledTowSecs.push_back(aTowSec);
      for (int jSegment=0; jSegment<12; jSegment++) {
        logTowSecSeg.segment=jSegment;
        theDisabledSegments.push_back(logTowSecSeg);
      }
    }
  }
// get disabled chips and translate into towers/sectors
// how to translate chip positions into logical segments?
  sqlQuery = "select tb.towerto, tb.towerfrom, tb.sector, c.position ";
  sqlQuery += "from CHIPDISABLED cd, CHIP c, BOARD b, TRIGGERBOARD tb ";
  sqlQuery += " where cd.chip_chipid=c.chipid and c.board_boardid=b.boardid and b.boardid=tb.triggerboardid and c.type='PAC' ";
//  sqlQuery += " where cd.chip_chipid=c.chipid and c.board_boardid=b.boardid and b.boardid=tb.triggerboardid and (c.type='PAC' or c.type='OPTO') ";
  sqlQuery += "order by tb.towerto, tb.sector, c.position ";
  stmt->setSQL(sqlQuery.c_str());
  rset = stmt->executeQuery();
  while (rset->next()) {
    std::cout<<"  found disabled chip..."<<std::endl;
    aTowSec.second=atoi((rset->getString(3)).c_str());
    logTowSecSeg.sector=atoi((rset->getString(3)).c_str());
    int first=atoi((rset->getString(1)).c_str());
    int last=atoi((rset->getString(2)).c_str());
    int chipPos=rset->getInt(4);
// for the moment assume that chip position 8 corresponds to lowest tower number
// and so on, unless there are more chips than towers for this TB, then the last
// tower is associated with the last two chips (this is certainly wrong).
    int towerNum=first+chipPos-8;
    if (towerNum>last) towerNum--;
    aTowSec.first=towerNum;
    logTowSecSeg.tower=towerNum;
    for (int jSegment=0; jSegment<12; jSegment++) {
      logTowSecSeg.segment=jSegment;
      theDisabledSegments.push_back(logTowSecSeg);
    }
  }

  cout << endl <<"Building L1RPC HwConfig done!" << flush << endl << endl;

}


//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1RPCHwConfigBuilder);

