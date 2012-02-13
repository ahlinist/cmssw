#include "TauAnalysis/RecoTools/plugins/PATObjectLUTrndSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorFromTH1.h"
#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorFromKNN.h"

typedef ObjectSelector<PATObjectLUTrndSelector<pat::Electron, PATElectronLUTvalueExtractorFromTH1> > PATElectronLUTrndSelectorFromTH1;
typedef ObjectSelector<PATObjectLUTrndSelector<pat::Electron, PATElectronLUTvalueExtractorFromKNN> > PATElectronLUTrndSelectorFromKNN;
typedef ObjectSelector<PATObjectLUTrndSelector<pat::Muon, PATMuonLUTvalueExtractorFromTH1> > PATMuonLUTrndSelectorFromTH1;
typedef ObjectSelector<PATObjectLUTrndSelector<pat::Muon, PATMuonLUTvalueExtractorFromKNN> > PATMuonLUTrndSelectorFromKNN;
typedef ObjectSelector<PATObjectLUTrndSelector<pat::Tau, PATTauLUTvalueExtractorFromTH1> > PATTauLUTrndSelectorFromTH1;
typedef ObjectSelector<PATObjectLUTrndSelector<pat::Tau, PATTauLUTvalueExtractorFromKNN> > PATTauLUTrndSelectorFromKNN;

typedef PATObjectLUTvalueExtractorFromTH1<reco::CompositeCandidate> CompositeCandidateLUTvalueExtractorFromTH1;
typedef PATObjectLUTvalueExtractorFromKNN<reco::CompositeCandidate> CompositeCandidateLUTvalueExtractorFromKNN;
typedef ObjectSelector<PATObjectLUTrndSelector<reco::CompositeCandidate, CompositeCandidateLUTvalueExtractorFromTH1> > CompositeCandidateLUTrndSelectorFromTH1;
typedef ObjectSelector<PATObjectLUTrndSelector<reco::CompositeCandidate, CompositeCandidateLUTvalueExtractorFromKNN> > CompositeCandidateLUTrndSelectorFromKNN;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronLUTrndSelectorFromTH1);
DEFINE_FWK_MODULE(PATElectronLUTrndSelectorFromKNN);
DEFINE_FWK_MODULE(PATMuonLUTrndSelectorFromTH1);
DEFINE_FWK_MODULE(PATMuonLUTrndSelectorFromKNN);
DEFINE_FWK_MODULE(PATTauLUTrndSelectorFromTH1);
DEFINE_FWK_MODULE(PATTauLUTrndSelectorFromKNN);

DEFINE_FWK_MODULE(CompositeCandidateLUTrndSelectorFromTH1);
DEFINE_FWK_MODULE(CompositeCandidateLUTrndSelectorFromKNN);




