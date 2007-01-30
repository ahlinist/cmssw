#include "RecoBTag/CombinedSV/interface/PartonTypes.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

std::string combsv::Partons::name ( PartonType p )
{
  switch ( p )
  {
    case B:
      return "B";
    case C:
      return "C";
    case UDSG:
      return "UDSG";
    default:
      return "?";
  }
}

combsv::Partons::PartonType combsv::Partons::type ( const std::string & s )
{
  if ( s.size() == 1 ) return type ( s[0] );
  if ( s=="UDSG" || s=="udsg" ) return UDSG;
  return UndefParton;
}

combsv::Partons::PartonType combsv::Partons::type ( char s )
{
  if ( s=='B' || s=='b' ) return B;
  if ( s=='C' || s=='c' ) return C;
  if ( s=='U' || s=='D' || s=='S' || s=='G' ) return UDSG;
  if ( s=='u' || s=='d' || s=='s' || s=='g' ) return UDSG;
  if ( s=='Q' || s=='q' ) return UDSG;
  int i = (int) s - 48; // simple numbers are ok, also
  if ( i>=0  && i < (int) UndefParton )
  {
    return ( combsv::Partons::PartonType ) (i);
  }
  return UndefParton;
}
