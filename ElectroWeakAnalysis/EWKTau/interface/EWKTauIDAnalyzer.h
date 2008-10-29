// Package:    EWKTauIDAnalyzer
// Class:      EWKTauIDAnalyzer
// Original Author:  Alfredo Gurrola
//         Created:  Oct 29 05:23:58 CET 2008

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <iostream>
#include <limits>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/View.h"
#include "TFile.h"
#include "TH1.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>

class EWKTauIDAnalyzer : public edm::EDAnalyzer {
   public:
      explicit EWKTauIDAnalyzer(const edm::ParameterSet&);
      ~EWKTauIDAnalyzer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

      edm::InputTag muonPassingEtaLabel_;
      edm::InputTag muonPassingPtLabel_;
      edm::InputTag muonPassingSeperationLabel_;
      edm::InputTag tauPassingEtaLabel_;
      edm::InputTag tauPassingEtLabel_;
      edm::InputTag tauPassingVetoLabel_;

      int EventsWithMuonsPassingEtaCut;
      int EventsWithMuonsPassingPtCut;
      int EventsWithMuonsPassingSeperationCut;
      int EventsWithTausPassingEtaCut;
      int EventsWithTausPassingEtCut;
      int EventsWithTausPassingSingleJetCut;

      TFile *m_file;

      TH1F *m_Events;
      TH1F *hSelectedEtaMuonsPt;
      TH1F *hSelectedEtaMuonsEta;
      TH1F *hSelectedPtMuonsPt;
      TH1F *hSelectedPtMuonsEta;
      TH1F *hSelectedEtaTausEt;
      TH1F *hSelectedEtaTausEta;
      TH1F *hSelectedEtTausEt;
      TH1F *hSelectedEtTausEta;
      TH1F *hSelectedVetoTausEt;
      TH1F *hSelectedVetoTausEta;
      TH1F *hSelectedSeperationMuonsPt;
      TH1F *hSelectedSeperationMuonsEta;
};

