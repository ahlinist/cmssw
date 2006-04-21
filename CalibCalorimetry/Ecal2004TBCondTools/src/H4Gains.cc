/*!
 \class H4Gains
 \brief Class which is able to read in gain factor information for
 the electronic. This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/10/03 08:59:32 $
 $Author: koblitz $

*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Gains.h"

ClassImp(H4Gains)

#include "TSQLResult.h"
#include "TSQLRow.h"
#include "stdlib.h"
#include <string>
#include <strstream>
#include <iostream>

using namespace std;

H4Gains::H4Gains(){
  //matchMethod=sloppy;
  //matchMethod=indexOnly;
  matchMethod=exact;
  valid=false;
}

H4Gains::~H4Gains(){
}


int H4Gains::getIndex() const{
  return version;
}

int H4Gains::getSubIndex() const{
  return crystal;
}


#ifdef WITH_MYSQL
int H4Gains::fillFromMySQL(TSQLServer *db, int vers, int crys,
				IndexMatch match)
{
 
  if(db->SelectDataBase("h4analysis")){
    std::cerr << "Could not open h4analysis DB." << std::endl;
  }


  // Make query: 2 different queries exist: one where we alreay know
  // how many event types there are and another where we need to find out.
  strstream query;

  if(crys == (int)NoSuchSubindex){
    query <<"select * from gain_factors where version = " << vers
	  << ";" << '\0';
  }else{
    if(match==exact) {
      query <<"select * from gain_factors where version = " << vers
	    << " and crystal = \"" << crys << "\" ;" << '\0';
    } else {
      
      strstream squery;
      squery <<"select * from gain_factors where version = " << vers
	    << ";" << '\0';
      TSQLResult *res = db->Query( squery.str() );
      
      int nfields = res->GetFieldCount();
//      cout << vers << "  " << crys << endl;
//      cout << "Found " << res->GetRowCount() << " rows!" << endl;
      if(nfields){
	int i;
	for(i=0; i< res->GetRowCount(); i++){
	  TSQLRow *row=res->Next();
	  if(atoi(row->GetField(1)) == crys){
//	    cout << "Found in list of rows!" << endl;
	    break;
	  }
	}
	if(i<res->GetRowCount()){
	  query <<"select * from gain_factors where version = " << vers
		<< " and crystal = \"" << crys << "\" ;" << '\0';
	} else {
	  query <<"select * from gain_factors where version = " << vers
		<< " and crystal = \"" << -1 << "\" ;" << '\0';
	}
	delete res;
      }
    }
  }
#ifdef DEBUG
  std::cout << "Using query string " << query.str() << std::endl;
#endif
  TSQLResult *res = db->Query( query.str() ); 

  if (!res) { // Some wery basic error handling if we lost the connection
    std::cerr << "Conection to server broken!" << std::endl;
    return -1;
  }

  int nrows   = res->GetRowCount();
  int nfields = res->GetFieldCount();

  if (!nfields) return -1;

  if(nrows < 1){ // No version found
    valid=false;
    return -1;
  }

  if(nrows >1){ // Too many versions found
    valid=false;
    return nrows;
  }

  TSQLRow *row=res->Next();

  version=0;
  if(row->GetField(0))
    version=(atoi(row->GetField(0)));
  
  crystal=0;
  if(row->GetField(1))
    crystal=(atoi(row->GetField(1)));
  
  baseGain=0.;
  if(row->GetField(2))
    baseGain=(atof(row->GetField(2)));
  
  ratio9=0.;
  if(row->GetField(3))
    ratio9=(atof(row->GetField(3)));
  
  ratio5=0;
  if(row->GetField(4))
    ratio5=(atof(row->GetField(4)));
  
  ratio1=0;
  if(row->GetField(5))
    ratio1=(atof(row->GetField(5)));
  
  delete row;
  delete res;
  valid = true;
  return 1;

}
#endif

void H4Gains::print(Option_t *option) const{
  std::cout << "Gain Factors version: "<< version
	    << " crystal " << crystal << " :  "
	    << " ratio gain 33 to 9 is " << ratio9
	    << ", 33 to 5 is " << ratio5
	    << ", 33 to 1 is " << ratio1
	    << std::endl; 
}  

int H4Gains::dumpToAsciiFile(ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << version    << " "
     << crystal    << " " 
     << baseGain   << " " 
     << ratio9     << " " 
     << ratio5     << " " 
     << ratio1     << " " 
     << std::endl;
  
  return 1;
}
