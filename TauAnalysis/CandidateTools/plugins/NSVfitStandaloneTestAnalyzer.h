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

  /// make generator level plots
  bool doGenPlots_;
  /// gen. tau lepton pairs
  edm::InputTag srcGenTauPairs_;
  /// gen. visible decay products
  edm::InputTag srcGenLeg1_;
  edm::InputTag srcGenLeg2_;
  /// gen. MET
  edm::InputTag srcGenMEt_;

  /// make reconstructed level plots
  bool doRecPlots_;
  /// rec. visible decay products
  edm::InputTag srcRecLeg1_;
  edm::InputTag srcRecLeg2_;
  /// rec. MET
  edm::InputTag srcRecMEt_;
  /// rec. MET uncertainty
  /// (in case this InputTag is left empty, 
  ///  the MET uncertainty matrix will be taken from reco::MET->getSignificanceMatrix)
  edm::InputTag srcRecMEtCov_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  /// SVfit configuration parameters:
  /// type of tau decay likelihood to be used for visible decay products ("lep"/"had")
  std::string typeLeg1_;
  std::string typeLeg2_;
  /// flag indicating how to compute SVfit solutions
  ///  o "fit":  MINUIT fit (fast, but suboptimal resolution)
  ///  o "int":  VEGAS integration (good resolution, but takes 1-2 seconds per event)
  ///  o "int2": Markov Chain integration (best compromise between resolution and computing time)
  enum { kFit, kInt, kInt2 };
  int mode_;
  /// flag to request recomputation of MET uncertainty matrix
  bool redoMEtCov_;
  PFMEtSignInterface* metSign_;

  /// flag to enable/disable booking+filling of histograms
  bool fillHistograms_;
  std::string dqmDirectory_;

  MonitorElement* genDiTauPt_;
  MonitorElement* genDiTauEta_;
  MonitorElement* genDiTauMass_;
  MonitorElement* genLeg1Pt_;
  MonitorElement* genLeg1Eta_;
  MonitorElement* genLeg2Pt_;
  MonitorElement* genLeg2Eta_;
  MonitorElement* genMEtPt_;

  MonitorElement* recDiTauPt_;
  MonitorElement* recDiTauEta_;
  MonitorElement* svFitMass_;
  MonitorElement* svFitStatus_;
  MonitorElement* visMass_;
  MonitorElement* recLeg1Pt_;
  MonitorElement* recLeg1Eta_;
  MonitorElement* recLeg2Pt_;
  MonitorElement* recLeg2Eta_;
  MonitorElement* recMEtPt_;

  MonitorElement* deltaDiTauPt_;   // delta = reconstructed - generated
  MonitorElement* deltaDiTauEta_;
  MonitorElement* deltaDiTauPhi_;
  MonitorElement* deltaDiTauMass_;

  TStopwatch* timer_;
  long numSVfitCalls_;

  int verbosity_;
};

#endif
