#include "RecoParticleFlow/PFAnalyses/interface/EcalCalibRechitProducer.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"

#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"

#include "DataFormats/Math/interface/Vector3D.h"

#include <iostream>
#include <fstream>
#include <string>
namespace reco
{
}
using namespace std;
using namespace reco;
using namespace edm;
using namespace pftools;

EcalCalibRechitProducer::EcalCalibRechitProducer(
    const edm::ParameterSet& params)
{

  string eeNoisesFile = params.getUntrackedParameter<string> ("EENoisesFile");
  string eeCoeffsFile = params.getUntrackedParameter<string> ("EECoeffsFile");

  inputTagEEUncalColl_ = params.getParameter<InputTag> ("EEUncalColl");

  produces<EERecHitCollection> ("ecalEERechitsCalib");
  produces<EBRecHitCollection> ("ecalEBRechitsCalib");

  //populate detector calibration coefficients,
  // Reading EE calibration constants

  ifstream calibFile;
  calibFile.open(eeCoeffsFile.c_str());

  string label;
  int rawid;
  float coef;

  for (int ich = 0; ich < 100; ich++)
    {
      calibFile >> label >> rawid >> label >> coef;
      detCalibs[rawid] = coef;
    }

  calibFile.close();

  ifstream noiseFile;
  noiseFile.open(eeNoisesFile.c_str());

  int ieta, iphi;

  float meanNoise, sigmaNoise;

  for (int ich = 0; ich < 100; ich++)
    {
      noiseFile >> ieta >> iphi >> meanNoise >> sigmaNoise;
      detNoises[make_pair<int, int> (ieta, iphi)] = make_pair<float, float> (
          meanNoise, sigmaNoise);
    }

  noiseFile.close();

}

EcalCalibRechitProducer::~EcalCalibRechitProducer()
{

}

void
EcalCalibRechitProducer::beginJob(const edm::EventSetup&)
{

}

void
EcalCalibRechitProducer::endJob()
{

}

void
EcalCalibRechitProducer::produce(edm::Event& event,
    const edm::EventSetup& setup)
{

  edm::ESHandle<CaloGeometry> geoHandle;
  setup.get<CaloGeometryRecord> ().get(geoHandle);


  // get the endcap geometry
  const CaloSubdetectorGeometry *eetmp = geoHandle->getSubdetectorGeometry(
      DetId::Ecal, EcalEndcap);

  const EcalEndcapGeometry* ecalEndcapGeometry =
      dynamic_cast<const EcalEndcapGeometry*> (eetmp);
  assert(ecalEndcapGeometry);

  // get the endcap topology
  EcalEndcapTopology ecalEndcapTopology(geoHandle);

  //loop over uncalibrated sausages
  eeUncalColl_ = new Handle<EcalUncalibratedRecHitCollection> ;
  getCollection(*eeUncalColl_, inputTagEEUncalColl_, event);

  auto_ptr<EERecHitCollection> ecalEERecHitColl(new EERecHitCollection());
  auto_ptr<EBRecHitCollection> ecalEBRecHitColl(new EBRecHitCollection());

  const EEUncalibratedRecHitCollection hits = **eeUncalColl_;
  EEUncalibratedRecHitCollection::const_iterator hitIt = hits.begin();
  //	LogInfo("EcalCalibRechitProducer") << "found " << hits.size()
  //			<< " hits to calibrate";
  for (; hitIt != hits.end(); ++hitIt)
    {
      const EcalUncalibratedRecHit hit = *hitIt;

      const int id = hit.id().rawId();

      map<int, float>::const_iterator cit = detCalibs.find(id);
      if (cit == detCalibs.end())
        {
          LogWarning("EcalCalibRechitProducer")
              << "couldn't find calibration for hit " << id;

        }
      else
        {
          float calibCoef = cit->second;

          //		float sigmaped = 2.33; // sigma of pedestals
          //if (amplEE> sigmaped * 3.) fEECalibEnergy += amplEE* calibCoef;

          EEDetId originalId(id);

          map<pair<int, int> , pair<float, float> >::const_iterator git =
              detNoises.find(make_pair<int, int> (originalId.ix(),
                  originalId.iy()));
          if (git != detNoises.end())
            {
              float noise = git->second.first;
              double energy = hit.amplitude() * calibCoef - noise;
              //								cout << "\tID = " << id << "(" << eedet.ix() << ", "
              //						<< eedet.iy() << "),  amplitude * coeff - noise = "
              //						<< hit.amplitude() << " * " << calibCoef << "- "
              //						<< noise << " = " << energy << "\n";
              pair<DetId, bool> ans = testbeamEndcapTranslation(hit.id(),
                  ecalEndcapGeometry, ecalEndcapTopology);
              DetId& detid = ans.first;
              if (!ans.second)
                {
                  //translation wasn't sucessfull - skip this rechit
                  continue;
                }
              EcalRecHit calibHit(detid, energy, 0., EcalRecHit::kGood);
              ecalEERecHitColl->push_back(calibHit);
            }
          else
            {
              LogWarning("EcalCalibRechitProducer")
                  << "couldn't find noise value for hit " << id << " = "
                  << originalId.ix() << ", " << originalId.iy();
            }
        }
    }

  if (eeUncalColl_)
    delete eeUncalColl_;

  event.put(ecalEERecHitColl, "ecalEERechitsCalib");
  event.put(ecalEBRecHitColl, "ecalEBRechitsCalib");

}

