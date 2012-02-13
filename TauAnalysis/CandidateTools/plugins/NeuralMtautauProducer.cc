#include "TauAnalysis/CandidateTools/plugins/NeuralMtautauProducer.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef NeuralMtautauProducer<pat::Electron, pat::Tau> NeuralMtautauProducerForElecTau;
typedef NeuralMtautauProducer<pat::Muon, pat::Tau> NeuralMtautauProducerForMuTau;
typedef NeuralMtautauProducer<pat::Tau, pat::Tau> NeuralMtautauProducerForDiTau;
typedef NeuralMtautauProducer<pat::Electron, pat::Muon> NeuralMtautauProducerForElecMu;
typedef NeuralMtautauProducer<pat::Electron, pat::Electron> NeuralMtautauProducerForDiElec;
typedef NeuralMtautauProducer<pat::Muon, pat::Muon> NeuralMtautauProducerForDiMu;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NeuralMtautauProducerForElecTau);
DEFINE_FWK_MODULE(NeuralMtautauProducerForMuTau);
DEFINE_FWK_MODULE(NeuralMtautauProducerForDiTau);
DEFINE_FWK_MODULE(NeuralMtautauProducerForElecMu);
DEFINE_FWK_MODULE(NeuralMtautauProducerForDiElec);
DEFINE_FWK_MODULE(NeuralMtautauProducerForDiMu);
