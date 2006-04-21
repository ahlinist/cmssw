/*!
 \class H4PnLinearity
 \brief Class which is able to read polynom coefficients used to correct
 non-linearities in raw PN amplitudes

 $Date: 2004/12/17 09:59:24 $
 $Author: brunelie $

*/

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4PnLinearity.h"

ClassImp(H4PnLinearity)

#include "TSQLResult.h"
#include "TSQLRow.h"
#include "stdlib.h"
#include <string>
#include <strstream>
#include <iostream>
  
H4PnLinearity::H4PnLinearity()
{
  // matchMethod = sloppy;
  // matchMethod = indexOnly;
  matchMethod = exact;
  valid = false;
}

H4PnLinearity::~H4PnLinearity()
{
  // Do nothing
}

int H4PnLinearity::getIndex() const
{
  return runNumber_;
}

int H4PnLinearity::getSubIndex() const
{
  return option_*10000 + gain_*100 + pnNumber_;
}

#ifdef WITH_MYSQL
int H4PnLinearity::fillFromMySQL(TSQLServer *db, int runNumber, int subIndex,
				 IndexMatch match)
{
  if (db->SelectDataBase("h4analysis")) 
    std::cerr <<"Could not open h4analysis DB." << std::endl;
  
  // Decode subIndex
  int option = subIndex/10000;
  int gain = (subIndex - option*10000)/100;
  int pnNb = subIndex%100;

  // Make query: 2 different queries exist: one where we alreay know
  // how many event types there are and another where we need to find out.
  std::strstream query;

  if (subIndex == (int)NoSuchSubindex)
    query <<"select * from pn_linearity where run_number = " << runNumber
	  << ";" << '\0';
  else {
    if (match != exact) { // Find the closest run number
      std::strstream squery;
      squery <<"select MAX(run_number) from pn_linearity where run_number <= " 
	     << runNumber << " and pn_option = " << option 
	     << " and pn_gain = " << gain << " and pn_number = " << pnNb 
	     << ";" << '\0';
      TSQLResult* res = db->Query(squery.str());
      
      int nrows   = res->GetRowCount();
      int nfields = res->GetFieldCount();
      if (nrows != 1 && nfields != 1) return -1;

      TSQLRow* row = res->Next();
      runNumber = (atoi(row->GetField(0)));
      delete row;
      delete res;
    }
    query <<"select * from pn_linearity where run_number = " << runNumber
	  << " and pn_option = " << option << " and pn_gain = " << gain 
	  << " and pn_number = " << pnNb << ";" << '\0';
  }
#ifdef DEBUG
  std::cout << "Using query string " << query.str() << std::endl;
#endif
  TSQLResult* res = db->Query(query.str()); 

  if (!res) { // Some wery basic error handling if we lost the connection
    std::cerr << "Conection to server broken!" << std::endl;
    return -1;
  }

  int nrows   = res->GetRowCount();
  int nfields = res->GetFieldCount();

  if (!nfields) return -1;

  if (nrows < 1) { // No runNumber found
    valid = false;
    return -1;
  }

  if (nrows > 1) { // Too many runNumber found
    valid = false;
    return nrows;
  }

  TSQLRow* row = res->Next();

  runNumber_ = 0;
  if (row->GetField(0)) runNumber_ = (atoi(row->GetField(0)));
  
  pnNumber_ = (atoi(row->GetField(1)));
  
  gain_ = 0;
  if(row->GetField(2)) gain_ = (atoi(row->GetField(2)));

  option_ = (atoi(row->GetField(3)));
  
  for (int order = 0; order < polynomOrder_; order++) {
    if (row->GetField(4 + order*2)) 
      polCoef_[order] = (atof(row->GetField(4 + order*2)));
    if (row->GetField(5 + order*2)) 
      polCoefErr_[order] = (atof(row->GetField(5 + order*2)));
  }

  if (row->GetField(12)) pnMin_ = (atof(row->GetField(12)));

  if (row->GetField(13)) pnMax_ = (atof(row->GetField(13)));

  delete row;
  delete res;
  valid = true;
  return 1;
}
#endif

void H4PnLinearity::print(Option_t* option) const
{
  std::cout << "PN linearity run_number = "<< runNumber_
	    << ", pn_number = " << pnNumber_ 
	    << ", pn_gain = " << gain_ << ", pn_option = " << option_ 
	    << " : " << std::endl;
  for (int order = 0; order < polynomOrder_; order++)
    std::cout << "order " << order << " coef = " << polCoef_[order]
	      << " +- " << polCoefErr_[order] << std::endl;
  std::cout << "PN min = " << pnMin_ << ", PN max = " << pnMax_ << std::endl;
}  

int H4PnLinearity::dumpToAsciiFile(ostream& os) const
{
  if (!os.good())
    return -1;
  
  os << runNumber_ << " "
     << pnNumber_  << " " 
     << gain_      << " " 
     << option_    << " "; 
  for (int order = 0; order < polynomOrder_; order++)
    os << polCoef_[order] << " " << polCoefErr_[order] << " ";
  os << pnMin_ << " " << pnMax_ << std::endl;
  
  return 1;
}
