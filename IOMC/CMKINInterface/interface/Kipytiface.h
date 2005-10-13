// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kipytiface.h -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/10/28
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Héctor Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Kipytiface_h
#define Kipytiface_h

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


// *******************
// * From cfortran.h *
// *******************

#ifndef apolloFortran
#define COMMON_BLOCK_DEF(DEFINITION, NAME) extern DEFINITION NAME
#define CF_NULL_PROTO
#else                
/* HP doesn't understand #elif. */
/* Without ANSI prototyping, Apollo promotes float functions to double.    */
/* Note that VAX/VMS, IBM, Mips choke on 'type function(...);' prototypes. */
#define CF_NULL_PROTO ...
#ifndef __CF__APOLLO67
#define COMMON_BLOCK_DEF(DEFINITION, NAME) \
 DEFINITION NAME __attribute((__section(NAME)))
#else
#define COMMON_BLOCK_DEF(DEFINITION, NAME) \
 DEFINITION NAME #attribute[section(NAME)]
#endif
#endif


// ***************************************************
// * Interface to Pythia double precision parameters *
// *    All real arithmetic in single precision.     *
// ***************************************************

extern "C" {

  typedef struct {
    float PARUS[200];
    float PARJS[200];
    float PMASS[500][4];
    float PARFS[2000];
    float VCKMS[4][4];
    float BRATS[8000];
    float PARPS[200]; 
    float PARIS[200];
    float CKINS[200];
    float RMSSS[100];

  } s_params;
COMMON_BLOCK_DEF(s_params,S_PARAMS);

} 


#endif







