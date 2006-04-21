// -*- C++ -*-
// 
// H4Alignment.h
//
// Class which stores intercalibration info. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/09 11:55:33 $
// by          : $Author: koblitz $
//
//

#ifndef H4ALIGNMENT_H
#define H4ALIGNMENT_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include <iostream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4Alignment: public H4DBEntry{
 public:
  
  H4Alignment();
  virtual ~H4Alignment() {};

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
  int getCrystalNumber() const { return crystalNumber; };

  //! The calibration constant itself
  float getX() const { return x; };

  //! The run number used to calculate this constant
  float getY() const { return y; }; 

private:

  Int_t    versionNumber;   /// Version of alignment 
  Int_t    crystalNumber;   /// The number of the crystal
  Float_t  x;               /// Calibration constant
  Float_t  y;               /// Number this constant comes from
  
  ClassDef(H4Alignment,1) // base class for DB objects
};

// H4ALIGNMENT_H
#endif
