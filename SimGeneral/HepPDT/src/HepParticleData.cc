// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:52 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepParticleData.

#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/HepPDT/interface/HepParticleData.h"
#include "GeneratorInterface/HepPDT/interface/HepDecayMode.h"

#ifdef HEP_DEBUG_INLINE
#include "GeneratorInterface/HepPDT/interface/HepParticleData.icc"
#endif

void HepParticleData::addDecayMode(HepDecayMode & dm) {
  dm.parent(*this);
  decayModes[dm.tag()] = &dm;
}

