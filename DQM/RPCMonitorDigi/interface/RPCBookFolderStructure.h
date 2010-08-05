/** \class RPCBookFolderStructure
 *
 *  $Date: 2009/01/09 17:41:30 $
 *  $Revision: 1.3 $
 * \author Anna Cimmino (INFN Napoli)
 *
 * Create folder structure for DQM histo saving
 */
#ifndef RPCBookFolderStructure_H
#define RPCBookFolderStructure_H

#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include<string>
#include <sstream>
class RPCBookFolderStructure {
   public:
      explicit RPCBookFolderStructure() {};
      ~RPCBookFolderStructure(){};

      std::string folderStructure(RPCDetId detId, std::string recHitType){ 
	
	std::stringstream myStream ;
	myStream.str("");
  
	if(detId.region() ==  0) 
	  myStream <<recHitType<<"/Barrel/Wheel_"<<detId.ring()<<"/sector_"<<detId.sector()<<"/station_"<<detId.station();
	else if(detId.region() == -1) 
	  myStream <<recHitType<<"/Endcap-/Disk_-"<<detId.station()<<"/ring_"<<detId.ring()<<"/sector_"<<detId.sector();
	else if(detId.region() ==  1) 
	  myStream <<recHitType<<"/Endcap+/Disk_"<<detId.station()<<"/ring_"<<detId.ring()<<"/sector_"<<detId.sector();
	else  myStream <<"Region "<<detId.region()<< "not found!!! --- ERROR";

      return myStream.str();
      }
};
#endif
