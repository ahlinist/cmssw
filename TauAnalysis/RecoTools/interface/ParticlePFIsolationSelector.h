#ifndef TauAnalysis_RecoTools_ParticlePFIsolationSelector_h
#define TauAnalysis_RecoTools_ParticlePFIsolationSelector_h

/** \class ParticlePFIsolationSelector
 *
 * Flexible selection of pat::Leptons based on charged hadrons, 
 * neutral hadrons and photons/pi0s reconstructed by particle-flow algorithm;
 * a pat::Electron, pat::Muon or pat::Tau passed the selection
 * if the sum of deposits of the speficied type and with Pt > ptMin 
 * in an annulus within dRvetoCone (inner cone) and dRisoCone (outer cone)
 * is more than sumPtMin and less than sumPtMax 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ParticlePFIsolationSelector.h,v 1.2 2010/10/22 11:51:17 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <TMath.h>

#include <vector>
#include <string>
#include <algorithm>

template <class T>
class ParticlePFIsolationSelector
{
 public:
  typedef std::vector<T> collection;

  explicit ParticlePFIsolationSelector(const edm::ParameterSet& cfg)
    : sumPtMethod_(kAbsoluteIso),
      cfgError_(0)
  {
    pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");

    edm::ParameterSet cfgChargedHadronIso = cfg.getParameter<edm::ParameterSet>("chargedHadronIso");
    pfChargedHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h, cfgChargedHadronIso);

    edm::ParameterSet cfgNeutralHadronIso = cfg.getParameter<edm::ParameterSet>("neutralHadronIso");
    pfNeutralHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h0, cfgNeutralHadronIso);

    edm::ParameterSet cfgPhotonIso = cfg.getParameter<edm::ParameterSet>("photonIso");
    pfPhotonIso_ = new pfIsoConfigType(reco::PFCandidate::gamma, cfgPhotonIso);
  
    sumPtMin_ = cfg.exists("sumPtMin") ? 
      cfg.getParameter<double>("sumPtMin") : -1.;
    sumPtMax_ = cfg.getParameter<double>("sumPtMax");
    
    if ( cfg.exists("sumPtMethod") ) {
      std::string sumPtMethod_string = cfg.getParameter<std::string>("sumPtMethod");
      if ( sumPtMethod_string == "absolute" ) {
	sumPtMethod_ = kAbsoluteIso;
      } else if ( sumPtMethod_string == "relative" ) {
	sumPtMethod_ = kRelativeIso;
      } else {
	edm::LogError("ParticlePFIsolationSelector") 
	  << " Configuration parameter 'sumPtMethod' = " << sumPtMethod_string << " invalid !!";
	cfgError_ = 1;
      }
    }
  }
  ~ParticlePFIsolationSelector()
  {
    delete pfChargedHadronIso_;
    delete pfNeutralHadronIso_;
    delete pfPhotonIso_;
  }

  typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
  typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>& isoParticleCandidates, const edm::Event& evt, const edm::EventSetup& es)
  {
    selected_.clear();
  
    if ( cfgError_ ) {
      edm::LogError ("select") 
	<< " Error in Configuration ParameterSet --> no iso. particle candidate will pass Selection !!";
      return;
    }
    
    edm::Handle<reco::PFCandidateCollection> pfCandidates;
    evt.getByLabel(pfCandidateSrc_, pfCandidates);

    for ( typename collection::const_iterator isoParticleCandidate = isoParticleCandidates->begin();
	  isoParticleCandidate != isoParticleCandidates->end(); ++isoParticleCandidate ) {
      double sumPt = pfChargedHadronIso_->compSumPt(*pfCandidates, isoParticleCandidate->p4());
      sumPt += pfNeutralHadronIso_->compSumPt(*pfCandidates, isoParticleCandidate->p4());
      sumPt += pfPhotonIso_->compSumPt(*pfCandidates, isoParticleCandidate->p4());
      
      if ( sumPtMethod_ == kAbsoluteIso ) {
	if ( sumPtMin_ > 0. && sumPt < sumPtMin_ ) continue;
	if ( sumPtMax_ > 0. && sumPt > sumPtMax_ ) continue;
      } else if ( sumPtMethod_ == kRelativeIso ) {
	double relIso = ( isoParticleCandidate->pt() > 1. ) ? (sumPt/isoParticleCandidate->pt()) : sumPt;
	if ( sumPtMin_ > 0. && relIso < sumPtMin_ ) continue;
	if ( sumPtMax_ > 0. && relIso > sumPtMax_ ) continue;
      }
      
      selected_.push_back(&(*isoParticleCandidate)); 
    }
  }

  size_t size() const { return selected_.size(); }

 private:
  std::vector<const T*> selected_;

  struct pfIsoConfigType
  {
    pfIsoConfigType(reco::PFCandidate::ParticleType pfParticleType, const edm::ParameterSet& cfg)
      : pfParticleType_(pfParticleType),
	ptMin_(cfg.getParameter<double>("ptMin")),
	dRvetoCone_(cfg.getParameter<double>("dRvetoCone")),
	dRisoCone_(cfg.getParameter<double>("dRisoCone"))
    {
      dEtaVeto_ = cfg.exists("dEtaVeto") ? 
	cfg.getParameter<double>("dEtaVeto") : -1.;
      dPhiVeto_ = cfg.exists("dPhiVeto") ? 
	cfg.getParameter<double>("dPhiVeto") : -1.;

      vetoNumHighestPtObjects_ = cfg.exists("vetoNumHighestPtObjects") ? 
	cfg.getParameter<unsigned>("vetoNumHighestPtObjects") : 0;
    }
    ~pfIsoConfigType() {}

    bool passesVeto(const reco::PFCandidate& pfCandidate, const reco::Particle::LorentzVector& isoParticleCandidateP4)
    {
      if ( pfCandidate.particleId() != pfParticleType_ ) return false;

      if ( pfCandidate.pt() < ptMin_ ) return false;

      double dR = deltaR(pfCandidate.p4(), isoParticleCandidateP4);
      if ( dR < dRvetoCone_ || dR > dRisoCone_ ) return false;

      if ( TMath::Abs(pfCandidate.eta() - isoParticleCandidateP4.eta()) < dEtaVeto_ ) return false;
      if ( TMath::Abs(pfCandidate.phi() - isoParticleCandidateP4.phi()) < dPhiVeto_ ) return false;

      return true;
    }

    double compSumPt(const reco::PFCandidateCollection& pfCandidates, const reco::Particle::LorentzVector& isoParticleCandidateP4)
    {
      double sumPt = 0.;

      if ( vetoNumHighestPtObjects_ > 0 ) {
	size_t numPFCandidates = pfCandidates.size();
 
	std::vector<double> pfCandidatePt;
	//pfCandidatePt.reserve(numPFCandidates);

	for ( size_t iPFCandidate = 0; iPFCandidate < numPFCandidates; ++iPFCandidate ) {
	  const reco::PFCandidate& pfCandidate = pfCandidates[iPFCandidate];
	  
	  if ( !passesVeto(pfCandidate, isoParticleCandidateP4) ) continue;
	  
	  pfCandidatePt.push_back(pfCandidate.pt());
	}

	// sort transverse momenta of particle-flow candidates
	// ( lowest/highest Pt value will be stored in pfCandidatePt[0]/pfCandidatePt[numPFCandidates - 1];
	//  cf. http://www.cplusplus.com/reference/algorithm/sort/ )
	std::sort(pfCandidatePt.begin(), pfCandidatePt.end());
	
	for ( size_t iPFCandidate = 0; iPFCandidate < (numPFCandidates - vetoNumHighestPtObjects_); ++iPFCandidate ) {
	  sumPt += pfCandidatePt[iPFCandidate];
	}
      } else {
	for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
	      pfCandidate != pfCandidates.end(); ++pfCandidate ) {
	  if ( !passesVeto(*pfCandidate, isoParticleCandidateP4) ) continue;

	  sumPt += pfCandidate->pt();
	}   
      } 

      return sumPt;
    }

    reco::PFCandidate::ParticleType pfParticleType_;

    double ptMin_;

    double dRvetoCone_;
    double dRisoCone_;

    double dEtaVeto_;
    double dPhiVeto_;

    unsigned vetoNumHighestPtObjects_;
  };

  edm::InputTag pfCandidateSrc_;

  pfIsoConfigType* pfChargedHadronIso_;
  pfIsoConfigType* pfNeutralHadronIso_;
  pfIsoConfigType* pfPhotonIso_;

  double sumPtMin_;
  double sumPtMax_;
  
  enum { kAbsoluteIso, kRelativeIso };
  int sumPtMethod_;

  int cfgError_;
};

#endif


