// H4TdcRange.h
//
// Class which stores tdc information. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/10 09:10:42 $
// by          : $Author: koblitz $
//

#ifndef H4TDC_RANGE_H
#define H4TDC_RANGE_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4TdcRange: public H4DBEntry{
 public:
  H4TdcRange();
  virtual ~H4TdcRange() ;

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns the current subrun i.e. event type
  virtual int getSubIndex() const;

#ifdef WITH_MYSQL
  //! Main routine for filling from MySQL database;
  virtual int fillFromMySQL(TSQLServer *db, int run,
			    int runType =0, IndexMatch match=exact);
#endif

  //! prints some information
  virtual void print(Option_t *option="") const;

  //! write itself to an ASCII stream
  virtual int dumpToAsciiFile(ostream& os) const;

  //! Returns the run-number
  int getRunNumber() const { return runNumber; }

  //! Returns the event type
  int getEvtType() const { return evType; }

  //! Returns the minimal TDC
  int getTdcMin() const { return tdcMin; }

  //! Returns the maximal TDC
  int getTdcMax() const { return tdcMax; }

  const static char *runTypes[];    //! Run Type names in DB

 private:
  Int_t runNumber;        //< The run number  
  Int_t evType;           //< The event type
  Int_t tdcMin;           //< Minimum TDC
  Int_t tdcMax;           //< Maximum TDC

  ClassDef(H4TdcRange,1) // base class for DB objects
};

// H4TDC_RANGE_H
#endif
