// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Hwevnt.cc -> CMKIN++ HWEVNT common block interface 
// -----------------------------------------------------------------------------
//  Date: 2004/11/23
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------


#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/CMKINInterface/interface/Hwevnt.h"

#ifndef _WIN32
# define HWEVNT hwevnt_
#endif


//***********************
//* HWEVNT common block *
//***********************

extern "C" {
  void *getaddr(void *);

  extern struct {
    double avwgt;
    double evwgt;
    double gamwt;
    double tlout;
    double wbigst;
    double wgtmax;
    double wgtsum;
    double wsqsum;
    int idhw[4000];
    int ierror;
    int istat;
    int lwevt;
    int maxer;
    int maxpr;
    int nowgt;
    int nrn[2];
    int numer;
    int numeru;
    int nwgts;
    int gensof;
  } HWEVNT;
}

struct Hwevnt::_hwevnt * Hwevnt::__hwevnt = 0;

int Hwevnt::nDummy = 0;
double Hwevnt::dDummy = 0.0;

Hwevnt::Hwevnt(void)
{
}

Hwevnt::~Hwevnt(void)
{
}

void Hwevnt::init(void)
{
  __hwevnt = static_cast<struct Hwevnt::_hwevnt *>(getaddr(&HWEVNT.avwgt));
}

double &Hwevnt::avwgt(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->avwgt;
}

double &Hwevnt::evwgt(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->evwgt;
}

double &Hwevnt::gamwt(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->gamwt;
}

double &Hwevnt::tlout(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->tlout;
}

double &Hwevnt::wbigst(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->wbigst;
}

double &Hwevnt::wgtmax(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->wgtmax;
}

double &Hwevnt::wgtsum(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->wgtsum;
}

double &Hwevnt::wsqsum(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->wsqsum;
}


int &Hwevnt::idhw(int n)
{
  if (__hwevnt == 0) init();
  if ((n<1)||(n>_lenidhw)) {
    nDummy = -999;
    return nDummy;
  }
  return Hwevnt::__hwevnt->idhw[n-1];
}

int &Hwevnt::ierror(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->ierror;
}

int &Hwevnt::istat(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->istat;
}

int &Hwevnt::lwevt(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->lwevt;
}

int &Hwevnt::maxer(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->maxer;
}

int &Hwevnt::maxpr(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->maxpr;
}

int &Hwevnt::nowgt(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->nowgt;
}

int &Hwevnt::nrn(int n)
{
  if (__hwevnt == 0) init();
  if ((n<1)||(n>_lennrn)) {
    nDummy = -999;
    return nDummy;
  }
  return __hwevnt->nrn[n-1];
}

int &Hwevnt::numer(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->numer;
}

int &Hwevnt::numeru(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->numeru;
}

int &Hwevnt::nwgts(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->nwgts;
}

int &Hwevnt::gensof(void)
{
  if (__hwevnt == 0) init();
  return __hwevnt->gensof;
}
