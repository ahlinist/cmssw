#include "TauAnalysis/RecoTools/plugins/VertexMultiplicityVsRhoPFNeutralReweightProducer.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"

VertexMultiplicityVsRhoPFNeutralReweightProducer::VertexMultiplicityVsRhoPFNeutralReweightProducer(const edm::ParameterSet& cfg)
  : inputFile_(0),
    lut_(0)
{
  srcVertices_ = cfg.getParameter<edm::InputTag>("srcVertices");
  srcRhoPFNeutral_ = cfg.getParameter<edm::InputTag>("srcRhoPFNeutral");

  edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
  std::string lutName = cfg.getParameter<std::string>("lutName");
  if ( !inputFileName.isLocal()) throw cms::Exception("VertexMultiplicityVsRhoPFNeutralReweightProducer") 
    << " Failed to find File = " << inputFileName << " !!\n";

  inputFile_ = new TFile(inputFileName.fullPath().data());
  lut_ = dynamic_cast<TH1*>(inputFile_->Get(lutName.data()));
  if ( !lut_ ) throw cms::Exception("VertexMultiplicityVsRhoPFNeutralReweightExtractor") 
    << " Failed to load LUT = " << lutName.data() << " from file = " << inputFileName.fullPath().data() << " !!\n";

  minPUreweight_ = cfg.getParameter<double>("minPUreweight");
  maxPUreweight_ = cfg.getParameter<double>("maxPUreweight");

  produces<double>("");
}

VertexMultiplicityVsRhoPFNeutralReweightProducer::~VertexMultiplicityVsRhoPFNeutralReweightProducer() 
{
  delete inputFile_;
}

void VertexMultiplicityVsRhoPFNeutralReweightProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices);
  size_t vtxMultiplicity = vertices->size();
  
  edm::Handle<double> pfNeutralRho_handle;
  evt.getByLabel(srcRhoPFNeutral_, pfNeutralRho_handle);
  double pfNeutralRho = (*pfNeutralRho_handle);

  double weight = 1.0;
  if ( vtxMultiplicity >= lut_->GetXaxis()->GetXmin() && vtxMultiplicity <= lut_->GetXaxis()->GetXmax() &&
       pfNeutralRho    >  lut_->GetYaxis()->GetXmin() && vtxMultiplicity <  lut_->GetYaxis()->GetXmax() ) {
    int bin = lut_->FindBin(vtxMultiplicity, pfNeutralRho);
    weight = lut_->GetBinContent(bin);
  }
  if ( weight < minPUreweight_ ) weight = minPUreweight_;
  if ( weight > maxPUreweight_ ) weight = maxPUreweight_;

  std::auto_ptr<double> weightPtr(new double(weight));
  evt.put(weightPtr);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(VertexMultiplicityVsRhoPFNeutralReweightProducer);


