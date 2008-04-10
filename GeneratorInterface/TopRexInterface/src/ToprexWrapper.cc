#include "GeneratorInterface/TopRexInterface/interface/ToprexWrapper.h"
#include <string.h>
//////////////////////////////////////////////////////////////////////////
// Sergey.Slabospitsky@cern.ch, July 2006
// Wrapper for FORTRAN version of TopReX

// Initialization routine
                                                                               
//--------------------------------------------------------------------------
// TopReX routine declaration

#define toprex toprex_ 

// define methods to hide the subtle syntax necessary to call fortran from C++
void call_toprex( const char* frame, const char* beam, const char* target,
		  double win ) 
{ toprex( frame,beam,target, &win,strlen(frame),strlen(beam),strlen(target) );}


