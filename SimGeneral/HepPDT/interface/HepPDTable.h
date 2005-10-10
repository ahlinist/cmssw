// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2004/06/02 09:47:59 $
//  $Revision: 1.3 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepPDTable_H_INCLUDED
#define HepPDTable_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepPDTable</B></TT> class. 
//
// The <B><TT>HepPDTable</B></TT> class implements the actual particle
// property data base in CLHEP. <B><TT>HepPDTable</B></TT> handles all
// <B><TT>HepParticleData</B></TT>,
// <B><TT>HepParticleMatcher</B></TT>, <B><TT>HepDecayMode</B></TT>,
// and <B><TT>HepPackageHandler</B></TT> objects to be used in a
// program.
//
// The main member is the <B><TT>std::map</B></TT> of pointers to all
// <B><TT>HepParticleData</B></TT> objects indexed by their STDHEP id
// numbers. Pointers to <B><TT>HepParticleData</B></TT> objects can be
// retrieved by their STDHEP id number, but also by its name or
// according to a user defined numbering scheme used by a
// package. Pointers to <B><TT>HepParticleMatcher</B></TT> objects can
// similarly be retrieved by their name.
//
// <B><TT>HepPDTable</B></TT> is not persistent. Instead it can be
// read and written to a file using a specified ascii format. Since
// neither the <B><TT>HepParticledata</B></TT> or
// <B><TT>HepDecayMode</B></TT> classes are polymorphic, all standard
// particle properties and decay modes can easily be handled this
// way. User defined <B><TT>HepParticleMatcher</B></TT> objects,
// however, are polymorphic and must therefore be instantiated and
// added to the <B><TT>HepDTable</B></TT> before any files are read
// where the name of the objects is encountered. In the same way, all
// <B><TT>HepPackageHandler</B></TT> objects must be instantiated and
// added to <B><TT>HepPDTable</B></TT> by hand and these objects must
// be able to instanciate all <B><TT>HepPatricleProducer</B></TT> and
// <B><TT>HepDecayModel</B></TT> objects stored in a file.
//
// All <B><TT>HepParticleData</B></TT>,
// <B><TT>HepParticleMatcher</B></TT>, <B><TT>HepDecayMode</B></TT>
// and <B><TT>HepPackageHandler</B></TT> objects added to
// <B><TT>HepPDTable</B></TT> must be allocated by new and will be
// deleted by <B><TT>HepPDTable</B></TT> upon destruction.
//
// In addition <B><TT>HepPDTable</B></TT> handles parameters used by
// different packages which are accessed by their package id number
// and a package specific name.
//
// After any changes has been made to <B><TT>HepPDTable</B></TT>, the
// <B><TT>init()</B></TT> method must be called to set up all cross
// references and check the consistency.
//
// CLASSDOC SUBSECTION See also:

// <a href="http:PDT.html">PDT.h</a>
// <a href="http:ParticleData.html">ParticleData.h</a>
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
// <a href="http:DecayMode.html">DecayMode.h</a>
// <a href="http:PackageHandler.html">PackageHandler.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepParticleMatcher.h"
#include "SimGeneral/HepPDT/interface/HepPackageHandler.h"
#include <map>

#ifdef HEP_DEBUG_INLINE
#define inline
#endif
#include <iosfwd>


// <P></UL><HR><UL>
class HepPDTable {

  friend class HepPDT;

public:

  typedef std::map<HepLong, HepParticleData *,
              std::less<HepLong> > ParticleMap;
  typedef std::map<HepString, HepParticleMatcher *,
              std::less<HepString> > MatcherMap;
  typedef std::map<HepPkg, HepPackageHandler *,
              std::less<HepPkg> > PackageMap;
  typedef std::map<HepString, HepDecayMode *,
              std::less<HepString> > ModeMap;
  typedef std::map<HepString, HepLong,
              std::less<HepString> > NameMap;
  typedef std::map<HepLong, HepLong,
              std::less<HepLong> > LongLongMap;
  typedef std::map<HepPkg, LongLongMap,
              std::less<HepPkg> > TranslationMap;
  typedef std::map<HepString, HepPkg,
              std::less<HepString> > PackageNameMap;
  typedef std::map<HepString, HepDouble,
              std::less<HepString> > ParameterMap;
  typedef std::map<HepPkg, ParameterMap,
              std::less<HepPkg> > PackageParameterMap;

private:

  HepPDTable();
  // The default constructor. Can only be called from the HepPDT class.

  HepDecayMode * constructDecayMode(HepString &);

public:

  ~HepPDTable();
  // The destructor.

