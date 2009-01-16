#ifndef ETauMassAnalyzer_h  
#define ETauMassAnalyzer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositVetos.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TH1.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <vector>
#include <sstream>


class ETauMassAnalyzer : public edm::EDAnalyzer {

 public:
  
  explicit ETauMassAnalyzer(const edm::ParameterSet&);
  ~ETauMassAnalyzer();
  
 private:
  
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;

  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag tau_;
  edm::InputTag elec_;
  edm::InputTag met_;
  VInputTag refObjects_;
  bool doMatching_;
  double eventWeight_;  

  unsigned int m_count_sel;
  
  TH1F *hETauMassVis, *hETauMassRaw, *hETauMassT1,
    *hETauMassMu, *hETauMassT1Mu;
  TH1F *hSelHisto;
  
};  

#endif  


