// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kicosmic.h -> CMKIN Kicosmic fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kicosmic_h
#define Kicosmic_h

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
#define ki_cos_fill ki_cos_fill_
#define ki_cos_init ki_cos_init_
#define ki_cos_stat ki_cos_stat_
//
#else
//
#define ki_cos_fill KI_COS_FILL
#define ki_cos_init KI_COS_INIT
#define ki_cos_stat KI_COS_STAT
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

  // Action: fill HEPEVT with cosmic muons
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_cos_fill();


  // Action: Initialise PYTHIA parameters within CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_cos_init();


  // Action: print statistics (number of tried/selected events)
  //         calculate muon flux
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_cos_stat();


  // Function comments are from the original fortran code

}

#endif


