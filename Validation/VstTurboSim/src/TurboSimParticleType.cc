/*
 *  TurboSimParticleType.cpp
 *  TurboSim
 *
 *  Created by David Friend on 7/2/05.
 *
 */

#include "Validation/VstTurboSim/interface/TurboSimParticleType.hh"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

TurboSimParticleType::TurboSimParticleType( const string &strType )
: _type( String2Enum( strType ) ) {

   if ( _type == NO_TYPE ) 
      cerr << "ParticleType::ParticleType: Unrecognized type " << strType
	   << "." << endl;   

   return;
}

// Create Particle from PDG code
TurboSimParticleType::TurboSimParticleType( int ParticleDataGroupCode ) 
: _type( PDG2Enum( ParticleDataGroupCode ) ) {

   if ( _type == UNRECOGNIZED )
      cerr << "ParticleType::ParticleType: Unrecongnized P.D.G. Code: " 
	   << ParticleDataGroupCode << endl;
   return;

}

// Copy Constructor
TurboSimParticleType::TurboSimParticleType( const TurboSimParticleType& original )
: _type( original._type ) { }

// Default Constuctor
TurboSimParticleType::TurboSimParticleType( ) 
: _type( NO_TYPE ) { }

// Private Constructor
TurboSimParticleType::TurboSimParticleType( TurboSimParticleType::enumParticleType enumType ) 
: _type( enumType ) { }

// Accessors
string TurboSimParticleType::toString() const {
   return Enum2String( _type );
}

int TurboSimParticleType::charge() const {
   switch ( _type ) {
      case E_MINUS:
      case MU_MINUS:
      case TAU_MINUS:
      case W_MINUS:
      case P_MINUS:
      case PI_MINUS:
      case K_MINUS:
      case P_BAR:
	 return -1;
	 break;
      case E_PLUS:
      case MU_PLUS:
      case TAU_PLUS:
      case W_PLUS:
      case P_PLUS:
      case PI_PLUS:
      case K_PLUS:
      case PROTON:
	 return 1;
	 break;
      default:
	 return 0;
	 break;
   }
}

bool TurboSimParticleType::chargeConjugateNeeded() const {
   if ( charge() == -1 )
      return true;
   return false;
}

TurboSimParticleType TurboSimParticleType::chargeConjugate() const {
   switch ( _type ) {
      case E_MINUS:
         return TurboSimParticleType( E_PLUS );
      case E_PLUS:
         return TurboSimParticleType( E_MINUS );
      case TAU_MINUS:
         return TurboSimParticleType( TAU_PLUS );
      case TAU_PLUS:
         return TurboSimParticleType( TAU_MINUS );
      case MU_MINUS:
         return TurboSimParticleType( MU_PLUS );
      case MU_PLUS:
         return TurboSimParticleType( MU_MINUS );
      case W_MINUS:
         return TurboSimParticleType( W_PLUS );
      case W_PLUS:
         return TurboSimParticleType( W_MINUS );
      case P_MINUS:
         return TurboSimParticleType( P_PLUS );
      case P_PLUS:
         return TurboSimParticleType( P_MINUS );
      case PI_MINUS:
         return TurboSimParticleType( PI_PLUS );
      case PI_PLUS:
         return TurboSimParticleType( PI_MINUS );
      case K_MINUS:
         return TurboSimParticleType( K_PLUS );
      case K_PLUS:
         return TurboSimParticleType( K_MINUS );
      case PROTON:
         return TurboSimParticleType( P_BAR );
      case P_BAR:
         return TurboSimParticleType( PROTON );
      default:
         return TurboSimParticleType( _type );
   }
}

TurboSimParticleType TurboSimParticleType::baseType() const {
   switch ( _type ) {
      case E_MINUS:
      case E_PLUS:
         return TurboSimParticleType( E );
      case MU_MINUS:
      case MU_PLUS:
         return TurboSimParticleType( MU );
      case TAU_MINUS:
      case TAU_PLUS:
         return TurboSimParticleType( TAU );
	   case P_MINUS:
      case P_PLUS:
         return TurboSimParticleType( P );
	   case W_MINUS:
      case W_PLUS:
         return TurboSimParticleType( W );
	   case PI_MINUS:
      case PI_PLUS:
         return TurboSimParticleType( PI );
	   case K_MINUS:
      case K_PLUS:
         return TurboSimParticleType( K );
	   case PROTON:
      case P_BAR:
         return TurboSimParticleType( PROTON );
      default:
         return TurboSimParticleType( _type );
   }
}

