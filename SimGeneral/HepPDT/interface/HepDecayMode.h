// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2003/07/21 08:52:18 $
//  $Revision: 1.5 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepDecayMode_H_INCLUDED
#define HepDecayMode_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepDecayMode</B></TT> class.
//
// The <B><TT>HepDecayMode</B></TT> class defines one possible decay
// of a particle.  It is designed to be able to handle most of the
// measured decay channels published by the ParticleData Group and the
// specification of different decay products is therefore fairly
// complex.
//
// An object of the <B><TT>HepDecayMode</B></TT> class
// contains the following
// <UL>
// <LI> a const pointer to a HepParticleData object corresponding to
// the decaying particle.
// <LI> a measured branching ratio with errors.
// <LI> a <B><TT>std::multiset</B></TT> of const pointers to
// HepParticleData objects corresponding to uniquely identified decay
// products.
// <LI> a std::multiset of const pointers to
// <B><TT>HepDecayMode</B></TT> objects corresponding to a subsequent
// decay of a uniquely identified resonant decay product (which is
// then not listed in the set of <B><TT>HepParticleData</B></TT>
// objects above).
// <LI> a <B><TT>std::set</B></TT> of <B><TT>HepParticleData</B></TT>
// objects corresponding to resonances which are excluded as
// intermediate decay products.
// <LI> a <B><TT>std::multiset</B></TT> of const pointers to
// HepParticleMatcher objects, each corresponding to one decay product
// identified to be one of the particles matched.
// <LI> a const pointer to a <B><TT>HepParticleMatcher</B></TT> object
// corresponding to an arbitrary number of decay products, all
// identified to be among the matched particles.
// </UL>
//
// Since there may be some overlap between different decay modes for a
// given particle, the <B><TT>HepDecayMode</B></TT> object also
// includes a list of other decay modes which are included as a
// special case of the current one.
//
// The object also contains an <B><TT>std::string</B></TT> tag which
// uniquely defines the decay mode. This string is made up from the
// name of the decaying particle, followed by a colon, followed by a
// comma-separaterd list of decay products specifiers ended by a
// semi-colon. The tag must not contain any white-space characters.
// The decay mode specifiers should be given in the following order:
// <UL>
// <LI> The names of the uniquely identified decay products in
// the order of their STDHEP numbers.
// <LI> The tags of the decay modes corresponding to subsequent
// decays of  uniquely identified resonant decay products, enclosed
// in square brackets and ordered lexicographically.
// <LI> The names of the <B><TT>HepParticleMatcher</B></TT> objects
// which each correspond to one decay product, preceeded by a '?' in
// lexicographical order.
// <LI> The name of the <B><TT>HepParticleMatcher</B></TT> object
// corresponding to an unknown number of decay products preceeded by
//  a '*'.
// <LI> The names of the excluded intermediate resonances
// preceeded by a '!' in the order of their STDHEP numbers.
// </UL>
//
// Finally, a <B><TT>HepDecayMode</B></TT> object contains a
// <B><TT>std::map</B></TT> associating
// <B><TT>HepPackageHandle</B></TT> numbers with pointers to
// <B><TT>HepDecayer</B></TT> objects paired with a branching ratio.
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:PDT.html">PDT.h</a>
// <a href="http:PDTable.html">PDTable.h</a>
// <a href="http:ParticleData.html">ParticleData.h</a>
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
// <a href="http:PackageHandler.html">PackageHandler.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepParticleMatcher.h"
#include <vector>
#include <set>

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

// CLASSDOC OFF
namespace std {
template<>
struct less<const HepDecayMode *> {
  inline HepBoolean operator () (const HepDecayMode *,
				 const HepDecayMode *) const;
};
} // end of namespace std
// CLASSDOC ON

class HepDecayModel;

// <P></UL><HR><UL>
class HepDecayMode {

public:

  typedef std::map<HepPkg, pair<HepDecayModel *, HepDouble>,
    std::less<HepPkg> > ModelMap;
  typedef std::multiset<const HepParticleData *,
    std::less<const HepParticleData *> > ParticleSet;
  typedef std::multiset<const HepParticleMatcher *,
    std::less <const HepParticleMatcher *> > MatcherSet;
  typedef std::multiset<const HepDecayMode *,
    std::less<const HepDecayMode *> > ModeSet;
  //Convenient typedefs.

  friend class HepDecayTable;
  friend class HepPDTable;

public:

  /// The constructor.
  inline HepDecayMode();

  /// The destructor.
  inline ~HepDecayMode();

