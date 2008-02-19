#ifndef GeneratorInterface_CommonInterface_TauolaWrapper_h
#define GeneratorInterface_CommonInterface_TauolaWrapper_h

/********** TauolaWrapper
 *
 * Wrapper to Fortran functions in TAUOLA tau decay library
 *
 * Christian Veelken
 *  04/17/07
 *
 ***************************************/

//
//-------------------------------------------------------------------------------
//

// main function of TAUOLA tau decay library

extern "C" {
  void tauola_(int*, int*);
}
#define tauola tauola_ 

void inline call_tauola (int mode, int polarization) { tauola(&mode, &polarization); }

//
//-------------------------------------------------------------------------------
//

// common block with steering parameters for CMS specific Fortran interface to TAUOLA

extern "C" {
  extern struct {
    int pjak1;
    int pjak2;
    int mdtau;
  } ki_taumod_;
}
#define ki_taumod ki_taumod_

extern "C" {
   extern struct {
     int jak1;
     int jak2;
     int itdkrc; 
     int ifphot; 
     int ifhadm; 
     int ifhadp; 
   } libra_ ;
} 
#define libra libra_

//
//-------------------------------------------------------------------------------
//

// auxiliary function defined in CMS specific Fortran interface to TAUOLA

extern "C" {
  void tauola_srs_(int*, int*);
}
#define tauola_srs tauola_srs_ 

void inline call_tauola_srs (int mode, int polarization) { tauola_srs(&mode, &polarization); }

extern "C" {
  void taurep_(int*);
}
#define taurep taurep_

void inline call_taurep (int mode) { taurep(&mode); }

extern "C" {
  void pyhepc_t_(int*);
}
#define pyhepc_t pyhepc_t_

void inline call_pyhepc_t (int direction) { pyhepc_t(&direction); }

//
//-------------------------------------------------------------------------------
//

// auxiliary functions defined in PYTHIA

extern "C" {
  int ihepdim_(int*);
}
#define ihepdim ihepdim_

int inline call_ihepdim(int dummy) { return ihepdim(&dummy); }

extern "C" {
  int pycomp_(int*);
}
#define pycomp pycomp_

int inline call_pycomp(int pdgCode) { return pycomp(&pdgCode); }

extern "C" {
  double pyr_(int*);
}
#define pyr pyr_

double inline call_pyr(int dummy) { return pyr(&dummy); }

extern "C" {
  void pygive_(const char*, int);
}
#define pygive pygive_

void inline call_pygive(const char* parameter_ptr, int parameter_len) { pygive(parameter_ptr, parameter_len); }

extern "C" {
  void pydecy_(int*);
}
#define pydecy pydecy_

void inline call_pydecy(int iParticle) { pydecy(&iParticle); }

//
//-------------------------------------------------------------------------------
//

#endif
