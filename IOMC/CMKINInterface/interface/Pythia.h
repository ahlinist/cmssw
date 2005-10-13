// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Pythia.h 
// -----------------------------------------------------------------------------
//  Date: 2005/05/18
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
#define PY6INI py6ini_
#define PDFSET pdfset_
#define PYEVNT pyevnt_
#define PYLIST pylist_
#define PYHEPC pyhepc_
#define PYCOMP pycomp_
#endif

extern "C" {
  void PYSTAT(int &istat);
  // From HepEventPythia6Reader.h
  void PY6INI(float*);
  void PDFSET();
  void PYEVNT();
  void PYLIST(int*);
  void PYHEPC(int*);
  int PYCOMP(int*);
}


#endif
