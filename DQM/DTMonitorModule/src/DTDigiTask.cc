 /*
 * \file DTDigiTask.cc
 * 
 * $Date: 2007/11/06 17:33:17 $
 * $Revision: 1.34 $
 * \author M. Zanetti - INFN Padova
 *
 */

#include <DQM/DTMonitorModule/interface/DTDigiTask.h>

// Framework
#include <FWCore/Framework/interface/EventSetup.h>

// Digis
#include <DataFormats/DTDigi/interface/DTDigi.h>
#include <DataFormats/DTDigi/interface/DTDigiCollection.h>
#include <DataFormats/MuonDetId/interface/DTLayerId.h>
#include <DataFormats/MuonDetId/interface/DTChamberId.h>

// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

// T0s
#include <CondFormats/DTObjects/interface/DTT0.h>
#include <CondFormats/DataRecord/interface/DTT0Rcd.h>
#include <CondFormats/DTObjects/interface/DTTtrig.h>
#include <CondFormats/DataRecord/interface/DTTtrigRcd.h>

#include "CondFormats/DataRecord/interface/DTStatusFlagRcd.h"
#include "CondFormats/DTObjects/interface/DTStatusFlag.h"

#include <stdio.h>
#include <sstream>
#include <math.h>

using namespace edm;
using namespace std;

DTDigiTask::DTDigiTask(const edm::ParameterSet& ps){
  
  debug = ps.getUntrackedParameter<bool>("debug", "false");
  if(debug)
    cout<<"[DTDigiTask]: Constructor"<<endl;

  maxTDCHits = ps.getUntrackedParameter<int>("maxTDCHitsPerChamber",30000);

  parameters = ps;
  
  dbe = edm::Service<DaqMonitorBEInterface>().operator->();

  if (ps.getUntrackedParameter<bool>("enableMonitorDaemon", "false")) {
    edm::Service<MonitorDaemon> daemon; 	 
    daemon.operator->();
  }


  dbe->setVerbose(1);

  syncNumTot = 0;
  syncNum = 0;

}


DTDigiTask::~DTDigiTask(){

  if(debug)
    cout << "DTDigiTask: analyzed " << nevents << " events" << endl;

}

void DTDigiTask::endJob(){

  if(debug)
    cout<<"[DTDigiTask] endjob called!"<<endl;
  
  dbe->rmdir("DT/DTDigiTask");
}

void DTDigiTask::beginJob(const edm::EventSetup& context){

  if(debug)
    cout<<"[DTDigiTask]: BeginJob"<<endl;

  nevents = 0;

  // Get the geometry
  context.get<MuonGeometryRecord>().get(muonGeom);

  // tTrig 
  if (parameters.getUntrackedParameter<bool>("readDB", true)) 
    context.get<DTTtrigRcd>().get(tTrigMap);
  // t0s 
  if (parameters.getParameter<bool>("performPerWireT0Calibration")) 
    context.get<DTT0Rcd>().get(t0Map);
  // tMax (not yet from the DB)
  tMax = parameters.getParameter<int>("defaultTmax");

}


void DTDigiTask::beginLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& context) {

  if(debug)
    cout<<"[DTDigiTask]: Begin of LS transition"<<endl;

  if(lumiSeg.id().luminosityBlock()%parameters.getUntrackedParameter<int>("ResetCycle", 3) == 0) {
    if(debug)
      cout<<"[DTDigiTask]: Reset at the LS transition : "<<lumiSeg.id().luminosityBlock()<<endl;
    for(map<string, map<uint32_t, MonitorElement*> > ::const_iterator histo = digiHistos.begin();
	histo != digiHistos.end();
	histo++) {
      for(map<uint32_t, MonitorElement*> ::const_iterator ht = (*histo).second.begin();
	  ht != (*histo).second.end();
	  ht++) {
	(*ht).second->Reset();
      }
    }
  }

  
}