  void init();
  // Initialize the table, set up all cross references correctly. This
  // is done automatically after a read call and shoulc be done
  // manually if any information is changed in the table.

  std::ostream & write(std::ostream &) const;
  std::istream & read(std::istream &);
  void readPDGFormat(std::istream &);
  // Read and write methods. The first two uses the internal ascii
  // format, while readPDGFormat reads the machine readable files
  // provided by the Particle Data Group. Note that for objects of
  // HepParticleMatcher and HepPackageHandler, only their name is
  // written, therefore the necessary objects must be added to the
  // HepPDTable before anything is read.

  void add(HepParticleData &);
  void add(HepParticleMatcher &);
  HepPkg add(HepPackageHandler &);
  // Add particle data, particle matcher or a package handler object
  // to the PDG table. The objects must have been allocated with new
  // and will be deleted together with the PDTable. If an object with
  // the same id is found it will be deleted and replaced by this one.

  inline const HepParticleData *
    getParticleData(HepLong) const;
  inline const HepParticleProducer *
    getParticleProducer(HepPkg, HepLong) const;
  inline const HepParticleData *
    getParticleData(const HepString &) const;
  inline const HepParticleProducer *
    getParticleProducer(const HepString &) const;
  inline HepParticleData * getParticleData(HepLong);
  inline HepParticleData * getParticleData(const HepString &);
  inline const HepParticleMatcher *
    getParticleMatcher(const HepString &) const;
  inline HepParticleMatcher * getParticleMatcher(const HepString &);
  // Retreve a particle data or particle matcher object corresponding
  // to the STDHEP id or character string name. Returns 0 if no
  // corresponding object is found. For the particle data method, the
  // character string is first translated into a number and the
  // particle data object with the corresponding id is returned if
  // found. For the particle producer method, the id number is first
  // translated into a STDHEP number, the correspondig particle data
  // object then returns the producer object corresponding to the
  // package number.

  inline HepDecayMode * getDecayMode(const HepString &);
  inline const HepDecayMode *
    getDecayMode(const HepString &) const;
  // Return a decay mode corresponding to the given string tag. If no
  // such decay mode exists the non-const method will create one (and
  // returns 0 on failior), while the second returns 0. If the given
  // string is inconsistent, eg. the product specifications are in the
  // wrong order, the const method will return 0 while the non-const
  // one will try to reconstruct a correct tag.

  inline const HepParticleData *
    getParticleData(HepLong, HepPkg) const;
  // Retrieve a particle data object according to a id of a numbering
  // scheme used by the package indicated by the second  argument.

  inline const ParticleMap & particles() const;
  inline const MatcherMap & matchers() const;
  inline const PackageMap & packages() const;
  inline const ModeMap & modes() const;
  // Access to all stuff within the table.

  inline HepPkg packageNumber(const HepString &) const;
  // Return the number used internally for the given package name.

  inline HepPackageHandler *
    packageHandler(const HepString &) const;
  inline HepPackageHandler * packageHandler(HepPkg) const;
  // Return a package handle corresponding to the given name or
  // number.

  inline HepDouble globalParameter(HepPkg, const HepString &) const;
  inline void globalParameter(HepPkg, const HepString &, HepDouble);
  // Get/set a global parameter for a given package and a given name.

  void dumpEnumNames(std::ostream &) const;
  // Take all particle names and translate them to something that can
  // be used as a c++ identifier and write a file defining an enum
  // assiciating each identifier with the corresponding STDHEP id
  // number.

private:

  ParticleMap allParticles;
  // A map of pointers to all HepParticleData objects indexed by their
  // STDHEP id numbers.

  MatcherMap allMatchers;
  // A map of pointers to all HepParticleMatcher objects indexed by
  // their names.

  PackageMap allPackages;
  // A map of pointers to all HepPackageHandler objects indexed by
  // their given id number.

  ModeMap allModes;
  // A map of pointers to all HepDecayMode objects indexed by their
  // tag.

  NameMap mapName2Stdhep;
  // A map of all STDHEP id numbers indexed by the standard name of
  // the corresponding particle.

  TranslationMap map2stdhep;
  // A map of a map of STDHEP id numbers indexed by a corresponding
  // number of an alternative numbering scheme indexed by the package
  // id number with the alternative numbering schene.

  PackageNameMap packageNumbers;
  // A map of all package name id numbers indexed by the corresponding
  // name.

  PackageParameterMap globalParameters;
  // A map of a map of parameter values indexed by the corresponding
  // parameter name indexed by the package number.

};

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepPDTable.icc"
#else
#undef inline
#endif

#endif
