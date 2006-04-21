// -*- C++ -*-
// 
// H4Run.h
//
// Class which stores H4 run info. To be managed by H4DBManager.
// 
// last change : $Date: 2003/09/04 12:15:57 $
// by          : $Author: koblitz $
//
//

#ifndef H4RUN_H
#define H4RUN_H

#ifdef HAVE_CONFIG_H
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/config.h"
#endif

#include <iostream>
#include "CalibCalorimetry/Ecal2004TBCondTools/interface/H4DBEntry.h"

class H4Run: public H4DBEntry{
 public:
  
  H4Run();
  virtual ~H4Run() {};

  //! Returns run-number
  virtual int getIndex() const;

  //! Returns the current subrun
  virtual int getSubIndex() const;

#ifdef WITH_MYSQL
  virtual int fillFromMySQL(TSQLServer *db, int run,
			    int subRun =-1, IndexMatch match=exact);
#endif
  
  //! read itself from an ASCII stream
  //! (see H4DBManager::readAsciiFile())
  virtual int fillFromAsciiFile(istream& is);
  
  //! write itself to an ASCII stream
  virtual int dumpToAsciiFile(ostream& os) const;
  
  virtual void print(Option_t *option="") const;

  // A lot of getter-functions for run-information

  //! Returns the run-number
  int getRunNumber() const { return runNumber; }

  //! Returns the sub-run number
  int getSubRun() const { return subRun; }

  //! Returns the start-time of the run

  //! The integer returned is a time_t (the  number 
  //! of seconds elapsed since 00:00:00 on January 1, 1970)
  //! To get the time in ASCII, use ctime(startTime)
  int getStartTime() const {return startTime; }

  //! Returns time of run-end, see getStartTime
  int getStopTime() const { return stopTime; }

  //! The sub-detector coded as an integer which was in the test-beam

  //! 1:proto2000, 2=proto_SIC, 3=proto99++, 4=PROTO2002, 5=M0Prime 
  int getSubDet() const { return subDet; }

  //! The place coded as an integer where the data was taken

  //! The place is either 11 for building 11 or 887 for the H4-Hall
  int getPlace() const { return place; }

  //! Returns the cristal number on which the beam was focused

  //! In building 11 the number was arbitrary for the number of the cristal
  //! which was tested. In the H4-Beamline, however, the number represents
  //! the number of the cristal in the respective supermodule
  int getCristalNumber() const { return cristalNumber; }

  //! The momentum of the test-beam
  int getPBeam() const { return pBeam; }

  //! Should be always 0, meaning that the run was not only locally logged
  int getLocalRec() const { return localRec; }

  //! Returns 1 for runs which were stored on Castor tapes
  int getCdrRec() const { return cdrRec; }

  //! The theta angle of the table position coded as an integer
  int getThetaTable() const { return thetaTable; }

  //! The phi angle of the table position coded as an integer
  int getPhiTable() const { return phiTable; }

  //! Number of events in run triggered by physics trigger
  int getNPhyTrg() const { return nPhyTrg; }

  //! Number of events in run triggered randomly for pedestals
  int getNPedTrg() const { return nPedTrg; }

  //! Number of events in run triggered by laser monitoring
  int getNCalTrg() const { return nCalTrg; }

  //! Unique run-id in database
  int getID() const { return ID; }

  //! Returns the run-type coded as an integer

  //! The number returned means the following:
  //! 1= Pedestal, 2=Beam, 3=LaserMonitoring, 4 = TempDark,
  //! 5=calibration, 6=BeamScan, 7=LaserRamp
  int getRunType() const { return runType; }

  //! Retruns the rose-mode coded as an integer

  //! The rose modes are coded as follows: DBG = 1, DAQ = 2
  int getRoseMode() const { return roseMode; }

  //! The number of events ???
  int getNSamples() const { return nSamples; }

  //! Returns the vfe mode
  int getVfeMode() const { return vfeMode; }

  //! Number of events in run triggered by temperature monitoring
  int getNTempTrg() const { return nTempTrg; }
  
  //! Number of events in run triggered by electronics temperature monitoring
  int getNTemdTrg() const { return nTemdTrg; }

  //! Number of events in run triggered for electronics current monitoring
  int getNLeakTrg() const { return nLeakTrg; }

  //! Events triggered for unknown purposes
  int getNArefTrg() const { return nArefTrg; }


 private:
  Int_t runNumber;       // The run number
  Int_t subRun;          // The number of the sub-run
  Int_t startTime;       // The start time of the run (time_t) 
  Int_t stopTime;        // The time of run-end (time_t) 
  Int_t subDet;          // Number of sub-detector: 1:proto2000, 2=proto_SIC,
			 //  3=proto99++, 4=PROTO2002, 5=M0Prime 
  Int_t place;           // Place ???? 
  Int_t cristalNumber;   // The number of the cristal 
  Int_t pBeam;           // pBeam 
  Int_t localRec;        // local Reconstruction 
  Int_t cdrRec;          // cdr Reconstruction 
  Int_t thetaTable;      // theta position of table 
  Int_t phiTable;        // phi position of table 
  Int_t nPhyTrg;         // The number of physics triggers 
  Int_t nPedTrg;         // The number of pedelstal triggers 
  Int_t nCalTrg;         // ???? 
  Int_t ID;              // Run ID in database 
  Int_t runType;         // Type of run: 1= Pedestal, 2=Beam,
			 //  3=LaserMonitoring, 4 = TempDark, 5=calibration,
			 //  6=BeamScan, 7=LaserRamp 
  Int_t roseMode;        // Rose Mode 
  Int_t nSamples;        // number of samples 
  Int_t vfeMode;         // vfe mode 
  Int_t nTempTrg;        // ????? 
  Int_t nTemdTrg;        // ????? 
  Int_t nLeakTrg;        // ????? 
  Int_t nArefTrg;        // ????? 
  
  ClassDef(H4Run,1) // base class for DB objects
};

// H4RUN_H
#endif
