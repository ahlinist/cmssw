// -----------------------------------------------------------------------------
//  24.02.99 taken from http://www.thep.lu.se/~leif/CLHEP-PDT/
// -----------------------------------------------------------------------------
//  $Date: 2001/04/11 08:30:49 $
//  $Revision: 1.1 $
// -----------------------------------------------------------------------------
// -*- C++ -*-
//
#ifndef HepNotMatcher_H_INCLUDED
#define HepNotMatcher_H_INCLUDED
//
// This file may become a part of CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the <B><TT>HepCombinedMatcher</B></TT>
// and <B><TT>HepNotMatcher</B></TT> classes.
//
// <B><TT>HepCombinedMatcher</B></TT> and
// <B><TT>HepNotMatcher</B></TT> are templated classes used to create
// combinations of classes derived from HepParticleMatcher.
//
// <B><TT>HepCombinedMatcher&lt;class T1, class Op, class
// T2&gt;</B></TT> takes three template arguments: <B><TT>T1</B></TT>
// and <B><TT>T2</B></TT> should be derived from HepParticleMatcher
// and <B><TT>Op</B></TT> should be one of the logical binary
// operators <B><TT>std::logical_or</B></TT>,
// <B><TT>std::logical_and</B></TT> or any similarly defined class. In
// the case of <B><TT>std::logical_and</B></TT>, the resulting class
// will match all particles matched by the class <B><TT>T1</B></TT>
// <B>and</B> <B><TT>T2</B></TT>, and similarly for the case of
// <B><TT>std::logical_or</B></TT>.
//
// <B><TT>HepNotMatcher&lt;class T&gt;</B></TT> takes one template
// argument which should be a class derived from
// <B><TT>HepParticleMatcher</B></TT>. The resulting class will match
// all particles not matched by the class <B><TT>T</B></TT>.
//
// CLASSDOC SUBSECTION Examples
//
// Assuming that the class <B><TT>ChargedMatcher</B></TT> matches all
// charged particles and the class <B><TT>BaryonMatcher</B></TT>
// matches all baryons. Then the class
// <br><B><TT>HepCombinedMatcher&lt;ChargedMatcher, std::logical_and,
// BaryonMatcher&gt</B></TT><br> will match all charged baryons and the
// class <br><B><TT>HepNotMatcher&lt;ChargedMatcher&gt;</B></TT><BR> will
// match all uncharged particles.
//
// CLASSDOC SUBSECTION See also:
//
// <a href="http:PDT.html">PDT.h</a>
// <a href="http:PDTable.html">PDTable.h</a>
// <a href="http:ParticleData.html">ParticleData.h</a>
// <a href="http:ParticleMatcher.html">ParticleMatcher.h</a>
// <a href="http:StandardMatchers.html">StandardMatchers.h</a>
//
// CLASSDOC SUBSECTION Author:
// Leif L&ouml;nnblad
//

#include "SimGeneral/HepPDT/interface/HepParticleMatcher.h"
#include <functional>

#ifdef HEP_DEBUG_INLINE
#define inline
#endif

// <P></UL><HR><UL>
template <class T>
class HepNotMatcher: public HepParticleMatcher {

public:

  inline HepNotMatcher(const HepString &);
 // The constructor.

  inline virtual ~HepNotMatcher();
  // The destructor.

  inline virtual HepBoolean checkMatch(const HepParticleData &) const;
  inline static HepBoolean matchCheck(const HepParticleData &);
  // Check if a particle meets the criteria.

};



#ifdef HEP_DEBUG_INLINE
#undef inline
#endif

#include "SimGeneral/HepPDT/interface/HepNotMatcher.icc"

#endif
