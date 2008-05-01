#ifndef __TurboSimRecoObject_h
#define __TurboSimRecoObject_h

#include "Validation/VstTurboSim/interface/TurboSimObject.hh"

class TurboSimPartonObject;

/** 
 * A <code>TurboSimObject</code> associated with a <code>TurboSimPartonObject</code>
 * 
 * Holds a pointer to a <code>TurboSimPartonObject</code> with which this
 * reconstrtucted object is associated. Also holds a pointer to the event
 * that was used to transform this object.
 *
 * @author David Friend, friend at mit dot edu
 * @version 1
 */

class TurboSimRecoObject : public TurboSimObject {
 
public: 

   // Constructors
   TurboSimRecoObject( const TurboSimObject &original );
   TurboSimRecoObject( const TurboSimObject &object, 
		       TurboSimPartonObject *partonObject,
		       const TurboSimEventData *eventData = NULL );
   TurboSimRecoObject( );

   // Accessors
   const TurboSimPartonObject *partonObject() const {
      return _partonObject; }
  TurboSimPartonObject *partonObject() {
    return _partonObject; }
   const TurboSimEventData *transformationEvent() const {
      return _transformationEvent; }

   // Modifiers
   void setPartonObject( TurboSimPartonObject *ptr ) {
      _partonObject = ptr; }
   void setTransformationEvent( const TurboSimEventData *ptr ) {
      _transformationEvent = ptr; }
   void clear() {
      _partonObject = NULL;
      _transformationEvent = NULL;
      TurboSimObject::clear(); }

private:

   TurboSimPartonObject *_partonObject;
   const TurboSimEventData *_transformationEvent;
   

};

#include "Validation/VstTurboSim/interface/TurboSimPartonObject.hh"

#endif
