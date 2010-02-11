#ifndef TauAnalysis_Skimming_EwkElecMuValHistManager_h
#define TauAnalysis_Skimming_EwkElecMuValHistManager_h

/** \class EwkElecMuValHistManager
 *
 * Booking and filling of histograms for data-quality monitoring purposes
 * in Z --> tau+ tau- --> electron + muon channel
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EwkElecMuValHistManager.h,v 1.1 2010/02/10 16:14:20 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Skimming/interface/EwkValHistManagerBase.h"

#include <string>

class EwkElecMuValHistManager : public EwkValHistManagerBase
{
 public:
  EwkElecMuValHistManager(const edm::ParameterSet&);

  void bookHistograms();
  void fillHistograms(const edm::Event&, const edm::EventSetup&);
  void finalizeHistograms();

 private:

//--- labels of muon, tau-jet and MEt collections being used
//    in event selection and when filling histograms
  edm::InputTag triggerResultsSource_;
  edm::InputTag vertexSource_;
  edm::InputTag beamSpotSource_;
  edm::InputTag electronSource_;
  edm::InputTag muonSource_;
  edm::InputTag caloMEtSource_;
  edm::InputTag pfMEtSource_;

//--- event selection criteria
  typedef std::vector<std::string> vstring;
  vstring hltPaths_;

  double electronEtaCut_;
  double electronPtCut_;
  double electronTrackIsoCut_;
  double electronEcalIsoCut_;
  int electronIsoMode_;

  double muonEtaCut_;
  double muonPtCut_;
  double muonTrackIsoCut_;
  double muonEcalIsoCut_;
  int muonIsoMode_;

  double visMassCut_;

//--- histograms  
  MonitorElement* hNumIdElectrons_;
  MonitorElement* hElectronPt_;
  MonitorElement* hElectronEta_;
  MonitorElement* hElectronPhi_;
  MonitorElement* hElectronTrackIsoPt_;
  MonitorElement* hElectronEcalIsoPt_;

  MonitorElement* hNumGlobalMuons_;
  MonitorElement* hMuonPt_;
  MonitorElement* hMuonEta_;
  MonitorElement* hMuonPhi_;
  MonitorElement* hMuonTrackIsoPt_;
  MonitorElement* hMuonEcalIsoPt_;
  
  MonitorElement* hVisMass_;
  MonitorElement* hMtElecCaloMEt_;
  MonitorElement* hMtElecPFMEt_;
  MonitorElement* hMtMuCaloMEt_;
  MonitorElement* hMtMuPFMEt_;
  MonitorElement* hPzetaCaloMEt_;
  MonitorElement* hPzetaPFMEt_;
  MonitorElement* hElecMuAcoplanarity_;
  MonitorElement* hElecMuCharge_;

  MonitorElement* hVertexChi2_;
  MonitorElement* hVertexZ_;
  MonitorElement* hVertexD0_;

  MonitorElement* hCaloMEtPt_;
  MonitorElement* hCaloMEtPhi_;

  MonitorElement* hPFMEtPt_;
  MonitorElement* hPFMEtPhi_;

  MonitorElement* hCutFlowSummary_;
  enum { kPassedPreselection = 1, kPassedTrigger = 2, kPassedMuonId = 3, kPassedMuonTrackIso = 4, kPassedMuonEcalIso = 5,
	 kPassedElectronId = 6, kPassedElectronTrackIso = 7, kPassedElectronEcalIso = 8 };
  
//--- counters for different types of warnings
  long numWarningsTriggerResults_;
  long numWarningsHLTpath_;
  long numWarningsVertex_;
  long numWarningsBeamSpot_;
  long numWarningsElectron_;
  long numWarningsMuon_;
  long numWarningsCaloMEt_;
  long numWarningsPFMEt_;
};

#endif

