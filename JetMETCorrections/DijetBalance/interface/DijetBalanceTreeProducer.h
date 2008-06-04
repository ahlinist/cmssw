#ifndef DIJETBALANCETREEPRODUCER_H
#define DIJETBALANCETREEPRODUCER_H

#include "TTree.h"
#include "TFile.h"
#include "TNamed.h"
#include <vector>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"

class DijetBalanceTreeProducer : public edm::EDAnalyzer 
{
  public:

    explicit DijetBalanceTreeProducer(edm::ParameterSet const& cfg);
    virtual void analyze(edm::Event const& e, edm::EventSetup const& iSetup);
    virtual void endJob();
    DijetBalanceTreeProducer();

  private:
    std::string histogramFile_;
    std::string jets_;
    bool isGenJets_; 
    TFile* m_file_;
    TTree* dijetTree_;
    double barrelEtaCut_;
    float dphi_,dphi_0_2pi_,ratioPtJet3_,ptBarrel_,ptProbe_,ptJet3_,etaProbe_,dijetPt_,balance_;
};

#endif
