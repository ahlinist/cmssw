
/** \class RPCReadOutMappingBuilder
 *
 * Description:
 *      Class to read directly OMDS DB with OCCI and fill Offline DB
 *
 * $Date: 2007/04/11 12:04:25 $
 * $Revision: 1.11 $
 * \author Michal Bluj -- INS Warsaw
 *
 */

#include "CondFormats/RPCObjects/interface/RPCReadOutMapping.h"
#include "CondFormats/RPCObjects/interface/DccSpec.h"
#include "CondFormats/RPCObjects/interface/TriggerBoardSpec.h"
#include "CondFormats/RPCObjects/interface/LinkConnSpec.h"
#include "CondFormats/RPCObjects/interface/LinkBoardSpec.h"
#include "CondFormats/RPCObjects/interface/ChamberLocationSpec.h"
#include "CondFormats/RPCObjects/interface/FebLocationSpec.h"
#include "CondFormats/RPCObjects/interface/FebConnectorSpec.h"
#include "CondFormats/RPCObjects/interface/ChamberStripSpec.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
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

class RPCReadOutMappingBuilder : public edm::EDAnalyzer {
  
public:
  explicit RPCReadOutMappingBuilder(const edm::ParameterSet& iConfig);
  ~RPCReadOutMappingBuilder();
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){}
  void ConnectOnlineDB(string host, string sid, string user, string pass, int port);
  void DisconnectOnlineDB();
  void WritePoolDB();
  unsigned int giveNumOfStripsPerFEB(int sector, int layer) const;
  void readCablingMap();

private:
  Environment* env;
  Connection* conn;
                                                                                
  RPCReadOutMapping *cabling;
    
  string record_;
  string tag_;
  string version_;
  int runno_;
                                                                                
  // utilities
  string IntToString(int num)
  {
    stringstream snum;
    snum << num << flush;
                                                                                
    return(snum.str());
  }
                                                                                
  typedef struct{int febId,chamberId,connectorId,lbInputNum,posInLocalEtaPart,posInCmsEtaPart;string localEtaPart,cmsEtaPart;} FEBStruct;
  
};

RPCReadOutMappingBuilder::RPCReadOutMappingBuilder(const edm::ParameterSet& iConfig) 
  : record_(iConfig.getParameter<std::string>("record")),
    tag_(iConfig.getUntrackedParameter<std::string>("tag",(string)"RPCReadOutMapping_v1")),
    version_(iConfig.getUntrackedParameter<std::string>("version",(string)"CreationTime")),
    runno_(iConfig.getUntrackedParameter<int>("runnumber",0))
{
  cout<<"CTor called"<<endl;

  cout <<" HERE record: "<<record_<<endl;

// this is not really used  
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

  ::putenv("CORAL_AUTH_USER=me");
  ::putenv("CORAL_AUTH_PASSWORD=test"); 

}

RPCReadOutMappingBuilder::~RPCReadOutMappingBuilder()
{ 
  cout<<"DTor called"<<endl;
}

void RPCReadOutMappingBuilder::endJob()
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

void RPCReadOutMappingBuilder::beginJob(const edm::EventSetup& iSetup)
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

void RPCReadOutMappingBuilder::ConnectOnlineDB(string host, string sid, string user, string pass, int port=1521)
{
  stringstream ss;
  ss << "//" << host << ":" << port << "/" << sid;
    
  cout << "Connecting to " << host << "..." << flush;
  env = Environment::createEnvironment(Environment::OBJECT);
  conn = env->createConnection(user, pass, ss.str());
  cout << "Done." << endl;
}

void RPCReadOutMappingBuilder::DisconnectOnlineDB()
{
  env->terminateConnection(conn);
  Environment::terminateEnvironment(env);
}

