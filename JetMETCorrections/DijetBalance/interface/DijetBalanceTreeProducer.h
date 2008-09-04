#ifndef DIJETBALANCETREEPRODUCER_H
#define DIJETBALANCETREEPRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
namespace cms
{
  template<class Jet>
  class DijetBalanceTreeProducer : public edm::EDAnalyzer 
  {
    public:
      typedef std::vector<Jet> JetCollection;
      explicit DijetBalanceTreeProducer(edm::ParameterSet const& cfg);
      virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
      virtual void endJob();
      void FindProbeJet(double eta1, double eta2, int &probe, int &barrel);
      DijetBalanceTreeProducer();
    private:
      std::string histogramFile_;
      std::string jets_;
      TFile* m_file_;
      TTree* dijetTree_;
      double barrelEtaCut_;
      double dijetPtCut_;
      float dphi_,ptBarrel_,ptProbe_,ptJet3_,etaProbe_;
  };
}
#include "../src/DijetBalanceTreeProducer.icc"
#endif
