#ifndef TauAnalysis_RecoTools_ParticlePFIsolationExtractor_h
#define TauAnalysis_RecoTools_ParticlePFIsolationExtractor_h

/** \class ParticlePFIsolationExtractor
 *
 * Auxiliary class to compute isolation of pat::Leptons based on charged hadrons, 
 * neutral hadrons and photons/pi0s reconstructed by particle-flow algorithm
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: ParticlePFIsolationExtractor.h,v 1.2 2011/02/10 13:56:44 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <TMath.h>

#include <vector>

template <class T>
class ParticlePFIsolationExtractor
{
 public:
  explicit ParticlePFIsolationExtractor(const edm::ParameterSet& cfg):
	pfChargedHadronIso_(0),
	pfNeutralHadronIso_(0),
	pfPhotonIso_(0)
  {
	
	if( cfg.exists("chargedHadronIso") ) {
		edm::ParameterSet cfgChargedHadronIso = cfg.getParameter<edm::ParameterSet>("chargedHadronIso");
		pfChargedHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h, cfgChargedHadronIso);
	}
	if( cfg.exists("neutralHadronIso") ) {
		edm::ParameterSet cfgNeutralHadronIso = cfg.getParameter<edm::ParameterSet>("neutralHadronIso");
		pfNeutralHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h0, cfgNeutralHadronIso);
	}
	if( cfg.exists("photonIso") ) {
		edm::ParameterSet cfgPhotonIso = cfg.getParameter<edm::ParameterSet>("photonIso");
		pfPhotonIso_ = new pfIsoConfigType(reco::PFCandidate::gamma, cfgPhotonIso);
	}
  }
  ~ParticlePFIsolationExtractor()
  {
    if(pfChargedHadronIso_) delete pfChargedHadronIso_;
    if(pfNeutralHadronIso_) delete pfNeutralHadronIso_;
    if(pfPhotonIso_) delete pfPhotonIso_;
  }

  double operator()(const T& lepton, const reco::PFCandidateCollection& pfCandidates)
  {
	double sumPt = 0;
    if(pfChargedHadronIso_) sumPt += pfChargedHadronIso_->compSumPt(pfCandidates, lepton.p4());
    if(pfNeutralHadronIso_) sumPt += pfNeutralHadronIso_->compSumPt(pfCandidates, lepton.p4());
    if(pfPhotonIso_) sumPt += pfPhotonIso_->compSumPt(pfCandidates, lepton.p4());
    return sumPt;
  }

 private:

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

      if ( TMath::IsNaN(pfCandidate.pt()) || pfCandidate.pt() < ptMin_ ) return false;

      double dR = deltaR(pfCandidate.p4(), isoParticleCandidateP4);
      if ( dR < dRvetoCone_ || dR > dRisoCone_ ) return false;

      if ( TMath::Abs(pfCandidate.eta() - isoParticleCandidateP4.eta()) < dEtaVeto_ ) return false;
      if ( TMath::Abs(pfCandidate.phi() - isoParticleCandidateP4.phi()) < dPhiVeto_ ) return false;

      //std::cout << "<ParticlePFIsolationExtractor::passesVeto>:" << std::endl;
      //std::cout << " veto passed: Pt = " << pfCandidate.pt() << "," 
      //	  << " eta = " << pfCandidate.eta() << ", phi = " << pfCandidate.phi() << ", dR = " << dR << std::endl;

      return true;
    }

	double compSumPt(const reco::PFCandidateCollection& pfCandidates, const reco::Particle::LorentzVector& isoParticleCandidateP4)
	{
		double sumPt = 0.;

		if ( vetoNumHighestPtObjects_ > 0 ) {
			std::vector<double> pfCandidatePt;

			for ( reco::PFCandidateCollection::const_iterator pfCandidate = pfCandidates.begin();
					pfCandidate != pfCandidates.end(); ++pfCandidate ) {
				if ( !passesVeto(*pfCandidate, isoParticleCandidateP4) ) continue;

				pfCandidatePt.push_back(pfCandidate->pt());
			}

			// sort transverse momenta of particle-flow candidates
			// ( lowest/highest Pt value will be stored in pfCandidatePt[0]/pfCandidatePt[numPFCandidates - 1];
			//  cf. http://www.cplusplus.com/reference/algorithm/sort/ )
			std::sort(pfCandidatePt.begin(), pfCandidatePt.end());

			int numPt = pfCandidatePt.size();
			for ( int iPt = 0; iPt < (numPt - (int)vetoNumHighestPtObjects_); ++iPt ) {
				sumPt += pfCandidatePt[iPt];
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

  pfIsoConfigType* pfChargedHadronIso_;
  pfIsoConfigType* pfNeutralHadronIso_;
  pfIsoConfigType* pfPhotonIso_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ParticlePFIsolationExtractor<pat::Electron> PATElectronPFIsolationExtractor;
typedef ParticlePFIsolationExtractor<pat::Muon> PATMuonPFIsolationExtractor;
typedef ParticlePFIsolationExtractor<pat::Tau> PATTauPFIsolationExtractor;

#endif


