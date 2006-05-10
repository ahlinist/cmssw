#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "PhysicsTools/ttHAnalysis/interface/TTHInputService.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronCandidateProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronSelector.h"
#include "PhysicsTools/ttHAnalysis/interface/SignalElectronSelector.h"
#include "PhysicsTools/ttHAnalysis/interface/MissingEtProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/NeutrinoCandidateProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/JetCandidateProducer.h"
#include "PhysicsTools/ttHAnalysis/interface/BJetDiscriminator.h"
#include "PhysicsTools/ttHAnalysis/interface/QJetDiscriminator.h"
#include "PhysicsTools/ttHAnalysis/interface/SimpleAnalysis.h"

using tth::SimpleAnalysis;
using tth::QJetDiscriminator;
using tth::BJetDiscriminator;
using tth::JetCandidateProducer;
using tth::NeutrinoCandidateProducer;
using tth::MissingEtProducer;
using tth::SignalElectronSelector;
using tth::ElectronSelector;
using tth::ElectronCandidateProducer;
using tth::TTHInputService;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(TTHInputService)
DEFINE_ANOTHER_FWK_MODULE( ElectronCandidateProducer );
DEFINE_ANOTHER_FWK_MODULE( ElectronSelector );
DEFINE_ANOTHER_FWK_MODULE( SignalElectronSelector );
DEFINE_ANOTHER_FWK_MODULE( MissingEtProducer );
DEFINE_ANOTHER_FWK_MODULE( NeutrinoCandidateProducer );
DEFINE_ANOTHER_FWK_MODULE( JetCandidateProducer );
DEFINE_ANOTHER_FWK_MODULE( BJetDiscriminator );
DEFINE_ANOTHER_FWK_MODULE( QJetDiscriminator );
DEFINE_ANOTHER_FWK_MODULE( SimpleAnalysis );
