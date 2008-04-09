#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/JobReport.h"

#include <stdio.h>
#include <math.h>
#include "time.h"
#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "GeneratorInterface/MuEnrichInterface/interface/Decay_Structs.h"
#include "GeneratorInterface/MuEnrichInterface/interface/PythiaMuEnrichProducer.h"
#include "GeneratorInterface/MuEnrichInterface/interface/PYR.h"
 #define PYGIVE pygive_
 extern "C" {
   void PYGIVE(const char*,int length);
 }
 #define pyexec pyexec_
 extern "C" {
   void pyexec();
 }
 #define pycomp pycomp_
 extern "C" {
   int pycomp(int *kf);
 }
 #define pyedit pyedit_
 extern "C" {
   void pyedit(int *kf);
 }
 #define pydecy pydecy_
 extern "C" {
   void pydecy(int *index);
 }
#ifdef NEVER
 #define pyr pyr_
 extern "C" {
   double pyr_();
 }
#endif
 #define TXGIVE txgive_
 extern "C" {
   void TXGIVE(const char*,int length);
 }

 #define TXGIVE_INIT txgive_init_
 extern "C" {
   void TXGIVE_INIT();
 }

 int muon_parents[] = {  15,   113,   130,   211,    221,    321,  411,  421,  431, 443,
		        511,   521,   531,   541,    553,   4122, 4132, 4232, 4332,
		       5122,  5132,  5232,  5332, 100443, 100553,    0};

 HepMC::IO_HEPEVT conv;


edm::PythiaMuEnrichProducer::PythiaMuEnrichProducer(const ParameterSet & pset) :
   EDProducer(), evt(0), 
   pythiaPylistVerbosity_ (pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0)),
   pythiaHepMCVerbosity_ (pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false)),
   maxEventsToPrint_ (pset.getUntrackedParameter<int>("maxEventsToPrint",1)),
   maxeta(pset.getUntrackedParameter<double>("maxeta", 2.4)),
   comenergy(pset.getUntrackedParameter<double>("comEnergy",14000.)),
   wtype(pset.getUntrackedParameter<int>("wtype", 1)),
   nmu_min(pset.getUntrackedParameter<int>("nmu_min", 1)),
   wbc(pset.getUntrackedParameter<double>("wbc", 1.)),
   wmin(pset.getUntrackedParameter<double>("wmin", 0.1)),
   wprintmin(pset.getUntrackedParameter<double>("wprintmin", 0.5)),
   lmu_ptmin(pset.getUntrackedParameter<double>("ptmin", 3.)),
   runNum_(0),
   eventNum_(0),
   numEvents_(0)
 {

   //now do what ever initialization is needed
   HepMC::HEPEVT_Wrapper::set_max_number_entries(4000);
   HepMC::HEPEVT_Wrapper::set_sizeof_real(8);
   nevnt=0;
   sumwg2=0;
   sumwg=0;  
   firstRun=-1;
   LogTrace ("PythiaMuEnrichProducer") << "PyhtiaMuEnrichProducer: initializing Pythia. " << std::endl; 

   // PYLIST Verbosity Level
   // Valid PYLIST arguments are: 1, 2, 3, 5, 7, 11, 12, 13
   pythiaPylistVerbosity_ = pset.getUntrackedParameter<int>("pythiaPylistVerbosity",0);
   LogTrace ("PythiaMuEnrichProducer") << "Pythia PYLIST verbosity level = " << pythiaPylistVerbosity_ << std::endl;

   // HepMC event verbosity Level
   pythiaHepMCVerbosity_ = pset.getUntrackedParameter<bool>("pythiaHepMCVerbosity",false);
   LogTrace ("PythiaMuEnrichProducer") << "Pythia HepMC verbosity = " << pythiaHepMCVerbosity_ << std::endl; 

   //Max number of events printed on verbosity level 
   maxEventsToPrint_ = pset.getUntrackedParameter<int>("maxEventsToPrint",0);
   LogTrace ("PythiaMuEnrichProducer") << "Number of events to be printed = " << maxEventsToPrint_ << std::endl;

   // Set PYTHIA parameters in a single ParameterSet
   ParameterSet pythia_params = 
     pset.getParameter<ParameterSet>("PythiaParameters") ;

   // The parameter sets to be read (default, min bias, user ...) in the
   // proper order.
   std::vector<std::string> setNames = pythia_params.getParameter<std::vector<std::string> >("parameterSets");

   // Loop over the sets
   for ( unsigned i=0; i<setNames.size(); ++i ) {

     std::string  mySet = setNames[i];

     // Read the PYTHIA parameters for each set of parameters
     std::vector<std::string> pars = 
       pythia_params.getParameter<std::vector<std::string> >(mySet);

     if (mySet != "CSAParameters"){
       LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl;
       LogTrace ("PythiaMuEnrichProducer") << "Read PYTHIA parameter set " << mySet << std::endl;
       LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl;

       // Loop over all parameters and stop in case of mistake
       for( std::vector<std::string>::const_iterator  
	      itPar = pars.begin(); itPar != pars.end(); ++itPar ) {
	 static std::string sRandomValueSetting("MRPY(1)");
	 if( 0 == itPar->compare(0,sRandomValueSetting.size(),sRandomValueSetting) ) {
	   throw edm::Exception(edm::errors::Configuration,"PythiaError")
	     <<" attempted to set random number using pythia command 'MRPY(1)' this is not allowed.\n  Please use the RandomNumberGeneratorService to set the random number seed.";
	 }
	 if( ! call_pygive(*itPar) ) {
	   throw edm::Exception(edm::errors::Configuration,"PythiaError") 
	     <<" pythia did not accept the following \""<<*itPar<<"\"";
	 }
       }
     }else if(mySet == "CSAParameters"){   

       // Read CSA parameter

       pars = pythia_params.getParameter<std::vector<std::string> >("CSAParameters");

       LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl; 
       LogTrace ("PythiaMuEnrichProducer") << "Reading CSA parameter settings. " << std::endl;
       LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl;                                                                          
       call_txgive_init();


       // Loop over all parameters and stop in case of a mistake
       for (std::vector<std::string>::const_iterator 
	      itPar = pars.begin(); itPar != pars.end(); ++itPar) {
	 call_txgive(*itPar); 

       } 

     }
   }

   //pypars.mstp[141]=2; 
   pydat1.mstj[21]=1;
   pydat1.parj[72]=RADIUS;
   pydat1.parj[73]=LENGTH;
   nbc=nbcsim=nrejbc=0; 
   ngenbfe=ngenbpf=ngencfe=ngencpf=ngenbgs=ngencgs=0;
   nselbfe=nselbpf=nselcfe=nselcpf=nselbgs=nselcgs=0;
   firstevent=true;
   initdc(&lmu_ptmin,&maxeta);

   //In the future, we will get the random number seed on each event and tell 
   // pythia to use that new seed
   LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl;
   LogTrace ("PythiaMuEnrichProducer") << "Setting Pythia random number seed " << std::endl;
   LogTrace ("PythiaMuEnrichProducer") << "----------------------------------------------" << std::endl;
   edm::Service<RandomNumberGenerator> rng;
   randomEngine = fRandomEngine = &(rng->getEngine());
   uint32_t seed = rng->mySeed();
   std::ostringstream sRandomSet;
   sRandomSet <<"MRPY(1)="<<seed;
   call_pygive(sRandomSet.str());
   call_pyinit( "CMS", "p", "p", comenergy );
      //********                                      

   produces<HepMCProduct>();
   LogTrace ("PythiaMuEnrichProducer") << "PyhtiaMuEnrichProducer: starting event generation ... " << std::endl;
 }

