#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/MadGraphInterface/interface/MadGraphSource.h"
#include "GeneratorInterface/MadGraphInterface/interface/HepMGProductAnalyzer.h"

using edm::MadGraphSource; 

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(MadGraphSource);
DEFINE_ANOTHER_FWK_MODULE(HepMGProductAnalyzer);
