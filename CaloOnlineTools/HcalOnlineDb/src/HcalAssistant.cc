// -*- C++ -*-
//
// Package:     HcalOnlineDb
// Class  :     HcalAssistant
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Gena Kukartsev, kukarzev@fnal.gov
//         Created:  Thu Jul 16 11:39:22 CEST 2009
// $Id: HcalAssistant.cc,v 1.1 2009/07/16 16:29:35 kukartse Exp $
//


#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "CaloOnlineTools/HcalOnlineDb/interface/HcalAssistant.h"
#include "CaloOnlineTools/HcalOnlineDb/interface/ConnectionManager.h"
#include "CaloOnlineTools/HcalOnlineDb/interface/ConfigurationDatabaseException.hh"
#include "xgi/Utils.h"
#include "toolbox/string.h"
#include "occi.h"

using namespace std;
using namespace oracle::occi;

HcalAssistant::HcalAssistant()
{
  addQuotes();
  srand(time(0));
  listIsRead = false;
}


HcalAssistant::~HcalAssistant()
{
}


int HcalAssistant::addQuotes(){
  quotes.push_back("Fear is the path to the Dark Side...");
  quotes.push_back("You don't know the power of the Dark Side...");
  quotes.push_back("You must learn the ways of the Force!");
  quotes.push_back("Where's the money, Lebowski?!");
  quotes.push_back("You see what happens when you find a stranger in the Alps!!!?");
  quotes.push_back("You hear this? This is the sound of inevitability. This is the sound of your death. Goodbye, mr. Anderson");
  quotes.push_back("Welcome to the desert of the Real");
  quotes.push_back("In Tyler we trust");
  quotes.push_back("How about a little snack?..Let's have a snack now, we can get friendly later");
  quotes.push_back("Is he human? Hey, no need for name calling!");
  quotes.push_back("Frankly, my dear, I don't give a damn");
  quotes.push_back("I've a feeling we're not in Kansas anymore");
  quotes.push_back("What we've got here is failure to communicate");
  quotes.push_back("I love the smell of napalm in the morning!");
  quotes.push_back("I see stupid people");
  quotes.push_back("Stella! Hey, Stella!");
  quotes.push_back("Houston, we have a problem");
  quotes.push_back("Mrs. Robinson, you're trying to seduce me. Aren't you?");
  quotes.push_back("I feel the need - the need for speed!");
  quotes.push_back("He's got emotional problems. What, beyond pacifism?");
}



std::string HcalAssistant::getRandomQuote(){
  int _quotes_array_size = quotes.size();
  int _num = rand()%_quotes_array_size;
  return quotes[_num];
}


std::string HcalAssistant::getUserName(void){
  struct passwd * _pwd = getpwuid(geteuid());
  std::string _name(_pwd->pw_name);
  return _name;
}



int HcalAssistant::getGeomId(HcalSubdetector _det, int _ieta, int _iphi, int _depth){
  int _geomId = 
    _det
    +   10 * _depth
    +  100 * _iphi
    +10000 * abs(_ieta);
  if (_ieta<0){
    _geomId = -_geomId;
  }
  return _geomId;
}


int HcalAssistant::getHcalIeta(int _geomId){
  int _ieta = _geomId/10000;
  return _ieta;
}


int HcalAssistant::getHcalIphi(int _geomId){
  int _iphi = abs( _geomId % 10000 )/100;
  return _iphi;
}


int HcalAssistant::getHcalDepth(int _geomId){
  int _depth = abs( _geomId % 100 ) / 10;
  return _depth;
}


HcalSubdetector HcalAssistant::getHcalSubdetector(int _geomId){
  int _det = abs(_geomId) % 10;
  if (_det==1) return HcalBarrel;
  if (_det==2) return HcalEndcap;
  if (_det==3) return HcalOuter;
  if (_det==4) return HcalForward;
  if (_det==5) return HcalTriggerTower;
  if (_det==0) return HcalEmpty;
  else return HcalOther;
}


HcalSubdetector HcalAssistant::getSubdetector(std::string _det){
  if      ( _det.find("HB") != std::string::npos ) return HcalBarrel;
  else if ( _det.find("HE") != std::string::npos ) return HcalEndcap;
  else if ( _det.find("HF") != std::string::npos ) return HcalForward;
  else if ( _det.find("HO") != std::string::npos ) return HcalOuter;
  else return HcalOther;
}

std::string HcalAssistant::getSubdetectorString(HcalSubdetector _det){
  std::string sDet;
  if      ( _det==HcalBarrel)  sDet = "HB";
  else if ( _det==HcalEndcap)  sDet = "HE";
  else if      ( _det==HcalForward) sDet = "HF";
  else if      ( _det==HcalOuter)   sDet = "HO";
  else sDet = "other";
  return sDet;
}



int HcalAssistant::getListOfChannelsFromDb(){
  int _n_channels = 0;
  static ConnectionManager conn;
  conn.connect();
  std::string query = "select ";
  query            += "       channel_map_id,subdet,ieta,iphi,depth ";
  query            += "from ";
  query            += "       cms_hcl_hcal_cond.hcal_channels ";
  query            += "where ";
  query            += "       subdet='HB' or subdet='HE' or subdet='HF' or subdet='HO' ";
  try {
    oracle::occi::Statement* stmt = conn.getStatement(query);
    oracle::occi::ResultSet *rs = stmt->executeQuery();
    geom_to_rawid.clear();
    rawid_to_geom.clear();
    while (rs->next()) {
      _n_channels++;
      int _rawid = rs->getInt(1);
      int _geomId = getGeomId( getSubdetector(rs->getString(2)),
			       rs->getInt(3),
			       rs->getInt(4),
			       rs->getInt(5)
			       );
      geom_to_rawid.insert(std::pair<int, int>(_geomId, _rawid));
      rawid_to_geom.insert(std::pair<int, int>(_rawid, _geomId));
    }
    listIsRead = true;
  }
  catch (SQLException& e) {
    cerr << ::toolbox::toString("Oracle  exception : %s",e.getMessage().c_str()) << endl;
    XCEPT_RAISE(hcal::exception::ConfigurationDatabaseException,::toolbox::toString("Oracle  exception : %s",e.getMessage().c_str()));
  }
  conn.disconnect();
  return _n_channels;
}


int HcalAssistant::getGeomId(int _rawid){
  if (listIsRead){
    std::map<int,int>::const_iterator _geomId = rawid_to_geom.find(_rawid);
    if (_geomId!=rawid_to_geom.end()){
      return _geomId->second;
    }
    else return -1;
  }
  else return -1;
}



int HcalAssistant::getRawId(int _geomId){
  if (listIsRead){
    std::map<int,int>::const_iterator _rawid = geom_to_rawid.find(_geomId);
    if (_rawid!=geom_to_rawid.end()){
      return _rawid->second;
    }
    else return -1;
  }
  else return -1;
}


int HcalAssistant::getSubdetector(int _rawid){
  return getHcalSubdetector( getGeomId(_rawid) );
}


int HcalAssistant::getIeta(int _rawid){
  return getHcalIeta( getGeomId(_rawid) );
}



int HcalAssistant::getIphi(int _rawid){
  return getHcalIphi( getGeomId(_rawid) );
}



int HcalAssistant::getDepth(int _rawid){
  return getHcalDepth( getGeomId(_rawid) );
}



int HcalAssistant::getRawId(HcalSubdetector _det, int _ieta, int _iphi, int _depth){
  return getRawId( getGeomId(_det, _ieta, _iphi, _depth) );
}




