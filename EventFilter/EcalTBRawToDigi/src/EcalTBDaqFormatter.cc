/*  
 *
 *  $Date: 2007/02/22 10:40:58 $
 *  $Revision: 1.39 $
 *  \author  N. Marinelli IASA 
 *  \author G. Della Ricca
 *  \author G. Franzoni
 *  \author A. Ghezzi
 *
 */

#include "EcalTBDaqFormatter.h"
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include <DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h>
#include <DataFormats/EcalDigi/interface/EBDataFrame.h>
#include <DataFormats/EcalDigi/interface/EcalDigiCollections.h>

#include <EventFilter/EcalTBRawToDigi/interface/EcalDCCHeaderRuntypeDecoder.h>
#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveDigi.h>
#include <DataFormats/EcalDigi/interface/EcalTriggerPrimitiveSample.h>

#include "DCCDataParser.h"
#include "DCCEventBlock.h"
#include "DCCTowerBlock.h"
#include "DCCTCCBlock.h"
#include "DCCXtalBlock.h"
#include "DCCDataMapper.h"

using namespace edm;
using namespace std;

#include <iostream>

EcalTBDaqFormatter::EcalTBDaqFormatter () {

  LogDebug("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter";
  vector<ulong> parameters;
  parameters.push_back(10); // parameters[0] is the xtal samples 
  parameters.push_back(1);  // parameters[1] is the number of trigger time samples for TPG's
  parameters.push_back(68); // parameters[2] is the number of TT
  parameters.push_back(68); // parameters[3] is the number of SR Flags
  parameters.push_back(1);  // parameters[4] is the dcc id
  parameters.push_back(1);  // parameters[5] is the sr id
  parameters.push_back(1);  // parameters[6] is the tcc1 id
  parameters.push_back(2);  // parameters[7] is the tcc2 id
  parameters.push_back(3);  // parameters[8] is the tcc3 id
  parameters.push_back(4);  // parameters[9] is the tcc4 id

  theParser_ = new DCCDataParser(parameters);

}

void EcalTBDaqFormatter::interpretRawData(const FEDRawData & fedData , 
					  EBDigiCollection& digicollection, EcalPnDiodeDigiCollection & pndigicollection , 
					  EcalRawDataCollection& DCCheaderCollection, 
					  EBDetIdCollection & dccsizecollection , 
					  EcalTrigTowerDetIdCollection & ttidcollection , EcalTrigTowerDetIdCollection & blocksizecollection,
					  EBDetIdCollection & chidcollection , EBDetIdCollection & gaincollection, 
					  EBDetIdCollection & gainswitchcollection, EBDetIdCollection & gainswitchstaycollection, 
					  EcalElectronicsIdCollection & memttidcollection,  EcalElectronicsIdCollection &  memblocksizecollection,
					  EcalElectronicsIdCollection & memgaincollection,  EcalElectronicsIdCollection & memchidcollection,
					  EcalTrigPrimDigiCollection &tpcollection)
{


  const unsigned char * pData = fedData.data();
  int length = fedData.size();
  bool shit=true;
  unsigned int tower=0;
  int ch=0;
  int strip=0;

  LogDebug("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter::interpretRawData"
			      << "size " << length;
 

  // mean + 3sigma estimation needed when switching to 0suppressed data
  digicollection.reserve(kCrystals);
  pnAllocated = false;
  

  theParser_->parseBuffer( reinterpret_cast<ulong*>(const_cast<unsigned char*>(pData)), static_cast<ulong>(length), shit );
  
  vector< DCCEventBlock * > &   dccEventBlocks = theParser_->dccEvents();

  // Access each DCC block
  for( vector< DCCEventBlock * >::iterator itEventBlock = dccEventBlocks.begin(); 
       itEventBlock != dccEventBlocks.end(); 
       itEventBlock++){
    
    bool _displayParserMessages = false;
    if( (*itEventBlock)->eventHasErrors() && _displayParserMessages)
      {
        LogWarning("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter::interpretRawData"
				      << "errors found from parser... ";
        LogWarning("EcalTBRawToDigi") << (*itEventBlock)->eventErrorString();
        LogWarning("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter::interpretRawData"
				      << "... errors from parser notified";
      }
    // getting the fields of the DCC header
    EcalDCCHeaderBlock theDCCheader;
    //    theDCCheader.setId((*itEventBlock)->getDataField("FED/DCC ID"));      // at the moment used as fed id
    theDCCheader.setId(1);                                                                                    // forced to 1 since tb unpacker
    // int SMid(1);                                                                                                  // here forcing slot=1, as always in test beam
    // theDCCheader.setiEB(SMid);                                                                  //  iEB is physical slot in CMS ECAL barrael (1.. 36)
    theDCCheader.setRunNumber((*itEventBlock)->getDataField("RUN NUMBER"));
    short trigger_type = (*itEventBlock)->getDataField("TRIGGER TYPE");
    short zs = (*itEventBlock)->getDataField("ZS");
    short tzs = (*itEventBlock)->getDataField("TZS");
    short sr = (*itEventBlock)->getDataField("SR");
    

    if(trigger_type >0 && trigger_type <5){theDCCheader.setBasicTriggerType(trigger_type);}
    else{ LogWarning("EcalTBRawToDigiTriggerType") << "@SUB=EcalTBDaqFormatter::interpretRawData"
					<< "unrecognized TRIGGER TYPE: "<<trigger_type;}
    theDCCheader.setLV1((*itEventBlock)->getDataField("LV1"));
    theDCCheader.setBX((*itEventBlock)->getDataField("BX"));
    theDCCheader.setErrors((*itEventBlock)->getDataField("DCC ERRORS"));
    theDCCheader.setSelectiveReadout( sr );
    theDCCheader.setZeroSuppression( zs );
    theDCCheader.setTestZeroSuppression( tzs );
    theDCCheader.setSrpStatus((*itEventBlock)->getDataField("SR_CHSTATUS"));




    vector<short> theTCCs;
    for(int i=0; i<MAX_TCC_SIZE; i++){
      
      char TCCnum[20]; sprintf(TCCnum,"TCC_CHSTATUS#%d",i+1); string TCCnumS(TCCnum);
      theTCCs.push_back ((*itEventBlock)->getDataField(TCCnumS) );
    }
    theDCCheader.setTccStatus(theTCCs);


    vector< DCCTCCBlock * > tccBlocks = (*itEventBlock)->tccBlocks();
    
    for(    vector< DCCTCCBlock * >::iterator itTCCBlock = tccBlocks.begin(); 
	    itTCCBlock != tccBlocks.end(); 
	    itTCCBlock ++)
      {

	vector< pair<int,bool> > TpSamples = (* itTCCBlock) -> triggerSamples() ;
	// vector of 3 bits
	vector<int> TpFlags      = (* itTCCBlock) -> triggerFlags() ;
	
	// there have always to be 68 primitives and flags, per FED
	if (TpSamples.size()==68   && TpFlags.size()==68)
	  {
	    for(int i=0; i<((int)TpSamples.size()); i++)	
	      {
		
		int etaTT = (i)  / kTowersInPhi +1;
		int phiTT = (i) % kTowersInPhi +1;

		EcalTriggerPrimitiveSample theSample(TpSamples[i].first, TpSamples[i].second, TpFlags[i]);
		
		EcalTrigTowerDetId idtt(1, EcalBarrel, etaTT, phiTT, 0);
		EcalTriggerPrimitiveDigi thePrimitive(idtt);
		thePrimitive.setSize(1);                          // hard coded
		thePrimitive.setSample(0, theSample);
		
		tpcollection.push_back(thePrimitive);
		
		LogDebug("EcalTBRawToDigiTpg") << "@SUBS=EcalTBDaqFormatter::interpretRawData"
					    << "tower: " << (i+1) 
					    << " primitive: " << TpSamples[i].first
					    << " flag: " << TpSamples[i].second
					    << endl;
		LogDebug("EcalTBRawToDigiTpg") << "@SUBS=EcalTBDaqFormatter::interpretRawData"<<
		  "tower: " << (i+1) << " flag: " << TpFlags[i] << endl;
	      }// end loop on tower primitives
	    
	  }// end if
	else
	  {
	    LogWarning("EcalTBRawToDigiTpg") << "68 elements not found for TpFlags or TpSamples, collection will be empty";
	  }
      }  
    
    
    
    
    short TowerStatus[MAX_TT_SIZE+1];
    char buffer[20];
    vector<short> theTTstatus;
    for(int i=1;i<MAX_TT_SIZE+1;i++)
      { 
 	sprintf(buffer, "FE_CHSTATUS#%d", i);
 	string Tower(buffer);
 	TowerStatus[i]= (*itEventBlock)->getDataField(Tower);
	theTTstatus.push_back(TowerStatus[i]);
	//cout << "tower " << i << " has status " <<  TowerStatus[i] << endl;  
      }
    theDCCheader.setTriggerTowerStatus(theTTstatus);
    
    EcalDCCHeaderRuntypeDecoder theRuntypeDecoder;
    ulong DCCruntype = (*itEventBlock)->getDataField("RUN TYPE");
    theRuntypeDecoder.Decode(DCCruntype, &theDCCheader);
    //DCCHeader filled!
    DCCheaderCollection.push_back(theDCCheader);
    
    vector< DCCTowerBlock * > dccTowerBlocks = (*itEventBlock)->towerBlocks();
    LogDebug("EcalTBRawToDigi") << "@SUBS=EcalTBDaqFormatter::interpretRawData"
				<< "dccTowerBlocks size " << dccTowerBlocks.size();



    _expTowersIndex=0;_numExpectedTowers=0;
    for (int v=0; v<71; v++){
      _ExpectedTowers[v]=99999;
    }
    // staus==0:  tower expected; status==1: tower not expected
    for (int u=1; u< (kTriggerTowersAndMem+1); u++)
      {
	if(   TowerStatus[u] ==0   ) 
	  {_ExpectedTowers[_expTowersIndex]=u;
	    _expTowersIndex++;
	    _numExpectedTowers++;
	  }
      }
    // resetting counter of expected towers
    _expTowersIndex=0;
      
      
    // FIXME: dccID hard coded, for now
    unsigned dccID = 1-1;// at the moment SM is 1 by default (in DetID)

    // if number of dccEventBlocks NOT same as expected stop
    if (!      (dccTowerBlocks.size() == _numExpectedTowers)      )
      {
        // we probably always want to know if this happens
        LogWarning("EcalTBRawToDigiNumTowerBlocks") << "@SUB=EcalTBDaqFormatter::interpretRawData"
				      << "number of TowerBlocks found (" << dccTowerBlocks.size()
				      << ") differs from expected (" << _numExpectedTowers 
				      << ") skipping event"; 
	
        EBDetId idsm(1, 1 + 20 * dccID);
        dccsizecollection.push_back(idsm);

	return;
	
      }
      




    // Access the Tower block    
    for( vector< DCCTowerBlock * >::iterator itTowerBlock = dccTowerBlocks.begin(); 
         itTowerBlock!= dccTowerBlocks.end(); 
         itTowerBlock++){

      tower=(*itTowerBlock)->towerID();
      
      // checking if tt in data is the same as tt expected 
      // else skip tower and increment problem counter
	    
      // compute eta/phi in order to have iTT = _ExpectedTowers[_expTowersIndex]
      // for the time being consider only zside>0

      int etaTT = (_ExpectedTowers[_expTowersIndex]-1)  / kTowersInPhi +1;
      int phiTT = (_ExpectedTowers[_expTowersIndex]-1) % kTowersInPhi +1;


      EcalTrigTowerDetId idtt(1, EcalBarrel, etaTT, phiTT, 0);
    

      if (  !(tower == _ExpectedTowers[_expTowersIndex])	  )
        {	
          LogWarning("EcalTBRawToDigiTowerId") << "@SUBS=EcalTBDaqFormatter::interpretRawData"
					<< "TTower id found (=" << tower 
					<< ") different from expected (=" <<  _ExpectedTowers[_expTowersIndex] 
					<< ") " << (_expTowersIndex+1) << "th tower checked"; 
	    
          // report on failed tt_id
          ttidcollection.push_back(idtt);

          ++ _expTowersIndex;
          continue;	
        }// if TT id found  different than expected 
	



      /*********************************
       //    tt: 1 ... 68: crystal data
       *********************************/
      if (  0<  (*itTowerBlock)->towerID() &&
	    (*itTowerBlock)->towerID() < (kTriggerTowers+1) 	    )
 	{
	  
	  vector<DCCXtalBlock * > & xtalDataBlocks = (*itTowerBlock)->xtalBlocks();	
	  
	  // if there is no zero suppression, tower block must have have 25 channels in it
	  if (  (!zs)   &&   (xtalDataBlocks.size() != kChannelsPerTower)   )
	    {     
	      LogWarning("EcalTBRawToDigiTowerSize") << "EcalTBDaqFormatter::interpretRawData, no zero suppression "
					    << "wrong tower block size is: "  << xtalDataBlocks.size() 
					    << " in event " << (*itEventBlock)->getDataField("LV1")
					    << " for TT " << _ExpectedTowers[_expTowersIndex];
	      // report on wrong tt block size
	      blocksizecollection.push_back(idtt);

	      ++ _expTowersIndex; 	      continue;	

	    }
	  

	  short cryInTower =0;

	  short expStripInTower;
	  short expCryInStrip;
	  short expCryInTower =0;

	  // Access the Xstal data
	  for( vector< DCCXtalBlock * >::iterator itXtalBlock = xtalDataBlocks.begin(); 
	       itXtalBlock!= xtalDataBlocks.end(); 
	       itXtalBlock++){ //loop on crys of a  tower

	    strip              =(*itXtalBlock)->stripID();
	    ch                 =(*itXtalBlock)->xtalID();
	    cryInTower  =(strip-1)* kChannelsPerCard + (ch -1);

	    expStripInTower = expCryInTower/5 +1;
	    expCryInStrip     =  expCryInTower%5 +1;
	  

	    // FIXME: waiting for geometry to do (TT, strip,chNum) <--> (SMChId)
	    // short abscissa = (_ExpectedTowers[_expTowersIndex]-1)  /4;
	    // short ordinate = (_ExpectedTowers[_expTowersIndex]-1)  %4;
	    // temporarily choosing central crystal in trigger tower
	    // int cryIdInSM  = 45 + ordinate*5 + abscissa * 100;


	    // in case of 0 zuppressed data, check that cryInTower constantly grows
	    if (zs)
	      {
		// correct ordering
		if(  cryInTower >= expCryInTower )
		  {   expCryInTower = cryInTower +1;	  } 
		
		// cry_id wrong because of incorrect ordering within trigger tower
		else
		  {
		    LogWarning("EcalTBRawToDigiChId") << "EcalTBDaqFormatter::interpretRawData with zero suppression, "
						  << " based on ch ordering within tt, wrong channel id: "
						  << "\t strip: "  << strip  << "\t channel: " << ch
						  << "\t cryInTower "  << cryInTower
						  << "\t expCryInTower: " << expCryInTower
						  << "\t in TT: " << _ExpectedTowers[_expTowersIndex]
						  << "\t at event: " << (*itEventBlock)->getDataField("LV1");
		    
		    int  sm = 1; // hardcoded because of test  beam
		    for (int StripInTower_ =1;  StripInTower_ < 6; StripInTower_++){
		      for (int  CryInStrip_ =1;  CryInStrip_ < 6; CryInStrip_++){
			int  ic        = cryIc(tower, StripInTower_,  CryInStrip_) ;
			EBDetId               idExp(sm, ic,1);
			chidcollection.push_back(idExp);
		      }
		    }
		  
		    // chennel with id which does not follow correct odering
		    expCryInTower++;		    continue;
		    
		  }// end 'ch_id does not respect growing order'
		
	      }// end   if zero supression
	  


	    else {	      // if there is no zero suppression, channel has to equal the expected channel

	      if(   (cryInTower != expCryInTower) )
		{
		  
		  int ic        = cryIc(tower, expStripInTower,  expCryInStrip) ;
		  int  sm = 1; // hardcoded because of test  beam
		  EBDetId  idExp(sm, ic,1);
		  
		  LogWarning("EcalTBRawToDigiChId") << "EcalTBDaqFormatter::interpretRawData no zero suppression "
						    << " wrong channel id for channel: "  << expCryInStrip
						    << "\t strip: " << expStripInTower
						    << "\t in TT: " << _ExpectedTowers[_expTowersIndex]
						    << "\t in event: " << (*itEventBlock)->getDataField("LV1")
						    << "\t   (in the data, found channel:  " << ch
						    << "\t strip:  " << strip << " ).\n";

		  
		  // report on wrong channel id
		  chidcollection.push_back(idExp);

		  // there has been unexpected crystal id, dataframe not to go to the Event
		  expCryInTower++; 		  continue;
		  
		} // if channel in data does not equal expected channel

	      expCryInTower++;

	    } // end 'not zero suppression'
	    
	    
	    
	    // data  to be stored in EBDataFrame, identified by EBDetId
	    int  ic        = cryIc(tower, strip, ch) ;
	    int  sm = 1;
	    EBDetId  id(sm, ic,1);                 
	    
	    EBDataFrame theFrame ( id );
	    vector<int> xtalDataSamples = (*itXtalBlock)->xtalDataSamples();   
	    theFrame.setSize(xtalDataSamples.size());
      
      

	    // gain cannot be 0, checking for that
	    bool        gainIsOk =true;
	    unsigned gain_mask      = 12288;    //12th and 13th bit
	    vector <int> xtalGain;

	    for (unsigned short i=0; i<xtalDataSamples.size(); ++i ) {
	      
	      theFrame.setSample (i, xtalDataSamples[i] );
	      
	      if((xtalDataSamples[i] & gain_mask) == 0){gainIsOk =false;}
	      
	      xtalGain.push_back(0);
	      xtalGain[i] |= (xtalDataSamples[i] >> 12);
	    }
	    
	    if (! gainIsOk) {
	      
	      LogWarning("EcalTBRawToDigiGainZero") << "@SUB=EcalTBDaqFormatter::interpretRawData"
					    << " gain==0 for strip: "  << expStripInTower
					    << "\t channel: " << expCryInStrip
					    << "\t in TT: " << _ExpectedTowers[_expTowersIndex]
					    << "\t in event: " << (*itEventBlock)->getDataField("LV1");
	      // report on gain==0
	      gaincollection.push_back(id);
	      
	      // there has been a gain==0, dataframe not to go to the Event
	      continue; //	      expCryInTower already incremented
	    }


	    
	    
	    // looking for forbidden gain transitions
	    
	    short firstGainWrong=-1;
	    short numGainWrong=0;
	    
	    for (unsigned short i=0; i<xtalGain.size(); i++ ) {
	      
	      if (i>0 && xtalGain[i-1]>xtalGain[i]) {
		
		numGainWrong++;// counting forbidden gain transitions
		
		if (firstGainWrong == -1) {
		  firstGainWrong=i;
		  LogWarning("EcalTBRawToDigiGainSwitch") << "@SUB=EcalTBDaqFormatter::interpretRawData"
						<< "channelHasGainSwitchProblem: crystal eta = " << id.ieta() << " phi = " << id.iphi();
		}
		LogWarning("EcalTBRawToDigiGainSwitch") << "@SUB=EcalTBDaqFormatter::interpretRawData"
					      << "channelHasGainSwitchProblem: sample = " << (i-1) 
					      << " gain: " << xtalGain[i-1] << " sample: " << i << " gain: " << xtalGain[i];
	      }
	    }

	    // only discriminating if gain stays the same after the forbidden gain transition
	    bool wrongGainStaysTheSame=false;
	    if (firstGainWrong!=-1 && firstGainWrong<9){
	      short gainWrong = xtalGain[firstGainWrong];
    
	      // does wrong gain stay the same after the forbidden transition?
	      for (unsigned short u=firstGainWrong+1; u<xtalGain.size(); u++){

		if( gainWrong == xtalGain[u]) 
		  wrongGainStaysTheSame=true; 
		else
		  wrongGainStaysTheSame=false; 

	      }// END loop on samples after forbidden transition
            
	    }// END OF if firstGainWrong!=0 && firstGainWrong<8


	    if (numGainWrong>0) {
	      gainswitchcollection.push_back(id);

	      if (numGainWrong == 1 && (wrongGainStaysTheSame)) {
              
		LogWarning("EcalTBRawToDigiGainSwitch") << "@SUB=EcalTBDaqFormatter:interpretRawData"
					      << "channelHasGainSwitchProblem: wrong transition stays till last sample"<< "\n";

		gainswitchstaycollection.push_back(id);              
	      }
	      else if (numGainWrong>1) {
		LogWarning("EcalTBRawToDigiGainSwitch") << "@SUB=EcalTBDaqFormatter:interpretRawData"
					      << "channelHasGainSwitchProblem: more than 1 wrong transition";
              
		for (unsigned short i1=0; i1<xtalDataSamples.size(); ++i1 ) {
		  int countADC = 0x00000FFF;
		  countADC &= xtalDataSamples[i1];
		  LogDebug("EcalTBRawToDigi") << "Sample " << i1 << " ADC " << countADC << " Gain " << xtalGain[i1];
		}

	      }// end 'if there is multiple transition'
	     

	      // there has been a forbidden gain transition,  dataframe not to go to the Event
	      continue; //	      expCryInTower already incremented

	    }// END of:   'if there is a forbidden gain transition'


	    // here (already continued if gain==0 or if forbidden-gain-switch),
	    // data frame needs go to the Event
	    digicollection.push_back(theFrame);
	    
	  }// end loop on crystals within a tower block
	  
	  
	  _expTowersIndex++;
	}// end: tt1 ... tt68, crystal data
      


      
      
      /******************************************************************
       //    tt 69 and 70:  two mem boxes, holding PN0 ... PN9
       ******************************************************************/	
      else if (       (*itTowerBlock)->towerID() == 69 
                      ||	   (*itTowerBlock)->towerID() == 70       )	
	{
	  
	  LogDebug("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter::interpretRawData"
				      << "processing mem box num: " << (*itTowerBlock)->towerID();

	  // if tt 69 or 70 found, allocate Pn digi collection
	  if(! pnAllocated) 
	    {
	      pndigicollection.reserve(kPns);
	      pnAllocated = true;
	    }

	  DecodeMEM( (*itTowerBlock),  pndigicollection , 
		     memttidcollection,  memblocksizecollection,
		     memgaincollection,  memchidcollection);
	  
	}// end of < if it is a mem box>
      
      
    


      // wrong tt id
      else  {
        LogWarning("EcalTBRawToDigiTowerId") <<"@SUB=EcalTBDaqFormatter::interpretRawData"
				      << " processing tt with ID not existing ( "
				      <<  (*itTowerBlock)->towerID() << ")";
        ++ _expTowersIndex;continue; 
      }// end: tt id error

    }// end loop on trigger towers
      
  }// end loop on events
}








void EcalTBDaqFormatter::DecodeMEM( DCCTowerBlock *  towerblock,  EcalPnDiodeDigiCollection & pndigicollection ,
				    EcalElectronicsIdCollection & memttidcollection,  EcalElectronicsIdCollection &  memblocksizecollection,
				    EcalElectronicsIdCollection & memgaincollection,  EcalElectronicsIdCollection & memchidcollection)
{
  
  LogDebug("EcalTBRawToDigi") << "@SUB=EcalTBDaqFormatter::DecodeMEM"
 			      << "in mem " << towerblock->towerID();  
  
  int  tower_id = towerblock ->towerID() ;
  int  mem_id   = tower_id-69;

  // initializing container
  for (int st_id=0; st_id< kStripsPerTower; st_id++){
    for (int ch_id=0; ch_id<kChannelsPerStrip; ch_id++){
      for (int sa=0; sa<11; sa++){      
	memRawSample_[st_id][ch_id][sa] = -1;}    } }

  
  // check that tower block id corresponds to mem boxes
  if(tower_id != 69 && tower_id != 70) 
    {
      LogWarning("EcalTBRawToDigiTowerId") << "@SUB=EcalTBDaqFormatter:decodeMem"
				    << "DecodeMEM: this is not a mem box tower (" << tower_id << ")"<< "\n";
      ++ _expTowersIndex;
      return;
    }


  // check that the mem-tower coming in is the one expected from DCC-header event status
  if ( tower_id != ( (int)_ExpectedTowers[_expTowersIndex])  )
    {
      LogWarning("EcalTBRawToDigiTowerId") << "@SUB=EcalTBDaqFormatter:decodeMem"
				    << "DecodeMEM: tower " << tower_id  
				    << " is not the same as expected " << ((int)_ExpectedTowers[_expTowersIndex])
				    << " (according to DCC header channel status)\n";
      
      // chosing channel 1 as representative as a dummy...
      EcalElectronicsId id(1, tower_id, 1);
      memttidcollection.push_back(id);
      ++ _expTowersIndex;
      return; // if NOT a mem tt block - do not build any Pn digis
    }
       

     
  /******************************************************************************
   // getting the raw hits from towerBlock while checking tt and ch data structure 
   ******************************************************************************/
  vector<DCCXtalBlock *> & dccXtalBlocks = towerblock->xtalBlocks();
  vector<DCCXtalBlock*>::iterator itXtal;

  // checking mem tower block fo size
  if (dccXtalBlocks.size() != kChannelsPerTower)
    {     
      LogDebug("EcalTBRawToDigiDccBlockSize") << "@SUB=EcalTBDaqFormatter:decodeMem"
				  << " wrong dccBlock size, namely: "  << dccXtalBlocks.size() 
				  << ", for mem " << _ExpectedTowers[_expTowersIndex];

      // reporting mem-tt block size problem
      // chosing channel 1 as representative as a dummy...
      EcalElectronicsId id(1, tower_id, 1);
      memblocksizecollection.push_back(id);

      ++ _expTowersIndex;
      return;  // if mem tt block size not ok - do not build any Pn digis
    }
  

  // loop on channels of the mem block
  int  cryCounter = 0;   int  strip_id  = 0;   int  xtal_id   = 0;  

  for ( itXtal = dccXtalBlocks.begin(); itXtal < dccXtalBlocks.end(); itXtal++ ) {
    strip_id                     = (*itXtal) ->getDataField("STRIP ID");
    xtal_id                      = (*itXtal) ->getDataField("XTAL ID");
    int wished_strip_id  = cryCounter/ kStripsPerTower;
    int wished_ch_id     = cryCounter% kStripsPerTower;
    
    if( (wished_strip_id+1) != ((int)strip_id) ||
	(wished_ch_id+1) != ((int)xtal_id) )
      {
	
	LogDebug("EcalTBRawToDigiChId") << "@SUB=EcalTBDaqFormatter:decodeMem"
				    << " in mem " <<  towerblock->towerID()
				    << ", expected:\t strip"
				    << (wished_strip_id+1)  << " cry " << (wished_ch_id+1) << "\tfound: "
				    << "  strip " <<  strip_id << "  cry " << xtal_id;
	
	// report on crystal with unexpected indices
	EcalElectronicsId id(1, tower_id, (strip_id-1)*5 + xtal_id);
	memchidcollection.push_back(id);
      }
    
    
    // Accessing the 10 time samples per Xtal:
    memRawSample_[wished_strip_id][wished_ch_id][1] = (*itXtal)->getDataField("ADC#1");
    memRawSample_[wished_strip_id][wished_ch_id][2] = (*itXtal)->getDataField("ADC#2");
    memRawSample_[wished_strip_id][wished_ch_id][3] = (*itXtal)->getDataField("ADC#3");
    memRawSample_[wished_strip_id][wished_ch_id][4] = (*itXtal)->getDataField("ADC#4");
    memRawSample_[wished_strip_id][wished_ch_id][5] = (*itXtal)->getDataField("ADC#5");
    memRawSample_[wished_strip_id][wished_ch_id][6] = (*itXtal)->getDataField("ADC#6");
    memRawSample_[wished_strip_id][wished_ch_id][7] = (*itXtal)->getDataField("ADC#7");
    memRawSample_[wished_strip_id][wished_ch_id][8] = (*itXtal)->getDataField("ADC#8");
    memRawSample_[wished_strip_id][wished_ch_id][9] = (*itXtal)->getDataField("ADC#9");
    memRawSample_[wished_strip_id][wished_ch_id][10] = (*itXtal)->getDataField("ADC#10");
      
    cryCounter++;
  }// end loop on crystals of mem dccXtalBlock
  
  // tower accepted and digi read from all 25 channels.
  // Increase counter of expected towers before unpacking in the 5 PNs
  ++ _expTowersIndex;



  /************************************************************
   // unpacking and 'cooking' the raw numbers to get PN sample
   ************************************************************/
  int tempSample=0;
  int memStoreIndex=0;
  int ipn=0;
  for (memStoreIndex=0; memStoreIndex<500; memStoreIndex++)    {
    data_MEM[memStoreIndex]= -1;   }
  
  
  for(int strip=0; strip<kStripsPerTower; strip++) {// loop on strips
    for(int channel=0; channel<kChannelsPerStrip; channel++) {// loop on channels

      if(strip%2 == 0) 
	{ipn= mem_id*5+channel;}
      else 
	{ipn=mem_id*5+4-channel;}

      for(int sample=0;sample< kSamplesPerChannel ;sample++) {
	tempSample= memRawSample_[strip][channel][sample+1];

	int new_data=0;
	if(strip%2 == 1) {
	  // 1) if strip number is even, 14 bits are reversed in order
	  for(int ib=0;ib<14;ib++)
	    { 
	      new_data <<= 1;
	      new_data=new_data | (tempSample&1);
	      tempSample >>= 1;
	    }
	} else {
	  new_data=tempSample;
	}

	// 2) flip 11th bit for AD9052 still there on MEM !
	// 3) mask with 1 1111 1111 1111
	new_data = (new_data ^ 0x800) & 0x3fff;    // (new_data  XOR 1000 0000 0000) & 11 1111 1111 1111
	// new_data = (new_data ^ 0x800) & 0x1fff;    // (new_data  XOR 1000 0000 0000) & 1 1111 1111 1111

	//(Bit 12) == 1 -> Gain 16;    (Bit 12) == 0 -> Gain 1	
	// gain in mem can be 1 or 16 encoded resp. with 0 ir 1 in the 13th bit.
	// checking and reporting if there is any sample with gain==2,3
	short sampleGain = (new_data &0x3000)/4096;
	if (  sampleGain==2 || sampleGain==3) 
	  {
	    EcalElectronicsId id(1, tower_id, strip*5 + channel + 1);
	    memgaincollection.push_back(id);
	    
	    LogWarning("EcalTBRawToDigiGainZero")  << "@SUB=EcalTBDaqFormatter:decodeMem"
					   << "in mem " <<  towerblock->towerID()
					   << " :\t strip: "
					   << (strip +1)  << " cry: " << (channel+1) 
					   << " has 14th bit non zero! Gain results: "
					   << sampleGain << ".";
	    
	    continue;
	  }// end 'if gain is zero'

	memStoreIndex= ipn*50+strip*kSamplesPerChannel+sample;
	// storing in data_MEM also the gain bits
	data_MEM[memStoreIndex]= new_data & 0x3fff;

      }// loop on samples
    }// loop on strips
  }// loop on channels
  



  for (int pnId=0; pnId<kPnPerTowerBlock; pnId++) pnIsOkInBlock[pnId]=true;
  // if anything was wrong with mem_tt_id or mem_tt_size: you would have already exited
  // otherwise, if any problem with ch_gain or ch_id: must not produce digis for the pertaining Pn

  if (!      (memgaincollection.size()==0 && memchidcollection.size()==0)          )
    {
      for ( EcalElectronicsIdCollection::const_iterator idItr = memgaincollection.begin();
	    idItr != memgaincollection.end();
	    ++ idItr ) {
	int ch = (*idItr).channelId();
	ch = (ch-1)/5;
	pnIsOkInBlock [ch] = false;
      }

      for ( EcalElectronicsIdCollection::const_iterator idItr = memchidcollection.begin();
	    idItr != memchidcollection.end();
	    ++ idItr ) {
	int ch = (*idItr).channelId();
	ch = (ch-1)/5;
	pnIsOkInBlock [ch] = false;
      }

    }// end: if any ch_gain or ch_id problems exclude the Pn's from digi production




  // looping on PN's of current mem box
  for (int pnId = 1;  pnId <  (kPnPerTowerBlock+1); pnId++){

    // if present Pn has any of its 5 channels with problems, do not produce digi for it
    if (! pnIsOkInBlock [pnId-1] ) continue;

    // fixme giof: second argumenti is DCCId, to be determined
    EcalPnDiodeDetId PnId(1, 1, pnId +  kPnPerTowerBlock*mem_id);
    EcalPnDiodeDigi thePnDigi(PnId );

    thePnDigi.setSize(kSamplesPerPn);

    for (int sample =0; sample<kSamplesPerPn; sample++)
      {
	//	int adc  = (data_MEM[(mem_id)*250 + (pnId-1)*kSamplesPerPn + sample ] & 0xfff);
	//	int gain = (data_MEM[(mem_id)*250 + (pnId-1)*kSamplesPerPn + sample ] & 0x3000) /4096;;
	//	EcalFEMSample thePnSample(adc, gain);
	EcalFEMSample thePnSample( data_MEM[(mem_id)*250 + (pnId-1)*kSamplesPerPn + sample ] );
	thePnDigi.setSample(sample,  thePnSample );  
      }
    pndigicollection.push_back(thePnDigi);
  }
  
  
}














pair<int,int>  EcalTBDaqFormatter::cellIndex(int tower_id, int strip, int ch) {
  
  int xtal= (strip-1)*5+ch-1;
  //  cout << " cellIndex input xtal " << xtal << endl;
  pair<int,int> ind;
  
  int eta = (tower_id - 1)/kTowersInPhi*kCardsPerTower;
  int phi = (tower_id - 1)%kTowersInPhi*kChannelsPerCard;

  if (rightTower(tower_id))
    eta += xtal/kCardsPerTower;
  else
    eta += (kCrystalsPerTower - 1 - xtal)/kCardsPerTower;

  if (rightTower(tower_id) && (xtal/kCardsPerTower)%2 == 1 ||
      !rightTower(tower_id) && (xtal/kCardsPerTower)%2 == 0)

    phi += (kChannelsPerCard - 1 - xtal%kChannelsPerCard);
  else
    phi += xtal%kChannelsPerCard;


  ind.first =eta+1;  
  ind.second=phi+1; 

  //  cout << "  EcalTBDaqFormatter::cell_index eta " << ind.first << " phi " << ind.second << " " << endl;

  return ind;

}



int  EcalTBDaqFormatter::cryIc(int tower, int strip, int ch) {
  pair<int,int> cellInd= EcalTBDaqFormatter::cellIndex(tower, strip, ch); 
  return cellInd.second + (cellInd.first-1)*kCrystalsInPhi;
}



bool EcalTBDaqFormatter::rightTower(int tower) const {
  
  if ((tower>12 && tower<21) || (tower>28 && tower<37) ||
      (tower>44 && tower<53) || (tower>60 && tower<69))
    return true;
  else
    return false;
}



bool EcalTBDaqFormatter::leftTower(int tower) const
{
  return !rightTower(tower);
}


