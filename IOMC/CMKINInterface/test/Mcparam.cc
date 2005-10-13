// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Mcparam.cc -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/15
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Héctor Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------


#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/CMKINInterface/interface/Mcparam.h"

#ifndef _WIN32
# define MC_PARAM     mc_param_
#endif


//*************************
//* MC_PARAM common block *
//*************************

extern "C" {
  void *getaddr(void *);

  extern struct {
    int ivercms;
    int irnmcp;
    int ievmcp;
    double wgtmcp;
    double xsecn;
    int ifilter;
    int nvrmcp;
    double varmcp[200];
  } MC_PARAM;
}

struct Mcparam::_mcparam * Mcparam::__mcparam = 0;

int Mcparam::nDummy = 0;
double Mcparam::dDummy = 0.0;

Mcparam::Mcparam(void)
{
}

Mcparam::~Mcparam(void)
{
}

void Mcparam::init(void)
{
  __mcparam = static_cast<struct  Mcparam::_mcparam *>(getaddr(&MC_PARAM.ivercms));
}


int &Mcparam::ivercms(void)
{
  if (__mcparam == 0) init();
  return __mcparam->ivercms; 
}

int &Mcparam::irnmcp(void)
{
  if (__mcparam == 0) init();
  return __mcparam->irnmcp;
}

int &Mcparam::ievmcp(void)
{
  if (__mcparam == 0) init();
  return __mcparam->ievmcp;
}

double &Mcparam::wgtmcp(void)
{
  if (__mcparam == 0) init();
  return __mcparam->wgtmcp;
}

double &Mcparam::xsecn(void)
{
  if (__mcparam == 0) init();
  return __mcparam->xsecn;
}

int &Mcparam::ifilter(void)
{
  if (__mcparam == 0) init();
  return __mcparam->ifilter;
}

int &Mcparam::nvrmcp(void)
{
  if (__mcparam == 0) init();
  return __mcparam->nvrmcp;
}
      
double &Mcparam::varmcp(int n)
{
  if (__mcparam == 0) init();
  if ((n<1)||(n>_nmxmcp)) {
    dDummy = -999.0;
    return dDummy;
  }
  return __mcparam->varmcp[n-1];
}
  
                                                                                


