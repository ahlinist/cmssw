#ifndef SUSYBSMAnalysis_Zprime2eeAnalysisr_SCCreationEfficiency_h
#define SUSYBSMAnalysis_Zprime2eeAnalysisr_SCCreationEfficiency_h

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/EfficiencyBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include <string>

// root includes
#include "TH1F.h"

class SCCreationEfficiency : public EfficiencyBase
{
 protected:

  edm::InputTag scEBTag_;
  edm::InputTag scEETag_;

  double scEtCut_;
  double dR_;
  double dEta_;

  bool MatchObjects(const reco::SuperCluster&, EgEff::EmObjectRef);
 
  // diagnostic histograms
  TH1F *h1_dEta;
  TH1F *h1_dR;


 public:

  SCCreationEfficiency() : EfficiencyBase("SCCreationEfficiency"){};
  virtual void Add(const edm::Event&, const edm::EventSetup&);
  virtual void Initialise(const edm::ParameterSet&);

};

#endif
