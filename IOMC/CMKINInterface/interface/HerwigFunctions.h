// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  HerwigFunctions.h 
// -----------------------------------------------------------------------------
//  Date: 2004/11/17
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef HerwigFunctions_H
#define HerwigFunctions_H
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
#define HWEFIN hwefin_
#define HWAEND hwaend_
#endif

extern "C" {
  void HWEFIN(void );
}

extern "C" {
  void HWAEND(void );
}

#endif
