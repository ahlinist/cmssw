#ifndef ZETauRecoLeptonEtFilter_h
#define ZETauRecoLeptonEtFilter_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include <vector>
#include <string>

class ZETauRecoLeptonEtFilter : public edm::EDFilter {

public:
  explicit ZETauRecoLeptonEtFilter(const edm::ParameterSet&);
  ~ZETauRecoLeptonEtFilter();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag m_LeptonTag;
  double m_MinEt;
  int m_MinN;
 

};

#endif
