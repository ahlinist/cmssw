#include <iostream>
#include <sstream>
#include <map>
#include <occi.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"

using namespace oracle::occi;
using namespace std;

class  DetDiagLEDReader{
private:
  Environment *env;
  Connection *conn;
  Statement *stmt;
  map<string,unsigned int> lookupMap;

public:
  DetDiagLEDReader(string user,string passwd,string db){
    env = Environment::createEnvironment(Environment::DEFAULT);
    conn = env->createConnection(user,passwd,db);
  }

  ~DetDiagLEDReader(){
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
  }
  void getDatasetList(){
      try{ 
          //prepare oracle sql statement
          Statement* stmnt= conn->createStatement("SELECT CONDR.RUN_NUMBER,TO_CHAR(RUNINFO.BOOKINGTIME,'DD-MM-YYYY HH24:MI:SS') \
             FROM CMS_HCL_CORE_COND.COND_DATA_SETS COND INNER JOIN CMS_HCL_CORE_COND.COND_RUNS CONDR \
             ON COND.COND_RUN_ID = CONDR.COND_RUN_ID \
             INNER JOIN CMS_HCL_CORE_COND.KINDS_OF_CONDITIONS KOC \
             ON COND.KIND_OF_CONDITION_ID = KOC.KIND_OF_CONDITION_ID \
             INNER JOIN CMS_RUNINFO.RUNNUMBERTBL RUNINFO \
             ON CONDR.RUN_NUMBER = RUNINFO.RUNNUMBER \
             WHERE KOC.NAME = 'HCAL LED [local]' \
             AND COND.IS_RECORD_DELETED = 'F' \
             AND CONDR.IS_RECORD_DELETED = 'F' \
             AND KOC.IS_RECORD_DELETED = 'F' \
             ORDER BY RUN_NUMBER");

          //excute statement
          ResultSet* rs=stmnt->executeQuery();
          stmnt->setPrefetchRowCount(10000);
          int iRowCount=0;
          cout.precision(2);
	  while(rs->next()){ 
	     iRowCount++;   
             cout << rs->getInt(1) << " ";
             cout << rs->getString(2) << " ";
             string str="SELECT TO_CHAR(DIPTIME,'DD-MM-YYYY HH24:MI:SS'),MAGENETCURRENT FROM CMS_BEAM_COND.BRM_MAGNETCURRENT "; 
             str+="WHERE DIPTIME=(SELECT MAX(DIPTIME) FROM CMS_BEAM_COND.BRM_MAGNETCURRENT WHERE DIPTIME<TO_TIMESTAMP('";
             str+=rs->getString(2);
             str+="','DD-MM-YYYY HH24:MI:SS'))";
             Statement* stmnt1= conn->createStatement(str.c_str());
             ResultSet* rs1=stmnt1->executeQuery();
             rs1->next();
             cout << rs1->getInt(2)/18164.0*3.8;
             cout << endl;
             stmnt1->closeResultSet(rs1);
             conn->terminateStatement(stmnt1);
          }
	  cout << iRowCount << " datasets found"<< endl;
          stmnt->closeResultSet(rs);
          conn->terminateStatement(stmnt);
      }catch(SQLException& ex){
           cout<<"Exception thrown for readLEDDataset"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
      }  
  }  
  void readLEDDatasetRange(string from,string to){
      try{ 
          //prepare oracle sql statement
          Statement* stmnt= conn->createStatement("SELECT DISTINCT CONDR.RUN_NUMBER, CONDS.SET_NUMBER \
           FROM CMS_HCL_CORE_COND.COND_DATA_SETS CONDS \
           INNER JOIN CMS_HCL_CORE_COND.KINDS_OF_CONDITIONS KOC \
           ON CONDS.KIND_OF_CONDITION_ID = KOC.KIND_OF_CONDITION_ID \
           INNER JOIN CMS_HCL_CORE_COND.COND_RUNS CONDR \
           ON CONDS.COND_RUN_ID = CONDR.COND_RUN_ID \
           WHERE KOC.NAME = 'HCAL LED [local]' \
           AND CONDS.IS_RECORD_DELETED = 'F' \
           AND KOC.IS_RECORD_DELETED = 'F' \
           AND CONDR.IS_RECORD_DELETED = 'F' \
           AND RUN_NUMBER > :1 AND RUN_NUMBER < :2 ORDER BY RUN_NUMBER");
          stmnt->setString(1,from);
          stmnt->setString(2,to);
         
          //excute statement
          ResultSet* rs=stmnt->executeQuery();
          stmnt->setPrefetchRowCount(10000);
	  while(rs->next()){ 
	     stringstream s(to); int TO; s >>TO;
	     if(rs->getInt(1)>TO) continue; 
	     readLEDDataset(rs->getString(1),rs->getString(2));
          }
          stmnt->closeResultSet(rs);
          conn->terminateStatement(stmnt);
      }catch(SQLException& ex){
           cout<<"Exception thrown for readLEDDataset"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
      }  
  }
  void readLEDDataset(string run,string set){ 
       try{
           //prepare oracle sql statement
           Statement* stmnt= conn->createStatement("SELECT SUBDET,IETA,IPHI,DEPTH,SIGNAL_MEAN,SIGNAL_RMS,TIME_MEAN,TIME_RMS,NUMBER_OF_EVENTS_USED,CHANNEL_STATUS_WORD FROM \
	   CMS_HCL_HCAL_COND.V_HCAL_DETMON_LED_LASER_V1 WHERE RUN_NUMBER=:1 AND SET_NUMBER=:2 AND KOC_TYPE='local'");
           stmnt->setString(1,run);
           stmnt->setString(2,set);
		  
           //excute statement
           ResultSet* rs=stmnt->executeQuery();
           stmnt->setPrefetchRowCount(10000);
		  
           //prepare lookup map
           lookupMap.clear();
           vector<MetaData> metaData=rs->getColumnListMetaData();
           for(unsigned int i=0; i<metaData.size();i++){
               lookupMap.insert(pair<string,unsigned int>(
               metaData[i].getString(MetaData::ATTR_NAME),i+1));
           }

           //iterate over returned records
           int iRowCount=0;
	   
	   double amp,rms,time,time_rms;
           int    Eta,Phi,Depth,Statistic,Status;
	   char   Subdet[20];
	   
	   string fname="HcalDetDiagLEDData_run"+run+"_"+set+".root";
	   TFile *theFile = new TFile(fname.c_str(), "RECREATE");
	   if(!theFile->IsOpen()) return;
	   theFile->cd();   
           TObjString RUN(run.c_str());    RUN.Write("run number");
	   TObjString dsnum(set.c_str());  dsnum.Write("Dataset number");
	   
           TTree *tree   =new TTree("HCAL LED data","HCAL LED data");
           if(tree==0)   return;
           tree->Branch("Subdet",   &Subdet,         "Subdet/C");
           tree->Branch("eta",      &Eta,            "Eta/I");
           tree->Branch("phi",      &Phi,            "Phi/I");
           tree->Branch("depth",    &Depth,          "Depth/I");
           tree->Branch("statistic",&Statistic,      "Statistic/I");
           tree->Branch("status",   &Status,         "Status/I");
           tree->Branch("led",      &amp,            "led/D");
           tree->Branch("rms",      &rms,            "rms/D");
           tree->Branch("time",     &time,           "time/D");
           tree->Branch("time_rms", &time_rms,       "time_rms/D");

	   while(rs->next()){
               iRowCount++ ;
               sprintf(Subdet,"%s",rs->getString(lookupMap.find("SUBDET")->second).c_str()); 
               Eta=rs->getInt(lookupMap.find("IETA")->second); 
               Phi=rs->getInt(lookupMap.find("IPHI")->second); 
               Depth=rs->getInt(lookupMap.find("DEPTH")->second);
               amp=rs->getFloat(lookupMap.find("SIGNAL_MEAN")->second);
               rms=rs->getFloat(lookupMap.find("SIGNAL_RMS")->second);
               time=rs->getFloat(lookupMap.find("TIME_MEAN")->second); 
               time_rms=rs->getFloat(lookupMap.find("TIME_RMS")->second);
               Statistic=rs->getInt(lookupMap.find("NUMBER_OF_EVENTS_USED")->second);
               Status=rs->getInt(lookupMap.find("CHANNEL_STATUS_WORD")->second);
	       tree->Fill();
           }
	   theFile->Write();
           theFile->Close();
	   
	   if(iRowCount==0){
	      string com="rm "+fname; system(com.c_str());
	      cout<< "No data found for run "<<run<< " dataset "<< set << endl;
	   }else{
	      cout<< iRowCount <<" records found for run "<<run<< " dataset "<< set << endl;
	   }   
           stmnt->closeResultSet(rs);
           conn->terminateStatement(stmnt);
       }catch(SQLException& ex){
           cout<<"Exception thrown for readPedestalDataset"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
       }
  };
}; 
void usage(){
  cout << "Usage:" <<endl;
  cout << "DetDiagLEDReader list - to get list of available datasets" <<endl;
  cout << "DetDiagLEDReader get RUN DATASET - to get a pedestal data" <<endl;
  cout << "DetDiagLEDReader mget RUN_FROM RUN_TO - to get a pedestal data for range of the runs" <<endl;
  cout << "(example DetDiagLEDReader get 158900 2)" <<endl;

}

int main (int argc,char **argv){
  if(argc==1){
     usage();
  }
  string user =   "";
  string passwd = "";
  string db =     "cms_omds_lb";

  try{
     DetDiagLEDReader *r = new DetDiagLEDReader(user,passwd,db);
     if(argc==4 && strcmp(argv[1],"get")==0)  r->readLEDDataset(argv[2],argv[3]);
     else if(argc==4 && strcmp(argv[1],"mget")==0) r->readLEDDatasetRange(argv[2],argv[3]);
     else if(argc==2 && strcmp(argv[1],"list")==0) r->getDatasetList(); 
     else usage(); 
     delete (r);
  }catch(SQLException& ex){
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
  }
}
