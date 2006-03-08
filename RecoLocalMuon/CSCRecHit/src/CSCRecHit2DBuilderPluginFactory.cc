// This is CSCRecHit2DBuilderPluginFactory.cc

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DBuilderPluginFactory.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

CSCRecHit2DBuilderPluginFactory CSCRecHit2DBuilderPluginFactory::s_instance;

CSCRecHit2DBuilderPluginFactory::CSCRecHit2DBuilderPluginFactory () : 
  seal::PluginFactory<CSCRecHit2DAlgo *(const edm::ParameterSet&)>("CSCRecHit2DBuilderPluginFactory"){}

CSCRecHit2DBuilderPluginFactory* CSCRecHit2DBuilderPluginFactory::get (){
  return &s_instance; 
}
