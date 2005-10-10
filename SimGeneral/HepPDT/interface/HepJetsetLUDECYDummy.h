// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:49 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepJetsetLUDECYDummy_H_INCLUDED
#define HepJetsetLUDECYDummy_H_INCLUDED
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


// </UL><HR><UL> First we define the decay model class called
// <B><TT>HepJetsetLUDECYDummy</B></TT>. It doesn't actually do
// anything but has a name corresponding to the matrix element numbering scheme in Jetset.
class HepJetsetLUDECYDummy: public HepDecayModel {
public:
  HepJetsetLUDECYDummy(int n)
    : theName(HepString("LUDECY")+char('0'+(n/100)%10)+
	      char('0'+(n/10)%10)+char('0'+n%10)) {}
  //The constructor just sets the name according to a given integer
  //corresponding to the matrix element to be used.
  const HepString & name() const {
    return theName;
  }
  // The only method returns the name. Typically one wold also expect
  // a <B><TT>'decay(JetsetParticle & p)'</B></TT> method or something
  // similar to actually perform a decay.
private:
  HepString theName;
};


#endif
