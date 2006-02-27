/*
 *  $Date: 2006/02/27 15:21:56 $
 *  $Revision: 1.9 $
 *  
 *  Filip Moorgat & Hector Naves 
 *  26/10/05
 * 
 * Patrick Janot : Add the PYTHIA card reading
 */


#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include <iostream>
#include "time.h"

using namespace edm;
using namespace std;

// Generator modifications
// ***********************
#include "CLHEP/HepMC/include/PythiaWrapper6_2.h"
#include "CLHEP/HepMC/ConvertHEPEVT.h"
#include "CLHEP/HepMC/CBhepevt.h"

extern "C" {
  int PYCOMP(int*);
}

extern struct {
  int imss[100];
  double rmss[100];
} pymssm_;
#define pymssm pymssm_;


HepMC::ConvertHEPEVT conv;
// ***********************


//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;

PythiaSource::PythiaSource( const ParameterSet & pset, 
			    InputSourceDescription const& desc ) :
  GeneratedInputSource(pset, desc), evt(0), 
  pythiaVerbosity_ (pset.getUntrackedParameter<bool>("pythiaVerbosity",false))
{
  
  cout << "PythiaSource: initializing Pythia. " << endl;

  // Verbosity
  pythiaVerbosity_ = pset.getUntrackedParameter<bool>("pythiaVerbosity",false);
  cout << "Pythia verbosity = " << pythiaVerbosity_ << endl;

  // Set PYTHIA parameters in a single ParameterSet
  ParameterSet pythia_params = 
    pset.getParameter<ParameterSet>("PythiaParameters") ;

  initializePysubs(pythia_params);
  initializePypars(pythia_params);
  initializePydat1(pythia_params);
  initializePydat2(pythia_params);
  initializePydat3(pythia_params);
  initializePydatr(pythia_params);
//initializePymssm(pythia_params);
  initializePyint2(pythia_params);

  call_pyinit( "CMS", "p", "p", 14000. );
  cout << endl; // Stetically add for the output
  //********                                      


  produces<HepMCProduct>();
  cout << "PythiaSource: starting event generation ... " << endl;
 }


PythiaSource::~PythiaSource(){
  cout << "PythiaSource: event generation done. " << endl;
  clear();
}

void PythiaSource::clear() {
 
}


bool PythiaSource::produce(Event & e) {

    auto_ptr<HepMCProduct> bare_product(new HepMCProduct());  
    //cout << "PythiaSource: Generating event ...  " << endl;

    //********                                         
    //
    call_pyevnt();      // generate one event with Pythia
    call_pyhepc( 1 );
    
    HepMC::GenEvent* evt = conv.getGenEventfromHEPEVT();
    evt->set_signal_process_id(pypars.msti[0]);
    evt->set_event_number(numberEventsInRun() - remainingEvents() - 1);
    
    cout << "Event process = " << pypars.msti[0] << endl 
	 << "----------------------" << endl;
    if (pythiaVerbosity_) evt->print();

    //evt = reader_->fillCurrentEventData(); 
    //********                                      

    if(evt)  bare_product->addHepMCData(evt );

    e.put(bare_product);

    return true;
}

