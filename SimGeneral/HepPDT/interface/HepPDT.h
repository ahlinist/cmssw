// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:50 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef PDT_H_INCLUDED
#define PDT_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepPDT</B></TT> class and some
// utilities needed for the classes representing particle properties.
//
// The <B><TT>HepPDT</B></TT> class is the main interface for using
// the CLHEP particle data table implementation. The actual
// implementation is contained in the <B><TT>HepPDTable</B></TT> class
// while the <B><TT>HepPDT</B></TT> class only implements static
// functions to access the only static instance of the
// <B><TT>HepPDTable</B></TT> class. All const methods of the
// <B><TT>HepPDTable</B></TT> class has their direct stait equivalents
// in <B><TT>HepPDT</B></TT>, while non-const methods has to be
// accessed indirectly via the <B><TT>theTable()</B></TT> method.
//
// The <B><TT>HepPDT</B></TT> class defined a set of enum's used by
// the different classes defining the particle data table
// implementation.
// <UL>
// <LI><B><TT>HepPDT::HepParity</B></TT> for parity information
// <LI><B><TT>HepPDT::HepSpin</B></TT> for spin information
// <LI><B><TT>HepPDT::HepCharge</B></TT> for charge information
// <LI><B><TT>HepPDT::HepColor</B></TT> for color information
// <LI><B><TT>HepPDT::HepParticleCodes</B></TT> for relating
// identifiers derived from the standard name of the particles with
// the STDHEP id number.
// </UL>
//
// CLASSDOC SUBSECTION See Also:
//
// <a href="http:PDTable.html">PDTable.h</a>
// <a href="http:ParticleData.html">ParticleData.h</a>
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
// <a href="http:DecayTable.html">DecayTable.h</a>
// <a href="http:DecayMode.html">DecayMode.h</a>
// <a href="http:PackageHandler.html">PackageHandler.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#define CLHEP_MAX_MIN_DEFINED
#include "CLHEP/config/CLHEP.h"
#include <string>
#include <map>
#include <iosfwd>

typedef std::string HepString;
typedef long HepLong;
typedef bool HepBoolean;
typedef int HepPkg;
// CLASSDOC OFF
const HepPkg HepPkgDefault = -1;
// CLASSDOC ON

class HepPDTable;
class HepParticleData;
class HepParticleMatcher;
class HepDecayMode;
class HepPackageHandler;

#include "SimGeneral/HepPDT/interface/HepPackageHandler.h"

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

// <P></UL><HR><UL>
class HepPDT {

public:

  typedef std::map<HepLong, HepParticleData *, std::less<HepLong> > ParticleMap;
  typedef std::map<HepString, HepParticleMatcher *, std::less<HepString> > MatcherMap;
  typedef std::map<HepPkg, HepPackageHandler *, std::less<HepPkg> > PackageMap;
  typedef std::map<HepString, HepDecayMode *, std::less<HepString> > ModeMap;
  // Convenient typedefs.

public:

  static HepPDTable & theTable();
  // Access the underlying object directly;

  static std::ostream & write(std::ostream &);
  static std::istream & read(std::istream &);
  static void readPDGFormat(std::istream &);
  // Read and write methods. The first two uses the internal ascii
  // format, while readPDGFormat reads the machine readable files
  // provided by the Particle Data Group. Note that for objects of
  // HepParticleMatcher, HepParticleProducer and HepDecayModel, only
  // their name is written, therefore the necessary objects must be
  // added to the HepPDTable before anything is read.

  void init();
  // Initialize the table, set up all cross references correctly. This
  // is done automatically after a read call and should be done
  // manually if any information is changed in the table.

  static const HepParticleData *
    getParticleData(HepLong);
  static const HepParticleData *
    getParticleData(const HepString &);
  static const HepParticleMatcher *
    getParticleMatcher(const HepString &);
  // Get particle data or particle matcher objects from the current
  // data table. Returns 0 if no corresponding object is found. For
  // the particle data method, the character string is first
  // translated into a number and the particle data object with the
  // corresponding id is returned if found.

  static const HepParticleData * getParticleData(HepLong, HepPkg);
  // Retrieve a particle data object according to a id of a numbering
  // scheme indicated by the second string argument.

  static const HepDecayMode * getDecayMode(const HepString &);
  // Return a decay mode corresponding to the given string. If no such
  // decay mode exists it returns 0.

  static const ParticleMap & particles();
  static const MatcherMap & matchers();
  static const PackageMap & packages();
  static const ModeMap & modes();
  // Access to all stuff within the current table.

  static HepPkg packageNumber(const HepString &);
  // Return the number used internally in the current for the given
  // package name.

  static HepString makeDecayTag(const HepDecayMode & dm);
  // Take a decay mode and produce a unique tag for the corresponding
  // initial and final state.

  static HepDouble globalParameter(HepPkg, const HepString &);
  // Get a global parameter for a given package and a given name.

public:

#include "SimGeneral/HepPDT/interface/enums.h"

  enum HepParity { PNA = -2, Pnegative = -1, Pminus = -1,
		   Punknown = 0, Pundefined = 0, Ppositive = 1, Pplus = 1 };
  //Definition of enumerated values used for parity information.

  enum HepSpin { SNA = -1, Sunknown = 0, Sundefined = 0,
		 S0 = 1, S12 = 2, S1 = 3, S32 = 4, S2 = 5,
		 S52 = 6, S3 = 7, S72 = 8, S4 = 9 };
  //Definition of enumerated values used for spin information.


  enum HepCharge { Cunknown = -99999, Cundefined = -99999,
		   Ccharged = 99999, Cpositive = 90000, Cnegative = -90000,
		   Cn3 = -9, Cn2 = -6, Cn53 = -5, Cn43 = -4,
		   Cn1 = -3, Cn23 = -2, Cn13 = -1, C0 = 0,
		   C13 = 1, C23 = 2, C1 = 3, C43 = 4,
		   C53 = 5, C2 = 6, C3 = 9 };
  //Definition of enumerated values used for charge information.

  enum HepColor { colunknown = -1, colundefined = -1,
		  col0 = 0, colcolored = 1, col3 = 3, col3bar = -3, col8 = 8 };
  //Definition of enumerated values used for color information.

  static inline HepBoolean charged(HepCharge c);
  // True if the argument corresponds to a non-zero charge.

  static inline HepBoolean positive(HepCharge c);
  // True if the argument corresponds to a positive charge.

  static inline HepBoolean negative(HepCharge c);
  // True if the argument corresponds to a negative charge.

  static inline HepBoolean colored(HepColor c);
  // True if the argument corresponds to a non-zero color charge.

};

// CLASSDOC OFF
std::ostream & operator << (std::ostream &, const HepPDTable &);
std::istream & operator >> (std::istream &, HepPDTable &);
// CLASSDOC ON

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepPDT.icc"
#else
#undef inline
#endif

#endif
