#ifndef RecoTauTag_PFConeIsolation
#define RecoTauTag_PFConeIsolation

/* class PFConeIsolation
 * EDProducer of the tagged TauJet with the PFConeIsolationAlgorithm, 
 * returns an PFIsolatedTauTagInfo collection,
 * methods implemented in the PFIsolatedTauTagInfo class are used to compute the discriminator variable.
 * created: Apr 21 2007,
 * revised: ,
 * authors: Simone Gennai, Ludovic Houchu
 */

#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include "RecoTauTag/PFConeIsolation/interface/PFConeIsolationAlgorithm.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CLHEP/Random/RandGauss.h"

#include <memory>

using namespace reco;
using namespace edm;
using namespace std;

class PFConeIsolation : public EDProducer {
 public:
  explicit PFConeIsolation(const ParameterSet& iConfig){
    PFJetmodule_        = iConfig.getParameter<string>("PFJetmodule");
    PFJetConeSize_      = iConfig.getParameter<double>("PFJetConeSize");
    PFCandidatemodule_  = iConfig.getParameter<string>("PFCandidatemodule");
    PVmodule_           = iConfig.getParameter<string>("PVmodule");
    smearedPVsigmaX_    = iConfig.getParameter<double>("smearedPVsigmaX");
    smearedPVsigmaY_    = iConfig.getParameter<double>("smearedPVsigmaY");
    smearedPVsigmaZ_    = iConfig.getParameter<double>("smearedPVsigmaZ");	
    PFConeIsolationAlgo_=new PFConeIsolationAlgorithm(iConfig);
    produces<JetTagCollection>(); 
    produces<PFIsolatedTauTagInfoCollection>();      
  }
  ~PFConeIsolation(){
    delete PFConeIsolationAlgo_;
  }
  virtual void produce(Event&,const EventSetup&);
 private:
  PFConeIsolationAlgorithm* PFConeIsolationAlgo_;
  string PFJetmodule_;
  double PFJetConeSize_;
  string PFCandidatemodule_;
  string PVmodule_;
  double smearedPVsigmaX_;
  double smearedPVsigmaY_;
  double smearedPVsigmaZ_;  
};
#endif

