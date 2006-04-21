// H4ShapeParam.h
//
// Class which stores crystal info. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/10 13:12:23 $
// by          : $Author: koblitz $
//

#ifndef H4SHAPE_PARAM_H
#define H4SHAPE_PARAM_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include "TClonesArray.h"
#include <map>

class H4ShapeParam: public H4DBEntry{
 public:
  H4ShapeParam();
  virtual ~H4ShapeParam() ;

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

  //! Returns the shape parameters for a given crystal 
  bool H4ShapeParam::getShapeParam(Int_t smCrystal, Float_t & alpha, Float_t & tPeak, Float_t & tMax, bool toBeInit=true) ;

  class data : public TObject
    {
    public:
      data():smCrystalNb(0), alpha(0.), tPeak(0.), tMax(0.), usedRun(0) { } 
      Int_t smCrystalNb ;    /// The supermodule crystal nb
      Float_t alpha ;        /// Roughly the pulse shape slope
      Float_t tPeak ;        /// Rising time of pulse shape
      Float_t tMax ;         /// Time of maximum in clock unit of pulse shape
      Int_t usedRun ;        /// Run used to create this calibration
      ClassDef(data,1) 
    } ;

  const static char *runTypes[];    //! Run Type names in DB

 private:
  Int_t runNumber ;        //< The run number  
  Int_t evType ;           //< The event type
  TClonesArray * params ;  // an array containing the shape parameters
  std::map<int,data> shapeMap ; //!

  ClassDef(H4ShapeParam,1) // base class for DB objects
};

// H4SHAPE_PARAM_H
#endif
