#ifndef TauAnalysis_CandidateTools_CompositePtrCandidateTMEtProducer_h
#define TauAnalysis_CandidateTools_CompositePtrCandidateTMEtProducer_h

/** \class CompositePtrCandidateTMEtProducer
 *
 * Produce combinations of visible tau decay products and missing transverse momentum 
 * (representing the undetected momentum carried away 
 *  the neutrino produced in a W --> tau nu decay and the neutrinos produced in the tau decay)
 * 
 * \authors Christian Veelken
 *
 * \version $Revision: 1.3 $
 *
 * $Id: CompositePtrCandidateTMEtProducer.h,v 1.3 2010/09/28 11:23:28 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"

#include "TauAnalysis/CandidateTools/interface/CompositePtrCandidateTMEtAlgorithm.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/METReco/interface/MET.h"

#include <string>

template<typename T>
class CompositePtrCandidateTMEtProducer : public edm::EDProducer 
{
  typedef edm::Ptr<T> TPtr;
  typedef edm::Ptr<reco::MET> MEtPtr;

  typedef std::vector<CompositePtrCandidateTMEt<T> > CompositePtrCandidateCollection;
  
 public:

  explicit CompositePtrCandidateTMEtProducer(const edm::ParameterSet& cfg)
    : algorithm_(cfg)
  {
    srcVisDecayProducts_ = cfg.getParameter<edm::InputTag>("srcVisDecayProducts");
    srcMET_ = cfg.getParameter<edm::InputTag>("srcMET");
    srcPV_ = ( cfg.exists("srcPrimaryVertex") ) ? cfg.getParameter<edm::InputTag>("srcPrimaryVertex") : edm::InputTag();
    srcBeamSpot_ = ( cfg.exists("srcBeamSpot") ) ? cfg.getParameter<edm::InputTag>("srcBeamSpot") : edm::InputTag();
    verbosity_ = cfg.getUntrackedParameter<int>("verbosity", 0);

    if ( srcMET_.label() != "" && srcBeamSpot_.label() != "" && srcPV_.label() != "" ) {
      doSVreco_ = ( cfg.exists("doSVreco") ) ? cfg.getParameter<bool>("doSVreco") : true;
    } 

    //std::cout << " doSVreco = " << doSVreco_ << std::endl;

    produces<CompositePtrCandidateCollection>("");
  }

  ~CompositePtrCandidateTMEtProducer() {}

  void beginJob()
  {
    algorithm_.beginJob();
  }

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    typedef edm::View<T> TView;
    edm::Handle<TView> visDecayProductsCollection;
    pf::fetchCollection(visDecayProductsCollection, srcVisDecayProducts_, evt);
  
    typedef edm::View<reco::MET> MEtView;
    edm::Handle<MEtView> metCollection;
    pf::fetchCollection(metCollection, srcMET_, evt);

    // Get primary vertex
    const reco::Vertex* pv = NULL;
    if ( srcPV_.label() != "" ) {
       edm::Handle<reco::VertexCollection> pvs;
       pf::fetchCollection(pvs, srcPV_, evt);
       pv = &((*pvs)[0]);
    }

    // Get beamspot
    const reco::BeamSpot* beamSpot = NULL;
    if ( srcBeamSpot_.label() != "" ) {
       edm::Handle<reco::BeamSpot> beamSpotHandle;
       pf::fetchCollection(beamSpotHandle, srcBeamSpot_, evt);
       beamSpot = beamSpotHandle.product();
    }

    const TransientTrackBuilder* trackBuilder = NULL;
    if ( doSVreco_ ) {
       edm::ESHandle<TransientTrackBuilder> myTransientTrackBuilder;
       es.get<TransientTrackRecord>().get("TransientTrackBuilder", myTransientTrackBuilder);
       trackBuilder = myTransientTrackBuilder.product();
       if ( !trackBuilder ) {
	 edm::LogError ("produce") << " Failed to access TransientTrackBuilder !!";
       }
    }

//--- check that there is exactly one MET object in the event
//    (missing transverse momentum is an **event level** quantity)
    MEtPtr metPtr;
    if ( metCollection->size() == 1 ) {
      metPtr = metCollection->ptrAt(0);
    } else {
      edm::LogError ("produce") 
	<< " Found " << metCollection->size() << " MET objects in collection = " << srcMET_ << ","
	<< " --> CompositePtrCandidateTMEt collection will NOT be produced !!";
      std::auto_ptr<CompositePtrCandidateCollection> emptyCompositePtrCandidateCollection(new CompositePtrCandidateCollection());
      evt.put(emptyCompositePtrCandidateCollection);
      return;
    }

//--- pass edm::Event and edm::EventSetup to SVfit algorithm
//    (needed by likelihood plugins for initialization of TransientTrackBuilder 
//     and to retrieve BeamSpot and genParticle collection from the event)
    algorithm_.beginEvent(evt, es);

    std::auto_ptr<CompositePtrCandidateCollection> compositePtrCandidateCollection(new CompositePtrCandidateCollection());

    for ( unsigned idxVisDecayProducts = 0, numVisDecayProducts = visDecayProductsCollection->size(); 
	  idxVisDecayProducts < numVisDecayProducts; ++idxVisDecayProducts ) {
      TPtr visDecayProductsPtr = visDecayProductsCollection->ptrAt(idxVisDecayProducts);
      
      CompositePtrCandidateTMEt<T> compositePtrCandidate = 
	algorithm_.buildCompositePtrCandidate(visDecayProductsPtr, metPtr, pv, beamSpot, trackBuilder, doSVreco_);
      compositePtrCandidateCollection->push_back(compositePtrCandidate);
    }

//--- add the collection of reconstructed CompositePtrCandidateTMEts to the event
    evt.put(compositePtrCandidateCollection);
  }

 private:

  CompositePtrCandidateTMEtAlgorithm<T> algorithm_;
  
  edm::InputTag srcVisDecayProducts_;
  edm::InputTag srcMET_;
  edm::InputTag srcPV_;
  edm::InputTag srcBeamSpot_;
  bool doSVreco_;
  int verbosity_;
};

#endif

