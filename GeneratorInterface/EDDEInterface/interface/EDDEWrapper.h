#ifndef EDDESource_EDDEWrapper_h
#define EDDESource_EDDEWrapper_h

#include <ctype.h>
#include <cstring>

// EDDE routines declaration
#define call_eddeini eddeini_ 
#define call_eddeeve eddeeve_
#define call_eddesw  eddesw_
extern "C" {
  void eddeini_(void);
  void eddeeve_(void);
  void eddesw_(void);
}

// EDDE common block declaration
const int mxglpar = 200;
//#ifdef _cplusplus
extern "C" {
//#endif
//  extern  struct{ // in case of first declaration in FORTRAN
  struct {
    int ipars[mxglpar];
    float rpars[mxglpar];
  } ki_txpar_;
//#ifdef _cplusplus
}
#define KI_TXPAR ki_txpar_
///#endif
#endif
