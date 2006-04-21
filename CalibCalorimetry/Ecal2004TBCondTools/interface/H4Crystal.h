// H4Crystal.h
//
// Class which stores crystal info. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/04 12:15:57 $
// by          : $Author: koblitz $
//

#ifndef H4CRYSTAL_H
#define H4CRYSTAL_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4Crystal: public H4DBEntry{
 public:
  H4Crystal();
  virtual ~H4Crystal() {};

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns the current subrun
  virtual int getSubIndex() const;

  //! Returns the name of the table in which this entry resides;
  const char * getTableName() const;

#ifdef WITH_MYSQL
  //! Main routine for filling from MySQL database;
  virtual int fillFromMySQL(TSQLServer *db, int table,
			    int crystal,  IndexMatch match=exact);
#endif

  //! prints some information on the crystal
  virtual void print(Option_t *option="") const;

  // A lot of getter-functions for crystal-information

  //! Returns the linear channel number
  int getLinCh() const { return linCh; }

  //! Returns eta of the crystal
  int getEta() const {return eta; }

  //! Returns phi of the crystal
  int getPhi() const { return phi; }

  //! Returns sm_id (Supermodule ID ?)
  int getSmId() const { return smID; }

  //! Returns sm_ch (Supermodule channel ?)
  int getSmCh() const { return smCh; }

  //! Returns the rose id
  int getRoseId() const { return roseID; }

  //! Returns the rose channel
  int getRoseCh() const { return roseCh; }

  //! Returns the HV ID
  int getHVId() const { return hvID; }

  //! Return the HV channel
  int getHVCh() const { return hvCh; }

  //! Return the LV ID
  int getLVID() const { return lvID; }

  //! Return the LV channel
  int getPhiTable() const { return lvCh; }

 private:
  Int_t table;           // This is the table in the DB we are referring to:
                         // 1 = H4, 2 = M0prime, 3 = M0prime_40202,
                         // 4 = lab11, 5 = proto 2001, 6 = proto2002,
                         // 5 = prototest, 6 = simuMO
  Int_t linCh;           // Linear channel number
  Int_t eta;             // Eta integer coded
  Int_t phi;             // Phi integer coded
  Int_t smID;            // Supermodule ID?
  Int_t smCh;            // Supermodule channel?
  Int_t roseID;          // Rose ID 
  Int_t roseCh;          // Rose Channel
  Int_t hvID;            // High Voltage ID 
  Int_t hvCh;            // High Voltage Channel
  Int_t lvID;            // Low voltage ID 
  Int_t lvCh;            // Low voltag Channel
  
  const static char *tableNames[];  //! array with names of tables in DB

  ClassDef(H4Crystal,1) // base class for DB objects
};

// H4CRYSTAL_H
#endif
