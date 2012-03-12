//////////////////////////////////////////////////////////////////
//
// counting_zscore.C
//
// This is an example how to use roostats_cl95.C
// to compute a z-score (significance) of an excess
// over background, for a counting experiment
//
// Gena Kukartsev, March 2012
//
// Usage:
//        - start root
//        - compile and run this routine
//
//          [1] .L counting_zscore.C+
//          [2] zscore()
//

#include <iostream>

#include "roostats_cl95.C"



void zscore( void ){
  //
  // compute z-score in a counting experiment
  //

  double ilum = 1.0;
  double slum = 0.1;
  double  eff = 1.0;
  double seff = 0.1;
  double  bkg = 1.0;
  double sbkg = 0.1;

  int    data = 2;

  // optional: set some parameters
  SetParameter("RandomSeed", 1111);
  SetParameter("CalculatorType", 0); // 0 for frequentist
  SetParameter("TestStatType", 2); // LHC-style profile likelihood
  SetParameter("NToys", 1000);
  SetParameter("Optimize", true);

  double zscore = roostats_zscore(ilum,slum, eff,seff, bkg,sbkg, data);

  std::cout << "Z-score = " << zscore << std::endl;

  return;
}

