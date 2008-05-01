//////////////////////////////////////////////
// use poisson function from command line
//
// Georgios Choudalakis, May 2007
//////////////////////////////////////////////

#include <stdio.h>
#include <cmath>
#include <iostream>
#include "Validation/VstMathUtils/interface/Math.hh"

using namespace std;

int main(int argc, char** argv){

  if ( argc != 2 && argc != 3 ) {
    cout << "USAGE: " << argv[0] << " <mean> [deltaMean]" << endl;
    return (1);
  }

  srand48((unsigned)(time(NULL)*getpid()));

  double mu0 = atof(argv[1]);
  double mu = mu0;
  if ( argc == 3 ) { // pull the mean from a gaussian.
    double deltaMu = atof(argv[2]);
    mu = Math::gasdev(mu0,deltaMu);
    while ( mu <= 0 ) { //Make sure it's not negative or 0
      mu = Math::gasdev(mu0,deltaMu);
    }
  }

  double ans=Math::poisson(mu);
  cout << ans << endl;

  return (0);
}
