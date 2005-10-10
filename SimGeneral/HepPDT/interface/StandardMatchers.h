// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2003/07/21 12:34:39 $
//  $Revision: 1.3 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepStandardMatchers_H_INCLUDED
#define HepStandardMatchers_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the standard particle matchers classes.
//
// The following classes are defined
// <UL>
// <LI><B><TT>HepAnyParticleMatcher</B></TT> matches any particle.
// <LI><B><TT>HepChargedParticleMatcher</B></TT> matches any charged
// particle.
// <LI><B><TT>HepPositiveParticleMatcher</B></TT> matches any
// positively charged particle.
// <LI><B><TT>HepNegativeParticleMatcher</B></TT> matches any
// negatively charged particle.
// <LI><B><TT>HepNeutralParticleMatcher</B></TT> matches any
// neutral particle.
// <LI><B><TT>HepBaryonMatcher</B></TT> matches any baryon.
// <LI><B><TT>HepMesonMatcher</B></TT> matches any meson.
// <LI><B><TT>HepLeptonMatcher</B></TT> matches any lepton.
// <LI><B><TT>HepAnyLightQuarkMatcher</B></TT> matches any light
// quark.
// <LI><B><TT>HepAnyLightAntiQuarkMatcher</B></TT> matches any
// light antiquark.
// <LI><B><TT>HepMeVMassRangeMatcher&lt;minMass, maxMass&gt;</B></TT>
// matches any particle with a mass in MeV between the integer
// numbers given by <B><TT>minMass</B></TT> and <B><TT>maxMass</B></TT>
// </UL>.
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepParticleMatcher.h"

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

struct HepMatchAny {
  static HepString name() { return "AnyParticle"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean(pd.id());
  }
};
typedef HepDerivedMatcher<HepMatchAny> HepAnyParticleMatcher;
// Matches any particle.


struct HepMatchCharged {
  static HepString name() { return "ChargedParticle"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( pd.charge() != HepPDT::C0 && 
		      pd.charge() != HepPDT::Cunknown );
  }
};
typedef HepDerivedMatcher<HepMatchCharged> HepChargedParticleMatcher;
// Matches any charged particle.


struct HepMatchPositive {
  static HepString name() { return "PositiveParticle"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( HepPDT::positive(pd.iCharge()) );
  }
};
typedef HepDerivedMatcher<HepMatchPositive> HepPositiveParticleMatcher;
// Matches any positively charged particle.


struct HepMatchNegative {
  static HepString name() { return "NegativeParticle"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( HepPDT::negative(pd.iCharge()));
  }
};
typedef HepDerivedMatcher<HepMatchNegative> HepNegativeParticleMatcher;
// Matches any negatively charged particle.

struct HepMatchNeutral {
  static HepString name() { return "NeutralParticle"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean(pd.charge() == HepPDT::C0 );
  }
};
typedef HepDerivedMatcher<HepMatchNeutral> HepNeutralParticleMatcher;
// Matches any neutral particle.

struct HepMatchBaryon {
  static HepString name() { return "Baryon"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( (pd.id()/10)%10 && (pd.id()/100)%10 && (pd.id()/1000)%10 );
  }
};
typedef HepDerivedMatcher<HepMatchBaryon> HepBaryonMatcher;
// Matches any baryon

struct HepMatchMeson {
  static HepString name() { return "Meson"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( (pd.id()/10)%10 && (pd.id()/100)%10 && 
		       (pd.id()/1000)%10 == 0 );
  }
};
typedef HepDerivedMatcher<HepMatchMeson> HepMesonMatcher;
// Matches any baryon

struct HepMatchLepton {
  static HepString name() { return "Lepton"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean(abs(pd.id()) > 10 && abs(pd.id()) < 19 );
  }
};
typedef HepDerivedMatcher<HepMatchLepton> HepLeptonMatcher;
// Matches any lepton

struct HepMatchAnyLightQuark {
  static HepString name() { return "AnyLightQuark"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( pd.id() > 0 && pd.id() < 4 );
  }
};
typedef HepDerivedMatcher<HepMatchAnyLightQuark> HepAnyLightQuarkMatcher;
// Matches any light quark

struct HepMatchAnyLightAntiQuark {
  static HepString name() { return "AnyLightAntiQuark"; }
  static HepBoolean check(const HepParticleData & pd) {
    return HepBoolean( pd.id() < 0 && pd.id() > -4 );
  }
};
typedef HepDerivedMatcher<HepMatchAnyLightAntiQuark> HepAnyLightAntiQuarkMatcher;
// Matches any light antiquark

// <P></UL><HR><UL>
// Temporarily commented out due to g++ compiler problem
// template <int minMass, int maxMass>
// class HepMeVMassRangeMatcher: public HepParticleMatcher {

// public:
//   HepMeVMassRangeMatcher(const HepString & m)
//     : HepParticleMatcher(m) {}

//   virtual ~HepMeVMassRangeMatcher() {}

//   virtual HepBoolean checkMatch(const HepParticleData & pd ) const {
//     return matchCheck(pd);
//   }

//   static HepBoolean matchCheck(const HepParticleData & pd) {
//     return pd.mass() > HepDouble(minMass) && pd.mass() <= HepDouble(maxMass);
//   }
    
// };

#ifdef HEP_DEBUG_INLINE
#undef inline
#endif

#endif
