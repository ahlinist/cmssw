
/** \class RPCEMapBuilder
 *
 * Description:
 *      Class to read directly OMDS DB with OCCI and fill Offline DB
 *
 * $Date: 2008/01/22 19:06:34 $
 * $Revision: 1.2 $
 *
 */

#include "CondFormats/RPCObjects/interface/RPCEMap.h"
#include "CondFormats/RPCObjects/interface/FebLocationSpec.h"
#include "CondFormats/RPCObjects/interface/ChamberLocationSpec.h"
#include "CondFormats/RPCObjects/interface/DBSpecToDetUnit.h"


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

class RPCEMapBuilder : public edm::EDAnalyzer {
  
public:
  explicit RPCEMapBuilder(const edm::ParameterSet& iConfig);
  ~RPCEMapBuilder();
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){}
  void ConnectOnlineDB(string host, string sid, string user, string pass, int port);
  void DisconnectOnlineDB();
  void WritePoolDB();
  void readCablingMap();

private:
  Environment* env;
  Connection* conn;

  RPCEMap* eMap;

  string record_;
  string tag_;
  string version_;

  // utilities
  string IntToString(int num)
  {
    stringstream snum;
    snum << num << flush;

    return(snum.str());
  }

  typedef struct{int febId,chamberId,connectorId,lbInputNum,posInLocalEtaPart,posInCmsEtaPart;string localEtaPart,cmsEtaPart;} FEBStruct;

};

RPCEMapBuilder::RPCEMapBuilder(const edm::ParameterSet& iConfig) 
  : record_(iConfig.getParameter<std::string>("record")),
    tag_(iConfig.getUntrackedParameter<std::string>("tag",(string)"RPCEMap_v1")),
    version_(iConfig.getUntrackedParameter<std::string>("version",(string)"CreationTime"))
{
  cout<<"CTor called"<<endl;

  if(version_==(string)"CreationTime"){
    time_t rawtime;
    time(&rawtime); //time since January 1, 1970
    tm * ptm = gmtime(&rawtime);//GMT time
    char buffer[20];
    strftime(buffer,20,"%d/%m/%Y_%H:%M:%S",ptm);
    version_=(string)buffer;
  }

  string host = iConfig.getParameter<std::string>("host"); 
  string sid = iConfig.getParameter<std::string>("sid"); 
  string user = iConfig.getParameter<std::string>("user"); 
  string pass = iConfig.getParameter<std::string>("pass"); 
  unsigned int port = iConfig.getParameter<unsigned int>("port"); 
  ConnectOnlineDB(host, sid, user, pass, port);

  ::putenv("CORAL_AUTH_USER=blah");
  ::putenv("CORAL_AUTH_PASSWORD=blaah"); 

}

RPCEMapBuilder::~RPCEMapBuilder()
{ 
  cout<<"DTor called"<<endl;
}

void RPCEMapBuilder::endJob()
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

void RPCEMapBuilder::beginJob(const edm::EventSetup& iSetup)
{ 
  try{
    readCablingMap();
  }
  catch (SQLException &e) {
    cerr << "SQLException:  " << e.getMessage() << endl;
  } catch (std::exception &e) { 
    cout <<"std::exception:  "<< e.what(); 
  } catch (...) { 
    cout << "Unknown error caught "<<endl; 
  }
}

void RPCEMapBuilder::ConnectOnlineDB(string host, string sid, string user, string pass, int port=1521)
{
  stringstream ss;
  ss << "//" << host << ":" << port << "/" << sid;
    
  cout << "Connecting to " << host << "..." << flush;
  env = Environment::createEnvironment(Environment::OBJECT);
  conn = env->createConnection(user, pass, ss.str());
  cout << "Done." << endl;
}

void RPCEMapBuilder::DisconnectOnlineDB()
{
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
}

void RPCEMapBuilder::WritePoolDB()
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
  mydbservice->writeOne<RPCEMap>(eMap, iov, record_);
  cout<<"... all done, end"<<endl;
}
  
