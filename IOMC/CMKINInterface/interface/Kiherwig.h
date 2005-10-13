// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kiherwig.h -> CMKIN kiherwig fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kiherwig_h
#define Kiherwig_h

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
#define kiu_hwg_hepevt kiu_hwg_hepevt_
#define hwevent hwevent_
#define kir_anal_hwg kir_anal_hwg_
#define ki_end_hwg ki_end_hwg_
#define ki_hwg_keys ki_hwg_keys_
#define ki_hwg_sel ki_hwg_sel_
#define ki_hwg_init ki_hwg_init_
#define ki_init_hwg ki_init_hwg_
//
#else
//
#define kiu_hwg_hepevt KIU_HWG_HEPEVT
#define hwevent HWEVENT
#define kir_anal_hwg KIR_ANAL_HWG
#define ki_end_hwg KI_END_HWG
#define ki_hwg_keys KI_HWG_KEYS
#define ki_hwg_sel KI_HWG_SEL
#define ki_hwg_init KI_HWG_INIT
#define ki_init_hwg KI_INIT_HWG
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

  extern "C" void type_of_call hwevent();


  // Action: Termination of CMKIN package for HERWIG
  // Input:  IFLAG = 0/1 (when creating/reading HEPEVT ntuple)
  // Output: None
  extern "C" void type_of_call ki_end_hwg(int &IFLAG);


  // Action: Initialise HERWIG parameters within CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_hwg_init();


  // Action: Initialisation of FFREAD data cards for CMKIN package
  //         for HERWIG generation
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_hwg_keys();


  // Action: User routine for HERWIG event selection
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ki_hwg_sel(int &IFLAG, bool &IWANT);



  // Action: Initialisation of CMKIN package (FFREAD, HBOOK, etc.)
  // Input:  IFLAG = 0/1 (for creating/reading HEPEVT ntuple)
  // Output: None
  extern "C" void type_of_call ki_init_hwg(int &IFLAG);



  // Action: User analysis for CMKIN "Reading ntuples" task
  //         Called once per event (e.g. to fill histograms from HEPEVT)
  //         Initialise the histograms in src/kine/kiuser/kir_init.F
  // Input:  None
  // Output: None
  extern "C" void type_of_call kir_anal_hwg();


  // Action: CMKIN utility to fill HEPEVT common block
  //         N.B. HERWIG supported
  // Input:  None
  // Output: None

  extern "C" void type_of_call kiu_hwg_hepevt();


// Function comments are from the original fortran files

}

#endif


