#ifndef ZETauRecoTauMcInfo_h
#define ZETauRecoTauMcInfo_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <vector>
#include <string>

class ZETauRecoTauMcInfo : public edm::EDProducer {

public:
  explicit ZETauRecoTauMcInfo(const edm::ParameterSet&);
  ~ZETauRecoTauMcInfo();

  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag genParticles;
  int m_PDG;

};

#endif
