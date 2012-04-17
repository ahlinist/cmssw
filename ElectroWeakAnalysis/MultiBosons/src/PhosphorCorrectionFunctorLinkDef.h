/// This file defines the ROOT dictionaries for the PHOSPHOR correction
/// See also other *LinkDef.h files.

#include "ElectroWeakAnalysis/MultiBosons/interface/PhosphorCorrectionFunctor.h"

#ifdef __CINT__

//never even gets here...
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all typedefs;
//#pragma GCC diagnostic ignored "-Wformat"
// #pragma GCC diagnostic warning "-Wwrite-strings"

#pragma link C++ namespace vgamma;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ class vgamma::PhosphorCorrectionFunctor+;
#pragma link C++ enum vgamma::PhosphorCorrectionFunctor::Mode;

#endif
