
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/CtfWithMaterialTrackEmObjectAlgo.h"

// framework includes
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// dataformats includes
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"

// root includes
#include "Math/GenVector/VectorUtil.h"

void CtfWithMaterialTrackEmObjectAlgo::initialise(const edm::ParameterSet &params)
{

   trackProducer_ = params.getParameter<edm::InputTag>("trackProducer");
   ptCut_ = params.getParameter<double>("ptCut");
   requireFiducial_ = params.getParameter<bool>("requireFiducial");
   requireIsolation_ = params.getParameter<bool>("requireIsolation");
   validHits_ = params.getParameter<int>("validHits");
   normalizedChi2_ = params.getParameter<double>("normalizedChi2");

}

void CtfWithMaterialTrackEmObjectAlgo::run(edm::Event &event, EgEff::EmObjectCollection &outCol)
{
  // Get the input collections
   edm::Handle<reco::TrackCollection> trackHandle;
   const reco::TrackCollection *trackCollection = 0;
   try {
      event.getByLabel(trackProducer_, trackHandle);
      trackCollection = trackHandle.product();
   }
   catch(cms::Exception& ex) {
      edm::LogError("CtfWithMaterialTrackEmObjectAlgo") 
        << "Error! Can't get collection" << trackProducer_.label();
      throw ex;   
   }

   const double ebMax = 1.4442;
   const double eeMin = 1.560;
   const double eeMax = 2.500;

   for (unsigned int i = 0; i < trackHandle->size(); ++i)
   {

      // check if the track is meeting the required
      // criterea...
      const reco::TrackRef track = edm::Ref<reco::TrackCollection>(trackHandle, i);
      bool goodTrack = true;

      // check pT cut
      //if (track->momentum().Rho() < ptCut_) goodTrack = false;

      // check fiducial
      if (goodTrack && requireFiducial_) {
         double candRho = track->innerPosition().Rho();
         double candVz = track->innerPosition().z();
         double candEta = track->momentum().eta();
         double candEcalEta = ecalEta(candEta, candVz, candRho);
         if ( (fabs(candEcalEta) > eeMax) || 
              ((fabs(candEcalEta) < eeMin && (fabs(candEcalEta) > ebMax))) )
            goodTrack = false;
      }      

      // check valid hits
      if (goodTrack) {
         if (track->numberOfValidHits() < validHits_) goodTrack = false;
      }

      // check normalized chi2
      if (goodTrack) {
         if (track->normalizedChi2() > normalizedChi2_) goodTrack = false;
      }

      // finally check the isolation if track is still good
      if (goodTrack && requireIsolation_) {
         if (computeTrackIsolation((*track), (*trackCollection)) > 0.02) goodTrack = false;
      }
 
      // finally if we still have a good track
      // then build an EmObject from it

      double t =  sqrt(track->momentum().mag2());
      math::XYZTLorentzVector trackp4(track->momentum().x(), track->momentum().y(), track->momentum().z(), t);

      if (trackp4.pt() > ptCut_){
       EgEff::EmObject obj(track->charge(), trackp4, track->vertex());
      obj.setTrack(track);
      outCol.push_back(obj);
      }
   }

}

double CtfWithMaterialTrackEmObjectAlgo::computeTrackIsolation(const reco::Track& track,
                                                        const reco::TrackCollection& trackCollection)
{

   double dR = 0;
   double ratio = 0;
   double iso = 0;

   const double innerR = 0.02;
   const double outerR = 0.60;

   for (unsigned int i = 0; i < trackCollection.size(); ++i)
   {
      if (trackCollection[i].pt() > 1.5)
      {
         dR = ROOT::Math::VectorUtil::DeltaR(trackCollection[i].momentum(), track.momentum());
         if ((dR > innerR) && (dR < outerR)) 
            { 
            ratio = (trackCollection[i].momentum().Rho()/track.momentum().Rho());
            iso += pow(ratio, 2);
         }
      }
   }

   return iso;

}

