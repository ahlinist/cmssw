#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFTree.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpGenerator.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFGenFilter.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTracks.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpSignal.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpTrigger.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpMuons.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpJets.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDumpStuff.h"

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(HFTree);
DEFINE_ANOTHER_FWK_MODULE(HFDumpGenerator);
DEFINE_ANOTHER_FWK_MODULE(HFGenFilter);
DEFINE_ANOTHER_FWK_MODULE(HFDumpTracks);
DEFINE_ANOTHER_FWK_MODULE(HFDumpSignal);
DEFINE_ANOTHER_FWK_MODULE(HFDumpTrigger);
DEFINE_ANOTHER_FWK_MODULE(HFDumpMuons);
DEFINE_ANOTHER_FWK_MODULE(HFDumpJets);
DEFINE_ANOTHER_FWK_MODULE(HFDumpStuff);

