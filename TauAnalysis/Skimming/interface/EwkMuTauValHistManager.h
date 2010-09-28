#ifndef TauAnalysis_Skimming_EwkMuTauValHistManager_h
#define TauAnalysis_Skimming_EwkMuTauValHistManager_h

/** \class EwkMuTauValHistManager
 *
 * Booking and filling of histograms for data-quality monitoring purposes
 * in Z --> tau+ tau- --> muon + tau-jet channel
 * 
 * \author Letizia Lusito,
 *         Christian Veelken
 *
 * \version $Revision: 1.2 $
 *
 * $Id: EwkMuTauValHistManager.h,v 1.2 2010/02/11 09:48:03 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Skimming/interface/EwkValHistManagerBase.h"

#include <string>

class EwkMuTauValHistManager : public EwkValHistManagerBase
{
 public:
  EwkMuTauValHistManager(const edm::ParameterSet&);

  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);
  void finalizeHistograms();

 private:

//--- labels of muon, tau-jet and MEt collections being used
//    in event selection and when filling histograms
  edm::InputTag triggerResultsSource_;
  edm::InputTag vertexSource_;
  edm::InputTag beamSpotSource_;
  edm::InputTag muonSource_;
  edm::InputTag tauJetSource_;
  edm::InputTag caloMEtSource_;
  edm::InputTag pfMEtSource_;

  edm::InputTag tauDiscrByLeadTrackFinding_;
  edm::InputTag tauDiscrByLeadTrackPtCut_;
  edm::InputTag tauDiscrByTrackIso_;
  edm::InputTag tauDiscrByEcalIso_;
  edm::InputTag tauDiscrAgainstMuons_;
  
//--- event selection criteria
  typedef std::vector<std::string> vstring;
  vstring hltPaths_;

  double muonEtaCut_;
  double muonPtCut_;
  double muonTrackIsoCut_;
  double muonEcalIsoCut_;
  int muonIsoMode_;

  double tauJetEtaCut_;
  double tauJetPtCut_;

  double visMassCut_;

//--- histograms  
  MonitorElement* hNumGlobalMuons_;
  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPhi_;
  MonitorElement* hMuonTrackIsoPt_;
  MonitorElement* hMuonEcalIsoPt_;

  MonitorElement* hTauJetPt_;
  MonitorElement* hTauJetEta_;
  MonitorElement* hTauJetPhi_;
  MonitorElement* hTauLeadTrackPt_;
  MonitorElement* hTauTrackIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauDiscrAgainstMuons_;
  MonitorElement* hTauJetCharge_;
  MonitorElement* hTauJetNumSignalTracks_;
  MonitorElement* hTauJetNumIsoTracks_;
  
  MonitorElement* hVisMass_;
  MonitorElement* hMtMuCaloMEt_;
  MonitorElement* hMtMuPFMEt_;
  MonitorElement* hPzetaCaloMEt_;
  MonitorElement* hPzetaPFMEt_;
  MonitorElement* hMuTauAcoplanarity_;
  MonitorElement* hMuTauCharge_;

  MonitorElement* hVertexChi2_;
  MonitorElement* hVertexZ_;
  MonitorElement* hVertexD0_;

  MonitorElement* hCaloMEtPt_;
  MonitorElement* hCaloMEtPhi_;

  MonitorElement* hPFMEtPt_;
  MonitorElement* hPFMEtPhi_;

  MonitorElement* hCutFlowSummary_;
  enum { kPassedPreselection = 1, kPassedTrigger = 2, kPassedMuonId = 3, kPassedMuonTrackIso = 4, kPassedMuonEcalIso = 5,
         kPassedTauLeadTrack = 6, kPassedTauLeadTrackPt = 7, kPassedTauTrackIso = 8, kPassedTauEcalIso = 9, 
	 kPassedTauDiscrAgainstMuons = 10 }; 
  
//--- counters for different types of warnings
  long numWarningsTriggerResults_;
  long numWarningsHLTpath_;
  long numWarningsVertex_;
  long numWarningsBeamSpot_;
  long numWarningsMuon_;
  long numWarningsTauJet_;
  long numWarningsTauDiscrByLeadTrackFinding_;
  long numWarningsTauDiscrByLeadTrackPtCut_;
  long numWarningsTauDiscrByTrackIso_;
  long numWarningsTauDiscrByEcalIso_;
  long numWarningsTauDiscrAgainstMuons_;
  long numWarningsCaloMEt_;
  long numWarningsPFMEt_;
};

#endif

