#ifndef EventShapeLorentz_h
#define EventShapeLorentz_h

/*  \class EventShapeLorentz
*
*  Class that, given the a TLorentzvector of the input objects in the event, 
*  allows to calculate event shapes (see below)
*
*  Authors: Matthias Weber                 Date: October 2009
*
*/

#include <iomanip>
#include <vector>
#include <iostream>
#include <cmath>
#include "TLorentzVector.h"

class EventShapeLorentz {

public:

  //constructor taking as argument TLorentzvectors
  //of input object in the event
  //in the event to calculate the event shapes 
  //
  //by eta_central the central region is chosen (in pseudorapidity)
  //only objects in the central region are used for the 
  //calculation of central event shapes (with and without recoil term)
  //
  //by rapidity the choice between using the rapidity 
  //or the pseudorapidity Eta is given
  // 0: pseudorapidity eta is used 
  // 1: rapidity is used for event shape calculation- the central region
  // is still defined in terms of the pseudorapidity in this case
  // 2: rapidity is used for event shape calculation and for the selection
  //of objects in the central region
  //the choices can be defined by SetMethod(rapidity);

  EventShapeLorentz(std::vector< TLorentzVector >& PtEtaPhiE_vector,float eta_central, int rapidity);


  //Destructor
~EventShapeLorentz(){};

 std::vector<float> getEventShapes(); 
//returns the values of the event shapes

//============================================================================
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//============================================================================
//1. Beware: if you have less than two input objects the Event Shape values
//are not very meaningful - in this case the values for ALL Event Shapes are set to
//-1.0 per default 
//2. Beware: if you have less than three (central) input objects the 
//directly global (central) three-jet resolution thresholds will be set 
//to -1.0 per default
//3: In the case of less than two central input objects all central variables will 
//be set to -1.0 per default
//
//-1.0 is outside of values of all event shape variables so if you have 
//events which give -1.0 as value for an event-shape variable, the number of
//input objects was less than needed 
//============================================================================

//===============================================================
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//transverse thrust values are the tau values, tau = 1-thrust
//the event-shape variables are given back in the following order 
//directly GLOBAL: all input objects are used to calculate the variable
//00. directly global transverse thrust
//here the value of tau = 1 - thrust is returned
//01. directly global thrust minor
//CENTRAL: only central objects |eta|<eta_c are used for calculation
//02. central transverse thrust
//03. central thrust minor
//04. central total jet mass
//05. central heavy jet mass
//06. central total jet broadening - for events with two inputs 0
//07. central wide jet broadening - for events with two inputs 0
//in terms of implementation these variables use parts of the broadenings code
//thus they are defined only in the central region - 
//choose eta_c>eta_max in your preselection
//in case you want the variable to be defined globally -> Volume Variables, 0 for events with 2 inputs
//8. S3 = max(\lambda_1_U,\lambda_1_D) - values >0-10
//9. S4 = min(\lambda_1_U,\lambda_1_D) - values >0-10
//10. S5 = max(\lambda_2_U,\lambda_2_D) - values <0-1 (lower limit very close to 0)
//11. S6 = min(\lambda_2_U,\lambda_2_D) - values <0-1 (lower limit very close to 0)
//so far commented out per default (returns -1), only defined for events with 3 global (central) objects
//calculation time consuming, uses kt-clustering with R=0.6
//12. directly global three-jet resolution threshold - not done per default
//13. central three-jet resolution threshold - not done per default
//14. exponentially suppressed forward term (calculated by non-central objects)
//15. recoil term (calculated by central objects)
//=============================================================================

 std::vector<float> getThrustAxis(); //returns the normalized global thrust axis Nx, Ny, Nz=0, tau-value
 std::vector<float> getThrustAxis_C(); //returns the normalized central thrust axis Nx, Ny, Nz=0, central tau value 
 //in the case of less than two (central) objects the global (central) thrust axis 
 //is set to Nx=10,Ny=10,Nz=10, tau=-1 which is outside of range of a normalized vector

 //in the case of less than two (central) objects the global (central) grouping
 //and hemisphere infos return as entries "-1"
 std::vector<int> getGrouping(); //returns the grouping of central jets into hemispheres with "1"s and "2"s
 std::vector<int> getGrouping_C(); //returns the grouping of all jets into hemispheres with "1"s and "2"s, 
 //returns "-1" if the jet is outside of the central region


 //eta_c: choice of the central region
 //recommended: the two hardest objects should be within the central region 

 void SetEtac(float eta_central){
   eta_c = eta_central;
 }


 //wether to take the rapidity y (rap==1)  or the pseudorapidity eta (rap==0)
 void SetMethod(int rapidity){
   rap = rapidity;
 }

 private:

 int calculate();

 std::vector< TLorentzVector > Object_PtEtaPhiE;
 std::vector<float> Object_Pt;
 std::vector<float> Object_E;
 std::vector<float> Object_Phi;
 //used for the DeltaR calculation
 //if rap==0 Eta is filled, if rap==1 Rapidity
 std::vector<float> Object_Eta_Rap;
 //eta vector for the calculation of the central region
 std::vector<float> Object_Eta;
 std::vector<float> EventShapeVector;
 std::vector<float> ThrustAxis;
 std::vector<float> ThrustValues;
 std::vector<float> ThrustAxis_c;
 std::vector<int> Grouping;
 std::vector<int> Grouping_c;

 float eta_c;
 int rap;

 // helper methods

 //returns the difference in phi between two vectors
 float DeltaPhi(float, float);

 float max (float, float);

 float min (float, float);

 //the lorentz scalar product
 float lorentz_sp (std::vector<float>, std::vector<float>);

 //calculates the three-jet resolutions
 float three_jet_res(std::vector<TLorentzVector >, int);

 //calculates the thrust axis and the tau values
 std::vector<float> Thrust_calculate(std::vector< TLorentzVector >);

};

#endif
