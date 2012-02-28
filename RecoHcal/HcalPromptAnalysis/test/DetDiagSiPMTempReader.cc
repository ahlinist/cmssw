#include <iostream>
#include <sstream>
#include <map>
#include <occi.h>
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"

using namespace oracle::occi;
using namespace std;

class  DetDiagSiPMTempReader{
private:
  Environment *env;
  Connection *conn;
  Statement *stmt;
  map<string,unsigned int> lookupMap;

public:
  DetDiagSiPMTempReader(string user,string passwd,string db){
    env = Environment::createEnvironment(Environment::DEFAULT);
    conn = env->createConnection(user,passwd,db);
  }

  ~DetDiagSiPMTempReader(){
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);
  }

  void getTemp(char *rbx,char *rm){
      map<unsigned int,string> runs;
      try{
          Statement* stmnt= conn->createStatement("SELECT CONDR.RUN_NUMBER,TO_CHAR(RUNINFO.BOOKINGTIME,'DD-MM-YYYY HH24:MI:SS') \
             ,RUNINFO.BOOKINGTIME FROM CMS_HCL_CORE_COND.COND_DATA_SETS COND INNER JOIN CMS_HCL_CORE_COND.COND_RUNS CONDR \
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
          while(rs->next()){ 
             runs.insert(pair<unsigned int,string>(rs->getInt(1),rs->getString(2)));
          }
          int col=runs.size();
          map<unsigned int,string>::iterator p;
          int run=0; string time;
          for(p=runs.begin();p!=runs.end();++p){
             if(col==0){ run=p->first; time=p->second; col++; continue;} 

             string str;
             if(run<=168723){
                str="SELECT \
                TO_CHAR(HSIPM.CHANGE_DATE,'DD-MM-YYYY HH24:MI:SS') CHANGE_DATE, HSIPM.STATUS_READTEMP_RM"; str+=rm; str+=" READ_TEMP \
                FROM CMS_HCL_CORE_CONSTRUCT.V_HBHEHO_RBX_RM_HPD_QIE_ADC HRDMAP \
                INNER JOIN CMS_HCL_CORE_PVSS_COND.DP_NAME2ID DPOINT \
                ON 'cms_hcal_dcs_02:HCAL_SiPM/'||HRDMAP.SECTOR||'_SiPM' = DPOINT.DPNAME \
                INNER JOIN CMS_HCL_CORE_PVSS_COND.HCAL_SIPM HSIPM \
                ON DPOINT.ID = HSIPM.DPID \
                WHERE HRDMAP.PIXEL = 1 AND HRDMAP.SECTOR = '";str+=rbx; str+="' \
                AND HRDMAP.RM_POS = SUBSTR('STATUS_READTEMP_RM"; str+=rm; str+="',19) \
                AND HSIPM.STATUS_READTEMP_RM"; str+=rm; str+=" > 0 \
                AND HSIPM.CHANGE_DATE BETWEEN TO_TIMESTAMP(:1,'DD-MM-YYYY HH24:MI:SS') \
                AND TO_TIMESTAMP(:2,'DD-MM-YYYY HH24:MI:SS') ORDER BY HSIPM.CHANGE_DATE";
              }else{
                str="SELECT \
                TO_CHAR(HSIPM.CHANGE_DATE,'DD-MM-YYYY HH24:MI:SS') CHANGE_DATE, HSIPM.STATUS_TEMP_RM"; str+=rm; str+=" READ_TEMP \
                FROM CMS_HCL_CORE_CONSTRUCT.V_HBHEHO_RBX_RM_HPD_QIE_ADC HRDMAP \
                INNER JOIN CMS_HCL_CORE_PVSS_COND.DP_NAME2ID DPOINT \
                ON 'cms_hcal_dcs_02:HCAL_SiPM/'||HRDMAP.SECTOR||'_SiPM' = DPOINT.DPNAME \
                INNER JOIN CMS_HCL_CORE_PVSS_COND.HCAL_SIPM HSIPM \
                ON DPOINT.ID = HSIPM.DPID \
                WHERE HRDMAP.PIXEL = 1 AND HRDMAP.SECTOR = '";str+=rbx; str+="' \
                AND HRDMAP.RM_POS = SUBSTR('STATUS_TEMP_RM"; str+=rm; str+="',15) \
                AND HSIPM.STATUS_TEMP_RM"; str+=rm; str+=" > 0 \
                AND HSIPM.CHANGE_DATE BETWEEN TO_TIMESTAMP(:1,'DD-MM-YYYY HH24:MI:SS') \
                AND TO_TIMESTAMP(:2,'DD-MM-YYYY HH24:MI:SS') ORDER BY HSIPM.CHANGE_DATE";

              }
              //prepare oracle sql statement
              Statement* stmnt1= conn->createStatement(str);

              stmnt1->setString(1,time);
              stmnt1->setString(2,p->second);

              //excute statement
              ResultSet* rs1=stmnt1->executeQuery();
              stmnt1->setPrefetchRowCount(10000);

              if(rs1->next()){
                 cout << run << " " << time << " ";
                 cout << rs1->getFloat(2) << endl;
              }//else cout << "no DCS information found for run " << run << endl;
              stmnt1->closeResultSet(rs1);
              conn->terminateStatement(stmnt1);
              run=p->first; time=p->second; col++;
         }
      }catch(SQLException& ex){
           cout<<"Exception thrown for DetDiagSiPMTempReader"<<endl;
           cout<<"Error number: "<<  ex.getErrorCode() << endl;
           cout<<ex.getMessage() << endl;
      }  
  };
}; 
void usage(){
  cout << "Usage: ./DetDiagSiPMTempReader HO1P10 1" << endl;
  cout << "HO1P10 - rbx, 1 - rm" << endl;
}

int main (int argc,char **argv){
  // Contact Hcal DB expert to to get inforamtion below
  string user =   "USER";
  string passwd = "PASSWORD";
  string db =     "DB";
  try{
     DetDiagSiPMTempReader *r = new DetDiagSiPMTempReader(user,passwd,db);
     r->getTemp(argv[1],argv[2]);
     delete (r);
  }catch(SQLException& ex){
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
  }
}
