/** \file
 * 
 * Authors N Ippolito and S Durkin
 *
*/
#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/Framework/interface/EventProcessor.h>
#include <FWCore/Utilities/interface/ProblemTracker.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>


#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDCCHeader.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDCCEventData.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDDUHeader.h>
#include <EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>
#include "EventFilter/CSCRawToDigi/interface/CSCEventData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCAnodeData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCALCTHeader.h"
#include "EventFilter/CSCRawToDigi/interface/CSCCLCTData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCTMBData.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDDUTrailer.h"
#include "EventFilter/CSCRawToDigi/interface/CSCDMBHeader.h"

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/ParameterSet/interface/FileInPath.h>

#include "RecoLocalMuon/CSCStandAlone/interface/Strip_Fit_Constants.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/AnoTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkFit3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkLink3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDump.h"
#include "RecoLocalMuon/CSCStandAlone/interface/RootWriterCSCMonitor.h"



#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace edm;
using namespace std;

namespace test{
  
 
  
  class CSCMonitor: public EDAnalyzer{

   
  public:
    CSCMonitor(const ParameterSet& ps)
    {
      //string theMappingFile = pset.getParameter<std::string>("theMappingFile");
      //theMapping=CSCReadoutMappingFromFile(theMappingFile);
      theMapping=CSCReadoutMappingFromFile(ps);
      writer.setup(theMapping);
      
      chamb_const=Strip_Fit_Constants();
              
  // register all chambers                                                
    int dmb_val[9]={1,2,3,4,5,7,8,9,10};
    for(int vmecrate=0;vmecrate<4;vmecrate++){
      for(int i=0;i<9;i++){
        int dmb=dmb_val[i];
        chamb_const.Register(theMapping,vmecrate,dmb);
      }
    }
  }
    
    ~CSCMonitor()
    {
      
      writer.done();
      
      
     }
    
    void analyze(const Event & e, const EventSetup& c){ 
      
     
      
      // end durkin stuff 
      writer.ZeroTrigID();
      
      Handle<FEDRawDataCollection> rawdata;
      //e.getByLabel("DaqSource", rawdata);
      e.getByType(rawdata);
      for (int id=FEDNumbering::getCSCFEDIds().first;
	   id<=FEDNumbering::getCSCFEDIds().second; ++id){ //for each of our DCCs
	const FEDRawData& data = rawdata->FEDData(id);
        cout << " data.size() " << data.size() << endl; 
	if(size_t size=data.size()) {
	  cout << "FED# " << id << " " << size << endl;
          if(id>=750&&id<=757){
            unsigned short * buf = (unsigned short *)data.data();
            CSCDCCEventData dccEvent(buf);
            std::vector<CSCDDUEventData> & ddudata = dccEvent.dduData();
            
	    if(ddudata.size()==0)continue;
            // start durkin stuff 
           

            printf(" DDU data size %d \n",ddudata.size());
            for(unsigned iddu=0;iddu<ddudata.size();iddu++){
              const std::vector<CSCEventData> & cscData = ddudata[iddu].cscData();
              int lvl1num=ddudata[iddu].header().lvl1num();
	      printf(" Level 1 Number %d \n",lvl1num);
              printf(" ddu number %d cscData.size() %d \n",iddu,cscData.size());                                                        
	      for (unsigned k=0; k<cscData.size(); ++k) {
	        AnoTrkFnd atrk(cscData[k]);              // find Anode tracks
	        CatTrkFnd ctrk(theMapping,cscData[k],chamb_const);   // find Cathode tracks (1/2 strip)
	        ctrk.CatTrkTime(theMapping,cscData[k],chamb_const);             // Fit time Cathode track hits
	        TrkFit3D triD(theMapping,cscData[k],ctrk,atrk,chamb_const);  // full blown gatti+
	        // least squares
          	writer.FillTrigID(theMapping,cscData[k]);
		//evtdump.dump3(theMapping,cscData[k],ctrk,atrk,triD);
	        evtdump.print_head(lvl1num,theMapping,cscData[k],ctrk,atrk,triD);
	        evtdump.print_gatti(theMapping,cscData[k],ctrk,atrk,triD);
	        evtdump.print_3Dtrack_fits(theMapping,cscData[k],ctrk,atrk,triD);
		/*
	
                */
		
		writer.dump3_ROOT(theMapping,cscData[k],ctrk,atrk,triD);
		writer.print_raw_tracks_ROOT(theMapping,cscData[k],ctrk,atrk,triD);
		//writer.PlotTrigID();
              }
            }
	   
	    
          }
          
	}

      }
     
      writer.PlotTrigID();
    }
   
  private:
    
    int init;
    RootWriterCSCMonitor writer;
    Strip_Fit_Constants chamb_const;
    CSCReadoutMappingFromFile theMapping;
    EvtDump evtdump;
     
  };
  DEFINE_FWK_MODULE(CSCMonitor)
    }
