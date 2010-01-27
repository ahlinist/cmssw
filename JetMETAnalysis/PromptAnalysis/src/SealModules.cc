#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

DEFINE_SEAL_MODULE();

#include "JetMETAnalysis/PromptAnalysis/interface/PromptAnaTree.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Event.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_MET.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_TcMET.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_PfMET.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Jet.h"
//#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_JPT.h"
//#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_PFJet.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_BeamHalo.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_CaloTowers.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Trigger.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_CleanUp.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Vertex.h"
#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_Track.h"

DEFINE_ANOTHER_FWK_MODULE(PromptAnaTree);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_Event);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_MET);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_TcMET);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_PfMET);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_Jet);
//DEFINE_ANOTHER_FWK_MODULE(PromptAna_JPT);
//DEFINE_ANOTHER_FWK_MODULE(PromptAna_PFJet);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_BeamHalo);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_CaloTowers);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_Trigger);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_CleanUp);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_Vertex);
DEFINE_ANOTHER_FWK_MODULE(PromptAna_Track);

