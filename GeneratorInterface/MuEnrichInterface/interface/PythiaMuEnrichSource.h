#ifndef PYTHIAMUENRICHSOURCE_h
#define PYTHIAMUENRICHSOURCE_h

/** \class 
 *
 * Generates Pythia enriched Muon sample ==> HepMC events
 *
 * Hector Naves                                  
 *   for the Generator Interface. 26/10/05 
 * Patrick Janot
 *   read all possible cards for Pythia Setup. 26/02/06
 *   ( port from FAMOS )
 ***************************************/

#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <map>
#include <string>
#include "CLHEP/HepMC/GenEvent.h"



/* maximum number of muons to generate: */
#define MAXMUON 10

/* flavour code for muon: */
#define IDMU 13

/* muon mass */
#define MASSMU 0.1057
#define MASSMU2 MASSMU*MASSMU

/* size of cylinder (allowed volume for vertices) */
#define RADIUS 2950
#define LENGTH 5600
/* kinematic range of muons to be acccepted */
/* number of times to decay any hadron in order to smooth probabilities */
#define NDECAY 50

#define N pyjets.n
#define NPAD pyjets.npad
#define K(a,b) pyjets.k[b-1][a-1]
#define P(a,b) pyjets.p[b-1][a-1]
#define V(a,b) pyjets.v[b-1][a-1]

 struct t_decayed_hadron
{
  int index;
  double prob[MAXMUON];
  struct t_decay *decay;
  struct t_decayed_hadron *next;
};
typedef struct t_decayed_hadron t_decayed_hadron;

struct t_decay
{
  double prob[MAXMUON];
  int first,last;
  t_decayed_hadron *daughter;
  struct t_decay *next;
};
typedef struct t_decay t_decay;


//
// class declaration
//

namespace edm
{
  class PythiaMuEnrichSource : public GeneratedInputSource {
  public:
    PythiaMuEnrichSource(const ParameterSet &, const InputSourceDescription &);
    virtual ~PythiaMuEnrichSource();

  private:
    // ----------member data ---------------------------
      
    bool call_pygive(const std::string& iParm );
    bool call_txgive(const std::string& iParm );
    bool call_txgive_init();
  private:
    virtual bool produce(Event & e);
    void clear();
	  
    HepMC::GenEvent  *evt;
	  
    //  void Nest( HepMC::GenEvent & evt ) ;      //HepMC::GenEvent&) ;
	  
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
	  
    double maxeta,comenergy;   
    int wtype, nmu_min;
    double wbc, wmin,wprintmin, lmu_ptmin;
    int nevnt;
    double sumwg;
    double sumwg2;
    int firstRun;
    int nbc,nbcsim;
    bool firstevent;

    
    double ptminmu, etaminmu, etamaxmu;
    /*      these particles can decay into mu + anything:
	
    tau,rho0,K_L0,pi+,eta, K+, D+, D0,D_s+,J/psi,
    B0,B+,B_s0,B_c+,Y,Lambda_c+,Xi_c0,Xi_c+,Omega_c0,
    Lambda_b0,Xi_b-,Xi_b0,Omega_b-,psi',Y'
    */

    int decay_mode_list[2000];
    int n_decay_mode;
    int first_muon[500];
    int last_muon[500];
    int last_nonmuon[500];
    int last_decay[500];
    double brat_muon[500];
    double brat_nonmuon[500];
    double par_a[500];
    double par_b[500];
    double par_m[500];
    double ptmin2[500];
    int ngenbfe,ngenbpf,ngencfe,ngencpf,ngenbgs,ngencgs;
    int nselbfe,nselbpf,nselcfe,nselcpf,nselbgs,nselcgs;
    int nrejbc,ptype;
    int nhadrons,ndecays;
    t_decayed_hadron hadrons[4000];
    t_decay decays[2000];

    int initdc(double *pt,double *eta);
    void pydecy_all(int index);
    void undecay(t_decay *decay);
    void prob_nmuon(t_decayed_hadron *hadron, int nmuon, double *pn);
    void pyexec_nomu();
    void choose_nomuon(t_decayed_hadron *hadron);
    void default_decay_modes();
    void choose_decay(t_decayed_hadron *hadron, unsigned int nmuon);
    double wgtmu(int nmu, float minweight);
    int muon_cut(int index);
    int count_muons(int low, int high);
    int count_all_muons(int low, int high);
    int pydecy_mu(int index,int ndecay);
    int set_muon_decay(int mode, int kc);
    double choose_decay_vertex(int index,int kc);
    void zero_hepevnt();
    void switch_off_decays();
    void countbc(int sel);
    t_decay* loop_hadrons(int first, int last);
    t_decayed_hadron* decay(int);
    bool isbc();
  };
}
#endif
