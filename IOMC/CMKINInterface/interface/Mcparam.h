// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Mcparam.h -> CMKIN++ specification of data cards variables 
// -----------------------------------------------------------------------------
//  Date: 2004/11/15
//  Revision: 0.002 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikeys.inc by Lucas Taylor 19/12/95)
// -----------------------------------------------------------------------------

#ifndef Mcparam_h
#define Mcparam_h

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

#define _nmxmcp      200     //
#define _nmched      7       // 
#define _nmctot      207     // _nmctot=_nmxmcp+_nmched




// **********************************
// *     Montecarlo parameters      *
// **********************************
//

class DLLLINKAGE Mcparam
{
 public:
  Mcparam(void);
  ~Mcparam(void);
  int &ivercms(void);
  int &irnmcp(void);
  int &ievmcp(void);
  double &wgtmcp(void);
  double &xsecn(void);
  int &ifilter(void);
  int &nvrmcp(void);
  double &varmcp(int n);
    
 private:
  void init(void);
  static int nDummy;
  static double dDummy;

  // static const int _nmxmcp = 200;
  
  // map on block MCPARAM
  struct _mcparam {
    int ivercms;
    int irnmcp;
    int ievmcp;
    double wgtmcp;
    double xsecn;
    int ifilter;
    int nvrmcp;
    double varmcp[_nmxmcp];
  };
  static struct _mcparam *__mcparam;

};

#endif
