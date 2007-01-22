#include "RecoBTag/CombinedSV/interface/V0Checker.h"
#include <iostream>

V0Checker::V0Checker ( double v0mass ) : vertexV0MassWindow_ ( v0mass ),
  mPiPlus_ ( 0.13957 ), mK0_ ( 0.497648 )
{}

bool V0Checker::check ( const reco::Vertex & v ) const
{
  return false;
  /*
    if (vertex.tracksSize() != 2) {                                                                               
    // message: not 2 tracks -> cannot be V0                                                                    
    return false;                                                                                               
  } // if #tracks                                                                                               
                                                                                                                
  // check if the two tracks have opposite charge                                                               
  vector < reco::Track >::const_iterator track1 = vertex.tracks_begin() ;                                       
  vector < reco::Track >::const_iterator track2 = (track1++);                                                   
  if (track1->charge() * track2->charge() != -1)                                                                
  {                                                                                                             
    // tracks have same charge -> cannot be V0                                                                  
    return false;                                                                                               
  }                                                                                                             
                                                                                                                
  // check if mass in defined mass window                                                                       
  reco::BKinematics vertexKinematics( vertex, mPiPlus_ );                                                       
  double mass = vertexKinematics.getMass();                                                                     
  if ( fabs(mass - mK0_) >= vertexV0MassWindow_) return false;                                                  
                                                                                                                
  return true;         
  */
}
