//////////////////////////////////////////////
// use sigma2prob function from command line
//
// Georgios Choudalakis, February 2007
//////////////////////////////////////////////

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "Validation/VstMathUtils/Math.hh"

using namespace std;

int main(int argc, char** argv){

  if ( argc != 2 ) {
    cout << "USAGE: " << argv[0] << " <sigmas>" << endl;
    return (1);
  }

  double sigma = atof(argv[1]);

  double p = Math::sigma2prob(sigma);
  cout << p << endl;

  return (0);
}
