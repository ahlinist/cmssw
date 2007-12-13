#ifndef ZETauRecoTauTagHcalEtFilter_h
#define ZETauRecoTauTagHcalEtFilter_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include <vector>
#include <string>

#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoPropagateTrackToCalo.h"

class ZETauRecoTauTagHcalEtFilter : public edm::EDFilter {

public:
  explicit ZETauRecoTauTagHcalEtFilter(const edm::ParameterSet&);
  ~ZETauRecoTauTagHcalEtFilter();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  

private:

  
  
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag m_TauTag;
  double m_HtotEt,m_OutTrHhotDEta;
  double m_PtLtr;
  int m_MinN;
 

};

#endif
