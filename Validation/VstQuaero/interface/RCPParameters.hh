
#ifndef __RCPParameters
#define __RCPParameters

#include <vector>
#include <string>
#include <map>
#include "Validation/VstQuaeroUtils/interface/ChatterTab.hh"
#include "Validation/VstQuaeroUtils/interface/Refine.hh"
#include "Validation/VstQuaero/interface/SystematicShift.hh"
#include "Validation/VstQuaero/interface/GraphicalOutput.hh"


namespace RCPParameters
{
  class EpsilonWt
  {
  public:
    EpsilonWt() {};
    void store(std::string colliderRun, std::string experiment, double epsilonWt);
    double get(std::string colliderRun, std::string experiment);
    double get();
  private:
    std::vector<std::string> colliderRunExperiments;
    std::vector<double> epsilonWeights;
  };
  extern int debugLevel;
  extern std::string discriminatingAlgorithm;
  extern bool useSlowMethodForHandlingErrorDueToNumberOfMonteCarloEvents;
  extern EpsilonWt epsilonWt;
  extern double weightQuantumForDesiredSmoothnessOfLogL;
  extern std::vector<double> desiredGeneratedLuminosity(std::string colliderRun);
  extern std::map<std::string,std::vector<double> > actualGeneratedLuminosity;
  extern double collectedLuminosity(std::string colliderRun);
  extern std::vector<std::string> requestorInfo;
  extern std::string modelDescription;
  extern HintSpec hintSpec;
  struct SignalFileStorage
  {
    string id;
    double scaleFactor;
  };
  extern SignalFileStorage signalFileStorage;
  extern std::map<std::string, std::vector<std::string> > colliderExperiments; // key is colliderRun, value is a vector of experiments
  extern std::map<std::string, int > privilegeLevel; // key is colliderRun+" "+experiment, value is user's priviledge level
  extern std::map<std::string, std::vector<std::string> > leaveOut; // key is colliderRun, value is a vector of backgrounds to leave out
  extern std::map<std::string,std::string> signalFormat; // key is colliderRun, value is "pythia", "madevent", "suspect", or "stdhep"
  extern std::map<std::string,std::vector<double> > signalXsec;  // signalXsec[colliderRun="lep2","hera","tev2"][iColliderSubRun] = 1.4 pb, for example
  extern std::string batchQueue;
  void write(std::string filename);
  void read(std::string filename);
  extern std::string credit(std::string colliderRun="", std::string experiment="", std::string finalState="");
  extern int targetTime;
  extern int nSignalEvents, nTurboSimTableLines, nBkgEvents, nFewKdeTrials, nSystematicShifts;
}

namespace GlobalVariables
{
  extern std::string id;
  extern std::string localDir;
  extern ChatterTab chatterTab;
  extern SystematicShift systematicShift;
  // iSystematicShift==-4:  initial state, setting up directories
  // iSystematicShift==-3:  partition <collider>-Quaero, see where signal lands, and determine useful final states
  // iSystematicShift==-2:  partition <collider>-Vista, and construct kernel density estimates
  // iSystematicShift==-1:  compute discriminant bin edges
  // iSystematicShift== 0:  fill discriminant bin, make plots, zero systematic shift
  // iSystematicShift== 1:  run analysis, zero systematic shift
  // iSystematicShift>= 2:  run analysis, imposing non-zero systematic shifts, for integration over systematic errors
  extern int iSystematicShift;
  extern std::string colliderType;
  extern std::map<std::string,GraphicalOutput> graphicalOutput;
  extern std::string silencerString;
}

namespace LEPParameters
{
  extern std::vector<double> ecm();
  extern std::vector<double> nominalLuminosity();
}

namespace TevatronParameters
{
}

#endif