void DTDigiTask::bookHistos(const DTSuperLayerId& dtSL, string folder, string histoTag) {

  if (debug) cout<<"[DTDigiTask]: booking"<<endl;

  stringstream wheel; wheel << dtSL.wheel();	
  stringstream station; station << dtSL.station();	
  stringstream sector; sector << dtSL.sector();	
  stringstream superLayer; superLayer << dtSL.superlayer();

  dbe->setCurrentFolder("DT/DTDigiTask/Wheel" + wheel.str() +
			"/Station" + station.str() +
			"/Sector" + sector.str() + "/" + folder);

  if (debug) {
    cout<<"[DTDigiTask]: folder "<< "DT/DTDigiTask/Wheel" + wheel.str() +
      "/Station" + station.str() +
      "/Sector" + sector.str() + "/" + folder<<endl;
    cout<<"[DTDigiTask]: histoTag "<<histoTag<<endl;
  }
  string histoName = histoTag 
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str() 
    + "_SL" + superLayer.str(); 

  if (debug) cout<<"[DTDigiTask]: histoName "<<histoName<<endl;

  if ( parameters.getUntrackedParameter<bool>("readDB", false) ) 
    tTrigMap->slTtrig( dtSL, tTrig, tTrigRMS); 
  else tTrig = parameters.getParameter<int>("defaultTtrig");
  

  if ( folder == "TimeBoxes") {
    string histoTitle = histoName + " (TDC Counts)";
    int timeBoxGranularity = parameters.getUntrackedParameter<int>("timeBoxGranularity",4);

    if (!parameters.getUntrackedParameter<bool>("readDB", true)) {
      int maxTDCCounts = 6400 * parameters.getUntrackedParameter<int>("tdcRescale", 1);
      (digiHistos[histoTag])[dtSL.rawId()] = 
	dbe->book1D(histoName,histoTitle, maxTDCCounts/timeBoxGranularity, 0, maxTDCCounts);
    }    
    else {
      (digiHistos[histoTag])[dtSL.rawId()] = 
	dbe->book1D(histoName,histoTitle, 2*tMax/timeBoxGranularity, tTrig-tMax, tTrig+2*tMax);
    }
  }

  if ( folder == "CathodPhotoPeaks" ) 
    (digiHistos[histoTag])[dtSL.rawId()] = dbe->book1D(histoName,histoName,500,0,1000);

  
}


