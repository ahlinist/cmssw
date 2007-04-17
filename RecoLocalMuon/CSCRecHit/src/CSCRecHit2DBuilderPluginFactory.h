#ifndef CSCRecHit2D_CSCRecHit2DBuilderPluginFactory_h
#define CSCRecHit2D_CSCRecHit2DBuilderPluginFactory_h

/** \class CSCRecHit2DBuilderPluginFactory
 *  Plugin factory for concrete CSCRecHit2DBuilder algorithms
 *
 *  \author Tim Cox (based on SEAL example)
 */

#include <FWCore/PluginManager/interface/PluginFactory.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DAlgo.h>

class edm::ParameterSet;

typedef edmplugin::PluginFactory<CSCRecHit2DAlgo *(const edm::ParameterSet &)> CSCRecHit2DBuilderPluginFactory;
#endif

