#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/MadGraphInterface/interface/MadGraphSource.h"
#include "GeneratorInterface/MadGraphInterface/interface/MadGraphProducer.h"
#include "GeneratorInterface/MadGraphInterface/interface/STLesHouchesMixerSource.h"
#include "GeneratorInterface/MadGraphInterface/interface/HepMGProductAnalyzer.h"
#include "GeneratorInterface/MadGraphInterface/interface/PdfAnalyzer.h"

using edm::MadGraphSource; 
using edm::MadGraphProducer; 
using edm::STLesHouchesMixerSource; 

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(MadGraphSource);
DEFINE_ANOTHER_FWK_INPUT_SOURCE(STLesHouchesMixerSource);
DEFINE_ANOTHER_FWK_MODULE(MadGraphProducer);
DEFINE_ANOTHER_FWK_MODULE(HepMGProductAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(PdfAnalyzer);

