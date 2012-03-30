/*
 *  TurboSimParticle.cpp
 *  TurboSim
 *
 *  Created by David Friend on 7/3/05.
 *
 */

#include "Validation/VstTurboSim/interface/TurboSimParticle.hh"
#include "Validation/VstMathUtils/interface/Math.hh"

using namespace std;

// Creation Constructor
TurboSimParticle::TurboSimParticle( const TurboSimParticleType &type,
                                    const CLHEP::HepLorentzVector &lv )
:  
   TurboSimParticleType( type ),
   _lorentzVector( lv ) {}

// Default Constructor
TurboSimParticle::TurboSimParticle( ) 
:
   TurboSimParticleType( DefinitionOf::NULL_PARTICLE ),
   _lorentzVector( CLHEP::HepLorentzVector( 0, CLHEP::Hep3Vector( 0, 0, 0 ) ) )
{}

// Accessors 

string TurboSimParticle::toString( int format, 
                                   int precision ) const {
   
   if ( format == DEFAULT )
      format = HADRON_COLLIDER_STANDARD;
      
   string ans = TurboSimParticleType::toString() + " ";
   
   if ( format == LEPTON_COLLIDER_STANDARD ) {
      if ( ( type() == DefinitionOf::JET ) ||
           ( type() == DefinitionOf::BJET ) ||
           ( type() == DefinitionOf::UNCL ) )
         ans += Math::ftoa( _lorentzVector.m() ) + " ";
      ans +=
         Math::ftoa( _lorentzVector.e() ) + " " +
         Math::ftoa( _lorentzVector.cosTheta() ) + " " +
         Math::ftoa( _lorentzVector.phi() );
      return ans;
   } else if ( format == HADRON_COLLIDER_STANDARD ) {
      if ( ( type() == DefinitionOf::JET ) ||
           ( type() == DefinitionOf::BJET ) ||
           ( type() == DefinitionOf::UNCL ) || 
           ( type() == DefinitionOf::W_MINUS_BOSON ) ||
           ( type() == DefinitionOf::W_PLUS_BOSON ) ||
           ( type() == DefinitionOf::Z_BOSON ) )
         ans += Math::ftoa( _lorentzVector.m() ) + " ";
      double phi_deg = fmod( ( _lorentzVector.phi() * 180. / M_PI ), 360. );
      ans +=
         Math::ftoa( _lorentzVector.perp() ) + " " +
         ( _lorentzVector.perp() == 0 ? 
           "0" : 
           Math::ftoa( Math::theta2eta( _lorentzVector.theta() ) ) ) + " " +
         Math::ftoa( phi_deg );
      return ans;
   } else if ( format == LORENTZ_VECTOR ) {
      ans += 
      Math::ftoa( _lorentzVector.e() ) + " " +
      Math::ftoa( _lorentzVector.px() ) + " " +
      Math::ftoa( _lorentzVector.py() ) + " " +
      Math::ftoa( _lorentzVector.pz() );
      return ans;
   } else 
      return ans;
}

// Operators

// Check exact equality
bool TurboSimParticle::operator==( const TurboSimParticle &rhs ) const {
   return ( ( type() == rhs.type()  ) &&
            ( _lorentzVector == rhs._lorentzVector ) );
}

// Approximate equality
bool TurboSimParticle::equals( const TurboSimParticle &rhs, 
                               double tolerance ) const {
   
   bool tmpReturn = true;
   
   if ( !( type() == rhs.type() ) )
      tmpReturn = false;
   
   if ( _lorentzVector.perp() + rhs._lorentzVector.perp() > 
        ParticleEqualityPerpMinimum ) {
      double deltaPt = 
      2 * fabs( _lorentzVector.perp() - rhs._lorentzVector.perp() ) /
	     ( _lorentzVector.perp() + rhs._lorentzVector.perp() );
      if ( deltaPt > tolerance )
         tmpReturn = false;
      double deltaR = 
         Math::deltaR( _lorentzVector.phi(), 
                       Math::theta2eta( _lorentzVector.theta() ),
                       rhs._lorentzVector.phi(),
                       Math::theta2eta( rhs._lorentzVector.theta() ) );
      if ( deltaR > tolerance )
         tmpReturn = false;
   }
   
   return tmpReturn;
}

// Ordering Operators
bool TurboSimParticle::operator<( const TurboSimParticle &rhs ) const {
      
   // sort on Pt
   return ( _lorentzVector.perp() < rhs._lorentzVector.perp() );   

}
      
TurboSimParticle TurboSimParticle::operator+( const TurboSimParticle &rhs ) const {
   if ( _lorentzVector.perp() > rhs._lorentzVector.perp() )
      return TurboSimParticle( type(),
                               _lorentzVector + rhs._lorentzVector );
   return TurboSimParticle( rhs.type(),
                            _lorentzVector + rhs._lorentzVector );   
}

