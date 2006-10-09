
/** \class RPCReadOutMappingBuilder
 *
 * Description:
 *      Class to read directly OMDS DB with OCCI and fill Offline DB
 *
 * $Date: 2006/08/25 10:41:16 $
 * $Revision: 1.6 $
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

//#include "DataFormats/MuonDetId/interface/RPCDetId.h"

#include "CondCore/DBCommon/interface/DBWriter.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"
#include "CondCore/IOVService/interface/IOV.h"
#include "CondCore/MetaDataService/interface/MetaData.h"

#include "FWCore/Framework/interface/IOVSyncValue.h"

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "occi.h"

using namespace std;
using namespace oracle::occi;

class RPCReadOutMappingBuilder {
  
public:

  RPCReadOutMappingBuilder(string host, string sid, string user, string pass, int port=1521, 
			   string poolDb="sqlite_file:cabling.db", string version="RPCReadOutMapping_v1", int first_run=0, int last_run=0) 
  { 
    ConnectOnlineDB(host, sid, user, pass, port);
    poolDbCon_=poolDb;

    version_=version;
    catalog_="file:cablingCatalog.xml";
    
    first_run>=0 ? run1_=first_run : run1_=0;
    last_run>run1_ ? run2_=last_run : run2_=run1_;

    ::putenv("CORAL_AUTH_USER=konec");
    ::putenv("CORAL_AUTH_PASSWORD=konecPass"); 

  }
 
 ~RPCReadOutMappingBuilder()
  { 
    DisconnectOnlineDB();
    WritePoolDB();
  }

  void ConnectOnlineDB(string host, string sid, string user, string pass, int port=1521)
  {
    stringstream ss;
    ss << "//" << host << ":" << port << "/" << sid;
    
    cout << "Connecting to " << host << "..." << flush;
    env = Environment::createEnvironment(Environment::OBJECT);
    conn = env->createConnection(user, pass, ss.str());
    cout << "Done." << endl;
  }

  void DisconnectOnlineDB()
  {
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
  }

  void WritePoolDB()
  {
    cout << endl << "Start writing to PoolDB" << flush << endl;
    try {
      loader = new cond::ServiceLoader;
      loader->loadAuthenticationService( cond::Env );
      loader->loadMessageService( cond::Error );

      session = new cond::DBSession(poolDbCon_);
      session->setCatalog(catalog_);
      session->connect(cond::ReadWriteCreate);

      metadataSvc = new cond::MetaData(poolDbCon_,*loader);
      metadataSvc->connect();

      mapWriter = new cond::DBWriter(*session, "RPCReadOutMapping");
      iovWriter = new cond::DBWriter(*session, "IOV");

      string cabTok;
      string cabIOVTok;
      cond::IOV* cabIOV= new cond::IOV; 

      // Write same mapping for all runs 
      for(int run=run1_; run<=run2_;run++){
	// Arguments 0 0 mean infinite IOV
	if (run1_ == 0 && run2_ == 0) {
	  cout << "Infinite IOV mode" << endl;
	  run = edm::IOVSyncValue::endOfTime().eventID().run();
	}
	session->startUpdateTransaction();
        cout << " Run " << run << endl << "  Write cabling..." << flush;
	cabTok = mapWriter->markWrite<RPCReadOutMapping>(cabling);  
	//cout << " Commit... " << flush;
	//session->commit();
	cout << " Done." << endl;
	cout << "  Associate IOV..." << flush;
	cabIOV->iov.insert(std::make_pair(run, cabTok));
	cout << " Done." << endl;
	// End loop on infinite IOV
	if (run1_ == 0 && run2_ == 0) {
	  break;
	}
      }
      session->startUpdateTransaction();

      cout << " Write IOV... " << flush;
      cabIOVTok = iovWriter->markWrite<cond::IOV>(cabIOV);  // ownership given
      cout << " Commit... " << flush;
      session->commit();  // IOV memory freed
      cout << " Done." << endl;

      cout << " Add MetaData..." << flush;
      metadataSvc->addMapping(version_,cabIOVTok);
      cout << " Done." << endl;

      session->disconnect();
      metadataSvc->disconnect();
    }
    catch(const cond::Exception& e) {
      std::cout<<"cond::Exception: " << e.what()<<std::endl;
      if(loader) delete loader;
    } 
    catch (pool::Exception& e) {
      cout << "pool::Exception:  " << e.what() << endl;
      if(loader) delete loader;
    }
    catch (std::exception &e) {
      cout << "std::exception:  " << e.what() << endl;
      if(loader) delete loader;
    }
    catch (...) {
      cout << "Unknown error caught" << endl;
      if(loader) delete loader;
    }
    if(session) delete session;
    if (mapWriter) delete mapWriter;
    if(iovWriter) delete iovWriter;
    if (metadataSvc) delete metadataSvc;
    if(loader) delete loader;
    cout << "PoolDB written!" << endl;
  }
  

// Method called to compute number of strips connected to FEB (barrel only)
  unsigned int giveNumOfStripsPerFEB(int sector, int layer) const {

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
  void readCablingMap()
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
	sqlQuery +=  IntToString(theTB[iFED].first);  
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
	      cout << " |    |    |    |    |-> Getting the Chamber and the Strips for FEB no. "<<  theFEB[iFEB].lbInputNum << " ... " << flush;
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
		chamber.chamberLocationName=rset->getString(5);
		chamber.febZOrnt=rset->getString(6);
		chamber.febZRadOrnt=rset->getString(7);
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
	      cout << " Done." << endl;
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
  
private:
  Environment* env;
  Connection* conn;

  cond::ServiceLoader* loader;
  cond::DBSession* session;
  cond::DBWriter* mapWriter;
  cond::DBWriter* iovWriter;
  cond::MetaData* metadataSvc;

  string poolDbCon_;
  
  RPCReadOutMapping *cabling;

  string version_;
  string catalog_;
  int run1_, run2_;

  // utilities
  string IntToString(int num)
  {
    stringstream snum;
    snum << num << flush;

    return(snum.str());
  }
  
  typedef struct{int febId,chamberId,connectorId,lbInputNum,posInLocalEtaPart,posInCmsEtaPart; string localEtaPart,cmsEtaPart;} FEBStruct;  

};

int main(int argc, char* argv[])
{

  // simple parsing command-line options
  if( (argc < 4 && argc!=2) || (argc==2 && (string)argv[1]!="-help") ) {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <passwd> "
	 << "[port] [POOL_DB] [version] [first_run] [last_run]" 
	 << endl;
    cout << "Try \'" << argv[0] << " -help\' for more information." << endl;
    return -1;
  }
  if (argc==2 && (string)argv[1]=="-help") {
    cout << "Usage:" << endl;
    cout << "  " << argv[0] << " <host> <SID> <user> <passwd> "
	 << "[port] [POOL DB] [version] [first run] [last run]" 
	 << endl;
    cout << "where:" << endl
	 << "  host, ID, user, passwd, port identify Online DB (default: port=1521)" << endl
	 << "  POOL_DB is POOL DB connection string (default: sqlite_file:cabling.db)" << endl
	 << "  version is a tag of Mapping (default: RPCReadOutMapping_v1)" << endl
	 << "  first_run, last_run are first and last runs for which Mapping is valid. Infinitive validity for first_run=last_run=0 (default: first_run=last_run=0)" << endl
	 << "Warning:" << endl
	 << "  Positions of parameters cannot be changed!" << endl
	 << endl;
    return 1;
  }
  string host = (string)argv[1];
  string sid = (string)argv[2];
  string user = (string)argv[3];
  string pass = (string)argv[4];
  int port = 1521;
  string poolCon = "sqlite_file:cabling.db";
  string ver = "RPCReadOutMapping_v1";
  int run1=0, run2=0;
  if(argc > 5 ) port = atoi(argv[5]);
  if(argc > 6 ) poolCon = (string)argv[6];
  if(argc > 7 ) ver = (string)argv[7];
  if(argc > 8 ) run1 = atoi(argv[8]);
  if(argc > 9 ) run2 = atoi(argv[9]);

  try {
    RPCReadOutMappingBuilder builder(host, sid, user, pass, port, poolCon, ver, run1, run2);
    
    builder.readCablingMap();
      
  } catch (SQLException &e) {
    cerr << "SQLException:  " << e.getMessage() << endl;
  } catch (exception &e) {
    cerr << "exception:  " << e.what() << endl;
  } catch (...) {
    cerr << "Unknown exception caught" << endl;
  }

  return 0;
}


