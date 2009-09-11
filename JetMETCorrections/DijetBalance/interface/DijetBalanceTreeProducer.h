#ifndef DIJETBALANCETREEPRODUCER_H
#define DIJETBALANCETREEPRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TH1.h"
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
namespace cms
{
  template<class Jet>
  class DijetBalanceTreeProducer : public edm::EDAnalyzer 
  {
    public:
      typedef std::vector<Jet> JetCollection;
      explicit DijetBalanceTreeProducer(edm::ParameterSet const& cfg);
      virtual void analyze(edm::Event const& event, edm::EventSetup const& iSetup);
      virtual void beginJob(const edm::EventSetup &iSetup); 
      virtual void endJob();
      virtual void beginRun(edm::Run const& run);
      void FindProbeJet(double eta1, double eta2, int &probe, int &barrel);
      DijetBalanceTreeProducer();
    private:
      std::vector<std::string> triggerNames_;
      std::string triggerProcessName_;
      std::vector<unsigned int> triggerIndex_;
      edm::InputTag triggerResultsTag_;
      HLTConfigProvider hltConfig_;
      std::string histogramFile_;
      std::string jets_;
      TFile* m_file_;
      TTree* dijetTree_;
      TH1F* hPtHat_;
      bool isMCarlo_;
      double barrelEtaCut_;
      double dijetPtCut_;
      float dphi_,ptBarrel_,ptProbe_,ptJet3_,etaProbe_,pthat_,xsec_;
      int HLTBits_[6];
  };
}
#include "../src/DijetBalanceTreeProducer.icc"
#endif
