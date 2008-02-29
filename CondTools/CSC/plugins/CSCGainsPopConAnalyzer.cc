#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CondTools/CSC/interface/CSCGainsHandler.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef popcon::PopConAnalyzer<popcon::CSCDBGainsImpl> CSCGainsPopConAnalyzer;

DEFINE_FWK_MODULE(CSCGainsPopConAnalyzer);
