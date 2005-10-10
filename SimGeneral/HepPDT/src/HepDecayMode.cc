// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:52 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepDecayMode.

#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/HepPDT/interface/HepDecayMode.h"

#ifdef HEP_DEBUG_INLINE
#include "GeneratorInterface/HepPDT/interface/HepDecayMode.icc"
#endif

HepBoolean HepDecayMode::addOverlap(const HepDecayMode & d) {
  HepBoolean inc = includes(d);
  if ( !inc ) return false;
  vector<const HepDecayMode *>::iterator it = overlappingModes.begin();
  while ( it != overlappingModes.end() ) if ( *it++ == &d ) return true;
  overlappingModes.push_back(&d);
  return true;
}

HepBoolean HepDecayMode::includes(const HepDecayMode & d) const {

  // Fast check for ordinary decay modes
  if ( cascadeDecayProducts.empty() && decayProductMatchers.empty() &&
       excludedIntermediateProducts.empty() &&  !wildDecayProductMatch &&
       d.cascadeDecayProducts.empty() &&  d.decayProductMatchers.empty() &&
       d.excludedIntermediateProducts.empty() &&  !d.wildDecayProductMatch &&
       decayProducts != d.decayProducts ) return false;

  // First check that none of the excluded products in this are
  // present in the other.
  ParticleSet::const_iterator pit = excluded().begin();
  while ( pit != excluded().end() ) {
    ParticleSet::const_iterator pit2 = d.products().find(*pit);
    if ( pit2 != d.products().end() ) return false;
    ModeSet::const_iterator mit = d.cascadeProducts().begin();
    while ( mit != d.cascadeProducts().end() )
      if ( *pit == (**mit++).parent() ) return false;
    ++pit;
  }

  // Check that all cascade decays in this overlaps with one in the
  // other. Save the ones that are left
  ModeSet cascleft = d.cascadeProducts();
  ModeSet::const_iterator mit = cascadeProducts().begin();
  while ( mit != cascadeProducts().end() ) {
    ModeSet::iterator mit2 = cascleft.begin();
    while ( mit2 != cascleft.end() && !(**mit).includes(**mit2) ) ++mit2;
    if ( mit2 == cascleft.end() ) return false;
    ++mit;
  }


  // Check that all cascade product parents in the other matches
  // something in this. Otherwise expand the cascade product.
  ParticleSet partleft = d.products();
  MatcherSet matchleft = d.productMatchers();
  ParticleSet excludeleft = d.excluded();
  MatcherSet wildleft;
  if ( d.wildProductMatcher() ) wildleft.insert(wildProductMatcher());
  ParticleSet part = products();
  MatcherSet match = productMatchers();
  while ( cascleft.size() ) {
    ModeSet::iterator cdmit = cascleft.begin();
    const HepDecayMode * cdm = *cdmit;
    ParticleSet::iterator pit = part.find(cdm->parent());
    if ( pit != part.end() ) {
      cascleft.erase(cdmit);
      part.erase(pit);
    } else {
      MatcherSet::iterator mit = match.begin();
      while ( mit != match.end() &&
	      !(**mit).matches(*(cdm->parent())) ) ++mit;
      if ( mit != match.end() ) {
	cascleft.erase(cdmit);
	match.erase(mit);
      } else {
	if ( wildProductMatcher() &&
	     wildProductMatcher()->matches(*(cdm->parent())) ) {
	  cascleft.erase(cdmit);
	} else {
	  cascleft.erase(cdmit);
	  ParticleSet::const_iterator pit2 = cdm->products().begin();
	  while ( pit2 != cdm->products().end() ) partleft.insert(*pit2++);
	  MatcherSet::const_iterator pmit2 = cdm->productMatchers().begin();
	  while ( pmit2 != cdm->productMatchers().end() )
	    matchleft.insert(*pmit2++);
	  if ( cdm->wildProductMatcher() )
	    wildleft.insert(cdm->wildProductMatcher());
	  pit2 = cdm->excluded().begin();
	  while ( pit2 != cdm->excluded().end() ) excludeleft.insert(*pit2++);
	  ModeSet::const_iterator mit2 = cdm->cascadeProducts().begin();
	  while ( mit2 != cdm->cascadeProducts().end() )
	    cascleft.insert(*mit2++);
	}
      }
    }
  }

  // Check that all excluded left in the other are absent in this.
  pit = excludeleft.begin();
  while ( pit != excludeleft.end() ) {
    ParticleSet::const_iterator pit2 = part.begin();
    while ( pit2 != part.end() )
      if ( *pit == *pit2++ ) return false;
    ++pit;
  }

  // Now all particles and matches left in this must match something
  // in the other.
  pit = part.begin();
  while ( pit != part.end() ) {
    ParticleSet::iterator pit2 = partleft.find(*pit++);
    if ( pit2 == partleft.end() ) return false;
    partleft.erase(pit2);
  }
  MatcherSet::const_iterator pmit = match.begin();
  while ( pmit != match.end() ) {
    ParticleSet::iterator pit2 = partleft.begin();
    while ( pit2 != partleft.end() && ! (**pmit).matches(**pit2) ) ++pit2;
    if ( pit2 != partleft.end() ) {
      partleft.erase(pit2);
    } else {
      MatcherSet::iterator pmit2 = matchleft.begin();
      while ( pmit2 != matchleft.end() && ! (**pmit).matches(**pmit2) ) ++pmit2;
      if ( pmit2 != matchleft.end() ) {
	matchleft.erase(pmit2);
      } else
	return false;
    }
  }

  // Now all particles and matchers left in the other must be matched
  // by the wild match in this.
  if ( wildProductMatcher() ) {
    pit = partleft.begin();
    while ( pit != partleft.end() )
      if ( !(wildProductMatcher()->matches(**pit++)) ) return false;
    pmit = matchleft.begin();
    while (pmit != matchleft.end() )
      if ( !(wildProductMatcher()->matches(**pmit++)) ) return false;
    pmit = wildleft.begin();
    while (pmit != wildleft.end() )
      if ( !(wildProductMatcher()->matches(**pmit++)) ) return false;
  } else
    return partleft.empty() && matchleft.empty() && wildleft.empty();
  return true;
}

