// -*- C++ -*-
//
// Package:    HcalChanMon
// Class:      HcalChanMon
// 
/**\class HcalChanMon HcalChanMon.cc HcalChanMon/HcalChanMon/src/HcalChanMon.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
// Original Authors :  Niki Saoulidou 
// 		       Mehmet Deliomeroglu
//                      
// The code takes as input N HCAL Channels (ieta,iphi,depth,detector)
// and produces diagnostic histograms of RAW, DIGItized and REConstructed data      
//         Created:  Thu Jun  4 13:12:17 CDT 2009
// $Id: HcalChanMon.cc,v 1.2 2010/05/31 22:24:16 ferencek Exp $
//
//

#include "RecoHcal/HcalPromptAnalysis/interface/HcalChanMon.h"


//
// constructors and destructor
//

HcalChanMon::HcalChanMon(const edm::ParameterSet& iConfig){

   ChannelList_   =  iConfig.getUntrackedParameter<Parameters>("ChannelList");  
   runBegin = -1;
   ChargeAbovePed =  10; // put this in config
   EnergyThres    =   0;
  
}


HcalChanMon::~HcalChanMon(){
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void HcalChanMon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

  using namespace edm;
  using namespace std;
  evt++;
  
  edm::ESHandle<HcalDbService> pSetup;
  iSetup.get<HcalDbRecord>().get(pSetup);
  readoutMap_ = pSetup->getHcalMapping();
  
  if (runBegin < 0){         
   runBegin = iEvent.id().run();
   bookHistograms();
  }
 
   Handle<FEDRawDataCollection> fedraw;
   iEvent.getByType(fedraw);
   Int_t count =0 ;
   
   Int_t num   = channelse.size();
   Int_t *raw_problem = new Int_t[num];
   
   for(Int_t i=0;i<num;i++) raw_problem[i]=0;  
   
   for (std::vector<HcalElectronicsId>::const_iterator cit = channelse.begin();
	cit != channelse.end(); cit++) {
        
     int subDet   = channelsd[count].subdet();
     int ieta	  = channelsd[count].ieta();
     int iphi	  = channelsd[count].iphi();
     int depth	  = channelsd[count].depth();
  
    // get dcc data
     const FEDRawData &raw = fedraw->FEDData(700 + cit->dccid());
        
     unsigned char* trailer_ptr = (unsigned char*) (raw.data()+raw.size()-sizeof(uint64_t));
     FEDTrailer trailer = FEDTrailer(trailer_ptr);
 
     const HcalDCCHeader *dccHeader = (const HcalDCCHeader *)(raw.data());
     if(!dccHeader) return;
      
     int dccid      = cit->dccid();
     int spigot     = cit->spigot();
//      int dccslot    = 9;
     
//      int htrfiber   = cit->htrChanId();
//      int htrtb      = cit->htrTopBottom() ;
//      int htrslot    = cit->htrSlot();
//      int crate      = cit->readoutVMECrateId();
     
//      int fiberchan  = cit->fiberChanId();
//      int fiberindx  = cit->fiberIndex();
     
     HcalHTRData htr;
     dccHeader->getSpigotData(cit->spigot(), htr, raw.size());     
     
      ////////// Histogram CDF ERRORS ////////////
     sprintf(channel,"h_CDFEErrorWord_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
     
     if(!dccHeader->thereIsASecondCDFHeaderWord()) histo1D[channel]->Fill(0.);
     
     CDFvers_it = CDFversionNumber_list.find(dccid);
     if (CDFvers_it  == CDFversionNumber_list.end()) {
      CDFversionNumber_list.insert(pair<int,short>
				 (dccid,dccHeader->getCDFversionNumber() ) );
     CDFvers_it = CDFversionNumber_list.find(dccid);
      } // then check against it.
     if (dccHeader->getCDFversionNumber()!= CDFvers_it->second)  histo1D[channel]->Fill(1.);     
     //Make sure a reference CDF EventType value has been recorded for this dccid
     CDFEvT_it = CDFEventType_list.find(dccid);
     if (CDFEvT_it  == CDFEventType_list.end()) {
       CDFEventType_list.insert(pair<int,short>
			     (dccid,dccHeader->getCDFEventType() ) );
      CDFEvT_it = CDFEventType_list.find(dccid);
     } // then check against it.
     if (dccHeader->getCDFEventType()!= CDFEvT_it->second) histo1D[channel]->Fill(2.);
     //There should always be a '5' in CDF Header word 0, bits [63:60]
     if (dccHeader->BOEshouldBe5Always()!=5) histo1D[channel]->Fill(3.);  
     //There should never be a third CDF Header word indicated.
     if (dccHeader->thereIsAThirdCDFHeaderWord()) histo1D[channel]->Fill(4.);  
    //Make sure a reference value of Reserved Bits has been recorded for this dccid
     CDFReservedBits_it = CDFReservedBits_list.find(dccid);
     if (CDFReservedBits_it  == CDFReservedBits_list.end()) {
       CDFReservedBits_list.insert(pair<int,short>
				(dccid,dccHeader->getSlink64ReservedBits() & 0x00FFFF ) );
       CDFReservedBits_it = CDFReservedBits_list.find(dccid);
     } // then check against it.
     if ((int) dccHeader->getSlink64ReservedBits()!= CDFReservedBits_it->second) histo1D[channel]->Fill(5.);   
     //There should always be 0x0 in CDF Header word 1, bits [63:60]
     if (dccHeader->BOEshouldBeZeroAlways() !=0)  histo1D[channel]->Fill(6.);
     //  There should only be one trailer
     if (trailer.moreTrailers()) histo1D[channel]->Fill(7.);
     // CDF Trailer [55:30] should be the # 64-bit words in the EvFragment
     if ((uint64_t) raw.size() != ( (uint64_t) trailer.lenght()*sizeof(uint64_t)) )  histo1D[channel]->Fill(8.);     
     //There is a rudimentary sanity check built into the FEDTrailer class
     if (!trailer.check()) histo1D[channel]->Fill(9.);
   

    sprintf(channel,"h_DCCEventFormatError_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth);
    
    ////////// Histogram DCC EVENT FORMAT ERRORS ////////////
    //Make sure a reference value of the DCC Event Format version has been noted for this dcc.
    DCCEvtFormat_it = DCCEvtFormat_list.find(dccid);
    if (DCCEvtFormat_it == DCCEvtFormat_list.end()) {
      DCCEvtFormat_list.insert(pair<int,short>
  	  		       (dccid,dccHeader->getDCCDataFormatVersion() ) );
      DCCEvtFormat_it = DCCEvtFormat_list.find(dccid);
    } // then check against it.
    if (dccHeader->getDCCDataFormatVersion()!= DCCEvtFormat_it->second) histo1D[channel]->Fill(0.);
      
    //Check for ones where there should always be zeros
    if (false) //dccHeader->getByte1Zeroes() || dccHeader->getByte3Zeroes() || dccHeader->getByte567Zeroes()) 
     histo1D[channel]->Fill(1.);
    //Check that there are zeros following the HTR Status words.
    int SpigotPad = HcalDCCHeader::SPIGOT_COUNT;
    if ( ( ((uint64_t) dccHeader->getSpigotSummary(SpigotPad)  ) 
  	  | ((uint64_t) dccHeader->getSpigotSummary(SpigotPad+1)) 
  	  | ((uint64_t) dccHeader->getSpigotSummary(SpigotPad+2)) ) != 0)
     histo1D[channel]->Fill(2.);
     //Check that there are zeros following the HTR Payloads, if needed.
    int nHTR32BitWords=0;
    // add up all the declared HTR Payload lengths
    for(int i=0; i<HcalDCCHeader::SPIGOT_COUNT; i++) {
      nHTR32BitWords += dccHeader->getSpigotDataLength(i);  }
    // if it's an odd number, check for the padding zeroes
    if (( nHTR32BitWords % 2) == 1) {
      uint64_t* lastDataWord = (uint64_t*) ( raw.data()+raw.size()-(2*sizeof(uint64_t)) );
      if ((*lastDataWord>>32) != 0x00000000)
       histo1D[channel]->Fill(3.);
    }

     sprintf(channel,"h_DCCErrorAndWarningConditions_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth);
     char TTS_state = ((raw.data()[raw.size()-8]>>4) & 0x0F);
 
     ////////// Histogram Errors and Warnings from the DCC;////////////
      /* [1:15] */ //Histogram HTR Status Bits from the DCC Header
     if (dccHeader->getSpigotErrorFlag(spigot)) histo1D[channel]->Fill(0.);
       
      /* [16:25] */ //Histogram DCC Error and Warning Counters being nonzero
     if (TTS_state & 0x1)		    	 histo1D[channel]->Fill(1.);
     if (TTS_state & 0x4)		    	 histo1D[channel]->Fill(2.);
     if (TTS_state & 0x2)		    	 histo1D[channel]->Fill(3.);
     if (dccHeader->SawL1A_EvN_MxMx()	)   	 histo1D[channel]->Fill(4.);
     if (dccHeader->SawL1A_BcN_MxMx()	)   	 histo1D[channel]->Fill(5.);
     if (dccHeader->SawCT_EvN_MxMx()	)   	 histo1D[channel]->Fill(6.);
     if (dccHeader->SawCT_BcN_MxMx()	)   	 histo1D[channel]->Fill(7.);
     if (dccHeader->SawOrbitLengthErr() )   	 histo1D[channel]->Fill(8.);
     if (dccHeader->SawTTC_SingErr()	)   	 histo1D[channel]->Fill(9.);
     if (dccHeader->SawTTC_DoubErr()	)   	 histo1D[channel]->Fill(10.);


    sprintf(channel,"h_SummariesOfHTRS_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth);

    ////////// Histogram Spigot Errors from the DCCs HTR Summaries;////////////
    /* [1:8] */ //Histogram HTR Error Bits in the DCC Headers  
    unsigned char WholeErrorList=0; 
    WholeErrorList=dccHeader->getSpigotErrorBits(spigot);
    if ((WholeErrorList>>0)&0x01) histo1D[channel]->Fill(0.);	 
    if ((WholeErrorList>>1)&0x01) histo1D[channel]->Fill(1.);
    if ((WholeErrorList>>2)&0x01) histo1D[channel]->Fill(2.);
    if ((WholeErrorList>>3)&0x01) histo1D[channel]->Fill(3.);
    if ((WholeErrorList>>4)&0x01) histo1D[channel]->Fill(4.);
    if ((WholeErrorList>>5)&0x01) histo1D[channel]->Fill(5.);
    if ((WholeErrorList>>6)&0x01) histo1D[channel]->Fill(6.);
    if ((WholeErrorList>>7)&0x01) histo1D[channel]->Fill(7.);
   /* [9:16] */ //Histogram LRB Error Bits in the DCC Headers
    WholeErrorList=0; 
    WholeErrorList=dccHeader->getLRBErrorBits(spigot);
    if ((WholeErrorList>>0)&0x03) { //HammingCode Corrected & Uncorr
     //HammingCode Corr
     if ((WholeErrorList>>0)&0x01)  histo1D[channel]->Fill(8.);
     //HammingCode Uncor
     if ((WholeErrorList>>1)&0x01)  histo1D[channel]->Fill(9.);
    }
    for (int i=2; i<8; i++) {
     if ((WholeErrorList>>i)&0x01)  histo1D[channel]->Fill(8.+i);	       
    }    
    /* [17:20] */ //Histogram condition of Enabled Spigots without data Present
    if (dccHeader->getSpigotEnabled(spigot) &&
	!dccHeader->getSpigotPresent(spigot)	  ) histo1D[channel]->Fill(16.);
    if (dccHeader->getSpigotPresent(spigot) &&
	!dccHeader->getBxMismatchWithDCC(spigot)  ) histo1D[channel]->Fill(17.);
    if (dccHeader->getSpigotPresent(spigot) &&
	!dccHeader->getSpigotValid(spigot)	  ) histo1D[channel]->Fill(18.);
    if (dccHeader->getSpigotDataTruncated(spigot) ) histo1D[channel]->Fill(19.);

      
     int errWord =  htr.getErrorsWord()&0x1FFFF ;
                    	        
     const short unsigned int* daq_first, *daq_last, *tp_first, *tp_last;
     const HcalQIESample* qie_begin, *qie_end, *qie_work;
 
     // get pointers
     htr.dataPointers(&daq_first,&daq_last,&tp_first,&tp_last);
     // HISTOGRAM HTR ERROR WORD
     sprintf(channel,"h_HTRErrorWord_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
     for(int i=0; i<16; i++){
    
      int errbit = errWord&(0x01<<i);
      if (i==15) errbit = errbit - 0x8000;  
      if(errbit !=0) histo1D[channel]->Fill(i);
      
     }
	  
     qie_begin=(HcalQIESample*)daq_first;
     qie_end  =(HcalQIESample*)(daq_last+1); // one beyond last..

     Int_t slice    =  0;
     Int_t capidsav = -1;
     Int_t capiderr =  0;
     
     Bool_t found = false;
     Int_t val=0;
     Int_t err=0;
      
     for (qie_work=qie_begin; qie_work!=qie_end; qie_work++) {
              
       // SELECT THE CHANNEL OF INTEREST LOOPING OVER TIMESLICES (10 IN THIS CASE)
       
       if(cit->fiberChanId() == qie_work->fiberChan() && cit->fiberIndex() == qie_work->fiber()){
          
	 found = true; 
	 if(qie_work->capid() == capidsav)                                      capiderr = 1;
	 if(qie_work->capid()<0 || qie_work->capid()>3)                         capiderr = 1;
	 if(slice!=0 && qie_work->capid()>0  && qie_work->capid()!=capidsav+1)  capiderr = 1;
	 if(slice!=0 && qie_work->capid()==0 && capidsav!=3)                    capiderr = 1;
	 
	
	 if(slice == 0){
   	 // First time for this channel  fill other histos as well 
	  sprintf(channel,"h_NormRaw_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 	 
	  histo1DNormRaw[channel]->Fill(0.5);	 
	 }         
	  
         sprintf(channel,"h_QIE_Validity_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
         histo1D[channel]->Fill(slice,qie_work->dv());
         if(qie_work->dv()!=1) val=1;
	 
	 sprintf(channel,"h_QIE_Error_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
         histo1D[channel]->Fill(slice,qie_work->er());
         if(qie_work->er()!=0) err=1;
	 
         sprintf(channel,"h_QIE_fC_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
         histo1D[channel]->Fill(slice,qie_work->nominal_fC());
      
         slice=slice+1;
         capidsav = qie_work->capid();
	 	 
	}      
     }
         
     sprintf(channel,"h_QIE_CAPIDERR_DetId%d_%d_%d_%d", subDet, ieta, iphi, depth); 
     if(found) histo1D[channel]->Fill(capiderr);
        
     if(found && (capiderr>0 || val>0 || err>0) )         raw_problem[count] = 1; 
     else if (found && capiderr==0 && val==0 && err == 0) raw_problem[count] = 0;
     count=count+1;
             
 }
   
 // END OF RAW DATA 
 
 // START OF REC HITS
 
 Handle<HBHERecHitCollection> hbhe_rechit;
 Handle<HORecHitCollection>   ho_rechit;
 Handle<HFRecHitCollection>   hf_rechit;
 
 iEvent.getByType(hbhe_rechit);
 iEvent.getByType(ho_rechit);
 iEvent.getByType(hf_rechit);
 
 count =0;
  
 for(std::vector<HcalDetId>::const_iterator cit = channelsd.begin(); cit!=channelsd.end();cit++){
 
  int subDet    = cit->subdet();
  int ieta      = cit->ieta();
  int iphi      = cit->iphi();
  int depth     = cit->depth();
  

  // Loop over hbhe rec hits, check if object is available
   if(!hbhe_rechit.failedToGet()){
    for(HBHERecHitCollection::const_iterator  rh = hbhe_rechit->begin();rh!=hbhe_rechit->end();rh++){
      
     HcalDetId id(rh->detid().rawId());
          
      if(id.ieta()==ieta && id.iphi()==iphi && id.depth()==depth && id.subdet()==subDet){
        
	sprintf(channel,   "h_NormRecHit_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_NormRecHit_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DNormRecHit[channel]->Fill(0.5);
	if(raw_problem[count]==0) histo1DNormRecHit_np[channel_np]->Fill(0.5);
	
	sprintf(channel,   "h_RecHitEnergy_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergy_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->energy());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->energy());
	
	sprintf(channel,   "h_RecHitTime_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth); 
	sprintf(channel_np,"h_RecHitTime_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth); 
	
	histo1DR[channel]->Fill(rh->time());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->time());
	
	sprintf(channel,   "h_RecHitFlags_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitFlags_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->flags());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->flags());
	
	sprintf(channel,   "h_RecHitEnergyThres_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergyThres_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	if(rh->energy()>EnergyThres) histo1DR[channel]->Fill(rh->energy());  
        if(raw_problem[count]==0 && rh->energy()>EnergyThres) histo1DR_np[channel_np]->Fill(rh->energy());  
            
      }
    
    } 
   }
 
  // Loop over ho rec hits, check if object is available
   if(!ho_rechit.failedToGet()){
    for(HORecHitCollection::const_iterator  rh = ho_rechit->begin();rh!=ho_rechit->end();rh++){

     HcalDetId id(rh->detid().rawId());
  
      if(id.ieta()==ieta && id.iphi()==iphi && id.depth()==depth && id.subdet()==subDet){

	
     	sprintf(channel,   "h_NormRecHit_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_NormRecHit_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DNormRecHit[channel]->Fill(0.5);
	if(raw_problem[count]==0) histo1DNormRecHit_np[channel_np]->Fill(0.5);
		
	sprintf(channel,   "h_RecHitEnergy_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergy_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->energy());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->energy());
	
	sprintf(channel,   "h_RecHitTime_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth); 
	sprintf(channel_np,"h_RecHitTime_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth); 
	
	histo1DR[channel]->Fill(rh->time());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->time());
	
	sprintf(channel,   "h_RecHitFlags_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitFlags_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->flags());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->flags());
	
	sprintf(channel,   "h_RecHitEnergyThres_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergyThres_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	if(rh->energy()>EnergyThres) histo1DR[channel]->Fill(rh->energy());  
        if(raw_problem[count]==0 && rh->energy()>EnergyThres) histo1DR_np[channel_np]->Fill(rh->energy());  
          
        

      }

    }
   }
   

 // Loop over ho rec hits, check if object is available
   if(!hf_rechit.failedToGet()){
    for(HFRecHitCollection::const_iterator  rh = hf_rechit->begin();rh!=hf_rechit->end();rh++){

     HcalDetId id(rh->detid().rawId());

      if(id.ieta()==ieta && id.iphi()==iphi && id.depth()==depth && id.subdet()==subDet){

       	sprintf(channel,   "h_NormRecHit_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_NormRecHit_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DNormRecHit[channel]->Fill(0.5);
	if(raw_problem[count]==0) histo1DNormRecHit_np[channel_np]->Fill(0.5);
	
	sprintf(channel,   "h_RecHitEnergy_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergy_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->energy());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->energy());
	
	sprintf(channel,   "h_RecHitTime_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth); 
	sprintf(channel_np,"h_RecHitTime_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth); 
	
	histo1DR[channel]->Fill(rh->time());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->time());
	
	sprintf(channel,   "h_RecHitFlags_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitFlags_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	histo1DR[channel]->Fill(rh->flags());
	if(raw_problem[count]==0) histo1DR_np[channel_np]->Fill(rh->flags());
	
	sprintf(channel,   "h_RecHitEnergyThres_DetId%d_%d_%d_%d",   subDet,ieta,iphi,depth);
	sprintf(channel_np,"h_RecHitEnergyThres_np_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	
	if(rh->energy()>EnergyThres) histo1DR[channel]->Fill(rh->energy());  
        if(raw_problem[count]==0 && rh->energy()>EnergyThres) histo1DR_np[channel_np]->Fill(rh->energy());  
          
       
      }
    
    }
   }
 
  count=count+1;
  
 }
 
 // END OF REC HITS
 

 // START OF DIGITIZED DATA
       
  Handle<HBHEDigiCollection> hbhe_digi;
  Handle<HODigiCollection>   ho_digi;
  Handle<HFDigiCollection>   hf_digi;

  iEvent.getByType(hbhe_digi);
  iEvent.getByType(ho_digi);
  iEvent.getByType(hf_digi);

  count =0;
  
for (std::vector<HcalDetId>::const_iterator cit = channelsd.begin(); cit != channelsd.end(); cit++) {

  int subDet	= cit->subdet();
  int ieta	= cit->ieta();
  int iphi	= cit->iphi();
  int depth	= cit->depth();


   if(!ho_digi.failedToGet()){ // object is available 

    for(HODigiCollection::const_iterator d=ho_digi->begin(); d!=ho_digi->end(); d++){
    const HODataFrame digi = (const HODataFrame)(*d);

     if(digi.id().ieta()==ieta && digi.id().iphi()==iphi && digi.id().subdet()==subDet && digi.id().depth()==depth){ 
              	 
     sprintf(channel,   "h_NormDigit_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_NormDigit_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     histo1DNormDigit[channel]->Fill(0.5); 
     if(raw_problem[count]==0) histo1DNormDigit_np[channel_np]->Fill(0.5); 
      
     sprintf(channel,   "h_PulseShape_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     sprintf(channel_np,"h_PulseShape_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());          
     for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
     if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
    
     sprintf(channel,   "h_DigSize_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigSize_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.size());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.size());
     
     sprintf(channel,   "h_DigPresamples_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigPresamples_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.presamples());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.presamples());
      
     sprintf(channel,   "h_DigTotCharge_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotCharge_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totcharge =0;
     for(int i=0; i<digi.size(); i++) totcharge = totcharge+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totcharge); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totcharge); 
           
     sprintf(channel,   "h_DigTotChargePreSample_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePreSample_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totchargep =0;
     for(int i=0; i<digi.presamples(); i++) totchargep = totchargep+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totchargep); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totchargep); 
     
     Double_t ped = (totchargep/digi.presamples())*digi.size(); 
     
     sprintf(channel,   "h_DigTotChargePedSub_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePedSub_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     if((totcharge-ped) >  0 ) histo1DD[channel]->Fill(totcharge-ped);
     if((totcharge-ped) <= 0 ) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if((totcharge-ped) >  0 ) histo1DD_np[channel_np]->Fill(totcharge-ped);
      if((totcharge-ped) <= 0 ) histo1DD_np[channel_np]->Fill(0);
     }
     
     
     Double_t thres = (totchargep/digi.presamples())*digi.size()+ChargeAbovePed; 
     sprintf(channel,   "h_PulseShapeThres_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_PulseShapeThres_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     if(totcharge>thres) {
      evts++;
      for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
      if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
      
      //Count how many times the number above was grater than zero.
      sprintf(channel,   "h_PrecentageSignal_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      sprintf(channel_np,"h_PrecentageSignal_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      histo1DD[channel]->Fill(0.5,evts);
      if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(0.5,evts);
     }
         
     sprintf(channel,   "h_DigzsMarkAndPass_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());   
     sprintf(channel_np,"h_DigzsMarkAndPass_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsMarkAndPass()) histo1DD[channel]->Fill(1);
     if(!digi.zsMarkAndPass()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(0);      
     }

     sprintf(channel,   "h_DigzsUnsuppressed_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     sprintf(channel_np,"h_DigzsUnsuppressed_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsUnsuppressed()) histo1DD[channel]->Fill(1);
     if(!digi.zsUnsuppressed()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(0);           
     }
     
     sprintf(channel,   "h_DigfiberIdleOffset_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth()); 
     sprintf(channel_np,"h_DigfiberIdleOffset_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     histo1DD[channel]->Fill(digi.fiberIdleOffset());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.fiberIdleOffset());
          	            
    }   
  }
}

  if(!hbhe_digi.failedToGet()){

    for(HBHEDigiCollection::const_iterator d=hbhe_digi->begin(); d!=hbhe_digi->end(); d++){
      const HBHEDataFrame digi = (const HBHEDataFrame)(*d);

     if(digi.id().ieta()==ieta && digi.id().iphi()==iphi && digi.id().subdet()==subDet && digi.id().depth()==depth){ 

    sprintf(channel,   "h_NormDigit_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_NormDigit_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     histo1DNormDigit[channel]->Fill(0.5); 
     if(raw_problem[count]==0) histo1DNormDigit_np[channel_np]->Fill(0.5); 
      
     sprintf(channel,   "h_PulseShape_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     sprintf(channel_np,"h_PulseShape_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());          
     for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
     if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
    
     sprintf(channel,   "h_DigSize_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigSize_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.size());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.size());
     
     sprintf(channel,   "h_DigPresamples_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigPresamples_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.presamples());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.presamples());
      
     sprintf(channel,   "h_DigTotCharge_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotCharge_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totcharge =0;
     for(int i=0; i<digi.size(); i++) totcharge = totcharge+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totcharge); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totcharge); 
           
     sprintf(channel,   "h_DigTotChargePreSample_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePreSample_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totchargep =0;
     for(int i=0; i<digi.presamples(); i++) totchargep = totchargep+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totchargep); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totchargep); 
     
     Double_t ped = (totchargep/digi.presamples())*digi.size(); 
     
     sprintf(channel,   "h_DigTotChargePedSub_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePedSub_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     if((totcharge-ped) >  0 ) histo1DD[channel]->Fill(totcharge-ped);
     if((totcharge-ped) <= 0 ) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if((totcharge-ped) >  0 ) histo1DD_np[channel_np]->Fill(totcharge-ped);
      if((totcharge-ped) <= 0 ) histo1DD_np[channel_np]->Fill(0);
     }
     
     
     Double_t thres = (totchargep/digi.presamples())*digi.size()+ChargeAbovePed; 
     sprintf(channel,   "h_PulseShapeThres_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_PulseShapeThres_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     if(totcharge>thres) {
      evts++;
      for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
      if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
      
      //Count how many times the number above was grater than zero.
      sprintf(channel,   "h_PrecentageSignal_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      sprintf(channel_np,"h_PrecentageSignal_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      histo1DD[channel]->Fill(0.5,evts);
      if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(0.5,evts);
     }
         
     sprintf(channel,   "h_DigzsMarkAndPass_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());   
     sprintf(channel_np,"h_DigzsMarkAndPass_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsMarkAndPass()) histo1DD[channel]->Fill(1);
     if(!digi.zsMarkAndPass()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(0);      
     }

     sprintf(channel,   "h_DigzsUnsuppressed_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     sprintf(channel_np,"h_DigzsUnsuppressed_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsUnsuppressed()) histo1DD[channel]->Fill(1);
     if(!digi.zsUnsuppressed()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(0);           
     }
     
     sprintf(channel,   "h_DigfiberIdleOffset_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth()); 
     sprintf(channel_np,"h_DigfiberIdleOffset_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     histo1DD[channel]->Fill(digi.fiberIdleOffset());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.fiberIdleOffset());
          
      
    }
  }
}
 
  if(!hf_digi.failedToGet()){
    for(HFDigiCollection::const_iterator d=hf_digi->begin(); d!=hf_digi->end(); d++){
      const HFDataFrame digi = (const HFDataFrame)(*d);

    if(digi.id().ieta()==ieta && digi.id().iphi()==iphi && digi.id().subdet()==subDet && digi.id().depth()==depth){ 
 
    sprintf(channel,   "h_NormDigit_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_NormDigit_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     histo1DNormDigit[channel]->Fill(0.5); 
     if(raw_problem[count]==0) histo1DNormDigit_np[channel_np]->Fill(0.5); 
      
     sprintf(channel,   "h_PulseShape_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     sprintf(channel_np,"h_PulseShape_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());          
     for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
     if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
    
     sprintf(channel,   "h_DigSize_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigSize_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.size());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.size());
     
     sprintf(channel,   "h_DigPresamples_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigPresamples_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     histo1DD[channel]->Fill(digi.presamples());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.presamples());
      
     sprintf(channel,   "h_DigTotCharge_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotCharge_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totcharge =0;
     for(int i=0; i<digi.size(); i++) totcharge = totcharge+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totcharge); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totcharge); 
           
     sprintf(channel,   "h_DigTotChargePreSample_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePreSample_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     Double_t totchargep =0;
     for(int i=0; i<digi.presamples(); i++) totchargep = totchargep+ digi[i].nominal_fC();
     histo1DD[channel]->Fill(totchargep); 
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(totchargep); 
     
     Double_t ped = (totchargep/digi.presamples())*digi.size(); 
     
     sprintf(channel,   "h_DigTotChargePedSub_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_DigTotChargePedSub_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     if((totcharge-ped) >  0 ) histo1DD[channel]->Fill(totcharge-ped);
     if((totcharge-ped) <= 0 ) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if((totcharge-ped) >  0 ) histo1DD_np[channel_np]->Fill(totcharge-ped);
      if((totcharge-ped) <= 0 ) histo1DD_np[channel_np]->Fill(0);
     }
     
     
     Double_t thres = (totchargep/digi.presamples())*digi.size()+ChargeAbovePed; 
     sprintf(channel,   "h_PulseShapeThres_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
     sprintf(channel_np,"h_PulseShapeThres_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());    
     if(totcharge>thres) {
      evts++;
      for(int i=0; i<digi.size(); i++) histo1DD[channel]->Fill(i,digi[i].nominal_fC());
      if(raw_problem[count]==0) for(int i=0; i<digi.size(); i++) histo1DD_np[channel_np]->Fill(i,digi[i].nominal_fC());
      
      //Count how many times the number above was grater than zero.
      sprintf(channel,   "h_PrecentageSignal_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      sprintf(channel_np,"h_PrecentageSignal_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());
      histo1DD[channel]->Fill(0.5,evts);
      if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(0.5,evts);
     }
         
     sprintf(channel,   "h_DigzsMarkAndPass_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());   
     sprintf(channel_np,"h_DigzsMarkAndPass_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsMarkAndPass()) histo1DD[channel]->Fill(1);
     if(!digi.zsMarkAndPass()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsMarkAndPass()) histo1DD_np[channel_np]->Fill(0);      
     }

     sprintf(channel,   "h_DigzsUnsuppressed_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());     
     sprintf(channel_np,"h_DigzsUnsuppressed_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     if( digi.zsUnsuppressed()) histo1DD[channel]->Fill(1);
     if(!digi.zsUnsuppressed()) histo1DD[channel]->Fill(0);
     if(raw_problem[count]==0) {
      if( digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(1);
      if(!digi.zsUnsuppressed()) histo1DD_np[channel_np]->Fill(0);           
     }
     
     sprintf(channel,   "h_DigfiberIdleOffset_DetId%d_%d_%d_%d",    digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth()); 
     sprintf(channel_np,"h_DigfiberIdleOffset_np_DetId%d_%d_%d_%d", digi.id().subdet(),digi.id().ieta() , digi.id().iphi(), digi.id().depth());      
     histo1DD[channel]->Fill(digi.fiberIdleOffset());
     if(raw_problem[count]==0) histo1DD_np[channel_np]->Fill(digi.fiberIdleOffset());
	  
     }
    }
  }
  
 count=count+1;
 
}


 /*
 // TRIGGERED PRIMITIVES
 
Handle<HcalTrigPrimDigiCollection> tpDigis;
iEvent.getByType(tpDigis);
  
for(std::vector<HcalDetId>::const_iterator cit = channelsd.begin(); cit!=channelsd.end();cit++){
 
  int subDet    = cit->subdet();
  int ieta      = cit->ieta();
  int iphi      = cit->iphi();
  int depth     = cit->depth();  
  
  if(!tpDigis.failedToGet()){
    
   for(HcalTrigPrimDigiCollection::const_iterator tpj=tpDigis->begin(); tpj!=tpDigis->end();tpj++)
    { 
      const HcalTriggerPrimitiveDigi digi = (const HcalTriggerPrimitiveDigi)(*tpj);
      
      HcalTrigTowerDetId tpid=digi.id(); 
      
      // Find the channel
      if(tpid.ieta() == ieta && tpid.iphi() == iphi && tpid.subdet() == subDet && tpid.depth()==depth){
      
	 sprintf(channel,"h_NormTPG_DetId%d_%d_%d_%d",subDet,ieta,iphi,depth);
	 histo1DNormTPG[channel]->Fill(0.5);
	 
        // For Digis
      
        // size
	
	// presamples
	
        // zsMarkAndPass
	
	// zsUnsuppressed
     
      } 
 
   }
   
  }
 
}
 // END OF TRIGGERED PRIMITIVES

*/ 
}


// ------------ method called once each job just before starting event loop  ------------
void HcalChanMon::beginJob(){

  evt         = 0;
  evts        = 0;

}

//********************************************************************************************************
TH1D *HcalChanMon::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
				int fNbins, double fXmin, double fXmax) const {
  char title[1024];

  sprintf(title, "%s RUN:%d ", fTitle.c_str(), runBegin);
  return fDir.make < TH1D > (fName.c_str(), title, fNbins, fXmin, fXmax);
}
//********************************************************************************************************
TH2D *HcalChanMon::book2DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
				int fNbinsX, double fXmin, double fXmax, int fNbinsY, double fYmin, double fYmax) const {
  char title[1024];

  sprintf(title, "%s [RUN:%d]", fTitle.c_str(), runBegin);
  return fDir.make < TH2D > (fName.c_str(), title, fNbinsX, fXmin, fXmax, fNbinsY, fYmin, fYmax);
}
//********************************************************************************************************
void HcalChanMon::bookHistograms() {
 
  TFileDirectory ChannelPlotsDirRaw     = fs->mkdir( "ChannelPlotsRAW" );
  TFileDirectory ChannelPlotsDirDigits  = fs->mkdir( "ChannelPlotsDIGITS" );
  TFileDirectory ChannelPlotsDirRecHits = fs->mkdir( "ChannelPlotsRECHITS");
  
  for(Parameters::iterator iChannel = ChannelList_.begin(); iChannel != ChannelList_.end(); ++iChannel ) { 
   
   int subDet_    = iChannel->getUntrackedParameter < uint32_t > ("subDet");
   int ieta_      = iChannel->getUntrackedParameter < int32_t > ("ieta");
   int iphi_      = iChannel->getUntrackedParameter < int32_t > ("iphi");
   int depth_     = iChannel->getUntrackedParameter < int32_t > ("depth");
 
   HcalDetId here(HcalSubdetector(subDet_), ieta_, iphi_, depth_);

   HcalElectronicsId there(readoutMap_->lookup(here));

   channelse.push_back(there);
   channelsd.push_back(here);

   /////create pulse shape plots for all channels listed////////////////////////////////////////////////////
   
    sprintf(name,"h_NormRaw_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Number of Events DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DNormRaw[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 1, 0, 1);
    histo1DNormRaw[name]->GetYaxis()->SetTitle("Number of Events");
    histo1DNormRaw[name]->GetXaxis()->SetTitle("Dummy");

    ///// CDFE ERROR FOUND  ////////////////////////////////////////////////////

    sprintf(name,"h_CDFErrorFound_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"CDFE ERROR  DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 10, 0, 10);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("Error");
    histo1D[name]->GetXaxis()->SetBinLabel(1, "Hdr1BitUnset");
    histo1D[name]->GetXaxis()->SetBinLabel(2, "FmtNumChange");
    histo1D[name]->GetXaxis()->SetBinLabel(3, "EvTypChange");
    histo1D[name]->GetXaxis()->SetBinLabel(4, "BOE not '0x5'");
    histo1D[name]->GetXaxis()->SetBinLabel(5, "Hdr2Bit Set");
    histo1D[name]->GetXaxis()->SetBinLabel(6, "Hdr1 36-59");
    histo1D[name]->GetXaxis()->SetBinLabel(7, "BOE not 0");
    histo1D[name]->GetXaxis()->SetBinLabel(8, "Trlr1Bit Set");
    histo1D[name]->GetXaxis()->SetBinLabel(9, "Size Error");
    histo1D[name]->GetXaxis()->SetBinLabel(10, "TrailerBad");

    ///// DCC EVENT FORMAT VIOLATION ////////////////////////////////////////////////////

    sprintf(name,"h_DCCEventFormatError_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"DCC Event Format ERROR  DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 4, 0, 4);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("Error");
    histo1D[name]->GetXaxis()->SetBinLabel(1, "FmtVersChng");
    histo1D[name]->GetXaxis()->SetBinLabel(2, "StrayBits");
    histo1D[name]->GetXaxis()->SetBinLabel(3, "HTRStatusPad");
    histo1D[name]->GetXaxis()->SetBinLabel(4, "32bitPadErr");
 
    ///// DCC ERROR AND WARNING CONDITIONS ////////////////////////////////////////////////////
 
    sprintf(name,"h_DCCErrorAndWarningConditions_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"DCC Error And Warning Conditions  DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);
   
    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 11, 0, 11);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("Error");   
    histo1D[name]->GetXaxis()->SetBinLabel(1, "Spigot MisM");
    histo1D[name]->GetXaxis()->SetBinLabel(2, "TTS_OFW");
    histo1D[name]->GetXaxis()->SetBinLabel(3, "TTS_BSY");
    histo1D[name]->GetXaxis()->SetBinLabel(4, "TTS_SYN");
    histo1D[name]->GetXaxis()->SetBinLabel(5, "L1A_EvN Mis");
    histo1D[name]->GetXaxis()->SetBinLabel(6, "L1A_BcN Mis");
    histo1D[name]->GetXaxis()->SetBinLabel(7, "CT_EvN Mis");
    histo1D[name]->GetXaxis()->SetBinLabel(8, "CT_BcN Mis");
    histo1D[name]->GetXaxis()->SetBinLabel(9, "OrbitLenEr");
    histo1D[name]->GetXaxis()->SetBinLabel(10,"TTC_SingEr");
    histo1D[name]->GetXaxis()->SetBinLabel(11,"TTC_DoubEr");

    ///// DCC VIEW OF SPIGOT CONDITIONS  ////////////////////////////////////////////////////
 
    sprintf(name,"h_SummariesOfHTRS_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"DCC Summaries of HTRS  DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);
    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 20, 0, 20);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("Error");   
    
    histo1D[name]->GetXaxis()->SetBinLabel(1, "HTR OFW");
    histo1D[name]->GetXaxis()->SetBinLabel(2, "HTR BSY");
    histo1D[name]->GetXaxis()->SetBinLabel(3, "Empty Events");
    histo1D[name]->GetXaxis()->SetBinLabel(4, "L1A Reject");
    histo1D[name]->GetXaxis()->SetBinLabel(5, "Latency Er");
    histo1D[name]->GetXaxis()->SetBinLabel(6, "Latncy Warn");
    histo1D[name]->GetXaxis()->SetBinLabel(7, "Optcl Data Err");
    histo1D[name]->GetXaxis()->SetBinLabel(8, "Clock");
    histo1D[name]->GetXaxis()->SetBinLabel(9, "CorrHamm LRB");
    histo1D[name]->GetXaxis()->SetBinLabel(10,"UncorrHam");
    histo1D[name]->GetXaxis()->SetBinLabel(11,"LRB Block OvF");
    histo1D[name]->GetXaxis()->SetBinLabel(12,"LRB EvN Hdr/Tlr");
    histo1D[name]->GetXaxis()->SetBinLabel(13,"FIFOs Empty");
    histo1D[name]->GetXaxis()->SetBinLabel(14,"LRB Trunct");
    histo1D[name]->GetXaxis()->SetBinLabel(15,"LRB No Hdr/tlr");
    histo1D[name]->GetXaxis()->SetBinLabel(16,"Odd 16-Bit Wd Cnt");
    histo1D[name]->GetXaxis()->SetBinLabel(17,"Spgt E not P");
    histo1D[name]->GetXaxis()->SetBinLabel(18,"Spgt BcN Mis");
    histo1D[name]->GetXaxis()->SetBinLabel(19,"P not V");
    histo1D[name]->GetXaxis()->SetBinLabel(20,"Trunct by LRB");
 
          
    ///// HTR ERROR WORD  ////////////////////////////////////////////////////

    sprintf(name,"h_HTRErrorWord_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"HTR ERROR WORD DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 16, 0, 16);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("Error Bit");
    histo1D[name]->GetXaxis()->SetBinLabel(1,"Overflow Warn");  
    histo1D[name]->GetXaxis()->SetBinLabel(2, "Buffer Busy"); 
    histo1D[name]->GetXaxis()->SetBinLabel(3, "Empty Event"); 
    histo1D[name]->GetXaxis()->SetBinLabel(4, "Reject L1A"); 
    histo1D[name]->GetXaxis()->SetBinLabel(5, "Latency Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(6, "Latency Warn"); 
    histo1D[name]->GetXaxis()->SetBinLabel(7, "OptData Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(8, "Clock Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(9, "Bunch Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(10,"Link Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(11,"CAPID Error"); 
    histo1D[name]->GetXaxis()->SetBinLabel(12,"FE Format Err");  
    histo1D[name]->GetXaxis()->SetBinLabel(13,"Test Mode"); 
    histo1D[name]->GetXaxis()->SetBinLabel(14,"Histo Mode"); 
    histo1D[name]->GetXaxis()->SetBinLabel(15,"Calib Trig"); 
    histo1D[name]->GetXaxis()->SetBinLabel(16,"Bit15 Err");  
   
   ///// QIE DATA VALIDITY PLOT FOR ALL CHANNELS LISTED ////////////////////////////////////////////////////

    sprintf(name,"h_QIE_Validity_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"QIE Validity for each Time Slice DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 10, 0, 10);
    histo1D[name]->GetYaxis()->SetTitle("QIE Data Validity (Percentage)");
    histo1D[name]->GetXaxis()->SetTitle("Time Slice");

   ///// QIE DATA ERROR PLOT FOR ALL CHANNELS LISTED ////////////////////////////////////////////////////

    sprintf(name,"h_QIE_Error_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"QIE Error for each Time Slice DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 10, 0, 10);
    histo1D[name]->GetYaxis()->SetTitle("QIE Data Error (Percentage)");
    histo1D[name]->GetXaxis()->SetTitle("Time Slice");

 ///// QIE DATA ERROR PLOT FOR ALL CHANNELS LISTED ////////////////////////////////////////////////////

    sprintf(name,"h_QIE_fC_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"QIE Nominal fC for each Time Slice DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 10, 0, 10);
    histo1D[name]->GetYaxis()->SetTitle("QIE Nominal fC (Average over Run)");
    histo1D[name]->GetXaxis()->SetTitle("Time Slice");

    sprintf(name,"h_QIE_CAPIDERR_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"QIE_CAPID Rotation Error DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1D[name]   =  book1DHistogram(ChannelPlotsDirRaw, name, title, 2, 0, 2);
    histo1D[name]->GetYaxis()->SetTitle("Percentage");
    histo1D[name]->GetXaxis()->SetTitle("CAPID Error");

///// DIGIT PLOTS /////////////////////////////////////////////////////////////////////////////////////////

    sprintf(name,"h_NormDigit_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Number of Events DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DNormDigit[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 1, 0, 1);
    histo1DNormDigit[name]->GetYaxis()->SetTitle("Number of Events");
    histo1DNormDigit[name]->GetXaxis()->SetTitle("Dummy");

    sprintf(name,"h_PulseShape_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Pulse Shape of Channel DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 10, 0, 10);
    histo1DD[name]->GetYaxis()->SetTitle("Nominal fC (Average over Run)");
    histo1DD[name]->GetXaxis()->SetTitle("Time Slice");
    
    sprintf(name,"h_DigSize_DetId%d_%d_%d_%d",subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Digit Size DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD[name]->GetXaxis()->SetTitle("Digit Size (Number of TimeSlices)");
    
    
    sprintf(name,"h_DigPresamples_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Digit Presamples DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD[name]->GetXaxis()->SetTitle("Digit Presamples");
    
       
    sprintf(name,"h_DigzsMarkAndPass_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit ZS MarkAndPass DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 2, 0, 2);
    histo1DD[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD[name]->GetXaxis()->SetTitle("Digit ZS MarkAndPass");
 
    sprintf(name,"h_DigzsUnsuppressed_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit ZS Unsuppressed DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 2, 0, 2);
    histo1DD[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD[name]->GetXaxis()->SetTitle("Digit ZS Unsuppressed");
    
    sprintf(name,"h_DigfiberIdleOffset_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit FiberIdleOffset_ DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD[name]->GetXaxis()->SetTitle("FiberIdleOffset");
       

     sprintf(name,"h_DigTotCharge_DetId%d_%d_%d_%d",subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD[name]->GetXaxis()->SetTitle("Total Charge");
   
          
     sprintf(name,"h_DigTotChargePreSample_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge in Presamples DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD[name]->GetXaxis()->SetTitle("Total Charge of Presamples");
   
    
     sprintf(name,"h_DigTotChargePedSub_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge Pedestal Subtracted DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD[name]->GetXaxis()->SetTitle("Total Charge Pedestal Subtracted");
   
   
     sprintf(name,"h_PulseShapeThres_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Nominal fC Above Threshold DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 10, 0, 10);
     histo1DD[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD[name]->GetXaxis()->SetTitle("Pulse Shape above Threshold");
    
     sprintf(name,"h_PrecentageSignal_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Percentage with Signal DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 1, 0, 1);
     histo1DD[name]->GetYaxis()->SetTitle("Percentage");
     histo1DD[name]->GetXaxis()->SetTitle("Non Empty Digits");
          	 
		 
    // rec hits
    
      sprintf(name,"h_NormRecHit_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"Percentage of Events DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DNormRecHit[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,1,0,1); 
      histo1DNormRecHit[name]->GetYaxis()->SetTitle("Number of Events");
      histo1DNormRecHit[name]->GetXaxis()->SetTitle("Dummy Variable");
      
      sprintf(name,"h_RecHitEnergy_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Energy DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,400,0.,50.);
      histo1DR[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR[name]->GetXaxis()->SetTitle("Energy (GeV)");

      sprintf(name,"h_RecHitTime_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Time DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,200,-100.,100.);    
      histo1DR[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR[name]->GetXaxis()->SetTitle("Time (sec)");

      sprintf(name,"h_RecHitFlags_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit FLAGS DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,10,0,10);
      histo1DR[name]->GetYaxis()->SetTitle("Percentage");
      histo1DR[name]->GetXaxis()->SetTitle("Flags");

      sprintf(name,"h_RecHitEnergyThres_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Energy above Threshold DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,400,0.,50.);
      histo1DR[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR[name]->GetXaxis()->SetTitle("Energy (GeV)");


//  NO PROBLEM DIGITS AND REC HIT PLOTS



///// DIGIT PLOTS /////////////////////////////////////////////////////////////////////////////////////////

    sprintf(name,"h_NormDigit_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Number of Events DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DNormDigit_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 1, 0, 1);
    histo1DNormDigit_np[name]->GetYaxis()->SetTitle("Number of Events");
    histo1DNormDigit_np[name]->GetXaxis()->SetTitle("Dummy");

    sprintf(name,"h_PulseShape_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Pulse Shape of Channel DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 10, 0, 10);
    histo1DD_np[name]->GetYaxis()->SetTitle("Nominal fC (Average over Run)");
    histo1DD_np[name]->GetXaxis()->SetTitle("Time Slice");
    
    sprintf(name,"h_DigSize_np_DetId%d_%d_%d_%d",subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Digit Size DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD_np[name]->GetXaxis()->SetTitle("Digit Size (Number of TimeSlices)");
    
    
    sprintf(name,"h_DigPresamples_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);
    sprintf(title,"Digit Presamples DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD_np[name]->GetXaxis()->SetTitle("Digit Presamples");
    
       
    sprintf(name,"h_DigzsMarkAndPass_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit ZS MarkAndPass DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 2, 0, 2);
    histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD_np[name]->GetXaxis()->SetTitle("Digit ZS MarkAndPass");
 
    sprintf(name,"h_DigzsUnsuppressed_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit ZS Unsuppressed DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 2, 0, 2);
    histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD_np[name]->GetXaxis()->SetTitle("Digit ZS Unsuppressed");
    
    sprintf(name,"h_DigfiberIdleOffset_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
    sprintf(title,"Digit FiberIdleOffset_ DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

    histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 40, 0, 40);
    histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
    histo1DD_np[name]->GetXaxis()->SetTitle("FiberIdleOffset");
       

     sprintf(name,"h_DigTotCharge_np_DetId%d_%d_%d_%d",subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD_np[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD_np[name]->GetXaxis()->SetTitle("Total Charge");
   
          
     sprintf(name,"h_DigTotChargePreSample_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge in Presamples DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD_np[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD_np[name]->GetXaxis()->SetTitle("Total Charge of Presamples");
   
    
     sprintf(name,"h_DigTotChargePedSub_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Total Charge Pedestal Subtracted DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 100, 0, 100);
     histo1DD_np[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD_np[name]->GetXaxis()->SetTitle("Total Charge Pedestal Subtracted");
   
   
     sprintf(name,"h_PulseShapeThres_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Nominal fC Above Threshold DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 10, 0, 10);
     histo1DD_np[name]->GetYaxis()->SetTitle("Average Over Run");
     histo1DD_np[name]->GetXaxis()->SetTitle("Pulse Shape above Threshold");
    
     sprintf(name,"h_PrecentageSignal_np_DetId%d_%d_%d_%d", subDet_, ieta_, iphi_, depth_);      
     sprintf(title,"Digit Percentage with Signal DetId%d %d %d %d", subDet_, ieta_, iphi_, depth_);

     histo1DD_np[name]   =  book1DHistogram(ChannelPlotsDirDigits, name, title, 1, 0, 1);
     histo1DD_np[name]->GetYaxis()->SetTitle("Percentage");
     histo1DD_np[name]->GetXaxis()->SetTitle("Non Empty Digits");
          	 
		 
    // rec hits
    
      sprintf(name,"h_NormRecHit_np_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"Percentage of Events DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DNormRecHit_np[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,1,0,1); 
      histo1DNormRecHit_np[name]->GetYaxis()->SetTitle("Number of Events");
      histo1DNormRecHit_np[name]->GetXaxis()->SetTitle("Dummy Variable");
      
      sprintf(name,"h_RecHitEnergy_np_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Energy DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR_np[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,400,0.,50.);
      histo1DR_np[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR_np[name]->GetXaxis()->SetTitle("Energy (GeV)");

      sprintf(name,"h_RecHitTime_np_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Time DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR_np[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,200,-100.,100.);    
      histo1DR_np[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR_np[name]->GetXaxis()->SetTitle("Time (sec)");

      sprintf(name,"h_RecHitFlags_np_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit FLAGS DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR_np[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,10,0,10);
      histo1DR_np[name]->GetYaxis()->SetTitle("Percentage");
      histo1DR_np[name]->GetXaxis()->SetTitle("Flags");

      sprintf(name,"h_RecHitEnergyThres_np_DetId%d_%d_%d_%d",subDet_,ieta_,iphi_,depth_);
      sprintf(title,"RecHit Energy above Threshold DetId%d %d %d %d",subDet_,ieta_,iphi_,depth_);
      
      histo1DR_np[name] = book1DHistogram(ChannelPlotsDirRecHits,name,title,400,0.,50.);
      histo1DR_np[name]->GetYaxis()->SetTitle("Percentage Over Run");
      histo1DR_np[name]->GetXaxis()->SetTitle("Energy (GeV)");





// END OF NO PROBLEM PLOTS

      sprintf(channel,"ieta_%d_iphi_%d_depth%d", ieta_, iphi_, depth_);
      myChannel[channel]=1;

  }
}
// ------------ method called once each job just after ending the event loop  ------------
void HcalChanMon::endJob() {
 
  for(std::map<std::string, TH1D*>::iterator it=histo1D.begin();it!=histo1D.end();it++){
   
   std::string chan2,chan3;
   size_t pos2,pos3,pos4;
  
   std::string chan = it->first; // here tra la la 
   TH1D *histo      = it->second;
   
  
   pos2   = chan.find("DetId");    // position of "DetId" in str
   chan2  = chan.substr(pos2);   // get from "DetId" to the end
   
   pos4  = chan.find("QIE_");
 
 
   TH1D *histon = 0;
 
   for(std::map<std::string, TH1D*>::iterator itn=histo1DNormRaw.begin();itn!=histo1DNormRaw.end();itn++){
    std::string chann  = itn->first; // here tra la la 
    pos3   = chann.find("DetId");    // position of "DetId" in str
    chan3  = chann.substr(pos3);    // get from "DetId" to the end
   
    if(chan3==chan2) histon       = itn->second;     
   }
     
   if(histon && pos4>0 && pos4 <200  && histon->GetBinContent(1)>0) histo ->Scale(1./histon->GetBinContent(1)); 
   else       histo ->Scale(1./evt); 
  
  
 }
  
  
  for(std::map<std::string, TH1D*>::iterator itn=histo1DNormRaw.begin();itn!=histo1DNormRaw.end();itn++){
    TH1D *histon       = itn->second;  
    histon->Scale(1./evt);        
  }
   
 
// DIGITS 
 for(std::map<std::string, TH1D*>::iterator itd=histo1DD.begin();itd!=histo1DD.end();itd++){

   std::string chan2,chan3;
   size_t pos2,pos3;

   std::string chan = itd->first;   // here tra la la 
   TH1D *histo      = itd->second;
    
   pos2   = chan.find("DetId");    // position of "DetId" in str
   chan2  = chan.substr(pos2);     // get from "DetId" to the end
   
   TH1D *histon = 0;
 
   for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormDigit.begin();itdn!=histo1DNormDigit.end();itdn++){
    std::string chann  = itdn->first; // here tra la la 
    pos3   = chann.find("DetId");     // position of "DetId" in str
    chan3  = chann.substr(pos3);      // get from "DetId" to the end
   
    if(chan3==chan2) histon       = itdn->second;     
   }
     
   if(histon && histon->GetBinContent(1)>0) histo ->Scale(1./histon->GetBinContent(1)); 
  
 }  

  for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormDigit.begin();itdn!=histo1DNormDigit.end();itdn++){ 
    TH1D *histon       = itdn->second;     
    histon->Scale(1./evt);      
   }

// DIGITS NO PROBLEM


 for(std::map<std::string, TH1D*>::iterator itd=histo1DD_np.begin();itd!=histo1DD_np.end();itd++){

   std::string chan2,chan3;
   size_t pos2,pos3;

   std::string chan = itd->first;   // here tra la la 
   TH1D *histo      = itd->second;
    
   pos2   = chan.find("DetId");    // position of "DetId" in str
   chan2  = chan.substr(pos2);     // get from "DetId" to the end
   
   TH1D *histon = 0;
 
   for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormDigit_np.begin();itdn!=histo1DNormDigit_np.end();itdn++){
    std::string chann  = itdn->first; // here tra la la 
    pos3   = chann.find("DetId");     // position of "DetId" in str
    chan3  = chann.substr(pos3);      // get from "DetId" to the end
   
    if(chan3==chan2) histon       = itdn->second;     
   }
     
   if(histon && histon->GetBinContent(1)>0) histo ->Scale(1./histon->GetBinContent(1)); 
  
 }  

  for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormDigit_np.begin();itdn!=histo1DNormDigit_np.end();itdn++){ 
    TH1D *histon       = itdn->second;     
    histon->Scale(1./evt);      
   }
   
// END OF DIGITS NO PROBLEM

// REC HITS

 for(std::map<std::string, TH1D*>::iterator itd=histo1DR.begin();itd!=histo1DR.end();itd++){

   std::string chan2,chan3;
   size_t pos2,pos3;

   std::string chan = itd->first;   // here tra la la
   TH1D *histo      = itd->second;

   pos2   = chan.find("DetId");    // position of "DetId" in str
   chan2  = chan.substr(pos2);     // get from "DetId" to the end

   TH1D *histon = 0;

   for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormRecHit.begin();itdn!=histo1DNormRecHit.end();itdn++){
    std::string chann  = itdn->first; // here tra la la
    pos3   = chann.find("DetId");     // position of "DetId" in str
    chan3  = chann.substr(pos3);      // get from "DetId" to the end

    if(chan3==chan2) histon       = itdn->second;
   }

   if(histon && histon->GetBinContent(1)>0) histo ->Scale(1./histon->GetBinContent(1));

 }

  for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormRecHit.begin();itdn!=histo1DNormRecHit.end();itdn++){
    TH1D *histon       = itdn->second;
    histon->Scale(1./evt);
   }


// REC HITS NO PROBLEM

 for(std::map<std::string, TH1D*>::iterator itd=histo1DR_np.begin();itd!=histo1DR_np.end();itd++){

   std::string chan2,chan3;
   size_t pos2,pos3;

   std::string chan = itd->first;   // here tra la la
   TH1D *histo      = itd->second;

   pos2   = chan.find("DetId");    // position of "DetId" in str
   chan2  = chan.substr(pos2);     // get from "DetId" to the end

   TH1D *histon = 0;

   for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormRecHit_np.begin();itdn!=histo1DNormRecHit_np.end();itdn++){
    std::string chann  = itdn->first; // here tra la la
    pos3   = chann.find("DetId");     // position of "DetId" in str
    chan3  = chann.substr(pos3);      // get from "DetId" to the end

    if(chan3==chan2) histon       = itdn->second;
   }

   if(histon && histon->GetBinContent(1)>0) histo ->Scale(1./histon->GetBinContent(1));

 }

  for(std::map<std::string, TH1D*>::iterator itdn=histo1DNormRecHit_np.begin();itdn!=histo1DNormRecHit_np.end();itdn++){
    TH1D *histon       = itdn->second;
    histon->Scale(1./evt);
   }
   
// END OF REC HITS NO PROBLEM

   
}

