//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_Tau_h
#define ValidationPhysicsToolsPATValidation_Tau_h


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//
// class declaration
//

class PATValidation_Tau : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Tau(const edm::ParameterSet&);
      ~PATValidation_MET();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoMET_;
edm::InputTag patMET_;

  // What's the reference for the Validation Leptons or Jets
  std::string dataType_;

 // Matching criteria
  double matchDeltaR_Leptons_;
  double matchDeltaR_Jets_;

  // output histograms
  bool saveoutputhistograms_;

 // Reference Collection
  edm::InputTag refCollection_;

  // In case you need to distinguish the output file
  std::string extensionName_;

  // Reconstructed product of interest
  std::string TauProducer_;

  // std::vector<std::string> TauProducerDiscriminators_; 
  // std::vector<double> TauDiscriminatorCuts_;

  std::vector< edm::ParameterSet > discriminators_;

  // CMSSW version

  std::string tversion;
  std::string outPutFile_;

  std::map<std::string,  MonitorElement *> ptTauVisibleMap;
  std::map<std::string,  MonitorElement *> etaTauVisibleMap;
  std::map<std::string,  MonitorElement *> phiTauVisibleMap;
  std::map<std::string,  MonitorElement *> energyTauVisibleMap;
  
  // All the extra MonitorElements that we would like to add for each Tau Tagging step
  // First for the PFTaus
  // Number of PFTau Candidates with a leading charged hadron in it (within a cone of 0.1 avound the jet axis and a minimum pt of 6 GeV)
  
  MonitorElement* nPFJet_LeadingChargedHadron_ChargedHadronsSignal_;	  
  MonitorElement* nPFJet_LeadingChargedHadron_ChargedHadronsIsolAnnulus_; 
  MonitorElement* nPFJet_LeadingChargedHadron_GammasSignal_;		  
  MonitorElement* nPFJet_LeadingChargedHadron_GammasIsolAnnulus_;
  MonitorElement* nPFJet_LeadingChargedHadron_NeutralHadronsSignal_;	 
  MonitorElement* nPFJet_LeadingChargedHadron_NeutralHadronsIsolAnnulus_;

  // Isolated PFTau with a Leading charged hadron with no Charged Hadrons inside the isolation annulus

  MonitorElement* nIsolated_NoChargedHadrons_ChargedHadronsSignal_;	
  MonitorElement* nIsolated_NoChargedHadrons_GammasSignal_;		  
  MonitorElement* nIsolated_NoChargedHadrons_GammasIsolAnnulus_;         
  MonitorElement* nIsolated_NoChargedHadrons_NeutralHadronsSignal_;	
  MonitorElement* nIsolated_NoChargedHadrons_NeutralHadronsIsolAnnulus_;

  // Isolated PFTau with a Leading charge hadron with no Charged Hadron inside the isolation annulus with no Ecal/Gamma candidates in the isolation annulus

  MonitorElement* nIsolated_NoChargedNoGammas_ChargedHadronsSignal_;    
  MonitorElement* nIsolated_NoChargedNoGammas_GammasSignal_;         
  MonitorElement* nIsolated_NoChargedNoGammas_NeutralHadronsSignal_;	 
  MonitorElement* nIsolated_NoChargedNoGammas_NeutralHadronsIsolAnnulus_;


  // Second for the CaloTaus
  // Number of CaloJets with a Leading Track (within a cone of 0.1 around the jet axis and a minimum pt of 5. GeV)

  MonitorElement* nCaloJet_LeadingTrack_signalTracksInvariantMass_;	  
  MonitorElement* nCaloJet_LeadingTrack_signalTracks_;
  MonitorElement* nCaloJet_LeadingTrack_isolationTracks_;
  MonitorElement* nCaloJet_LeadingTrack_isolationECALhitsEtSum_;          

 // Track Isolated CaloTau with a Leading Track

  MonitorElement* nTrackIsolated_isolationECALhitsEtSum_; 
  MonitorElement* nTrackIsolated_signalTracksInvariantMass_;
  MonitorElement* nTrackIsolated_signalTracks_;             	          

 // EM Isolated CaloTau with a Leading with no tracks in the Isolation Annulus

  MonitorElement* nEMIsolated_signalTracksInvariantMass_;    
  MonitorElement* nEMIsolated_signalTracks_;              
 
  // book-keeping variables

  DQMStore* dbeTau;

  int numEvents_;

 protected:

  PFBenchmarkAlgo *algo_;

};

#endif
