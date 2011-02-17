// -*- C++ -*-
//
// Package:    JetTrigAnalyzer
// Class:      JetTrigAnalyzer
//
/**\class JetTrigAnalyzer JetTrigAnalyzer.cc JetTrigger/JetTrigAnalyzer/src/JetTrigAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  "Sertac Ozturk"
//         Created:  Mon Jul 27 11:09:40 CDT 2009
// $Id: JetTrigAnalyzer.h,v 1.2 2010/05/31 22:24:14 ferencek Exp $
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"

class TH1F;
class TH2F;
class TNtuple;

//
// class decleration
//

class JetTrigAnalyzer : public edm::EDAnalyzer {
   public:
      explicit JetTrigAnalyzer(const edm::ParameterSet&);
      ~JetTrigAnalyzer();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

   edm::Service<TFileService> fs;

  edm::TriggerNames triggerNames_;
  edm::InputTag triggerTag_, l1CollectionsTag_;
  bool do_print_;
  int evt_cnt;

  TNtuple* ntuple_alljets;
  TNtuple* ntuple_Leadingjets;
  TNtuple* ntuple_L1Jets;

  TNtuple* ntuple_alljets_bit15;
  TNtuple* ntuple_Leadingjets_bit15;
  TNtuple* ntuple_L1Jets_bit15;

  TNtuple* ntuple_alljets_bit16;
  TNtuple* ntuple_Leadingjets_bit16;
  TNtuple* ntuple_L1Jets_bit16;

  TNtuple* ntuple_alljets_bit17;
  TNtuple* ntuple_Leadingjets_bit17;
  TNtuple* ntuple_L1Jets_bit17;

  TNtuple* ntuple_alljets_bit18;
  TNtuple* ntuple_Leadingjets_bit18;
  TNtuple* ntuple_L1Jets_bit18;

  TNtuple* ntuple_alljets_bit55;
  TNtuple* ntuple_Leadingjets_bit55;
  TNtuple* ntuple_L1Jets_bit55;

  TNtuple* ntuple_run_info;

  TH1F* h_caloMet_Met;
  TH1F* h_caloMet_Phi;
  TH1F* h_caloMet_SumEt;
  TH1F* h_caloMet_cut;
  
  TH1F* h_l1_gtbit;
  TH1F* h_Nhltbits;
  TH1F* h_Nl1bits;
  TH1F* h_hltbitOn;
  TH1F* h_l1bitOn;
  TH1F* h_bx;
  TH2F* h_l1bx;
  TH2F* h_hltbx;
  TH1F* h_lb;
  TH2F* h_lbbx;
  TH2F* h_hltlb;
  TH2F* h_l1lb;


};
