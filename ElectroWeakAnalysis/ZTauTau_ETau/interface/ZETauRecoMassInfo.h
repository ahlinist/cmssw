#ifndef ZETauRecoMassInfo_h
#define ZETauRecoMassInfo_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
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

class ZETauRecoMassInfo : public edm::EDProducer {

public:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;

  explicit ZETauRecoMassInfo(const edm::ParameterSet&);
  ~ZETauRecoMassInfo();

  virtual void produce(edm::Event&, const edm::EventSetup&);
  void HighestEtObject(Handle<LorentzVectorCollection>&,LorentzVector&);  
  void CalculateMass(LorentzVectorCollection&, vector<float>&);
  double DeltaPhi(LorentzVector&,LorentzVector&);

 private:
  
  edm::InputTag m_TauTag;
  edm::InputTag m_LeptonTag;
  edm::InputTag m_CaloMETTag;
  edm::InputTag m_CaloJetTag;

};

#endif
