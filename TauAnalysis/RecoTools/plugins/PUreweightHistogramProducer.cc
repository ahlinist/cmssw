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
  srcRho_ = cfg.getParameter<edm::InputTag>("srcRho");

  rhoMax_ = cfg.getParameter<double>("rhoMax");

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
    dqmStore.book1D("VtxMultiplicity", "VtxMultiplicity", 35, -0.5, +34.5);
  meVtxMultiplicity_->getTH1()->Sumw2();
  meRho_ = 
    dqmStore.book1D("Rho", "PFNeutralRho", 100, -1.e-3, rhoMax_);
  meRho_->getTH1()->Sumw2();

  meRhoVsVtxMultiplicity_ = 
    dqmStore.book2D("RhoVsVtxMultiplicity", "RhoVsVtxMultiplicity", 35, -0.5, +34.5, 100, -1.e-3, rhoMax_);
  meRhoVsVtxMultiplicity_->getTH1()->Sumw2();
}

void PUreweightHistogramProducer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices);
  size_t vtxMultiplicity = vertices->size();
  
  edm::Handle<double> rho_handle;
  evt.getByLabel(srcRho_, rho_handle);
  double rho = (*rho_handle);

  double evtWeight = 1.0;
  if ( srcWeight_.label() != "" ) {
    edm::Handle<double> evtWeight_handle;
    evt.getByLabel(srcWeight_, evtWeight_handle);
    evtWeight = (*evtWeight_handle);
  }
  //std::cout << "evtWeight = " << evtWeight << std::endl;

  meVtxMultiplicity_->Fill(vtxMultiplicity, evtWeight);
  meRho_->Fill(rho, evtWeight);

  meRhoVsVtxMultiplicity_->Fill(vtxMultiplicity, rho, evtWeight);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PUreweightHistogramProducer);
 
