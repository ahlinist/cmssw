// -*- C++ -*-
// 
/*! 
 \class H4Alignment
 \brief Class which is able to read in H4 Beamline run-info from the
 MySQL database. This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/09/09 11:59:55 $
 $Author: koblitz $

 The interface for the management by H4DBManager is inherited form
 H4RootDBEntry.
*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Alignment.h"

#include <iostream>

ClassImp(H4Alignment)

#include <time.h>
#include "TSQLResult.h"
#include "TSQLRow.h"
#include <cstdlib>
#include <string>
#include <strstream>

using namespace std;

H4Alignment::H4Alignment(){
#ifdef DEBUG
  std::cout << "Setting matching method to exact" << std::endl;
#endif
  matchMethod=exact;
  valid=false;
}

int H4Alignment::getIndex() const{
  return versionNumber;
}

int H4Alignment::getSubIndex() const{
  return crystalNumber;
}

#ifdef WITH_MYSQL
int H4Alignment::fillFromMySQL(TSQLServer *db, int version,
				      int crystal,  IndexMatch match)
{
  // This method takes the version as index to access the database and
  // the crystal number as sub-index

  if(db->SelectDataBase("h4analysis")){
    std::cerr << "Could not open h4analysis DB." << std::endl;
  }

  ostrstream query;

  query <<"select * from alignment where version = " << version 
	<< " and " << "crystal=" << crystal << ";" << '\0';
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

  if(nrows < 1){ // No entry found
    valid=false;
    return -1;
  }

  TSQLRow *row = res->Next();

  versionNumber=(atoi(row->GetField(0)));

  crystalNumber=-1;
  if(row->GetField(1))
    crystalNumber=(atoi(row->GetField(1)));

  x=0.;
  if(row->GetField(2))
    x=(atof(row->GetField(2)));

  y=0;
  if(row->GetField(3))
    y=(atof(row->GetField(3)));


  delete row;
  delete res;
  valid = true;
  return nrows;
}
#endif


int H4Alignment::fillFromAsciiFile(std::istream& is)
{
  if( !is.good() ) 
    return -1;
  
  is >> versionNumber
     >> crystalNumber
     >> x
     >> y;    
  return 1;
}


int H4Alignment::dumpToAsciiFile(std::ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << versionNumber
     << crystalNumber
     << x
     << y << std::endl;
  return 1;
}

void H4Alignment::print(Option_t *option) const{
  std::cout << versionNumber << ":  crystal: " << crystalNumber
	    << " x:" << x << "  y:" << y << std::endl;
}  
