// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kikeys.cc -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/10
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Héctor Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------


#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/CMKINInterface/interface/Kikeys.h"

#ifndef _WIN32
# define CFREAD     cfread_
# define KI_FFGEN   ki_ffgen_
# define KI_EXTRA   ki_extra_
# define KI_TOPREX  ki_toprex_
# define KI_TXPAR   ki_txpar_
# define KI_FCNC    ki_fcnc_
# define KI_TXDEC   ki_txdec_
# define KI_TAUMOD  ki_taumod_
#endif


//***********************
//* CFREAD common block *
//***********************

extern "C" {
  void *getaddr(void *);

  extern struct {
    double ffspace[50000];
  } KI_FFGEN;
}

struct Cfread::_cfread * Cfread::__cfread = 0;

int Cfread::nDummy = 0;
double Cfread::dDummy = 0.0;

Cfread::Cfread(void)
{
}

Cfread::~Cfread(void)
{
}

void Cfread::init(void)
{
  __cfread = static_cast<struct Cfread::_cfread *>(getaddr(&CFREAD.ffspace[0]));
}

double &Cfread::ffspace(int n)
{
  if (__cfread == 0) init();
  if ((n<1)||(n>_nffword)) {
    dDummy = -999.0;
    return dDummy;
  }
  return Cfread::__cfread->ffspace[n-1];
}



//*************************
//* KI_FFGEN common block *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    int nTrig;
    int nNsel;
    double energyCMS;
    int ksel;
    int icut[100];
    double rcut[100];
    double cut1;
    double cut2;
    int ikine;
    double pkine[10];
    int ihwg;
    int nrnhwg[2];
  } KI_FFGEN;
}

struct Kiffgen::_kiffgen * Kiffgen::__kiffgen = 0;

int Kiffgen::nDummy = 0;
double Kiffgen::dDummy = 0.0;

Kiffgen::Kiffgen(void)
{
}

Kiffgen::~Kiffgen(void)
{
}

void Cfread::init(void)
{
  __kiffgen = static_cast<struct  Kiffgen::_kiffgen *>(getaddr(&KI_FFGEN.nTrig));
}


int &Kiffgen::nTrig(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->nTrig; 
}

int &Kiffgen::nNsel(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->nNsel;
}

double &Kiffgen::energyCMS(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->energyCMS;
}

int &Kiffgen::ksel(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->ksel;
}

int &Kiffgen::icut(int n)
{
  if (__kiffgen == 0) init();
  if ((n<1)||(n>_ncut)) {
    nDummy = -999;
    return nDummy;
  }
  return __kiffgen->icut[n-1];
}
      
double &Kiffgen::rcut(int n)
{
  if (__kiffgen == 0) init();
  if ((n<1)||(n>_ncut)) {
    dDummy = -999.0;
    return dDummy;
  }
  return __kiffgen->rcut[n-1];
}
  
double &Kiffgen::cut1(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->cut1;
}

double &Kiffgen::cut2(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->cut2;
}

int &Kiffgen::ikine(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->ikine;
}

double &Kiffgen::pkine(int n)
{
  if (__kiffgen == 0) init();
  if ((n<1)||(n>_lenPkine)) {
    dDummy = -999.0;
    return dDummy;
  }
  return __kiffgen->pkine[n-1];
}

double &Kiffgen::ihwg(void)
{
  if (__kiffgen == 0) init();
  return __kiffgen->ihwg;
}

int &Kiffgen::nrnhwg(int n)
{
  if (__kiffgen == 0) init();
  if ((n<1)||(n>_lenNrnhwg)) {
    nDummy = -999;
    return nDummy;
  }
  return __kiffgen->nrnhwg[n-1];
}




//*************************
//* KI_EXTRA common block *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    int nNfil;
  } KI_EXTRA;
}

struct Kiextra::_kiextra *Kiextra::__kiextra = 0;

int Kiextra::nDummy = 0;
double Kiextra::dDummy = 0.0;
                                                                                
Kiextra::Kiextra(void)
{
}
                                                                                
Kiextra::~Kiextra(void)
{
}

void Kiextra::init(void)
{
  __kiextra = static_cast<struct Kiextra::_kiextra *>(getaddr(&KI_EXTRA.nNfil));
}

int &Kiextra::nNfil(void)
{
  if (__kiextra == 0) init();
  return __kiextra->nNfil;
}



