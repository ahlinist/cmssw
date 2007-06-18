/*
 * 
 * $Date: 2007/05/15 17:21:35 $
 * $Revision: 1.3 $
 * \author A. Gresele - INFN Trento
 *
 */

#include "DQM/DTMonitorClient/src/DTNoiseTest.h"

// Framework
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <DQMServices/Core/interface/MonitorElementBaseT.h>

// Geometry
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTTopology.h"

#include <CondFormats/DTObjects/interface/DTTtrig.h>
#include <CondFormats/DataRecord/interface/DTTtrigRcd.h>

#include "CondFormats/DataRecord/interface/DTStatusFlagRcd.h"
#include "CondFormats/DTObjects/interface/DTStatusFlag.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

using namespace edm;
using namespace std;

DTNoiseTest::DTNoiseTest(const edm::ParameterSet& ps){

  edm::LogVerbatim ("noise") <<"[DTNoiseTest]: Constructor";  

  parameters = ps;
  
  dbe = edm::Service<DaqMonitorBEInterface>().operator->();
  dbe->setVerbose(1);
  dbe->setCurrentFolder("DT/TestNoise");
}


DTNoiseTest::~DTNoiseTest(){

edm::LogVerbatim ("noise") <<"DTNoiseTest: analyzed " << updates << " events";

}

void DTNoiseTest::endJob(){

  edm::LogVerbatim ("noise") <<"[DTNoiseTest] endjob called!";
  
  if ( parameters.getUntrackedParameter<bool>("writeHisto", true) ) 
    dbe->save(parameters.getUntrackedParameter<string>("outputFile", "DTNoiseTest.root"));
  
  dbe->rmdir("DT/TestNoise");
}

void DTNoiseTest::beginJob(const edm::EventSetup& context){

  edm::LogVerbatim ("noise") <<"[DTNoiseTest]: BeginJob";

  updates = 0;
  nevents = 0;

  // Get the geometry
  context.get<MuonGeometryRecord>().get(muonGeom);

}


void DTNoiseTest::bookHistos(const DTChamberId & ch, string folder, string histoTag ) {

  stringstream wheel; wheel << ch.wheel();	
  stringstream station; station << ch.station();	
  stringstream sector; sector << ch.sector();	

  dbe->setCurrentFolder("DT/TestNoise/" + folder);

  string histoName =  histoTag + "W" + wheel.str() + "_St" + station.str() + "_Sec" + sector.str(); 
 
  if (folder == "NoiseAverage")
  (histos[histoTag])[ch.rawId()] = dbe->book1D(histoName.c_str(),histoName.c_str(),3,0,3);
 
  if ( folder == "New Noisy Channels")
  (histos[histoTag])[ch.rawId()] = dbe->book1D(histoName.c_str(),histoName.c_str(),3,0,3);
  
}


void DTNoiseTest::bookHistos(const DTLayerId & lId, int nWires, string folder, string histoTag) {

  stringstream wheel; wheel << lId.superlayerId().wheel();
  stringstream station; station << lId.superlayerId().station();	
  stringstream sector; sector << lId.superlayerId().sector();
  stringstream superLayer; superLayer << lId.superlayerId().superlayer();
  stringstream layer; layer << lId.layer();

  string histoName = histoTag + "_W" + wheel.str() + "_St" + station.str() + "_Sec" + sector.str() +  "_SL" + superLayer.str() +  "_L" + layer.str();

  dbe->setCurrentFolder("DT/TestNoise/" + folder +
			"/Wheel" + wheel.str() +
			"/Station" + station.str() +
			"/Sector" + sector.str());

  (histos[histoTag])[lId.rawId()] = dbe->book1D(histoName.c_str(),histoName.c_str(),nWires,0,nWires);

}



