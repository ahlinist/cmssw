#include "TauAnalysis/BgEstimationTools/plugins/SPlotAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

SPlotAnalyzer::SPlotAnalyzer(const edm::ParameterSet& cfg)
{
  std::cout << "<SPlotAnalyzer::SplotAnalyzer>:" << std::endl;
}

SPlotAnalyzer::~SPlotAnalyzer()
{
// nothing to be done yet...
}

void SPlotAnalyzer::beginJob()
{

}

void SPlotAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
// nothing to be done yet...
}

void SPlotAnalyzer::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SPlotAnalyzer);