void PythiaSource::initializePysubs(const ParameterSet& pset) {

  // Initialize msel and mselpd
  int pysubs_msel = pset.getUntrackedParameter<int>("Pysubs_msel",0);
  pysubs.msel = pysubs_msel;

  int pysubs_mselpd = pset.getUntrackedParameter<int>("Pysubs_mselpd",0);
  pysubs.mselpd = pysubs_mselpd;

  // Initialize msub(200)
  vector<int> pysubs_msub = 
    pset.getUntrackedParameter< vector<int> >("Pysubs_msub", vector<int>());
  int evenSize = pysubs_msub.size();
  for(int i=0;i<evenSize;++i) pysubs.msub[pysubs_msub[i]-1] = 1;
  
  // Initialize ckin(200)
  genericInitCard(pset,pysubs.ckin[0],"Pysubs_ckin",200);

  // Initialize kfin(1,-40:40) & kfin(2,-40:40) simultaneously
  // (1 is for the beam, 2 is for the target - no difference at LHC)
  vector<int> pysubs_kfin = 
    pset.getUntrackedParameter< vector<int> > ("Pysubs_kfin", vector<int>());

  evenSize = pysubs_kfin.size();
  if(evenSize%2!=0) {

    cout <<"In PythiaSource::initializePysubs_kfin " 
	 <<" : wrong number of parameters " << evenSize
	 << "(should be even). Will drop the last one."<< endl;
    evenSize--;

  }

  for(int i=0;i<evenSize;i+=2) {
    int kf = pysubs_kfin[i];
    if ( abs(kf) > 40 )
      cout << "PythiaSource::initializePysubs : kfin[" 
	   << kf << "] cannot be changed - Check your config file"
	   << endl;
    else {
      
      int newValue = pysubs_kfin[i+1];
      cout << "PythiaSource::initializePysubs :" 
	   << "kfin[" << kf << "] changed from " 
	   << pysubs.kfin[kf+40][0] 
	   << " to " <<  newValue << endl;
      
      pysubs.kfin[kf+40][0] = newValue;
      pysubs.kfin[kf+40][1] = newValue;
      
    }
  }
    
}

void PythiaSource::initializePypars(const ParameterSet& pset) {

  // Initialize mstp(200)
  genericInitCard(pset,pypars.mstp[0],"Pypars_mstp",200); // mstp(..)

  // Initialize parp(200)
  genericInitCard(pset,pypars.parp[0],"Pypars_parp",200); // parp(..)

}

void PythiaSource::initializePydat1(const ParameterSet& pset) {

  // Initialize mstu(200)
  genericInitCard(pset,pydat1.mstu[0],"Pydat1_mstu",200); // mstu(..)

  // Initialize paru(200)
  genericInitCard(pset,pydat1.paru[0],"Pydat1_paru",200); // paru(..)

  // Initialize parj(200)
  genericInitCard(pset,pydat1.parj[0],"Pydat1_parj",200); // parj(..)

  // Initialize mstj(200)
  genericInitCard(pset,pydat1.mstj[0],"Pydat1_mstj",200); // mstj(..)
}


void PythiaSource::initializePydat2(const ParameterSet& pset) {

  // Initialize kchg(500,3)
  genericInitCard(pset,pydat2.kchg[0][0],"Pydat2_kchg1",500,1,1); // kchg(..,1)
  genericInitCard(pset,pydat2.kchg[1][0],"Pydat2_kchg2",500,1,1); // kchg(..,2)
  genericInitCard(pset,pydat2.kchg[2][0],"Pydat2_kchg3",500,1,1); // kchg(..,3)

  // Initialize pmas(500,4)
  genericInitCard(pset,pydat2.pmas[0][0],"Pydat2_pmas1",500,1,1); // pmas(..,1)
  genericInitCard(pset,pydat2.pmas[1][0],"Pydat2_pmas2",500,1,1); // pmas(..,2)
  genericInitCard(pset,pydat2.pmas[2][0],"Pydat2_pmas3",500,1,1); // pmas(..,3)
  genericInitCard(pset,pydat2.pmas[3][0],"Pydat2_pmas4",500,1,1); // pmas(..,4)

  // Initialize vckm(4,4)
  genericInitCard(pset,pydat2.vckm[0][0],"Pydat2_vckm1",4); // vckm(..,1)
  genericInitCard(pset,pydat2.vckm[1][0],"Pydat2_vckm2",4); // vckm(..,2)
  genericInitCard(pset,pydat2.vckm[2][0],"Pydat2_vckm3",4); // vckm(..,3)
  genericInitCard(pset,pydat2.vckm[3][0],"Pydat2_vckm4",4); // vckm(..,4)

  // Initialize parf(2000)
  genericInitCard(pset,pydat2.parf[0]  ,"Pydat2_parf",2000); // parf(..)

}

