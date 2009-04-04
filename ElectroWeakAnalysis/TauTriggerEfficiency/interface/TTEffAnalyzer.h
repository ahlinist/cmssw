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
// $Id: TTEffAnalyzer.h,v 1.21 2009/03/25 15:08:19 gfball Exp $
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

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25and3TauEfficiencyAnalyzer.h"

//
class TTEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTEffAnalyzer(const edm::ParameterSet&);
      ~TTEffAnalyzer();

      TTree *_TTEffTree;
      TFile *_TTEffFile;


   private:
      typedef math::XYZTLorentzVector LorentzVector;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      template <class T> void loop(const edm::Event& iEvent, const T& collection) {
        for(typename T::const_iterator particle = collection.begin(); particle != collection.end(); ++particle) {
          // Fill common variables
	  unsigned int i = particle - collection.begin();
          fill(*particle,i);

          // Call individual analyzers
          _L1analyzer.fill(iEvent, *particle);
          _L2analyzer.fill(iEvent, *particle);
          _L25and3analyzer.fill(iEvent, *particle);

          // Finally, fill the entry to tree
          _TTEffTree->Fill();
        }
      }
      
      virtual void fill(const reco::PFTau&,unsigned int i = 0); 
      virtual void fill(const reco::CaloTau&,unsigned int i = 0);
      virtual void fill(const LorentzVector&,unsigned int i = 0);
      virtual void fill(const reco::GsfElectron&,unsigned int i = 0);

      //Helper function :RMS of the PF Candidates
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );

      // ----------member data ---------------------------
      edm::InputTag  PFTaus_,PFTauIso_,MCTaus_; //Path to analyze
      std::string rootFile_;

      edm::Handle<PFTauCollection> PFTaus;
      edm::Handle<PFTauDiscriminator> thePFTauDiscriminatorByIsolation;

      // PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
        float PFPt,PFInvPt,PFEt,PFEta,PFPhi,PFProng,PFIso,PFIsoSum,PFEnergy;
      L1TauEfficiencyAnalyzer _L1analyzer;
      L2TauEfficiencyAnalyzer _L2analyzer;
      L25and3TauEfficiencyAnalyzer _L25and3analyzer;

};

