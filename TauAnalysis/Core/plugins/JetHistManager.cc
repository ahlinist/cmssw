#include "TauAnalysis/Core/plugins/JetHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <assert.h>

bool matchesGenJet(const pat::Jet& patJet)
{
  bool isGenMatched = false;
// not implemented yet...
  return isGenMatched;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

JetHistManager::JetHistManager(const edm::ParameterSet& cfg)
  : dqmError_(0)
{
  //std::cout << "<JetHistManager::JetHistManager>:" << std::endl;

  jetSrc_ = cfg.getParameter<edm::InputTag>("jetSource");
  //std::cout << " jetSrc = " << jetSrc_ << std::endl;

  dqmDirectory_store_ = cfg.getParameter<std::string>("dqmDirectory_store");
  //std::cout << " dqmDirectory_store = " << dqmDirectory_store_ << std::endl;

  requireGenJetMatch_ = cfg.getParameter<bool>("requireGenJetMatch");
  //std::cout << " requireGenJetMatch = " << requireGenJetMatch_ << std::endl;

  edm::ParameterSet cfgCentralJetsToBeVetoed = cfg.getParameter<edm::ParameterSet>("centralJetsToBeVetoed");  
  centralJetsToBeVetoedEtMin_ = cfgCentralJetsToBeVetoed.getParameter<vdouble>("etMin");
  centralJetsToBeVetoedEtaMax_ = cfgCentralJetsToBeVetoed.getParameter<vdouble>("etaMax");
  centralJetsToBeVetoedAlphaMin_ = cfgCentralJetsToBeVetoed.getParameter<vdouble>("alphaMin");

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgBTaggingDiscriminators = cfg.getParameter<vParameterSet>("bTaggingDiscriminators");
  for ( vParameterSet::const_iterator cfgBTaggingDiscriminator = cfgBTaggingDiscriminators.begin();
	cfgBTaggingDiscriminator != cfgBTaggingDiscriminators.end(); ++cfgBTaggingDiscriminator ) {
    std::string bTaggingDiscrName = cfgBTaggingDiscriminator->getParameter<std::string>("name");
    double bTaggingDiscrThreshold = cfgBTaggingDiscriminator->getParameter<double>("threshold");

    bTaggingDiscriminators_.push_back(bTaggingDiscrName);
    bTaggingDiscriminatorThresholds_.push_back(bTaggingDiscrThreshold);
  }
}

JetHistManager::~JetHistManager()
{
//--- nothing to be done yet...
}

void JetHistManager::bookHistograms()
{
  //std::cout << "<JetHistManager::bookHistograms>:" << std::endl;

  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("bookHistograms") << " Failed to access dqmStore --> histograms will NOT be booked !!";
    dqmError_ = 1;
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  dqmStore.setCurrentFolder(dqmDirectory_store_);
  
  hNumJets_ = dqmStore.book1D("NumJets", "NumJets", 50, -0.5, 49.5);
  
  bookJetHistograms(dqmStore, hJetPt_, hJetEta_, hJetPhi_, "Jet");
  hJetPtVsEta_ = dqmStore.book2D("JetPtVsEta", "Jet #eta vs P_{T}", 24, -3., +3., 30, 0., 150.);
  
  hJetAlpha_ = dqmStore.book1D("JetAlpha", "Jet #alpha", 102, -0.01, +1.01);
  hJetNumTracks_ = dqmStore.book1D("JetNumTracks", "Jet Track Multiplicity", 50, -0.5, 49.5);
  hJetTrkPt_ = dqmStore.book1D("JetTrkPt", "Jet All Tracks P_{T}", 100, 0., 50.);
  hJetLeadTrkPt_ = dqmStore.book1D("JetLeadTrkPt", "Jet Lead Track P_{T}", 75, 0., 75.);
  
  for ( vdouble::const_iterator etMin = centralJetsToBeVetoedEtMin_.begin();
	etMin != centralJetsToBeVetoedEtMin_.end(); ++etMin ) {
    for ( vdouble::const_iterator etaMax = centralJetsToBeVetoedEtaMax_.begin();
	  etaMax != centralJetsToBeVetoedEtaMax_.end(); ++etaMax ) {
      for ( vdouble::const_iterator alphaMin = centralJetsToBeVetoedAlphaMin_.begin();
	    alphaMin != centralJetsToBeVetoedAlphaMin_.end(); ++alphaMin ) {
	std::ostringstream hName;
	hName.setf(std::ios::fixed);
	hName.precision(1);
	hName << "numJetsEtGt" << (*etMin) << "EtaLt" << (*etaMax) << "AlphaGt" << (*alphaMin);
	int errorFlag = 0;
	std::string hName_mod = replace_string(hName.str(), ".", "_", 0, 3, errorFlag);
	//std::cout << "hName_mod = " << hName_mod << std::endl;
	
	hNumCentralJetsToBeVetoed_.push_back(dqmStore.book1D(hName_mod, hName_mod, 50, -0.5, 49.5));
      }
    }
  }
  
  assert(bTaggingDiscriminators_.size()== bTaggingDiscriminatorThresholds_.size());
  for (unsigned int i=0;i<bTaggingDiscriminators_.size();i++) {
    std::ostringstream hName0;
    std::ostringstream hTitle0;
    hName0 << "BtagDisc_" << bTaggingDiscriminators_[i];
    hTitle0 << "Jet B-Tag Discriminator: " << bTaggingDiscriminators_[i];
  
    hBtagDisc_.push_back(dqmStore.book1D(hName0.str(),hTitle0.str(),120,-10.,50.));
    
    std::ostringstream hName1;
    std::ostringstream hTitle1;
    hName1 << "NumBtags_" << bTaggingDiscriminators_[i];
    hTitle1 << "Jet B-Tag Count: " << bTaggingDiscriminators_[i] << ">" << bTaggingDiscriminatorThresholds_[i];
  
    hNumBtags_.push_back(dqmStore.book1D(hName1.str(),hTitle1.str(),15,-0.5,14.5));
    
    std::ostringstream hName2;
    std::ostringstream hTitle2;
    hName2 << "PtBtags_" << bTaggingDiscriminators_[i];
    hTitle2 << "B-Tagged Jets P_{T}: " << bTaggingDiscriminators_[i] << ">" << bTaggingDiscriminatorThresholds_[i];
  
    hPtBtags_.push_back(dqmStore.book1D(hName2.str(),hTitle2.str(),75,0.,150.));
  }
  
}

double compAlpha(const pat::Jet& jet)
{
  double sumPt = 0.;
  for ( reco::TrackRefVector::const_iterator track = jet.associatedTracks().begin();
	track != jet.associatedTracks().end(); ++track ) {
    sumPt += (*track)->pt();
  }
  return ( jet.et() > 0 ) ? sumPt/jet.et() : -1.;
}

void JetHistManager::fillHistograms(const edm::Event& evt, const edm::EventSetup& es)
{  
  //std::cout << "<JetHistManager::fillHistograms>:" << std::endl; 

  if ( dqmError_ ) {
    edm::LogError ("fillHistograms") << " Failed to access dqmStore --> histograms will NOT be filled !!";
    return;
  }

  edm::Handle<std::vector<pat::Jet> > patJets;
  evt.getByLabel(jetSrc_, patJets);

  //std::cout << " patJets.size = " << patJets->size() << std::endl;

  hNumJets_->Fill(patJets->size());

  std::vector<unsigned int> nbtags;
  for (unsigned int i=0;i<bTaggingDiscriminators_.size();i++) {
    nbtags.push_back(0);
  }
  for ( std::vector<pat::Jet>::const_iterator patJet = patJets->begin(); 
	patJet != patJets->end(); ++patJet ) {
  
    //bool isGenJetMatched = matchesGenJet(*patJet);
    //std::cout << " Pt = " << patJet->pt() << ", eta = " << patJet->eta() << ", phi = " << patJet->phi() << std::endl;
    //std::cout << " isGenJetMatched = " << isGenJetMatched << std::endl;

    if ( requireGenJetMatch_ && !matchesGenJet(*patJet) ) continue;

    fillJetHistograms(*patJet, hJetPt_, hJetEta_, hJetPhi_);
    hJetPtVsEta_->Fill(patJet->eta(), patJet->pt());

    hJetAlpha_->Fill(compAlpha(*patJet));
    unsigned numTracks = 0;
    double maxPt = 0.;
    for ( reco::TrackRefVector::const_iterator track = patJet->associatedTracks().begin();
	  track != patJet->associatedTracks().end(); ++track ) {
      ++numTracks;
      hJetTrkPt_->Fill((*track)->pt());
      if ( (*track)->pt() > maxPt ) maxPt = (*track)->pt();
    }
    hJetNumTracks_->Fill(numTracks);
    if ( numTracks > 0 ) hJetLeadTrkPt_->Fill(maxPt);

    
    for (unsigned int i=0;i<bTaggingDiscriminators_.size();i++) {
      hBtagDisc_[i]->Fill(patJet->bDiscriminator(bTaggingDiscriminators_[i]));
      if (patJet->bDiscriminator(bTaggingDiscriminators_[i])> bTaggingDiscriminatorThresholds_[i]) {
        nbtags[i]++;
        hPtBtags_[i]->Fill(patJet->pt());
      }
    }
  }
  for (unsigned int i=0;i<bTaggingDiscriminators_.size();i++) {
    hNumBtags_[i]->Fill(nbtags[i]);
  }

  int index = 0;
  for ( vdouble::const_iterator etMin = centralJetsToBeVetoedEtMin_.begin();
	etMin != centralJetsToBeVetoedEtMin_.end(); ++etMin ) {
    for ( vdouble::const_iterator etaMax = centralJetsToBeVetoedEtaMax_.begin();
	  etaMax != centralJetsToBeVetoedEtaMax_.end(); ++etaMax ) {
      for ( vdouble::const_iterator alphaMin = centralJetsToBeVetoedAlphaMin_.begin();
	    alphaMin != centralJetsToBeVetoedAlphaMin_.end(); ++alphaMin ) {
	fillNumCentralJetsToBeVetoesHistograms(*patJets, hNumCentralJetsToBeVetoed_[index], *etMin, *etaMax, *alphaMin);
	++index;
      }
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void JetHistManager::bookJetHistograms(DQMStore& dqmStore, MonitorElement*& hJetPt, MonitorElement*& hJetEta, MonitorElement*& hJetPhi, const char* histoSetName)
{
  std::string hJetPtName = std::string(histoSetName).append("Pt");
  hJetPt = dqmStore.book1D(hJetPtName, hJetPtName, 75, 0., 150.);
  
  std::string hJetEtaName = std::string(histoSetName).append("Eta");
  hJetEta = dqmStore.book1D(hJetEtaName, hJetEtaName, 60, -3., +3.);
  
  std::string hJetPhiName = std::string(histoSetName).append("Phi");
  hJetPhi = dqmStore.book1D(hJetPhiName, hJetPhiName, 36, -TMath::Pi(), +TMath::Pi());
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void JetHistManager::fillJetHistograms(const pat::Jet& patJet, MonitorElement* hJetPt, MonitorElement* hJetEta, MonitorElement* hJetPhi)
{
  //std::cout << "<JetHistManager::fillJetHistograms>:" << std::endl;

  hJetPt->Fill(patJet.pt());
  hJetEta->Fill(patJet.eta());
  hJetPhi->Fill(patJet.phi());
}

void JetHistManager::fillNumCentralJetsToBeVetoesHistograms(const std::vector<pat::Jet>& patJets, MonitorElement* hNumJets,
							    double etMin, double etaMax, double alphaMin)
{
  unsigned numJets = 0;
  for ( std::vector<pat::Jet>::const_iterator patJet = patJets.begin();
	patJet != patJets.end(); ++patJet ) {
    if ( patJet->et() >= etMin && patJet->eta() <= etaMax && compAlpha(*patJet) >= alphaMin ) ++numJets;
  }
  hNumJets->Fill(numJets);
}


#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, JetHistManager, "JetHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, JetHistManager, "JetHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<JetHistManager> JetAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(JetAnalyzer);
