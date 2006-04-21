// -*- C++ -*-
// 
/*! 
 \class H4Run
 \brief Class which is able to read in H4 Beamline run-info from the
 MySQL database. This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/09/05 16:11:24 $
 $Author: koblitz $

 The interface for the management by H4DBManager is inherited form
 H4RootDBEntry.
*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#else 
// Don't remove the following lines any longer !
// otherwise, the old build system won't work !
#ifdef __LINUX__
// glib2 needs this for strptime
#define _XOPEN_SOURCE
// TMath needs this for finite()
#define _GNU_SOURCE
#endif // __LINUX__
#endif



#ifdef HAVE_STRPTIME
// glib2 needs this for strptime
//#define _XOPEN_SOURCE
//#define _GNU_SOURCE
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Run.h"

#include <iostream>

ClassImp(H4Run)

#include <time.h>
#include "TSQLResult.h"
#include "TSQLRow.h"
#include <cstdlib>
#include <string>
#include <strstream>

using namespace std;

H4Run::H4Run(){
#ifdef DEBUG
  std::cout << "Setting matching method to indexOnly" << std::endl;
#endif
  matchMethod=indexOnly;
  valid=false;
}

int H4Run::getIndex() const{
  return runNumber;
}

int H4Run::getSubIndex() const{
  return subRun;
}

#ifdef WITH_MYSQL
int H4Run::fillFromMySQL(TSQLServer *db, int run, int srun,
			 IndexMatch match)
{
  // This method takes run and subrun-number and queries the mysql server
  // for appropriate runs. The information from the database is then
  // stored in the data members of the H4Run object

  if(db->SelectDataBase("rclog")){
    std::cerr << "Could not open rclog DB." << std::endl;
  }

  struct tm myTime;

  // Make query: 2 different queries exist: one where we alreay know
  // how many subruns there are and another where we need to find out.
  ostrstream query;

  if(srun == (int)NoSuchSubindex){
    query <<"select * from runs where run_number = " << run << ";" << '\0';
  }else
    query <<"select * from runs where run_number = " << run 
	  <<" and subrun = " << srun << ";" << '\0';

#ifdef DEBUG
  std::cout << "Using query string " << query.str() << std::endl;
#endif

  TSQLResult *res = db->Query( query.str() ); 

  if (!res) { // Some wery basic error handling if we lost the connection
    std::cout << "H4Run::fillFromMySQL: Conection to server broken!" << std::endl;
    return -1;
  }

  int nrows   = res->GetRowCount();
  int nfields = res->GetFieldCount();

  if (!nfields) return -1;

  if(nrows < 1){ // No run found
    valid=false;
    return -1;
  }

  if(nrows >1){ // A run with sub-Runs
    valid=false;
    TSQLRow *row = res->Next();

    if(!row->GetField(1)) return -1;  // Safeguard damaged db entries

    minSubIndex=maxSubIndex=atoi(row->GetField(1));
    while( (row = res->Next()) ){
#ifdef __LINUX__
      minSubIndex = (minSubIndex <? atoi(row->GetField(1)));
      maxSubIndex = (maxSubIndex >? atoi(row->GetField(1)));
#else
      minSubIndex = minSubIndex < atoi(row->GetField(1))?
	minSubIndex : atoi(row->GetField(1));
      maxSubIndex = maxSubIndex > atoi(row->GetField(1))?
	maxSubIndex : atoi(row->GetField(1));
#endif // __LINUX__		   

    }
    return nrows;
  }

  TSQLRow *row = res->Next();
  
  //  std::cout << "Field 0: " << row->GetField(0) << std::endl;

  runNumber=(atoi(row->GetField(0)));

  subRun=0;
  if(row->GetField(1))
    subRun=(atoi(row->GetField(1)));
  minSubIndex=maxSubIndex=subRun;

  startTime=0;
  if(row->GetField(2)){
    strptime(row->GetField(2),"%Y-%m-%d %T", &myTime);
    startTime=mktime(&myTime);
  }
  stopTime=0;
  if(row->GetField(3)){
    strptime(row->GetField(3),"%Y-%m-%d %T", &myTime);
    stopTime=mktime(&myTime);
  }

  subDet=0;
  if(row->GetField(4)){
    if(!strcmp("proto2000",row->GetField(4))) subDet=1;
    if(!strcmp("proto_SIC",row->GetField(4))) subDet=2;
    if(!strcmp("proto99+",row->GetField(4))) subDet=3;
    if(!strcmp("PROTO2002",row->GetField(4))) subDet=4;
    if(!strcmp("MOPrime",row->GetField(4))) subDet=5;
  }

  place =0;
  if(row->GetField(5))
    place=(atoi(row->GetField(5)));

  cristalNumber=0;
  if(row->GetField(6))
    cristalNumber=(atoi(row->GetField(6)));

  pBeam=0;
  if(row->GetField(7))
    pBeam=(atoi(row->GetField(7)));

  localRec=0;
  if(row->GetField(8))
    localRec=(atoi(row->GetField(8)));

  cdrRec=0;
  if(row->GetField(9))
    cdrRec=(atoi(row->GetField(9)));
  
  thetaTable=0;
  if(row->GetField(10))
    thetaTable=(atoi(row->GetField(10)));

  phiTable=0;
  if(row->GetField(11))
    phiTable=(atoi(row->GetField(11)));

  nPhyTrg=0;
  if(row->GetField(12))
    nPhyTrg=(atoi(row->GetField(12)));
  nPedTrg=0;
  if(row->GetField(13))
    nPedTrg=(atoi(row->GetField(13)));
  nCalTrg=0;
  if(row->GetField(14))
    nCalTrg=(atoi(row->GetField(14)));

  ID = (atoi(row->GetField(15)));

  runType=0;
  if(row->GetField(16)){
    if(!strcmp("Pedestal",row->GetField(16))) runType=1;
    if(!strcmp("Beam",row->GetField(16))) runType=2;
    if(!strcmp("LaserMonitoring",row->GetField(16))) runType=3;
    if(!strcmp("TempDark",row->GetField(16))) runType=4;
    if(!strcmp("calibration",row->GetField(16))) runType=5;
    if(!strcmp("BeamScan",row->GetField(16))) runType=6;
    if(!strcmp("LaserRamp",row->GetField(16))) runType=7;
  }

  roseMode=0;
  if(row->GetField(17)){
    if(!strcmp("DBG",row->GetField(17))) roseMode = 1;
    if(!strcmp("DAQ",row->GetField(17))) roseMode = 2;
  }  

  nSamples=0;
  if(row->GetField(18))
    nSamples = (atoi(row->GetField(18)));

  vfeMode=0;
  if(row->GetField(19))
    vfeMode = (atoi(row->GetField(19)));

  nTempTrg = 0;
  if(row->GetField(20))
    nTempTrg = (atoi(row->GetField(20)));
  nTemdTrg = 0;
  if(row->GetField(21))
    nTemdTrg = (atoi(row->GetField(21)));
  nLeakTrg =0;
  if(row->GetField(22))
    nLeakTrg = (atoi(row->GetField(22)));
  nArefTrg =0;
  if(row->GetField(23))
    nArefTrg = (atoi(row->GetField(23)));

  delete row;
  delete res;
  valid = true;
  return nrows;
}
#endif


int H4Run::fillFromAsciiFile(istream& is)
{
  if( !is.good() ) 
    return -1;
  
  is >> runNumber      
     >> subRun         
     >> startTime      
     >> stopTime       
     >> subDet         
     >> place          
     >> cristalNumber  
     >> pBeam          
     >> localRec       
     >> cdrRec         
     >> thetaTable     
     >> phiTable       
     >> nPhyTrg        
     >> nPedTrg        
     >> nCalTrg        
     >> ID             
     >> runType        
     >> roseMode       
     >> nSamples       
     >> vfeMode        
     >> nTempTrg       
     >> nTemdTrg       
     >> nLeakTrg       
     >> nArefTrg;
  
  return 1;
}


int H4Run::dumpToAsciiFile(ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << runNumber      << " "
     << subRun         << " "
     << startTime      << " "
     << stopTime       << " "
     << subDet         << " "
     << place          << " "
     << cristalNumber  << " "
     << pBeam          << " "
     << localRec       << " "
     << cdrRec         << " "
     << thetaTable     << " "
     << phiTable       << " "
     << nPhyTrg        << " "
     << nPedTrg        << " "
     << nCalTrg        << " "
     << ID             << " "
     << runType        << " "
     << roseMode       << " "
     << nSamples       << " "
     << vfeMode        << " "
     << nTempTrg       << " "
     << nTemdTrg       << " "
     << nLeakTrg       << " "
     << nArefTrg << std::endl;
  
  return 1;
}

void H4Run::print(Option_t *option) const{
  std::cout << runNumber << ":  sub run: " << subRun 
       << "  start time: " << ctime((const time_t *)&startTime)
       << "  stop time: " << ctime((const time_t *)&stopTime);
}  
