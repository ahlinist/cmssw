/*!
 \class H4ShapeParam
 \brief Class which is able to read in shape info. 
 This class is written out to a root-database
 by the H4DBManager class.

 $Date: 2004/03/22 16:44:17 $
 $Author: koblitz $

*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4ShapeParam.h"

ClassImp(H4ShapeParam)
ClassImp(H4ShapeParam::data)

#include "TSQLResult.h"
#include "TSQLRow.h"
#include "stdlib.h"
#include <string>
#include <strstream>
#include <iostream>

using namespace std;

#include <iostream>

const char *H4ShapeParam::runTypes[]={
  "Unknown", "e-", "laser_blue", "laser_green", "laser_IR", "laser_red"};
 
H4ShapeParam::H4ShapeParam(){
  //matchMethod=sloppy;
  //matchMethod=indexOnly;
  matchMethod=exact;
  valid=false;
  int nbOfCrystals = 1701 ;
  params = new TClonesArray("H4ShapeParam::data",  nbOfCrystals) ;
}

H4ShapeParam::~H4ShapeParam(){
  if (params) delete params ;
}


int H4ShapeParam::getIndex() const{
  return runNumber;
}

int H4ShapeParam::getSubIndex() const{
  return evType;
}


#ifdef WITH_MYSQL
int H4ShapeParam::fillFromMySQL(TSQLServer *db, int run, int eventType,
				IndexMatch match)
{
 
  if(db->SelectDataBase("h4analysis")){
    std::cerr << "Could not open h4analysis DB." << std::endl;
  }


  // Make query: 2 different queries exist: one where we alreay know
  // how many event types there are and another where we need to find out.
  strstream query;

  if(eventType == (int)NoSuchSubindex){
    query <<"select * from pulse_fit where run_num = " << run 
	  << ";" << '\0';
  }else{
    if( match== exact ){
      query <<"select * from pulse_fit where run_num = " << run 
	    << " and run_type = \"" << runTypes[eventType] << "\" ;" << '\0';
    } else {
      strstream subQuery;
      subQuery <<"select MAX(run_num) from pulse_fit where run_num <= " << run 
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
      if(!row) return -1; // We got an empty result
      run=(atoi(row->GetField(0)));
      strstream a;
      query <<"select * from pulse_fit where run_num = " << run 
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

  if(nrows >=1){ // A run with lots of crystals/eventTypes
    // Check for different run-types
    valid=false;
    char buffer[20];
    buffer[0]='\0';
    for(int i=0; i<nrows; i++){
      TSQLRow *row = res->Next();
      if(!strlen(buffer)) strcpy(buffer, row->GetField(1));
      if(strcmp(buffer, row->GetField(1))){
	valid=false;
	return -1;
      }
    }
  }
  // A way to reset the row???
  params->Clear() ;
  res = db->Query( query.str() ); 
  TSQLRow *row;
  for(int i=0; i<nrows; i++){
    row = res->Next();
    data parametres;

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

    parametres.smCrystalNb=0;
    if(row->GetField(2))
      parametres.smCrystalNb=(atoi(row->GetField(2)));

    parametres.alpha=0;
    if(row->GetField(3))
      parametres.alpha=(atof(row->GetField(3)));
    
    parametres.tPeak=0;
    if(row->GetField(4))
      parametres.tPeak=(atof(row->GetField(4)));
    
    parametres.tMax=0;
    if(row->GetField(5))
      parametres.tMax=(atof(row->GetField(5)));

    parametres.usedRun=0;
    if(row->GetField(6))
      parametres.usedRun=(atoi(row->GetField(6)));

    std::cout << "params " 
	      << parametres.smCrystalNb << " " 
	      << parametres.alpha << " " 
	      << parametres.tPeak << " " 
	      << parametres.tMax << " " 
	      << parametres.usedRun << " "
	      << std::endl;

    new ((*params)[params->GetEntriesFast()]) data(parametres) ;
  }

  std::cout << "Print " << std::endl;
  
  print();

  delete row;
  delete res;
  valid = true;
  return 1 ;

}
#endif

void H4ShapeParam::print(Option_t *option) const{
  std::cout <<"runNumber: "<< runNumber
	    <<" evType: "<< evType
	    << std::endl ; 
  data * mydata ;
  for (int i=0 ; i<params->GetEntriesFast() ; i++)
    {
      mydata = (data *) (*params)[i] ;
      std::cout << mydata->smCrystalNb << " "
		<< mydata->alpha << " "
		<< mydata->tPeak << " "
		<< mydata->tMax  << " " 
		<< std::endl ;
    }
}  

int H4ShapeParam::dumpToAsciiFile(ostream& os) const
{
  if( !os.good() )
    return -1;
  
  os << runNumber      << " "
     << evType         << " " ;

  data * mydata ;
  for (Int_t i=0 ; i<params->GetEntriesFast() ; i++)
    {      
      mydata = (data *) (*params)[i] ;
      os << mydata->smCrystalNb << " "
	 << mydata->alpha << " "
	 << mydata->tPeak << " "
	 << mydata->tMax  << " " ;
    }
  os << std::endl;
  
  return 1;
}


bool H4ShapeParam::getShapeParam(Int_t smCrystal, Float_t & alpha, Float_t & tPeak, Float_t & tMax, bool toBeInit)
{
  alpha = 0. ;
  tPeak = 0. ;
  tMax = 0. ;
  data * mydata ;
  if (toBeInit)
    {
      shapeMap.clear() ;
      for (int i=0 ; i<params->GetEntriesFast() ; i++)
	{
	  mydata = (data *) (*params)[i] ;
	  int crys = mydata->smCrystalNb ;
	  shapeMap[crys] = *mydata ;
	}
    }
  
  map<int,data>::const_iterator it;
  it = shapeMap.find(smCrystal) ;
  if (it == shapeMap.end()) return false ;
  alpha = it->second.alpha ;
  tPeak = it->second.tPeak ;
  tMax = it->second.tMax ;
  return true ;
}
