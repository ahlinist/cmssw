// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2003/01/06 13:01:52 $
//  $Revision: 1.2 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepJetsetDummyHandler_H_INCLUDED
#define HepJetsetDummyHandler_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the
// <B><TT>HepJetsetDummyHandler</B></TT>,
// <B><TT>HepJetsetLUDECYDummy</B></TT> and
// <B><TT>HepJetsetProducerDummy</B></TT> classes.
//
// This is not a real package, but may be considered an embryo for a
// package inerfacing Jetset particle production and decay to the
// CLHEP particle data table. It is provided as an illustration of how
// to create packages and includes the most important points.
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:PDT.html">PDT.h</a>
// <a href="http:PDTable.html">PDTable.h</a>
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
// <a href="http:PackageHandler.html">PackageHandler.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepPackageHandler.h"
#include "SimGeneral/HepPDT/interface/HepJetsetLUDECYDummy.h"
#include "SimGeneral/HepPDT/interface/HepJetsetProducerDummy.h"
#include <iostream>

// <P>Finally we declare the <B><TT>HepJetsetDummyHandler</B></TT> to
// handle the creation and destruction of
// <B><TT>HepJetsetProducerDummy</B></TT> and
// <B><TT>HepJetsetLUDECYDummy</B></TT> objects.
class HepJetsetDummyHandler: public HepPackageHandler {

public:

  typedef map<HepString, HepJetsetLUDECYDummy *, less<HepString> > ModelMap;

public:
  HepJetsetDummyHandler(): theName("JETSET") {
    models["LUDECY000"] = new HepJetsetLUDECYDummy(0);
    models["LUDECY001"] = new HepJetsetLUDECYDummy(1);
    models["LUDECY002"] = new HepJetsetLUDECYDummy(2);
    models["LUDECY003"] = new HepJetsetLUDECYDummy(3);
    models["LUDECY004"] = new HepJetsetLUDECYDummy(4);
    models["LUDECY011"] = new HepJetsetLUDECYDummy(11);
    models["LUDECY012"] = new HepJetsetLUDECYDummy(12);
    models["LUDECY013"] = new HepJetsetLUDECYDummy(13);
    models["LUDECY031"] = new HepJetsetLUDECYDummy(31);
    models["LUDECY032"] = new HepJetsetLUDECYDummy(32);
    models["LUDECY033"] = new HepJetsetLUDECYDummy(33);
    models["LUDECY041"] = new HepJetsetLUDECYDummy(41);
    models["LUDECY042"] = new HepJetsetLUDECYDummy(42);
    models["LUDECY045"] = new HepJetsetLUDECYDummy(45);
    models["LUDECY046"] = new HepJetsetLUDECYDummy(46);
    models["LUDECY048"] = new HepJetsetLUDECYDummy(48);
    models["LUDECY084"] = new HepJetsetLUDECYDummy(84);
    models["LUDECY085"] = new HepJetsetLUDECYDummy(85);
    models["LUDECY086"] = new HepJetsetLUDECYDummy(86);
    models["LUDECY087"] = new HepJetsetLUDECYDummy(87);
    models["LUDECY088"] = new HepJetsetLUDECYDummy(88);
    models["LUDECY102"] = new HepJetsetLUDECYDummy(102);
    models["LUDECY200"] = new HepJetsetLUDECYDummy(200);
  }
  // The constructor creates all decay model objects needed.

  virtual ~HepJetsetDummyHandler() {
    ModelMap::iterator it = models.begin();
    while ( it != models.end() ) delete (*it++).second;
    std::vector<HepJetsetProducerDummy *>::iterator pit = producers.begin();
    while ( pit != producers.end() ) delete *pit++;
  }
  // The destructor deletes all decay model and producer objects
  // allocated.

  virtual const HepString & name() const { return theName; }
  // Return the name of the handler (The id number is handled in the
  // base class).

  virtual HepDecayModel * readDecayModel(std::istream & is, const HepDecayMode &) {
    HepString s;
    is >> s;
    ModelMap::iterator it = models.find(s);
    return ( it == models.end() )? 0: (*it).second;
  }
  // Read the name of a decay model object from the stream and return
  // the corresponding <B><TT>HepJetsetLUDECYDummy</B></TT> object if
  // any.

  virtual HepParticleProducer *
    readParticleProducer(std::istream & is, const HepParticleData & pd) {
      HepString s;
      is >> s;
      if ( s != "Default" ) return 0;
      HepDouble wc;
      is >> wc;
      producers.push_back(new HepJetsetProducerDummy(pd, wc*MeV));
      return producers.back();
  }
  // Read the name of a producer (there is only one kind of producer
  // called "Default") and a number corresponding to a width cut. Then
  // create a corresponding <B><TT>HepJetsetProducerDummy</B></TT>
  // object, register it and return it.

  virtual void writeDecayModel(std::ostream &os , const HepDecayModel & d) const {
    ModelMap::const_iterator it = models.begin();
    while ( it != models.end() && (*it).second != &d ) ++it;
    if ( it != models.end() ) os << (*it).first;
  }
  // Check if the decay model belongs to this package and write out
  // its name.

  virtual void
    writeParticleProducer(std::ostream & os, const HepParticleProducer & pp) const {
      const HepJetsetProducerDummy & jpp = (const HepJetsetProducerDummy &) pp;
      os << "Default " << jpp.widthCut()/MeV;
  }
  // Write out the name (they are all called "Default") and the width
  // cut used for the producer.

  HepDecayModel * getDecayModel(const HepString & name) const {
    ModelMap::const_iterator dm = models.find(name);
    return dm == models.end()? 0: (*dm).second;
  }
  // Get a decay model based on the name.

  void addProducer(HepJetsetProducerDummy & pp) {
    producers.push_back(&pp);
  }
  // Add a producer object to the list.

private:
  ModelMap models;
  std::vector<HepJetsetProducerDummy *> producers;
  HepString theName;
};

#endif
