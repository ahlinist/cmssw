#ifndef DIJET_BALANCE_TREE_PRODUCER_H
#define DIJET_BALANCE_TREE_PRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

namespace cms
{
  template<class Jet>
  class DijetBalanceTreeProducer : public edm::EDAnalyzer 
  {
    public:
      typedef std::vector<Jet> JetCollection;
      explicit DijetBalanceTreeProducer(edm::ParameterSet const& cfg);
      virtual void analyze(edm::Event const& event, edm::EventSetup const& iSetup);
      virtual void beginJob(); 
      virtual void endJob();
      virtual void beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup);
      void FindProbeJet(double eta1, double eta2, int &probe, int &barrel);
      ~DijetBalanceTreeProducer();
    private:
      typedef struct 
        {
          int prescale;
          int fired;
        } TrigStruct;

      TrigStruct HLTTrigResults_[100];
      std::vector<std::string> triggerNames_;
      std::string triggerProcessName_;
      std::vector<unsigned int> triggerIndex_;
      edm::InputTag triggerResultsTag_;
      HLTConfigProvider hltConfig_;
      std::string histogramFile_;
      std::string jets_;
      std::string jetType_;
      std::string jetsID_;
      edm::Service<TFileService> fs;
      TTree* dijetTree_;
      TH1F* hPtHat_;
      bool isMCarlo_;
      double barrelEtaCut_;
      double dijetPtCut_;
      float dphi_,ptBarrel_,ptProbe_,ptJet3_,etaBarrel_,etaProbe_,etaJet3_,dijetPt_;
      float pthat_,weight_,xsec_;
      int passLooseIdBarrel_,passLooseIdProbe_,passLooseIdJet3_; 
      int runNo_, evtNo_, lumi_, bunch_;
  };
}
#include "JetMETCorrections/DijetBalance/src/DijetBalanceTreeProducer.icc"
#endif