double TurboSimParticleType::mass() const {
   switch ( _type ) {
      case B:
         return BOTTOM_QUARK_MASS;
      case T:
         return TOP_QUARK_MASS;
      case E_MINUS:
      case E_PLUS:
      case E:
         return ELECTRON_MASS;
      case MU_MINUS:
      case MU_PLUS:
      case MU:
         return MUON_MASS;
      case TAU_MINUS:
      case TAU_PLUS:
      case TAU:
         return TAU_MASS;
      case W_PLUS:
      case W_MINUS:
      case W:
         return W_BOSON_MASS;
      case Z:
         return Z_BOSON_MASS;
      case PI_MINUS:
      case PI_PLUS:
      case PI:
         return PI_MESON_MASS;
      case K_MINUS:
      case K_PLUS:
      case K:
         return KAON_MESON_MASS;
      case P_MINUS:
      case P_PLUS:
      case P:
         return RHO_MESON_MASS;
      case PROTON:
      case P_BAR:
         return PROTON_MASS;
      default:
         return 0.0;
   }
}

bool TurboSimParticleType::operator==( const string &rhs ) const {
   enumParticleType rhs_enum = String2Enum( rhs );
   if ( ( rhs_enum != NO_TYPE ) &&
	( rhs_enum == _type ) )
      return true;
   return false;
}

// Modifiers

bool TurboSimParticleType::read( istream &source ) {
   string temp;
   source >> temp;
   parseErrorData( temp );
   _type = String2Enum( temp );
   return ( _type != DELIMITER &&
	    _type != NO_TYPE );
}

// Static Private Utility functions
string TurboSimParticleType::Enum2String( TurboSimParticleType::enumParticleType type ) {
   switch ( type ) {

      // Quarks / Jets
      case JET:
         return "j";
      case B:
         return "b";
      case T:
         return "t";
         
      // Leptons
      case E:
         return "e";
      case E_MINUS:
         return "e-";
      case E_PLUS:
         return "e+";
      case E_NEUTRINO:
         return "nu_e";
      case MU:
         return "mu";
      case MU_MINUS:
         return "mu-";
      case MU_PLUS:
         return "mu+";
      case MU_NEUTRINO:
         return "nu_mu";
      case TAU:
         return "tau";
      case TAU_MINUS:
         return "tau-";
      case TAU_PLUS:
         return "tau+";
      case TAU_NEUTRINO:
         return "nu_tau";
      case NEUTRINO:
         return "nu";
         
      // Gauge Bosons
      case PHOTON:
         return "ph";
      case Z:
         return "z";
      case W:
         return "w";
      case W_PLUS:
         return "w+";
      case W_MINUS:
         return "w-";
         
      // RHIC objects
      case P:
         return "p";
      case P_PLUS:
         return "p+";
      case P_MINUS:
         return "p-";
      case PI:
         return "pi";
      case PI_PLUS:
         return "pi+";
      case PI_MINUS:
         return "pi-";
      case K:
         return "K";
      case K_MINUS:
         return "K-";
      case K_PLUS:
         return "K+";
         
      // Other types
      case UNCL:
         return "uncl";
      case KILL:
         return "kill";
      case PMISS:
         return "pmiss";
      case PROTON:
         return "p";
      case P_BAR:
         return "pbar";
      case UNRECOGNIZED:
         return "unknown";
      case DELIMITER:
	 return ( string() += DELIMITER_VALUE );

      // Default Cases
      case NO_TYPE:
      default:
         return "";
   }
}

