#include "TauAnalysis/Core/plugins/VertexHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

VertexHistManager::VertexHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<VertexHistManager::VertexHistManager>:" << std::endl;

  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;
}

VertexHistManager::~VertexHistManager()
{
//--- nothing to be done yet...
}

void VertexHistManager::bookHistograms()
{
  //std::cout << "<VertexHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hVertexX_ = dqmStore.book1D("VertexX", "VertexX", 200, -1., +1.);
  hVertexSigmaX_ = dqmStore.book1D("VertexSigmaX", "VertexSigmaX", 200, -0.01, +0.01);
  hVertexY_ = dqmStore.book1D("VertexY", "VertexY", 200, -1., +1.);
  hVertexSigmaY_ = dqmStore.book1D("VertexSigmaY", "VertexSigmaY", 200, -0.01, +0.01);
  hVertexXvsY_ = dqmStore.book2D("VertexXvsY", "VertexXvsY", 200, -1., +1., 200, -1, +1.);
  hVertexZ_ = dqmStore.book1D("VertexZ", "VertexZ", 100, -50., 50.);
  hVertexSigmaZ_ = dqmStore.book1D("VertexSigmaZ", "VertexSigmaZ", 200, -0.10, 0.10);

  hVertexNumTracks_ = dqmStore.book1D("VertexNumTracks", "VertexNumTracks", 101, -0.5, 100.5);
  
  hVertexChi2Prob_ = dqmStore.book1D("VertexChi2Prob", "VertexChi2Prob", 102, -0.01, 1.01);
}

void VertexHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)

{  
  //std::cout << "<VertexHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  edm::Handle<std::vector<reco::Vertex> > recoVertices;
  evt.getByLabel(vertexSrc_, recoVertices);
  if ( recoVertices->size() >= 1 ) {
    const reco::Vertex& thePrimaryEventVertex = (*recoVertices->begin());

    hVertexX_->Fill(thePrimaryEventVertex.x());
    hVertexSigmaX_->Fill(thePrimaryEventVertex.xError());
    hVertexY_->Fill(thePrimaryEventVertex.y());
    hVertexSigmaY_->Fill(thePrimaryEventVertex.yError());
    hVertexXvsY_->Fill(thePrimaryEventVertex.x(), thePrimaryEventVertex.y());
    hVertexZ_->Fill(thePrimaryEventVertex.z());
    hVertexSigmaZ_->Fill(thePrimaryEventVertex.zError());

    hVertexNumTracks_->Fill(thePrimaryEventVertex.tracksSize());

    hVertexChi2Prob_->Fill(TMath::Prob(thePrimaryEventVertex.chi2(), TMath::Nint(thePrimaryEventVertex.ndof())));
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, VertexHistManager, "VertexHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, VertexHistManager, "VertexHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<VertexHistManager> VertexAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(VertexAnalyzer);
