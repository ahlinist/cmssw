#ifndef ZETauRecoTauTagProngFilter_h
#define ZETauRecoTauTagProngFilter_h

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

class ZETauRecoTauTagProngFilter : public edm::EDFilter {

public:
  explicit ZETauRecoTauTagProngFilter(const edm::ParameterSet&);
  ~ZETauRecoTauTagProngFilter();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag m_TauTag;
  edm::InputTag m_PFTauTag; 
  double m_Rm, m_Rsig, m_PtLtr, m_PtTr;
  bool m_RunOnPFTau;
  int m_MinN; 

};

#endif
