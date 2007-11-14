#include "CSA07anl/Analyzer/interface/Z2TAUEMUFILTER.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

Z2TAUEMUFILTER::Z2TAUEMUFILTER(const edm::ParameterSet& iConfig) :
  label_(iConfig.getUntrackedParameter("moduleLabel", string("source"))),
  Verbosity_(iConfig.getUntrackedParameter<bool>("verbosity", false)),
  maxEvents_(iConfig.getUntrackedParameter<int>("MaxEvents",100000))
{  

  nev_ = 0; 

}

Z2TAUEMUFILTER::~Z2TAUEMUFILTER()
{

  if (Verbosity_) {

    cout << "=== Selected events ===" << endl;
    cout << "total   : " << nev_ << endl;
  }

}

bool Z2TAUEMUFILTER::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if(nev_ >= maxEvents_)   {
    throw cms::Exception("endJob") << "we have reached the maximum number of events ";
  }

  Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);

  //HepMC::GenEvent *myGenEvent = new HepMC::GenEvent(*(evt->GetEvent()));
  const HepMC::GenEvent *myGenEvent = evt->GetEvent();

  HepMC::GenEvent::particle_const_iterator p;

 

  bool foundZto2tau2emu = false;


  for (p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {
    
    bool foundZtoTauToE = false;
    bool foundZtoTauToMu = false;

    // Look for heavy bosons and look at their children
    if (abs( (*p)->pdg_id()) == 23 	 )
    {

      HepMC::GenVertex * bosonDecayVertex = (*p)->end_vertex();
      // now look for tau
      if (bosonDecayVertex != 0) {
	for ( HepMC::GenVertex::particles_out_const_iterator kidit = bosonDecayVertex->particles_out_const_begin();
	      kidit != bosonDecayVertex->particles_out_const_end(); kidit++) {
	  if (abs( (*kidit)->pdg_id()) == 15) {
	    // now look for tau -> e or mu
	    HepMC::GenVertex * tauDecayVertex = (*kidit)->end_vertex();
	    while (tauDecayVertex !=0) {
	      bool foundTauDaughter = false;
	      for ( HepMC::GenVertex::particles_out_const_iterator taukid = tauDecayVertex->particles_out_const_begin();
		    taukid != tauDecayVertex->particles_out_const_end(); 
		    taukid++) {
		if (abs( (*taukid)->pdg_id()) == 15) {      // this is to take into account that tau shows up as its own daughter
		  tauDecayVertex = (*taukid)->end_vertex(); // at least in the pythia decay tree
		  foundTauDaughter = true;
		  break;
		}
		if (abs( (*taukid)->pdg_id()) == 11) {
		  foundZtoTauToE = true;
		} else if (abs( (*taukid)->pdg_id()) == 13) {
		  foundZtoTauToMu = true;
		}
	      }
	      if (!foundTauDaughter) tauDecayVertex = 0;
	    }
	  }
	}
      }
    }
    if (foundZtoTauToE && foundZtoTauToMu) {
      cout << "FOUND Z -> 2 tau -> e+mu " << std::endl;
      nev_++;
      return true;
    }
  }
    
  return false;

}

