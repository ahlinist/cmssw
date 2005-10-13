// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Pretauola.h 
// -----------------------------------------------------------------------------
//  Date: 2004/12/02
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Pretauola_H
#define Pretauola_H
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
#define PRETAUOLA pretauola_
#endif

extern "C" {
  void PRETAUOLA(int &istat);
}


#endif