// Method called to read Online DB and fill RPCEMap
void RPCEMapBuilder::readCablingMap()
{

  cout << "Reading map with mode 0:  Basic OCCI" << endl;

  Statement* stmt = conn->createStatement();
  string sqlQuery ="";

  cout << endl <<"Start to build RPC e-Map..." << flush << endl << endl;
  eMap =  new RPCEMap(version_);

  // Get FEDs
  RPCEMap::dccItem thisDcc;
  sqlQuery=" SELECT DCCBoardId, FEDNumber FROM DCCBoard WHERE DCCBoardId>0 ORDER BY FEDNumber ";
  stmt->setSQL(sqlQuery.c_str());
  ResultSet* rset = stmt->executeQuery();
  std::pair<int,int> tmp_tbl;
  std::vector< std::pair<int,int> > theDAQ;
  while (rset->next()) {
    tmp_tbl.first=rset->getInt(1);
    tmp_tbl.second=rset->getInt(2);
    theDAQ.push_back(tmp_tbl);
  }
  for(unsigned int iFED=0;iFED<theDAQ.size();iFED++) {
    thisDcc.theId=theDAQ[iFED].second;
    std::vector<std::pair<int,int> > theTB;
// get TBs
    RPCEMap::tbItem thisTB;
    sqlQuery = " SELECT TriggerBoardId, DCCInputChannelNum FROM TriggerBoard ";
    sqlQuery += " WHERE DCCBoard_DCCBoardId= ";
    sqlQuery += IntToString(theDAQ[iFED].first);
    sqlQuery += " ORDER BY DCCInputChannelNum ";
    stmt->setSQL(sqlQuery.c_str());
    rset = stmt->executeQuery();
    int ntbs=0;
    while (rset->next()) {
      ntbs++;
      tmp_tbl.first=rset->getInt(1);
      tmp_tbl.second=rset->getInt(2);
      theTB.push_back(tmp_tbl);
    }
    for(unsigned int iTB=0;iTB<theTB.size();iTB++) {
      thisTB.theNum=theTB[iTB].second;
      thisTB.theMaskedLinks=0;
      std::vector<std::pair<int,int> > theLink;
// get links
      RPCEMap::linkItem thisLink;
      sqlQuery = " SELECT TriggerBoardId, DCCInputChannelNum FROM TriggerBoard ";
      sqlQuery = " SELECT Board_BoardId, TriggerBoardInputNum FROM LinkConn ";
      sqlQuery += " WHERE TB_TriggerBoardId= ";
      sqlQuery +=  IntToString(theTB[iTB].first);  
      sqlQuery += " ORDER BY TriggerBoardInputNum ";
      stmt->setSQL(sqlQuery.c_str());
      rset = stmt->executeQuery();
      int nlinks=0;
      while (rset->next()) {
        nlinks++;
        tmp_tbl.first=rset->getInt(1);
        tmp_tbl.second=rset->getInt(2);
        theLink.push_back(tmp_tbl);
      }
      for(unsigned int iLink=0;iLink<theLink.size();iLink++) {
	int boardId=theLink[iLink].first;
        thisLink.theTriggerBoardInputNumber=theLink[iLink].second;
	std::vector<std::pair<int,string> > theLB;
	std::pair<int,string> tmpLB;
	// Get master LBs first...
        RPCEMap::lbItem thisLB;
	sqlQuery = " SELECT Name ";
	sqlQuery += " FROM Board ";
	sqlQuery += " WHERE BoardId= ";
	sqlQuery +=  IntToString(theLink[iLink].first);
	stmt->setSQL(sqlQuery.c_str());
	rset = stmt->executeQuery();
        int nlbs=0;
	while (rset->next()) {
          nlbs++;
	  tmpLB.first=theLink[iLink].first;
	  tmpLB.second=rset->getString(1);
	  theLB.push_back(tmpLB);
	}
	// then slaves
	sqlQuery = " SELECT LinkBoard.LinkBoardId, Board.Name ";
	sqlQuery += " FROM LinkBoard, Board ";
	sqlQuery += " WHERE LinkBoard.MasterId= ";
	sqlQuery +=  IntToString(theLink[iLink].first);
	sqlQuery += " AND Board.BoardId=LinkBoard.LinkBoardId";
	sqlQuery += " AND LinkBoard.MasterId<>LinkBoard.LinkBoardId";
	sqlQuery += " ORDER BY LinkBoard.LinkBoardId ";
	stmt->setSQL(sqlQuery.c_str());
	rset = stmt->executeQuery();
	while (rset->next()) {
          nlbs++;
	  tmpLB.first=rset->getInt(1);
	  tmpLB.second=rset->getString(2);
	  theLB.push_back(tmpLB);
        }
	for(unsigned int iLB=0; iLB<theLB.size(); iLB++) {
	  thisLB.theLinkBoardNumInLink=atoi(((theLB[iLB].second).substr((theLB[iLB].second).length()-1,1)).c_str());
	  thisLB.theMaster = (theLB[iLB].first==boardId);
	  FEBStruct tmpFEB;
          std::vector<FEBStruct> theFEB;
// get FEBs
          RPCEMap::febItem thisFeb;
	  sqlQuery = " SELECT FEBLocation.FEBLocationId,";
	  sqlQuery += "  FEBLocation.CL_ChamberLocationId,";
	  sqlQuery += "  FEBConnector.FEBConnectorId,";
	  sqlQuery += "  FEBLocation.FEBLocalEtaPartition,";
	  sqlQuery += "  FEBLocation.PosInLocalEtaPartition,";
	  sqlQuery += "  FEBLocation.FEBCMSEtaPartition,";
	  sqlQuery += "  FEBLocation.PosInCMSEtaPartition,";
	  sqlQuery += "  FEBConnector.LinkBoardInputNum ";
	  sqlQuery += " FROM FEBLocation, FEBConnector "; 
	  sqlQuery += " WHERE FEBLocation.LB_LinkBoardId= ";
	  sqlQuery +=  IntToString(theLB[iLB].first);
	  sqlQuery += "  AND FEBLocation.FEBLocationId=FEBConnector.FL_FEBLocationId"; 
	  sqlQuery += " ORDER BY FEBLocation.FEBLocationId, FEBConnector.FEBConnectorId";
	  stmt->setSQL(sqlQuery.c_str());
	  rset = stmt->executeQuery();
          int nfebs=0;
	  while (rset->next()) {
            nfebs++;
	    tmpFEB.febId=rset->getInt(1);
	    tmpFEB.chamberId=rset->getInt(2);
	    tmpFEB.connectorId=rset->getInt(3);
	    tmpFEB.localEtaPart=rset->getString(4);
	    tmpFEB.posInLocalEtaPart=rset->getInt(5);
	    tmpFEB.cmsEtaPart=rset->getString(6);
	    tmpFEB.posInCmsEtaPart=rset->getInt(7);
	    tmpFEB.lbInputNum=rset->getInt(8);
            theFEB.push_back(tmpFEB);
          }
          for(unsigned int iFEB=0; iFEB<theFEB.size(); iFEB++) {
	    thisFeb.localEtaPartition=theFEB[iFEB].localEtaPart;
	    thisFeb.positionInLocalEtaPartition=theFEB[iFEB].posInLocalEtaPart;
	    thisFeb.cmsEtaPartition=theFEB[iFEB].cmsEtaPart;
	    thisFeb.positionInCmsEtaPartition=theFEB[iFEB].posInCmsEtaPart;
	    thisFeb.theLinkBoardInputNum=theFEB[iFEB].lbInputNum;
            FebLocationSpec febLocation = {theFEB[iFEB].cmsEtaPart,theFEB[iFEB].posInCmsEtaPart,theFEB[iFEB].localEtaPart,theFEB[iFEB].posInLocalEtaPart};
	    // Get chamber 
	    sqlQuery = "SELECT DiskOrWheel, Layer, Sector, Subsector,";
	    sqlQuery += " ChamberLocationName,";
	    sqlQuery += " FEBZOrnt, FEBRadOrnt, BarrelOrEndcap";
	    sqlQuery += " FROM ChamberLocation "; 
	    sqlQuery += " WHERE ChamberLocationId= ";
	    sqlQuery +=  IntToString(theFEB[iFEB].chamberId);
	    stmt->setSQL(sqlQuery.c_str());
	    rset = stmt->executeQuery();
	    while (rset->next()) {
              thisFeb.diskOrWheel=rset->getInt(1);
	      thisFeb.layer=rset->getInt(2);
	      thisFeb.sector=rset->getInt(3);
	      thisFeb.subsector=rset->getString(4);
              if (thisFeb.subsector=="") thisFeb.subsector="0";
	      thisFeb.chamberLocationName=rset->getString(5);
	      thisFeb.febZOrnt=rset->getString(6);
	      thisFeb.febZRadOrnt=rset->getString(7);
              if (thisFeb.febZRadOrnt=="") thisFeb.febZRadOrnt="N/A";
	      thisFeb.barrelOrEndcap=rset->getString(8);
              ChamberLocationSpec chamber = {thisFeb.diskOrWheel,thisFeb.layer,thisFeb.sector,thisFeb.subsector,thisFeb.chamberLocationName,thisFeb.febZOrnt,thisFeb.febZRadOrnt,thisFeb.barrelOrEndcap};
              DBSpecToDetUnit toDU;
              thisFeb.theRawId=toDU(chamber,febLocation);
            }
	    // Get Strips
            RPCEMap::stripItem thisStrip;
  	    sqlQuery = "SELECT CableChannelNum, ChamberStripNumber, CmsStripNumber";
	    sqlQuery += " FROM ChamberStrip "; 
	    sqlQuery += " WHERE FC_FEBConnectorId= ";
	    sqlQuery +=  IntToString(theFEB[iFEB].connectorId);
	    sqlQuery += " ORDER BY CableChannelNum";
	    stmt->setSQL(sqlQuery.c_str());
	    rset = stmt->executeQuery();
            int nstrips=0;
	    while (rset->next()) {
              thisStrip.cablePinNumber=rset->getInt(1);
              thisStrip.chamberStripNumber=rset->getInt(2);
              thisStrip.cmsStripNumber=rset->getInt(3);
              eMap->theStrips.push_back(thisStrip);
              nstrips++;
	    }
            thisFeb.nStrips=nstrips;
            eMap->theFebs.push_back(thisFeb);
          }
          thisLB.nFebs=nfebs;
          eMap->theLBs.push_back(thisLB);
	}
        thisLink.nLBs=nlbs;
        eMap->theLinks.push_back(thisLink);
      }
      thisTB.nLinks=nlinks;
      eMap->theTBs.push_back(thisTB);
    }
    thisDcc.nTBs=ntbs;
    std::cout<<"DCC added"<<std::endl;
    eMap->theDccs.push_back(thisDcc);
  }
  cout << endl <<"Building RPC e-Map done!" << flush << endl << endl;

}


//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RPCEMapBuilder);

