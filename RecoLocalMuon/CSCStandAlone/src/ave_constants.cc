/** \file
 * 
 * 
 * $Date: 2006/02/06 15:15:59 $
 * $Revision: 1.4 $
 * \author N. Amapane - S. Argiro'
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
//#include "RecoLocalMuon/CSCStandAlone/interface/condbc.h"
#include "RecoLocalMuon/CSCStandAlone/interface/Strip_Fit_Constants.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/AnoTrkFnd.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkFit3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/CatTrkDisp.h"
#include "RecoLocalMuon/CSCStandAlone/interface/TrkLink3D.h"
#include "RecoLocalMuon/CSCStandAlone/interface/EvtDump.h"
#include "RecoLocalMuon/CSCStandAlone/interface/RootWriterCSCMonitor.h"

//#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TFile.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>

using namespace edm;
using namespace std;

namespace test{
  
  
  
  class ave_constants: public EDAnalyzer{
    
    
  public:


    ave_constants(const ParameterSet& pset)
    {
      string theMappingFile = pset.getParameter<std::string>("theMappingFile");
      theMapping=CSCReadoutMappingFromFile(theMappingFile);
	// theMapping=CSCReadoutMappingFromFile("/home/ippolito/CMSSW_0_8_0_pre2/RecoLocalMuon/CSCStandAlone/src/csc_slice_test_map.txt");
      // register all chambers                                                
      int dmb_val[9]={1,2,3,4,5,7,8,9,10};
      for(int vmecrate=0;vmecrate<4;vmecrate++){
	for(int i=0;i<9;i++){
	  int dmb=dmb_val[i];
	  chamb_const.Register(theMapping,vmecrate,dmb);
	}
      }
      init=0;
    }
    
    ~ave_constants(){}

    void analyze(const Event & e, const EventSetup& c){
      if(init==0){
      init=1;
      int dmb_val[9]={1,2,3,4,5,7,8,9,10};
      for(int vmecrate=0;vmecrate<4;vmecrate++){
	for(int i=0;i<9;i++){
	  int dmb=dmb_val[i];
	  int tmb = -1;
	  int endcap = -1;
	  int station = -1;
	  int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
	  CSCDetId cid(id);
	  int num_strips=chamb_const.num_strips(id);
          printf(" id %d num_strips %d %d/%d/%d \n",id,num_strips,cid.station(),cid.ring(),cid.chamber());
	  //float *gains=chamb_const.gain(id);
	  float *xtlkil=chamb_const.xtalk_intercept_left(id);
	  float *xtlkir=chamb_const.xtalk_intercept_right(id);
          float *xtlksl=chamb_const.xtalk_slope_left(id);
	  float *xtlksr=chamb_const.xtalk_slope_right(id);
	  float *as=chamb_const.sigma(id);
          nxave=0.;
          ave_xtlkil=0.0;
          ave_xtlkir=0.0;
          ave_xtlksl=0.0;
          ave_xtlksr=0.0;
          for(int j=0;j<12;j++){ave_sig[j]=0.0;nsave[j]=0.0;}
          for(int j=0;j<6*num_strips;j++){
            if(xtlkil[j]>-98.){
	      // printf(" j %d %f %f %f \n",j,nxave,ave_xtlkil,xtlkil[j]);
              ave_xtlkil=ave_xtlkil+xtlkil[j];
              ave_xtlkir=ave_xtlkir+xtlkir[j];
              ave_xtlksl=ave_xtlksl+xtlksl[j];
              ave_xtlksr=ave_xtlksr+xtlksr[j];
              nxave=nxave+1.0;
            }
            for(int k=0;k<12;k++){
              if(as[j*12+k]>-98.){
                ave_sig[k]=ave_sig[k]+as[j*12+k];
                nsave[k]=nsave[k]+1.0;                
	      }
            }
          } 
	  ave_xtlkil=ave_xtlkil/nxave;          
          ave_xtlkir=ave_xtlkir/nxave;
          ave_xtlksl=ave_xtlksl/nxave;
          ave_xtlksr=ave_xtlksr/nxave;
          for(int k=0;k<12;k++){
            ave_sig[k]=ave_sig[k]/nsave[k];
          }
          printf("id %d \n",id);
          printf(" xtlk %f %f %f %f \n",ave_xtlksl,ave_xtlkil,ave_xtlksr,ave_xtlkir);
          printf(" sig ");
          for(int k=0;k<12;k++)printf("%6.2f ",ave_sig[k]);printf("\n");
        }
      }
      }
    }
    
    /*      void analyze(const Event & e, const EventSetup& c){ 
      
      printf(" entering analyze \n");      
      
      } */ 
    
  private:
    
    int init;
    Strip_Fit_Constants chamb_const;
    CSCReadoutMappingFromFile theMapping;
    double ave_xtlkil,ave_xtlkir,ave_xtlksl,ave_xtlksr;    
    double nxave;
    double ave_sig[12];
    double nsave[12];
  };
  DEFINE_FWK_MODULE(ave_constants)
    }
