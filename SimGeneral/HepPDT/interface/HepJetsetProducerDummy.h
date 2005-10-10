// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:49 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepJetsetProducerDummy_H_INCLUDED
#define HepJetsetProducerDummy_H_INCLUDED
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


// <P>Then we define the particle producer class called
// <B><TT>HepJetsetProducerDummy</B></TT>. It also doesn't actually do
// anything but has an variable storing a cut on the width of the
// particle which Jetset needs in order to generate the mass of a
// particle instance.
class HepJetsetProducerDummy: public HepParticleProducer {
public:
  HepJetsetProducerDummy(const HepParticleData & pd, HepDouble wc)
    : HepParticleProducer(pd), wcut(wc) {}
  // The constructor takes the cut in the width as an argument.
  void widthCut(HepDouble wc) { wcut = wc; }
  HepDouble widthCut() const { return wcut; }
  // Set/Get the cut in the width. Typically one would also expect a
  // method <B><TT>'JetsetParticle * create()'</B></TT> or something
  // similar to actually create an instance of a particle
private:
  HepDouble wcut;
};


#endif
