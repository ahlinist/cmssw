/******* \class DTEffAnalyzer *******
 *
 * Description:
 *  
 *  detailed description
 *
 * \author : Mario Pelliccioni, pellicci@cern.ch
 * $date   : 20/11/2008 16:50:57 CET $
 *
 * Modification:
 *
 *********************************/

#include "DTChamberEfficiency.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "RecoMuon/Navigation/interface/DirectMuonNavigation.h"

#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"


#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h" 
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "DataFormats/Common/interface/RefToBase.h" 





#include <cmath>

using namespace std;
using namespace edm;

DTChamberEfficiency::DTChamberEfficiency(const ParameterSet& pSet)
{
  // Get the debug parameter for verbose output
  debug = pSet.getUntrackedParameter<bool>("debug","false");
  
  LogVerbatim("DTDQM|DTMonitorModule|DTChamberEfficiency")
    << "DTChamberEfficiency: constructor called";

  // Get the DQM needed services
  theDbe = Service<DQMStore>().operator->();
  theDbe->setCurrentFolder("DT/05-ChamberEff/Task");

  // service parameters
  ParameterSet serviceParameters = pSet.getParameter<ParameterSet>("ServiceParameters");
  theService = new MuonServiceProxy(serviceParameters);

  theTracksLabel = pSet.getParameter<InputTag>("TrackCollection");

  theMaxChi2 = static_cast<unsigned int>(pSet.getParameter<double>("theMaxChi2"));
  theNSigma = pSet.getParameter<double>("theNSigma");
  theMinNrec = static_cast<int>(pSet.getParameter<double>("theMinNrec"));

  labelRPCRecHits = pSet.getParameter<InputTag>("theRPCRecHits");

  thedt4DSegments = pSet.getParameter<InputTag>("dt4DSegments");
  thecscSegments = pSet.getParameter<InputTag>("cscSegments");

  theMeasurementExtractor = new MuonDetLayerMeasurements(thedt4DSegments,thecscSegments,
							 labelRPCRecHits,true,false,false); 

  theNavigationType = pSet.getParameter<string>("NavigationType");

  theEstimator = new Chi2MeasurementEstimator(theMaxChi2,theNSigma); 
}


DTChamberEfficiency::~DTChamberEfficiency()
{
  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency")
    << "DTChamberEfficiency: destructor called";

  // free memory
  delete theService;
  delete theMeasurementExtractor;
  delete theEstimator;
}

void DTChamberEfficiency::beginJob(const EventSetup& eventSetup) {

  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency")
    << "DTChamberEfficiency: beginOfJob";

  bookHistos();

  return;
}

void DTChamberEfficiency::beginRun(const Run& run, const EventSetup& setup)
{
  // Get the DT Geometry
  setup.get<MuonGeometryRecord>().get(dtGeom);

  setup.get<IdealMagneticFieldRecord>().get(magfield);
  setup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry);

  return;
}

void DTChamberEfficiency::endJob()
{
  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency")
    << "DTChamberEfficiency: endOfJob";

  return;
}

// Book a set of histograms for a given Layer
void DTChamberEfficiency::bookHistos()
{
  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency")
    << "DTChamberEfficiency: booking histos";

  // Create the monitor elements
  theDbe->setCurrentFolder("DT/05-ChamberEff/Task");

  for(int wheel=-2;wheel<=2;wheel++){

    vector<MonitorElement *> histos;

    stringstream wheel_str; wheel_str << wheel;	

    histos.push_back(theDbe->book2D("hCountSectVsChamb_All_W"+ wheel_str.str(),
				    "Countings for wheel " + wheel_str.str(),14,1.,15.,4,1.,5.));

    histos.push_back(theDbe->book2D("hCountSectVsChamb_Qual_W"+ wheel_str.str(),
				    "Countings for wheel " + wheel_str.str(),14,1.,15.,4,1.,5.));


    histos.push_back(theDbe->book2D("hExtrapSectVsChamb_W"+ wheel_str.str(),
				    "Extrapolations for wheel " + wheel_str.str(),14,1.,15.,4,1.,5.));

    histosPerW.push_back(histos);
  }

  return;
}

