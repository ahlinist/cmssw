#include "GeneratorInterface/ComphepInterface/interface/ComphepWrapper.h"
//////////////////////////////////////////////////////////////////////////
// Sergey.Slabospitsky@cern.ch, July 2006
// Wrapper for FORTRAN version of PEVMAIN

// Initialization routine
                                                                               
//--------------------------------------------------------------------------
// 
    extern "C" {
        void pevmain();
        void call_pevmain(){ pevmain_(); }
      }
