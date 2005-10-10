// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2003/07/21 08:54:07 $
//  $Revision: 1.6 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepParticleMatcher_H_INCLUDED
#define HepParticleMatcher_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepParticleMatcher</B></TT> class.
//
// <B><TT>HepParticleMatcher</B></TT> is an abstract base class used
// to represent groups of <B><TT>HepParticleData</B></TT> objects. It
// contains an <B><TT>std::set</B></TT> of pointers to <I>matched</I>
// <B><TT>HepParticleData</B></TT> objects and an
// <B><TT>std::set</B></TT> of pointers to other
// <B><TT>HepParticleMatcher</B></TT> objects which matches a subset
// of the matched <B><TT>HepParticleData</B></TT> objects.
//
// A derived class should implement at least two methods:
// <UL>
// <LI><B><TT>matchCheck(const HepParticleData &)</B></TT> should be a
// static method returning true if the HepParticleData object be
// included among the matched particles.
// <LI><B><TT>checkMatch(const HepParticleData &)</B></TT> must return
// the result of the static <B><TT>matchCheck()</B></TT> method.
// </UL>
// In addition, the constructor must call the constructor of
// <B><TT>HepParticleMatcher</B></TT> class giving a string to be used
// as identfier for the derived matcher class.
//
// To simplified the implementation of derived classes a macro called
// <B><TT>HEP_PARTICLE_MATCHER</B></TT> is defined whose argument are
// the name of the derived class, the name used as the identifier, and
// an identifier used as argument for the static
// <B><TT>matchCheck()</B></TT> method. The use of the macro must be
// followed by the implementation of the <B><TT>matchCheck()</B></TT>
// method encosed in curly brackets using the given identifier. See
// example below. There is also a templated helper class called
// HepDerivedMatcher for the same purpose. Again see the example below.
//
//
// When a object of a derived class is added to
// <B><TT>HepPDTable</TT></B>, it will be presented to all
// <B><TT>HepParticleData</TT></B> objects via the
// <B><TT>checkMatch()</TT></B> method and the matched particles will be
// added to the list in the base class.
//
// If all the matched particles have common properties, the
// <B><TT>HepParticleMatcher</TT></B> objects will also have these
// properties accessible with the <B><TT>mass()</TT></B>,
// <B><TT>width()</TT></B>, <B><TT>cTau()</TT></B>,
// i<B><TT>Charge()</TT></B>, <B><TT>iSpin()</TT></B>,
// <B><TT>iColor()</TT></B> and <B><TT>stable()</TT></B> methods.
//
// CLASSDOC SUBSECTION Examples:
//
// A particle matcher class matching any lepton may be implemented
// with the <B><TT>HEP_PARTICLE_MATCHER</TT></B> as follows
//
// <B><TT>HEP_PARTICLE_MATCHER(HepLeptonMatcher,"Lepton",pd) {<BR>
//     &nbsp;return HepBoolean( abs(pd.id()) > 10 && abs(pd.id()) < 19 );<BR>
//   }<BR></TT></B>
//
// This class can also be implemented with the templated
// HepDerivedMatcher class as follows:
//
// <B><TT>struct MatchLeptons {<BR>
// &nbsp;static HepString name () { return "Lepton";}<BR>
// &nbsp;static HepBoolean check(const HepParticleData & pd) {<BR>
// &nbsp;&nbsp;return HepBoolean( abs(pd.id()) > 10 && abs(pd.id()) < 19 );<BR>
// &nbsp;}<BR>
// };<BR>
// typedef HepDerivedMatcher&lt;MatchLeptons&gt; HepLeptonMatcher;<BR></TT></B>
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:PDT.html">PDT.h</a>
// <a href="http:PDTable.html">PDTable.h</a>
// <a href="http:ParticleData.html">ParticleData.h</a>
// <a href="http:StandardMatchers.html">StandardMatchers.h</a>
// <a href="http:CombinedMatcher.html">CombinedMatcher.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepParticleData.h"
#include <set>

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

namespace std {
// CLASSDOC OFF
template <>
struct less<const HepParticleData *> {
  HepBoolean operator() (const HepParticleData * x,
			 const HepParticleData * y) const
  { return x->id() < y->id(); }
};

template <>
struct less<const HepParticleMatcher *> {
  inline HepBoolean operator()(const HepParticleMatcher * x,
			       const HepParticleMatcher * y) const;
};
} // end namespace std
// CLASSDOC ON

// <P></UL><HR><UL>
class HepParticleMatcher {

public:

  typedef std::set<const HepParticleData *,
              std::less<const HepParticleData *> > ParticleSet;
  typedef std::set<const HepParticleMatcher*,
              std::less<const HepParticleMatcher*> > MatcherSet;
  friend class HepPDTable;

public:

