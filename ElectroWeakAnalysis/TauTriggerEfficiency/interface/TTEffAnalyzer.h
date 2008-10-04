// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      TTEffAnalyzer
// 
/**\class TTEffAnalyzer TTEffAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/TTEffAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Wed Oct  1 13:04:54 CEST 2008
// $Id: TTEffAnalyzer.h,v 1.2 2008/10/03 11:48:40 mkortela Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"


//
class TTEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTEffAnalyzer(const edm::ParameterSet&);
      ~TTEffAnalyzer();

      TTree *_TTEffTree;
      TFile *_TTEffFile;


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void fillPFTau(const reco::PFTau&); 

      //Helper function :RMS of the PF Candidates
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );

      // ----------member data ---------------------------
      edm::InputTag  PFTaus_; //Path to analyze
      std::string rootFile_;

      // PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
      float PFPt, PFEt,PFEta,PFPhi,PFEGIsolEt,PFHighestClusterEt,PFEGEtaRMS,PFEGPhiRMS,PFEGDrRMS;
      
      L1TauEfficiencyAnalyzer _L1analyzer;
      L2TauEfficiencyAnalyzer _L2analyzer;




};

