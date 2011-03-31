#include <iostream>
#include <sstream>
#include <map>
#include <occi.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"

using namespace oracle::occi;
using namespace std;

class  DetDiagPedestsalReader{
private:
  Environment *env;
  Connection *conn;
  Statement *stmt;
  map<string,unsigned int> lookupMap;

public:
  DetDiagPedestsalReader(string user,string passwd,string db){
    env = Environment::createEnvironment(Environment::DEFAULT);
    conn = env->createConnection(user,passwd,db);
  }

  ~DetDiagPedestsalReader(){
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
  }
  void getDatasetList(){
      try{ 
          //prepare oracle sql statement
          Statement* stmnt= conn->createStatement("SELECT DISTINCT CONDR.RUN_NUMBER, CONDS.SET_NUMBER \
           FROM CMS_HCL_CORE_COND.COND_DATA_SETS CONDS \
           INNER JOIN CMS_HCL_CORE_COND.KINDS_OF_CONDITIONS KOC \
           ON CONDS.KIND_OF_CONDITION_ID = KOC.KIND_OF_CONDITION_ID \
           INNER JOIN CMS_HCL_CORE_COND.COND_RUNS CONDR \
           ON CONDS.COND_RUN_ID = CONDR.COND_RUN_ID \
           WHERE KOC.NAME = 'HCAL Pedestals [abort gap global]' \
           AND CONDS.IS_RECORD_DELETED = 'F' \
           AND KOC.IS_RECORD_DELETED = 'F' \
           AND CONDR.IS_RECORD_DELETED = 'F' \
           ORDER BY RUN_NUMBER");
          //excute statement
          ResultSet* rs=stmnt->executeQuery();
          stmnt->setPrefetchRowCount(10000);
          int iRowCount=0;
	  while(rs->next()){ 
	     iRowCount++;   
             cout << rs->getInt(1) << " ";
             cout << rs->getInt(2) << endl;
          }
	  cout << iRowCount << " datasets found"<< endl;
      }catch(SQLException& ex){
           cout<<"Exception thrown for getDatasetList"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
      }  
  }  
  void readPedestalDatasetRange(string from,string to){
      try{ 
          //prepare oracle sql statement
          Statement* stmnt= conn->createStatement("SELECT DISTINCT CONDR.RUN_NUMBER, CONDS.SET_NUMBER \
           FROM CMS_HCL_CORE_COND.COND_DATA_SETS CONDS \
           INNER JOIN CMS_HCL_CORE_COND.KINDS_OF_CONDITIONS KOC \
           ON CONDS.KIND_OF_CONDITION_ID = KOC.KIND_OF_CONDITION_ID \
           INNER JOIN CMS_HCL_CORE_COND.COND_RUNS CONDR \
           ON CONDS.COND_RUN_ID = CONDR.COND_RUN_ID \
           WHERE KOC.NAME = 'HCAL Pedestals [abort gap global]' \
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
	     //stringstream s(to); int TO; s >>TO;
	     //if(rs->getInt(1)>TO) continue; 
	     readPedestalDataset(rs->getString(1),rs->getString(2));
          }
      }catch(SQLException& ex){
           cout<<"Exception thrown for readPedestalDatasetRange"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
      }  
  }
  void readPedestalDataset(string run,string set){
       try{
           //prepare oracle sql statement
           Statement* stmnt= conn->createStatement("SELECT SUBDET,IETA,IPHI,DEPTH,MEAN0,MEAN1,MEAN2,MEAN3,RMS0,RMS1,RMS2,RMS3,NUMBER_OF_EVENTS_USED,CHANNEL_STATUS_WORD FROM \
	   CMS_HCL_HCAL_COND.V_HCAL_DETMON_PEDESTALS_V1 WHERE RUN_NUMBER=:1 AND SET_NUMBER=:2 AND KOC_TYPE='abort gap global'");
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
	   
	   double ped[4],rms[4];
           int    Eta,Phi,Depth,Statistic,Status;
	   char   Subdet[20];
	   
	   string fname="HcalDetDiagPedestalData_run"+run+"_"+set+".root";
	   TFile *theFile = new TFile(fname.c_str(), "RECREATE");
	   if(!theFile->IsOpen()) return;
	   theFile->cd();   
           TObjString RUN(run.c_str());    RUN.Write("run number");
	   TObjString dsnum(set.c_str());  dsnum.Write("Dataset number");
	   
           TTree *tree   =new TTree("HCAL Pedestal data","HCAL Pedestal data");
           if(tree==0)   return;
           tree->Branch("Subdet",   &Subdet,         "Subdet/C");
           tree->Branch("eta",      &Eta,            "Eta/I");
           tree->Branch("phi",      &Phi,            "Phi/I");
           tree->Branch("depth",    &Depth,          "Depth/I");
           tree->Branch("statistic",&Statistic,      "Statistic/I");
           tree->Branch("status",   &Status,         "Status/I");
           tree->Branch("cap0_ped", &ped[0],         "cap0_ped/D");
           tree->Branch("cap0_rms", &rms[0],         "cap0_rms/D");
           tree->Branch("cap1_ped", &ped[1],         "cap1_ped/D");
           tree->Branch("cap1_rms", &rms[1],         "cap1_rms/D");
           tree->Branch("cap2_ped", &ped[2],         "cap2_ped/D");
           tree->Branch("cap2_rms", &rms[2],         "cap2_rms/D");
           tree->Branch("cap3_ped", &ped[3],         "cap3_ped/D");
           tree->Branch("cap3_rms", &rms[3],         "cap3_rms/D");

	   while(rs->next()){
               iRowCount++ ;
               sprintf(Subdet,"%s",rs->getString(lookupMap.find("SUBDET")->second).c_str()); 
               Eta=rs->getInt(lookupMap.find("IETA")->second); 
               Phi=rs->getInt(lookupMap.find("IPHI")->second); 
               Depth=rs->getInt(lookupMap.find("DEPTH")->second);
               ped[0]=rs->getFloat(lookupMap.find("MEAN0")->second);
               ped[1]=rs->getFloat(lookupMap.find("MEAN1")->second);
               ped[2]=rs->getFloat(lookupMap.find("MEAN2")->second); 
               ped[3]=rs->getFloat(lookupMap.find("MEAN3")->second);
               rms[0]=rs->getFloat(lookupMap.find("RMS0")->second);
               rms[1]=rs->getFloat(lookupMap.find("RMS1")->second);
               rms[2]=rs->getFloat(lookupMap.find("RMS2")->second); 
               rms[3]=rs->getFloat(lookupMap.find("RMS3")->second);
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
       }catch(SQLException& ex){
           cout<<"Exception thrown for readPedestalDataset"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
       }
  };
}; 
void usage(){
  cout << "Usage:" <<endl;
  cout << "DetDiagPedestsalReader list - to get list of available datasets" <<endl;
  cout << "DetDiagPedestsalReader get RUN DATASET - to get a pedestal data" <<endl;
  cout << "DetDiagPedestsalReader mget RUN_FROM RUN_TO - to get a pedestal data for range of the runs" <<endl;
  cout << "(example DetDiagPedestsalReader get 158900 2)" <<endl;

}

int main (int argc,char **argv){
  // Contact Hcal DB expert to to get inforamtion below
  string user =   "USER";
  string passwd = "PASSWORD";
  string db =     "DB";

  try{
     DetDiagPedestsalReader *r = new DetDiagPedestsalReader(user,passwd,db);
     if(argc==4 && strcmp(argv[1],"get")==0)  r->readPedestalDataset(argv[2],argv[3]);
     else if(argc==4 && strcmp(argv[1],"mget")==0) r->readPedestalDatasetRange(argv[2],argv[3]);
     else if(argc==2 && strcmp(argv[1],"list")==0) r->getDatasetList(); 
     else usage(); 
     delete (r);
  }catch(SQLException& ex){
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
  }
}
