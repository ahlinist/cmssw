
#include "Validation/VstMathUtils/interface/matrix.hh"
#include "Validation/VstOptimalBinning/interface/OptimalBinning.hh"

using namespace std;

int main(int argc, char* argv[])
{

  vector<double> improvement;
  vector<double> binPoints;
  double epsilonWt = 0.01;
  int N = 1000;
  vector<double> hWt = vector<double>(N,epsilonWt);
  vector<double> bWt = vector<double>(N,epsilonWt);
  vector<double> hD, bD;
  for(int i=0; i<N; i++)
    hD.push_back(drand48()*2);
  for(int i=0; i<N; i++)
    bD.push_back(drand48());
  
  OptimalBinning::chooseBinPoints(hD, hWt, bD, bWt, binPoints, improvement, epsilonWt);

  cout << "Bin points:" << endl << "  " << flush;  print(binPoints);

  cout << endl << "done!" << endl;

  return 0;
}