TurboSimParticleType::enumParticleType TurboSimParticleType::String2Enum( const string &type ) {
   
   // for speed, get c-string pointer
   int len = type.length();
   const char *type_ptr = type.data();
   
   // sort by length . . .
   switch ( len ) {
      case 0:
         return NO_TYPE;
         break;
      case 1:
         // Look at first character only
         switch ( *type_ptr ) {
	    case DELIMITER_VALUE:   // end of object character
	       return DELIMITER;
            case 'j':
               return JET;
            case 'b':
               return B;
            case 'e':
               return E;
            case 't':
               return T;
            case 'z':
               return Z;
            case 'w':
               return W;
            case 'p':
               return P;
            case 'K':
               return K;
            case 'v':
               return NEUTRINO; // for backward compatibility
         }
         break;
      case 2:
         // Sort by 1st character, 2nd character.
         switch ( *type_ptr ) {
            // ph, pi, p-, p+
            case 'p':
               if ( *(type_ptr + 1) == 'h' )
                  return PHOTON;
               if ( *(type_ptr + 1) == '+' )
                  return P_PLUS;
                  if ( *(type_ptr + 1) == '-' )
                     return P_MINUS;
                     if ( *(type_ptr + 1) == 'i' )
                        return PI;
                        break;
               // e+ e-
            case 'e':
               if ( *(type_ptr + 1) == '+' )
                  return E_PLUS;
               if ( *(type_ptr + 1) == '-' )
                  return E_MINUS;
                  break;
               // mu
            case 'm':
               if ( *(type_ptr + 1) == 'u' )
                  return MU;
               break;
               // nu
            case 'n':
               if ( *(type_ptr + 1) == 'u' )
                  return NEUTRINO;
               // w+ w-
            case 'w':
               if ( *(type_ptr + 1) == '+' )
                  return W_PLUS;
               if ( *(type_ptr + 1) == '-' )
                  return W_MINUS;
                  break;
               // K+ K-
            case 'K':
               if ( *(type_ptr + 1) == '+' )
                  return K_PLUS;
               if ( *(type_ptr + 1) == '-' )
                  return K_MINUS;
                  break;
         }
         break;
      case 3:
         // muon
         if ( *type_ptr == 'm' )
            if ( *(type_ptr + 1) == 'u' ) {
               if ( *(type_ptr + 2) == '+' )
                  return MU_PLUS;
               if ( *(type_ptr + 2) == '-' )
                  return MU_MINUS;
            }
               // tau
               if ( *type_ptr == 't' )
                  if ( *(type_ptr + 1) == 'a' ) 
                     if ( *(type_ptr + 2) == 'u' )
                        return TAU;
         // pion
         if ( *type_ptr == 'p' )
            if ( *(type_ptr + 1) == 'i' ) {
               if ( *(type_ptr + 2) == '+' )
                  return PI_PLUS;
               if ( *(type_ptr + 2) == '-' )
                  return PI_MINUS;
            }
               break;
         case 4:
            // tau
            if ( *type_ptr == 't' )
               if ( *(type_ptr + 1) == 'a' )
                  if ( *(type_ptr + 2) == 'u' ) {
                     if ( *(type_ptr + 3) == '+' ) 
                        return TAU_PLUS;
                     if ( *(type_ptr + 3) == '-' )
                        return TAU_MINUS;
                  }
                     // nu_e
                     if ( type == "nu_e" )
                        return E_NEUTRINO;
            // kill
            if (type == "kill" )
               return KILL;
               // uncl
               if ( type == "uncl" )
                  return UNCL;
                  // pbar
                  if ( type == "pbar" )
                     return P_BAR;
                     break;
         case 5:
            // nu_mu
            if ( type == "nu_mu" )
               return MU_NEUTRINO;
            // pmiss
            if ( type == "pmiss" )
               return PMISS;
               break;
         case 6:
            // proton
            if ( type == "proton" )
               return PROTON;
            // nu_tau
            if ( type == "nu_tau" )
               return TAU_NEUTRINO;
               break;
         default:
            // unknown
            if ( type == "unknown" )
               return UNRECOGNIZED;
            break;
   }
   
   cerr << "Warning ParticleTypeString passed for conversion "
      << type << " is unknown." << endl;
   return NO_TYPE;
   
}

