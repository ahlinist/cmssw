// g++ -o $RELEASE_DIR/printRange printRange.cc -lm

#include <string>
#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

double sigFigRound(double x, int nSigFigs)
{

  // Round x to nSigFigs significant digits
  
  if(x==0)
    return(0);
  int sign = (x>0) - (x<0);
  x = fabs(x);
  int a = (int)(log10(x) + (x>=1.));
  double ans = (int)(x/pow(10.,a-nSigFigs) + .5) * pow(10.,a-nSigFigs);
  ans *= sign;
  return(ans);
}

int main(int argc, char* argv[])
{
  if(argc!=3)
    {
      cout << "Usage:  sigFigRound <float> <significant digits>" << endl;
      exit(1);
    }
  assert(argc==3);
  cout << sigFigRound(atof(argv[1]), atoi(argv[2])) << endl;
  return(0);
}

