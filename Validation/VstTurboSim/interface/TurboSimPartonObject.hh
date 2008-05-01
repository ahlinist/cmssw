#ifndef __TURBOSIMPARTONOBJECT_H
#define __TURBOSIMPARTONOBJECT_H

#include "Validation/VstTurboSim/interface/TurboSimObject.hh"

class TurboSimRecoObject;


/** 
 * A <code>TurboSimObject</code> associated with a <code>TurboSimRecoObject</code>.
 * 
 * Holds a pointer to a <code>TurboSimRecoObject</code>, to which this parton
 * object is associated with. 
 *
 * @author David Friend, friend at mit dot edu
 * @version 1
 */

class TurboSimPartonObject : public TurboSimObject {
 
public: 

   // Constructors
   TurboSimPartonObject( const TurboSimObject &original );
   TurboSimPartonObject( const TurboSimObject &object, 
			 TurboSimRecoObject *recoObject );
   TurboSimPartonObject( double weight = 1 );

   // Accessors
   const TurboSimRecoObject *recoObject() const {
      return _recoObject; }
   TurboSimRecoObject *recoObject() {
      return _recoObject; }

   // Modifiers
   void setRecoObject( TurboSimRecoObject *ptr ) {
      _recoObject = ptr; }
   void clear() {
      _recoObject = NULL; }

private:

   TurboSimRecoObject *_recoObject;

};

#include "Validation/VstTurboSim/interface/TurboSimRecoObject.hh"

#endif