void DTChamberEfficiency::analyze(const Event & event,
                            const EventSetup& eventSetup)
{

  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency") <<
    "--- [DTChamberEfficiency] Event analysed #Run: " <<
    event.id().run() << " #Event: " << event.id().event() << endl;

  theService->update(eventSetup);
  theMeasurementExtractor->setEvent(event); 

  //Read tracks from event
  Handle<reco::TrackCollection> tracks;
  event.getByLabel(theTracksLabel, tracks);
  
  if(tracks.isValid()) { // check the validity of the collection

    //loop over the muons
    for(reco::TrackCollection::const_iterator track = tracks->begin(); track!=tracks->end(); ++track) {

      
      reco::TransientTrack trans_track(*track,magfield.product(),theTrackingGeometry);
      const int recHitsize = (int)trans_track.recHitsSize();
      if(recHitsize < theMinNrec) continue;

      // printout the DT rechits used by the track
      if (debug) {
	LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency") << "--- New track" << endl;
	set<DTChamberId> chAlrUsed;
	for(trackingRecHit_iterator rHit = trans_track.recHitsBegin();
	    rHit != trans_track.recHitsEnd(); ++rHit) {
	  DetId rHitid = (*rHit)->geographicalId();
	  if(!(rHitid.det() == DetId::Muon && rHitid.subdetId() == MuonSubdetId::DT)) continue;
	  DTChamberId wId(rHitid.rawId());
	  if(chAlrUsed.find(wId) !=  chAlrUsed.end()) continue;
	  chAlrUsed.insert(wId);
	  LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency") << "   " << wId << endl;
	}
      }

      // Get the layer on which the seed relies
      DetId id = trans_track.track().innerDetId();
      const DetLayer *initialLayer = theService->detLayerGeometry()->idToLayer(id);

      TrajectoryStateOnSurface init_fs = trans_track.innermostMeasurementState();
      FreeTrajectoryState *init_fs_free = init_fs.freeState();

      //get the list of compatible layers
      vector<const DetLayer*> layer_list = compatibleLayers(initialLayer,*init_fs_free,alongMomentum);
      vector<const DetLayer*> layer_list_2 = compatibleLayers(initialLayer,*init_fs_free,oppositeToMomentum);

      layer_list.insert(layer_list.end(),layer_list_2.begin(),layer_list_2.end());
      
      set<DTChamberId> alreadyCheckedCh;

      //loop over the list of compatible layers
      for(int i=0;i< (int)layer_list.size();i++) {

	//propagate the track to the i-th layer
	TrajectoryStateOnSurface tsos = propagator()->propagate(init_fs,layer_list.at(i)->surface()); 
	if(!tsos.isValid()) continue;

	//determine the chambers kinematically compatible with the track on the i-th layer
	vector<DetWithState> dss = layer_list.at(i)->compatibleDets(tsos, *propagator(), *theEstimator);
	
	if(dss.size() == 0) continue;

	// get the first det (it's the most compatible)
	const DetWithState detWithState = dss.front();
	const DetId idDetLay = detWithState.first->geographicalId(); 

	// check if this is a DT and the track has the needed quality
	if(!chamberSelection(idDetLay,trans_track)) continue;

	DTChamberId DTid = (DTChamberId) idDetLay;

	// check if the chamber has already been counted
	if(alreadyCheckedCh.find(DTid) != alreadyCheckedCh.end()) continue;
	alreadyCheckedCh.insert(DTid);
	
	// get the compatible measurements
	MeasurementContainer detMeasurements_initial = theMeasurementExtractor->measurements(layer_list.at(i),
											     detWithState.first,
											     detWithState.second,
											     *theEstimator, event);
	LogTrace("DTDQM|DTMonitorModule|DTChamberEfficiency") << "     chamber: " << DTid
							      << " has: " << detMeasurements_initial.size() << " comp. meas." << endl;


	//we want to be more picky about the quality of the segments:
	//exclude the segments with less than 12 hits
	MeasurementContainer detMeasurements = segQualityCut(detMeasurements_initial);
	
	// get the histos for this chamber
	vector<MonitorElement *> histos =  histosPerW[DTid.wheel()+2];  
	// fill them
	if (detMeasurements_initial.size() != 0) histos[0]->Fill(DTid.sector(),DTid.station(),1.);
	if (detMeasurements.size() != 0) histos[1]->Fill(DTid.sector(),DTid.station(),1.);
	histos[2]->Fill(DTid.sector(),DTid.station(),1.);
	

      }
    }
  } else {
    LogInfo("DTDQM|DTMonitorModule|DTChamberEfficiency") << "[DTChamberEfficiency] Collection: " << theTracksLabel
							 << " is not valid!" << endl;
  }
  return;
}

