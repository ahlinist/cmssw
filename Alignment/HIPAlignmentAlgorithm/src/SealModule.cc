// Plugin definition for the algorithm

#include "Alignment/HIPAlignmentAlgorithm/interface/HIPAlignmentAlgorithm.h"
#include "Alignment/HIPAlignmentAlgorithm/interface/MuonHIPAlignmentAlgorithm.h"
#include "Alignment/CommonAlignmentAlgorithm/interface/AlignmentAlgorithmPluginFactory.h"

DEFINE_EDM_PLUGIN( AlignmentAlgorithmPluginFactory, HIPAlignmentAlgorithm, "HIPAlignmentAlgorithm" );
DEFINE_EDM_PLUGIN(AlignmentAlgorithmPluginFactory, MuonHIPAlignmentAlgorithm, "MuonHIPAlignmentAlgorithm");

