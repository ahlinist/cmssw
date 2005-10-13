// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kiffread.h -> CMKIN Kiffread fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kiffread_h
#define Kiffread_h

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
#define ki_check_pars ki_check_pars_
#define ki_def_keys ki_def_keys_
#define ki_ffgo ki_ffgo_
#define ki_ffkey2 ki_ffkey2_
//
#else
//
#define ki_check_pars KI_CHECK_PARS
#define ki_def_keys KI_DEF_KEYS
#define ki_ffgo KI_FFGO
#define ki_ffkey2 KI_FFKEY2
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


  // Action: Checks input parameters of CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_check_pars();


  // Action: Initialisation of FFREAD data cards for CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_def_keys();


  // Action: CMKIN version of FFGO (handles 2D arrays + inline comments)
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_ffgo();


  // Action: CMKIN utility to define FFREAD data card for a 2D array
  //         Based on the CERNLIB FFKEY routine
  // Input:  KEY    - character string containing the key to use
  // Input:  ADRESS - variable/array to store values to
  // Input:  I1,I2  - dimensions of the array
  // Input:  CHTYPE - character string containing either INTE, REAL,
  //                   or LOGI to signify ADRESS contains integer,
  //                   floating point, or logical data, respectively.
  // Output: None
  extern "C" void type_of_call ki_ffkey2(char &KEY, int &ADRESS,
					 int &I1, int &I2, char &CHTYPE); 


  // Function comments are from the original fortran code

}

#endif



