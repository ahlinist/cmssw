/** \file
 * 
 * Authors: N Ippolito and S Durkin
 * 
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


#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace edm;
using namespace std;


namespace test{

  class CSCEventDisplay: public EDAnalyzer{
  
  public:

    CSCEventDisplay(const edm::ParameterSet& ps){
      init=0;
      // edm::FileInPath fp = ps.getParameter<edm::FileInPath>("theMappingFile");
      // theMappingFile = fp.fullPath();
      //string theMappingFile = pset.getParameter<std::string>("theMappingFile");
      theMapping=CSCReadoutMappingFromFile(ps);

    }
 
    void analyze(const Event & e, const EventSetup& c){ 


      // start durkin stuff

      if(init==0){
        init=1;
        
	// theMapping=CSCReadoutMappingFromFile("/home/ippolito/CMSSW_0_6_0/src/RecoLocalMuon/CSCStandAlone/test/csc_slice_test_map.txt");
	chamb_const= Strip_Fit_Constants(); 
        // register all chambers
        int dmb_val[9]={1,2,3,4,5,7,8,9,10};
        for(int vmecrate=0;vmecrate<4;vmecrate++){
        for(int i=0;i<9;i++){
          int dmb=dmb_val[i];  
          chamb_const.Register(theMapping,vmecrate,dmb);
        }
        }
      }
      // end durkin stuff 

      cout << "--- Run: " << e.id().run()
	   << " Event: " << e.id().event() << endl;
      Handle<FEDRawDataCollection> rawdata;
      //e.getByLabel("DaqSource", rawdata);
      e.getByType(rawdata);
      for (int i=FEDNumbering::getCSCFEDIds().first;
           i<=FEDNumbering::getCSCFEDIds().second; ++i){ //for each of our DCCs
	const FEDRawData& data = rawdata->FEDData(i);
	if(size_t size=data.size()) {
	  cout << "FED# " << i << " " << size << endl;
          if(i>0){
            unsigned short * buf = (unsigned short *)data.data()+8;
            for(int k=0;k<100;k++)printf(" %d %04x\n",k,buf[k]&0xffff); 
            CSCDDUEventData dduEvent(buf);
            const std::vector<CSCEventData> & cscData = dduEvent.cscData(); 
            int lvl1num=dduEvent.header().lvl1num();
            printf(" Level 1 Number %d \n",lvl1num);


    // start durkin stuff  
	    printf(" cscData.size() %d \n",cscData.size());
   EvtDisp ed;   // start event display
   ed.plotMEs_start(lvl1num);
   ed.plotMEs_lvl1num(lvl1num);
   ed.plotMEs_backgroundxy();
   ed.plotMEs_backgroundz();
   /* for EvtDisp plotting in rz get an average angle */ 
   int nnalct=0;
   float aangl=0; 
   for (unsigned k=0; k<cscData.size(); ++k) { 
      int vmecrate = cscData[k].dmbHeader().crateID(); 
      int dmb = cscData[k].dmbHeader().dmbID();
      int tmb = -1;
      int endcap = -1;
      int station = -1;
      int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
      CSCDetId cid(id);   
      nnalct=nnalct+1;
      aangl=aangl+ed.plotMEs_angl(cid.station(),cid.ring(),cid.chamber());
   }
   if(nnalct>0){
      aangl=aangl/nnalct;
   }
    
   /* main loop including track finding and EvtDisp */

   for (unsigned k=0; k<cscData.size(); ++k) {
      int vmecrate = cscData[k].dmbHeader().crateID(); 
      int dmb = cscData[k].dmbHeader().dmbID();
      printf(" crateid cscid %d %d \n",vmecrate,dmb);
      int tmb = -1;
      int endcap = -1;
      int station = -1;
      int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
      CSCDetId cid(id);   
      ed.plotMEs_chamber(cid.station(),cid.ring(),cid.chamber());
      printf(" station %d ring %d chamber %d \n",cid.station(),cid.ring(),cid.chamber());
      AnoTrkFnd atrk(cscData[k]);
      CatTrkFnd ctrk(theMapping,cscData[k],chamb_const); 
      /* print out tracks and hits */
      printf(" number of anode tracks %d\n",atrk.size());
      for(int l=0;l<atrk.size();l++){
	printf("*** ano track %d \n",l);
	std::vector<int> x=atrk.atrk_layer(l);
	std::vector<int> y=atrk.atrk_wire(l);
	  for(unsigned m=0;m<x.size();m++){
	    printf(" %d %d %d \n",m,x[m],y[m]);
	  }       
      } 
      printf(" number of cathode tracks %d\n",ctrk.size()); 
      for(int l=0;l<ctrk.size();l++){
	printf("*** cat track %d \n",l);
	std::vector<int> x=ctrk.ctrk_layer(l);
	std::vector<float> y=ctrk.ctrk_hstrip(l);
	  for(unsigned m=0;m<x.size();m++){
	    printf(" %d %d %5.2f \n",m,x[m],y[m]);
	  }       
      }
      /* now plot hits on EvtDisp */
      for(int l=0;l<atrk.size();l++){
      for(int l=0;l<ctrk.size();l++){
	std::vector<int> ax=atrk.atrk_layer(l);
	std::vector<int> ay=atrk.atrk_wire(l);
	std::vector<int> cx=ctrk.ctrk_layer(l);
	std::vector<float> cy=ctrk.ctrk_hstrip(l);
        for(unsigned am=0;am<ax.size();am++){
        for(unsigned cm=0;cm<cx.size();cm++){
          if(ax[am]==cx[cm]){
            ed.plotMEs_hit(cid.station(),cid.ring(),cid.chamber(),cy[cm],ay[am]);
            ed.plotMEs_hitz(cid.station(),cid.ring(),cid.chamber(),ax[am],cy[cm],ay[am],aangl);
	   
	  }
	}
        }
      }
      }
      ctrk.CatTrkTime(theMapping,cscData[k],chamb_const);
      TrkFit3D triD(theMapping,cscData[k],ctrk,atrk,chamb_const);
      CatTrkDisp ctd(lvl1num,theMapping,cscData[k],triD);

   }
   ed.plotMEs_end();
   ed.plotMEs_callroot(lvl1num);
   ed.~EvtDisp();
   // end durkin stuff


          }
	}
      }
    }

  private:

    int init;
    Strip_Fit_Constants chamb_const;
   CSCReadoutMappingFromFile theMapping; 


  };
DEFINE_FWK_MODULE(CSCEventDisplay)
}

