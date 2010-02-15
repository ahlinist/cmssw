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
#include "HeavyFlavorAnalysis/Examples/interface/HFLambdaB2JpsiLambda.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFBd2JpsiKstar.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFBu2JpsiKp.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFBs2JpsiPhi.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDimuons.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFMuonAndTrack.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFDiTracks.h"
#include "HeavyFlavorAnalysis/Examples/interface/HFB2muD0.hh"
#include "HeavyFlavorAnalysis/Examples/interface/HFB2D0Ds.hh"

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
DEFINE_ANOTHER_FWK_MODULE(HFLambdaB2JpsiLambda);
DEFINE_ANOTHER_FWK_MODULE(HFBd2JpsiKstar);
DEFINE_ANOTHER_FWK_MODULE(HFBu2JpsiKp);
DEFINE_ANOTHER_FWK_MODULE(HFBs2JpsiPhi);
DEFINE_ANOTHER_FWK_MODULE(HFDimuons);
DEFINE_ANOTHER_FWK_MODULE(HFMuonAndTrack);
DEFINE_ANOTHER_FWK_MODULE(HFDiTracks);
DEFINE_ANOTHER_FWK_MODULE(HFB2muD0);
DEFINE_ANOTHER_FWK_MODULE(HFB2D0Ds);

