// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:51 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepParticleData_H_INCLUDED
#define HepParticleData_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepParticleData</B></TT> class.
//
// An <B><TT>HepParticleData</B></TT> object contains particle
// properties of a given type of particle closely relate to what is
// published by the Particle Data Group. It does not contain any
// information of a given particle instance such as momentum or point
// of creation.
//
// The following information is available for a given particle using
// <TT>CLHEP/Units/SystemOfUnits.h</TT> where applicable:
// <UL>
// <LI> The particle id number as given by STDHEP.
// <LI> The ASCII string name of the particle (in the future Hopefully
// also given by STDHEP).
// <LI> The measured mass with errors.
// <LI> The measured width and/or decay length (c-tau) with errors. If
// only one is given, the other is calculated, but in principle
// inconsistent widths and decay lengths can be specified.
// <LI> The charge in units defined by the enum
// <B><TT>HepPDT::HepCharge</B></TT> in
// <TT>CLHEP/PDT/PDT.h</TT>. The <B><TT>charge()</B></TT>
// method, however, returns the charge in standard.
// <LI> The spin in units defined by the enum
// <B><TT>HepPDT::HepSpin</B></TT>.  The <B><TT>spin()</B></TT>
// method, however, returns the spin in standard units.
// <LI> The color in units defined by the enum
// <B><TT>HepPDT::HepColor</B></TT>
// <LI> A boolean indicating whether the particle is considered stable
// or not.
// <LI> A pointer to the corresponding anti-particle (set to 0 if the
// particle is its own anti-particle).
// <LI> A decay table with decay modes represented by pointers to
// <B><TT>HepDecayMode objects</B></TT>.
// </UL>
//
// In addition each <B><TT>HepParticleData</B></TT> objects contains a
// map relating pointers to objects of base class
// <B><TT>HepParticleProducer</B></TT> with integers representing
// packages with different implementations of particle instances. A
// class derived from the <B><TT>HepParticleProducer</B></TT> class
// may contain any kind of information about the particle (or group of
// particles) which points to it. But mainly, the
// <B><TT>HepParticleProducer</B></TT> object pointed to by a
// <B><TT>HepParticledata</B></TT> object is intended to be able to
// produce an instance of the corresponding particle. The assignment
// of <B><TT>HepParticleProducer</B></TT> objects can be done by hand
// or may be handled by the <B><TT>HepPDTable</B></TT> class via user
// implemented classes derived from <B><TT>HepPackageHandler</B></TT>.
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
#include "CLHEP/Units/PhysicalConstants.h"
#include "SimGeneral/HepPDT/interface/HepDecayTable.h"

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

class HepDecayMode;

// <P></UL><HR><UL>
class HepParticleData {

public:

  typedef std::map<HepPkg, HepParticleProducer *,
              std::less<HepPkg> > ProducerMap;

  friend class HepPDTable;

public:

  inline HepParticleData(HepLong, const HepString &);
  // Constructor.

  inline virtual ~HepParticleData();
  // The destructor.

  inline HepLong id() const;
  inline const HepString & name() const;
  // Acces to name and id number.

  inline const HepDecayTable & decayTable() const;
  // Access the decy table.

  inline const HepDecayMode *
  selectMode(HepDouble, HepPkg = HepPkgDefault) const;
  // Selects a decay mode for a given package number based on a given
  // random number in the range ]0,1[ and the exclusive branching
  // ratios of the list of decay channels.

  inline HepDouble mass(HepDouble, HepDouble = 0.0,
			HepDouble = -1.0);
  inline HepDouble mass() const;
  inline HepDouble massErrPos() const;
  inline HepDouble massErrNeg() const;
  // Set/get the mass. Arguments for the set method are the mass, the
  // positive and negative error. If the negative error is left out,
  // it is set to the positive error.

  inline HepDouble width(HepDouble, HepDouble = 0.0,
			 HepDouble = -1.0);
  inline HepDouble width() const;
  inline HepDouble widthErrPos() const;
  inline HepDouble widthErrNeg() const;
  // Set/get the width. Arguments for the set method are the widht,
  // the positive and negative error. If the negative error is left
  // out, it is set to the positive error. If no width is specified,
  // it is calculated from the lifetime.

  inline HepDouble cTau(HepDouble, HepDouble = 0.0,
			HepDouble = -1.0);
  inline HepDouble cTau() const;
  inline HepDouble cTauErrPos() const;
  inline HepDouble cTauErrNeg() const;
  // Set/get cTau. Arguments for the set method are the widht, the
  // positive and negative error. If the negative error is left out,
  // it is set to the positive error. If no life time is specified, it
  // is calculated from the width.

  inline HepPDT::HepCharge iCharge(HepPDT::HepCharge);
  inline HepDouble charge() const;
  inline HepPDT::HepCharge iCharge() const;
  inline HepBoolean charged() const;
  inline HepBoolean positive() const;
  inline HepBoolean negative() const;
  // Set/get the charge. In the set method the charge should be given
  // in units of e/3. in the charge() method the charge is returned in
  // standard units and in iCharge the charge is returned in units of e/3.

  inline HepPDT::HepSpin iSpin(HepPDT::HepSpin);
  inline HepDouble spin() const;
  inline HepPDT::HepSpin iSpin() const;
  // Set/get the spin. In the set method the spin should be given as
  // 2J+1 in units of hbar/2. In the spin() method the spin is
  // returned in standard units, and in iSpin the spin is returned as
  // 2J+1 in units of hbar/2.

  inline HepPDT::HepColor iColor(HepPDT::HepColor);
  inline HepPDT::HepColor iColor() const;
  inline HepBoolean colored() const;
  // Set/get the color of the particle in units of HepPDT::HepColor.

  inline HepBoolean CC(const HepParticleData &);
  inline const HepParticleData * CC() const;
  // Set/get the pointer to the corresponding anti partner. The set
  // method returns true if the operation succeded.

  inline void stable(HepBoolean);
  inline HepBoolean stable() const;
  // Set/get if particle is to be considered stable. If the decay
  // table is empty the get method always returns true, even if the
  // member variable is false.

  inline void addProducer(HepPkg, HepParticleProducer &);
  inline HepParticleProducer *
    producer(HepPkg = HepPkgDefault) const;
  // Add/get a pointer to an object corresponding to given package
  // number capable of producing a particle object corresponding to this
  // particle data object.

  inline void setDefaultProducer(HepPkg dp);
  inline HepPkg defaultProducer() const;
  // Get/set the default producer;

private:

  void addDecayMode(HepDecayMode &);
  // Add a decay mode for this particle. Should only be done from the
  // HepPDTable object which handles the creation and destruction of
  // decay modes.

  HepLong stdhepId;
  HepString stdhepName;
  // Name and Id number according to the STDHEP standard.

  HepBoolean isStable;
  // True if the particle is considered stable.

  HepDecayTable decayModes;
  // List of decay modes.

  HepDouble m, dmp, dmn;
  HepDouble w, dwp, dwn;
  HepDouble t, dtp, dtn;
  // Mass, width and lifetime with errors.

  HepPDT::HepCharge c;
  HepPDT::HepSpin s;
  // Three times the charge and 2 times the spin plus one.

  HepPDT::HepColor col;
  // The colour for this particle.

  const HepParticleData * antiPartner;
  // Pointer to the object corresponding to the antiparticle. Set to
  // zero if it is its own antiparticle.

   ProducerMap producers;
  // A map relating package numbers to a pointer to a producer object.

  HepPkg defprod;
  // The number corresponding to a default producer;

};

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepParticleData.icc"
#else
#undef inline
#endif

#endif