void PythiaSource::initializePydat3(const ParameterSet& pset) {


  // Initialize mdcy(500,3)
  genericInitCard(pset,pydat3.mdcy[0][0],"Pydat3_mdcy1",500,1,1); // mdcy(..,1)
  genericInitCard(pset,pydat3.mdcy[1][0],"Pydat3_mdcy2",500,1,1); // mdcy(..,2)
  genericInitCard(pset,pydat3.mdcy[2][0],"Pydat3_mdcy3",500,1,1); // mdcy(..,3)

  // Initialize mdme(8000,2)

  //First, disable series of channels (special card mdme0)
  vector<int> pydat3_mdme0 = 
    pset.getUntrackedParameter< vector<int> > ("Pydat3_mdme0", vector<int>());
  int evenSize = pydat3_mdme0.size();
  if(evenSize%2!=0) {

    cout<<"PythiaSource::initializePydat3(): "
	   <<"Wrong number of parameters in pydat3_mdme0: "<<evenSize
	   <<" (should be even). Will drop the last one."<<endl;
    evenSize--;
  }

  for(int i=0;i<evenSize;i+=2) {
    int begin = pydat3_mdme0[i];
    int end = pydat3_mdme0[i+1];
    for(int j=begin;j<=end;j++) { 

      if ( pydat3.mdme[0][j-1] > 0 ) {  
	cout << "PythiaSource::initializePydat3 :" 
	     << "mdme[" << j << ",1] changed from " 
	     << pydat3.mdme[0][j-1] << " to 0" << endl;
	
	pydat3.mdme[0][j-1] = 0;
      }
    }
  }
    
  // Then enable/disable individual channels
  genericInitCard(pset,pydat3.mdme[0][0],"Pydat3_mdme1",8000); // mdme(..,1)

  // and add special information for each decay channel
  genericInitCard(pset,pydat3.mdme[1][0],"Pydat3_mdme2",8000); // mdme(..,2)

  // Initialize brat(8000)
  genericInitCard(pset,pydat3.brat[0]  ,"Pydat3_brat",8000); // brat(..)

  // Initialize kfdp(8000,5)
  genericInitCard(pset,pydat3.kfdp[0][0],"Pydat3_kfdp1",8000); // kfdp(..,1)
  genericInitCard(pset,pydat3.kfdp[1][0],"Pydat3_kfdp2",8000); // kfdp(..,2)
  genericInitCard(pset,pydat3.kfdp[2][0],"Pydat3_kfdp3",8000); // kfdp(..,3)
  genericInitCard(pset,pydat3.kfdp[3][0],"Pydat3_kfdp4",8000); // kfdp(..,4)
  genericInitCard(pset,pydat3.kfdp[4][0],"Pydat3_kfdp5",8000); // kfdp(..,5)


}

void PythiaSource::initializePydatr(const ParameterSet& pset) {

  // Initialize mrpy[0], the seed of all random numbers. Take the 
  // time if not specified in Pydatr.
  
  int pydatr_mrpy = pset.getUntrackedParameter<int>("Pydatr_mrpy",time(NULL)); 
  pydatr.mrpy[0] = pydatr_mrpy;

}

/*
void PythiaSource::initializePymssm(const ParameterSet& pset) {

  // Initialize imss(0:99)
  genericInitCard(pset,pymssm.imss[0],"Pymssm_imss",100); // imss(..)

  // Initialize rmss(0:99)
  genericInitCard(pset,pymssm.rmss[0],"Pymssm_rmss",100); // rmss(..)
}
*/

