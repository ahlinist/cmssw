// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kicwn.h -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/16
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------

#ifndef Kicwn_h
#define Kicwn_h

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


#define _nwpawc      2000000



class DLLLINKAGE Pawc
{
 public:
  Pawc(void);
  ~Pawc(void);
  double &hpawc(int n);
    
 private:
  void init(void);
  static int nDummy;
  static double dDummy;

  // static const int _nwpawc = 2000000;
  
  // map on block PAWC
  struct _pawc {
    double hpawc[_nwpawc];
  };
  static struct _pawc *__pawc;

};



class DLLLINKAGE Kicwn
{
 public:
  Kicwn(void);
  ~Kicwn(void);
  int &idcwn(void);
                                                                                      
 private:
  void init(void);
  static int nDummy;
  static double dDummy;
                                                                                      
  // map on block KICWN
  struct _kicwn {
    int idcwn;
  };
  static struct _kicwn *__kicwn;
                                                                                      
};





#endif
