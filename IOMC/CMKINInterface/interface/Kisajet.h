// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kisajet.h -> CMKIN kisajet fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kisajet_h
#define Kisajet_h

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
#define ki_end_isa ki_end_isa_
#define ki_init_isa ki_init_isa_
#define ki_isa_init ki_isa_init_
#define ki_isa_keys ki_isa_keys_
#define ki_isa_sel ki_isa_sel_
#define ki_isahep ki_isahep_
#define kir_anal_isa kir_anal_isa_
//
#else
//
#define ki_end_isa KI_END_ISA
#define ki_init_isa KI_INIT_ISA
#define ki_isa_init KI_ISA_INIT
#define ki_isa_keys KI_ISA_KEYS
#define ki_isa_sel KI_ISA_SEL
#define ki_isahep KI_ISAHEP
#define kir_anal_isa KIR_ANAL_ISA
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


  // Action: Termination of CMKIN package for ISAJET
  // Input:  IFLAG = 0/1 (when creating/reading HEPEVT ntuple)
  // Output: None
  extern "C" void type_of_call ki_end_isa(int *IFLAG);



  // Action: Initialisation of CMKIN package (FFREAD, HBOOK, etc.)
  // Input:  IFLAG = 0/1 (for creating/reading HEPEVT ntuple)
  // Output: None
    extern "C" void type_of_call ki_init_isa(int *IFLAG);


  // Action:    Initialisation for ISAJET in CMGEN package
  // Called by: KI_ISA_MAIN
  // Input:     None
  // Output:    IFL    = 0 for success
  //?????????????????????????????????????????????????????
  //?????????????????????????????????????????????????????
  extern "C" int type_of_call ki_isa_init(char *IFL);
  
  

  // Action: Initialisation of FFREAD data cards for CMKIN package
  //         for HERWIG generation
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_isa_keys();


  // Action: User routine for HERWIG event selection
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ki_isa_sel(int *IFLAG; bool IWANT);



  // Action: To convert ISAJET event record contents to or from
  //         the standard event record common block.
  //   Convert (MCONV=1) from ISAJET numbering scheme to PDG numbering
  //        or (MCONV=2) from PDG numbering scheme to ISAJET numbering
  // Called by: KI_ISA_MAIN
  extern "C" void type_of_call ki_isahep(int *MCONV);



  // Action: User analysis for CMKIN "Reading ntuples" task
  //         Called once per event (e.g. to fill histograms from HEPEVT)
  //         Initialise the histograms in src/kine/kiuser/kir_init.F
  // Input:  None
  // Output: None
  extern "C" void type_of_call kir_anal_isa();


  // Function comments are from the original fortran code

}

#endif



