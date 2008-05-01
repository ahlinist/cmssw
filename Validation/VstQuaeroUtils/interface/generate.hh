/***********************
Interface to Fortran routine that uses Pythia to generate events
Bruce Knuteson 2003
***********************/

#ifndef __Quaero_generate__
#define __Quaero_generate__

extern "C" double generatehera_(char* sigFName, char* pythiaFName, 
				double* lumi1, double* lumi2, double* lumi3,
				double* xsec1, double* xsec2, double* xsec3,
				int* nmax,
				int sigFilenameLength, int pythiaFilenameLength); // lumi in pb^-1

extern "C" double generate_(char* sigFName, char* pythiaFName, 
			    double* lumi, double* xsec, char* collider, 
			    int* nmax,
			    int sigFilenameLength, int pythiaFilenameLength, int colliderLength); // lumi in pb^-1


#endif
