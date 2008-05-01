#ifndef __TurboSimRule_Utilities_h
#define __TurboSimRule_Utilities_h

#include "Validation/VstMathUtils/interface/Math.hh"

// function defintions in Validation/VstTurboSim/interface/TurboSimRule_Utilities.ii
// definitions compiled from src/TurboSimRule.cc via line
// #include "Validation/VstTurboSim/interface/TurboSimRule_Utilities.ii"

namespace TSRule {

   /**
    * Examines event for non-null particles.
    *
    * @param event Event of interest
    * @return True if event has at least one non-null particle.
    */
   bool hasParticles( const TurboSimEvent &event );
   bool hasParticles( const TurboSimObject &obj );
   bool hasRecoParticles( const TurboSimEvent &event );
   bool hasPartonParticles( const TurboSimEvent &event );
   bool notEmpty( const TurboSimEvent &event );



   enum AngleMeasure {
      DEFAULT,
            DELTA_R,
      DELTA_ANGLE,
      MANHATTAN_DISTANCE,
      KT_DURHAM };
   
   double deltaAngle( const HepLorentzVector& v1, 
		      const HepLorentzVector& v2, 
		      AngleMeasure angleMeasureMethod );

   std::string getFileName( const std::string &baseFilename,
			    const std::vector< std::string > &tags,
			    const std::string &type = "");
} // TSRule namespace

#endif