  inline virtual ~HepParticleMatcher();
  // The destructor. Note that the only (default) constructor is
  // protected.

  inline const HepString & name() const;
  // Return the name of this matcher

  virtual HepBoolean checkMatch(const HepParticleData &) const = 0;
  // Check if a particle meets the criteria. To be implemented by
  // derived classes where it should call the static function
  // matchCheck(const HepParticleData &).

  inline HepBoolean matches(const HepParticleData &) const;
  // Check if a given particle belongs to the set of matches
  
  inline HepBoolean matches(const HepParticleMatcher &) const;
  // Check if a particle matcher belongs to the set of matches.

  inline const ParticleSet & particles() const;
  inline const MatcherSet & matchers() const;
  inline ParticleSet::const_iterator begin() const;
  inline ParticleSet::const_iterator end() const;
  inline ParticleSet::const_iterator
    find(const HepParticleData *) const;
  // Access to the map of matching particle and matchers. Also access
  // the begin, end and find methods of the set of matching particles.

  inline HepDouble mass() const;
  // Returns the common mass of the matching particles. If all matching
  // particles do not have exactly the same mass -1.0 is returned.

  inline HepDouble width() const;
  // Returns the common width of the matching particles. If all matching
  // particles do not have exactly the same width -1.0 is returned.

  inline HepDouble cTau() const;
  // Returns the common decay length of the matching particles. If all
  // matching particles do not have exactly the same decay length -1.0
  // is returned.

  inline HepPDT::HepCharge iCharge() const;
  inline HepBoolean charged() const;
  inline HepBoolean positive() const;
  inline HepBoolean negative() const;
  // If all matching particles have the same charge the common charge
  // is returned. Otherwise if all are positive (negative),
  // HepPDT::Cpositive (HepPDT::Cnegative) is returned. Otherwise if
  // all are charged, HepPDT::Ccharged is returned. Otherwise
  // HepPDT::Cundefined is returned.

  inline HepPDT::HepSpin iSpin() const;
  // If all matching particles have the same spin the common spin is
  // returned. Otherwise HepPDT::Sundefined is returned.

  inline HepPDT::HepColor iColor() const;
  inline HepBoolean colored() const;
  // If all matching particles have the same color the common color is
  // returned. Otherwise if all are colored, HepPDT::colcolored is
  // returned. Otherwise HepPDT::colundefined is returned.

  inline HepInt stable() const;
  // Returns (0)1 if all matching particles are (un)stable. Otherwise
  // -1 is returned.

  inline const HepParticleMatcher * CC() const;
  HepBoolean CC(HepParticleMatcher &);
  // Get/set a matcher object matching the antiparticles of this. If
  // no-one exists 0 is returned by the get function. If the argument
  // of the set functions does not match the anti-particles of this,
  // false is returned..

  HepBoolean checkCC(const HepParticleMatcher &) const;
  // Check if a given matcher matches all the antiparticles of this,
  // but do not do anything.

  void init();
  // Used for particle matchers not handled by the HepPDTable object
  // to initialize the list of matching particles.

protected:

  void addIfMatch(const HepParticleData &);
  void addIfMatch(const HepParticleMatcher &);
  // Add a particle or a particle matcher id to the set of matching
  // particles if it meets the criteria.

  inline HepParticleMatcher(const HepString &);
  // Constructor only used by derived objects.

  inline void clear();
  // Clear information about matching particles and matchers.

private:

  HepString idString;
  // A character string for identification.

  ParticleSet matchingParticles;
  // The set of particle data objects matched by this matcher.

  MatcherSet matchingMatchers;
  // A set of matchers which matches a subset of this matcher.

  HepDouble commonMass;
  HepDouble commonWidth;
  HepDouble commonCTau;
  HepPDT::HepCharge commonCharge;
  HepPDT::HepSpin commonSpin;
  HepPDT::HepColor commonColor;
  HepInt commonStable;
  // Variables containing common properties of all matched
  // particles. All are set to a sensible 'undefined' value if the
  // properties differs between the matched particles.

  const HepParticleMatcher * antiPartner;
  // Pointer to a matcher object which matches all anti particles
  // which are matched by this matcher.

};

template <class T>
class HepDerivedMatcher: public HepParticleMatcher {
  public:
  HepDerivedMatcher(const HepString & m = T::name())
    : HepParticleMatcher(m) {}
  virtual ~HepDerivedMatcher() {} 
  virtual HepBoolean checkMatch(const HepParticleData & pd ) const { 
    return matchCheck(pd);                                           
  }                                                                  
  static HepBoolean matchCheck(const HepParticleData & pd) {
    return T::check(pd);
  }
};

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepParticleMatcher.icc"
#else
#undef inline
#endif

#endif
