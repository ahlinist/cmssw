// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:49 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepDecayTable_H_INCLUDED
#define HepDecayTable_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepDecayTable</B></TT> class.
//
// The <B><TT>HepDecayTable</B></TT> class is derived from
// <B><TT>std::map&lt;HepString,HepDecayMode*&gt;</B></TT> and is a
// table of decay modes for a particle indexed by the identification
// tag. An object of class <B><TT>HepdecayTable</B></TT> also keeps a
// map for fast random selection of one of the listed decay modes
// based on the branching ratios associated with a certain package.
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:DecayMode.html">DecayMode.h</a>
// <a href="http:PackageHandler.html">PackageHandler.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepPDT.h"
#include <vector>

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

class HepDecayMode;

// <P></UL><HR><UL>
class HepDecayTable: public std::map<HepString, HepDecayMode*, std::less<HepString> > {

public:

  typedef std::map<HepString, HepDecayMode*,
    std::less<HepString> > basemap;
  typedef std::map<HepDouble, HepDecayMode*,
    std::less<HepDouble> > DoubleDecayMap;
  typedef std::vector<DoubleDecayMap> SumMap;
  // Convenient typedefs.

public:

  inline HepDecayTable();
  // The constructor.

  inline ~HepDecayTable();
  // The destructor.

  void init();
  // Initialize cross referencing etc.

  const HepDecayMode * selectMode(HepDouble, HepPkg) const;
  // Selects a decay mode for a given package number based on a given
  // random number in the range ]0,1[ and the exclusive branching
  // ratios of the list of decay channels.

private:

  std::vector<HepDouble> sumFractions;
  SumMap sumTree;

};

#ifndef HEP_DEBUG_INLINE
#include "SimGeneral/HepPDT/interface/HepDecayTable.icc"
#else
#undef inline
#endif

#endif