  /// Return the tag for this decay mode.
  inline const HepString & tag() const;

  /// Return the tag for this decay mode.
  inline const HepString & tag(const HepString &);

  /// Add identified decay products.
  inline void addProduct(const HepParticleData &);

  /// Add identified resonant product with specified decay mode.
  inline void addCascadeProduct(const HepDecayMode &);

  /// Add a mathcer corresponding to one decay product.
  inline void addProductMatcher(const HepParticleMatcher &);

  /// Add a matcher corresponding to any number of decay products.
  inline void setWildMatcher(const HepParticleMatcher &);

  /* Add a particle corresponding to an excluded intermediate
   *  resonance. */
  inline void addExcluded(const HepParticleData &);

  /* Set a pointer to the particle data object corresponding to
   * the decaying particle. */
  inline void parent(const HepParticleData &);
  /* Get a pointer to the particle data object corresponding to
   * the decaying particle. */
  inline const HepParticleData * parent() const;

  /// The set of identified decay products.
  inline const ParticleSet & products() const;

  /// The set of identified resonant products with specified decay modes
  inline const ModeSet & cascadeProducts() const;

  /// The set of matchers each corresponding to one decay product.
  inline const MatcherSet & productMatchers() const;

  /// The pointer to a matcher corresponding to any number of decay products
  inline const HepParticleMatcher * wildProductMatcher() const;

  /// The set particles corresponding to excluded intermediate resonances.
  inline const ParticleSet & excluded() const;

  inline HepDouble bRatio(HepDouble, HepDouble = 0.0,
			  HepDouble = -1.0);
  inline HepDouble bRatio() const;
  inline HepDouble bRatioErrPos() const;
  inline HepDouble bRatioErrNeg() const;
  // Set/return the measured branching ratio. Arguments for the set
  // method are the branching ratio, the positive and negative
  // error. If the negative error is left out, it is set to the
  // positive error.

  inline HepDouble bRatio(HepPkg, HepDouble);
  inline HepDouble bRatio(HepPkg) const;
  // Set/return the bRatio branching ratio to be used for by a given
  // package number.

  inline void switchOn(HepPkg = HepPkgDefault);
  inline void switchOff(HepPkg = HepPkgDefault);
  // Switch on/off this decay mode for a given package number.

  HepBoolean includes(const HepDecayMode &) const;
  // Check if another decay mode is included in this one.

  inline HepBoolean operator == (const HepDecayMode &) const;
  // Check if another decay mode has the same final state as this
  // one.

  inline const std::vector<const HepDecayMode *> & overlap() const;
  // Return the list of overlapping decay modes.

  inline void addModel(HepPkg, HepDecayModel &,
		       HepDouble = -1.0);
  inline HepDecayModel * model(HepPkg = HepPkgDefault) const;
  // Add/return a pointer to an object corresponding to given package
  // number capable of performing the decay. Optionally also set the
  // branching ratio to be used by this package.

  inline void setDefaultModel(HepPkg);
  inline HepPkg defaultModel() const;
  // Set/get the default decay model.

private:

  HepBoolean addOverlap(const HepDecayMode &);
  // Add a decay mode to the list of overlapping modes if included.

  inline void resetOverlap();
  // Remove all decay modes from the list of overlapping modes.

private:

  HepString nameTag;
  // The tag.

  const HepParticleData * decayingParticle;
  // Pointer to a particle data object corresponding to the decaying
  // particle.

  HepDouble brat;
  HepDouble dbratp;
  HepDouble dbratn;
  // The measured branching fraction for this decay mode.

  ParticleSet decayProducts;
  // The set of specified decay particles.

  ModeSet cascadeDecayProducts;
  // The set of matching decay channels corresponding to a specified
  // particle with a specified subsequent decay mode.

  MatcherSet decayProductMatchers;
  // The set of matching decay products. Each of the matchers
  // correspond to one particle.

  const HepParticleMatcher * wildDecayProductMatch;
  // A particle matcher which corresponds to zero or more particles.

  ParticleSet excludedIntermediateProducts;
  // A set of particles which are not allowed as intermediate
  // resonances.

  std::vector<const HepDecayMode *> overlappingModes;
  // A list of decay modes which are included in this one.

  ModelMap models;
  // A map relating package numbers (related to package names such as
  // "G4" or "Jetset" in HepPDTable) to a pointer to an object which
  // is able to perform the actual decay and the exclusive branching
  // ratio used for the package.

  HepPkg defmod;
  // The number of the default decay model.

};

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepDecayMode.icc"
#else
#undef inline
#endif

#endif
