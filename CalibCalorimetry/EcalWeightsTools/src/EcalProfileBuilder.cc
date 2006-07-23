/**
 * \file EcalProfileBuilder.cc 
 * Determination of channel signal pulse profile to be used
 * in the calculation of optimized weights for Test Beam Anlysis
 *
 * NOTE: this version can only run on Laser data!!
 *
 * $Date: 2006/07/19 10:29:21 $
 * $Revision: 1.1 $
 * Alexandre Zabi - Imperial College
*/

#include "CalibCalorimetry/EcalWeightsTools/interface/EcalProfileBuilder.h"

#include <iostream>
#include <fstream>
#include <vector>

EcalProfileBuilder::EcalProfileBuilder(edm::ParameterSet const& pSet)
{
  //verbosity
  verbosity_        = pSet.getUntrackedParameter("verbosity", 1U);

  //Prints out 
  debug_            = pSet.getUntrackedParameter<bool>("debug", false);

  //SPECIFY WHICH SUPERMODULE
  SuperModule_      = pSet.getUntrackedParameter<unsigned int>("SuperModule", 0);

}//CONSTRUCTOR

EcalProfileBuilder::~EcalProfileBuilder()
{

}//DESTRUCTOR

void EcalProfileBuilder::analyze(const edm::Event& evt, const edm::EventSetup& evtSetup) 
{
   std::cout << "HELLO" << std::endl;
	
}//ANALYZE
