#ifndef SimMuon_RPCPolluter_h
#define SimMuon_RPCPolluter_h


#include <time.h>
#include <vector>
#include <algorithm>
#include <iterator>

#include "CLHEP/Geometry/Point3D.h"

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoisson.h"

#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCRollSpecs.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include <Geometry/RPCGeometry/interface/RPCGeometry.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>

#include <DataFormats/RPCDigi/interface/RPCDigiCollection.h>
#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>


/*  class RPCPolluter
 *
 *  Creates neutron hits in a given RPCRoll
 *
 *  Authors : C. Avila, B. Gomez, J.C. Sanabria
 *
 *            Universidad de los Andes, Bogota
 */


class RPCPolluter
{

  double rate;

  unsigned int nbxing;


 public :

  int N_hits;

  std::vector<int> strip_vector;

  std::vector<double> time_vector;


  RPCPolluter(const RPCRoll*);

  ~RPCPolluter();

};


#endif
