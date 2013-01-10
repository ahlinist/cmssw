#include "TauAnalysis/RecoTools/plugins/VertexByLeptonTrackMatchSelector.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/OneToManyWithQuality.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <TMath.h>

typedef edm::AssociationMap<edm::OneToManyWithQuality<reco::VertexCollection, reco::TrackCollection, float> > TrackVertexAssMap;

VertexByLeptonTrackMatchSelectorImp::VertexByLeptonTrackMatchSelectorImp(const edm::ParameterSet& cfg )
{ 
  srcLeptons_ = cfg.getParameter<vInputTag>("srcLeptons");

  srcTrackToVertexAssociations_ = cfg.getParameter<edm::InputTag>("srcTrackToVertexAssociations");

  dZ_ = cfg.getParameter<double>("dZ");
  minAssocProb_ = cfg.getParameter<double>("minAssocProb");

  verbosity_ = cfg.exists("verbosity") ?
    cfg.getParameter<int>("verbosity") : 0;
}

namespace
{
  std::vector<const reco::Track*> extractLeptonTracks(const reco::Candidate* lepton)
  {
    std::vector<const reco::Track*> leptonTracks;
    if ( dynamic_cast<const reco::GsfElectron*>(lepton) ) {
      const reco::GsfElectron* electron = dynamic_cast<const reco::GsfElectron*>(lepton);
      if ( !electron->gsfTrack().isNull() ) leptonTracks.push_back(electron->gsfTrack().get());
      //else if ( !electron->closestCtfTrackRef().isNull() ) leptonTracks.push_back(electron->closestCtfTrackRef().get());
    } else if ( dynamic_cast<const pat::Electron*>(lepton) ) {
      const pat::Electron* electron = dynamic_cast<const pat::Electron*>(lepton);
      if ( !electron->gsfTrack().isNull() ) leptonTracks.push_back(electron->gsfTrack().get());
      //else if ( !electron->closestCtfTrackRef().isNull() ) leptonTracks.push_back(electron->closestCtfTrackRef().get());
    } else if ( dynamic_cast<const reco::Muon*>(lepton) ) {
      const reco::Muon* muon = dynamic_cast<const reco::Muon*>(lepton);
      if ( !muon->innerTrack().isNull() ) leptonTracks.push_back(muon->innerTrack().get());
    } else if ( dynamic_cast<const pat::Muon*>(lepton) ) {
      const pat::Muon* muon = dynamic_cast<const pat::Muon*>(lepton);
      if ( !muon->innerTrack().isNull() ) leptonTracks.push_back(muon->innerTrack().get());
    } else if ( dynamic_cast<const reco::PFTau*>(lepton) ) {
      const reco::PFTau* tau = dynamic_cast<const reco::PFTau*>(lepton);
      const reco::PFCandidateRefVector& tauPFChargedHadrCands = tau->signalPFChargedHadrCands();
      for ( reco::PFCandidateRefVector::const_iterator tauPFChargedHadrCand = tauPFChargedHadrCands.begin();
	    tauPFChargedHadrCand != tauPFChargedHadrCands.end(); ++tauPFChargedHadrCand ) {
	if ( !(*tauPFChargedHadrCand)->trackRef().isNull() ) leptonTracks.push_back((*tauPFChargedHadrCand)->trackRef().get());
	else if ( !(*tauPFChargedHadrCand)->gsfTrackRef().isNull() ) leptonTracks.push_back((*tauPFChargedHadrCand)->gsfTrackRef().get());
      }
    } else if ( dynamic_cast<const pat::Tau*>(lepton) ) {
      const pat::Tau* tau = dynamic_cast<const pat::Tau*>(lepton);
      if ( tau->isPFTau() ) {
	const reco::PFCandidateRefVector& tauPFChargedHadrCands = tau->signalPFChargedHadrCands();
	for ( reco::PFCandidateRefVector::const_iterator tauPFChargedHadrCand = tauPFChargedHadrCands.begin();
	      tauPFChargedHadrCand != tauPFChargedHadrCands.end(); ++tauPFChargedHadrCand ) {
	  if ( !(*tauPFChargedHadrCand)->trackRef().isNull() ) leptonTracks.push_back((*tauPFChargedHadrCand)->trackRef().get());
	  else if ( !(*tauPFChargedHadrCand)->gsfTrackRef().isNull() ) leptonTracks.push_back((*tauPFChargedHadrCand)->gsfTrackRef().get());
	}
      } else if ( tau->isCaloTau() ) {
	const reco::TrackRefVector& tauSignalTracks = tau->signalTracks();
	for ( reco::TrackRefVector::const_iterator tauSignalTrack = tauSignalTracks.begin();
	      tauSignalTrack != tauSignalTracks.end(); ++tauSignalTrack ) {
	  leptonTracks.push_back(tauSignalTrack->get());
	}
      } else assert(0);
    } else {
      throw cms::Exception("extractLeptonTracks")
	<< " Invalid Type of lepton passed as function Argument !!\n";
    }
    return leptonTracks;
  }
}

