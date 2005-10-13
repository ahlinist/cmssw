// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kiunit.h -> Logical units for KINE package
// -----------------------------------------------------------------------------
//  Date: 2005/05/26
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
//
// -----------------------------------------------------------------------------
#ifndef Kiunit_h
#define Kiunit_h

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

#define _lun_ffin   5    // FFREAD input (standard input!)
#define _lun_ffout  6    // FFREAD output



#endif