std::pair<DetId, bool>
EcalCalibRechitProducer::testbeamEndcapTranslation(const DetId& detid,
    const CaloSubdetectorGeometry* geom, const CaloSubdetectorTopology& topo)
{
  math::XYZVector position;
  math::XYZVector axis;

  //move around a bit for S & G :-)

  DetId currStep1 = detid;
  bool ok = true;

  for (unsigned p(0); p < 1; ++p)
    {
      vector<DetId> currStep = topo.east(currStep1);
      if (currStep.size() != 1)
        {
          cout << "Erm, next east vector size was " << currStep.size()
              << ", iteration = " << p << "\n";
          ok = false;
          break;
        }
      currStep1 = currStep[0];
    }

  for (unsigned j(0); j < 16; ++j)
    {
      vector<DetId> currStep = topo.north(currStep1);
      if (currStep.size() != 1)
        {
          cout << "Erm, next north vector size was " << currStep.size()
              << ", iteration = " << j << "\n";
          ok = false;
          break;
        }
      currStep1 = currStep[0];
    }

  const CaloCellGeometry *currStepCell = geom->getGeometry(currStep1);
  math::XYZVector currStep1Pos;
  currStep1Pos.SetCoordinates(currStepCell->getPosition().x(),
      currStepCell->getPosition().y(), currStepCell->getPosition().z());

  const CaloCellGeometry *thisCell = geom->getGeometry(detid);

  // find rechit geometry
  if (!thisCell && !currStepCell)
    {
      LogError("PFRecHitProducerECAL") << "warning detid " << detid.rawId()
          << " not found in geometry" << endl;
      ok = false;

    }

  position.SetCoordinates(thisCell->getPosition().x(),
      thisCell->getPosition().y(), thisCell->getPosition().z());

  cout << "---\n";
  cout << "Before we were at  " << position.eta() << ", " << position.phi()
      << "\n";
  cout << "\t" << position.x() << ", " << position.y() << ", " << position.z()
      << "\n";

  cout << "Now we are at  " << currStep1Pos.eta() << ", " << currStep1Pos.phi()
      << "\n";
  cout << "\t" << currStep1Pos.x() << ", " << currStep1Pos.y() << ", "
      << currStep1Pos.z() << "\n";

  cout << "DIFF is " << currStep1Pos.eta() - position.eta() << ", "
      << currStep1Pos.phi() - position.phi() << "\n";

  return make_pair<DetId, bool> (currStep1, ok);
}

//define this as a plug-in
DEFINE_FWK_MODULE( EcalCalibRechitProducer);
