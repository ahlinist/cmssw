/** \file
 * Implementation of class RPCFileProcessor
 *
 *  $Date: 2006/07/21 14:31:45 $
 *  $Revision: 1.1 $
 *
 * \author Raffaello Trentadue & Alessandro Grelli
 */
#include "IORawData/RPCLocalDAQ/interface/Mapping.h"
#include <vector>
#include <fstream>
#include <map>
#include <iostream>

using namespace std;

Mapping::Mapping(){}

Mapping::Mapping(int wheel, int sector) : w(wheel), s(sector)
{
  std::map<std::string, std::vector<int> > lbnames;

  std::vector<int> missing_channel;
  missing_channel.push_back(0);
  lbnames["RB1IN"]  = missing_channel;

  missing_channel.clear();
  missing_channel.push_back(15-0);
  missing_channel.push_back(15-8);
  lbnames["RB1OUT"] = missing_channel;  

  missing_channel.clear();
  missing_channel.push_back(0);
  lbnames["RB22IN"] = missing_channel;

  missing_channel.clear();
  missing_channel.push_back(15-0);
  missing_channel.push_back(15-1);
  lbnames["RB22OUT"]=missing_channel;

  missing_channel.clear();
  missing_channel.push_back(0);
  lbnames["RB23IN"] =missing_channel;


  missing_channel.clear();
  missing_channel.push_back(15-0);
  missing_channel.push_back(15-1);
  lbnames["RB23OUT"]=missing_channel;

  missing_channel.clear();
  missing_channel.push_back(0);
  missing_channel.push_back(15);
  lbnames["RB3-"]    = missing_channel;

  missing_channel.clear();
  missing_channel.push_back(0); 
  missing_channel.push_back(15);
  lbnames["RB3+"]    = missing_channel;

  missing_channel.clear();
  missing_channel.push_back(0); 
  missing_channel.push_back(15);
  lbnames["RB3"]    = missing_channel;

// if (wheel==1 && sector==10){
    missing_channel.clear();
    missing_channel.push_back(0);
    missing_channel.push_back(1);
    missing_channel.push_back(14);
    missing_channel.push_back(15);
    lbnames["RB4+"]  = missing_channel;
    lbnames["RB4-"]  = missing_channel; 
 // }

  std::map<int, int> map;

  /* Start main loop: channel 0-96. Strip number depend on RPC tipology. For every channel we 
     have 1 strip. When the channel under investigation correspond to missing_channel ( see
     previous map ) the strip association return strip number 0. */
 
  for(std::map<std::string, std::vector<int> >::iterator i=lbnames.begin();
      i!=lbnames.end(); i++){
    int str = (16- (i->second).size())*6;
 
    for (int ich=0;ich<96;ich++){
      bool foundMissingChannel = false;
      for(std::vector<int>::iterator j = (i->second).begin();
	  j != (i->second).end(); j++){
	if(*j == ich%16) {
	  foundMissingChannel = true;
	  map[ich] = 0;
	  break;
	}
      }
      if(!foundMissingChannel){
       	map[ich] = str--;
      }
      
    }
  
    maps[i->first] = map;
  }
  
  
  /* for(std::map<std::string, std::map<int,int> >::iterator i=maps.begin();
     i!=maps.end(); i++){
     
     std::cout<<"LbName = "<<i->first<<std::endl;
     
     
     for(std::map<int,int>::iterator k=(i->second).begin();
     k!=(i->second).end(); k++){
     if( k->second !=0){
     
     }else if( k->second ==0){
     }
     std::cout<<"Channel = "<<k->first<<"  "<<"Strip = "<<k->second<<std::endl;
     }
     }*/
}


int 
Mapping::stripind(std::string lbname, int channel){
  std::cout<<"sonoqui0: "<<lbname<<" " <<channel<<std::endl;
  //--- strip extraction from lbname and channel --- 
 // std::string lbnameNew =  lbname.erase((lbname.size()-2),2);
  std::map<std::string, std::map<int, int> >::iterator i = maps.find(lbname);
  std::map<int, int>::iterator k  = (i->second).find(channel);
  int strip = k->second;
  
  return strip;
}

