//////////////////////////////////////////////
// Return the statistical significance of an effect
// not in sigmas, but in absolute probability (Pvalue)
//
// Georgios Choudalakis, February 2007
//////////////////////////////////////////////

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "Validation/VstMathUtils/Math.hh"

using namespace std;

int main(int argc, char** argv){

  if ( argc != 4 ) {
    cout << "USAGE: " << argv[0] << " <d> <b0> <deltab>" << endl;
    return (1);
  }

  int d = atoi(argv[1]);
  double b0 = atof(argv[2]);
  double deltab = atof(argv[3]);
   
  //ignore the minus sign if d<b0. That doesn't mean that Pval>0.5, it just signifies the deficit.
  cout << Math::sigma2prob(fabs(Math::accurateMagnitudeOfDiscrepancy(d, b0, deltab))) << endl;

  return (0);
}
