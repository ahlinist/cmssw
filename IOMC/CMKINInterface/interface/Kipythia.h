// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kipythia.h -> CMKIN kipythia fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/19
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kipythia_h
#define Kipythia_h

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
#define ki_fi_stab ki_fi_stab_
#define ki_pyt_init ki_pyt_init_
#define kis_bd_psiks kis_bd_psiks_
#define kis_eemm kis_eemm_
#define pyalps_srs pyalps_
//
#else
//
#define ki_fi_stab KI_FI_STAB
#define ki_pyt_init KI_PYT_INIT
#define kis_bd_psiks KIS_BD_PSIKS
#define kis_eemm KIS_EEMM
#define pyalps_srs PYALPS
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

  // Action: Find all stable daugters of a particle
  // Input:  LINB = index of particle in LUJETS
  // Output: NSDAU = number of stable daugters
  //         LSDAU = list of indices of stable particles
  extern "C" int type_of_call ki_fi_stab (int &LINB, int *NSDAU, int *LSDAU);

  
  // Action: Initialise PYTHIA parameters within CMKIN package
  // Input:  None
  // Output: None
  extern "C" void type_of_call ki_pyt_init();


  // Action: Selection routine (Pythia) for B0 --> J/psi K0s
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" bool type_of_call kis_bd_psiks(int &IFLAG, bool *IWANT);


  // Action: Selection routine (Pythia) for H -> Z Z* -> e e mu mu
  // Input:  I_FLAG = -1/0/1 for initialisation/event loop/termination
  // Output: I_WANT = .TRUE. to select event; .FALSE. to reject event
  extern "C" void type_of_call kis_eemm(int &IFLAG, bool *IWANT);

  

  //Gives the value of alpha_strong.
  extern "C" double type_of_call pyalps(double *Q2);


  // Function comments are from the original fortran code

}

#endif