// Modifiers
bool TurboSimParticle::read ( istream &source ) {
   return read( source, DEFAULT );
}

bool TurboSimParticle::read ( istream &source,
                              int format ) {
   
   *this = DefinitionOf::nullParticle;

   string tmpString;
   double m = 0, e = 0, px = 0, py = 0, pz = 0;
   
   if ( !TurboSimParticleType::read( source ) ) {
      if ( type() != DefinitionOf::DELIMITER )
	 *this = DefinitionOf::nullParticle;
      return false;
   }
   
   assert( type() != DefinitionOf::NULL_PARTICLE );
   
   if ( ( type() == DefinitionOf::JET ) || 
        ( type() == DefinitionOf::BJET ) || 
        ( type() == DefinitionOf::UNCL ) ||
        ( type() == DefinitionOf::Z_BOSON ) || 
        ( type() == DefinitionOf::W_MINUS_BOSON ) || 
        ( type() == DefinitionOf::W_PLUS_BOSON ) ) {
      source >> tmpString;
      try {
	 m = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected mass." << endl;
	 return false;
      }
   }
   else {
      m = TurboSimParticleType::mass();
   }

   if ( format == LEPTON_COLLIDER_STANDARD ) {
      
      double costheta, phi, p, pt;
      
      source >> tmpString; 
      try {
	 e  = parseData( tmpString );
      } catch ( ParseException e) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected e." << endl;
	 return false;
      }
	 
      source >> tmpString; 
      try {
	 costheta = parseData( tmpString );
      } catch ( ParseException e) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected cosTheta." << endl;
	 return false;
      }

      source >> tmpString; 
      try {
	 phi = parseData( tmpString );
      } catch ( ParseException e) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected phi." << endl;
	 return false;
      }

      p = sqrt( e * e - m * m );
      
      pz = p * costheta;
      pt = sqrt( p * p - pz * pz );
      px = pt * cos( phi );
      py = pt * sin( phi );
      
   }
   
   else if ( format == HADRON_COLLIDER_STANDARD ) {
      
      if ( type() == DefinitionOf::UNCL )
         m = 0;
      
      double pt, eta, phi, theta, p;
      
      source >> tmpString; 
      try {
	 pt = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected pt." << endl;
	 return false;
      }
	 
      source >> tmpString; eta = 0;
      if ( ( pt > 0.001 ) && ( type() != DefinitionOf::UNCL ) )
      try {
	 eta = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected eta." << endl;
	 return false;
      }

      source >> tmpString; 
      try {
	 phi = parseData( tmpString ) * M_PI / 180.0;
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected phi." << endl;
	 return false;
      }

      if ( pt < 0.001 ) pt = 0.001;
      
      theta = Math::eta2theta( eta );
      p = pt / sin( theta );
      
      e = sqrt( p * p + m * m );
      px = pt * cos( phi );
      py = pt * sin( phi );
      pz = p * cos( theta );
      
   }
   
   else if ( format == LORENTZ_VECTOR ) {      
      source >> tmpString; 
      try {
	 e = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected e." << endl;
	 return false;
      }

      source >> tmpString;
      try {
	 px = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected px." << endl;
	 return false;
      }

      source >> tmpString;
      try {
	 py = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected py." << endl;
	 return false;
      }

      source >> tmpString;
      try {
	 pz = parseData( tmpString );
      } catch ( ParseException e ) {
	 cerr << "TurboSimParticle::read failure. "
	      << "Delimiter found prematurely. Expected pz." << endl;
	 return false;
      }
   }
   
   if ( baseType() == DefinitionOf::NEUTRINO ) {
      *( reinterpret_cast< TurboSimParticleType* >( this ) ) = DefinitionOf::UNCL;
      e = px = py = pz = 0;
   }
   
   _lorentzVector = CLHEP::HepLorentzVector( e, CLHEP::Hep3Vector( px, py, pz ) );

   return true;
}

// Utility Function
// static, private
double TurboSimParticle::parseData( string &num ) throw ( TurboSimParticle::ParseException ) {
   if ( num == DefinitionOf::DELIMITER.toString() )
      throw DELIMITER_EXCEPTION;
   for ( size_t i = 0; i < num.length(); i++ )
      if ( num.substr( i, 1 ) == "{" ) {
         num = num.substr( 0, i );
         break;
      }
   return atoi( num.c_str() );
}

// Streaming Support
ostream &operator<<( ostream &out, 
                     const TurboSimParticle &data ) {
   out << data.TurboSimParticle::toString();
   return out;
}

istream &operator>>( istream &in,
                     TurboSimParticle &data ) {
   data.read( in );
   return in;
}