bool DTChamberEfficiency::chamberSelection(const DetId& idDetLay, reco::TransientTrack& trans_track) const
{
  //check that we have a muon and that is a DT detector
  if(!(idDetLay.det() == DetId::Muon && idDetLay.subdetId() == MuonSubdetId::DT)) return false;

  if(trans_track.recHitsSize() == 2)
    if(trans_track.recHit(0)->geographicalId() == idDetLay ||
       trans_track.recHit(1)->geographicalId() == idDetLay) return false;

  return true;
}

//riempi una per ogni segmento e una per segmento sopra 12 hit

MeasurementContainer DTChamberEfficiency::segQualityCut(const MeasurementContainer seg_list) const
{

  MeasurementContainer result;

  for(MeasurementContainer::const_iterator mescont_Itr = seg_list.begin();
      mescont_Itr != seg_list.end(); ++mescont_Itr){

    //get the rechits of the segment
    TransientTrackingRecHit::ConstRecHitContainer recHit_list = mescont_Itr->recHit()->transientHits();

    //loop over the rechits and get the number of hits
    int nhit_seg(0);
    for(TransientTrackingRecHit::ConstRecHitContainer::const_iterator recList_Itr = recHit_list.begin();
	recList_Itr != recHit_list.end(); ++recList_Itr){

      nhit_seg += (int)(*recList_Itr)->transientHits().size();
    }

    DTChamberId tmpId = (DTChamberId)mescont_Itr->recHit()->hit()->geographicalId();

    if(tmpId.station() < 4 && nhit_seg >= 12) result.push_back(*mescont_Itr);
    if(tmpId.station() == 4 && nhit_seg >= 8) result.push_back(*mescont_Itr);
  }

  return result;
}

vector<const DetLayer*> DTChamberEfficiency::compatibleLayers(const DetLayer *initialLayer,
                                const FreeTrajectoryState& fts, PropagationDirection propDir)
{

vector<const DetLayer*> detLayers;

if(theNavigationType == "Standard"){
   // ask for compatible layers
   detLayers = initialLayer->compatibleLayers(fts,propDir);
    // I have to fit by hand the first layer until the seedTSOS is defined on the first rechit layer
   // In fact the first layer is not returned by initialLayer->compatibleLayers.
   detLayers.insert(detLayers.begin(),initialLayer);
   }
 else if (theNavigationType == "Direct"){

   DirectMuonNavigation navigation(ESHandle<MuonDetLayerGeometry>(&*theService->detLayerGeometry()));
   detLayers = navigation.compatibleLayers(fts,propDir);
     }
 else
   LogError("DTDQM|DTMonitorModule|DTChamberEfficiency") << "No Properly Navigation Selected!!"<<endl;

 return detLayers;
} 

inline ESHandle<Propagator> DTChamberEfficiency::propagator() const {
  return theService->propagator("SteppingHelixPropagatorAny");
}