void VertexByLeptonTrackMatchSelectorImp::select(const edm::Handle<collection>& vertexCollection,
						 edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  std::vector<const reco::Track*> allLeptonTracks;
  for ( vInputTag::const_iterator srcLepton = srcLeptons_.begin();
	srcLepton != srcLeptons_.end(); ++srcLepton ) {
    typedef edm::View<reco::Candidate> CandidateView;
    edm::Handle<CandidateView> leptons;
    evt.getByLabel(*srcLepton, leptons);
    for ( CandidateView::const_iterator lepton = leptons->begin();
	  lepton != leptons->end(); ++lepton ) {
      std::vector<const reco::Track*> leptonTracks = extractLeptonTracks(&(*lepton));
      allLeptonTracks.insert(allLeptonTracks.begin(), leptonTracks.begin(), leptonTracks.end());
    }
  }

  edm::Handle<TrackVertexAssMap> trackToVertexAssociations;
  evt.getByLabel(srcTrackToVertexAssociations_, trackToVertexAssociations);
  for ( TrackVertexAssMap::const_iterator trackToVertexAssociation = trackToVertexAssociations->begin();
	trackToVertexAssociation != trackToVertexAssociations->end(); ++trackToVertexAssociation ) {
    typedef std::vector<std::pair<reco::TrackRef, float> > TrackQualityPairVector;
    const reco::Vertex& vertex = (*trackToVertexAssociation->key);
    if ( verbosity_ >= 2 ) std::cout << "checking vertex: z = " << vertex.position().z() << std::endl;
    const TrackQualityPairVector& tracks_vertex = trackToVertexAssociation->val;
    unsigned numUnassociatedLeptonTracks = 0;
    for ( std::vector<const reco::Track*>::const_iterator leptonTrack = allLeptonTracks.begin();
	  leptonTrack != allLeptonTracks.end(); ++leptonTrack ) {      
      if ( verbosity_ >= 2 ) std::cout << " associating track: Pt = " << (*leptonTrack)->pt() << ", eta = " << (*leptonTrack)->eta() << ", phi = " << (*leptonTrack)->phi() << std::endl;
      bool isAssociated = false;
      if ( verbosity_ >= 2 ) std::cout << "dZ = " << TMath::Abs((*leptonTrack)->dz(vertex.position())) << std::endl;
      if ( TMath::Abs((*leptonTrack)->dz(vertex.position())) < dZ_ ) {
	for ( TrackQualityPairVector::const_iterator track_vertex = tracks_vertex.begin();
	      track_vertex != tracks_vertex.end(); ++track_vertex ) {
	  if ( deltaR((*leptonTrack)->eta(), (*leptonTrack)->phi(), track_vertex->first->eta(), track_vertex->first->phi()) < 1.e-2 ) {
	    double prob = track_vertex->second;
	    if ( verbosity_ >= 2 ) std::cout << "prob = " << prob << std::endl;
	    if ( TMath::Max(0., prob) > minAssocProb_ ) isAssociated = true;
	  }
	}
      }
      if ( verbosity_ ) std::cout << "--> isAssociated = " << isAssociated << std::endl;
      if ( !isAssociated ) ++numUnassociatedLeptonTracks;
    }
    if ( numUnassociatedLeptonTracks == 0 ) selected_.push_back(&vertex);
  }
  if ( verbosity_ ) std::cout << "#selected vertices = " << selected_.size() << std::endl;
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ObjectSelector<VertexByLeptonTrackMatchSelectorImp> VertexByLeptonTrackMatchSelector;

DEFINE_FWK_MODULE(VertexByLeptonTrackMatchSelector);


