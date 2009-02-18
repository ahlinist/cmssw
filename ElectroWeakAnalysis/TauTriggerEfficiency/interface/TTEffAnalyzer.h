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
// $Id: TTEffAnalyzer.h,v 1.14 2009/02/09 01:54:13 smaruyam Exp $
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
//#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L2TauEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L25TauEfficiencyAnalyzer.h"

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
          fill(*particle);

          // Call individual analyzers
          _L1analyzer.fill(iEvent, *particle);
          _L2analyzer.fill(iEvent, *particle);
          _L25analyzer.fill(iEvent, *particle);

          // Finally, fill the entry to tree
          _TTEffTree->Fill();
        }
      }

      virtual void fill(const reco::PFTau&); 
      virtual void fill(const reco::CaloTau&);
      virtual void fill(const LorentzVector&);

      //Helper function :RMS of the PF Candidates
      std::vector<double> clusterSeparation(const reco::PFCandidateRefVector& ,const reco::PFCandidateRefVector& );

//      virtual void loop2(const Event& iEvent, Handle<PFTauCollection> taus, PFTauDiscriminator isos);
      virtual void loop2(const Event& iEvent, Handle<PFTauCollection> taus, PFTauDiscriminatorByIsolation isos);

      // ----------member data ---------------------------
      edm::InputTag  PFTaus_,PFTauIso_; //Path to analyze
      std::string rootFile_;

      // PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
        float PFPt, PFEt,PFEta,PFPhi,PFProng,PFIso,PFIsoSum,PFEnergy;
      L1TauEfficiencyAnalyzer _L1analyzer;
      L2TauEfficiencyAnalyzer _L2analyzer;
      L25TauEfficiencyAnalyzer _L25analyzer;

};

