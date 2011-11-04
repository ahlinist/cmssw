#include "TauAnalysis/RecoTools/plugins/RhoNeutralCorrAnalyzer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

RhoNeutralCorrAnalyzer::RhoNeutralCorrAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label"))
{
  srcVertices_ = cfg.getParameter<edm::InputTag>("srcVertices");

  srcRhoNeutral_ = cfg.getParameter<edm::InputTag>("srcRhoNeutral");
  srcRhoChargedHadronsNoPileUp_ = cfg.getParameter<edm::InputTag>("srcRhoChargedHadronsNoPileUp");
  srcRhoChargedHadronsPileUp_ = cfg.getParameter<edm::InputTag>("srcRhoChargedHadronsPileUp");
  srcRhoChargedHadrons_ = cfg.getParameter<edm::InputTag>("srcRhoChargedHadrons");
  srcRhoNeutralHadrons_ = cfg.getParameter<edm::InputTag>("srcRhoNeutralHadrons");
  srcRhoPhotons_ = cfg.getParameter<edm::InputTag>("srcRhoPhotons");

  srcGenPileUp_ = ( cfg.exists("srcGenPileUp") ) ?
    cfg.getParameter<edm::InputTag>("srcGenPileUp") : edm::InputTag(); 

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights"); 
}

RhoNeutralCorrAnalyzer::~RhoNeutralCorrAnalyzer()
{
// nothing to be done yet...
}

void RhoNeutralCorrAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("RhoNeutralCorrAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  dqmStore.setCurrentFolder(moduleLabel_);
  
  meNumVertices_ = 
    dqmStore.book1D("numVertices", 
		    "numVertices", 35, 0., 34.5);

  meRhoNeutral_ = 
    dqmStore.book1D("rhoNeutral", 
		    "rhoNeutral", 240, 0., 12.);
  meRhoChargedHadronsNoPileUp_ = 
    dqmStore.book1D("RhoChargedHadronsNoPileUp", 
		    "RhoChargedHadronsNoPileUp", 240, 0., 12.);
  meRhoChargedHadronsPileUp_ = 
    dqmStore.book1D("RhoChargedHadronsPileUp", 
		    "RhoChargedHadronsPileUp", 240, 0., 12.);
  meRhoChargedHadrons_ = 
    dqmStore.book1D("RhoChargedHadrons", 
		    "RhoChargedHadrons", 240, 0., 12.);
  meRhoNeutralHadrons_ = 
    dqmStore.book1D("RhoNeutralHadrons", 
		    "RhoNeutralHadrons", 240, 0., 12.);
  meRhoPhotons_ = 
    dqmStore.book1D("RhoPhotons", 
		    "RhoPhotons", 240, 0., 12.);
  
  meRhoNeutralDivRhoChargedHadronsNoPileUp_ = 
    dqmStore.book1D("RhoNeutralDivRhoChargedHadronsNoPileUp", 
		    "RhoNeutralDivRhoChargedHadronsNoPileUpV", 200, 0., 20.);
  meRhoNeutralDivRhoChargedHadronsPileUp_ = 
    dqmStore.book1D("RhoNeutralDivRhoChargedHadronsPileUp", 
		    "RhoNeutralDivRhoChargedHadronsPileUp", 200, 0., 20.);
  meRhoNeutralDivRhoChargedHadrons_ = 
    dqmStore.book1D("RhoNeutralDivRhoChargedHadrons", 
		    "RhoNeutralDivRhoChargedHadrons", 200, 0., 20.);
  
  if ( srcGenPileUp_.label() != "" ) {
    meNumVerticesVsGenPileUp_ = 
      dqmStore.book2D("numVerticesVsGenPileUp", 
		      "numVerticesVsGenPileUp", 37, -0.5, 36.5, 35, 0., 34.5);
  
    meRhoNeutralVsGenPileUp_ = 
      dqmStore.book2D("rhoNeutralVsGenPileUp", 
		      "rhoNeutralVsGenPileUp", 37, -0.5, 36.5, 240, 0., 12.);
    meRhoChargedHadronsNoPileUpVsGenPileUp_ = 
      dqmStore.book2D("RhoChargedHadronsNoPileUpVsGenPileUp", 
		      "RhoChargedHadronsNoPileUpVsGenPileUp", 37, -0.5, 36.5, 240, 0., 12.);
    meRhoChargedHadronsPileUpVsGenPileUp_ = 
      dqmStore.book2D("RhoChargedHadronsPileUpVsGenPileUp", 
		      "RhoChargedHadronsPileUpVsGenPileUp", 37, -0.5, 36.5, 240, 0., 12.);
    meRhoChargedHadronsVsGenPileUp_ = 
      dqmStore.book2D("RhoChargedHadronsVsGenPileUp", 
		      "RhoChargedHadronsVsGenPileUp", 37, -0.5, 36.5, 240, 0., 12.);

    meRhoNeutralDivRhoChargedHadronsNoPileUpVsGenPileUp_ = 
      dqmStore.book2D("RhoNeutralDivRhoChargedHadronsNoPileUpVsGenPileUp", 
		      "RhoNeutralDivRhoChargedHadronsNoPileUpVsGenPileUp", 37, -0.5, 36.5, 200, 0., 20.);
    meRhoNeutralDivRhoChargedHadronsPileUpVsGenPileUp_ = 
      dqmStore.book2D("RhoNeutralDivRhoChargedHadronsPileUpVsGenPileUp", 
		      "RhoNeutralDivRhoChargedHadronsPileUpVsGenPileUp", 37, -0.5, 36.5, 200, 0., 20.);
    meRhoNeutralDivRhoChargedHadronsVsGenPileUp_ = 
      dqmStore.book2D("RhoNeutralDivRhoChargedHadronsVsGenPileUp", 
		      "RhoNeutralDivRhoChargedHadronsVsGenPileUp", 37, -0.5, 36.5, 200, 0., 20.);
  }
}

void RhoNeutralCorrAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<double> rhoNeutral;
  evt.getByLabel(srcRhoNeutral_, rhoNeutral);
  edm::Handle<double> rhoChargedHadronsNoPileUp;
  evt.getByLabel(srcRhoChargedHadronsNoPileUp_, rhoChargedHadronsNoPileUp);
  edm::Handle<double> rhoChargedHadronsPileUp;
  evt.getByLabel(srcRhoChargedHadronsPileUp_, rhoChargedHadronsPileUp);
  edm::Handle<double> rhoChargedHadrons;
  evt.getByLabel(srcRhoChargedHadrons_, rhoChargedHadrons);
  edm::Handle<double> rhoNeutralHadrons;
  evt.getByLabel(srcRhoNeutralHadrons_, rhoNeutralHadrons);
  edm::Handle<double> rhoPhotons;
  evt.getByLabel(srcRhoPhotons_, rhoPhotons);

//--- compute event weight
//   (reweighting for in-time and out-of-time pile-up, Data/MC correction factors,...)
  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  edm::Handle<reco::VertexCollection> vertices;
  evt.getByLabel(srcVertices_, vertices);
  size_t numRecVertices = vertices->size();

  double rhoNeutralDivRhoChargedHadronsNoPileUp = ( (*rhoChargedHadronsNoPileUp) > 0. ) ?
    ((*rhoNeutral)/(*rhoChargedHadronsNoPileUp)) : 19.99;
  double rhoNeutralDivRhoChargedHadronsPileUp = ( (*rhoChargedHadronsPileUp) > 0. ) ?
    ((*rhoNeutral)/(*rhoChargedHadronsPileUp)) : 19.99;
  double rhoNeutralDivRhoChargedHadrons = ( (*rhoChargedHadrons) > 0. ) ?
    ((*rhoNeutral)/(*rhoChargedHadrons)) : 19.99;
  
  meNumVertices_->Fill(numRecVertices, evtWeight);
  
  meRhoNeutral_->Fill(*rhoNeutral, evtWeight);
  meRhoChargedHadronsNoPileUp_->Fill(*rhoChargedHadronsNoPileUp, evtWeight);
  meRhoChargedHadronsPileUp_->Fill(*rhoChargedHadronsPileUp, evtWeight);
  meRhoChargedHadrons_->Fill(*rhoChargedHadrons, evtWeight);
  meRhoNeutralHadrons_->Fill(*rhoNeutralHadrons, evtWeight);
  meRhoPhotons_->Fill(*rhoPhotons, evtWeight);
  
  meRhoNeutralDivRhoChargedHadronsNoPileUp_->Fill(rhoNeutralDivRhoChargedHadronsNoPileUp, evtWeight);
  meRhoNeutralDivRhoChargedHadronsPileUp_->Fill(rhoNeutralDivRhoChargedHadronsPileUp, evtWeight);
  meRhoNeutralDivRhoChargedHadrons_->Fill(rhoNeutralDivRhoChargedHadrons, evtWeight);

  if ( srcGenPileUp_.label() != "" ) {
    double numGenPileUp = -1.;
    typedef std::vector<PileupSummaryInfo> PileupSummaryInfoCollection;
    edm::Handle<PileupSummaryInfoCollection> genPileUpInfos;
    evt.getByLabel(srcGenPileUp_, genPileUpInfos);
    for ( PileupSummaryInfoCollection::const_iterator genPileUpInfo = genPileUpInfos->begin();
	  genPileUpInfo != genPileUpInfos->end(); ++genPileUpInfo ) {
      // CV: in-time PU is stored in getBunchCrossing = 0, 
      //    cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupInformation
      if ( genPileUpInfo->getBunchCrossing() == 0 ) {
	numGenPileUp = genPileUpInfo->getPU_NumInteractions();
	if ( numGenPileUp > 35 ) numGenPileUp = 36;
      }
    }
    if ( numGenPileUp == -1. ) 
      throw cms::Exception("RhoNeutralCorrAnalyzer") 
	<< " Failed to find PileupSummaryInfo object for in-time Pile-up !!\n";

    meNumVerticesVsGenPileUp_->Fill(numGenPileUp, numRecVertices, evtWeight);

    meRhoNeutralVsGenPileUp_->Fill(numGenPileUp, *rhoNeutral, evtWeight);
    meRhoChargedHadronsNoPileUpVsGenPileUp_->Fill(numGenPileUp, *rhoChargedHadronsNoPileUp, evtWeight);
    meRhoChargedHadronsPileUpVsGenPileUp_->Fill(numGenPileUp, *rhoChargedHadronsPileUp, evtWeight);
    meRhoChargedHadronsVsGenPileUp_->Fill(numGenPileUp, *rhoChargedHadrons, evtWeight);
  
    meRhoNeutralDivRhoChargedHadronsNoPileUpVsGenPileUp_->Fill(numGenPileUp, rhoNeutralDivRhoChargedHadronsNoPileUp, evtWeight);
    meRhoNeutralDivRhoChargedHadronsPileUpVsGenPileUp_->Fill(numGenPileUp, rhoNeutralDivRhoChargedHadronsPileUp, evtWeight);
    meRhoNeutralDivRhoChargedHadronsVsGenPileUp_->Fill(numGenPileUp, rhoNeutralDivRhoChargedHadrons, evtWeight);
  }
}

void RhoNeutralCorrAnalyzer::endJob()
{
// nothing to be done yet...
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(RhoNeutralCorrAnalyzer);





