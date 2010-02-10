#ifndef TauAnalysis_Skimming_EwkElecTauValHistManager_h
#define TauAnalysis_Skimming_EwkElecTauValHistManager_h

/** \class EwkElecTauValHistManager
 *
 * Booking and filling of histograms for data-quality monitoring purposes
 * in Z --> electron + tau-jet channel
 * 
 * \author Joshua Swanson
 *        (modified by Christian Veelken)
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EwkElecTauValHistManager.h,v 1.1 2009/12/01 17:03:36 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>

class EwkElecTauValHistManager
{
 public:
  EwkElecTauValHistManager(const edm::ParameterSet&, DQMStore*);

  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);
  void finalizeHistograms(); 

 private:

//--- labels of electron, tau-jet and MEt collections being used
//    in event selection and when filling histograms
  edm::InputTag triggerResultsSource_;
  edm::InputTag vertexSource_;
  edm::InputTag beamSpotSource_;
  edm::InputTag electronSource_;
  edm::InputTag tauJetSource_;
  edm::InputTag caloMEtSource_;
  edm::InputTag pfMEtSource_;

  edm::InputTag tauDiscrByLeadTrackFinding_;
  edm::InputTag tauDiscrByLeadTrackPtCut_;
  edm::InputTag tauDiscrByTrackIso_;
  edm::InputTag tauDiscrByEcalIso_;
  edm::InputTag tauDiscrAgainstElectrons_;
  edm::InputTag tauDiscrAgainstMuons_;

//--- event selection criteria
  typedef std::vector<std::string> vstring;
  vstring hltPaths_;

  double electronEtaCut_;
  double electronPtCut_;
  double electronTrackIsoCut_;
  double electronEcalIsoCut_;
  int electronIsoMode_;

  double tauJetEtaCut_;
  double tauJetPtCut_;

  double visMassCut_;

//--- pointer to DQM histogram management service
  DQMStore* dqmStore_;

//--- name of DQM directory in which histograms for Z --> electron + tau-jet channel get stored
  std::string dqmDirectory_;
  
//--- histograms  
  MonitorElement* hNumIdElectrons_;
  MonitorElement* hElectronPt_;
  MonitorElement* hElectronEta_;
  MonitorElement* hElectronPhi_;
  MonitorElement* hElectronTrackIsoPt_;
  MonitorElement* hElectronEcalIsoPt_;

  MonitorElement* hTauJetPt_;
  MonitorElement* hTauJetEta_;
  MonitorElement* hTauJetPhi_;
  MonitorElement* hTauLeadTrackPt_;
  MonitorElement* hTauTrackIsoPt_;
  MonitorElement* hTauEcalIsoPt_;
  MonitorElement* hTauDiscrAgainstElectrons_;
  MonitorElement* hTauDiscrAgainstMuons_;
  MonitorElement* hTauJetCharge_;
  MonitorElement* hTauJetNumSignalTracks_;
  MonitorElement* hTauJetNumIsoTracks_;
  
  MonitorElement* hVisMass_;
  MonitorElement* hMtElecCaloMEt_;
  MonitorElement* hMtElecPFMEt_;
  MonitorElement* hPzetaCaloMEt_;
  MonitorElement* hPzetaPFMEt_;
  MonitorElement* hElecTauAcoplanarity_;
  MonitorElement* hElecTauCharge_;

  MonitorElement* hVertexChi2_;
  MonitorElement* hVertexZ_;
  MonitorElement* hVertexD0_;

  MonitorElement* hCaloMEtPt_;
  MonitorElement* hCaloMEtPhi_;

  MonitorElement* hPFMEtPt_;
  MonitorElement* hPFMEtPhi_;

  MonitorElement* hCutFlowSummary_;
  enum { kPassedPreselection = 1, kPassedTrigger = 2, kPassedElectronId = 3, kPassedElectronTrackIso = 4, kPassedElectronEcalIso = 5,
         kPassedTauLeadTrack = 6, kPassedTauLeadTrackPt = 7, kPassedTauTrackIso = 8, kPassedTauEcalIso = 9, 
         kPassedTauDiscrAgainstElectrons = 10, kPassedTauDiscrAgainstMuons = 11 }; 

//--- counters for filter-statistics output
  unsigned numEventsAnalyzed_;
  unsigned numEventsSelected_;

  int cfgError_;

  int maxNumWarnings_;

  long numWarningsTriggerResults_;
  long numWarningsHLTpath_;
  long numWarningsVertex_;
  long numWarningsBeamSpot_;
  long numWarningsElectron_;
  long numWarningsTauJet_;
  long numWarningsTauDiscrByLeadTrackFinding_;
  long numWarningsTauDiscrByLeadTrackPtCut_;
  long numWarningsTauDiscrByTrackIso_;
  long numWarningsTauDiscrByEcalIso_;
  long numWarningsTauDiscrAgainstElectrons_;
  long numWarningsTauDiscrAgainstMuons_;
  long numWarningsCaloMEt_;
  long numWarningsPFMEt_;
};

#endif

