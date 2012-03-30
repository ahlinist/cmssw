/*
 *  TurboSimParticle.h
 *  TurboSim
 *
 *  @author David Friend, friend at mit dot edu on 7/3/05.
 *
 */

#ifndef __TurboSimParticle_h
#define __TurboSimParticle_h

#include "Validation/VstTurboSim/interface/TurboSimParticleType.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
//#include "Validation/VstTurboSim/interface/atox.hh"
//#include "Validation/VstTurboSim/interface/xtoa.hh"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"

#define ParticleEqualityPerpMinimum 20  // cutoff energy

class TurboSimParticle : public TurboSimParticleType {
   
public:
   
   enum StringFormat {
      DEFAULT = 0,  // hadron 
      HADRON_COLLIDER_STANDARD,  // [m]-pt-eta-phi(deg) 
      LEPTON_COLLIDER_STANDARD,  // [m]-e-costheta-phi(rad)
      LORENTZ_VECTOR // e px py pz
   };
   
   // Constructors
   TurboSimParticle( const TurboSimParticleType &type, 
                     const CLHEP::HepLorentzVector& );
   TurboSimParticle( ); // default
   
   // Accessors
   const CLHEP::HepLorentzVector &lorentzVector() const { return _lorentzVector; }
   double mass() const { return _lorentzVector.m(); }
   double restMass() const { return TurboSimParticleType::mass(); }
   std::string toString( int format,
                         int precision = 3 ) const;
   std::string toString( ) const { return toString( DEFAULT ); }
   TurboSimParticle particle() const { 
      return TurboSimParticle( *this ); }
   
   // Operators 
   
   // Equality
   bool operator==( const TurboSimParticle& ) const;
   bool equals( const TurboSimParticle&, double tolerance = 0.0 ) const;
   
   // Ordering
   bool operator<( const TurboSimParticle& ) const;
   bool operator>( const TurboSimParticle &rhs ) const { return !( *this < rhs ); }
   
   // add lorentz vectors, particle charges.
   TurboSimParticle operator+( const TurboSimParticle& ) const;
   
   // Modifiers
   bool read( std::istream &fin );
   bool read( std::istream &fin,
              int format );
   
private:   
   
   CLHEP::HepLorentzVector _lorentzVector;
   
   // Utility Function
   enum ParseException { DELIMITER_EXCEPTION = 0 };
   static double parseData( std::string & ) throw( ParseException );
 
};


// Stream Operators
std::ostream &operator<<( std::ostream &out,
                          const TurboSimParticle &data );
std::istream &operator>>( std::istream &in,
                          TurboSimParticle &data );

// Predefined null particle
namespace DefinitionOf {
   const static TurboSimParticle nullParticle = TurboSimParticle();
}

#endif

