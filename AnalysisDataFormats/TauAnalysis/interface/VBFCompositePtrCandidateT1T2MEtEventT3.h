#ifndef AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3_h
#define AnalysisDataFormats_TauAnalysis_VBFCompositePtrCandidateT1T2MEtEventT3_h

/** \class VBFCompositePtrCandidateT1T2MEtEventT3
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski,
 *          Christian Veelken
 *
 */

#include "DataFormats/Candidate/interface/LeafCandidate.h" 
#include "DataFormats/Common/interface/Ptr.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "Math/VectorUtil.h"

#include <vector>

template<typename T1, typename T2, typename T3> 
class VBFCompositePtrCandidateT1T2MEtEventT3 : public reco::LeafCandidate
{
  typedef CompositePtrCandidateT1T2MEt<T1,T2> diTauType;
  typedef edm::Ptr<diTauType> diTauPtr;
  typedef edm::Ptr<T3> jetPtr;

 public:

  /// default constructor
  VBFCompositePtrCandidateT1T2MEtEventT3()
  {}

  /// constructor with central jets
  VBFCompositePtrCandidateT1T2MEtEventT3(const jetPtr tagJet1, const jetPtr tagJet2, const diTauPtr diTau,
					 const std::vector<jetPtr>& centralJets)
    : tagJet1_(tagJet1), 
      tagJet2_(tagJet2), 
      diTau_(diTau), 
      centralJets_(centralJets)
  {
    initialize();
  }

  /// constructor without central jets
  VBFCompositePtrCandidateT1T2MEtEventT3(const jetPtr tagJet1, const jetPtr tagJet2, const diTauPtr diTau)
    : tagJet1_(tagJet1), 
      tagJet2_(tagJet2), 
      diTau_(diTau)
  {
    initialize();
  }

  /// destructor
  ~VBFCompositePtrCandidateT1T2MEtEventT3() {}

  /// access to tag jets
  const jetPtr tagJet1() const { return tagJet1_; }
  const jetPtr tagJet2() const { return tagJet2_; }

  /// access to diTau object
  const diTauPtr diTau() const { return diTau_; }

  /// access to central jets
  std::vector<jetPtr> centralJets(double etaMargin = 0.) const 
  {
    if ( etaMargin > 0. ) {
      std::vector<jetPtr> retVal;

      for ( typename std::vector<jetPtr>::const_iterator centralJet = centralJets_.begin();
	    centralJet != centralJets_.end(); ++centralJet ) {
	if ( (*centralJet)->eta() < (tagJetEtaMax_ - etaMargin) &&
	     (*centralJet)->eta() > (tagJetEtaMin_ + etaMargin) ) retVal.push_back(*centralJet);
      }

      return retVal;
    } else {
      return centralJets_;
    }
  }
  
  /// compute Zeppenfeld variable: eta_j3 - (etaMin + etaMax)/2
  double tagJetEtaZeppenfeld(unsigned idx = 0) const 
  {
    if ( idx < centralJets_.size() ) {
      return centralJets_[idx]->eta() - 0.5*(tagJetEtaMax_ + tagJetEtaMin_);
    } else {
      edm::LogWarning ("VBFCompositePtrCandidateT1T2MEtEventT3::etaZeppenfeld") 
	<< " Idx = " << idx << " outside Range, number of central Jets = " << centralJets_.size() << " !!";
      return -99.;
    }
  }

  /// get invariant mass of the tag jets
  double mjj() const { return tagJetMass_; }

  /// get delta eta between tag jets
  double dEta() const { return tagJetEtaMax_ - tagJetEtaMin_; }

  /// get delta phi between tag jets
  double dPhi() const { return ROOT::Math::VectorUtil::DeltaPhi(tagJet1()->p4(), tagJet2()->p4()); }

  /// get max. eta of the tag jets
  double tagJetEtaMax() const { return tagJetEtaMax_; }

  /// get min. eta of the tag jets
  double tagJetEtaMin() const { return tagJetEtaMin_; }

 private:

  /// initialise datamembers
  void initialize()
  {
    tagJetEtaMax_ = TMath::Max(tagJet1_->eta(), tagJet2_->eta());
    tagJetEtaMin_ = TMath::Min(tagJet1_->eta(), tagJet2_->eta());
    tagJetMass_   = (tagJet1()->p4() + tagJet2()->p4()).mass();
  }

  /// pointers to tag jets
  jetPtr tagJet1_;
  jetPtr tagJet2_;

  /// pointer to diTau object
  diTauPtr diTau_;

  /// pointers to central jets (sorted in order of descending Pt)
  std::vector<jetPtr> centralJets_;

  /// min./max. eta, mass of tag jets
  double tagJetEtaMax_;
  double tagJetEtaMin_;
  double tagJetMass_;

};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef VBFCompositePtrCandidateT1T2MEtEventT3<reco::Candidate, reco::Candidate, reco::Jet> DiCandidatePairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Electron, pat::Tau, pat::Jet> PATElecTauPairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Muon, pat::Tau, pat::Jet> PATMuTauPairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Tau, pat::Tau, pat::Jet> PATDiTauPairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Electron, pat::Muon, pat::Jet> PATElecMuPairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Electron, pat::Electron, pat::Jet> PATDiElecPairVBFEvent;
typedef VBFCompositePtrCandidateT1T2MEtEventT3<pat::Muon, pat::Muon, pat::Jet> PATDiMuPairVBFEvent;

#endif
