// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Hwevnt.h -> CMKIN++ HWEVNT common block interface
// -----------------------------------------------------------------------------
//  Date: 2004/11/23
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Hwevnt_h
#define Hwevnt_h

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


#define _lenidhw 4000
#define _lennrn  2




class DLLLINKAGE Hwevnt
{
 public:
  Hwevnt(void);
  ~Hwevnt(void);
  double &avwgt(void); 
  double &evwgt(void); 
  double &gamwt(void); 
  double &tlout(void); 
  double &wbigst(void); 
  double &wgtmax(void); 
  double &wgtsum(void); 
  double &wsqsum(void);
  int &idhw(int n); 
  int &ierror(void); 
  int &istat(void); 
  int &lwevt(void); 
  int &maxer(void); 
  int &maxpr(void);
  int &nowgt(void);
  int &nrn(int n);
  int &numer(void);
  int &numeru(void);
  int &nwgts(void);
  int &gensof(void);

 private:
  void init(void);

  static int nDummy;
  static double dDummy;

  //static const int  _lenidhw = 4000;
  //static const int  _lennrn  = 2;

  // map on block HWEVENT

  struct _hwevnt {
    double avwgt; 
    double evwgt; 
    double gamwt; 
    double tlout; 
    double wbigst; 
    double wgtmax; 
    double wgtsum; 
    double wsqsum;
    int idhw[_lenidhw]; 
    int ierror; 
    int istat; 
    int lwevt; 
    int maxer; 
    int maxpr;
    int nowgt;
    int nrn[_lennrn];
    int numer;
    int numeru;
    int nwgts;
    int gensof;
  };
  static struct _hwevnt *__hwevnt;

};

#endif







