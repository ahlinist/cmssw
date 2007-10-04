
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/SuperClusterEmObjectAlgo.h"

// framework includes
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// dataformats includes
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/LorentzVector.h"

void SuperClusterEmObjectAlgo::initialise(const edm::ParameterSet &params)
{

   std::cout << "SuperClusterEmObjectAlgo::initialise" << std::endl;

   vertexProducer_ = params.getParameter<edm::InputTag>("vertexProducer");
   eeSuperClusterProducer_ = params.getParameter<edm::InputTag>("ebSuperClusterProducer");
   ebSuperClusterProducer_ = params.getParameter<edm::InputTag>("eeSuperClusterProducer");

   scEtCut_ = params.getParameter<double>("scEtCut");
   requireFiducial_ = params.getParameter<bool>("requireFiducial");

   std::cout << "SuperClusterEmObjectAlgo::initialise done" << std::endl;

}

void SuperClusterEmObjectAlgo::run(edm::Event &event, EgEff::EmObjectCollection &outCol)
{
  // Get the input collections
   edm::Handle<reco::VertexCollection> vertexHandle;
   edm::Handle<reco::SuperClusterCollection> eeSCHandle;
   edm::Handle<reco::SuperClusterCollection> ebSCHandle;
   try
   {
      event.getByLabel(vertexProducer_, vertexHandle);
      event.getByLabel(eeSuperClusterProducer_, eeSCHandle);
      event.getByLabel(ebSuperClusterProducer_, ebSCHandle);
   }
   catch(cms::Exception& ex)
   {
      edm::LogError("SuperClusterEmObjectAlgo") << "Error! Can't get collections";
      throw ex;   
   }

   const reco::VertexCollection *vertices = vertexHandle.product();
   if(vertices->size() > 0)
   {
      const math::XYZPoint pvPos((*vertices)[0].x(), (*vertices)[0].y(), (*vertices)[0].z());
      processSCCollection(pvPos, eeSCHandle, outCol);
      processSCCollection(pvPos, ebSCHandle, outCol);
   }

}

void SuperClusterEmObjectAlgo::processSCCollection(const math::XYZPoint &pvPos,
                                                   edm::Handle<reco::SuperClusterCollection> &scHandle, 
                                                   EgEff::EmObjectCollection &outCol)
{

   const double ebMax = 1.4442;
   const double eeMin = 1.560;
   const double eeMax = 2.500;

   const reco::SuperClusterCollection *scCollection = scHandle.product();
   for(unsigned int i = 0; i < scCollection->size(); ++i)
   {

      // get reference to supercluster
      const reco::SuperClusterRef scRef = edm::Ref<reco::SuperClusterCollection>(scHandle, i);

      // test requirements to become a probe object
      bool goodSuperCluster = true;

      // Et Cut
      if (scRef->energy()/cosh(scRef->eta()) < scEtCut_)
         goodSuperCluster = false;

      // check fiducial
      if (goodSuperCluster && requireFiducial_) {
         if ( (fabs(scRef->eta()) > eeMax) ||
              ((fabs(scRef->eta()) < eeMin && (fabs(scRef->eta()) > ebMax))) )
            goodSuperCluster = false;
      }

      if (goodSuperCluster)
      {
         math::XYZTLorentzVector p4 = initP4(pvPos, scRef);
	 EgEff::EmObject obj(0, p4, pvPos);
	 obj.setSuperCluster(scRef);
	 outCol.push_back(obj);
      }

   }  // end loop on superclusters
}

math::XYZTLorentzVector SuperClusterEmObjectAlgo::initP4(const math::XYZPoint &pvPos, 
                                                         const reco::SuperClusterRef &sc)
{

   math::XYZVector scPos(sc->x(), sc->y(), sc->z());
   math::XYZVector pvPosVec(pvPos.x(), pvPos.y(), pvPos.z());
   math::XYZVector objPosition = scPos - pvPosVec;
   double scale = sc->energy() / objPosition.R();
   return math::XYZTLorentzVector(objPosition.x() * scale, objPosition.y() * scale, objPosition.z() * scale, sc->energy());

}

