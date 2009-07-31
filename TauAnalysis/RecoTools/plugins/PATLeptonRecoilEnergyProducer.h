#ifndef TauAnalysis_RecoTools_PATLeptonRecoilEnergyProducer_h
#define TauAnalysis_RecoTools_PATLeptonRecoilEnergyProducer_h

/** \class PATLeptonRecoilEnergyProducer
 *
 * Produce information about objects opposite in transverse plane to pat::Lepton
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATLeptonRecoilEnergyProducer.h,v 1.1 2009/07/29 13:03:38 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

#include "AnalysisDataFormats/TauAnalysis/interface/PATLeptonRecoilEnergy.h"

#include "TauAnalysis/RecoTools/interface/PATLeptonRecoilEnergyAlgorithm.h"

#include <string>

template<typename T1, typename T2>
class PATLeptonRecoilEnergyProducer : public edm::EDProducer 
{
  typedef std::vector<PATLeptonRecoilEnergy<T1,T2> > PATLeptonRecoilEnergyCollection;

 public:

  explicit PATLeptonRecoilEnergyProducer(const edm::ParameterSet& cfg)
    : algorithm_(cfg)
  {
    srcLeptons_ = cfg.getParameter<edm::InputTag>("srcLeptons");
    srcEnergyObjects_ = cfg.getParameter<edm::InputTag>("srcEnergyObjects");
    
    produces<PATLeptonRecoilEnergyCollection>("");
  }

  ~PATLeptonRecoilEnergyProducer() {}

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    typedef edm::View<T1> T1View;
    edm::Handle<T1View> leptonCollection;
    pf::fetchCollection(leptonCollection, srcLeptons_, evt);
    typedef edm::View<T2> T2View;
    edm::Handle<T2View> energyObjectCollection;
    pf::fetchCollection(energyObjectCollection, srcEnergyObjects_, evt);
  
    std::auto_ptr<PATLeptonRecoilEnergyCollection> leptonRecoilEnergyCollection(new PATLeptonRecoilEnergyCollection());

    for ( unsigned idxLepton = 0, numLeptons = leptonCollection->size(); 
	  idxLepton < numLeptons; ++idxLepton ) {
      typedef edm::Ptr<T1> T1Ptr;
      T1Ptr leptonPtr = leptonCollection->ptrAt(idxLepton);

      PATLeptonRecoilEnergy<T1,T2> leptonRecoilEnergy = algorithm_.buildRecoilEnergy(leptonPtr, *energyObjectCollection);

      leptonRecoilEnergyCollection->push_back(leptonRecoilEnergy);
    }

    evt.put(leptonRecoilEnergyCollection);
  }

 private:

  PATLeptonRecoilEnergyAlgorithm<T1,T2> algorithm_;
  
  edm::InputTag srcLeptons_;
  edm::InputTag srcEnergyObjects_;
};

#endif

