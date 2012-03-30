#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
//#include "Validation/VstStdhep/interface/stdhep_mcfio.h"
//#include "Validation/VstStdhep/interface/stdhep_declarations.h"
#include "Validation/VstQuaeroUtils/interface/HepevtBlock.hh"

#include "HepMC/HEPEVT_Wrapper.h"

extern "C" double stdhep2parton_(char *filename, int length);

using namespace std;

void stdhep2partonWrap(std::string inputStdhepFilename,std::string outputTextFilename)
{
  char  infile[230] = " ";
  char  txtfile[230] = " ";
  strncpy(infile,inputStdhepFilename.c_str(),inputStdhepFilename.length());
  strncpy(txtfile,outputTextFilename.c_str(),outputTextFilename.length());

  //int ierr = StdHepXdrReadInit(infile, nevt, istr);
  int ierr = 1;
  if (ierr != 0) {
    std::cout << "convertStdHep ERROR: problem opening input file: "
	      << infile << std::endl; 
    return;
  }
  // start reading
  /*  int i = 0;
      int lbl = 0; */
  /*  while (ierr == 0) {
    //    ierr = StdHepXdrRead(&lbl,istr);
    if (ierr == 0) {
      ++i;
      if( lbl==1 || lbl==2 ) {
	HepMC::HEPEVT_Wrapper::set_event_number( myhepevt.nevhep );
	HepMC::HEPEVT_Wrapper::set_number_entries( myhepevt.nhep );
	for ( int i1 = 1; i1 <= myhepevt.nhep; ++i1 ) {
	  int j1=i1-1;
	  HepMC::HEPEVT_Wrapper::set_status( i1, myhepevt.isthep[j1]);
	  HepMC::HEPEVT_Wrapper::set_id( i1, myhepevt.idhep[j1]);
	  HepMC::HEPEVT_Wrapper::set_parents( i1, myhepevt.jmohep[j1][0],myhepevt.jmohep[j1][1]);
	  HepMC::HEPEVT_Wrapper::set_children( i1, myhepevt.jdahep[j1][0],myhepevt.jdahep[j1][1]);
	  HepMC::HEPEVT_Wrapper::set_momentum( i1, myhepevt.phep[j1][0],myhepevt.phep[j1][1],myhepevt.phep[j1][2],myhepevt.
					       phep[j1][3]);
	  HepMC::HEPEVT_Wrapper::set_mass( i1, myhepevt.phep[j1][4]);
	  HepMC::HEPEVT_Wrapper::set_position( i1, myhepevt.vhep[j1][0],myhepevt.vhep[j1][1],myhepevt.vhep[j1][2],myhepevt.
					       vhep[j1][3]);
	}
	stdhep2parton_(txtfile,outputTextFilename.length());
      } else if (lbl==200) {
	//	StdHepXdrEnd(istr);
	ierr=1;
      }
    }
    } */
  return;
}

void stdhep2partonWrapSimple(std::string outputTextFilename, const HepevtEvent &e)
{
  char  txtfile[230] = " ";
  strncpy(txtfile,outputTextFilename.c_str(),outputTextFilename.length());

  HepMC::HEPEVT_Wrapper::set_event_number( e.nevhep );
  HepMC::HEPEVT_Wrapper::set_number_entries( e.particles.size() );
  for ( size_t i1 = 1; i1 <= e.particles.size(); ++i1 ) {
    size_t j1=i1-1;
    HepevtParticle p = e.particles[j1];
    HepMC::HEPEVT_Wrapper::set_status( i1, p.isthep);
    HepMC::HEPEVT_Wrapper::set_id( i1, p.idhep);
    HepMC::HEPEVT_Wrapper::set_parents( i1, p.jmohep[0], p.jmohep[1] );
    HepMC::HEPEVT_Wrapper::set_children( i1, p.jdahep[0], p.jdahep[1] );
    HepMC::HEPEVT_Wrapper::set_momentum( i1, p.phep[0], p.phep[1], p.phep[2], p.phep[3]);
    HepMC::HEPEVT_Wrapper::set_mass( i1, p.phep[4]);
    HepMC::HEPEVT_Wrapper::set_position( i1, p.vhep[0], p.vhep[1], p.vhep[2], p.vhep[3]);
  }
  //  stdhep2parton_(txtfile,outputTextFilename.length());
  return;
}
