#ifndef __TURBOSIMEVENTDATA_H
#define __TURBOSIMEVENTDATA_H

class TurboSimObject;

#include <string>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstTurboSim/interface/TurboSimParticleType.hh"

class TurboSimEventData {

public:
   
   enum Collider { SLAC, LEP, HERA, TEV_I, TEV_II, TEV, LHC, ILC, 
		   COLLIDER_NOT_SPECIFIED };

   enum Detector { ALEPH, L3, OPAL, DELPHI, 
		   H1, ZEUS, 
		   CDF, D0, 
		   CMS, 
		   DETECTOR_NOT_SPECIFIED };

   enum ColliderType { HADRON_COLLIDER, LEPTON_COLLIDER,
		       DEFAULT_COLLIDER_TYPE };

   struct CollisionType {
      TurboSimParticleType p1;
      TurboSimParticleType p2;
   };

   // Constructors
   TurboSimEventData( const TurboSimEventData &original );
   
   // Accessors
   
   const TurboSimEventData *eventDataPointer() const {
      return this;
   }

   // Data members
   Collider collider() const { return _collider; }
   Detector detector() const { return _detector; }
   /**
    * Inlined accessor that returns center of mass energy of event.
    *
    * @return center of mass energy (in GeV)
    */
   double rootS() const { return _rootS; }

   int runNumber() const { return _runNumber; }
   int eventNumber() const { return _eventNumber; }
   const std::string &eventType() const { return _type; }
   
   /**
    * Inlined accessor that returns z vertex of primary vertex of event.
    *
    * @return Z position of primary vertex (in cm)
    */
   double zVertex() const { return _zVertex; }

   /**
    * Inlined accessor that returns the generated summed transverse momentum
    * of the event.
    *
    * @return Generated summed transverse momentum (in GeV)
    */
   double generatedSumPt() const { return _generatedSumPt; }

   // derived data
   CollisionType collisionType() const;
   ColliderType colliderType() const;

   enum StringFormat { COLLIDER = 1, 
		       DETECTOR = 2,
		       EVENT_TYPE = 4,
		       RUN_NUMBER = 8,
		       EVENT_NUMBER = 16 };

   std::string toString( ) const { return toString( EVENT_TYPE | 
						    RUN_NUMBER | 
						    EVENT_NUMBER ); }
   std::string toString( int stringFormat ) const;

   // Comparators
   bool operator==( const TurboSimEventData& ) const;
   bool operator< ( const TurboSimEventData& ) const;

   // Modifiers
   bool read( std::istream& );

protected:

   // Constructor
   TurboSimEventData();
   
   // Collider / Detector / Experiment info
   Collider _collider;
   Detector _detector;
   int _rootS;   // in GeV

   // Event Info
   std::string _type;
   CollisionType _collisionType;
   int _runNumber;
   int _eventNumber;
   double _zVertex, _generatedSumPt;

};

#include "Validation/VstTurboSim/interface/TurboSimObject.hh"

#endif
