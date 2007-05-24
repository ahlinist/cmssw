
#include "SimMuon/RPCPolluter/interface/RPCPolluter.h"
#include <time.h>
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
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Framework/interface/MakerMacros.h>




// RPCPolluter

// Constructor of RPCPolluter

RPCPolluter::RPCPolluter(const edm::ParameterSet& iConfig)
{

  produces<RPCDigiCollection>();

  rate=iConfig.getParameter<double>("Rate");

  nbxing=iConfig.getParameter<int>("Nbxing");

  gate=iConfig.getParameter<double>("Gate");

}


void RPCPolluter::produce(edm::Event& e, const edm::EventSetup& es)
{


  edm::ESHandle<RPCGeometry> rpcGeo;
  es.get<MuonGeometryRecord>().get(rpcGeo);

  std::auto_ptr<RPCDigiCollection> pDigis(new RPCDigiCollection());


  for (TrackingGeometry::DetContainer::const_iterator it=rpcGeo->dets().begin();it<rpcGeo->dets().end();it++){
    
    RPCRoll* roll = dynamic_cast<RPCRoll*>(*it);
    if (roll) {
    RPCDetId rpcId = roll->id();

    int nstrips = roll->nstrips();

    double area = 0.0;

    if ( rpcId.region() == 0 )
      {
	const RectangularStripTopology* top_ = dynamic_cast<const
	  RectangularStripTopology*>(&(roll->topology()));
	float xmin = (top_->localPosition(0.)).x();
	float xmax = (top_->localPosition((float)roll->nstrips())).x();
	float striplength = (top_->stripLength());
	area = striplength*(xmax-xmin);
      }

    else
      {
	const TrapezoidalStripTopology* top_=dynamic_cast<const TrapezoidalStripTopology*>(&(roll->topology()));
	float xmin = (top_->localPosition(0.)).x();
	float xmax = (top_->localPosition((float)roll->nstrips())).x();
	float striplength = (top_->stripLength());
	area = striplength*(xmax-xmin);
      }

    double ave = rate*nbxing*gate*area*1.0e-9;

    N_hits = RandPoisson::shoot(ave);
    std::cout <<" Number of hits "<<N_hits<<std::endl;
    for (int i = 0; i < N_hits; i++ )
      {
	int strip = RandFlat::shootInt(nstrips);
	int time_hit = static_cast<int>(RandFlat::shoot((nbxing*gate))/gate);
	RPCDigi rpcDigi(strip,time_hit);
	pDigis->insertDigi(rpcId,rpcDigi);
      }
    }    
  }
  std::cout <<"Storing the data on the event "<<std::endl;
  e.put(pDigis);
  std::cout <<"Done !! "<<std::endl;

}  // End Constructor of RPCPolluter





// Destructor of RPCPolluter

RPCPolluter::~RPCPolluter()
{


}  // End Destructor of RPCPolluter

#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(RPCPolluter);
