/**********************************************
TurboSimMorphisms1 implements the creation and action of the TurboSimMorphisms1
table, generated from events that have been run through the full detector 
simulation, that encodes the response of the detector.

Primary Author: Bruce Knuteson 2003
Editor: David Friend, friend@mit.edu
**********************************************/

#ifndef __TurboSimMorphisms1
#define __TurboSimMorphisms1

#include <vector>
#include <string>
#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonObject.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsSingle.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsDouble.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsTriple.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms.hh"

class TurboSimMorphisms1 {

public:

  // Constructor
  TurboSimMorphisms1();

  /* TurboSimMorphisms1::create ( . . .)
     ABSTRACT
       Create a turboSimMorphisms table from events that have already been run 
       through the full detector simulation.
     INPUT:
       partonFileName:  name of text file containing events at parton level, in
                        Quaero 4-vector format.
       recoFileName:  name of text file containing reconstructed events from 
                      the full simulation, in Quaero 4-vector format 
       targetTurboSimMorphisms1FileName:  name of text file that will contain 
                                           the output turboSimMorphisms table
     OUTPUT:
       TurboSim Morphisms table to file.
       void return.
  */
  void create(const std::string & partonFileName, 
	      const std::string & recoFileName, 
	      const std::string & targetTurboSimMorphisms1FileName);

  // TurboSimMorphisms1::readFromFile ( . . .)
  // ABSTRACT: Read in TurboSimMorphisms1 table
  void readFromFile(const std::string & filename);

  /* TurboSimMorphisms1::morph ( . . )
     ABSTRACT: 
       Simulate the detector response: morph the parton-level event
       partonEvent.
     INPUT: 
       partonEvent: event to morph
     OUTPUT:
       returns QuaeroEvent object with morphed event.
     NOTES: This is an overloaded function.
  */
  QuaeroEvent morph(const QuaeroPartonEvent & partonEvent);

private:

  // Morph a cluster of QuaeroPartonObjects
  std::vector<QuaeroRecoObject> morph(const std::vector<QuaeroPartonObject> & partonObjects, 
				      double zvtx, 
				      std::string & comment);

  // Cluster a vector of QuaeroPartonObjects
  void clusterObjects(const std::vector<QuaeroPartonObject>& partonObjects, 
					 std::vector<std::vector<QuaeroPartonObject> >& partonObjectsList);

  // Cluster a vector of QuaeroPartonObjects, and match to corresponding QuaeroRecoObjects
  void clusterObjects(const std::vector<QuaeroPartonObject>& partonObjects, 
					 const std::vector<QuaeroRecoObject>& recoObjects, 
					 std::vector<std::vector<QuaeroPartonObject> >& partonObjectsList, 
					 std::vector<std::vector<QuaeroRecoObject> >& recoObjectsList);

  /*** Members ***/

  // List of single QuaeroParticleMorphisms:  1 -> any #
  QuaeroParticleMorphismsSingle<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestSingle;

  // List of double QuaeroParticleMorphisms:  2 -> any #
  QuaeroParticleMorphismsDouble<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestDouble;

  // List of triple QuaeroParticleMorphisms:  3 -> any #
  QuaeroParticleMorphismsTriple<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestTriple;

  // Format for printing objects
  std::string objectPrintFormat;

  AngleMeasure angleMeasureStick, angleMeasureFat;
  double clusterDistanceStick, clusterDistanceFat, swallowDistance, mergeMass, mergeEnergy, ignoreEnergy;
  int maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime, maxPartonObjectsOnOneLine;

 
  double deltaAngle(const CLHEP::HepLorentzVector& v1, 
		    const CLHEP::HepLorentzVector& v2,
  		    AngleMeasure angleMeasureMethod = DEFAULT);


};

#endif

