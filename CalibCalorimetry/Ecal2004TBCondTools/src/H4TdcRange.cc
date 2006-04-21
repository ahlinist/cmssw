/*!
 \class H4TdcRange
 \brief Class which is able to read in tdc range information. 
 This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/09/24 09:10:18 $
 $Author: nrl $

*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4TdcRange.h"

ClassImp(H4TdcRange)

#include "TSQLResult.h"
#include "TSQLRow.h"
#include "stdlib.h"
#include <string>
#include <strstream>
#include <iostream>

using namespace std;

const char *H4TdcRange::runTypes[]={
  "Unknown", "e-", "laser_blue", "laser_green", "laser_IR", "laser_red"};
 
H4TdcRange::H4TdcRange(){
  matchMethod=sloppy;
  valid=false;
}

H4TdcRange::~H4TdcRange(){
}


int H4TdcRange::getIndex() const{
  return runNumber;
}

int H4TdcRange::getSubIndex() const{
  return evType;
}


#ifdef WITH_MYSQL
int H4TdcRange::fillFromMySQL(TSQLServer *db, int run, int eventType,
				IndexMatch match)
{
 
  if(db->SelectDataBase("h4analysis")){
    std::cerr << "Could not open h4analysis DB." << std::endl;
  }


  // Make query: 2 different queries exist: one where we alreay know
  // how many event types there are and another where we need to find out.
  strstream query;

  if(eventType == (int)NoSuchSubindex){
    query <<"select * from tdc where run_num = " << run 
	  << ";" << '\0';
  }else{
    if( match== exact ){
      query <<"select * from tdc where run_num = " << run 
	    << " and run_type = \"" << runTypes[eventType] << "\" ;" << '\0';
    } else {
      strstream subQuery;
      subQuery <<"select MAX(run_num) from tdc where run_num <= " << run 
	    <<" and run_type = \"" << runTypes[eventType] << "\";" << '\0';
      //  std::cout << "Subquery: " << subQuery.str() << std::endl;
      TSQLResult *res = db->Query( subQuery.str() ); 
      if(!res){
	std::cerr << "Conection to server broken!" << std::endl;
	return -1;
      }
      int nrows   = res->GetRowCount();
      int nfields = res->GetFieldCount();
      if(nrows!=1 || nfields !=1) return -1;
      
      TSQLRow *row = res->Next();
      run=(atoi(row->GetField(0)));
      strstream a;
      query <<"select * from tdc where run_num = " << run 
	    << " and run_type = \"" << runTypes[eventType] << "\";" << '\0';
    }
  }
  //std::cout << "Using query string " << query.str() << std::endl;

  TSQLResult *res = db->Query( query.str() ); 

  if (!res) { // Some wery basic error handling if we lost the connection
    std::cerr << "Conection to server broken!" << std::endl;
    return -1;
  }

  int nrows   = res->GetRowCount();
  int nfields = res->GetFieldCount();

  if (!nfields) return -1;

  if(nrows < 1){ // No run found
    valid=false;
    return -1;
  }

  if(nrows >1){ // Too many runs found
    valid=false;
    return nrows;
  }

  // A way to reset the row???
  res = db->Query( query.str() ); 
  TSQLRow *row=res->Next();

  runNumber=0;
  runNumber=(atoi(row->GetField(0)));
  
  evType=0;
  if(row->GetField(1)){
    if(!strcmp("e-",row->GetField(1))) evType=1;
    if(!strcmp("laser_blue",row->GetField(1))) evType=2;
    if(!strcmp("laser_green",row->GetField(1))) evType=3;
    if(!strcmp("laser_IR",row->GetField(1))) evType=4;
    if(!strcmp("laser_red",row->GetField(1))) evType=5;
  }

  tdcMin=0;
  if(row->GetField(2))
    tdcMin=(atoi(row->GetField(2)));
  
  tdcMax=0;
  if(row->GetField(3))
    tdcMax=(atoi(row->GetField(3)));
  
  delete row;
  delete res;
  valid = true;
  return 1 ;
  
}
#endif

void H4TdcRange::print(Option_t *option) const{
  std::cout << "runNumber: "<< runNumber
	    << "  evType: "<< evType
	    << "  min: " << tdcMin
	    << "  max: " << tdcMax << std::endl;
}  

int H4TdcRange::dumpToAsciiFile(ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << runNumber << " "
     << evType    << " "
     << tdcMin    << " "
     << tdcMax    << " "
     << std::endl;
  
  return 1;
}