edm::PythiaMuEnrichProducer::~PythiaMuEnrichProducer()
 {
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)
   LogVerbatim ("PythiaMuEnrichProducer") << "PyhtiaMuEnrichProducer: event generation done.\n" << std::endl;
   call_pystat(1);
   nbcsim=pyint5.ngen[2][0]-nevnt;
   double norm=nbcsim*pyint5.xsec[2][0];
   LogVerbatim ("PythiaMuEnrichProducer") << "Simulated "<<nbcsim<<" events for bc production, about to end";
   LogVerbatim ("PythiaMuEnrichProducer")<<"Production Nb 2 (bc)";
   LogVerbatim ("PythiaMuEnrichProducer")<<"Run Number "<<runNum_;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Total Number of Generated bc Events "<<ngenbfe+ngenbpf+ngenbgs+ngencfe+ngencpf+ngencgs;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated B (FE) evts,Xsec prod 2 :"<<ngenbfe<<" "<<((double)ngenbfe)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated B (PF) evts,Xsec prod 2 :"<<ngenbpf<<" "<<((double)ngenbpf)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated B (GS) evts,Xsec prod 2 :"<<ngenbgs<<" "<<((double)ngenbgs)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated B,    Total Xsec prod 2 :"<<ngenbpf+ngenbgs+ngenbfe<<" "<<((double)(ngenbpf+ngenbgs+ngenbfe))/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated C (FE) evts,Xsec prod 2 :"<<ngencfe<<" "<<((double)ngenbfe)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated C (PF) evts,Xsec prod 2 :"<<ngencpf<<" "<<((double)ngenbpf)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated C (GS) evts,Xsec prod 2 :"<<ngencgs<<" "<<((double)ngenbgs)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Generated C,    Total Xsec prod 2 :"<<ngencpf+ngencgs+ngencfe<<" "<<((double)(ngencpf+ngencgs+ngencfe))/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Total Number of Selected bc Events "<<(nselbfe+nselbpf+nselbgs+nselcfe+nselcpf+nselcgs);
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected B (FE) evts,Xsec prod 2 :"<<nselbfe<<" "<<((double)nselbfe)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected B (PF) evts,Xsec prod 2 :"<<nselbpf<<" "<<((double)nselbpf)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected B (GS) evts,Xsec prod 2 :"<<nselbgs<<" "<<((double)nselbgs)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected B,    Total Xsec prod 2 :"<<nselbpf+nselbgs+nselbfe<<" "<<((double)(nselbpf+nselbgs+nselbfe))/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected C (FE) evts,Xsec prod 2 :"<<nselcfe<<" "<<((double)nselbfe)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected C (PF) evts,Xsec prod 2 :"<<nselcpf<<" "<<((double)nselbpf)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected C (GS) evts,Xsec prod 2 :"<<nselcgs<<" "<<((double)nselbgs)/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected C,    Total Xsec prod 2 :"<<nselcpf+nselcgs+nselcfe<<" "<<((double)(nselcpf+nselcgs+nselcfe))/norm;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Total Selected events           "<<(eventNum_+nselbfe+nselbpf+nselbgs+nselcfe+nselcpf+nselcgs);

   clear();
 }

