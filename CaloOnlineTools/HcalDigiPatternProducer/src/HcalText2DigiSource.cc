#include "CaloOnlineTools/HcalDigiPatternProducer/interface/HcalText2DigiSource.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h" 
#include "FWCore/Framework/interface/Event.h" 
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CondFormats/DataRecord/interface/HcalElectronicsMapRcd.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include "CLHEP/Random/RandFlat.h"

using namespace std;

HcalText2DigiSource::HcalText2DigiSource(const edm::ParameterSet & ps, edm::InputSourceDescription const& desc) 
  : ExternalInputSource(ps, desc)
{
  //read in configuration parameters...
  produces<HBHEDigiCollection>();
  produces<HFDigiCollection>();
  string file = fileNames()[0];
  file.erase(0,5);
  //cout << "FILENAME IS -> " << file << endl;
  bool zs = ps.getUntrackedParameter<bool>("zs");
  //cout << "zero supression? : " << zs << endl;
  input.open(file.c_str());
}

HcalText2DigiSource::~HcalText2DigiSource()
{
  input.close();
}

void HcalText2DigiSource::beginJob(edm::EventSetup const& eventSetup)
{
  edm::ESHandle<CaloGeometry> geometry;
  eventSetup.get<IdealGeometryRecord>().get(geometry);
  hbCells =  geometry->getValidDetIds(DetId::Hcal, HcalBarrel);
  heCells =  geometry->getValidDetIds(DetId::Hcal, HcalEndcap);
  hfCells =  geometry->getValidDetIds(DetId::Hcal, HcalForward);
  // cout << "beginJob -> got geometry" << endl;
}

bool HcalText2DigiSource::produce(edm::Event& e)
{
  std::auto_ptr<HBHEDigiCollection> hbheresult(new HBHEDigiCollection());
  std::auto_ptr<HFDigiCollection> hfresult(new HFDigiCollection());


  //read external data into a map of triggertower id vs. energy
  // format:  <start evt>
  //          <ieta> <iphi> <adc>
  //          <end   evt>
  // if file becomes invalid, return false
  //if (no more data) return false;

  if(!ReadEvent())
    {
      return false;
    }

  for(vector<DetId>::const_iterator itr = hbCells.begin(); itr != hbCells.end(); ++itr)
    {
      //if trigger tower id of cell is in map and depth == 1, create using energy
      HcalDetId id = HcalDetId(*itr);
      HBHEDataFrame frame = HBHEDataFrame(id);
      if (zs)
	{
	  if(FindEnergy(id) != 0)
	    {
	      ConstructDigi(&frame,FindEnergy(id));
	      hbheresult->push_back(frame);
	    }
	}
      else
	{
	  ConstructDigi(&frame,FindEnergy(id));
	  hbheresult->push_back(frame);
	}
    }

  for(vector<DetId>::const_iterator itr = heCells.begin(); itr != heCells.end(); ++itr)
    {
      HcalDetId id = HcalDetId(*itr);
      HBHEDataFrame frame = HBHEDataFrame(id);
      if (zs)
	{
	  if(FindEnergy(id) !=0)
	    {
	      ConstructDigi(&frame,FindEnergy(id));
	      hbheresult->push_back(frame);
	    }
	}
      else
	{
	  ConstructDigi(&frame,FindEnergy(id));
	  hbheresult->push_back(frame);
	}
    }

  for(vector<DetId>::const_iterator itr = hfCells.begin(); itr != hfCells.end(); ++itr)
    {
      HcalDetId id = HcalDetId(*itr);
      HFDataFrame frame = HFDataFrame(id);
      if (zs)
	{
	  if(FindEnergy(id) !=0)
	    {
	      ConstructDigi(&frame,FindEnergy(id));
	      hfresult->push_back(frame);
	    }
	}
      else
	{
	  ConstructDigi(&frame,FindEnergy(id));
	  hfresult->push_back(frame);
	}
    }

  //fill collections;
  e.put(hbheresult);
  e.put(hfresult);
  return true;
}

int HcalText2DigiSource::FindEnergy(HcalDetId id)
{
  //make sure that its depth == 1
  //make sure to put energy in right trig tower
  //for now, only output 1 number : find out if there are any cells which are always only in 1 tower
  vector<HcalTrigTowerDetId> towerids;
  towerids = theTrigTowerGeometry.towerIds(id);
  int output = 0;
  for(uint i=0; i < towerids.size(); i++)
    {
      if(eventMap.count(towerids[i]) == 1 && id.depth() == 1)
	{
	  output = eventMap.find(towerids[i])->second;
	}
      else
	{
	  output = 0;
	}
    }
  return output;
}

bool HcalText2DigiSource::ReadEvent()
{
  string line;
  string ieta,iphi,adc;
  // stringstream stream;
  eventMap.clear();
  if(input.good())
    {
      getline(input,line);
      while(line!="<startevt>")
	{
	  getline(input,line);
	  if(!input.good()) {return false;}
	}
      while(true)
	{
	  getline(input,line);
	  if(line=="<endevt>") {return true;}
	  if(!input.good()) {return false;}
	  stringstream stream;
	  stream.str(line);
	  stream >> ieta >> iphi >> adc;
	  HcalTrigTowerDetId trigid(atoi(ieta.c_str()),atoi(iphi.c_str()));
	  eventMap.insert(map<HcalTrigTowerDetId,int>::value_type(trigid,atoi(adc.c_str())));
	}
    }
  else
    {
      return false;
    }
}

void HcalText2DigiSource::ConstructDigi(HBHEDataFrame* frame, int adc)
{
  frame->setSize(10);
  frame->setPresamples(4);
  int theStartingCapId = RandFlat::shootInt(4);
  //int theStartingCapId = 0;
  for (int tbins = 0; tbins < 10; ++tbins)
    {
      int capId = (theStartingCapId+tbins)%4;
      if(tbins==4)
	{
	  frame->setSample(tbins, HcalQIESample(adc, capId, 0, 0));
	}
      else
	{
	  frame->setSample(tbins, HcalQIESample(0, capId, 0, 0));
	}
    }
}

void HcalText2DigiSource::ConstructDigi(HFDataFrame* frame, int adc)
{
  frame->setSize(6);
  frame->setPresamples(3);
  //int theStartingCapId = 0;
  int theStartingCapId = RandFlat::shootInt(4);
  for (int tbins = 0; tbins < 6; ++tbins)
    {
      int capId = (theStartingCapId+tbins)%4;
      if(tbins==3)
        {
          frame->setSample(tbins, HcalQIESample(adc, capId, 0, 0));
        }
      else
        {
          frame->setSample(tbins, HcalQIESample(0, capId, 0, 0));
        }
    }
}