void DTNoiseTest::analyze(const edm::Event& e, const edm::EventSetup& context){

  // counts number of updats (online mode) or number of events (standalone mode)
  updates++;
  // if running in standalone perform diagnostic only after a reasonalbe amount of events
  if ( parameters.getUntrackedParameter<bool>("runningStandalone", false) &&
       updates%parameters.getUntrackedParameter<int>("diagnosticPrescale", 1000) != 0 ) return;

  edm::LogVerbatim ("noise") <<"[DTNoiseTest]: "<<updates<<" updates";

  ESHandle<DTStatusFlag> statusMap;
  context.get<DTStatusFlagRcd>().get(statusMap);
  
  context.get<DTTtrigRcd>().get(tTrigMap);
  float tTrig, tTrigRMS;

  string histoTag;
  // loop over chambers
  vector<DTChamber*>::const_iterator ch_it = muonGeom->chambers().begin();
  vector<DTChamber*>::const_iterator ch_end = muonGeom->chambers().end();

  for (; ch_it != ch_end; ++ch_it) {
    DTChamberId ch = (*ch_it)->id();
    vector<const DTSuperLayer*>::const_iterator sl_it = (*ch_it)->superLayers().begin(); 
    vector<const DTSuperLayer*>::const_iterator sl_end = (*ch_it)->superLayers().end();
	    
    MonitorElement * noiseME = dbe->get(getMEName(ch));
    if (noiseME) {
      MonitorElementT<TNamed>* ob = dynamic_cast<MonitorElementT<TNamed>*>(noiseME);
      if (ob) {
	TH2F * noiseHisto = dynamic_cast<TH2F*> (ob->operator->());

	// WARNING uncorrect normalization!! TO BE PROVIDED CENTRALLY
	nevents = (int) noiseHisto->GetEntries();	
	
	double normalization =0;
	
	if (noiseHisto) {
	  
	  float average=0;
	  float nOfChannels=0;
	  float noiseStatistics=0;
	  int newNoiseChannels=0;

	  for(; sl_it != sl_end; ++sl_it) {
	    const DTSuperLayerId & slID = (*sl_it)->id();
	    
	    tTrigMap->slTtrig(slID, tTrig, tTrigRMS);
	    if (tTrig==0) tTrig=1;
	    const double ns_s = 1e9*(32/25);
	    normalization = ns_s/float(tTrig*nevents);
	    
	    noiseHisto->Scale(normalization);
	    
	    // loop over layers
	    
	    for (int binY=(slID.superLayer()-1)*4+1 ; binY <= (slID.superLayer()-1)*4+4; binY++) {
	      
	      int Y = binY - 4*(slID.superLayer()-1);
	      
	      // the layer
	      
	      const DTLayerId theLayer(slID,Y);
	     
	      // loop over channels 
	      for (int binX=1; binX <= noiseHisto->GetNbinsX(); binX++) {
		
		if (noiseHisto->GetBinContent(binX,binY) > parameters.getUntrackedParameter<int>("HzThreshold", 300))
		  theNoisyChannels.push_back(DTWireId(theLayer, binX));
		  
		// get rid of the dead channels
		else {
		  average += noiseHisto->GetBinContent(binX,binY); 
		  nOfChannels++; 
		}
	      }
	    }
	    
	    if (nOfChannels) noiseStatistics = average/nOfChannels;
	    histoTag = "NoiseAverage";

	    if (histos[histoTag].find((*ch_it)->id().rawId()) == histos[histoTag].end()) bookHistos((*ch_it)->id(),string("NoiseAverage"), histoTag );
	    histos[histoTag].find((*ch_it)->id().rawId())->second->setBinContent(slID.superLayer(),noiseStatistics); 

	    for ( vector<DTWireId>::const_iterator nb_it = theNoisyChannels.begin();
		  nb_it != theNoisyChannels.end(); ++nb_it) {
	      
	      bool isNoisy = false;
	      bool isFEMasked = false;
	      bool isTDCMasked = false;
	      bool isTrigMask = false;
	      bool isDead = false;
	      bool isNohv = false;
	      statusMap->cellStatus((*nb_it), isNoisy, isFEMasked, isTDCMasked, isTrigMask, isDead, isNohv);
	    	      
	      if (!isNoisy) newNoiseChannels++;
	    }
	    theNoisyChannels.clear();
	    histoTag = "New Noisy Channels";
	    if (histos[histoTag].find((*ch_it)->id().rawId()) == histos[histoTag].end()) bookHistos((*ch_it)->id(),string("New Noisy Channels"), histoTag );
	    histos[histoTag].find((*ch_it)->id().rawId())->second->setBinContent(slID.superLayer(), newNoiseChannels);   
	  }
	}
      }
    }
    //To compute the Noise Mean test
    vector<const DTSuperLayer*>::const_iterator sl2_it = (*ch_it)->superLayers().begin(); 
    vector<const DTSuperLayer*>::const_iterator sl2_end = (*ch_it)->superLayers().end();
    for(; sl2_it != sl2_end; ++sl2_it) {
      DTSuperLayerId slID = (*sl2_it)->id();
      vector<const DTLayer*>::const_iterator l_it = (*sl2_it)->layers().begin(); 
      vector<const DTLayer*>::const_iterator l_end = (*sl2_it)->layers().end();
      for(; l_it != l_end; ++l_it) {
	
	DTLayerId lID = (*l_it)->id();
	MonitorElement * noisePerEventME = dbe->get(getMEName(lID));
	if (noisePerEventME) {
	  MonitorElementT<TNamed>* obPerEvent = dynamic_cast<MonitorElementT<TNamed>*>(noisePerEventME);
	  if (obPerEvent) {
	    TH2F * noiseHistoPerEvent = dynamic_cast<TH2F*> (obPerEvent->operator->());
	    int nWires = muonGeom->layer(lID)->specificTopology().channels();
	    double MeanNumerator=0, MeanDenominator=0;
	    histoTag = "MeanDigiPerEvent";
	    for (int w=1; w<=nWires; w++){
	      for(int numDigi=1; numDigi<=10; numDigi++){
		MeanNumerator+=(noiseHistoPerEvent->GetBinContent(w,numDigi)*(numDigi-1));
		MeanDenominator+=noiseHistoPerEvent->GetBinContent(w,numDigi);
	      }
	      double Mean=MeanNumerator/MeanDenominator;
	      cout<<"Mean: "<<Mean<<endl;
	      if (histos[histoTag].find((*l_it)->id().rawId()) == histos[histoTag].end()) bookHistos((*l_it)->id(),nWires, string("MeanDigiPerEvent"), histoTag );
	      histos[histoTag].find((*l_it)->id().rawId())->second->setBinContent(w, Mean);   
	    } 
	  }
	}
      }
    }
  }
  
  // Noise Mean test 
  cout<<"[DTNoiseTest]: Mean Noise Tests results"<<endl;
  histoTag = "MeanDigiPerEvent";
  string MeanCriterionName = parameters.getUntrackedParameter<string>("meanTestName","NoiseMeanInRange");
  for(map<uint32_t, MonitorElement*>::const_iterator hMean = histos[histoTag].begin();
  hMean != histos[histoTag].end();
      hMean++) {
      const QReport * theMeanQReport = (*hMean).second->getQReport(MeanCriterionName);
    if(theMeanQReport) {
    vector<dqm::me_util::Channel> badChannels = theMeanQReport->getBadChannels();
      for (vector<dqm::me_util::Channel>::iterator channel = badChannels.begin(); 
      channel != badChannels.end(); channel++) {
	cout<<"LayerId : "<<(*hMean).first<<" Bad mean channels: "<<(*channel).getBin()<<"  Contents : "<<(*channel).getContents()<<endl;
	cout << "-------- LayerId : "<<(*hMean).first<<"  "<<theMeanQReport->getMessage()<<" ------- "<<theMeanQReport->getStatus()<<endl; 
	}
      }
    }

  
  if (updates%parameters.getUntrackedParameter<int>("resultsSavingRate",10) == 0){
    if ( parameters.getUntrackedParameter<bool>("writeHisto", true) ) 
      dbe->save(parameters.getUntrackedParameter<string>("outputFile", "DTNoiseTest.root"));
  }
}

