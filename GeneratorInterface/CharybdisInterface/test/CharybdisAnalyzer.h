#ifndef CharybdisAnalyzer_H
#define CharybdisAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"

class TFile;
class TH1D;

class CharybdisAnalyzer : public edm::EDAnalyzer
{
 public:
  explicit CharybdisAnalyzer(const edm::ParameterSet& );
  virtual ~CharybdisAnalyzer() {} 

  virtual void analyze(const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(const edm::EventSetup& );
  virtual void endJob();

 private:
   TH1D * h_eta1, * h_phi, * h_invMass, * h_pt, * h_st;
   unsigned int cnt_events;
};

#endif


