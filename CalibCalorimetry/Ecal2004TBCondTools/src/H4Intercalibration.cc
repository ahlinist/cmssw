// -*- C++ -*-
// 
/*! 
 \class H4Intercalibration
 \brief Class which is able to read in H4 Beamline run-info from the
 MySQL database. This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2003/09/08 14:46:11 $
 $Author: koblitz $

 The interface for the management by H4DBManager is inherited form
 H4RootDBEntry.
*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4Intercalibration.h"

#include <iostream>

ClassImp(H4Intercalibration)

#include <time.h>
#include "TSQLResult.h"
#include "TSQLRow.h"
#include <cstdlib>
#include <string>
#include <strstream>

using namespace std;

H4Intercalibration::H4Intercalibration(){
#ifdef DEBUG
  std::cout << "Setting matching method to exact" << std::endl;
#endif
  matchMethod=exact;
  valid=false;
}

int H4Intercalibration::getIndex() const{
  return versionNumber;
}

int H4Intercalibration::getSubIndex() const{
  return crystalNumber;
}

#ifdef WITH_MYSQL
int H4Intercalibration::fillFromMySQL(TSQLServer *db, int version,
				      int crystal,  IndexMatch match)
{
  // This method takes the version as index to access the database and
  // the crystal number as sub-index

  if(db->SelectDataBase("h4analysis")){
    std:: cerr << "Could not open h4analysis DB." <<std::endl;
  }

  ostrstream query;
  query <<"select * from intercalib where version = " << version 
	<< " and " << "crystal=" << crystal << ";" << '\0';

  //  std::cout << "Using query string " << query.str() << std::endl;

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

  energy=0.;
  if(row->GetField(1))
    energy=(atof(row->GetField(1)));

  pFitMethod=-1;
  if(row->GetField(2))
    pFitMethod=(atoi(row->GetField(2)));

  crystalNumber=-1;
  if(row->GetField(3))
    crystalNumber=(atoi(row->GetField(3)));

  constant=0.;
  if(row->GetField(4))
    constant=(atof(row->GetField(4)));

  delete row;
  delete res;
  valid = true;
  return nrows;
}
#endif


int H4Intercalibration::fillFromAsciiFile(istream& is)
{
  if( !is.good() ) 
    return -1;
  
  is >> versionNumber
     >> energy
     >> pFitMethod
     >> crystalNumber
     >> constant; 
  return 1;
}


int H4Intercalibration::dumpToAsciiFile(ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << versionNumber
     << energy
     << pFitMethod
     << crystalNumber
     << constant << std::endl;
  return 1;
}

void H4Intercalibration::print(Option_t *option) const{
  std::cout << versionNumber << ":  crystal: " << crystalNumber
	    << "  constant: " << constant;
}  
