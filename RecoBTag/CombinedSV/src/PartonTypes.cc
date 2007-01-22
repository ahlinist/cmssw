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
  if ( s=="B" ) return B;
  if ( s=="C" ) return C;
  if ( s=="UDSG" ) return UDSG;
  int i = atoi ( s.c_str() );
  if ( i > 0 ) return ( combsv::Partons::PartonType ) (i);
  if ( ( i==0 ) && s == "0" ) return ( combsv::Partons::PartonType ) (i);

  return UndefParton;
}

combsv::Partons::PartonType combsv::Partons::type ( char s )
{
  if ( s=='B' || s=='b' ) return B;
  if ( s=='C' || s=='c' ) return C;
  if ( s=='U' || s=='D' || s=='S' || s=='G' ) return UDSG;
  if ( s=='u' || s=='d' || s=='s' || s=='g' ) return UDSG;
  return UndefParton;
}
