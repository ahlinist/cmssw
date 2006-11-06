#include "RecoBTag/MCTools/interface/MCBaseParticle.h"

using namespace edm;
using namespace std;
using namespace HepMC;

// is the event needed here?
MCBaseParticle::MCBaseParticle( const HepMC::GenParticle* particle, const HepMC::GenEvent* event ):
  particleInfo_( particle->pdg_id() )
 {
  hepParticle = particle;
  hepEvent  = event;
  fourVector_ = lorentzVect( particle->momentum() );
}

MCBaseParticle::~MCBaseParticle() {
  //this->print();
}


void MCBaseParticle::print() const {
  cout << "--> MCBaseParticle:" << endl;
//   cout << "--> LundCode                :" << particleInfo.LundCode() << endl;
//   cout << "--> mass                    :" << mass                  << endl;
//   cout << "--> eta                     :" << eta                   << endl;
//   cout << "--> phi                     :" << phi                   << endl;
//   cout << "--> pabs                    :" << pabs                  << endl;
//   cout << "--> e                       :" << e                     << endl;
//   cout << "--> px                      :" << FourVector.px()       << endl;
//   cout << "--> py                      :" << FourVector.py()       << endl;
//   cout << "--> pz                      :" << FourVector.pz()       << endl;
}
  

