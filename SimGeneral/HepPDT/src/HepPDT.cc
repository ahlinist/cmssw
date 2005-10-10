// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:52 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// These are the implementations of the non-inlined memberfunctions
// of class HepPDT.

#include "SimGeneral/HepPDT/interface/HepPDT.h"

#ifdef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepPDT.icc"
#endif

#include "SimGeneral/HepPDT/interface/HepPDTable.h"
#include "SimGeneral/HepPDT/interface/HepDecayMode.h"

std::ostream & HepPDT::write(std::ostream & os) {
  return theTable().write(os);
}

std::istream & HepPDT::read(std::istream & is) {
  return theTable().read(is);
}

void HepPDT::readPDGFormat(istream & is) {
  theTable().readPDGFormat(is);
}

void HepPDT::init() {
  theTable().init();
}

const HepParticleData * HepPDT::getParticleData(HepLong id) {
  return theTable().getParticleData(id);
}

const HepParticleData * HepPDT::getParticleData(const HepString & name) {
  return theTable().getParticleData(name);
}

const HepParticleMatcher * HepPDT::getParticleMatcher(const HepString & name) {
  return theTable().getParticleMatcher(name);
}

const HepParticleData * HepPDT::getParticleData(HepLong id, HepPkg scheme) {
  return theTable().getParticleData(id, scheme);
}

const HepDecayMode * HepPDT::getDecayMode(const HepString & tag) {
  return theTable().getDecayMode(tag);
}

const HepPDT::ParticleMap & HepPDT::particles() {
  return theTable().particles();
}

const HepPDT::MatcherMap & HepPDT::matchers() {
  return theTable().matchers();
}

const HepPDT::PackageMap & HepPDT::packages() {
  return theTable().packages();
}

const HepPDT::ModeMap & HepPDT::modes() {
  return theTable().modes();
}

HepDouble HepPDT::
globalParameter(HepPkg pkg, const HepString & name) {
  return theTable().globalParameter(pkg, name);
}

HepPkg HepPDT::packageNumber(const HepString & package) {
  return theTable().packageNumber(package);
}

std::ostream & operator << (std::ostream & os, const HepPDTable & tab) {
  return tab.write(os);
}
  
std::istream & operator >> (std::istream & is, HepPDTable & tab) {
  return tab.read(is);
}

HepPDTable & HepPDT::theTable() {
  static HepPDTable table;
  return table;
}

HepString HepPDT::makeDecayTag(const HepDecayMode & dm) {
  HepString tag = dm.parent()->name() + ':';
  HepDecayMode::ParticleSet::const_iterator pit = dm.products().begin();
  while ( pit != dm.products().end() )
    tag += (**pit++).name() + ',';
  HepDecayMode::ModeSet::const_iterator dit = dm.cascadeProducts().begin();
  while ( dit != dm.cascadeProducts().end() )
    tag += '[' + (**dit++).tag() + "],";
  HepDecayMode::MatcherSet::const_iterator mit = dm.productMatchers().begin();
  while ( mit != dm.productMatchers().end() )
    tag += '?' + (**mit++).name() + ',';
  if ( dm.wildProductMatcher() )
    tag += '*' + dm.wildProductMatcher()->name() + ',';
  pit = dm.excluded().begin();
  while ( pit != dm.excluded().end() )
    tag += '!' + (**pit++).name() + ',';
  tag[tag.size()-1] = ';';
  return tag;
}