//*************************
//* KI_TXPAR common block *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    int ipars[200];
    double rpars[200];
  } KI_TXPAR;
}

struct Kitxpar::_kitxpar *Kitxpar::__kitxpar = 0;

int Kitxpar::nDummy = 0;
double Kitxpar::dDummy = 0.0;

Kitxpar::Kitxpar(void)
{
}

Kitxpar::~Kitxpar(void)
{
}

void Kitxpar::init(void)
{
  __kitxpar = static_cast<struct Kitxpar::_kitxpar *>(getaddr(&KI_TXPAR.ipars[0]));
}

int &Kitxpar::ipars(int n)
{
  if (__kitxpar == 0) init();
  if ((n<1)||(n>_lenPars)) {
    nDummy = -999;
    return nDummy;
  }
  return __kitxpar->ipars[n-1];
}

double &Kitxpar::rpars(int n)
{
  if (__kitxpar == 0) init();
  if ((n<1)||(n>_lenPars)) {
    dDummy = -999.0;
    return dDummy;
  }
  return __kitxpar->rpars[n-1];
}



//************************
//* KI_FCNC common block *
//************************

extern "C" {
  void *getaddr(void *);
                                                                                       
  extern struct {
    double gfcncs[2][8];
  } KI_FCNC;
}

struct Kifcnc::_kifcnc *Kifcnc::__kifcnc = 0;

int Kifcnc::nDummy = 0;
double Kifcnc::dDummy = 0.0;
                                                                                
Kifcnc::Kifcnc(void)
{
}
                                                                                
Kifcnc::~Kifcnc(void)
{
}
                                                                                
void Kifcnc::init(void)
{
  __kifcnc = static_cast<struct Kifcnc::_kifcnc *>(getaddr(&KI_FCNC.gfcncs[0][0]));
}

double &Kifcnc::gfcncs(int i, int j)
{
  if (__kifcnc == 0) init();
  if ( ((i<1)||(i>_lenGfcncs)) ||
       ((j<1)||(j>_depthGfcncs)) )
    {
      dDummy = -999.0;
      return dDummy;
    }
  else
    return Kifcnc::__kifcnc->gfcncs[i-1][j-1];

}



//*************************
//* KI_TXDEC common block *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    int midx[50][4];
  } KI_TXDEC;
}

struct Kitxdec::_kitxdec *Kitxdec::__kitxdec = 0;


int Kitxdec::nDummy = 0;
double Kitxdec::dDummy = 0.0;
                                                                                
Kitxdec::Kitxdec(void)
{
}
                                                                                
Kitxdec::~Kitxdec(void)
{
}
                                                                                
void Kitxdec::init(void)
{
  __kitxdec = static_cast<struct Kitxdec::_kitxdec *>(getaddr(&KI_TXDEC.midx[0][0]));
}

int &Kitxdec::midx(int i, int j)
{
  if (__kitxdec == 0) init();
  if ( ((i<1)||(i>_lenMidx)) ||
       ((j<1)||(j>_depthMidx)) )
    {
      nDummy = -999;
      return nDummy;
    }
  else
    return Kitxdec::__kitxdec->midx[i-1][j-1];
}



//**************************
//* KI_TAUMOD common block *
//**************************


extern "C" {
  void *getaddr(void *);

  extern struct {
    int jakk1;
    int jakk2;
    int mdtau;
  } KI_TAUMOD;
}

struct Kitaumod::_kitaumod *Kitaumod::__kitaumod = 0;

int Kitaumod::nDummy = 0;
double Kitaumod::dDummy = 0.0;
                                                                                
Kitaumod::Kitaumod(void)
{
}
                                                                                
Kitaumod::~Kitaumod(void)
{
}
                                                                                
void Kitaumod::init(void)
{
  __kitaumod = static_cast<struct Kitaumod::_kitaumod *>(getaddr(&KI_TAUMOD.jakk1));
}

int &Kitaumod::jakk1(void)
{
  if (__kitaumod == 0) init();
  return __kitaumod->jakk1;

}

int &Kitaumod::jakk2(void)
{
  if (__kitaumod == 0) init();
  return __kitaumod->jakk2;
}

int &Kitaumod::mdtau(void)
{
  if (__kitaumod == 0) init();
  return __kitaumod->mdtau;
}