void DTDigiTask::bookHistos(const DTChamberId& dtCh, string folder, string histoTag) {

  if (debug) cout<<"[DTDigiTask]: booking"<<endl;
  
  stringstream wheel; wheel << dtCh.wheel();	
  stringstream station; station << dtCh.station();	
  stringstream sector; sector << dtCh.sector();

  dbe->setCurrentFolder("DT/DTDigiTask/Wheel" + wheel.str() +
			"/Station" + station.str() +
			"/Sector" + sector.str() + "/" + folder);

  if (debug){
    cout<<"[DTDigiTask]: folder "<< "DT/DTDigiTask/Wheel" + wheel.str() +
      "/Station" + station.str() +
      "/Sector" + sector.str() + "/" + folder<<endl;
    cout<<"[DTDigiTask]: histoTag "<<histoTag<<endl;
  }
  
  string histoName = histoTag 
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str(); 
  
  if (debug) cout<<"[DTDigiTask]: histoName "<<histoName<<endl;
  
  if (folder == "Occupancies")    {
    
    const DTChamber* dtchamber = muonGeom->chamber(dtCh);
    const std::vector<const DTSuperLayer*> dtSupLylist = dtchamber->superLayers();
    std::vector<const DTSuperLayer*>::const_iterator suly = dtSupLylist.begin();
    std::vector<const DTSuperLayer*>::const_iterator sulyend = dtSupLylist.end();
    
    int nWires = 0;
    int firstWire = 0;
    int nWires_max = 0;
    
    while(suly != sulyend) {
      const std::vector<const DTLayer*> dtLyList = (*suly)->layers();
      std::vector<const DTLayer*>::const_iterator ly = dtLyList.begin();
      std::vector<const DTLayer*>::const_iterator lyend = dtLyList.end();
      stringstream superLayer; superLayer << (*suly)->id().superlayer();
      
      while(ly != lyend) {
	nWires = muonGeom->layer((*ly)->id())->specificTopology().channels();
	firstWire = muonGeom->layer((*ly)->id())->specificTopology().firstChannel();
	stringstream layer; layer << (*ly)->id().layer();
	string histoName_layer = histoName + "_SL" + superLayer.str()  + "_L" + layer.str();
	if(histoTag == "OccupancyAllHits_perL" 
	   || histoTag == "OccupancyNoise_perL"
	   || histoTag == "OccupancyInTimeHits_perL")
	  (digiHistos[histoTag])[(*ly)->id().rawId()] = dbe->book1D(histoName_layer,histoName_layer,nWires,firstWire,nWires+firstWire);
	++ly;
	if((nWires+firstWire) > nWires_max) nWires_max = (nWires+firstWire);
	
      }
      ++suly;
    }
   
    if(histoTag != "OccupancyAllHits_perL" 
	   && histoTag != "OccupancyNoise_perL"
	   && histoTag != "OccupancyInTimeHits_perL"){
      (digiHistos[histoTag])[dtCh.rawId()] = dbe->book2D(histoName,histoName,nWires_max,1,nWires_max+1,12,0,12);
    
      for(int i=1;i<=12;i++) { 
	if(i<5){
	  stringstream layer;
	  string layer_name;
	  layer<<i;
	  layer>>layer_name;
	  string label="SL1: L"+layer_name;
	  (digiHistos[histoTag])[dtCh.rawId()]->setBinLabel(i,label,2);
	}
	else if(i>4 && i<9){
	  stringstream layer;
	  string layer_name;
	  layer<<(i-4);
	  layer>>layer_name;
	  string label="SL2: L"+layer_name;
	  (digiHistos[histoTag])[dtCh.rawId()]->setBinLabel(i,label,2);
	}
	else if(i>8 && i<13){
	  stringstream layer;
	  string layer_name;
	  layer<<(i-8);
	  layer>>layer_name;
	  string label="SL3: L"+layer_name;
	  (digiHistos[histoTag])[dtCh.rawId()]->setBinLabel(i,label,2);
	}
      }
    }

  }
}


