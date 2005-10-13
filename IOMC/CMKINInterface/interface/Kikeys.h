// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kikeys.h -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/10
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------


#ifndef Kikeys_h
#define Kikeys_h


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

#define _nffword     50000   // Space for FFREAD;
#define _ncharPerKey 4       // N0. characters / key word
#define _ncut        100     // Number of CMKIN selection cuts
#define _lenPkine    10
#define _lenNrnhwg   2
#define _lenPars     200
#define _lenGfcncs   2
#define _depthGfcncs 8
#define _lenMidx     50
#define _depthMidx   4



// ***********************************
// *        FFREAD parameters        *
// ***********************************
//

class DLLLINKAGE Cfread
{
 public:
  Cfread(void);
  ~Cfread(void);
  double &ffspace(int n);
  
 private:
  void init(void);
  static int nDummy;
  static double dDummy;

  // static const int _nffword = 50000;
  
  // map on block CFREAD
  struct _cfread {
    double ffspace[_nffword];
  };
  static struct _cfread *__cfread;

};


// ***********************************
// *   General data card variables   *
// ***********************************

class DLLLINKAGE Kiffgen
{
 public:
  Kiffgen(void); 
  ~Kiffgen(void); 
  int &nTrig(void);        // No. of events to "trigger" (i.e. generate)
  int &nNsel(void);        // No. of events required to pass selection
  double &energyCMS(void);  // Centre of mass energy
  int &ksel(void);         // Selection flag
  int &icut(int n);        // Integer CMKIN selection cuts
  double &rcut(int n);      // Real    CMKIN selection cuts
  double &cut1(void);       // e/gamma trigger preselection cuts
  double &cut2(void);       // e/gamma trigger preselection cuts
  int &ikine(void);        // Herwig process number
  double &pkine(int n);
  int &ihwg(void);         // Herwig process number
  int &nrnhwg(int n);      // Herwig random seeds    (040816)

 private:
  void init(void);
                                                                                
  static int nDummy;
  static double dDummy;
  
  // static const int _ncut      = 100;
  // static const int _lenPkine  = 10;
  // static const int _lenNrnhwg = 2;

  // map on block KI_FFGEN
  struct _kiffgen {
    int nTrig;
    int nNsel;
    double energyCMS;
    int ksel;
    int icut[_ncut];
    double rcut[_ncut];
    double cut1;     
    double cut2;     
    int ikine;      
    double pkine[_lenPkine];
    int ihwg;     
    int nrnhwg[_lenNrnhwg];
  };
  static struct _kiffgen *__kiffgen;

};


class DLLLINKAGE Kiextra
{
 public:
  Kiextra(void);
  ~Kiextra(void);
  int &nNfil(void);

 private:
  void init(void);
                                                                                
  static int nDummy;
  static double dDummy;


  // map on block KI_EXTRA
  struct _kiextra {
    int nNfil;
  };
  static struct _kiextra *__kiextra;

};



class DLLLINKAGE Kitoprex
{
 public:
  Kitoprex(void);
  ~Kitoprex(void);
  int &txproc(void);

 private:

  void init(void);
  
  static int nDummy;
  static double dDummy;
  
  // map on block KI_TOPREX
  struct _kitoprex {
    int txproc;
  };
  static struct _kitoprex *__kitoprex;

};



class DLLLINKAGE Kitxpar
{
 public:
  Kitxpar(void);
  ~Kitxpar(void);
  int &ipars(int n);
  double &rpars(int n);

 private:
  void init(void);
                                                                                
  static int nDummy;
  static double dDummy;
  
  // static const int _lenPars = 200;

  // map on block KI_TXPAR  
  struct _kitxpar {
    int ipars[_lenPars];
    double rpars[_lenPars];
  };
  static struct _kitxpar *__kitxpar;

};




class DLLLINKAGE Kifcnc {

 public:
  Kifcnc(void);
  ~Kifcnc(void);
  double &gfcncs(int i, int j) ;

 private:
  void init(void);
                                                                                       
  static int nDummy;
  static double dDummy;

  // static const int _lenGfcncs = 2;
  // static const int _depthGfcncs = 8;

  //map on block KI_FCNC
  struct _kifcnc {
    double gfcncs[_lenGfcncs][_depthGfcncs];
  };
  static struct _kifcnc *__kifcnc;

};



class DLLLINKAGE Kitxdec 
{
 public:
  Kitxdec(void);
  ~Kitxdec(void);
  int &midx(int i, int j);

 private:
  void init(void);
                                                                                
  static int nDummy;
  static double dDummy;

  // static const int _lenMidx = 50;
  // static const int _depthMidx = 4;

  // map on block KI_TXDEC
  struct _kitxdec {
    int midx[_lenMidx][_depthMidx];
  };
  static struct _kitxdec *__kitxdec;

};


class DLLLINKAGE Kitaumod         //  To choose tauola code
{
 public:
  Kitaumod(void);
  ~Kitaumod(void);
  int &jakk1(void);
  int &jakk2(void);
  int &mdtau(void);

 private:
  void init(void);

  static int nDummy;
  static double dDummy;

  // map on block KI_TAUMOD
  struct _kitaumod {
    int jakk1;
    int jakk2;
    int mdtau;
  };
  static struct _kitaumod *__kitaumod;

};

#endif
