/**********************************************
TurboSimMorphisms2 implements the creation and action of the TurboSimMorphisms2 table, 
generated from events that have been run through the full detector simulation, 
that encodes the response of the detector.

Bruce Knuteson 2003
**********************************************/

#ifndef __TurboSimMorphisms2
#define __TurboSimMorphisms2

#include <vector>
#include <string>
#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonObject.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonEvent.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsSingle.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsDouble.hh"
#include "Validation/VstTurboSim/interface/QuaeroParticleMorphismsTriple.hh"
#include "Validation/VstTurboSim/interface/TurboSimMorphisms.hh" //enum constants for AngleMeasure type

class TurboSimMorphisms2
{
public:

  /// Constructor
  TurboSimMorphisms2();

  /// Create a turboSimMorphisms table from events that have already been run through the full detector simulation
  /// Inputs:
  ///    partonFileName:  name of text file containing events at parton level, in Quaero 4-vector format
  ///      recoFileName:  name of text file containing reconstructed events from the full simulation, in Quaero 4-vector format 
  ///   targetTurboSimMorphisms2FileName:  name of text file that will contain the output turboSimMorphisms table
  ///
  void create(std::string partonFileName, std::string recoFileName, 
	      std::string targetTurboSimMorphisms2FileName);

  /// Simulate the detector response:  morph a file with parton-level events into a file with reco-level events
  void morph(std::string turboSimMorphismsFileName, 
	     std::vector<std::string> inputPartonFiles, std::string outputRecoFile);

private:

  /// Read an individual morphisms tree from a file
  void readFromFile(std::string filename);

  /// Morph a cluster of QuaeroPartonObjects
  std::vector<QuaeroRecoObject> morph(const std::vector<QuaeroPartonObject>& partonObjects, double zvtx, std::string& comment);

   // Cluster a vector of QuaeroPartonObjects
   // D
   void clusterObjects(const std::vector<QuaeroPartonObject>& partonObjects, 
					  std::vector<std::vector<QuaeroPartonObject> >& partonObjectsList,
					  std::map<std::string, std::map<int, double> >& fractionOfClustersInThisMassRangeToPair);

  /// Cluster a vector of QuaeroPartonObjects, and match to corresponding QuaeroRecoObjects
  void clusterObjects(const std::vector<QuaeroPartonObject>& partonObjects, 
					  const std::vector<QuaeroRecoObject>& recoObjects, 
					  std::vector<std::vector<QuaeroPartonObject> >& partonObjectsList, 
					  std::vector<std::vector<QuaeroRecoObject> >& recoObjectsList,
					  std::map<std::string, std::map<int, int> >& clustersInThisMassRange_total,
					  std::map<std::string, std::map<int, int> >& clustersInThisMassRange_paired);

   void interpolatePairingFractions( std::map< std::string, std::map< int, int > > &clustersInThisMassRange_total,
				     std::map< std::string, std::map< int, int > > &clustersInThisMassRange_paired );

  /*** Members ***/

  /// List of single QuaeroParticleMorphisms:  1 -> any #
  QuaeroParticleMorphismsSingle<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestSingle;

  /// List of double QuaeroParticleMorphisms:  2 -> any #
  QuaeroParticleMorphismsDouble<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestDouble;

  /// List of triple QuaeroParticleMorphisms:  3 -> any #
  QuaeroParticleMorphismsTriple<QuaeroPartonObject, QuaeroRecoObject> turboSimMorphismsForestTriple;

  /// Format for printing objects
  std::string objectPrintFormat;

  AngleMeasure angleMeasureStick, angleMeasureFat;
  double clusterDistanceStick, clusterDistanceFat, swallowDistance, mergeMass, mergeEnergy, ignoreEnergy;
  int maximumNumberOfMorphismTableRowsAbleToBeHeldInMemoryAtOneTime, maxPartonObjectsOnOneLine;

  double deltaAngle(const CLHEP::HepLorentzVector& v1, const CLHEP::HepLorentzVector& v2, AngleMeasure angleMeasure = DEFAULT);

};

#endif

