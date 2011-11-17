#ifndef EventShape_h
#define EventShape_h

/*  \class EventShape
*
*  Class that, given the 4-momenta of the objects in the event, 
*  allows to calculate event shapes (see below)
*
*  Authors: Matthias Weber                 Date: July/October 2007
*
*/

#include <vector>
#include <iostream>
#include <cmath>

class EventShape {

public:

  //constructor taking as argument vectors of 
  //Px, Py, Pz and E of input Objects 
  //in the event to calculate the event shapes 
  //
  //by eta_central the central region is chosen (in pseudorapidity)
  //only objects in the central region are used for the 
  //calculation of central event shapes (with and without recoil term)
  //
  //by rapidity the choice between using the rapidity 
  //or the pseudorapidity Eta is given
  // 0: pseudorapidity eta is used 
  // 1: rapidity is used - the central region
  // is still defined in terms of the pseudorapidity in this case
  //the choices are then defined by SetMethod(rapidity);

  EventShape(std::vector<double>& Px_vector, std::vector<double>& Py_vector, std::vector<double>& Pz_vector,std::vector<double>& E_vector, double eta_central, int rapidity);


  //Destructor
~EventShape(){};

 std::vector<double> getEventShapes(); 
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
//00. directly global transverse thrust
//here the value of tau = 1 - thrust is returned
//01. directly global thrust minor
//02. directly global three-jet resolution threshold
//03. central transverse thrust
//04. central transverse thrust with exponentially suppressed forward term 
//her the value tau + exp is returned here
//05. central transverse thrust with recoil term
//here the value of tau + rec is returned
//06. central thrust minor
//07. central thrust minor with exponentially suppressed forward term
//08. central thrust minor with recoil term
//09. central total jet mass
//10. central total jet mass with exponentially suppressed forward term
//11. central total jet mass with recoil term
//12. central heavy jet mass
//13. central heavy jet mass with exponentially suppressed forward term
//14. central heavy jet mass with recoil term
//15. central three-jet resolution threshold
//16. central three-jet resolution threshold with exponentially suppressed forward term
//17. central three-jet resolution threshold with recoil term
//18. central total jet broadening 
//in the case of two central jets the jet broadenings are 0
//19. central total jet broadening with exponentially suppressed forward term
//20. central total jet broadening with recoil term
//21. central wide jet broadening 
//22. central wide jet broadening with exponentially suppressed forward term
//23. central wide jet broadening with recoil term
//24. central total transverse jet mass
//25. central total transverse jet mass with exponentially suppressed forward term
//26. central total transverse jet mass with recoil term
//27. central heavy transverse jet mass
//28. central heavy transverse jet mass with exponentially suppressed forward term
//29. central heavy transverse jet mass with recoil term
//=============================================================================

 std::vector<double> getThrustAxis(); //returns the normalized global thrust axis Nx, Ny, Nz=0
 std::vector<double> getThrustAxis_C(); //returns the normalized central thrust axis Nx, Ny, Nz=0 
 //in the case of less than two (central) objects the global (central) thrust axis 
 //is set to Nx=10,Ny=10,Nz=10 which is outside of range of a normalized vector

 //in the case of less than two (central) objects the global (central) grouping
 //return as entries "-1"

 std::vector<int> getGrouping(); //returns the grouping of central jets into hemispheres with "1"s and "2"s
 std::vector<int> getGrouping_C(); //returns the grouping of all jets into hemispheres with "1"s and "2"s, 
 //returns "-1" if the jet is outside of the central region


 //eta_c: choice of the central region
 //recommended: the two hardest objects should be within the central region 

 void SetEtac(double eta_central){
   eta_c = eta_central;
 }


 //wether to take the rapidity y (rap==1)  or the pseudorapidity eta (rap==0)
 void SetMethod(int rapidity){
   rap = rapidity;
 }

 private:

 int calculate();

 std::vector<double> Object_Px;
 std::vector<double> Object_Py;
 std::vector<double> Object_Pz;
 std::vector<double> Object_Pt;
 std::vector<double> Object_E;
 std::vector<double> Object_Phi;
 //used for the DeltaR calculation
 //if rap==0 Eta is filled, if rap==1 Rapidity
 std::vector<double> Object_Eta_Rap;
 //eta vector for the calculation of the central region
 std::vector<double> Object_Eta;
 std::vector<double> EventShapes;
 std::vector<double> ThrustAxis;
 std::vector<double> ThrustAxis_c;
 std::vector<int> Grouping;
 std::vector<int> Grouping_c;

 double eta_c;
 int rap;

 // helper methods

 //returns the difference in phi between two vectors
 double DeltaPhi(double, double);

 double max (double, double);

 double min (double, double);

 //the lorentz scalar product
 double lorentz_sp (std::vector<double>, std::vector<double>);

 //calculates the three-jet resolutions
 double three_jet_res(std::vector<double>,std::vector<double>,std::vector<double>,std::vector<double>, int);

 //calculates the thrust axis and the tau values
 std::vector<double> Thrust_calculate(std::vector<double>, std::vector<double>);

};

#endif