string DTNoiseTest::getMEName(const DTChamberId & ch) {
  
  stringstream wheel; wheel << ch.wheel();	
  stringstream station; station << ch.station();	
  stringstream sector; sector << ch.sector();	
  
  string folderTag = parameters.getUntrackedParameter<string>("folderTag", "Occupancies");
  string folderName = 
    "Collector/FU0/DT/DTDigiTask/Wheel" +  wheel.str() +
    "/Station" + station.str() +
    "/Sector" + sector.str() + "/" + folderTag + "/";
  
  string histoTag = parameters.getUntrackedParameter<string>("histoTag", "OccupancyNoise_perCh");
  string histoname = folderName + histoTag  
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str(); 
    
    
  return histoname;
  
}

  string DTNoiseTest::getMEName(const DTLayerId & ly) {
  
  stringstream wheel; wheel << ly.wheel();	
  stringstream station; station << ly.station();	
  stringstream sector; sector << ly.sector();
  stringstream superLayer; superLayer << ly.superlayer();
  stringstream layer; layer << ly.layer();
  
  string folderTag = parameters.getUntrackedParameter<string>("folderTagForDigiPerEventTest", "DigiPerEvent");
  string folderName = 
    "Collector/FU0/DT/DTDigiTask/Wheel" +  wheel.str() +
    "/Station" + station.str() +
    "/Sector" + sector.str() + "/" + folderTag + "/";
  
  string histoTag = parameters.getUntrackedParameter<string>("folderTagForDigiPerEventTest", "DigiPerEvent");
  string histoname = folderName + histoTag  
    + "_W" + wheel.str() 
    + "_St" + station.str() 
    + "_Sec" + sector.str()
    + "_SL" + superLayer.str()
    + "_L" + layer.str();
    
    
  return histoname;

}
