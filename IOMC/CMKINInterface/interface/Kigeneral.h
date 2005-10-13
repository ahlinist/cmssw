// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kigeneral.h -> CMKIN kigeneral fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/18
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kigeneral_h
#define Kigeneral_h

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
#define ki_end ki_end_
#define ki_evt_sel ki_evt_sel_
#define ki_init ki_init_
#define ki_ntpl_def_100 ki_ntpl_def_100_
#define ki_ntpl_def_101 ki_ntpl_def_101_
#define ki_ntpl_def ki_ntpl_def_
#define ki_rd_evt ki_rd_evt_
#define ki_wr_evt ki_wr_evt_
//
#else
//
#define ki_end KI_END
#define ki_evt_sel KI_EVT_SEL
#define ki_init KI_INIT
#define ki_ntpl_def_100 KI_NTPL_DEF_100
#define ki_ntpl_def_101 KI_NTPL_DEF_101
#define ki_ntpl_def KI_NTPL_DEF
#define ki_rd_evt KI_RD_EVT
#define ki_wr_evt KI_WR_EVT
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

  // Action: Termination of CMKIN package
  // Input:  IFLAG = 0/1 (when creating/reading HEPEVT ntuple)
  // Output: None
  extern "C" void type_of_call ki_end(int &I_FLAG);

  // Action: Event selection within CMKIN package based on KSEL data card
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call ki_evt_sel(int &I_FLAG, bool &I_WANT);
    
  // Action: Initialisation of CMKIN package (FFREAD, HBOOK, etc.)
  // Input:  IFLAG = 0/1 (for creating/reading HEPEVT ntuple)
  // Output: None
  extern "C" void type_of_call ki_init(int &I_FLAG);

  // Action: Define CMKIN standard HEPEVT event n-tuple
  // Input:  IDN = identifier for ntuple
  // Input:  IFL = 0 => book the (not yet existing) ntuple
  // Input:  IFL = 1 => define the blocks of the existing ntuple
  // Output: None
  extern "C" void type_of_call ki_ntpl_def_100(int &IDN, int &IFL);


  // Action: Define CMKIN standard HEPEVT event n-tuple
  // Input:  IDN = identifier for ntuple
  // Input:  IFL = 0 => book the (not yet existing) ntuple
  // Input:  IFL = 1 => define the blocks of the existing ntuple
  // Output: None
  extern "C" void type_of_call ki_ntpl_def_101(int &IDN, int &IFL);


  // Action: Define CMKIN standard HEPEVT event n-tuple
  // Input:  IDN = identifier for ntuple
  // Input:  IFL = 0 => book the (not yet existing) ntuple
  // Input:  IFL = 1 => define the blocks of the existing ntuple
  // Output: None
  extern "C" void type_of_call ki_ntpl_def(int &IDN, int &IFL);
  
  
  // Action: Filling of ntuple in CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_rd_evt(int *IDCWN);

  
  // Action: Filling of ntuple in CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_wr_evt();


  // All the comments are from the original fortran code

}

#endif

