// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kiutil.h -> CMKIN kiutil fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kiutil_h
#define Kiutil_h

#ifdef _WIN32
# pragma once
# ifdef DLLEXPORT
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllexport)
#  endif
# else
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllimport)
#  endif
# endif
#endif
#ifndef DLLLINKAGE
# define DLLLINKAGE
#endif

#ifndef _WIN32
//
#define etafun etafun_
#define givept givept_
#define ki_isolation ki_isolation_
#define ki_pyts2pytd ki_pyts2pytd_
#define ki_txs2txd ki_txs2txd_
#define kiu_fill_hepevt kiu_fill_hepevt_
#define kiu_pyhepc kiu_pyhepc_
#define kiu_zeroepevt kiu_zeroepevt_
#define phidis phidis_
#define phifun phifun_
//
#else
//
#define etafun ETAFUN
#define givept GIVEPT
#define ki_isolation KI_ISOLATION
#define ki_pyts2pytd KI_PYTS2PYTD
#define ki_txs2txd KI_TXS2TXD
#define kiu_fill_hepevt KIU_FILL_HEPEVT
#define kiu_pyhepc KIU_PYHEPC
#define kiu_zeroepevt KIU_ZEROEPEVT
#define phidis PHIDIS
#define phifun PHIFUN

//
#endif

//From ROOT file TCallf77
//(From Pythia6Functions.h, I.Gonzalez Caballero)
#ifndef WIN32
# define type_of_call
#else
# define type_of_call  _stdcall
#endif




extern "C" {


  extern "C" float type_of_call etafun(float *X, float *Y, float *Z);


  // Action: To generate Pt in the range PTMIN-PTMAX
  //         Distribution is of the form  x*exp(-a*x)            0<x<x0
  //                                      c*(x**alpha+b)**beta  x0<x
  //         The latter parametrization from CMS TN/95-150
  // Input: A (exp slope par), PTMIN, PTMAX (GeV)
  // Output: PT (transverse momentum, GeV)
  extern "C" float type_of_call givept(float &A,    float &PTMIN,
				       float &PTMAX, float *PT);


  extern "C" void type_of_call ki_isolation();


  // Action: Conversion Pythia single precision <-> double precision
  //         (for only modifyable arrays, see ki_def_keys.F)
  // Input:  MODE = 1/2  D2S / S2D
  // Output: None
  extern "C" void type_of_call ki_pyts2pytd(int &MODE);


  // Action: Conversion TopReX single precision <-> double precision
  //         (for only modifyable arrays, see ki_def_keys.F)
  // Input:  MODE = 1/2  D2S / S2D
  // Output: None
  extern "C" void type_of_call ki_txs2txd(int &MODE);


  // Action: CMKIN utility to fill HEPEVT common block
  //         N.B. currently only Pythia is supported
  // Input:  None
  // Output: None
  extern "C" void type_of_call kiu_fill_hepevt();



  // Converts PYTHIA event record contents to or from
  // the standard event record commonblock.
  extern "C" void type_of_call kiu_pyhepc(int *MCONV);


  // Action: CMKIN utility to sets contents of HEPEVT common to zero
  // Input:  None
  // Output: None
  extern "C" void type_of_call kiu_zeroepevt();


  extern "C" float type_of_call phidis(float *PHI1, float PHI2);

  extern "C" float type_of_call phifun(float *X, float *Y);


  // Function comments are from the original fortran code

}

#endif



