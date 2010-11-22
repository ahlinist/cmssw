#include "TauAnalysis/RecoTools/plugins/VertexMultiplicityReweightProducer.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

VertexMultiplicityReweightProducer::VertexMultiplicityReweightProducer(const edm::ParameterSet& cfg)
  : inputFile_(0),
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
  
  src_ = cfg.getParameter<edm::InputTag>("src");
  
  produces<double>("");
}

VertexMultiplicityReweightProducer::~VertexMultiplicityReweightProducer() 
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

void VertexMultiplicityReweightProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<VertexMultiplicityReweightProducer::produce>:" << std::endl;
  
  edm::Handle<reco::VertexCollection> vertexCollection;
  pf::fetchCollection(vertexCollection, src_, evt);
  
  //std::cout << " numVertices = " << vertexCollection->size() << std::endl;

  int bin = getBin(lut_->GetXaxis(), vertexCollection->size());
  double weight = lut_->GetBinContent(bin);
  //std::cout << "--> weight = " << weight << std::endl;
  
  std::auto_ptr<double> weightPtr(new double(weight));
  evt.put(weightPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(VertexMultiplicityReweightProducer);


