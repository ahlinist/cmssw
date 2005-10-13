// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kicwn.cc -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/15
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Héctor Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------


#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/CMKINInterface/interface/Kicwn.h"

#ifndef _WIN32
# define PAWC      pawc_
# define KICWN     kicwn_
#endif


//*************************
//*   PAWC common block   *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    double hpawc[2000000];
  } PAWC;
}

struct Pawc::_pawc * Pawc::__pawc = 0;

int Pawc::nDummy = 0;
double Pawc::dDummy = 0.0;

Pawc::Pawc(void)
{
}

Pawc::~Pawc(void)
{
}

void Pawc::init(void)
{
  __pawc = static_cast<struct  Pawc::_pawc *>(getaddr(&PAWC.hpawc[0]));
}

double &Pawc::hpawc(int n)
{
  if (__pawc == 0) init();
  if ((n<1)||(n>_nwpawc)) {
    dDummy = -999.0;
    return dDummy;
  }
  return __pawc->hpawc[n-1];
}





//*************************
//*   KICWN common block   *
//*************************
                                                                                      
extern "C" {
  void *getaddr(void *);
                                                                                      
  extern struct {
    int idcwn;
  } KICWN;
}
                                                                                      
struct Kicwn::_kicwn * Kicwn::__kicwn = 0;
                                                                                      
int Kicwn::nDummy = 0;
double Kicwn::dDummy = 0.0;
                                                                                      
Kicwn::Kicwn(void)
{
}
                                                                                      
Kicwn::~Kicwn(void)
{
}
                                                                                      
void Kicwn::init(void)
{
  __kicwn = static_cast<struct  Kicwn::_kicwn *>(getaddr(&KICWN.idcwn));
}

int &Kicwn::idcwn(void)
{
  if (__kicwn == 0) init();
  return __kicwn->idcwn; 
}

  
                                                                                


