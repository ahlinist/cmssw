#ifndef __AnalysisDataFormats_TauAnalysis_GenPhaseSpaceEventInfo_h__
#define __AnalysisDataFormats_TauAnalysis_GenPhaseSpaceEventInfo_h__

/** \class GenPhaseSpaceEventInfo
 *
 * Event level information about phase-space simulated in Monte Carlo
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: GenPhaseSpaceEventInfo.h,v 1.1 2009/01/23 14:58:12 veelken Exp $
 *
 */

#include "DataFormats/Candidate/interface/Particle.h"

class GenPhaseSpaceEventInfo
{
 public:
  /// constructor 
   explicit GenPhaseSpaceEventInfo() {}
   ~GenPhaseSpaceEventInfo() {}
  
  double ptHat() const { return ptHat_; }

  const reco::Particle::LorentzVector& leadingGenElectron() const { return leadingGenElectron_; }
  const reco::Particle::LorentzVector& leadingGenMuon() const { return leadingGenMuon_; }
  const reco::Particle::LorentzVector& leadingGenTauLepton() const { return leadingGenTauLepton_; }
 
 private:

  /// allow only GenPhaseSpaceEventInfoProducer to change values of data-members
  friend class GenPhaseSpaceEventInfoProducer;
 
  /// get generated Pt(hat) value
  /// (NOTE: to be used only in case of QCD samples generated with Pythia !!)
  double ptHat_;

  /// highest Pt generator level electrons, muons and tau leptons
  reco::Particle::LorentzVector leadingGenElectron_;
  reco::Particle::LorentzVector leadingGenMuon_;
  reco::Particle::LorentzVector leadingGenTauLepton_;
};

#endif
