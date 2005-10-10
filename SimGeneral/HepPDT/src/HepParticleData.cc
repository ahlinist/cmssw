// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2005/10/10 10:22:02 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepParticleData.

#include "SimGeneral/HepPDT/interface/HepParticleData.h"
#include "SimGeneral/HepPDT/interface/HepDecayMode.h"

#ifdef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepParticleData.icc"
#endif

void HepParticleData::addDecayMode(HepDecayMode & dm) {
  dm.parent(*this);
  decayModes[dm.tag()] = &dm;
}

