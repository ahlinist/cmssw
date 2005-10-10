// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:52 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepDecayTable.

#include "Utilities/Configuration/interface/Architecture.h"
#include "GeneratorInterface/HepPDT/interface/HepDecayTable.h"
#include "GeneratorInterface/HepPDT/interface/HepDecayMode.h"

#ifdef HEP_DEBUG_INLINE
#include "GeneratorInterface/HepPDT/interface/HepDecayTable.icc"
#endif

void HepDecayTable::init() {
  HepPkg npac = -1;
  iterator it = basemap::begin();
  while ( it != basemap::end() ) {
    (*it).second->resetOverlap();
    const_iterator oit = begin();
    while ( oit != end() ) {
      if ( const_iterator(it) != oit )
	(*it).second->addOverlap(*((*oit).second));
      ++oit;
    }
    HepDecayMode::ModelMap::const_iterator mit = (*it).second->models.begin();
    while ( mit != (*it).second->models.end() )
      npac = max(npac, (*mit++).first);
    ++it;
  }
  sumFractions = vector<HepDouble>(npac+1, 0.0);
  sumTree = SumMap(npac+1, DoubleDecayMap());
  it = basemap::begin();
  while ( it != basemap::end() ) {
    HepDecayMode::ModelMap::const_iterator mit = (*it).second->models.begin();
    while ( mit != (*it).second->models.end() ) {
      if ( (*mit).second.second > 0.0 ) {
	sumFractions[(*mit).first] += (*mit).second.second;
	sumTree[(*mit).first][sumFractions[(*mit).first]] = (*it).second;
      }
      ++mit;
    }
    ++it;
  }
}

const HepDecayMode * HepDecayTable::
selectMode(HepDouble r, HepPkg package) const {
  if ( empty() || package >= HepPkg(sumFractions.size()) || package < 0 )
    return 0;
  HepDouble sum = r * sumFractions[package];
  if ( sum <= 0.0 ) return 0;
  const DoubleDecayMap::const_iterator it = sumTree[package].upper_bound(sum);
  return it == sumTree[package].end()? 0: (*it).second;
}

// const HepDecayMode * HepDecayTable::
// selectMode(HepDouble r, HepPkg package) const {
//   if ( empty() || package >= sumFractions.size() ) return 0;
//   HepDouble sum = r * sumFractions[package];
//   if ( sum <= 0.0 ) return 0;
//   const HepDecayMode * last = 0;
//   const_iterator it = begin();
//   while ( it != end() ) {
//     HepDouble bri = (*it)->exclusive(package);
//     sum -= bri;
//     if ( sum <= 0.0 ) return *it;
//     if ( bri > 0.0 ) last = *it;
//     ++it;
//   }
//   return last;
// }
