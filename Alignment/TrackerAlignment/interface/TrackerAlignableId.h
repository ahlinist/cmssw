#ifndef Alignment_CommonAlignment_TrackerAlignableId_H
#define Alignment_CommonAlignment_TrackerAlignableId_H

#include <map>

#include "Alignment/CommonAlignment/interface/Alignable.h"
#include "Alignment/CommonAlignment/interface/AlignableDet.h"
#include "Alignment/CommonAlignment/interface/AlignableObjectId.h"

/// Helper class to provide unique numerical ID's for Alignables. 
/// The unique ID is formed from:
///  - the AlignableObjectId (DetUnit, Det, Rod, Layer, etc.)
///  - the geographical ID of the first GeomDet in the composite.
/// A mapping between the AlignableObjectId and the string name
/// is also provided.

class GeomDet;
class Alignable;

class TrackerAlignableId
{

public:
  
  typedef AlignableObjectId::AlignableObjectIdType idType;
  typedef std::map<int,std::string> MapEnumType;

  /// Constructor (builds map)
  TrackerAlignableId( );

  /// Destructor
  ~TrackerAlignableId() {};

  /// Return geographical ID of first GeomDet
  unsigned int alignableId( Alignable* alignable );

  /// Return Type ID (Det, Rod etc.) of Alignable
  int alignableTypeId( Alignable* alignable ); 

  /// Return type and layer of Alignable
  std::pair<int,int> typeAndLayerFromAlignable( Alignable* alignable );

  /// Return type and layer of GeomDet
  std::pair<int,int> typeAndLayerFromGeomDet( const GeomDet& geomDet );

  /// Return string corresponding to given Alignable
  const std::string alignableTypeName( const Alignable* alignable ) const;

  /// Return string corresponding to given alignable object ID
  const std::string alignableTypeIdToName( const int& id ) const;
  

private:

  /// Get first AlignableDet of an Alignable
  AlignableDet* firstDet( Alignable* alignable );
 
  /// Get unique identifyer of first AlignableDet of alignable
  unsigned int firstDetId( Alignable* alignable );

  // Map between AlignableObjectId and string name
  MapEnumType theMap;

};

#endif
