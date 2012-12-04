#ifndef TauAnalysis_CandidateTools_interface_NSVfitStandaloneTestAnalyzer_h
#define TauAnalysis_CandidateTools_interface_NSVfitStandaloneTestAnalyzer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <TStopwatch.h>

#include <string>

class PFMEtSignInterface;

/**
   \class NSVfitEventAnalyzer NSVfitEventAnalyzer.h "TauAnalysis/CandidateTools/interface/NSVfitEventAnalyzer.h"
   \brief Basic edm and fwlite friendly analyzer class to do basic testing of NSVfit

   EDAnalyzer to test the standalone verison of SVfit against the plugin version. The EDAnalyzer reads exactly
   the same input from the EDM event that is used by the plugin version, prepares the input, instantiates and 
   executed the standalone version of the NSVfitAlgorithmByLikelihoodMaximization version of NSVfit. Note that 
   the selection of the lepton candidates, which are to be used by the fit is left to the user. When run in one 
   configuration with the plugin version the two versions can be cross checked against each other to check that
   they give identical results. For a first study have a look to 

   https://indico.cern.ch/getFile.py/access?contribId=23&sessionId=0&resId=0&materialId=slides&confId=155708

   Note that the EDAnalyzer is a very simplistic example only. It is capable of using any combination of lepton 
   types as defined by the input tags (accoording to the use of edm::Views)

   For an example for a genuine use of the standalone version of the algorithm have a look to the 
   nsvfitStandalone.cc executable as defined in the bin directory of this package. 
*/

class NSVfitStandaloneTestAnalyzer : public edm::EDAnalyzer {

 public:
  /// default constructor
  NSVfitStandaloneTestAnalyzer(const edm::ParameterSet& cfg);
  /// default destructor
  virtual ~NSVfitStandaloneTestAnalyzer();
  /// everything that needs to be done before the event loop
  void beginJob();
  /// everything that needs to be done after the event loop
  void endJob();
  /// everything that needs to be done during the event loop
  void analyze(const edm::Event& event, const edm::EventSetup& eventSetup);

 private:

  std::string moduleLabel_;

  /// input tag for MET
  edm::InputTag met_;
  /// input tag for electrons
  edm::InputTag leps1_;
  /// input tag for muons
  edm::InputTag leps2_;
  /// lepton type1
  std::string type1_;
  /// lewpton type2
  std::string type2_;

  /// MET significance interface
  PFMEtSignInterface* metSign_;

  std::string dqmDirectory_;
  bool fillHistograms_;
  MonitorElement* leg1Pt_;
  MonitorElement* leg2Pt_;
  MonitorElement* metPt_;
  MonitorElement* svFitMass_;
  MonitorElement* visMass_;

  TStopwatch* timer_;
  long numSVfitCalls_;

  int verbosity_;
};

#endif