TurboSimParticleType::enumParticleType TurboSimParticleType::PDG2Enum( int code ) {
   
   // promote int code to an enum
   // this cast is dangerous because it may not correspond to
   // a defined enumerated constant.
   enumParticleType type = enumParticleType( code );

   // now check to see if the promotion to enum worked properly.
   // this time we cast the enum, type, as an int and compare
   // two int/s. This cast is safe.
   if ( type == code )
      return type;
   else
      return UNRECOGNIZED;
}

void TurboSimParticleType::parseErrorData( string &type ) {
   
   // parse the systematic error
   
   /* e.g. e+{0533/ph/0.292} should be e+
           e+{0533/ph/0.723} should be ph
             ^    ^  ^     ^
     Marker# 1    2  3     4               */
 	
   vector<int> marker;
   
   for ( size_t i = 0; i < type.length(); i++) {
      if( ( (marker.size()==3) && (type.substr(i,1)==",") ) ||
          ( (marker.size()==3) && (type.substr(i,1)=="}") ) ||
          ( (marker.size()==2) && (type.substr(i,1)=="/") ) ||
          ( (marker.size()==1) && (type.substr(i,1)=="/") ) ||
          ( (marker.size()==0) && (type.substr(i,1)=="{") ) )
         marker.push_back(i);
   }
   
   if ( marker.empty() )
      return;
   
   if ( marker.size() < 4 )
      assert( false );
   
   double r = atof( type.substr( marker[ 2 ] + 1, 
                                 marker[ 3 ] - marker[ 2 ] - 1 ).c_str());
   
   assert( ( r >= 0 ) && ( r <= 1 ) );
   
   if ( r < 0.5 )
      type = type.substr( 0, marker[ 0 ] );
   else
      type = type.substr( marker[ 1 ] + 1, marker[ 2 ] - marker[ 1 ] - 1 );
   
}


// Streaming Support
ostream &operator<<( ostream &out, 
                     const TurboSimParticleType &data ) {
   out << data.toString();
   return out;
}

istream &operator>>( istream &in,
                     TurboSimParticleType &data ) {
   data.read( in );
   return in;
}

vector< TurboSimParticleType > const *DefinitionOf::getAllFinals() {

   // List of all types that could be in a final state
   // ALPHABETICAL ORDER

   static vector< TurboSimParticleType > ALL_FINALS;
   
   if ( ALL_FINALS.empty() ) {
      ALL_FINALS.push_back( BJET );  // b
      ALL_FINALS.push_back( ELECTRON.chargeConjugate() ); // e+
      ALL_FINALS.push_back( ELECTRON ); // e-
      ALL_FINALS.push_back( JET ); // j
      ALL_FINALS.push_back( MUON.chargeConjugate() ); // mu+
      ALL_FINALS.push_back( MUON ); // mu-
      ALL_FINALS.push_back( NEUTRINO ); // nu
      ALL_FINALS.push_back( PHOTON ); // ph
      ALL_FINALS.push_back( PMISS ); // pmiss
      ALL_FINALS.push_back( TAU.chargeConjugate() ); // tau+
      ALL_FINALS.push_back( TAU );  // tau-
      ALL_FINALS.push_back( UNCL ); // uncl
   }

   return &ALL_FINALS;
}

vector< TurboSimParticleType> const *DefinitionOf::getBaseFinals() {

   // List of all types that could be in a final state
   // ALPHABETICAL ORDER

   static vector< TurboSimParticleType > BASE_FINALS;
   
   if ( BASE_FINALS.empty() ) {
      BASE_FINALS.push_back( BJET );  // b
      BASE_FINALS.push_back( ELECTRON.baseType() ); // e
      BASE_FINALS.push_back( JET ); // j
      BASE_FINALS.push_back( MUON.baseType() ); // mu
      BASE_FINALS.push_back( NEUTRINO ); // nu
      BASE_FINALS.push_back( PHOTON ); // ph
      BASE_FINALS.push_back( PMISS ); // pmiss
      BASE_FINALS.push_back( TAU.baseType() ); // tau
      BASE_FINALS.push_back( UNCL ); // uncl
   }

   return &BASE_FINALS;
}
