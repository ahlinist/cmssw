#include<sstream>
#include<string>
#include "Validation/VstTurboSim/interface/TurboSimEventData.hh"

using namespace std;

TurboSimEventData::TurboSimEventData() 
   : _collider( COLLIDER_NOT_SPECIFIED ),
     _detector( DETECTOR_NOT_SPECIFIED ),
     _rootS( 0 ),
     _type( "" ),
     _runNumber( 0 ),
     _eventNumber( 0 ),
     _zVertex( 0 ),
     _generatedSumPt( 0 ) { }

TurboSimEventData::TurboSimEventData( const TurboSimEventData &original ) 
   : _collider( original._collider ),
     _detector( original._detector ),
     _rootS( original._rootS ),
     _type( original._type ),
     _runNumber( original._runNumber ),
     _eventNumber( original._eventNumber ),
     _zVertex( original._zVertex ),
     _generatedSumPt( original._generatedSumPt ) {}

TurboSimEventData::ColliderType TurboSimEventData::colliderType() const {
   
   switch ( _collider ) {
      
      case LEP:
	 return LEPTON_COLLIDER;

      case HERA:
      case TEV:
      case TEV_I:
      case TEV_II:
      case LHC:
	 return HADRON_COLLIDER;

      default:
	 return DEFAULT_COLLIDER_TYPE;
   }
}


TurboSimEventData::CollisionType TurboSimEventData::collisionType() const {
   
   CollisionType collision;

   switch ( _collider ) {
      
      case LEP:
	 collision.p1 = DefinitionOf::ELECTRON;
	 collision.p2 = DefinitionOf::ELECTRON.chargeConjugate();
	 return collision;

      case HERA:
	 collision.p1 = DefinitionOf::ELECTRON;
	 collision.p2 = DefinitionOf::PROTON;
	 return collision;
	 
      case TEV:
      case TEV_I:
      case TEV_II:
	 collision.p1 = DefinitionOf::PROTON;
	 collision.p2 = DefinitionOf::PROTON.chargeConjugate();
	 return collision;

      case LHC:
	 collision.p1 = DefinitionOf::PROTON;
	 collision.p2 = DefinitionOf::PROTON;
	 return collision;

      default:
	 collision.p1 = DefinitionOf::NULL_PARTICLE;
	 collision.p2 = DefinitionOf::NULL_PARTICLE;
	 return collision;
   }
}

string TurboSimEventData::toString( int stringFormat ) const {

   string ret = "";

   if ( stringFormat & COLLIDER )
      switch ( _collider ) {
	 case LEP:
	    ret += "(LEP) ";
	    break;
	 case HERA:
	    ret += "(HERA) ";
	    break;
	 case TEV:
	    ret += "(tev) ";
	    break;
	 case TEV_I:
	    ret += "(tevI) ";
	    break;
	 case TEV_II:
	    ret += "(tevII) ";
	    break;
	 case LHC:
	    ret += "(LHC) ";
	    break;
	 default:
	    ret += "(unknown) ";
	    break;
      }

   if ( stringFormat & DETECTOR )
     switch ( _detector ) {
         case ALEPH:
	    ret += "(aleph) ";
	    break;
	 case L3:
	    ret += "(L3) ";
	    break;
	 case DELPHI:
	    ret += "(delphi) ";
	    break;
	 case OPAL:
	    ret += "(opal) ";
	    break;
	 case H1:
	    ret += "(H1)";
	    break;
	 case ZEUS:
	    ret += "(zeus) ";
	    break;
	 case CDF:
	    ret += "(cdf) ";
	    break;
	 case D0:
	    ret += "(D0) ";
	    break;
	 case CMS:
	    ret += "(cms) ";
	    break;
         case DETECTOR_NOT_SPECIFIED:
	   assert(1==0);
	   break;
      }

   if ( stringFormat & EVENT_TYPE ) 
      ret += _type + " ";

   if ( stringFormat & RUN_NUMBER ) {
     std::ostringstream oss;
     oss <<  _runNumber ;
     ret += std::string(oss.str());
   }

   if ( stringFormat & ( RUN_NUMBER + EVENT_NUMBER ) )
      ret += ".";
   else
      ret += " ";

   if ( stringFormat & EVENT_NUMBER ) {
     std::ostringstream oss;
     oss <<  _eventNumber ;
     ret += std::string(oss.str()) + " ";
   }

   return ret.substr( 0, ret.length() - 1); // remove extra space
}


// Comparators

bool TurboSimEventData::operator==( const TurboSimEventData &rhs ) const {
   return ( _collider == rhs._collider  &&
	    _detector == rhs._detector &&
	    _type == rhs._type &&
	    _runNumber == rhs._runNumber &&
	    _eventNumber == rhs._eventNumber );
}
      
bool TurboSimEventData::operator<( const TurboSimEventData &rhs ) const {
   if ( _runNumber < rhs._runNumber )
      return true;
   if ( _runNumber == rhs._runNumber )
      return ( _eventNumber < rhs._eventNumber );
   return false;
}