void edm::PythiaMuEnrichProducer::clear()
 { }

 //
 // member functions
 //

 // ------------ method called to produce the data  ------------
 void edm::PythiaMuEnrichProducer::produce(Event& iEvent, const EventSetup& es)
 {
   std::auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
   LogVerbatim ("PythiaMuEnrichProducer") <<"==============================================================";
   LogTrace ("PythiaMuEnrichProducer") << "PyhtiaMuEnrichProducer: Generating event ...  " << std::endl;
   double wg=0.;
   if (firstevent) {
     firstRun=runNum_;
     firstevent=false;
   }
   bool bcevent=false;
   while ( wg <wmin ) {
     call_pyevnt();      // generate one event with Pythia
     bcevent=isbc();
     if(bcevent) countbc(1);
     if(!bcevent) ++nevnt;
     if( runNum_== firstRun) ++nbcsim;
     if ( runNum_ == firstRun && bcevent ) {
       wg=0.;
       nrejbc++;
     }else if ( runNum_ == firstRun || bcevent ) wg=wgtmu(nmu_min,wmin);
     else wg=0.;
   }    
   
   if (wg >= wmin ) {
     if(bcevent) countbc(2);
     call_pyhepc(1); 
     
     //HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
     HepMC::GenEvent* evt = conv.read_next_event();
     evt->set_signal_process_id(pypars.msti[0]);
     evt->set_event_scale(pypars.pari[16]);
     ++eventNum_;
     evt->set_event_number(eventNum_);
     evt->weights().push_back(wg);
     //******** Verbosity ********
     
     if(iEvent.id().event() <= maxEventsToPrint_ &&
	(pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {
       
       // Prints PYLIST info
       if(pythiaPylistVerbosity_) {
	 call_pylist(pythiaPylistVerbosity_);
       }
       
       // Prints HepMC event
       if(pythiaHepMCVerbosity_) {
	 LogTrace ("PythiaMuEnrichProducer") << "Event process = " << pypars.msti[0] << std::endl 
	      << "----------------------" << std::endl;
	 evt->print();
       }
     }

     if(evt)  {
       bare_product->addHepMCData(evt);
//     std::cerr << "\n\t*** Putting HepMCProduct object " << eventNum_ << " into the Event\n" << std::endl;
     }
     iEvent.put(bare_product);
   }
// Throw an exception on failure.  Use the EventCorruption
// exception since it maps onto SkipEvent which is what we want to do here.

#ifdef NEVER	// I don't understand the logic here.  The following test ALWAYS
		// fails.  Till I make sense of this, don't make the test.
   if ( nbcsim >= nbc && nbcsim > 1 ) {
     std::ostringstream sstr;
     sstr << "PythiaMuEnrichProducer: Generation failed. nbcsim = " << nbcsim 
          << " and nbc = " << nbc << "\n";
     edm::Exception except(edm::errors::EventCorruption, sstr.str());
     throw except;
   }
#endif

   return;
}

void edm::PythiaMuEnrichProducer::endJob()
{
   call_pystat(1);
   nevnt=pyint5.ngen[2][0];
   nbc= (int)(((double)nevnt)/wbc);
   LogVerbatim ("PythiaMuEnrichProducer")<<"Number of events to generate for b-c events: "<< nbc;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Number of rejected events in prod ! because of bc: "<< nrejbc;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Production Nb1 (without bc)";
   LogVerbatim ("PythiaMuEnrichProducer")<<"Run Number "<< firstRun;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Total Number of Generated Events "<< nevnt;
   LogVerbatim ("PythiaMuEnrichProducer")<<"Cross Section (mb)              "<< pyint5.xsec[2][0];
   LogVerbatim ("PythiaMuEnrichProducer")<<"Selected events                 "<< eventNum_;
   double integlumi=pyint5.ngen[2][0]/(1.E+6*pyint5.xsec[2][0]);
   LogVerbatim ("PythiaMuEnrichProducer")<<"Integrated LHC lumi  (nb-1)     "<< integlumi; 
   edm::Service<JobReport> reportSvc;
   std::stringstream stmp,srun;
   srun << firstRun;
   stmp << integlumi/nanobarn;
   reportSvc->reportGeneratorInfo("runX",srun.str());
   reportSvc->reportGeneratorInfo("lumiX",stmp.str());
   //pypars.mstp[81]=1; // for speed of production
}

bool edm::PythiaMuEnrichProducer::call_pygive(const std::string& iParm ) 
{

  int numWarn = pydat1.mstu[26]; //# warnings
  int numErr = pydat1.mstu[22];// # errors

//call the fortran routine pygive with a fortran string
  PYGIVE( iParm.c_str(), iParm.length() );
  //  PYGIVE( iParm );
//if an error or warning happens it is problem
  return pydat1.mstu[26] == numWarn && pydat1.mstu[22] == numErr;

}

bool 
edm::PythiaMuEnrichProducer::call_txgive(const std::string& iParm ) {
  
  TXGIVE( iParm.c_str(), iParm.length() );
  LogTrace ("PythiaMuEnrichProducer") << "     " <<  iParm.c_str() << std::endl; 

  return 1;  
}

bool 
edm::PythiaMuEnrichProducer::call_txgive_init() {
  
  TXGIVE_INIT();
  LogTrace ("PythiaMuEnrichProducer") << "  Setting CSA defaults.   "   << std::endl; 

  return 1;  
}

int edm::PythiaMuEnrichProducer::set_muon_decay (int mode, int kc)
  /* select predefined set of decay modes for particle with compressed code kc
     mode = 0   switch on all default decay modes
     1   switch on only those modes which give a prompt muon
     2   switch on only decay modes without prompt muon
     returned:  1 if selected decay has nonzero branching ratio
     0 otherwise
  */
{
  int i,j,k,last;

  k=0;
  j=(mode!=2);
  last=last_muon[kc];
  for (i=first_muon[kc]; i<=last; ++i)
    {
      pydat3.mdme[0][decay_mode_list[i]-1]=j;
      k|=j;
    }
  last=last_decay[kc];
  j=(mode!=1);
  for (; i<=last; ++i)
    {
      pydat3.mdme[0][decay_mode_list[i]-1]=j;
      k|=j;
    }
  return(k);
}

void edm::PythiaMuEnrichProducer::switch_off_decays()
  /* inhibit decays of muon parents */
{
  int i,a,kc,kf;

  i=0;
  while ((kf=muon_parents[i]))
    {
      a=abs(kf);
      kc=pycomp(&a);
      pydat3.mdcy[0][kc-1]=0;
      ++i;
    }

  pydat1.mstj[21]=1; 
                     
}

int edm::PythiaMuEnrichProducer::initdc(double *minpt, double *etamax)
  /***********************************************************************/
  /*                                                                     */
  /* Initialize n-muon generator                                         */
  /*                                                                     */
  /* The main task of this routine is to scan the JETSET decay table     */
  /* for decay modes of hadrons with muons as daughters and store the    */
  /* information which is necessary to select quickly modes with or      */
  /* without prompt muons                                                */
  /*                                                                     */
  /* Input: *minpt  - minimum pt required for a muon to be accepted      */
  /*        *etamax - maximum pseudorapidity for accepted muons          */
  /*                  (muons are accepted in the range -etamax .. etamax)*/
  /*                                                                     */
  /***********************************************************************/
{
  int i,j,l,first,last,a;
  int kf,kc,kcd,mu;
  int idecay;
  double mass,ptmin,pt2minmu;

  for (i=0; i<500; ++i)
    first_muon[i]=last_muon[i]=last_nonmuon[i]=last_decay[i]=-1;

  i=0;
  idecay=0;
  kf=muon_parents[i];
  while (kf)
    {
      a=abs(kf);
      kc=pycomp(&a);
      first_muon[kc]=idecay;
      brat_muon[kc]=0.;
      brat_nonmuon[kc]=0.;
      kcd=kc;
      first=pydat3.mdcy[1][kcd-1];
      if (pydat3.mdme[1][first-1]>=84 && pydat3.mdme[1][first-1]<=88)
	{
	  kcd=pydat3.mdme[1][first-1];            // generic heavy quark decay
	  first=pydat3.mdcy[1][kcd-1];
	}
      last=pydat3.mdcy[1][kcd-1]+pydat3.mdcy[2][kcd-1];
      for (j=first; j<last; ++j)            // loop over decay modes
	{
	  mu=0;
	  for (l=1; l<=5; ++l)            // loop over decay products
	    if (abs(pydat3.kfdp[l-1][j-1])==IDMU ||      // and count muons
		(abs(pydat3.kfdp[l-1][j-1])==443 &&      //  include also J/psi to muon channels
		 a!=30443)) ++mu;            //  (but not for psi')
	  if (mu)                        // find decay into muons
	    {
	      if (pydat3.mdme[0][j-1]==1)            // decay switched on?
		{
		  decay_mode_list[idecay++]=j;  // store decay mode index
		  brat_muon[kc]+=pydat3.brat[j-1];      // sum branching ratio
		}
	    }
	}
      last_muon[kc]=idecay-1;
      for (j=first; j<last; ++j)            // loop again over decay modes
	{
	  mu=0;
	  for (l=1; l<=5; ++l)            // loop over decay products
	    if (abs(pydat3.kfdp[l-1][j-1])==IDMU) ++mu;      // and count muons
	  if (!mu)                        // find decay without muons
	    {
	      if (pydat3.mdme[0][j-1]==1)            // decay switched on?
		{
		  decay_mode_list[idecay++]=j;  // store decay mode index
		  brat_nonmuon[kc]+=pydat3.brat[j-1];      // sum branching ratio
		}
	    }
	}
      last_decay[kc]=last_nonmuon[kc]=idecay-1;

      /* get pt and eta range for muons to select */
      ptminmu=*minpt;
      pt2minmu=ptminmu*ptminmu;
      etaminmu=-*etamax;
      etamaxmu=*etamax;

      /* store some parameters of 2-body decay to allow quick estimate whether
	 a muon coming out of the decay of the particle can possibly have a
	 large transverse momentum
      */
      mass=pydat2.pmas[0][kc-1];
      par_a[kc]=0.5*(1.+MASSMU2/(mass*mass));
      par_b[kc]=1.-par_a[kc];
      par_m[kc]=mass;
      ptmin=ptminmu*(-par_b[kc]*mass*mass/
		     (pt2minmu*(sqrt(MASSMU2/pt2minmu+1.)+1.)) +1.);
      LogTrace ("PythiaMuEnrichProducer") <<"kf="<<kf<<" mass="<<mass<<", ptmin="<<ptmin; 
      if (ptmin>0.) ptmin2[kc]=ptmin*ptmin;
      else ptmin2[kc]=0.;

      kf=muon_parents[++i];            // get next hadron
    }

  /* switch off neutral pion decay */
  a=111;
  pydat3.mdcy[0][pycomp(&a)-1]=0;

  switch_off_decays();

  return idecay;
}

void edm::PythiaMuEnrichProducer::prob_nmuon(t_decayed_hadron *hadron, int nmuon, double *pn)
  /***********************************************************************/
  /*                                                                     */
  /*  for a linked list of hadrons, calculate the probabilities to get   */
  /*  more than n muons from their decays                                */
  /*                                                                     */
  /*  input: hadron - first hadron in the list, hadron->next is expected */
  /*                  to point to the next one, the last has next=0      */
  /*                  hadron->prob[n] is the probability to get >n muons */
  /*                  from this hadron                                   */
  /*         nmuon  - maximum number of muons to calculate               */
  /*  output:  pn[] - array of probabilities                             */
  /*                  p[0] is the probability to get >0 muons            */
  /*                  p[1]  .  .      .       .   .  >1 muons            */
  /*                   ..   .  .      .       .   .  ..  ....            */
  /*                  p[nmuon] .      .       .   .  >nmuon muons        */
  /*                                                                     */
  /***********************************************************************/
{
  int i,j;
  t_decayed_hadron *h;

  for (i=0; i<=nmuon; ++i) pn[i]=0;

  for (h=hadron; h!=0; h=h->next)
    for (i=nmuon; i>=0; --i)
      {
	for (j=i-1; j>=0; --j)
	  pn[i]+=h->prob[i-j-1]*(pn[j]-pn[j+1]);
	pn[i]+=h->prob[i]*((double)1-pn[0]);
      }
}


t_decay *edm::PythiaMuEnrichProducer::loop_hadrons(int first,int last)
  /***********************************************************************/
  /*                                                                     */
  /* loop over a list of hadrons, have them decay, and calculate         */
  /* the combined probability to obtain >0 or >1 muons                   */
  /*                                                                     */
  /***********************************************************************/
{
  int i,n;
  //int mu;
  double pn[MAXMUON];
  t_decayed_hadron *h1;
  t_decayed_hadron *previous;
  //  t_decay d;
  t_decay *next;
  t_decayed_hadron *daughter;
  next=0;
  daughter=0;
  n=N;
  previous=0;
  //LogTrace ("PythiaMuEnrichProducer") << "in Loop_hadrons first="<<first<<" last="<<last;
  for (i=first; i<=last; ++i)
    {
      if (K(i,1)>0 && K(i,1)<=10)            // undecayed particle
	{	  
	  
	  h1=decay(i);
	  if (h1!=0)
	    {
	      pn[0]=h1->prob[0];
	      if (pn[0]>0)                  // any chance to get a muon?
		{
		  if (previous!=0) previous->next=h1;
		  else daughter=h1;
		  previous=h1;
		}
	    }
	}
    }
  if (daughter!=0) prob_nmuon(daughter,MAXMUON-1,pn);
  else return 0;

  if (pn[0]>0)
    {
      for (i=0; i<MAXMUON; ++i) decays[ndecays].prob[i]=pn[i];
      decays[ndecays].first=first;
      decays[ndecays].last=N;
      decays[ndecays].daughter=daughter;
      decays[ndecays].next=next;
      return (&(decays[ndecays++]));
    }
  else
    return 0;
}


int edm::PythiaMuEnrichProducer::muon_cut(int index)
  /* test whether the muon at position index in the event record
     satisfies the selection criteria
     returns 1 if muon accepted, 0 otherwise
  */
{
  double px,py,pz,pt,pt2,eta,p;
  int iselp,iselm;
  iselp=wtype;
  iselm=nmu_min;
  px=P(index,1);
  py=P(index,2);
  pz=P(index,3);
  pt2=px*px+py*py;
  p=sqrt((double)(pt2+pz*pz));
  pt=sqrt((double)(pt2));

  eta=log((double)((p+fabs((double)pz))/pt))*((pz>0)-(pz<0));

  if(iselp == 1 && iselm==1)

    //  special for single mu, pt1 (wtype=1) production (U.G.) :
    {
        
      if( pt > 4.0 ) return 0;
      if (eta<etaminmu || eta>etamaxmu) return 0;
      if (eta>etaminmu && eta<-1.7 && p<3.5) return 0;
      if (eta>-1.7 && eta<-1.2 && pt<1.8) return 0;
      if (eta>-1.2 && eta< 1.2 && pt<3.0) return 0;
      if (eta> 1.2 && eta< 1.7 && pt<1.8) return 0;
      if (eta> 1.7 && eta<etamaxmu && p<3.5) return 0;
    }

  //  for all other productions :
  else
    {
      if (eta<etaminmu || eta>etamaxmu || pt<ptminmu) return 0;
    } 
  return 1;
}


int edm::PythiaMuEnrichProducer::count_muons(int low, int high)
  /***********************************************************************/
  /* count the number of muons in the event record at index low .. high  */
  /***********************************************************************/
{
  int i,n;

  n=0;
  for (i=low; i<=high; ++i)
    if (abs(K(i,2))==IDMU && K(i,1)>0 && K(i,1)<=10)
      if (muon_cut(i)) ++n;
  return n;
}
int edm::PythiaMuEnrichProducer::count_all_muons(int low, int high)
  /***********************************************************************/
  /* count the number of muons in the event record at index low .. high  */
  /***********************************************************************/
{
  int i,n;

  n=0;
  for (i=low; i<=high; ++i)
    if (abs(K(i,2))==IDMU)
      if (muon_cut(i)) ++n;
  return n;
}



double edm::PythiaMuEnrichProducer::choose_decay_vertex(int index,int kc)
  /***********************************************************************/
  /* choose a decay vertex for long living particles which is inside the */
  /* detector, and return the decay probability                          */
  /***********************************************************************/
{
  int dum;
  int i;
  double prob;

  double lifetime,mass;
  double r,x0,y0;
  double px,py,pz,pt,p;
  double length,radius,v;

  lifetime=pydat2.pmas[3][kc-1];
  r=pyr_(&dum);
  
  /* get momentum and determine length of track before intersection with
     the bounding cylinder */
  px=P(index,1);
  py=P(index,2);
  pz=P(index,3);
  pt=px*px+py*py;
  p=sqrt((double)(pt+pz*pz));
  pt=sqrt((double)(pt));

  length=LENGTH-V(index,3)*((pz>0)-(pz<0));
  v=V(index,1)*V(index,1)+V(index,2)*V(index,2);
  if (v>RADIUS*RADIUS || length<0)      // particle produced outside detector?
    return 0.;
  if (pt==0) radius=RADIUS;
  else
    {
      radius=V(index,1)*P(index,1)/pt;
      radius+=sqrt((double)radius*radius+RADIUS*RADIUS-v);
    }
  pz=fabs((double)pz);
  if (pt*length>pz*radius) x0=radius*p/pt;
  else x0=length*p/pz;

  mass=P(index,5);
  y0=exp(-mass*x0/(lifetime*p));

  prob=1.-y0;

  if (prob<=0)
    {
      if (prob<0) LogTrace ("PythiaMuEnrichProducer") << "choose_decay_vertex: prob<0!";
      return 0;
    }

  lifetime*=(-log((double)(y0*(1-r)+r)));      // choose random proper lifetime

  for (i=1; i<=4; ++i) V(index,i)+=lifetime*P(index,i)/mass;

  K(index,1)=5;            // force decay at a given vertex
  
  return prob;
  
}


void edm::PythiaMuEnrichProducer::pydecy_all(int index)
  /* decay a particle by calling PYDECY from JETSET, and decay also all its
     unstable decay products recursively */
{
  int i,n,kf,last;

  n=N+1;
  pydecy(&index);
  last=N;

  for (i=n; i<=last; ++i)
    {
      if (K(i,1)>0 && K(i,1)<=10)
	{
	  kf=abs(K(i,2));
	  if ( K(i,1) != 1 ) LogTrace ("PythiaMuEnrichProducer") << " in loop pydecy_all i="<<i<<" kf="<<kf<<" status="<<K(i,1)<<" daughters="<<K(i,4)<<"-"<<K(i,5);
	  if (pydat3.mdcy[0][pycomp(&kf)-1]) pydecy_all(i);
	}
    }
}


void edm::PythiaMuEnrichProducer::undecay(t_decay *decay)
  /***********************************************************************/
  /* delete a decay and all decay products                               */
  /***********************************************************************/
{
  int i,last;
  last=decay->last;
  for (i=decay->first; i<=last; ++i) K(i,1)=0; 
}


void edm::PythiaMuEnrichProducer::pyexec_nomu()
  /***********************************************************************/
  /*                                                                     */
  /* decay all undecayed particles in the event record, but avoid muons  */
  /* in the acceptance region as decay products                          */
  /*                                                                     */
  /***********************************************************************/
{
#define MAXTRYNOMU 50
  int i,kf,last,n,tried;

  last=N;

  for (i=1; i<=last; ++i)
    {
      kf=abs(K(i,2));
      if (K(i,1)==1)
	{
	  if (pydat3.mdcy[0][pycomp(&kf)-1])
	    {
	      n=N+1;
	      pydecy_all(i);                        // decay
	      tried=0;
	      while (count_muons(n,N))
		{
		  if (++tried>=MAXTRYNOMU)
		    {
		      LogTrace ("edm::PythiaMuEnrichProducer") << "error in pyexec_nomu: unable to get decay without muons after "<<tried<<" attempts";
		      N=n-1;
		      K(i,1)=1;
		      break;
		    }
		  N=n-1;
		  K(i,1)=1;
		  pydecy_all(i);
		}
	    } 
	}
    }
}

void edm::PythiaMuEnrichProducer::choose_nomuon(t_decayed_hadron *hadron)
  /***********************************************************************/
  /*                                                                     */
  /* delete decay products of a hadron                                   */
  /*                                                                     */
  /***********************************************************************/
{
  t_decay *d;
  for (d=hadron->decay; d!=0; d=d->next)      // delete decay products
    undecay(d);
  K(hadron->index,1)=1;                        // declare undecayed
}


int edm::PythiaMuEnrichProducer::pydecy_mu(int index,int ndecay)
  /* decay a particle ndecay times and count the number of times
     an accepted muon was the direct daughter.
     Keep the first successful decay (if any), and return the
     number of successes
  */
{
  int i,j,found,k,kd,kf,n,nsuccess;

  n=N+1;
  
  nsuccess=0;
  k=kd=K(index,1);            // remember status code of undecayed particle
  for (i=0; i<ndecay; ++i)
    {
      pydecy(&index);                  // decay the particle
      found=0;
      for (j=n; j<=N; ++j)            // loop over decay products
	if (abs(K(j,2))==IDMU)            // find muons
	  if (muon_cut(j))
	    {
	      found++;
	      break;
	    }
      if (found)                        
	{
	  ++nsuccess;                  // in case of success,
	  for (j=n; j<=N; ++j)            // make sure all daughters are decayed
	    if (K(j,1)>0 && K(j,1)<=10)
	      {
		kf=abs(K(j,2));
		if (pydat3.mdcy[0][pycomp(&kf)-1]) pydecy_all(j);
	      }
	  n=N+1;                        // keep decay products
	  kd=K(index,1);                  // remember decayed status code
	  break;
	}
      else                        // no muon found
	{
	  K(index,1)=k;                  // forget about decay
	  N=n-1;
	}
    }

  for (++i; i<ndecay; ++i)            // continue the loop
    {
      pydecy(&index);                    // decay the particle again
      found=0;
      for (j=n; j<=N; ++j)                // loop over decay products
	if (abs(K(j,2))==IDMU)              // find muons
	  if (muon_cut(j))
	    {
	      found++;
	      break;
	    }
      if (found)                          
	++nsuccess;                       // count successes,
      K(index,1)=k;                  // but forget about the decay anyway
      N=n-1;
    }

  K(index,1)=kd;                  // update status code (possibly decayed)

  return nsuccess;
}


t_decayed_hadron *edm::PythiaMuEnrichProducer::decay (int index)
  /***********************************************************************/
  /* decay the hadron at the given index in the event record, and        */
  /* store all relevant decay modes (relevant are those which can        */
  /* eventually lead to the production of a muon in the accepted region) */
  /***********************************************************************/
{
  int kf,kc;
  int i,k,kd,a,n;
  t_decay *d1=0;
  t_decay *d2=0;
  double v[6];
  double pv;
  double px,py,pt2;
  double prob[MAXMUON];

  kf=K(index,2);

  if (abs(kf)==IDMU)            // muon
    {
      if (muon_cut(index))
	{
	  hadrons[nhadrons].prob[0]=1;
	  for (i=1; i<MAXMUON; ++i) hadrons[nhadrons].prob[i]=0.;
	  hadrons[nhadrons].index=index;
	  hadrons[nhadrons].decay=0;
	  hadrons[nhadrons].next=0;
	  return (&(hadrons[nhadrons++]));
	}
      else
	return 0;
    }

  a=abs(kf);
  kc=pycomp(&a);
  if (last_decay[kc]<0) return 0;      // no possible muon parent

  if (ptmin2[kc]>0.)            // test whether muon is kinematically possible
    {
      px=P(index,1);            // calculate pt of hadron
      py=P(index,2);
      pt2=px*px+py*py;
      if (pt2<ptmin2[kc]) return 0;
    }

  for (i=0; i<MAXMUON; ++i) prob[i]=0;
  pv=1.;

  for (i=1; i<=5; ++i) v[i]=V(index,i);      // save production vertex position
  if (pydat2.pmas[3][kc-1]>100.)                  // lifetime long (>10cm ?)
    {
      pv=choose_decay_vertex(index,kc);
      if (pv<=0)
	{
	  K(index,1)=4;      // mark particle as "could have decayed but didn't"

	  return 0;
	}
    }

  pydat3.mdcy[0][kc-1]=1;                 // switch on decay of this particle

  k=kd=K(index,1);                  // remember status code

  if (set_muon_decay (1,kc))      // switch on decays into muons
    {
      n=N+1;
      {
	pydecy_all(index);

	d2=d1=loop_hadrons (n,N);      // decay daughters and count muons

	if (d1==0)            // no muons?
	  {
	    K(index,1)=k;            // declare particle as undecayed
	    N=n-1;                  // forget all decay products
	  }
	else
	  for (i=0; i<MAXMUON; ++i) prob[i]=(d1->prob[i]*=brat_muon[kc]);
      }
    }
  if (set_muon_decay (2,kc))      // switch on decays without prompt muons
    {
      n=N+1;
      kd=K(index,1);            // remember status code
      K(index,1)=k;            // declare particle undecayed
      pydecy_all(index);            // have the particle decay again

      d2=loop_hadrons (n,N);      // loop over decay products

      if (d2==0)                  // no muons?
	{
	  N=n-1;                  // => forget about decay
	  K(index,1)=kd;            // restore old status code
	  d2=d1;
	}
      else
	{
	  for (i=0; i<MAXMUON; ++i)
	    prob[i]+=(d2->prob[i]*=brat_nonmuon[kc]);      // add probabilities
	  if (prob[0]>0) d2->next=d1;            // => keep decays (d1 may be 0)
	  
	}
    }
  pydat3.mdcy[0][kc-1]=0;                  // switch decay off again
  
  for (i=1; i<=5; ++i) V(index,i)=v[i]; // restore production vertex position
  if (K(index,1)==5) K(index,1)=1;
  if (prob[0]>0)            // any muon finally?
    {
      for (i=0; i<MAXMUON; ++i)
	hadrons[nhadrons].prob[i]=pv*prob[i];      // take decay probability into account
      hadrons[nhadrons].index=index;
      hadrons[nhadrons].decay=d2;
      hadrons[nhadrons].next=0;
      return (&(hadrons[nhadrons++]));
    }
  else
    return (0);
    }


void edm::PythiaMuEnrichProducer::default_decay_modes()
  /***********************************************************************/
  /* restore default decay modes for all particles                       */
  /***********************************************************************/
{
  int i,j,a,kc,kf;

  i=0;
  while ((kf=muon_parents[i]))
    {
      a=abs(kf);
      kc=pycomp(&a);
      for (j=first_muon[kc]; j<=last_decay[kc]; ++j)
	pydat3.mdme[0][decay_mode_list[j]-1]=1;
      pydat3.mdcy[0][kc-1]=1;
      ++i;
    }

  pydat1.mstj[21]=4;            // limit decay to cylinder
  pydat1.parj[72]=RADIUS;
  pydat1.parj[73]=LENGTH;

}


void edm::PythiaMuEnrichProducer::choose_decay(t_decayed_hadron *hadron, unsigned int nmuon)
  /***********************************************************************/
  /*                                                                     */
  /*  from the set of possible decay modes of a hadron,                  */
  /*  choose one which gives exactly nmuon muons                         */
  /*                                                                     */
  /***********************************************************************/
{
  int dum;
  unsigned int i;
  double p;
  double r;
  double ph[MAXMUON+1],pn[MAXMUON];
  t_decay *d=0;
  t_decayed_hadron *h;

  if (!nmuon)
    {
      choose_nomuon(hadron);
      return;
    }

  if (nmuon>MAXMUON)
    {
      LogTrace ("PythiaMuEnrichProducer") << "error in choose_decay: too many muons requested";
      LogTrace ("PythiaMuEnrichProducer") << " - won't get more than "<<MAXMUON;
      nmuon=MAXMUON;
    }

  d=hadron->decay;
  if (d==0) return;
  if (d->next!=0)                  // more than one decay
    {                              // => decision needed
      p=0;
      for (; d!=0; d=d->next)
	p+= (nmuon==MAXMUON ? d->prob[nmuon-1] : d->prob[nmuon-1]-d->prob[nmuon]);
      r=pyr_(&dum)*p;
      for (d=hadron->decay; d!=0; d=d->next)
	{
	  r-= (nmuon==MAXMUON ? d->prob[nmuon-1] : d->prob[nmuon-1]-d->prob[nmuon]);
	  if (r<0)
	    {
	      r+=1.;      // was 2 but 1 should be sufficient
	      hadron->decay=d;
	    }
	  else
	    undecay (d);
	}
    }

  d=hadron->decay;
  d->next=0;                        // only one decay left

  // need to update daughter information here which may be
  // wrong because of multiple decays of the same particle:
  if ( hadron->index > 0 ) {  // don't do it for muons: they have index 0 !!!
    K(hadron->index,4)=d->first;
    K(hadron->index,5)=d->last;
  // note: d->last is the last of all particles arising
  //       from the decay, not only the immediate daughters
  }
  h=d->daughter;
  if (h==0)
    {
      LogTrace ("PythiaMuEnrichProducer") << "decay without products in cbhoose_decay";
      return;
    }
  while (h->next!=0)                  // more than one decay product
    {                              // => decision needed
      if (nmuon)
	{
	  prob_nmuon(h->next,nmuon,pn);
	  if (nmuon==MAXMUON)      // for nmuon=MAXMUON, use probabilities for
	    {                        // >= MAXMUON, but select only MAXMUON
	      p=ph[0]=((double)1-h->prob[0])*pn[nmuon-1];
	      for (i=1; i<nmuon; ++i)
		p+=(ph[i]=(h->prob[i-1]-h->prob[i])*(pn[nmuon-i-1]));
	      p+=(ph[nmuon]=h->prob[nmuon-1]);
	    }
	  else
	    {
	      p=ph[0]=((double)1-h->prob[0])*(pn[nmuon-1]-pn[nmuon]);
	      for (i=1; i<nmuon; ++i)
		p+=(ph[i]=(h->prob[i-1]-h->prob[i])*(pn[nmuon-i-1]-pn[nmuon-i]));
	      p+=(ph[nmuon]=(h->prob[nmuon-1]-h->prob[nmuon])*((double)1-pn[0]));
	    }

	  r=pyr_(&dum)*p;      // choose number of muons for this hadron
	  for (i=nmuon; i; --i)
	    {
	      r-=ph[i];
	      if (r<0) break;
	    }
          
	  nmuon-=i;
	}
      else i=0;
      choose_decay(h,i);
      h=h->next;
    }
    choose_decay(h,nmuon);
}

void edm::PythiaMuEnrichProducer::zero_hepevnt()
{
  for (  int i=0; i < HepMC::HEPEVT_Wrapper::number_entries(); ++i ) {
    HepMC::HEPEVT_Wrapper::set_status( i, 0 );
    HepMC::HEPEVT_Wrapper::set_id( i, 0 );
    HepMC::HEPEVT_Wrapper::set_parents( i, 0, 0 );
    HepMC::HEPEVT_Wrapper::set_children( i, 0, 0 );
    HepMC::HEPEVT_Wrapper::set_momentum( i, 0, 0, 0, 0 );
    HepMC::HEPEVT_Wrapper::set_mass( i, 0 );
    HepMC::HEPEVT_Wrapper::set_position( i, 0, 0, 0, 0 );
  }
  HepMC::HEPEVT_Wrapper::set_number_entries( 0 );
  
}

bool edm::PythiaMuEnrichProducer::isbc()
  // true if the partons in the partons interacting are c or b quarks, or c and b quarks originate from gluon splitting
{

  if (abs(K(7,2)) == 4 || abs(K(8,2)) == 4 ) return true;  
  if (abs(K(7,2)) == 5 || abs(K(8,2)) == 5 ) return true;  
  int ngcs=0;
  int ngbs=0;
  for ( int i=1; i<=N; ++i){
    if ( abs(K(K(i,3),2)) == 21 && abs(K(i,2)) == 4 ) ++ngcs;
    if ( abs(K(K(i,3),2)) == 21 && abs(K(i,2)) == 5 ) ++ngbs;
  }
  if ( ngcs >1 || ngbs > 1) return true;
  return false;
}
void edm::PythiaMuEnrichProducer::countbc(int sel)
  // true if the partons in the partons interacting are c or b quarks, or c and b quarks originate from gluon splitting
{
  int nbhs=0;
  int nchs=0;
  int nbgs=0;
  int ncgs=0;
  if (abs(K(7,2)) == 4)  ++nchs;  
  else if (abs(K(7,2)) == 5 ) ++nbhs;
  if (abs(K(8,2)) == 4 ) ++nchs;  
  else if (abs(K(8,2)) == 5 ) ++nbhs;  
  for ( int i=1; i<=N; ++i){
    if ( abs(K(K(i,3),2)) == 21 && abs(K(i,2)) == 4 ) ++ncgs;
    if ( abs(K(K(i,3),2)) == 21 && abs(K(i,2)) == 5 ) ++nbgs;
  }
  if ( nbhs == 1 ) {
    if (sel==1)++ngenbfe;
    else ++nselbfe;
    ptype=1;
  }else if ( nbhs == 2){
    if (sel==1)ngenbpf++;
    else ++nselbpf;
    ptype=2;
  }else if (nchs==1){
    if (sel==1)ngencfe++;
    else ++nselcfe;
    ptype=11;
  }else if (nchs==2){
    if (sel==1)ngencpf++;
    else ++nselcpf;
    ptype=12;
  } else if (ncgs==2 && (nchs==0 && nbhs==0) ) {
    if (sel==1)ngenbgs++;
    else ++nselbgs;
    ptype=3;
  }else if (nbgs==2 && (nchs==0 && nbhs==0) ) {
    if (sel==1)ngencgs++;
    else ++nselcgs;
    ptype=13;
  }
}
double edm::PythiaMuEnrichProducer::wgtmu(int nmuon, float fl)
  /* force an event to have at least nmu muons
     input: nmuon      minimum number of desired muons
     fl      minimum desired weight
     returned:      weight of the event (discard event if wgtmu_=0.)
  */
{
  // bc_separately declaration

  int dum;
  int idpy,ndpy;
  int hard_b,hard_c;
  int ngs_b,ngs_c;
  int i;
  t_decay *d=0;
  t_decayed_hadron h;
  double r;
  double weight,wtxs;

  weight=1;

  if ((i=count_muons(1,N)))
    {
      LogTrace ("PythiaMuEnrichProducer") << i<< " muons already found before decay\n";
    }

  nmuon-=i;
  nhadrons=ndecays=0;
  if (nmuon<=0)                  // already enough muons?
    {
      default_decay_modes();
      pyexec();                  // decay the event without constraints
      switch_off_decays();      // prepare for next event  
   }
  else
    {
      d=loop_hadrons (9,N);      // decay all hadrons into muons
      if (d==0) return 0;      // no more muons?
  
      weight=d->prob[nmuon-1];
      if (weight==0) return 0;

      r=pyr_(&dum)*d->prob[nmuon-1];      // choose number of muons to select

      for (i=nmuon; i<MAXMUON; ++i)
	if (r>d->prob[i]) break;

      if (i==MAXMUON)
	{
	  LogTrace ("PythiaMuEnrichProducer") << "Warning in WGTMU: Event wants "<<i<<" or more muons,";
	  LogTrace ("PythiaMuEnrichProducer") << " but will never get more than "<<i;
	  LogTrace ("PythiaMuEnrichProducer") << "   If needed, this can be fixed by increasing the parameter MAXMUON";
	}

      nmuon=i;
    }


  // this is the  pyevwt of Nicola
  /* get PYTHIA weight */
  
  //double pt2=pyint1.vint[47];
  //double pthat=sqrt(pt2);
  //double w1=1;
  //if ( wtype == 3 ) w1=0.2*(1+0.0003*pthat*pthat);
  //wtxs=w1<1?w1:1; 
  //wtxs=weight/wtxs;
  wtxs=weight;

  // ============================================================================
  //  identify leading order c-cbar and b-bbar production and flavour excitation
  //  and reduce the weight of these events by factor wbc
  //  These events have to generated separately and added in order to recover
  //  a complete sample!
  //  b-events are additionally scaled by 500mb/bbxsec(pthat=0,mstp(82)=1)=0.42 -> not done
  //  following recommendations for LHC b-physics
  // ============================================================================
  /* down-scale gluon splitting (GS) events and b,c events (FE + PF) */
  
  //  
  ngs_b =0;
  ngs_c =0;          
  hard_b=0;
  hard_c=0;
  ndpy=pyjets.n;
 
  if (abs(K(7,2))==5 || abs(K(8,2))==5)hard_b=1;
  if (abs(K(7,2))==4 || abs(K(8,2))==4)hard_c=1;

  for (idpy=1; idpy<=ndpy; ++idpy) 
    {
      if ( abs(K(K(idpy,3),2))==21 &&  abs(K(idpy,2))==4 ) ++ngs_c;
      if ( abs(K(K(idpy,3),2))==21 &&  abs(K(idpy,2))==5 ) ++ngs_b;
    }
  //rescale by 10 (wbc) to account for enriched luminosity

  if( ngs_b >= 2 || hard_b==1 || ngs_c>=2 || hard_c==1 )wtxs*=wbc; 

  //additional rescaling for bb xsec

  //if( ngs_b >= 2 || hard_b==1 ) wtxs*=0.42; 

  // ============================================================================

  /* downscale events with low weights      */
  if (wtxs>0 && wtxs<fl)
    {
      r=pyr_(&dum);
      if (r<wtxs/fl)
	{
	  wtxs=fl;
	}
      else
	{
	  return 0.;
	}
    }
  else
    LogTrace ("PythiaMuEnrichProducer") << "Event not downscaled, weight="<<wtxs<<" Number of muons to produce by decay:"<<nmuon;

  if (nmuon>0)
    {
      // create dummy hadron in order to avoid duplicating the loop over hadrons
      // the last decay pointer contains all the probabilities although no real decay
      h.index=0;
      for (i=0; i<MAXMUON; ++i) h.prob[i]=d->prob[i];
      h.decay=d;  
      h.next=0;
      default_decay_modes();
      choose_decay(&h,nmuon);      // select a decay mode with nmu muons
      switch_off_decays();      // prepare for next event
      i=12;
      pyedit(&i);            // remove deleted particles from event record
      LogTrace ("PythiaMuEnrichProducer") << "WGTMU: event weight="<<d->prob[0]<<", after downscaling:"<<wtxs;
    }  
  default_decay_modes();
  pyexec_nomu();            // decay all hadrons without any muons
  switch_off_decays();            // prepare for next event
  LogTrace ("PythiaMuEnrichProducer") << "After All N="<<N<<" and number of muons=" << count_all_muons(9,N) <<" nbcsim="<<nbcsim;
  return wtxs;
}