void RPCReadOutMappingBuilder::WritePoolDB()
{
  cout << endl << "Start writing to PoolDB" << flush << endl;

  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if( !mydbservice.isAvailable() ){
    cout<<"DB service unavailable"<<endl;
    return;
  } else { 
    cout<<"DB service OK"<<endl; 
  }
  
  try {
    if( mydbservice->isNewTagRequest(record_) ) {
      std::cout<<"Creating new tag for "<<record_<<std::endl;
      mydbservice->createNewIOV<RPCReadOutMapping>(cabling, mydbservice->endOfTime(), record_);
    } else {
      int iov=mydbservice->currentTime();
      if (runno_ > 0) iov=runno_;
      std::cout<<"Current IOV is "<<iov<<std::endl;
      mydbservice->appendSinceTime<RPCReadOutMapping>(cabling, iov, record_);
    }
  }
  catch (std::exception &e) { cout <<"std::exception:  "<< e.what(); }
  catch (...) { cout << "Unknown error caught "<<endl; }
  cout<<"... all done, end"<<endl;

}
  

// Method called to compute number of strips connected to FEB (barrel only)
unsigned int RPCReadOutMappingBuilder::giveNumOfStripsPerFEB(int sector, int layer) const {

  unsigned int nStrips = 0;
  if(layer==1 || layer==3) nStrips = 90;
  if(layer==2 || layer==4) nStrips = 84;
  if(layer==5) nStrips = 42;
  if(layer==6){
    if(sector==4) nStrips = 36;
    else if(sector==9 || sector==11) nStrips = 52;
    else if(sector==10) nStrips = 60;
    else nStrips = 48;
  } 
 
  unsigned int tmp;
  const unsigned int FEB = 16;

  if(nStrips==60) return 12;

  for(int i=0;i<5;i++){
    tmp = nStrips%(FEB-i);
    if(tmp==0) return (FEB-i);
  }

  cout << "[RPCReadOutMappingBuilder::giveNumOfStripsPerFEB]: " << endl
       << "Can't figure out number of FEB channels for this number of strips: "
       << nStrips << " Returning 0 channels!" << endl;

  return 0;

}

