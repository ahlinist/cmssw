//  Author:             Christian.Weiser@cern.ch
//  Ported to CMSSW by: Andrea.Bocci@cern.ch
//  Last Update:        12/07/2006

#include "RecoBTag/MCTools/interface/MCBaseParticle.h"

using namespace edm;
using namespace std;
using namespace HepMC;

MCBaseParticle::MCBaseParticle( const HepMC::GenParticle* particle, const HepMC::GenEvent* event ):
  m_LundCode( particle->pdg_id() ) //why was it barcode???
 {
  m_HepParticle = particle;
  m_HepEvent  = event;

  setFourVector( particle->momentum() );
}

MCBaseParticle::~MCBaseParticle() {
  //this->print();
}


void MCBaseParticle::print() const {
  cout << "--> MCBaseParticle:" << endl;
//   cout << "--> LundCode                :" << m_LundCode.LundCode() << endl;
//   cout << "--> mass                    :" << mass                  << endl;
//   cout << "--> eta                     :" << eta                   << endl;
//   cout << "--> phi                     :" << phi                   << endl;
//   cout << "--> pabs                    :" << pabs                  << endl;
//   cout << "--> e                       :" << e                     << endl;
//   cout << "--> px                      :" << FourVector.px()       << endl;
//   cout << "--> py                      :" << FourVector.py()       << endl;
//   cout << "--> pz                      :" << FourVector.pz()       << endl;
}
  

