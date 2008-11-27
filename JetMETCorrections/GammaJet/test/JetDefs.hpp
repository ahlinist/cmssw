// Adapted from D0 Experiment jetcorr/jetcorr/JetDefs.hpp

#ifndef INC_JETDEFS
#define INC_JETDEFS
///////////////////////////////////////////////////////////////////////////////
// $Id: JetDefs.hpp,v 1.42 2007/11/25 23:59:29 mavoutil Exp $
// 
// File: JetDefs.hpp
//
// Purpose:  various definitions and parameters for jet corrections package
//
// Created:  Nov-3-20017   Mikko Voutilainen (adapted from D0 Experiment)
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "ErrorTypes.hpp"

namespace jec {

  const ErrorTypes kNone                = ErrorTypes(0L, 0L);
  const ErrorTypes kL3Stat              = ErrorTypes(0L, 1L << 0);
  const ErrorTypes kL3PhotonES          = ErrorTypes(0L, 1L << 1);
  //const ErrorTypes kL3QCDBackground     = ErrorTypes(0L, 1L << 2);
  const ErrorTypes kL3DeltaCStat        = ErrorTypes(0L, 1L << 2);
  const ErrorTypes kL3DeltaCRphot       = ErrorTypes(0L, 1L << 3);
  const ErrorTypes kL3DeltaCKphot       = ErrorTypes(0L, 1L << 4);
  //const ErrorTypes kL3DeltaCKphotSig    = ErrorTypes(0L, 1L << 5);
  const ErrorTypes kL3DeltaCRjet        = ErrorTypes(0L, 1L << 5);
  const ErrorTypes kL3DeltaCKjet        = ErrorTypes(0L, 1L << 6);
  const ErrorTypes kL3DeltaCKjetPeak    = ErrorTypes(0L, 1L << 7);
  const ErrorTypes kL3PurityStat        = ErrorTypes(0L, 1L << 8);
  const ErrorTypes kL3PurityID          = ErrorTypes(0L, 1L << 9);
  const ErrorTypes kL3PurityXsec        = ErrorTypes(0L, 1L << 10);
  const ErrorTypes kL3Purity2ndJet      = ErrorTypes(0L, 1L << 11);
  const ErrorTypes kL3PartonFrag        = ErrorTypes(0L, 1L << 12);
  const ErrorTypes kL3PartonUE          = ErrorTypes(0L, 1L << 13);
  const ErrorTypes kL3HardGluon         = ErrorTypes(0L, 1L << 14);

  //const ErrorTypes kL3DeltaC = kL3DeltaCStat | kL3DeltaCRphot | kL3DeltaCKphotBkg | kL3DeltaCKphotSig | kL3DeltaCRjet | kL3DeltaCKjet;
  const ErrorTypes kL3DeltaC = kL3DeltaCStat | kL3DeltaCRphot | kL3DeltaCKphot | kL3DeltaCRjet | kL3DeltaCKjet | kL3DeltaCKjetPeak;
  const ErrorTypes kL3Purity = kL3PurityStat | kL3PurityID | kL3PurityXsec | kL3Purity2ndJet;
  const ErrorTypes kL3QCDBackground = kL3DeltaC | kL3Purity;
  const ErrorTypes kL3Parton = kL3PartonFrag | kL3PartonUE;

  const ErrorTypes kL3Sys     = kL3PhotonES | kL3QCDBackground | kL3Parton | kL3HardGluon;

  const ErrorTypes kStat      = kL3Stat;
  const ErrorTypes kSys       = kL3Sys;
  const ErrorTypes kAll       = kStat | kSys;
  
} // namespace jec

#endif // INC_JETCORR
