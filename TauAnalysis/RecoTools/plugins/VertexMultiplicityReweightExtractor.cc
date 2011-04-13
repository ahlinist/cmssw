#include "TauAnalysis/RecoTools/plugins/VertexMultiplicityReweightExtractor.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

VertexMultiplicityReweightExtractor::VertexMultiplicityReweightExtractor(const edm::ParameterSet& cfg)
  : src_(cfg.getParameter<edm::InputTag>("src")),
    inputFile_(0),
    lut_(0)
{
  edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
  std::string lutName = cfg.getParameter<std::string>("lutName");
  if ( inputFileName.isLocal()) {       
    inputFile_ = new TFile(inputFileName.fullPath().data());
    lut_ = (TH1*)inputFile_->Get(lutName.data());
  } else {
    throw cms::Exception("VertexMultiplicityReweightProducer") 
      << " Failed to find file = " << inputFileName << " !!";
  }
}

VertexMultiplicityReweightExtractor::~VertexMultiplicityReweightExtractor() 
{
  delete inputFile_;
}

int getBin(TAxis* axis, double x)
{
  int bin = axis->FindFixBin(x);
  int nBins = axis->GetNbins();
  if ( bin < 1     ) bin = 1;
  if ( bin > nBins ) bin = nBins;
  return bin;
}

double VertexMultiplicityReweightExtractor::operator()(const edm::Event& evt) const
{
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(src_, vertices);

  int bin = getBin(lut_->GetXaxis(), vertices->size());
  double weight = lut_->GetBinContent(bin);

  return weight;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, VertexMultiplicityReweightExtractor, "VertexMultiplicityReweightExtractor");

