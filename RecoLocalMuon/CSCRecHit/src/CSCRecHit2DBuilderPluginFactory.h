#ifndef CSCRecHit2D_CSCRecHit2DBuilderPluginFactory_h
#define CSCRecHit2D_CSCRecHit2DBuilderPluginFactory_h

/** \class CSCRecHit2DBuilderPluginFactory
 *  Plugin factory for concrete CSCRecHit2DBuilder algorithms
 *
 *  \author Tim Cox (based on SEAL example)
 */

#include <PluginManager/PluginFactory.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DAlgo.h>

class edm::ParameterSet;

class CSCRecHit2DBuilderPluginFactory : public seal::PluginFactory<CSCRecHit2DAlgo *(const edm::ParameterSet&)>{
 public:
  CSCRecHit2DBuilderPluginFactory();
    static CSCRecHit2DBuilderPluginFactory* get (void);

 private:
   static CSCRecHit2DBuilderPluginFactory s_instance;
};
#endif

