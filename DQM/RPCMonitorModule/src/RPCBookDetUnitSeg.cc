#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>


#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <DQM/RPCMonitorModule/interface/MuonSegmentEff.h>
#include <DQMOffline/Muon/interface/RPCBookFolderStructure.h>
#include "DQMServices/Core/interface/MonitorElement.h"

#include "Geometry/RPCGeometry/interface/RPCGeomServ.h"
std::map<std::string, MonitorElement*> MuonSegmentEff::bookDetUnitSeg(RPCDetId & detId,int nstrips,float stripw,float stripl) {
  
  std::map<std::string, MonitorElement*> meMap;
   
  RPCBookFolderStructure *  folderStr = new RPCBookFolderStructure(); //Anna
  std::string folder = "Muons/MuonSegEff/" +  folderStr->folderStructure(detId);

  dbe->setCurrentFolder(folder);

  RPCGeomServ RPCname(detId);
  std::string nameRoll = RPCname.name();

  char detUnitLabel[128];
  char layerLabel[128];

  sprintf(detUnitLabel ,"%s",nameRoll.c_str());
  sprintf(layerLabel ,"%s",nameRoll.c_str());

  char meId [128];
  char meTitle [128];

  float scale2D = 0.6;

  //Begin booking DT
  if(detId.region()==0) {
    
    sprintf(meId,"ExpectedOccupancyFromDT_%s",detUnitLabel);
    sprintf(meTitle,"ExpectedOccupancyFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips, 0, nstrips);
    
    sprintf(meId,"RealDetectedOccupancyFromDT_%s",detUnitLabel);
    sprintf(meTitle,"RealDetectedOccupancyFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips, 0, nstrips);
    
    sprintf(meId,"RPCDataOccupancyFromDT_%s",detUnitLabel);
    sprintf(meTitle,"RPCDataOccupancyFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips,0,nstrips);
    
    sprintf(meId,"BXDistribution_%s",detUnitLabel);
    sprintf(meTitle,"BXDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 11,-5.5, 5.5);

    sprintf(meId,"Signal_BXDistribution_%s",detUnitLabel);
    sprintf(meTitle,"Signal_BXDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 11,-5.5, 5.5);

    sprintf(meId,"CLSDistribution_%s",detUnitLabel);
    sprintf(meTitle,"CLSDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 10,0.5,10.5); //ESTO EN REALIDAD ES 10, no olvidar cambiar!

    sprintf(meId,"BXYDistribution_%s",detUnitLabel);
    sprintf(meTitle,"BXYDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle,7,-3.5,3.5,20,0,stripl);   

    //New 2D and more

    sprintf(meId,"ExpectedOccupancy2DFromDT_%s",detUnitLabel);
    sprintf(meTitle,"ExpectedOccupancy2DFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle, 
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);

    sprintf(meId,"RPCDataOccupancy2DFromDT_%s",detUnitLabel);
    sprintf(meTitle,"RPCDataOccupancy2DFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle, 
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);
    
    sprintf(meId,"Inefficiency2DFromDT_%s",detUnitLabel);
    sprintf(meTitle,"Inefficiency2DFromDT_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle, 
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);
    
    sprintf(meId,"RPCResidualsFromDT_Clu1_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromDT_Clu1_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20,20);

    sprintf(meId,"RPCResidualsFromDT_Clu2_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromDT_Clu2_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20,20);

    sprintf(meId,"RPCResidualsFromDT_Clu3_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromDT_Clu3_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20,20);
    
    sprintf(meId,"RPCResidualsFromDT_Other_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromDT_Other_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20,20);

  }else{
    //std::cout<<"Booking for the EndCap"<<detUnitLabel<<std::endl;

    sprintf(meId,"ExpectedOccupancyFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"ExpectedOccupancyFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips,0,nstrips);
    
    sprintf(meId,"RealDetectedOccupancyFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"RealDetectedOccupancyFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips,0, nstrips);
    
    sprintf(meId,"RPCDataOccupancyFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"RPCDataOccupancyFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, nstrips, 0,nstrips);
    
    sprintf(meId,"BXDistribution_%s",detUnitLabel);
    sprintf(meTitle,"BXDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 11,-5.5, 5.5);

    sprintf(meId,"Signal_BXDistribution_%s",detUnitLabel);
    sprintf(meTitle,"Signal_BXDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 11,-5.5, 5.5);

    sprintf(meId,"CLSDistribution_%s",detUnitLabel);
    sprintf(meTitle,"CLSDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle, 10,0.5,10.5);

    sprintf(meId,"BXYDistribution_%s",detUnitLabel);
    sprintf(meTitle,"BXYDistribution_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle,7,-3.5,3.5,20,0,stripl);   
    
    //New 2D and more

    sprintf(meId,"ExpectedOccupancy2DFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"ExpectedOccupancy2DFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle, 
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);
    
    sprintf(meId,"RPCDataOccupancy2DFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"RPCDataOccupancy2DFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle,
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);
    
    sprintf(meId,"Inefficiency2DFromCSC_%s",detUnitLabel);
    sprintf(meTitle,"Inefficiency2DFromCSC_for_%s",layerLabel);
    meMap[meId] = dbe->book2D(meId, meTitle, 
			      2*(int)(scale2D*nstrips*stripw),-scale2D*nstrips*stripw,scale2D*nstrips*stripw,
			      2*(int)(scale2D*stripl),-scale2D*stripl,scale2D*stripl);
    
    sprintf(meId,"RPCResidualsFromCSC_Clu1_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromCSC_Clu1_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20.,20.);

    sprintf(meId,"RPCResidualsFromCSC_Clu2_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromCSC_Clu2_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20.,20.);
    
    sprintf(meId,"RPCResidualsFromCSC_Clu3_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromCSC_Clu3_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20.,20.);
   
    sprintf(meId,"RPCResidualsFromCSC_Other_%s",detUnitLabel);
    sprintf(meTitle,"RPCResidualsFromCSC_Other_%s",layerLabel);
    meMap[meId] = dbe->book1D(meId, meTitle,101,-20.,20.);
   
  }
  return meMap;
}



