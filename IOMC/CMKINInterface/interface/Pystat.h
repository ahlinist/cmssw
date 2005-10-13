// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Pystat.h 
// -----------------------------------------------------------------------------
//  Date: 2004/11/17
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Pystat_H
#define Pystat_H
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
#define PYSTAT pystat_
#endif

extern "C" {
  void PYSTAT(int &istat);
}


#endif
