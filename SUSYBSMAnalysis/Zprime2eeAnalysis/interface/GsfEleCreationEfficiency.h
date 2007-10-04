#ifndef SUSYBSMAnalysis_Zprime2eeAnalysis_h
#define SUSYBSMAnalysis_Zprime2eeAnalysis_h

#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/EfficiencyBase.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"

#include <string>

// root includes
#include "TH1F.h"

class GsfEleCreationEfficiency : public EfficiencyBase
{
 protected:

  edm::InputTag gsfTrackEBTag_;
  edm::InputTag gsfTrackEETag_;
  edm::InputTag gsfEleTag_;

  // match sc to electron or if not then
  // match (sc with track) to electron
  bool scToEle_;

  double dR_;
  double normalizedChi2_;
  int validHits_;

  bool MatchObjects(const reco::GsfTrack&, EgEff::EmObjectRef);

 public:

  GsfEleCreationEfficiency() : EfficiencyBase("GsfEleCreationEfficiency"){};
  virtual void Add(const edm::Event&, const edm::EventSetup&);
  virtual void Initialise(const edm::ParameterSet&);

};

#endif
