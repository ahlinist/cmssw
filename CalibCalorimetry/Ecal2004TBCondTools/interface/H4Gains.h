// H4Gains.h
//
// Class which stores information on the electronics gain factors.
// Managed by H4DBManager.
// 
// last change : $Date: 2003/10/02 09:29:33 $
// by          : $Author: koblitz $
//

#ifndef H4GAINS_H
#define H4GAINS_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include "TClonesArray.h"

class H4Gains: public H4DBEntry{
 public:
  H4Gains();
  virtual ~H4Gains() ;

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns the current subrun i.e. event type
  virtual int getSubIndex() const;

#ifdef WITH_MYSQL
  //! Main routine for filling from MySQL database;
  virtual int fillFromMySQL(TSQLServer *db, int version,
			    int crystal =0, IndexMatch match=exact);
#endif

  //! prints some information
  virtual void print(Option_t *option="") const;

  //! write itself to an ASCII stream
  virtual int dumpToAsciiFile(ostream& os) const;

  //! Returns the version of this calibration
  int getVersion() const { return version; }

  //! Returns the crystal in SuperModule numbering convention
  int getCrystal() const { return crystal; }

  //! Returns the base gain factor in highest gain (FPPA: 33)
  float getBaseGain() const { return baseGain; }

  //! Returns the ratio of the amplification of gain 33 to gain 9
  float getRatio9() const { return ratio9; }

  //! Returns the ratio of the amplification of gain 33 to gain 5
  float getRatio5() const { return ratio5; }

  //! Returns the ratio of the amplification of gain 33 to gain 1
  float getRatio1() const { return ratio1; }

 private:
  Int_t   version;         //< Version number
  Int_t   crystal;         //< The crystal number
  Float_t baseGain;        //< The base gain factor (33 for FPPA)
  Float_t ratio9;          //< The ratio of gain 9 to an assumed gain of 33
  Float_t ratio5;          //< The ratio of gain 5 to an assumed gain of 33
  Float_t ratio1;          //< The ratio of gain 1 to an assumed gain of 33
  

  ClassDef(H4Gains,1) // base class for DB objects
};

// H4GAINS_H
#endif
