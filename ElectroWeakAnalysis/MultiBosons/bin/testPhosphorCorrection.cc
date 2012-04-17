#include <iostream>
#include <iomanip>
#include <vector>
#include "ElectroWeakAnalysis/MultiBosons/interface/PhosphorCorrectionFunctor.h"

using namespace std;
using namespace vgamma;

void testPhosphorCorrection();
void dumpCorrectionFactors();
void testMonteCarlo2011APhotonCorrection();
void testMonteCarlo2011BPhotonCorrection();
void testMonteCarlo2011allPhotonCorrection();
void testMonteCarloPhotonCorrectionAndSmearingTo2011A();
void testMonteCarloPhotonCorrectionAndSmearingTo2011B();
void testMonteCarloPhotonCorrectionAndSmearingTo2011all();
void testRealDataPhotonCorrection();
void testRealDataPhotonCorrectionAveraged2011all();

///____________________________________________________________________________
/// Entry point for execution of a stand-alone binary
int main (int argc, char ** argv)
{
  testPhosphorCorrection();
  return 0;
}


///____________________________________________________________________________
/// Entry point of execution as a ROOT Macro
void testPhosphorCorrection()
{
  cout << "Welcome to PHOSPHOR correction test!" << endl << endl;

  /// Test the correction.
  // dumpCorrectionFactors();
  testMonteCarlo2011APhotonCorrection();
  testMonteCarlo2011BPhotonCorrection();
  testMonteCarlo2011allPhotonCorrection();
  testMonteCarloPhotonCorrectionAndSmearingTo2011A();
  testMonteCarloPhotonCorrectionAndSmearingTo2011B();
  testMonteCarloPhotonCorrectionAndSmearingTo2011all();
  testRealDataPhotonCorrection();
  testRealDataPhotonCorrectionAveraged2011all();
  
  cout << "Exiting PHOSPHOR correction test with success." << endl;
} // testPhosphorCorrectionInRoot()


///____________________________________________________________________________
/// Dump the correction factors.
void dumpCorrectionFactors() {
  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor myCorrector(
    PhosphorCorrectionFunctor::kMonteCarlo2011A, ""
  );
  
  vector<double> const cfactors = myCorrector.correctionFactors();
  for (unsigned cat = 0; cat < cfactors.size(); ++cat) {
    cout << cat << " " << cfactors[cat] << endl;
  }
} // dumpCorrectionFactors()


///____________________________________________________________________________
/// Tests the energy correction for Monte Carlo photons with 2011B PU.
void testMonteCarlo2011APhotonCorrection() {

  cout << "== MC Correction for 2011A PU ==" << endl;
  
  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarlo2011A, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarlo2011APhotonCorrection()


///____________________________________________________________________________
/// Tests the energy correction for Monte Carlo photons with 2011B PU.
void testMonteCarlo2011BPhotonCorrection() {

  cout << "== MC Correction for 2011B PU ==" << endl;

  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarlo2011B, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarlo2011BPhotonCorrection()


///____________________________________________________________________________
/// Tests the energy correction for Monte Carlo photons with 2011A+B PU.
void testMonteCarlo2011allPhotonCorrection() {

  cout << "== MC Correction for 2011A+B PU ==" << endl;

  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarlo2011all, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarlo2011allPhotonCorrection()


///____________________________________________________________________________
/// Tests the energy correction and smearing for Monte Carlo photons to match
/// the resolutin for 2011A data.
void testMonteCarloPhotonCorrectionAndSmearingTo2011A() {

  cout << "== MC Correction and Smearing for 2011A PU ==" << endl;

  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarloSmearedTo2011A, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarloPhotonCorrectionAndSmearingTo2011A()


///____________________________________________________________________________
/// Tests the energy correction and smearing for Monte Carlo photons to match
/// the resolutin for 2011B data.
void testMonteCarloPhotonCorrectionAndSmearingTo2011B() {

  cout << "== MC Correction and Smearing for 2011B PU ==" << endl;

  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarloSmearedTo2011B, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarloPhotonCorrectionAndSmearingTo2011B()


///____________________________________________________________________________
/// Tests the energy correction and smearing for Monte Carlo photons to match
/// the resolutin for 2011A+B data.
void testMonteCarloPhotonCorrectionAndSmearingTo2011all() {

  cout << "== MC Correction and Smearing for 2011A+B PU ==" << endl;

  /// Instantiate the correction functor for MC.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kMonteCarloSmearedTo2011all, ""
  );

  /// Define some example MC photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  double genEnergy = 15.9;
  
  cout << "  pt   eta  Egen  ptCorrected  (comment):" << endl
       << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << endl;

  /// Check behavior if not gen-match found
  genEnergy = 0;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (no gen match)" << endl;


  /// Check behavior for pt < 10 GeV
  pt = 9.87;
  genEnergy = 13.5;
  cout << pt << "  " << eta << "  " << setw(4) << genEnergy << "  "
       /// Use the correction functor to get a corrected et.
       << setw(7) << getCorrectedPhotonPt(pt, eta, genEnergy)
       << " (pt extrapolation)" << endl << endl;

} // testMonteCarloPhotonCorrectionAndSmearingTo2011all()


///____________________________________________________________________________
/// Tests the energy correction for photons in real data measured separately
/// for 2011A and 2011B.
void testRealDataPhotonCorrection() {
  
  cout << "== Data Correction Separtely for 2011A and 2011B ==" << endl;

  /// Instantiate the correction functor for real data.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kRealData, ""
  );

  /// Define some example 2011A real-data photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  int runNumber = 123456;
  
  cout << "  pt   eta    run   ptCorrected  (comment): " << endl
       << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011A)" << endl;
  
  /// Change the run number to correspond to 2011B
  runNumber = 234567;
  
  cout << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011B)" << endl;

  /// Test the low-pt
  pt = 9.87;
  cout << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011B, low pt)" << endl << endl;
  
} // testRealDataPhotonCorrection()


///____________________________________________________________________________
/// Tests the energy correction for photons in real data averaged over both
/// 2011A and 2011B.
void testRealDataPhotonCorrectionAveraged2011all() {

  cout << "== Data Correction Averged for 2011A and 2011B ==" << endl;
  
  /// Instantiate the correction functor for real data.
  PhosphorCorrectionFunctor getCorrectedPhotonPt(
    PhosphorCorrectionFunctor::kRealDataAverage2011all, ""
  );

  /// Define some example 2011A real-data photon quantities.
  double pt = 12.3;
  double eta = -0.8;
  int runNumber = 123456;
  
  cout << "  pt   eta    run   ptCorrected  (comment): " << endl
       << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011A)" << endl;
  
  /// Change the run number to correspond to 2011B
  runNumber = 234567;
  
  cout << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011B)" << endl;

  /// Test the low-pt
  pt = 9.87;
  cout << pt << "  " << eta << "  " << runNumber << "  "
       /// Use the correction functor to get a corrected et:
       << setw(7) << getCorrectedPhotonPt(pt, eta, runNumber) 
       << "  (2011B, low pt)" << endl << endl;
  
} // testRealDataPhotonCorrection()

