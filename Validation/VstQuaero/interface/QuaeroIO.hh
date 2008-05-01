/****************************************
Quaero I/O utility namespace
Bruce Knuteson 2003
****************************************/

#include <string>
#include <vector>

namespace QuaeroIO
{

  /*****  Opening: Parsing routines  *****/

  /// Parse options given to quaero on the command line
  void parseOptions(int argc, char* argv[]);

  /// Parse the Quaero request file from the web interface
  void parseRequestFile();


  /*****  Midgame:  File manipulations  *****/

  /// Generate Quaero signal events using Pythia
  /// Input:
  ///       colliderRun:  "tev1", "tev2", "lep2", "hera", or "lhc"
  ///    pythiaFilename:  file containing pythia commands  
  ///       sigFilename:  name of output stdhep file containing generated events
  ///
  void generateModelPythia(std::string colliderRun, std::vector<double>& resonanceMasses, std::string pythiaFilename="", std::string sigFilename="");

  /// Generate Quaero signal events using Pythia, with Susy interface to Suspect
  /// Input:
  ///       colliderRun:  "tev1", "tev2", "lep2", "hera", or "lhc"
  ///   suspectFilename:  file containing Suspect commands  
  ///       sigFilename:  name of output stdhep file containing generated events
  ///
  void generateModelSuspect(std::string colliderRun, std::string suspectFilename="", std::string sigFilename="");

  /// Generate Quaero signal events using MadEvent
  /// Input:
  ///       colliderRun:  "tev1", "tev2", "lep2", "hera", or "lhc"
  ///  MadEventFilename:  file containing MadEvent commands  
  ///       sigFilename:  name of output stdhep file containing generated events
  ///
  void generateModelMadEvent(std::string colliderRun, std::vector<double>& resonanceMasses, std::string MadEventFilename="", std::string sigFilename="");

  /// This routine keeps track of file names used during Quaero's execution
  /// Input:
  ///           type:  the type of file requested
  ///    colliderRun:  "tev2", "lep2", etc.
  ///     experiment:  "cdf", "d0", etc.
  ///     finalstate:  "1e+1e-", "5j", etc.
  ///
  std::string getFilename(std::string type, std::string colliderRun="", std::string experiment="", std::string finalstate="");

  /// Make temporary directory corresponding to specified systematic shift
  void makeTmpDirectory(int iSystematicShift=-4);

  /// Remove temporary directory corresponding to specified systematic shift
  void removeTmpDirectory(int iSystematicShift=-4);

  /*****  Endgame:  Finishing routines  *****/

  /// Inform calling process that we are finished
  void informOurParentProcessThatWeHaveFinishedSuccessfully();

  /// Produce an email with Quaero's result
  /// Input:  
  ///    logLikelihood:  Quaero's result, a single number
  ///
  void produceEmailResult(double logLikelihood, double err = 0);

  /// Produce an email with an error message
  void produceEmailError(std::string errormessage);


}
