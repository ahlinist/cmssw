#include "TauAnalysis/Skimming/plugins/MuonIsolationTagAndProbeProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Common/interface/Association.h"

#include <TMath.h>

#include <vector>

typedef std::vector<reco::Candidate::LorentzVector> vLorentzVector;

typedef edm::ValueMap<int> MuonIsolationTagAndProbeFlags;

const char* instanceNameFlags = "flags";
const char* instanceNameP4Tags = "p4Tags";
const char* instanceNameP4Probes = "p4Probes";

MuonIsolationTagAndProbeProducer::MuonIsolationTagAndProbeProducer(const edm::ParameterSet& cfg)
{
//--- read configuration parameters
  srcAllMuons_ = cfg.getParameter<edm::InputTag>("srcAllMuons");
  srcGoodMuons_ = cfg.getParameter<edm::InputTag>("srcGoodMuons");
  srcGoodIsoMuons_ = cfg.getParameter<edm::InputTag>("srcGoodIsoMuons");

//--- register products
  produces<MuonIsolationTagAndProbeFlags>(instanceNameFlags);

  produces<vLorentzVector>(instanceNameP4Tags);
  produces<vLorentzVector>(instanceNameP4Probes);
}

MuonIsolationTagAndProbeProducer::~MuonIsolationTagAndProbeProducer()
{
  // nothing to be done yet...
}

bool isMatched(const reco::Muon& ref, const reco::CandidateView& muons, double dMatchMax = 0.1)
{
  for ( reco::CandidateView::const_iterator muon = muons.begin();
	muon != muons.end(); ++muon ) {
    if ( muon->pt() == 0. ) continue;
    
    double dInvPt = ( ref.pt() != 0. ) ? (1./muon->pt() - 1./ref.pt()) : 0.;
    double dEta = ref.eta() - muon->eta();
    double dPhi = TMath::ACos(TMath::Cos(ref.phi() - muon->phi()));
    
    double dMatch = TMath::Sqrt(dInvPt*dInvPt + dEta*dEta + dPhi*dPhi);
    
    if ( dMatch < dMatchMax ) return true;
  }
  
  return false;
}

void MuonIsolationTagAndProbeProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
//--- create products
  std::auto_ptr<MuonIsolationTagAndProbeFlags> flags(new MuonIsolationTagAndProbeFlags);
  MuonIsolationTagAndProbeFlags::Filler valueMapFiller(*flags);

  std::auto_ptr<vLorentzVector> p4Tags(new vLorentzVector);
  std::auto_ptr<vLorentzVector> p4Probes(new vLorentzVector);

  edm::Handle<reco::MuonCollection> allMuons;
  evt.getByLabel(srcAllMuons_, allMuons);
  size_t numMuons = allMuons->size();

  edm::Handle<reco::CandidateView> goodMuons;
  evt.getByLabel(srcGoodMuons_, goodMuons);
  
  edm::Handle<reco::CandidateView> goodIsoMuons;
  evt.getByLabel(srcGoodIsoMuons_, goodIsoMuons);
  size_t numGoodIsoMuons = goodIsoMuons->size();
  
  std::vector<int> flags_tmp(numMuons);

  for ( size_t iMuon = 0; iMuon < numMuons; ++iMuon ) {
    const reco::Muon& muon = allMuons->at(iMuon);

    bool isGoodMuon = isMatched(muon, *goodMuons);
    bool isGoodIsoMuon = isMatched(muon, *goodIsoMuons);

    bool isTag   = ( isGoodMuon &&   numGoodIsoMuons >= 1 &&  isGoodIsoMuon ) ? true : false;
    bool isProbe = ( isGoodMuon && ((numGoodIsoMuons == 1 && !isGoodIsoMuon) || numGoodIsoMuons >= 2) ) ? true : false;

    int flag = 0;
    if ( isTag   ) flag += tagBit;
    if ( isProbe ) flag += probeBit;
    flags_tmp[iMuon] = flag;

    if ( isTag   ) p4Tags->push_back(muon.p4());
    if ( isProbe ) p4Probes->push_back(muon.p4());
  }

  valueMapFiller.insert(allMuons, flags_tmp.begin(), flags_tmp.end());
  valueMapFiller.fill();

  evt.put(flags, instanceNameFlags);

  evt.put(p4Tags, instanceNameP4Tags);
  evt.put(p4Probes, instanceNameP4Probes);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(MuonIsolationTagAndProbeProducer);