// Method called to read Online DB and fill RPCReadOutMapping
void RPCReadOutMappingBuilder::readCablingMap()
{

  cout << "Reading cabling map with mode 0:  Basic OCCI" << endl;

  string cabling_version = version_;

  Statement* stmt = conn->createStatement();
  string sqlQuery ="";

  cout << endl <<"Start to build RPC Cabling..." << flush << endl << endl;
  cabling =  new RPCReadOutMapping(cabling_version);

  // Get FEDs
  cout << "Getting the FEDs... " << flush;
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
  cout << "Done." << endl;
  for(unsigned int iFED=0;iFED<theDAQ.size();iFED++) {
    cout << " |-> Getting the TBs for FED no. "<< theDAQ[iFED].second << " ... " << flush;
    std::vector<std::pair<int,int> > theTB;
    DccSpec dcc(theDAQ[iFED].second);
    sqlQuery = " SELECT TriggerBoardId, DCCInputChannelNum FROM TriggerBoard ";
    sqlQuery += " WHERE DCCBoard_DCCBoardId= ";
    sqlQuery += IntToString(theDAQ[iFED].first);
    sqlQuery += " ORDER BY DCCInputChannelNum ";
    stmt->setSQL(sqlQuery.c_str());
    rset = stmt->executeQuery();
    while (rset->next()) {
      tmp_tbl.first=rset->getInt(1);
      tmp_tbl.second=rset->getInt(2);
      theTB.push_back(tmp_tbl);
    }
    cout << "Done." << endl;
    for(unsigned int iTB=0;iTB<theTB.size();iTB++) {
      cout << " |    |-> Getting the Links for TB no. "<< theTB[iTB].second << " ... " << flush;
      std::vector<std::pair<int,int> > theLink;
      TriggerBoardSpec tb(theTB[iTB].second);
      sqlQuery = " SELECT Board_BoardId, TriggerBoardInputNum FROM LinkConn ";
      sqlQuery += " WHERE TB_TriggerBoardId= ";
      sqlQuery +=  IntToString(theTB[iTB].first);  
      sqlQuery += " ORDER BY TriggerBoardInputNum ";
      stmt->setSQL(sqlQuery.c_str());
      rset = stmt->executeQuery();
      while (rset->next()) {
	tmp_tbl.first=rset->getInt(1);
	tmp_tbl.second=rset->getInt(2);
	theLink.push_back(tmp_tbl);
      }
      cout << "Done." << endl;
      for(unsigned int iLink=0;iLink<theLink.size();iLink++) {
	cout << " |    |    |-> Getting the LBs for Link no. "
	     << theLink[iLink].second << " ... " << flush;
	std::vector<std::pair<int,string> > theLB;
	std::pair<int,string> tmpLB;
	// Get master first...
	sqlQuery = " SELECT Name ";
	sqlQuery += " FROM Board ";
	sqlQuery += " WHERE BoardId= ";
	sqlQuery +=  IntToString(theLink[iLink].first);
	stmt->setSQL(sqlQuery.c_str());
	rset = stmt->executeQuery();
	while (rset->next()) {
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
	  tmpLB.first=rset->getInt(1);
	  tmpLB.second=rset->getString(2);
	  theLB.push_back(tmpLB);
	}
	LinkConnSpec  lc(theLink[iLink].second);
	cout << "Done." << endl;
	int linkChannel;
	for(unsigned int iLB=0; iLB<theLB.size(); iLB++) {
	  linkChannel=atoi(((theLB[iLB].second).substr((theLB[iLB].second).length()-1,1)).c_str());
	  bool master = (theLB[iLB].first==theLink[iLink].first);
	  LinkBoardSpec lb(master,linkChannel);
	  cout << " |    |    |    |-> Getting the FEBs for LB no. "<< linkChannel << " ... " << flush;
	  FEBStruct tmpFEB;
	  std::vector<FEBStruct> theFEB;
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
	  while (rset->next()) {
	    tmpFEB.febId=rset->getInt(1);
	    tmpFEB.chamberId=rset->getInt(2);
	    tmpFEB.connectorId=rset->getInt(3);
	    tmpFEB.localEtaPart=rset->getString(4);
	    tmpFEB.posInLocalEtaPart=rset->getInt(5);
	    tmpFEB.cmsEtaPart=rset->getString(6);
	    tmpFEB.posInCmsEtaPart=rset->getInt(7);
	    tmpFEB.lbInputNum=rset->getInt(8);
	    theFEB.push_back(tmpFEB);
	    /*
	      if(theFEB.size()>1){
		if(theFEB[theFEB.size()-1].chamberId!=theFEB[theFEB.size()-2].chamberId){
		  cout << "WARNING: FEBs from different chamber in one LB!" << flush;
		  cout << theFEB.size() << ". (" << theFEB[theFEB.size()-1].febId << "," << theFEB[theFEB.size()-1].chamberId << ") "
		       << theFEB.size() << ". (" << theFEB[theFEB.size()-2].febId << "," << theFEB[theFEB.size()-2].chamberId << ") " << flush;
		}
	      }
	      */
	  }
	  cout<< " Done." << endl;
	  for(unsigned int iFEB=0; iFEB<theFEB.size(); iFEB++) {
	    FebLocationSpec febLocation = {theFEB[iFEB].cmsEtaPart,theFEB[iFEB].posInCmsEtaPart,theFEB[iFEB].localEtaPart,theFEB[iFEB].posInLocalEtaPart};
//	    cout << " |    |    |    |    |-> Getting the Chamber and the Strips for FEB no. "<<  theFEB[iFEB].lbInputNum << " ... " << flush;
	    // Get chamber 
	    ChamberLocationSpec chamber;
	    sqlQuery = "SELECT DiskOrWheel, Layer, Sector, Subsector,";
	    sqlQuery += " ChamberLocationName,";
	    sqlQuery += " FEBZOrnt, FEBRadOrnt, BarrelOrEndcap";
	    sqlQuery += " FROM ChamberLocation "; 
	    sqlQuery += " WHERE ChamberLocationId= ";
	    sqlQuery +=  IntToString(theFEB[iFEB].chamberId);
	    stmt->setSQL(sqlQuery.c_str());
	    rset = stmt->executeQuery();
	    while (rset->next()) {
	      chamber.diskOrWheel=rset->getInt(1);
	      chamber.layer=rset->getInt(2);
	      chamber.sector=rset->getInt(3);
	      chamber.subsector=rset->getString(4);
              if (chamber.subsector=="") chamber.subsector="0";
	      chamber.chamberLocationName=rset->getString(5);
	      chamber.febZOrnt=rset->getString(6);
	      chamber.febZRadOrnt=rset->getString(7);
              if (chamber.febZRadOrnt=="") chamber.febZRadOrnt="N/A";
	      chamber.barrelOrEndcap=rset->getString(8);
	    }
	    FebConnectorSpec febConnector(theFEB[iFEB].lbInputNum,chamber,febLocation);
	    cout << " Done ... " << flush;
	    // Get Strips
	    //sqlQuery = "SELECT CablePinNumber, ChamberStripNumber, CmsStripNumber";
	    sqlQuery = "SELECT CableChannelNum, ChamberStripNumber, CmsStripNumber";
	    sqlQuery += " FROM ChamberStrip "; 
	    sqlQuery += " WHERE FC_FEBConnectorId= ";
	    sqlQuery +=  IntToString(theFEB[iFEB].connectorId);
	    //sqlQuery += " ORDER BY CablePinNumber";
	    sqlQuery += " ORDER BY CableChannelNum";
	    stmt->setSQL(sqlQuery.c_str());
	    rset = stmt->executeQuery();
//	    cout << " Done." << endl;
	    unsigned int iStripEntry=0;
	    while (rset->next()) {
	      cout << " |    |    |    |    |    |-> Adding the Strip no. "<< rset->getInt(1) << " ... " << flush;
	      ChamberStripSpec strip = {rset->getInt(1),rset->getInt(2),rset->getInt(3)};
	      febConnector.add(strip);
	      cout << " Done." << endl;
	      iStripEntry++;
	    }	    
	    if(iStripEntry==0){
	      cout << " |    |    |    |    |    |    Zero strips found in DB for FEB no. " << theFEB[iFEB].lbInputNum << flush << endl;
	      cout << " |    |    |    |    |    |    Dummy strip generation ... " << flush;
	      
	      unsigned int max_iStrip = giveNumOfStripsPerFEB(chamber.sector,chamber.layer);
	      max_iStrip = 16;
	      for(unsigned int iStrip=0; iStrip < max_iStrip; iStrip++) {
		int stripCablePin = iStrip;
		int chamberStrip =(theFEB[iFEB].lbInputNum-1)*max_iStrip+iStrip+1;
		if(chamber.subsector=="+" && chamber.layer==5) chamberStrip-=42;
		int cmsStrip = chamberStrip;
		//ChamberStripSpec strip = {stripCablePin+2, chamberStrip, cmsStrip};
		ChamberStripSpec strip = {iStrip+1, chamberStrip, cmsStrip};
		//cout<<iStrip+1<<" "<<chamberStrip<<" "<<cmsStrip<<endl;
		febConnector.add(strip);
	      }
		
	      cout << " Done." << endl;		
	    }
	    lb.add(febConnector); 
	  }
	  lc.add(lb);
	}
	tb.add(lc);
      }
      dcc.add(tb);
    }
    cabling->add(dcc);
  }
  cout << endl <<"Building RPC Cabling done!" << flush << endl << endl;
}


//define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RPCReadOutMappingBuilder);

