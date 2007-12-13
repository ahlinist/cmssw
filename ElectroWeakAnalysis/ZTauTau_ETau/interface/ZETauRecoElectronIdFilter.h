#ifndef ZETauRecoElectronIdFilter_h
#define ZETauRecoElectronIdFilter_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"

#include "DataFormats/Math/interface/deltaR.h"
#include <vector>
#include <string>

class ZETauRecoElectronIdFilter : public edm::EDFilter {

public:
  explicit ZETauRecoElectronIdFilter(const edm::ParameterSet&);
  ~ZETauRecoElectronIdFilter();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag m_LeptonTag,m_LeptonIdTag,m_HLTTag;
  int m_MinN;bool m_doMatch;
 

};

#endif