void DTDigiTask::analyze(const edm::Event& e, const edm::EventSetup& c){
  
  nevents++;
  //  cout << "events:  " << nevents << endl;
  if (nevents%1000 == 0 && debug) {}
  
  edm::Handle<DTDigiCollection> dtdigis;
  e.getByLabel("dtunpacker", dtdigis);
  
  if ( !parameters.getUntrackedParameter<bool>("localrun", true) ) e.getByType(ltcdigis);
  
  bool checkNoisyChannels = parameters.getUntrackedParameter<bool>("checkNoisyChannels","false");
  ESHandle<DTStatusFlag> statusMap;
  if(checkNoisyChannels) {
    // Get the map of noisy channels
    c.get<DTStatusFlagRcd>().get(statusMap);
  }

  string histoTag;

  int tdcCount = 0;

  bool doSync = true;

  int digiSize = 0;
  for (DTDigiCollection::DigiRangeIterator dtLayerId_It=dtdigis->begin(); dtLayerId_It!=dtdigis->end(); ++dtLayerId_It){
    digiSize++;
  }
  
  if (digiSize == 0) {
    doSync = false;
    cout << "Event " << nevents << " empty." << endl;
  }

  if (doSync) {
    DTChamberId chDone = ((*(dtdigis->begin())).first).superlayerId().chamberId();
    
    DTDigiCollection::DigiRangeIterator dtLayerId_It;
    for (dtLayerId_It=dtdigis->begin(); dtLayerId_It!=dtdigis->end(); dtLayerId_It++){
      DTChamberId chId = ((*dtLayerId_It).first).superlayerId().chamberId();
      if (!(chId == chDone)) {
	hitMap.insert(make_pair(chDone,tdcCount));
	tdcCount = 0;
	chDone = chId;

      }
      
      
      tdcCount += (((*dtLayerId_It).second).second - ((*dtLayerId_It).second).first);
            
    }
    
    hitMap.insert(make_pair(chDone,tdcCount));
    
    bool eventSync = false;
    bool stat = false;
    std::map<DTChamberId,int>::iterator iter;
    for (iter = hitMap.begin(); iter != hitMap.end(); iter++) {
      
      stat = false;
      if ((iter->second) > maxTDCHits) { 
	stat = true;
	eventSync = true;
      }
      hitMapCheck.insert(make_pair((iter->first),stat));

    }
    hitMap.clear();
    
    if (eventSync) {
      cout << "Event " << nevents << " probably sync noisy: time box not filled! " << endl;
      syncNumTot++;
      syncNum++;
    }
    if (nevents%1000 == 0) {
      cout << (syncNumTot*100./nevents) << "% sync noise events since the beginning " << endl;
      cout << (syncNum*0.1) << "% sync noise events in the last 1000 events " << endl;
      syncNum = 0;
    }
  }

  bool isSyncNoisy;

  DTDigiCollection::DigiRangeIterator dtLayerId_It;
  for (dtLayerId_It=dtdigis->begin(); dtLayerId_It!=dtdigis->end(); ++dtLayerId_It){


    isSyncNoisy = false;
    if (doSync) {
      DTChamberId chId = ((*dtLayerId_It).first).superlayerId().chamberId();
      std::map<DTChamberId,bool>::iterator iterch;
      
      for (iterch = hitMapCheck.begin(); iterch != hitMapCheck.end(); iterch++) {
	if ((iterch->first) == chId) isSyncNoisy = iterch->second;

      }

    }

    for (DTDigiCollection::const_iterator digiIt = ((*dtLayerId_It).second).first;
	 digiIt!=((*dtLayerId_It).second).second; ++digiIt){

	bool isNoisy = false;
	bool isFEMasked = false;
	bool isTDCMasked = false;
	bool isTrigMask = false;
	bool isDead = false;
	bool isNohv = false;
	if(checkNoisyChannels) {
	  const DTWireId wireId(((*dtLayerId_It).first), (*digiIt).wire());
	  statusMap->cellStatus(wireId, isNoisy, isFEMasked, isTDCMasked, isTrigMask, isDead, isNohv);
	}
	
	

	// for clearness..
	const  DTSuperLayerId dtSLId = ((*dtLayerId_It).first).superlayerId();
	uint32_t indexSL = dtSLId.rawId();
	const  DTChamberId dtChId = dtSLId.chamberId(); 
	uint32_t indexCh = dtChId.rawId();
	int layer_number=((*dtLayerId_It).first).layer();
	int superlayer_number=dtSLId.superlayer();
	const  DTLayerId dtLId = (*dtLayerId_It).first;
	uint32_t indexL = dtLId.rawId();
	
	if ( parameters.getUntrackedParameter<bool>("readDB", false) ) 
	  tTrigMap->slTtrig( ((*dtLayerId_It).first).superlayerId(), tTrig, tTrigRMS); 
	else tTrig = parameters.getParameter<int>("defaultTtrig");
	
	int inTimeHitsLowerBound = int(round(tTrig)) - parameters.getParameter<int>("inTimeHitsLowerBound");
	int inTimeHitsUpperBound = int(round(tTrig)) + tMax + parameters.getParameter<int>("inTimeHitsUpperBound");
	
	float t0; float t0RMS;
	int tdcTime = (*digiIt).countsTDC();
	
	if (parameters.getParameter<bool>("performPerWireT0Calibration")) {
	  const DTWireId dtWireId(((*dtLayerId_It).first), (*digiIt).wire());
	  t0Map->cellT0(dtWireId, t0, t0RMS) ;
	  tdcTime += int(round(t0));
	}
	
	// avoid to fill TB and PhotoPeak with noise. Occupancy are anyway filled
	if (( !isNoisy ) && (!isSyncNoisy)) {
	  
	  // TimeBoxes per SL
	  histoTag = "TimeBox" + triggerSource();
	  if (digiHistos[histoTag].find(indexSL) == digiHistos[histoTag].end())
	    bookHistos( dtSLId, string("TimeBoxes"), histoTag );
	  (digiHistos.find(histoTag)->second).find(indexSL)->second->Fill(tdcTime);
	  
	  
	  // 2nd - 1st (CathodPhotoPeak) per SL
	  if ( (*digiIt).number() == 1 ) {
	    
	    DTDigiCollection::const_iterator firstDigiIt = digiIt;
	    firstDigiIt--;
	    
	    histoTag = "CathodPhotoPeak";
	    if (digiHistos[histoTag].find(indexSL) == digiHistos[histoTag].end())
	      bookHistos( dtSLId, string("CathodPhotoPeaks"), histoTag );
	    (digiHistos.find(histoTag)->second).find(indexSL)->second->Fill((*digiIt).countsTDC()-
									    (*firstDigiIt).countsTDC());
	  }
	}
	if (!isSyncNoisy) {


	  // only when tTrig is not available 
	  if (!parameters.getUntrackedParameter<bool>("readDB", true)) {
	    
	    //Occupancies per chamber & layer
	    histoTag = "OccupancyAllHits_perCh";
	    if (digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end())
	      bookHistos( dtChId, string("Occupancies"), histoTag );
	    (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill((*digiIt).wire(),
									    (layer_number+(superlayer_number-1)*4)-1);
	    histoTag = "OccupancyAllHits_perL";
	    if (digiHistos[histoTag].find(indexL) == digiHistos[histoTag].end())
	      bookHistos( dtChId, string("Occupancies"), histoTag );
	    (digiHistos.find(histoTag)->second).find(indexL)->second->Fill((*digiIt).wire());
	  }
	  
	  // after-Calibration jobs 
	  else {
	    
	    // Noise: Before tTrig
	    if (tdcTime < inTimeHitsLowerBound ) {
	      
	      //Occupancies Noise per chamber & layer
	      histoTag = "OccupancyNoise_perCh";
	      if (digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end())
		bookHistos( dtChId, string("Occupancies"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill((*digiIt).wire(),
									      (layer_number+(superlayer_number-1)*4)-1);
	      histoTag = "OccupancyNoise_perL";
	      if (digiHistos[histoTag].find(indexL) == digiHistos[histoTag].end())
		bookHistos( dtChId, string("Occupancies"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexL)->second->Fill((*digiIt).wire());
	    }
	    
	    // Physical hits: within the time window	
	    else if (tdcTime > inTimeHitsLowerBound && tdcTime < inTimeHitsUpperBound) { 
	      
	      //Occupancies Signal per chamber & layer
	      histoTag = "OccupancyInTimeHits_perCh";
	      if (digiHistos[histoTag].find(indexCh) == digiHistos[histoTag].end()) 
		bookHistos( dtChId, string("Occupancies"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexCh)->second->Fill((*digiIt).wire(),
									      (layer_number+(superlayer_number-1)*4)-1);
	      histoTag = "OccupancyInTimeHits_perL";
	      if (digiHistos[histoTag].find(indexL) == digiHistos[histoTag].end())
		bookHistos( dtChId, string("Occupancies"), histoTag );
	      (digiHistos.find(histoTag)->second).find(indexL)->second->Fill((*digiIt).wire());
	    }
	  }
	}
	
    }
  }
  
  hitMapCheck.clear();
}


string DTDigiTask::triggerSource() {

  string l1ASource;

  if ( !parameters.getUntrackedParameter<bool>("localrun", true) ){
    for (std::vector<LTCDigi>::const_iterator ltc_it = ltcdigis->begin(); ltc_it != ltcdigis->end(); ltc_it++){
      int otherTriggerSum=0;
      for (int i = 1; i < 6; i++)
	otherTriggerSum += int((*ltc_it).HasTriggered(i));
      
      if ((*ltc_it).HasTriggered(0) && otherTriggerSum == 0) 
	l1ASource = "DTonly";
      else if (!(*ltc_it).HasTriggered(0))
	l1ASource = "NoDT";
      else if ((*ltc_it).HasTriggered(0) && otherTriggerSum > 0)
	l1ASource = "DTalso";
    }
  }

  return l1ASource;

}

