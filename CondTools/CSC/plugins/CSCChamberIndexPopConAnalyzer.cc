#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "CSCChamberIndexHandler.h"
#include "FWCore/Framework/interface/MakerMacros.h"

typedef popcon::PopConAnalyzer<popcon::CSCChamberIndexImpl> CSCChamberIndexPopConAnalyzer;

DEFINE_FWK_MODULE(CSCChamberIndexPopConAnalyzer);
