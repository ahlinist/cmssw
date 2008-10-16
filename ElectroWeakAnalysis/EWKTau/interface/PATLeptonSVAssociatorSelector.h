// -*- C++ -*-
//
// Package:    PATLeptonSVAssociatorSelector
// Class:      PATLeptonSVAssociatorSelector
// 
/**\class PATLeptonSVAssociatorSelector PATLeptonSVAssociatorSelector.cc MyCode/PATLeptonSVAssociatorSelector/src/PATLeptonSVAssociatorSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonSVAssociatorSelector.h,v 1.1 2008/10/16 10:15:20 gurrola Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/MultiVertexFit/interface/MultiVertexReconstructor.h"
#include "RecoVertex/AdaptiveVertexFinder/interface/AdaptiveVertexReconstructor.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/LinearizedTrackStateFactory.h"
#include "RecoVertex/VertexTools/interface/VertexTrackFactory.h"
#include "RecoVertex/AdaptiveVertexFit/interface/KalmanChiSquare.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexTrackCompatibilityEstimator.h"
#include "RecoVertex/TrimmedKalmanVertexFinder/interface/KalmanTrimmedVertexFinder.h"
#include "RecoVertex/ConfigurableVertexReco/interface/ConfigurableVertexReconstructor.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "Math/GenVector/VectorUtil.h"
#include "FWCore/Framework/interface/ESHandle.h"

class PATLeptonSVAssociatorSelector : public edm::EDProducer {
   public:
      explicit PATLeptonSVAssociatorSelector(const edm::ParameterSet&);
      ~PATLeptonSVAssociatorSelector();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      typedef std::vector<pat::Muon> PATMuonCollection;
      typedef std::vector<pat::Electron> PATElectronCollection;
      edm::InputTag src_;
      std::string LeptonType_;
      std::string TrackProd_;
      std::string VertexProd_;
      edm::ParameterSet vfPSet;
      double VertexMathingDistance_;
      double VertexMuonMaxDistance_;
      double MinAssociatedSVCut_;
      double MaxAssociatedSVCut_;
};
