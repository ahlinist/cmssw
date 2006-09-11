//////////////////////////////////////////////////////////////////////////
// Sergey.Slabospitsky@cern.ch, July 2006
// Wrapper for FORTRAN version of PEVMAIN

// Initialization routine
                                                                               
//--------------------------------------------------------------------------
// 
#define pevmain pevmain_
    extern "C" {
        void pevmain();
        void call_pevmain(){ pevmain(); }

      }


