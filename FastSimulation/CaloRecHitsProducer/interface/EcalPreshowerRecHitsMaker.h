#ifndef FastSimulation__EcalPreshowerRecHitsMaker__h
#define FastSimulation__EcalPreshowerRecHitsMaker__h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "FWCore/Framework/interface/Event.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FastSimulation/Utilities/interface/GaussianTail.h"
#include "FastSimulation/CaloRecHitsProducer/interface/SignalHit.h"

#include <map>
#include <vector>

#include <boost/cstdint.hpp>

class CaloGeometry;
class RandomEngine;

class EcalPreshowerRecHitsMaker
{
 public:
  EcalPreshowerRecHitsMaker(edm::ParameterSet const & p, const RandomEngine* random);
  ~EcalPreshowerRecHitsMaker();

  void loadEcalPreshowerRecHits(edm::Event &iEvent, ESRecHitCollection& esRecHits);
  void init(const edm::EventSetup &es);
  


 private:
  void loadPSimHits(const edm::Event & iEvent);
  
  void clean();

  unsigned createVectorsOfCells(const edm::EventSetup &es);
  void noisifySubdet(std::map<SignalHit,float>& theMap, const std::vector<uint32_t>& thecells, unsigned ncells);
  void noisifyAndFill(uint32_t id,float energy, std::map<SignalHit,float>& myHits);

 private:
  double threshold_;
  double noise_;
  double preshowerHotFraction_;
  bool initialized_;
  unsigned ncells_;
  std::map<SignalHit,float> ecalsRecHits_;
  std::vector<uint32_t> escells_;
  GaussianTail myGaussianTailGenerator_;
  const RandomEngine* random_;
};

#endif
