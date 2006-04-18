/** \file
 *
 * $Date: 2006/04/13 07:23:16 $
 * $Revision: 1.4 $
 * \author Stefano Lacaprara - INFN Legnaro <stefano.lacaprara@pd.infn.it>
 * \author Riccardo Bellan - INFN TO <riccardo.bellan@cern.ch>
 */

/* This Class Header */
#include "RecoLocalMuon/DTSegment/src/DTRecSegment2DProducer.h"

/* Collaborating Class Header */
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
using namespace edm;
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1D.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2D.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"
#include "RecoLocalMuon/DTSegment/src/DTRecSegment2DAlgoFactory.h"

#include "DataFormats/MuonDetId/interface/DTDetIdAccessor.h"

/* C++ Headers */
#include <string>
using namespace std;

/* ====================================================================== */
// FIXME
bool DTRecSegment2DProducer::debug = false;

/// Constructor
DTRecSegment2DProducer::DTRecSegment2DProducer(const edm::ParameterSet& pset) {
  // Set verbose output
  debug = pset.getUntrackedParameter<bool>("debug"); 

  // the name of the 1D rec hits collection
  theRecHits1DLabel = pset.getParameter<string>("recHits1DLabel");

  if(debug)
    cout << "[DTRecSegment2DProducer] Constructor called" << endl;

  produces<DTRecSegment2DCollection>();

  // Get the concrete reconstruction algo from the factory
  string theAlgoName = pset.getParameter<string>("recAlgo");
  cout << "theAlgoName is " << theAlgoName << endl;
  theAlgo = DTRecSegment2DAlgoFactory::get()->create(theAlgoName,
                                                     pset.getParameter<ParameterSet>("recAlgoConfig"));
}

/// Destructor
DTRecSegment2DProducer::~DTRecSegment2DProducer() {
  if(debug)
    cout << "[DTRecSegment2DProducer] Destructor called" << endl;
}

/* Operations */ 
void DTRecSegment2DProducer::produce(edm::Event& event, const
                                     edm::EventSetup& setup) {
  if(debug)
    cout << "[DTRecSegmentProducer] produce called" << endl;
  // Get the DT Geometry
  ESHandle<DTGeometry> dtGeom;
  setup.get<MuonGeometryRecord>().get(dtGeom);

  theAlgo->setES(setup);
  
  // Get the 1D rechits from the event
  Handle<DTRecHitCollection> allHits; 
  event.getByLabel(theRecHits1DLabel, "DT1DRecHits", allHits);

  // Create the pointer to the collection which will store the rechits
  auto_ptr<DTRecSegment2DCollection> segments(new DTRecSegment2DCollection());

  // Iterate through all hit collections ordered by LayerId
  DTRecHitCollection::id_iterator dtLayerIt;
  DTSuperLayerId oldSlId;
  for (dtLayerIt = allHits->id_begin(); dtLayerIt != allHits->id_end(); ++dtLayerIt){
    // The layerId
    DTLayerId layerId = (*dtLayerIt);
    const DTSuperLayerId SLId = layerId.superlayerId();
    if (SLId==oldSlId) continue; // I'm on the same SL as before
    oldSlId = SLId;

    cout << SLId << endl;

    // // Get the GeomDet from the setup
    // const DTLayer* layer = dtGeom->layer(layerId);
    const DTSuperLayer* sl = dtGeom->superLayer(SLId);
    cout << sl->id() << endl;

    // // Get the iterators over the digis associated with this LayerId
    // const DTRecHitCollection::Range& range = (*dtLayerIt).second;
    
    DTRecHitCollection::range range =
      allHits->get(layerId, DTSuperLayerIdComparator());
    
    // Loop over all digis in the given range
    vector<DTRecHit1DPair> pairs(range.first,range.second);
    cout << "pairs " << pairs.size() << endl;

    // vector<DTRecHit1D> hits;
    // for (vector<DTRecHit1DPair>::const_iterator pair=pairs.begin();
    //      pair!=pairs.end(); ++pair) {
    //   hits.push_back(*(*pair).componentRecHit(DTEnums::Right));
    // }

    cout << "Start Reco " << pairs.size() << endl;
    OwnVector<DTRecSegment2D> segs = theAlgo->reconstruct(sl, pairs);
    cout << "Get segments " << segs.size() << endl;

    if (segs.size() >0 )
      segments->put(SLId, segs.begin(),segs.end());
  }

  event.put(segments);
}


