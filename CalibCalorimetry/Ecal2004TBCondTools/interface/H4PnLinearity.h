// H4PnLinearity.h
//
// Class which stores information on polynom coefficients used to correct
// PN diodes non-linearities.
// Managed by H4DBManager.
// 
// last change : $Date: 2005/01/17 09:06:14 $
// by          : $Author: brunelie $
//

#ifndef H4PNLINEARITY_H
#define H4PNLINEARITY_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"
#include "TClonesArray.h"

class H4PnLinearity: public H4DBEntry {
 public:
  enum { polynomOrder_ = 4 };

  H4PnLinearity();
  virtual ~H4PnLinearity();

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns option_*10000 + gain_*100 + pnNumber_
  virtual int getSubIndex() const;

#ifdef WITH_MYSQL
  //! Main routine for filling from MySQL database;
  virtual int fillFromMySQL(TSQLServer *db, int runNumber,
			    int subIndex = 0, IndexMatch match = exact);
#endif

  //! prints some information
  virtual void print(Option_t *option = "") const;

  //! write itself to an ASCII stream
  virtual int dumpToAsciiFile(ostream& os) const;

  //! Returns the run number from which corrections are valid
  int getRunNumber() const { return runNumber_; }

  //! Returns pn diode number
  int getPnNumber() const { return pnNumber_; }
  
  //! Returns gain
  int getPnGain() const { return gain_; }

  //! Returns option (with/without zero subtraction)
  int getOption() const { return option_; }

  //! Returns order i polynom parameter
  double getPolynomCoef(int order = 0) const { return polCoef_[order]; }

  //! Returns order i polynom parameter error
  double getPolynomCoefError(int order = 0) const { 
    return polCoefErr_[order]; 
  }

  //! Returns minimal pn amplitude
  double getPnMin() const { return pnMin_; }

  //! Returns maximal pn amplitude
  double getPnMax() const { return pnMax_; }

  const static int getPolynomOrder() { return polynomOrder_; }

 private:
  int runNumber_;      //< Run number
  int pnNumber_;       //< PN diode number
  int gain_;           //< PN gain x1, x16
  int option_;         //< with/without zero subtraction
  double polCoef_[polynomOrder_];    //< Polynom coefficients
  double polCoefErr_[polynomOrder_]; //< Polynom coefficients errors
  double pnMin_;       //< Minimal PN amplitude
  double pnMax_;       //< Maximal PN amplitude

  ClassDef(H4PnLinearity,1) // base class for DB objects
};

// H4PNLINEARITY_H
#endif
