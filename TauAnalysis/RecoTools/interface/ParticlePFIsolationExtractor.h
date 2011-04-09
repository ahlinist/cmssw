#ifndef TauAnalysis_RecoTools_ParticlePFIsolationExtractor_h
#define TauAnalysis_RecoTools_ParticlePFIsolationExtractor_h

/** \class ParticlePFIsolationExtractor
 *
 * Auxiliary class to compute isolation of pat::Leptons based on charged hadrons, 
 * neutral hadrons and photons/pi0s reconstructed by particle-flow algorithm
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: ParticlePFIsolationExtractor.h,v 1.5 2011/04/05 13:27:58 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/RecoTools/interface/PATLeptonTrackRefVectorExtractor.h"
#include "TauAnalysis/RecoTools/interface/pfCandAuxFunctions.h"

#include <TMath.h>

#include <vector>

template <class T>
class ParticlePFIsolationExtractor
{
 public:
  enum { kNone, kBeta, kDeltaBeta };
  explicit ParticlePFIsolationExtractor(const edm::ParameterSet& cfg):
    pfChargedHadronIso_(0),
    addChargedHadronIso_(false),
    pfNeutralHadronIso_(0),
    addNeutralHadronIso_(false),
    pfPhotonIso_(0),
    addPhotonIso_(false),
    methodPUcorr_(kNone),
    trackExtractor_(0),
    pfNeutralHadronIsoPUcorr_(0),
    pfPhotonIsoPUcorr_(0)
  {    
    if( cfg.exists("chargedHadronIso") ) {
      edm::ParameterSet cfgChargedHadronIso = cfg.getParameter<edm::ParameterSet>("chargedHadronIso");
      pfChargedHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h, cfgChargedHadronIso);
      addChargedHadronIso_ = cfgChargedHadronIso.exists("add") ? cfgChargedHadronIso.getParameter<bool>("add") : true;
    }

    if( cfg.exists("neutralHadronIso") ) {
      edm::ParameterSet cfgNeutralHadronIso = cfg.getParameter<edm::ParameterSet>("neutralHadronIso");
      pfNeutralHadronIso_ = new pfIsoConfigType(reco::PFCandidate::h0, cfgNeutralHadronIso);
      addNeutralHadronIso_ = cfgNeutralHadronIso.exists("add") ? cfgNeutralHadronIso.getParameter<bool>("add") : true;
    }

    if( cfg.exists("photonIso") ) {
      edm::ParameterSet cfgPhotonIso = cfg.getParameter<edm::ParameterSet>("photonIso");
      pfPhotonIso_ = new pfIsoConfigType(reco::PFCandidate::gamma, cfgPhotonIso);
      addPhotonIso_ = cfgPhotonIso.exists("add") ? cfgPhotonIso.getParameter<bool>("add") : true;
    }

    if ( cfg.exists("pileUpCorr") ) {
      edm::ParameterSet cfgPUcorr = cfg.getParameter<edm::ParameterSet>("pileUpCorr");

      std::string method_string = cfgPUcorr.getParameter<std::string>("method");
      if      ( method_string == "beta"      ) methodPUcorr_ = kBeta;
      else if ( method_string == "deltaBeta" ) methodPUcorr_ = kDeltaBeta;
      else if ( method_string == "none"      ) methodPUcorr_ = kNone;
      else throw cms::Exception("ParticlePFIsolationExtractor")
	<< "Invalid Configuration parameter method = " << method_string << "!!\n";
      
      if ( methodPUcorr_ != kNone ) {
	trackExtractor_ = new PATLeptonTrackRefVectorExtractor<T>();
	
	deltaZ_ = cfgPUcorr.getParameter<double>("deltaZ");
	if ( methodPUcorr_ == kDeltaBeta ) chargedToNeutralFactor_ = cfgPUcorr.getParameter<double>("chargedToNeutralFactor");
	
	if ( pfNeutralHadronIso_ ) {
	  edm::ParameterSet cfgNeutralHadronIso = cfg.getParameter<edm::ParameterSet>("neutralHadronIso");
	  pfNeutralHadronIsoPUcorr_ = new pfIsoConfigType(reco::PFCandidate::h, cfgNeutralHadronIso);
	}
	
	if ( pfPhotonIso_ ) {
	  edm::ParameterSet cfgPhotonIso = cfg.getParameter<edm::ParameterSet>("photonIso");
	  pfPhotonIsoPUcorr_ = new pfIsoConfigType(reco::PFCandidate::h, cfgPhotonIso);
	}
	
	if ( !pfChargedHadronIso_ ) throw cms::Exception("ParticlePFIsolationExtractor")
	  << "Pile-up correction requires 'chargedHadronIso' Configuration parameter !!\n";
      }
    }
  }
  ~ParticlePFIsolationExtractor()
  {
    delete pfChargedHadronIso_;
    delete pfNeutralHadronIso_;
    delete pfPhotonIso_;

    delete trackExtractor_;
    delete pfNeutralHadronIsoPUcorr_;
    delete pfPhotonIsoPUcorr_;
  }

  double operator()(const T& lepton, const reco::PFCandidateCollection& pfCandidates,
		    const reco::VertexCollection* vertices = 0, const reco::BeamSpot* beamSpot = 0)
  {
    //std::cout << "<ParticlePFIsolationExtractor::operator()>:" << std::endl;

    std::vector<const reco::PFCandidate*> pfChargedHadrons, pfNeutralHadrons, pfPhotons;
    if ( addChargedHadronIso_   || 
	 methodPUcorr_ != kNone ) pfChargedHadrons = getPFCandidatesOfType(pfCandidates, reco::PFCandidate::h);
    if ( addNeutralHadronIso_   ) pfNeutralHadrons = getPFCandidatesOfType(pfCandidates, reco::PFCandidate::h0);
    if ( addPhotonIso_          ) pfPhotons        = getPFCandidatesOfType(pfCandidates, reco::PFCandidate::gamma);

    double sumPt = 0.;
    
    if ( methodPUcorr_ == kNone ) {
      if ( addChargedHadronIso_ ) sumPt += pfChargedHadronIso_->compSumPt(pfChargedHadrons, lepton.p4());
      if ( addNeutralHadronIso_ ) sumPt += pfNeutralHadronIso_->compSumPt(pfNeutralHadrons, lepton.p4());
      if ( addPhotonIso_        ) sumPt += pfPhotonIso_->compSumPt(pfPhotons, lepton.p4());
    } else {
      if ( vertices == 0 || beamSpot == 0 ) throw cms::Exception("ParticlePFIsolationExtractor")
	<< "Pile-up correction Method = 'deltaBeta' requires Vertex collection and BeamSpot !!\n";

      std::vector<reco::TrackBaseRef> signalTracks = (*trackExtractor_)(lepton);

      std::vector<const reco::PFCandidate*> pfNoPileUpChargedHadrons, pfPileUpChargedHadrons;
      getPileUpPFCandidates(pfChargedHadrons, signalTracks, *vertices, deltaZ_, *beamSpot, pfNoPileUpChargedHadrons, pfPileUpChargedHadrons);

      if ( methodPUcorr_ == kBeta ) {
	//std::cout << "applying beta correction..." << std::endl;
	double pfNoPileUpChargedHadronIsoSumPt = pfChargedHadronIso_->compSumPt(pfNoPileUpChargedHadrons, lepton.p4());
	double pfPileUpChargedHadronIsoSumPt   = pfChargedHadronIso_->compSumPt(pfPileUpChargedHadrons, lepton.p4());
	sumPt = pfNoPileUpChargedHadronIsoSumPt;

	//std::cout << " pfNoPileUpChargedHadronIsoSumPt = " << pfNoPileUpChargedHadronIsoSumPt << std::endl;
	//std::cout << " pfPileUpChargedHadronIsoSumPt = " << pfPileUpChargedHadronIsoSumPt << std::endl;

	double pfNeutralIsoCorrFactor = ( pfPileUpChargedHadronIsoSumPt > 0. ) ? 
	  (pfNoPileUpChargedHadronIsoSumPt/(pfNoPileUpChargedHadronIsoSumPt + pfPileUpChargedHadronIsoSumPt)) : 1.0;
	//std::cout << " pfNeutralIsoCorrFactor = " << pfNeutralIsoCorrFactor << std::endl;
	
	if ( pfNeutralHadronIso_ ) sumPt += pfNeutralHadronIso_->compSumPt(pfNeutralHadrons, lepton.p4())*pfNeutralIsoCorrFactor;
	if ( pfPhotonIso_        ) sumPt += pfPhotonIso_->compSumPt(pfPhotons, lepton.p4())*pfNeutralIsoCorrFactor;
	//std::cout << "--> sumPt = " << sumPt << std::endl;
      } else if ( methodPUcorr_ == kDeltaBeta ) {
	//std::cout << "applying deltaBeta correction..." << std::endl;
	sumPt = pfChargedHadronIso_->compSumPt(pfNoPileUpChargedHadrons, lepton.p4());
	//std::cout << "pfNoPileUpChargedHadronIsoSumPt = " << sumPt << std::endl;

	double sumPtNeutralIsoSumPt  = 0.;
	double sumPtNeutralIsoPUcorr = 0.;
      
	if ( pfNeutralHadronIso_ ) {
	  sumPtNeutralIsoSumPt += pfNeutralHadronIso_->compSumPt(pfNeutralHadrons, lepton.p4());
	  sumPtNeutralIsoPUcorr += pfNeutralHadronIsoPUcorr_->compSumPt(pfPileUpChargedHadrons, lepton.p4());
	}
	
	if ( pfPhotonIso_ ) {
	  sumPtNeutralIsoSumPt += pfPhotonIso_->compSumPt(pfPhotons, lepton.p4());
	  sumPtNeutralIsoPUcorr += pfPhotonIsoPUcorr_->compSumPt(pfPileUpChargedHadrons, lepton.p4());
	}

	//std::cout << " sumPtNeutralIsoSumPt = " << sumPtNeutralIsoSumPt << std::endl;
	//std::cout << " sumPtNeutralIsoPUcorr = " << sumPtNeutralIsoPUcorr << std::endl;

	sumPt += TMath::Max(sumPtNeutralIsoSumPt - chargedToNeutralFactor_*sumPtNeutralIsoPUcorr, 0.); 
	//std::cout << "--> sumPt = " << sumPt << std::endl;
      } else assert(0);
    }

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

    double compSumPt(const std::vector<const reco::PFCandidate*>& pfCandidates, 
		     const reco::Particle::LorentzVector& isoParticleCandidateP4)
    {
      double sumPt = 0.;
      
      if ( vetoNumHighestPtObjects_ > 0 ) {
	std::vector<double> pfCandidatePt;
	
	for ( std::vector<const reco::PFCandidate*>::const_iterator pfCandidate = pfCandidates.begin();
	      pfCandidate != pfCandidates.end(); ++pfCandidate ) {
	  if ( !passesVeto(**pfCandidate, isoParticleCandidateP4) ) continue;
	  
	  pfCandidatePt.push_back((*pfCandidate)->pt());
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
	for ( std::vector<const reco::PFCandidate*>::const_iterator pfCandidate = pfCandidates.begin();
	      pfCandidate != pfCandidates.end(); ++pfCandidate ) {
	  if ( !passesVeto(**pfCandidate, isoParticleCandidateP4) ) continue;
	  
	  sumPt += (*pfCandidate)->pt();
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
  bool addChargedHadronIso_;
  pfIsoConfigType* pfNeutralHadronIso_;
  bool addNeutralHadronIso_;
  pfIsoConfigType* pfPhotonIso_;
  bool addPhotonIso_;

  int methodPUcorr_;
  double deltaZ_;
  double chargedToNeutralFactor_;
  PATLeptonTrackRefVectorExtractor<T>* trackExtractor_;			  
  pfIsoConfigType* pfNeutralHadronIsoPUcorr_;
  pfIsoConfigType* pfPhotonIsoPUcorr_;  
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef ParticlePFIsolationExtractor<pat::Electron> PATElectronPFIsolationExtractor;
typedef ParticlePFIsolationExtractor<pat::Muon> PATMuonPFIsolationExtractor;
typedef ParticlePFIsolationExtractor<pat::Tau> PATTauPFIsolationExtractor;

#endif


