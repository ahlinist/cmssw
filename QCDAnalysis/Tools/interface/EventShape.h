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
  //by rapidity the choice between using the rapidity 
  //or the pseudorapidity Eta is given
  // 0: pseudorapidity eta is used (recommended)
  // 1: rapidity is used - please note that the central region
  // is defined in terms of the real rapidity in this case
  // so you should use it only in the case of 
  // massless input objects
  //the choices are then defined by SetMethod(rapidity);

  EventShape(std::vector<double> Px_vector, std::vector<double> Py_vector, std::vector<double> Pz_vector,std::vector<double> E_vector, double eta_central, int rapidity);


  //Destructor
~EventShape(){};

 std::vector<double> getEventShapes(); 
//returns the values of the event shapes

//============================================================================
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//============================================================================
//beware if you have less than three (central) input objects the 
//directly global (central) three-jet resolution thresholds will be set 
//to -1.0 per default
//in the case of no central input object the central variables will 
//be set to -1.0 per default
//-1.0 is outside of values of all event shape variables so if you have 
//events which give -1.0 as value for an event shape,they had 
//less input momenta than needed for the calculations
//============================================================================

//===============================================================
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//transverse thrust values are the tau values, tau = 1-thrust
//in the following order 
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

 std::vector<double> getThrustAxis(); //returns the global thrust axis Nx, Ny, Nz=0
 std::vector<double> getThrustAxis_C(); //returns the central thrust axis Nx, Ny, Nx=0


 //eta_c: choice of the central region
 //recommended: the two hardest jets should be within the central region

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
 std::vector<double> Object_P;
 std::vector<double> Object_Pt;
 std::vector<double> Object_E;
 std::vector<double> Object_Phi;
 std::vector<double> Object_Eta;
 std::vector<double> EventShapes;
 std::vector<double> ThrustAxis;
 std::vector<double> ThrustAxis_c;
 double eta_c;
 int rap;
 int y3_recom;

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
