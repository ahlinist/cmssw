/*
 *  TurboSimParticleType.h
 *  TurboSim
 *
 *  Created by David Friend on 7/2/05.
 *
 */

#ifndef __TurboSimParticleType_h
#define __TurboSimParticleType_h

#include <string>
#include <vector>

//#include "Validation/VstTurboSim/interface/xtoa.hh"
#include "Validation/VstTurboSim/interface/TurboSimSaveableItem.hh"

// Delimiter between particles
#define DELIMITER_VALUE ';'  // must be single character

// Masses in GeV
#define BOTTOM_QUARK_MASS 4.3
#define TOP_QUARK_MASS 175.
#define ELECTRON_MASS 0.000511
#define MUON_MASS 0.106
#define TAU_MASS 1.78
#define W_BOSON_MASS 80.4
#define Z_BOSON_MASS 91.2
#define PI_MESON_MASS 0.140
#define KAON_MESON_MASS 0.494
#define RHO_MESON_MASS 0.770
#define PROTON_MASS 0.938

class TurboSimParticleType : public TurboSimSaveableItem {
   
public:
   
   // Constructors
   TurboSimParticleType( const std::string& );
   TurboSimParticleType( const TurboSimParticleType& );
   TurboSimParticleType( int ); // construct from PDG code
   TurboSimParticleType( ); // default constructor
   
   
   // Accessors
   std::string toString() const;
   int charge() const;  // electric charge
   bool chargeConjugateNeeded() const;
   TurboSimParticleType chargeConjugate() const;
   TurboSimParticleType baseType() const;
   double mass() const; // mass in GeV
   TurboSimParticleType type() const { 
      return TurboSimParticleType( *this ); }
   
   // Operators
   
   // Equality
   bool operator==( const TurboSimParticleType &rhs ) const { 
      return ( rhs._type == _type ); }
   bool operator==( const std::string &rhs ) const;
   bool operator!=( const TurboSimParticleType &rhs) const { return !( rhs._type == _type ); }
   bool operator!=( const std::string &rhs ) const {
      return !( *this == rhs ); }
   
   // Ordering
   bool operator<( const TurboSimParticleType &rhs ) const { 
      return ( this->toString() < rhs.toString() ); }
   bool operator>( const TurboSimParticleType &rhs ) const { 
      return !( *this < rhs ); }   
   
   // Modifiers
   bool read( std::istream& );
   
private:
      
      enum enumParticleType { 
         
         NO_TYPE = 0, // uninitialized, used when converting from string
         
         // Quarks / Jets (standard numbering)
         JET = 1,             // j (equivalent to u, d, s, c)
         B = 5,               // b 
         T = 6,               // t
         
         // Leptons (standard numbering)
         E_MINUS = 11,        // e-   
         E_PLUS = -11,        // e+
         E_NEUTRINO = 12,      // nu_e
         MU_MINUS = 13,       // mu-
         MU_PLUS = -13,       // mu+
         MU_NEUTRINO = 14,     // nu_mu
         TAU_MINUS = 15,      // tau-
         TAU_PLUS = -15,      // tau+
         TAU_NEUTRINO = 16,    // nu_tau
         
         // Gauge bosons (standard numbering)
         PHOTON = 22,         // ph
         Z = 23,              // z
         W_PLUS = 24,         // w+
         W_MINUS = -24,       // w-
         
         // RHIC objects (standard numbering)
         P = 113,             // p   ( rho )
         P_PLUS = 213,        // p+
         P_MINUS = -213,      // p-
         PI = 111,            // pi
         PI_PLUS = 211,       // pi+
         PI_MINUS = -211,     // pi-
         K = 311,             // K
         K_PLUS = 321,        // K+
         K_MINUS = -321,      // K-
         
         // Other
         UNCL = 500,          // unclustered energy, "uncl"
         KILL,                // kill
         PMISS,               // pmiss
         PROTON,              // proton  ( included for collision typing )
         P_BAR,               // pbar    ( included for collision typing )
         UNRECOGNIZED,        // unknown ( used when converting from PDG code )
	 DELIMITER,           // DELIMITER_VALUE ( ; to seperate objects )

         // Base 'particles' 
         E,                   // e
         MU,                  // mu
         TAU,                 // tau
         NEUTRINO,            // nu
         W                    // w
         
      };
   
   // Private Constructor
   TurboSimParticleType( enumParticleType );
   // Private Comparator
   bool operator==( enumParticleType rhs ) const {
      return ( rhs == _type ); }
   
   enumParticleType _type;
   
   static std::string Enum2String( enumParticleType );
   static enumParticleType String2Enum( const std::string& );
   static enumParticleType PDG2Enum( int code );
   static void parseErrorData( std::string& );
};


// Streaming support
std::ostream &operator<<( std::ostream &,
                     const TurboSimParticleType & );
std::istream &operator>>( std::istream &,
                     TurboSimParticleType & );


// Predefined Particles
namespace DefinitionOf {
   
   const static TurboSimParticleType JET =
      TurboSimParticleType( "j" );
   const static TurboSimParticleType BJET =
      TurboSimParticleType( "b" );
   const static TurboSimParticleType UNCL =
      TurboSimParticleType( "uncl" );
   const static TurboSimParticleType PMISS =
      TurboSimParticleType( "pmiss" );
   const static TurboSimParticleType PHOTON =
      TurboSimParticleType( "ph" );
   const static TurboSimParticleType W_MINUS_BOSON =
      TurboSimParticleType( "w-" );
   const static TurboSimParticleType W_PLUS_BOSON =
      TurboSimParticleType( "w+" );
   const static TurboSimParticleType Z_BOSON =
      TurboSimParticleType( "z" );
   const static TurboSimParticleType ELECTRON =
      TurboSimParticleType( "e-" );
   const static TurboSimParticleType MUON =
      TurboSimParticleType( "mu-" );
   const static TurboSimParticleType TAU =
      TurboSimParticleType( "tau-" );
   const static TurboSimParticleType NEUTRINO =
      TurboSimParticleType("nu");
   const static TurboSimParticleType PROTON =
      TurboSimParticleType("proton");
   const static TurboSimParticleType NULL_PARTICLE =
      TurboSimParticleType( );

   // DELIMITER_VALUE
//    const static TurboSimParticleType DELIMITER =
//      TurboSimParticleType( xtoa( char(DELIMITER_VALUE) ) ); 
   const static TurboSimParticleType DELIMITER =
     TurboSimParticleType( char(DELIMITER_VALUE)  ); 

   // array of all types that could be in a final state
   // with sign (i.e. real particles)
   // defined in TurboSimParticleType.cc file.
  std::vector<TurboSimParticleType> const *getAllFinals();

   // array of all types that could be in a final state
   // without signs (i.e. base particles)
   // defined in TurboSimParticleType.cc file.
  std::vector<TurboSimParticleType> const *getBaseFinals();

}


#endif
