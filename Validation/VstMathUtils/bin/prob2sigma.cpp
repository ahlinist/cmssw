//////////////////////////////////////////////
// use prob2sigma function from command line
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
    cout << "USAGE: " << argv[0] << " <probability>" << endl;
    return (1);
  }

  double p = atof(argv[1]);

  double sigma=Math::prob2sigma(p);
  cout << sigma << endl;

  return (0);
}
