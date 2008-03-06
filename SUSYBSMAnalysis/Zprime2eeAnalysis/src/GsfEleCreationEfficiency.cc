
#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/GsfEleCreationEfficiency.h"

// framework includes
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// dataformats includes
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

// Root includes
#include "Math/GenVector/VectorUtil.h"

//---------------------------------------------------------------------
// GsfEleCreationEfficiency::Add
//  Adds this event to the histograms
void GsfEleCreationEfficiency::Add(const edm::Event &event, const edm::EventSetup &eventSetup)
{

  // Get this event's weight

  double weight = 1.0;

  try
  {
  // Get this event's weight
  edm::Handle< double> weightHandle;
  event.getByLabel ("weight","weight", weightHandle);
  weight = * weightHandle;
  } catch(cms::Exception &ex) {
    // edm::LogInfo("GsfEleCreationEfficiency") i
    //   << "INFO:  No weight in event - weight is 1.0";
  }

  // Get edm data

  edm::Handle<reco::GsfTrackCollection> gsfTrackEBHandle;
  edm::Handle<reco::GsfTrackCollection> gsfTrackEEHandle;
  edm::Handle<reco::PixelMatchGsfElectronCollection> gsfEleHandle;

  event.getByLabel(gsfTrackEBTag_, gsfTrackEBHandle);
  event.getByLabel(gsfTrackEETag_, gsfTrackEEHandle);
  event.getByLabel(gsfEleTag_, gsfEleHandle);

  const reco::PixelMatchGsfElectronCollection *gsfEleCollection = gsfEleHandle.product();

  // Get the tag + probe association map

  edm::Handle<EgEff::TagProbeAssociationCollection> _tpAssocMap;
  event.getByLabel("gsfEleTagProbes", _tpAssocMap);

  EgEff::TagProbeAssociationCollection::const_iterator itr; 

  // Loop over tag objects

  //std::cout << "will loop on tag probe map" << std::endl;
  for(itr = _tpAssocMap->begin(); itr != _tpAssocMap->end(); ++itr)
    {
     //std::cout << "looping on probe map..." << std::endl;
     //std::cout << "getting tag" << std::endl;
      const EgEff::EmObjectRef tag = itr->key;

      // Get associated probes
      //std::cout << "getting probes " << std::endl;
      const EgEff::EmObjectRefVector &probes = itr->val;

      // get probes in the mass range
      //   if more than one in the mass range
      //   ignore this tag     

      double mass = 0;
      EgEff::EmObjectRefVector probesInMassRange;
      for(unsigned int i = 0; i < probes.size(); ++i)
      {
         mass = ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probes[i]->p4());
         if (mass < 95.0 && mass > 85.0) probesInMassRange.push_back(probes[i]);
      }
      if (probesInMassRange.size()  <  2) {

        // Test the probe against all trigger cands until match found

        for (unsigned int probeIndex = 0; probeIndex < probes.size(); ++probeIndex)
        {

           // does the track matched probe match a gsf electron?
           // or if going straight from sc to electron, just does
           // the sc match the electron
	   bool matchEle = false;
	   unsigned int j = 0;
           while((j < gsfEleCollection->size()) && !matchEle) {

              matchEle = MatchObjects((*(gsfEleCollection->at(j).gsfTrack())), probes[probeIndex]);
              if ((gsfEleCollection->at(j).superCluster()->energy())/(cosh(gsfEleCollection->at(j).superCluster()->eta())) < 15) matchEle = false;
              j++;
           }

           //std::cout << "adding to histograms" << std::endl;
           mass = ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probes[probeIndex]->p4());
           if (mass < 95.0 && mass > 85.0) AddToTotalHistograms(probes[probeIndex], weight);
	   if (mass < 95.0 && mass > 85.0 && matchEle) AddToPassHistograms(probes[probeIndex], weight);

           bool os = false; // no sign information if probe sc
           if (matchEle) AddToTree(probes[probeIndex], tag, weight, matchEle, os);

           //std::cout << "done adding" << std::endl;

        } // end loop on all probes

     }  // end if 1 or fewer probes in mass range

   }  // end loop on tag probe map

}

//---------------------------------------------------------------------
// GsfEleCreationEfficiency::Initialise
//  Load specific HL parameters
void GsfEleCreationEfficiency::Initialise(const edm::ParameterSet &params)
{

  // Get specific parameters
  const edm::ParameterSet pset = params.getUntrackedParameter<edm::ParameterSet>("params");

  gsfTrackEBTag_ = pset.getUntrackedParameter<edm::InputTag>("gsfTrackEBTag");
  gsfTrackEETag_ = pset.getUntrackedParameter<edm::InputTag>("gsfTrackEETag");
  gsfEleTag_ = pset.getUntrackedParameter<edm::InputTag>("gsfEleTag");

  scToEle_ = pset.getUntrackedParameter<bool>("scToEle");
  dR_ = pset.getUntrackedParameter<double>("dR");
  validHits_ = pset.getUntrackedParameter<int>("validHits");
  normalizedChi2_ = pset.getUntrackedParameter<double>("normalizedChi2");  

  // Create generic histograms
  InitialiseHistograms(pset);

}

//---------------------------------------------------------------------
// GsfEleCreationEfficiency::MatchObjects
//  Matches a trigger candidate to a probe candidates
bool GsfEleCreationEfficiency::MatchObjects(const reco::GsfTrack &trackCand, EgEff::EmObjectRef probeCand)
{

   double trackPosRho = trackCand.dxy();
   double trackVz = trackCand.vz();
   double trackEta = trackCand.eta();
   double trackEcalEta = ecalEta(trackEta, trackVz, trackPosRho);

   double trackMomRho = trackCand.pt();
   int trackCharge = trackCand.charge();
   double trackPhi = trackCand.phi();
   double trackEcalPhi = ecalPhi(trackMomRho, trackEta, trackPhi, trackCharge, trackPosRho);

   double probeEta = 0;
   double probePhi = 0;

   double probePosRho = probeCand->vertex().Rho() / 10;
   double probeVz = probeCand->vertex().z() / 10;
   double probeEtaRaw = probeCand->p4().eta();
   probeEta = ecalEta(probeEtaRaw, probeVz, probePosRho);

   double probeMomRho = probeCand->p4().Rho();
   int probeCharge = probeCand->charge();
   double probePhiRaw = probeCand->p4().phi();

   probePhi = ecalPhi(probeMomRho, probeEtaRaw, probePhiRaw, probeCharge, probePosRho);


   //std::cout << "eta, phi " << probeEta << ", " << probePhi << std::endl;

   double dPhi = fabs(probePhi - trackEcalPhi);
   double dEta = fabs(probeEta - trackEcalEta);
   double dR = sqrt(dPhi*dPhi + dEta*dEta);
   //std::cout << dR << std::endl;

   bool goodTrack = true;
   if (trackCand.numberOfValidHits() < validHits_) goodTrack = false;
   if (trackCand.normalizedChi2() > normalizedChi2_) goodTrack = false;

   return ((dR < dR_) && goodTrack);

}