void PythiaSource::initializePyint2(const ParameterSet& pset) {

  // Initialize iset(500)
  genericInitCard(pset,pyint2.iset[0],"Pyint2_iset",500);      // iset

  // Initialize kfpr(500,2)
  genericInitCard(pset,pyint2.kfpr[0][0],"Pyint2_kfpr1",500);   // kfpr(..,1)
  genericInitCard(pset,pyint2.kfpr[1][0],"Pyint2_kfpr2",500);   // kfpr(..,2)

  // Initialize coef(500,20)
  genericInitCard(pset,pyint2.coef[0][0],"Pyint2_coef1",500);   // coef(..,1)
  genericInitCard(pset,pyint2.coef[1][0],"Pyint2_coef2",500);   // coef(..,2)
  genericInitCard(pset,pyint2.coef[2][0],"Pyint2_coef3",500);   // coef(..,3)
  genericInitCard(pset,pyint2.coef[3][0],"Pyint2_coef4",500);   // coef(..,4)
  genericInitCard(pset,pyint2.coef[4][0],"Pyint2_coef5",500);   // coef(..,5)
  genericInitCard(pset,pyint2.coef[5][0],"Pyint2_coef6",500);   // coef(..,6)
  genericInitCard(pset,pyint2.coef[6][0],"Pyint2_coef7",500);   // coef(..,7)
  genericInitCard(pset,pyint2.coef[7][0],"Pyint2_coef8",500);   // coef(..,8)
  genericInitCard(pset,pyint2.coef[8][0],"Pyint2_coef9",500);   // coef(..,9)
  genericInitCard(pset,pyint2.coef[9][0],"Pyint2_coef10",500); // coef(..,10)
  genericInitCard(pset,pyint2.coef[10][0],"Pyint2_coef11",500); // coef(..,11)
  genericInitCard(pset,pyint2.coef[11][0],"Pyint2_coef12",500); // coef(..,12)
  genericInitCard(pset,pyint2.coef[12][0],"Pyint2_coef13",500); // coef(..,13)
  genericInitCard(pset,pyint2.coef[13][0],"Pyint2_coef14",500); // coef(..,14)
  genericInitCard(pset,pyint2.coef[14][0],"Pyint2_coef15",500); // coef(..,15)
  genericInitCard(pset,pyint2.coef[15][0],"Pyint2_coef16",500); // coef(..,16)
  genericInitCard(pset,pyint2.coef[16][0],"Pyint2_coef17",500); // coef(..,17)
  genericInitCard(pset,pyint2.coef[17][0],"Pyint2_coef18",500); // coef(..,18)
  genericInitCard(pset,pyint2.coef[18][0],"Pyint2_coef19",500); // coef(..,19)
  genericInitCard(pset,pyint2.coef[19][0],"Pyint2_coef20",500); // coef(..,20)

  // Initialize icol(40,4,2)
  genericInitCard(pset,pyint2.icol[0][0][0],"Pyint2_icol11",40); // icol(.,1,1)
  genericInitCard(pset,pyint2.icol[1][0][0],"Pyint2_icol12",40); // icol(.,1,2)
  genericInitCard(pset,pyint2.icol[0][1][0],"Pyint2_icol21",40); // icol(.,2,1)
  genericInitCard(pset,pyint2.icol[1][1][0],"Pyint2_icol22",40); // icol(.,2,2)
  genericInitCard(pset,pyint2.icol[0][2][0],"Pyint2_icol31",40); // icol(.,3,1)
  genericInitCard(pset,pyint2.icol[1][2][0],"Pyint2_icol32",40); // icol(.,3,2)
  genericInitCard(pset,pyint2.icol[0][3][0],"Pyint2_icol41",40); // icol(.,4,1)
  genericInitCard(pset,pyint2.icol[1][3][0],"Pyint2_icol42",40); // icol(.,4,2)

}

template <class T>
void PythiaSource::genericInitCard(const ParameterSet& pset, 
					 T& myArray,
					 const std::string& myCard,
					 int maxBound, 
					 int minBound, 
					 int compressed) {
  
  vector<T> values =
    pset.template getUntrackedParameter<vector<T> > (myCard, vector<T>());

  int evenSize = values.size();
  if(evenSize%2!=0) {

    cout <<"In PythiaSource::genericInitialization " 
     	 << myCard
	 <<" : wrong number of parameters " << evenSize
	 << "(should be even). Will drop the last one."<< endl;
    evenSize--;

  }

  for(int i=0;i<evenSize;i+=2) {

    int kf = (int)values[i];
    int kc  = compressed != 0  ? PYCOMP(&kf) : kf;
    T newValue =  values[i+1];

    if ( kc <= maxBound && kc >= minBound ) {

      cout << "PythiaSource::genericInitialization " 
	   << myCard
	   << "[" << kc << "] changed from " << (&myArray)[kc-minBound]
	   << " to " << newValue << endl;

      (&myArray)[ kc - minBound ]  = newValue;

    } else {

      cout << "PythiaSource::genericInitialization " << myCard
	      << "[" << kc << "] out of range - Check config file" << endl;
      
    }

  }
}  

