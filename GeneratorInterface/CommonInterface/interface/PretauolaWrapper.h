//////////////////////////////////////////////////////////////////////////
// Sergey.Slabospitsky@cern.ch, July 2006
// Wrapper for FORTRAN version of preTAUOLA

// Initialization routine
                                                                               
//--------------------------------------------------------------------------
// 
#define pretauola pretauola_ 
    extern "C" {
        void pretauola(int*);
        void call_pretauola( int mode ){ pretauola( &mode ); }

      }


