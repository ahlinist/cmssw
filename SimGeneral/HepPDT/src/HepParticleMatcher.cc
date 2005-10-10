// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2005/10/10 10:22:02 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepParticleMatcher.

#include "SimGeneral/HepPDT/interface/HepParticleMatcher.h"

#ifdef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepParticleMatcher.icc"
#endif

void HepParticleMatcher::addIfMatch(const HepParticleMatcher & pm) {
  ParticleSet::const_iterator i = pm.matchingParticles.begin();
  while ( i != pm.matchingParticles.end() ) {
    if ( matchingParticles.find(*i++) == matchingParticles.end() ) return;
  }
  matchingMatchers.insert(&pm);
}


void HepParticleMatcher::addIfMatch(const HepParticleData & pd) {
  if ( !checkMatch(pd) ) return;
  if ( matchingParticles.size() ) {
    commonMass = pd.mass();
    commonWidth = pd.width();
    commonCTau = pd.cTau();
    commonCharge = pd.iCharge();
    commonSpin = pd.iSpin();
    commonColor = pd.iColor();
    commonStable = pd.stable();
  } else {
    if ( commonMass != pd.mass() ) commonMass = -1.0;
    if ( commonWidth != pd.width() ) commonWidth = -1.0;
    if ( commonCTau != pd.cTau() ) commonCTau = -1.0;
    if ( commonCharge != pd.iCharge() ) {
      switch ( commonCharge ) {
      case HepPDT::Cundefined:
	break;
      case HepPDT::Cpositive:
	if ( HepPDT::negative(pd.iCharge()) ) commonCharge = HepPDT::Ccharged;
	else if ( !HepPDT::positive(pd.iCharge()) )
	  commonCharge = HepPDT::Cundefined;
	break;
      case HepPDT::Cnegative:
	if ( HepPDT::positive(pd.iCharge()) ) commonCharge = HepPDT::Ccharged;
	else if ( !HepPDT::negative(pd.iCharge()) )
	  commonCharge = HepPDT::Cundefined;
	break;
      case HepPDT::Ccharged:
	if ( !HepPDT::charged(pd.iCharge()) )
	  commonCharge = HepPDT::Cundefined;
	break;
      default:
	if ( HepPDT::positive(commonCharge) ) {
	  if ( HepPDT::positive(pd.iCharge()) )
	    commonCharge = HepPDT::Cpositive;
	  else if ( HepPDT::negative(pd.iCharge()) )
	    commonCharge = HepPDT::Ccharged;
	  else commonCharge = HepPDT::Cundefined;
	}
	else if ( HepPDT::negative(commonCharge) ) {
	  if ( HepPDT::negative(pd.iCharge()) )
	    commonCharge = HepPDT::Cnegative;
	  else if ( HepPDT::positive(pd.iCharge()) )
	    commonCharge = HepPDT::Ccharged;
	  else commonCharge = HepPDT::Cundefined;
	}
	else commonCharge = HepPDT::Cundefined;
      }
    }
    if ( commonSpin != pd.iSpin() ) commonSpin = HepPDT::Sundefined;
    if ( commonColor != pd.iColor() ) {
      if ( HepPDT::colored(commonColor) && HepPDT::colored(pd.iColor()) )
	commonColor = HepPDT::colcolored;
      else
	commonColor = HepPDT::colundefined;
      if ( commonStable != pd.stable() ) commonStable = -1;
    }
  }
  matchingParticles.insert(&pd);
}

HepBoolean HepParticleMatcher::checkCC(const HepParticleMatcher & pm) const {
  ParticleSet::const_iterator it = pm.matchingParticles.begin();
  ParticleSet antiParticles;
  while ( it != pm.matchingParticles.end() ) {
    const HepParticleData * pd = *it++;
    if ( pd->CC() ) pd = pd->CC();
    antiParticles.insert(pd);
  }
  return HepBoolean( antiParticles == matchingParticles );
}



HepBoolean HepParticleMatcher::CC(HepParticleMatcher & pm) {
  if ( !checkCC(pm) ) return false;
  antiPartner = &pm;
  pm.antiPartner = this;
  return true;
}

void HepParticleMatcher::init() {
  HepPDT::ParticleMap::const_iterator pit = HepPDT::particles().begin();
  while ( pit != HepPDT::particles().end() ) addIfMatch(*(*pit++).second);
  HepPDT::MatcherMap::const_iterator mit = HepPDT::matchers().begin();
  while ( mit != HepPDT::matchers().end() ) {
    if ( checkCC(*(*mit).second) ) antiPartner = (*mit).second;
    addIfMatch(*(*mit++).second);
  }
}

    
  
