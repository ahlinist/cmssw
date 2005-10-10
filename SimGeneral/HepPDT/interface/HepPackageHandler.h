// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:51 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepPackageHandler_H_INCLUDED
#define HepPackageHandler_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepPackageHandler</B></TT>,
// <B><TT>HepDecayModel</B></TT> and
// <B><TT>HepParticleProducer</B></TT> classes.
//
// The <B><TT>HepPackageHandler</B></TT> class is an abstract base
// class used to allow the CLHEP particle data table to handle
// information used by user defined packaged with different particle
// instanciation schemes and/or models for particle decays. In this
// way, all the information needed for a program using the CLHEP
// particle data table can be handled by <B><TT>HepPDTable</B></TT>
// and can be stored in the same ASCII file as the particle properties
// handled directly by <B><TT>HepPDTable</B></TT>.
//
// The <B><TT>HepDecayModel</B></TT> is an empty abstract base class
// intended to keep information needed to perform an actual particle
// decay and also to perform the decay given an object corresponding
// to an actual particle instance in the given package.
//
// The <B><TT>HepParticleProducer</B></TT> is an empty abstract base
// class intended to store package specific information for a particle
// type and should be able to create an object correponding to an
// actual particle instance in the given package.
//
// <B><TT>HepPackageHandler</B></TT> have five purely virtual methods
// which has to be implemented by a derived class.
// <UL>
// <LI><B><TT>name()</B></TT> should return a unique name of the package.
// <LI><B><TT>writeDecayModel(ostream &, const HepDecayModel
// &)</B></TT> should write the data of a
// <B><TT>HepDecayModel</B></TT> object so that it can be read again
// by <B><TT>readDecayModel</B></TT>.
// <LI><B><TT>writeParticleProducer(ostream &, const
// HepParticleProducer &)</B></TT> should write the data of a
// <B><TT>HepParticleProducer</B></TT> object so that it can be read
// again by .B <B><TT>readParticleProducer</B></TT>
// <LI><B><TT>readDecayModel(istream &, const HepDecayMode &)</B></TT>
// should be able to read information previously written by
// <B><TT>writeDecayModel</B></TT> and reconstruct and return the
// corresponding <B><TT>HepDecayModel</B></TT> object.
// <LI><B><TT>readParticleProducer(istream &, const HepParticleData
// &)</B></TT> should be able to read information previously written
// by <B><TT>writeParticleProducer</B></TT> and reconstruct and return
// the corresponding <B><TT>HepParticleProducer</B></TT> object.
// </UL>
//
// Note that while the <B><TT>HepPDTable</B></TT> is responsible for
// the handeling of <B><TT>HepPackageHandler</B></TT> objects, the
// class derived from <B><TT>HepPackageHandler</B></TT> is itself
// responsible for the handeling of corresponding
// <B><TT>HepParticleProducer</B></TT> and
// <B><TT>HepDecayModel</B></TT> objects.
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

#include "SimGeneral/HepPDT/interface/HepPDT.h"
#include "SimGeneral/HepPDT/interface/HepDecayModel.h"
#include "SimGeneral/HepPDT/interface/HepParticleProducer.h"
#include <iosfwd>

class HepParticleData;

class HepPackageHandler {
  friend class HepPDTable;
public:
  virtual ~HepPackageHandler() {}
  // The destructor.

  virtual const HepString & name() const = 0;
  // Returns the name of this package.

  virtual HepDecayModel * readDecayModel(std::istream &, const HepDecayMode &) = 0;
  // read information from a stream, previously written by
  // writeDecayModel(), construct the corresponding HepDecayModel
  // object and return it.

  virtual HepParticleProducer *
    readParticleProducer(std::istream &, const HepParticleData &) = 0;
  // read information from a stream, previously written by
  // writeParticleProducer(), construct the corresponding
  // HepParticleProducer object and return it. Note that the Producer
  // object will have a reference to the particle data object and
  // therefore one particle producer object must be created for each
  // particle data object.

  virtual void writeDecayModel(std::ostream &,
			       const HepDecayModel &) const = 0;
  // Write information from a HepDecayModel object so that it can be
  // reconstructed by readDecayModel().

  virtual void writeParticleProducer(std::ostream &,
				     const HepParticleProducer &) const = 0;
  // Write information from a HepParticleProducer object so that it
  // can be reconstructed by readParticleProducer().

  HepPkg handlerId() const { return phid; }
  // Return the package id number given by HepPDTable.

protected:
  HepPackageHandler() {};

private:
  void handlerId(HepPkg id) { phid = id; }
  // Set the package id number. Is done automatically when added to
  // HepPDTable

  HepPkg phid;
  // The package id number given by HepPDTable.

};

#endif
