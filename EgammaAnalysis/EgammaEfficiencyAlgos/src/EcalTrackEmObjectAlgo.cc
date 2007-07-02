#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EcalTrackEmObjectAlgo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Math/interface/LorentzVector.h"

void EcalTrackEmObjectAlgo::initialise(const edm::ParameterSet &params)
{
   trackProducer_ = params.getParameter<std::string>("TrackProducer");
   vertexProducer_ = params.getParameter<std::string>("VertexProducer");
   eeSuperClusterProducer_ = params.getParameter<std::string>("EESuperClusterProducer");
   ebSuperClusterProducer_ = params.getParameter<std::string>("EBSuperClusterProducer");
   trackAssociation_ = params.getParameter<bool>("TrackAssociation");
   minSuperClusterEnergy_ = params.getParameter<double>("MinSuperClusterEnergy");
   if(trackAssociation_)
   {
      minTrackPt_ = params.getParameter<double>("MinTrackPt");
      dRSuperClusterTrack_ = params.getParameter<double>("dRSuperClusterTrack");
   }
}

void EcalTrackEmObjectAlgo::run(const edm::Event &event, EgEff::EmObjectCollection &outCol)
{
  // Get the input collections
   edm::Handle<reco::VertexCollection> vertexHandle;
   edm::Handle<reco::TrackCollection> trackHandle;
   edm::Handle<reco::SuperClusterCollection> eeSCHandle;
   edm::Handle<reco::SuperClusterCollection> ebSCHandle;
   try
   {
      event.getByLabel(vertexProducer_, vertexHandle);
      event.getByLabel(trackProducer_, trackHandle);
      event.getByLabel(eeSuperClusterProducer_, eeSCHandle);
      event.getByLabel(ebSuperClusterProducer_, ebSCHandle);
   }
   catch(cms::Exception& ex)
   {
      edm::LogError("ProbeEmObjectAlgo") << "Error! Can't get collections";
      throw ex;   
   }

   // Now create the objects
   const reco::VertexCollection *vertices = vertexHandle.product();

   if(vertices->size() > 0)
   {
      const math::XYZPoint pvPos((*vertices)[0].x(), (*vertices)[0].y(), (*vertices)[0].z());
      processSCCollection(pvPos, eeSCHandle, trackHandle, outCol);
      processSCCollection(pvPos, ebSCHandle, trackHandle, outCol);
   }
}

void EcalTrackEmObjectAlgo::processSCCollection(const math::XYZPoint &pvPos, edm::Handle<reco::SuperClusterCollection> &scHandle, edm::Handle<reco::TrackCollection> &trackHandle, EgEff::EmObjectCollection &outCol)
{
   const reco::SuperClusterCollection *scs = scHandle.product();
   for(unsigned int i = 0; i < scs->size(); ++i)
   {
      // Get the SC seed
      const reco::SuperClusterRef seed = edm::Ref<reco::SuperClusterCollection>(scHandle, i);

      // Check SC is of high enough energy
      if(seed->energy() > minSuperClusterEnergy_)
      {
         // Get the index of highest pT track with pT > 10GeV and dR < 0.2
         if(trackAssociation_)
         {
	    int trackFound = findTrack(seed, trackHandle.product());
            if(trackFound != -1)
	    {
	       const reco::TrackRef track = edm::Ref<reco::TrackCollection>(trackHandle, trackFound);
	       math::XYZTLorentzVector p4 = initP4(pvPos, seed);
	       EgEff::EmObject obj(0, p4, pvPos);
	       obj.setTrack(track);
	       obj.setSuperCluster(seed);
	       outCol.push_back(obj);
	    }
         }
         else
         {
            math::XYZTLorentzVector p4 = initP4(pvPos, seed);
	    EgEff::EmObject obj(0, p4, pvPos);
	    obj.setSuperCluster(seed);
	    outCol.push_back(obj);
         }
      }
   }
}

math::XYZTLorentzVector EcalTrackEmObjectAlgo::initP4(const math::XYZPoint &pvPos, const reco::SuperClusterRef &sc)
{
   math::XYZVector scPos(sc->x(), sc->y(), sc->z());
   math::XYZVector pvPosVec(pvPos.x(), pvPos.y(), pvPos.z());
   math::XYZVector objPosition = scPos - pvPosVec;
   double scale = sc->energy() / objPosition.R();
   return math::XYZTLorentzVector(objPosition.x() * scale, objPosition.y() * scale, objPosition.z() * scale, sc->energy());
}

int EcalTrackEmObjectAlgo::findTrack(const reco::SuperClusterRef &seed, const reco::TrackCollection *tracks)
{
   int retTrack = -1;
   
   // See if we have a track with pT > 10GeV and dR < 0.2
   unsigned int numTracks = tracks->size();
   double highestPt = 0;

   for(unsigned int j = 0; j < numTracks; j++)
    {
      if(dR((*tracks)[j].eta(), (*tracks)[j].phi(), seed->position().Eta(), seed->position().Phi()) < dRSuperClusterTrack_)
      {
         double trackPt = (*tracks)[j].pt();
         if((trackPt > 10) && (trackPt > highestPt))
         {
            retTrack = j;
            highestPt = trackPt;
         }
      }
   }

   return retTrack;
}

double EcalTrackEmObjectAlgo::dPhi(double phi1, double phi2)
{
   double diff = fabs(phi2 - phi1);
   double corr = 2*acos(-1.) - diff;
   if (diff < acos(-1.)){ return diff;} else { return corr;}
}

double EcalTrackEmObjectAlgo::dR(double eta1, double phi1, double eta2, double phi2)
{
   return sqrt(((eta1-eta2)*(eta1-eta2)) + (dPhi(phi1, phi2) * dPhi(phi1, phi2)));
}
