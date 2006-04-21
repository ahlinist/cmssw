// -*- C++ -*-
// 
// H4Intercalibration.h
//
// Class which stores intercalibration info. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/08 07:48:15 $
// by          : $Author: koblitz $
//
//

#ifndef H4INTERCALIBRATION_H
#define H4INTERCALIBRATION_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include <iostream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4Intercalibration: public H4DBEntry{
 public:
  
  H4Intercalibration();
  virtual ~H4Intercalibration() {};

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns the current subrun
  virtual int getSubIndex() const;

#ifdef WITH_MYSQL
  virtual int fillFromMySQL(TSQLServer *db, int vers,
			    int subIndex =-1, IndexMatch match=exact);
#endif
  int fillFromAsciiFile(istream& is);

  int dumpToAsciiFile(ostream& os) const;

  void print(Option_t *option="") const;

  //! The version of this intercalibration constant
  int getVersion() const { return versionNumber; };
  
  //! The crystal number
  int getCrystalNumber() const{ return crystalNumber; };

  //! The Pulse reconstruction method used
  int getPulseRecMethod() const { return pFitMethod; };

  //! The energy at which calibration was done
  float getEnergy() const { return energy; };

  //! The calibration constant itself
  float getConstant() const { return constant; };

  // //! The run number used to calculate this constant
  // int getRunNumberUsed() const { return runNumberUsed; }; 

private:

  Int_t     versionNumber;   // Place ???? 
  Int_t     crystalNumber;   // The number of the crystal
  Float_t   constant;        // Calibration constant
  Int_t     pFitMethod;      // Pulse reconstruction method used
  Float_t   energy;          // Energy used for fit
  
  ClassDef(H4Intercalibration,1) // base class for DB objects
};

// H4INTERCALIBRATION_H
#endif
