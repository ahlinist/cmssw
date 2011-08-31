#include "TauAnalysis/RecoTools/plugins/PUreweightHistogramProducer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

PUreweightHistogramProducer::PUreweightHistogramProducer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  srcVertices_ = cfg.getParameter<edm::InputTag>("srcVertices");
  srcPFNeutralRho_ = cfg.getParameter<edm::InputTag>("srcPFNeutralRho");
  
  if ( cfg.exists("srcWeight") ) srcWeight_ = cfg.getParameter<edm::InputTag>("srcWeight");
}

void PUreweightHistogramProducer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) 
    throw cms::Exception("PUreweightHistogramProducer::beginJob")
      << "Failed to access dqmStore !!\n";

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  dqmStore.setCurrentFolder(moduleLabel_);

  meVtxMultiplicity_ = 
    dqmStore.book1D("VtxMultiplicity", "VtxMultiplicity", 20, -0.5, +19.5);
  meVtxMultiplicity_->getTH1()->Sumw2();
  mePFNeutralRho_ = 
    dqmStore.book1D("PFNeutralRho", "PFNeutralRho", 100, 0., 20.);
  mePFNeutralRho_->getTH1()->Sumw2();

  mePFNeutralRhoVsVtxMultiplicity_ = 
    dqmStore.book2D("PFNeutralRhoVsVtxMultiplicity", "PFNeutralRhoVsVtxMultiplicity", 20, -0.5, +19.5, 100, 0., 20.);
  mePFNeutralRhoVsVtxMultiplicity_->getTH1()->Sumw2();
}

void PUreweightHistogramProducer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices);
  size_t vtxMultiplicity = vertices->size();
  
  edm::Handle<double> pfNeutralRho_handle;
  evt.getByLabel(srcPFNeutralRho_, pfNeutralRho_handle);
  double pfNeutralRho = (*pfNeutralRho_handle);

  double evtWeight = 1.0;
  if ( srcWeight_.label() != "" ) {
    edm::Handle<double> evtWeight_handle;
    evt.getByLabel(srcWeight_, evtWeight_handle);
    evtWeight = (*evtWeight_handle);
  }

  meVtxMultiplicity_->Fill(vtxMultiplicity, evtWeight);
  mePFNeutralRho_->Fill(pfNeutralRho, evtWeight);

  mePFNeutralRhoVsVtxMultiplicity_->Fill(vtxMultiplicity, pfNeutralRho, evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PUreweightHistogramProducer);
 
