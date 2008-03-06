
#include "SUSYBSMAnalysis/Zprime2eeAnalysis/interface/SCCreationEfficiency.h"

// framework includes
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

// dataformats includes
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

// Root includes
#include "Math/GenVector/VectorUtil.h"

//---------------------------------------------------------------------
// SCCreationEfficiency::Add
//  Adds this event to the histograms
void SCCreationEfficiency::Add(const edm::Event &event, const edm::EventSetup &eventSetup)
{

  double weight = 1.0;

  try
  {
  // Get this event's weight
  edm::Handle< double> weightHandle;
  event.getByLabel ("weight","weight", weightHandle);
  weight = * weightHandle;
  } catch(cms::Exception &ex) {
    // edm::LogInfo("SCCreationEfficiency") i
    //   << "INFO:  No weight in event - weight is 1.0";
  }

  // Get the data

  edm::Handle<reco::SuperClusterCollection> scEBHandle;
  edm::Handle<reco::SuperClusterCollection> scEEHandle;

  try
  {
     event.getByLabel(scEBTag_, scEBHandle);
     event.getByLabel(scEETag_, scEEHandle);
  } catch(cms::Exception &ex) {
    std::cout << "superclusters not present" << std::endl;
    std::cout << "EB label " << scEBTag_.label() << std::endl;
    std::cout << "EE label " << scEETag_.label() << std::endl;
  }

  const reco::SuperClusterCollection *scEBCollection = scEBHandle.product();
  const reco::SuperClusterCollection *scEECollection = scEEHandle.product();

  // Get the tag + probe association map

  edm::Handle<EgEff::TagProbeAssociationCollection> _tpAssocMap;
  event.getByLabel("scTagProbes", _tpAssocMap);

  EgEff::TagProbeAssociationCollection::const_iterator itr; 

  for(itr = _tpAssocMap->begin(); itr != _tpAssocMap->end(); ++itr)
    {

      const EgEff::EmObjectRef tag = itr->key;

      // Get associated probes

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

          bool match = false;
	  unsigned int j = 0;

	  while((j < scEBCollection->size()) && !match) {
              match = MatchObjects(scEBCollection->at(j), probes[probeIndex]);
	      j++;
	  } if (match == false) {
              j = 0;
              while((j < scEECollection->size()) && !match) {
                 match = MatchObjects(scEECollection->at(j), probes[probeIndex]);
                 j++;
              }
          }

	 mass = ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probes[probeIndex]->p4());
         if (mass < 95.0 && mass > 85.0) AddToTotalHistograms(probes[probeIndex], weight);
         if (mass < 95.0 && mass > 85.0 && match) AddToPassHistograms(probes[probeIndex], weight);

          // os requirement
          bool os = true;
	  //std::cout << probes[probeIndex]->charge() << ", " << tag->charge() << std::endl;
          if (probes[probeIndex]->charge() * tag->charge() == 1) os = false;
          AddToTree(probes[probeIndex], tag, weight, match, os);

       } // end loop on all probes

     }  // end if 1 or fewer probes in mass range
 
   }  // end loop on tag probe map

}

//---------------------------------------------------------------------
// SCCreationEfficiency::Initialise
//  Load specific HL parameters
void SCCreationEfficiency::Initialise(const edm::ParameterSet &params)
{

  // Get specific parameters
  const edm::ParameterSet pset = params.getUntrackedParameter<edm::ParameterSet>("params");
  scEBTag_ = pset.getUntrackedParameter<edm::InputTag>("scEBTag");
  scEETag_ = pset.getUntrackedParameter<edm::InputTag>("scEETag");
  dR_ = pset.getUntrackedParameter<double>("dR");
  dEta_ = pset.getUntrackedParameter<double>("dEta");
  scEtCut_ = pset.getUntrackedParameter<double>("scEtCut");

  edm::Service<TFileService> fs;
  h1_dEta = fs->make<TH1F>("h1_dEta", "h1_dEta", 200, 0, 1);
  h1_dR = fs->make<TH1F>("h1_dR", "h1_dR", 200, 0, 1);

  // Create generic histograms
  InitialiseHistograms(pset);

}

//---------------------------------------------------------------------
// SCCreationEfficiency::MatchObjects
//  Matches a trigger candidate to a probe candidates
bool SCCreationEfficiency::MatchObjects(const reco::SuperCluster &scCand, EgEff::EmObjectRef probeCand)
{

   double trackPosRho = probeCand->track()->dxy();
   double trackVz = probeCand->track()->vz();
   double trackEta = probeCand->eta();
   double trackEcalEta = ecalEta(trackEta, trackVz, trackPosRho);

   double trackMomRho = probeCand->track()->pt();
   int trackCharge = probeCand->track()->charge();
   double trackPhi = probeCand->track()->phi();
   double trackEcalPhi = ecalPhi(trackMomRho, trackEta, trackPhi, trackCharge, trackPosRho);

   double scEta = scCand.eta();
   double scPhi = scCand.phi();
   double scEt = scCand.energy()/cosh(scCand.eta());

   double dPhi = fabs(scPhi - trackEcalPhi);
   double dEta = fabs(scEta - trackEcalEta);
   double dR = sqrt(dPhi*dPhi + dEta*dEta);

   if (trackEcalEta > 1.5) {
     h1_dEta->Fill(dEta);
     h1_dR->Fill(dR);
   }

   return ((dR < dR_) && (dEta < dEta_) && (scEt > scEtCut_));

}

