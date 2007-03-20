/** \file
 * Implementation of class RPCRecordFormatter
 *
 *  $Date: 2007/03/09 11:41:16 $
 *  $Revision: 1.25 $
 *
 * \author Ilaria Segoni
 */

#include "EventFilter/RPCRawToDigi/interface/RPCRecordFormatter.h"
#include "EventFilter/RPCRawToDigi/interface/RPCLinkBoardData.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/RPCDigi/interface/RPCDigiCollection.h"
#include "DataFormats/RPCDigi/interface/RPCDigi.h"

#include "CondFormats/RPCObjects/interface/RPCReadOutMapping.h"
#include "CondFormats/RPCObjects/interface/ChamberLocationSpec.h"
#include "CondFormats/RPCObjects/interface/ChamberRawDataSpec.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "EventFilter/RPCRawToDigi/interface/EventRecords.h"

#include <vector>
#include <bitset>
#include <sstream>

using namespace std;
using namespace edm;
using namespace rpcrawtodigi;


RPCRecordFormatter::RPCRecordFormatter(int fedId, const RPCReadOutMapping *r)
 : currentFED(fedId), readoutMapping(r){
}

RPCRecordFormatter::~RPCRecordFormatter(){
}


std::vector<EventRecords> RPCRecordFormatter::recordPack(
    uint32_t rawDetId, const RPCDigi & digi, int trigger_BX) const 
{
  std::vector<EventRecords> result;
   
  LogTrace("") << " DIGI;  det: " << rawDetId<<", strip: "<<digi.strip()<<", bx: "<<digi.bx();
  int stripInDU = digi.strip();

  // decode digi<->map
  std::pair< ChamberRawDataSpec, LinkBoardChannelCoding>
      rawDataFrame = readoutMapping->rawDataFrame(rawDetId, stripInDU);

  const ChamberRawDataSpec & eleIndex = rawDataFrame.first;
  const LinkBoardChannelCoding & channelCoding = rawDataFrame.second;
  if (eleIndex.dccId == currentFED) {

    LogTrace("pack:")
         <<" dccId= "<<eleIndex.dccId
         <<" dccInputChannelNum= "<<eleIndex.dccInputChannelNum
         <<" tbLinkInputNum= "<<eleIndex.tbLinkInputNum
         <<" lbNumInLink="<<eleIndex.lbNumInLink;


    // BX 
    int current_BX = trigger_BX+digi.bx();
    BXRecord bxr(current_BX);
   

    // TB 
    int tbLinkInputNumber = eleIndex.tbLinkInputNum;
    int rmb = eleIndex.dccInputChannelNum; 
    TBRecord tbr( tbLinkInputNumber, rmb);   

    // LB record
    RPCLinkBoardData lbData;
    lbData.setLbNumber(eleIndex.lbNumInLink);
    lbData.setEod(0);
    lbData.setHalfP(0);
    int channel = channelCoding.channel();
    vector<int> bitsOn; bitsOn.push_back(channel);                        
    lbData.setPartitionNumber( channel/8 );
    lbData.setBits(bitsOn);
    LBRecord lbr(lbData);

    result.push_back(  EventRecords(trigger_BX, bxr, tbr, lbr) );
  }
  return result;
}

void RPCRecordFormatter::recordUnpack(
    const EventRecords & event, std::auto_ptr<RPCDigiCollection> & prod)
{
  int triggerBX = event.triggerBx();
  int currentBX = event.bxRecord().bx();
  int currentRMB = event.tbRecord().rmb(); 
  int currentTbLinkInputNumber = event.tbRecord().tbLinkInputNumber();
  RPCLinkBoardData lbData = event.lbRecord().lbData();

  ChamberRawDataSpec eleIndex;
  eleIndex.dccId = currentFED;
  eleIndex.dccInputChannelNum = currentRMB;
  eleIndex.tbLinkInputNum = currentTbLinkInputNumber;
  eleIndex.lbNumInLink = lbData.lbNumber();


  const LinkBoardSpec* linkBoard = readoutMapping->location(eleIndex);

  if (!linkBoard) {
    throw cms::Exception("Invalid Linkboard location!") 
              << "dccId: "<<eleIndex.dccId
              << "dccInputChannelNum: " <<eleIndex.dccInputChannelNum
              << " tbLinkInputNum: "<<eleIndex.tbLinkInputNum
              << " lbNumInLink: "<<eleIndex.lbNumInLink;
  }

  std::vector<int> bits=lbData.bitsOn();
  for(std::vector<int>::iterator pBit = bits.begin(); pBit != bits.end(); ++pBit){

    int lbBit = *(pBit);
    uint32_t rawDetId;
    int geomStrip;
    try {
      RPCReadOutMapping::StripInDetUnit stripInDetUnit=readoutMapping->strip(eleIndex,lbBit);

      // DetUnit
      rawDetId = stripInDetUnit.first;
	if(!rawDetId) continue;//A dirty fix to avoid crashes. To be FIXED (a geometry should be revised?)
      geomStrip = stripInDetUnit.second;
    } 
    catch (cms::Exception & e) {
      edm::LogInfo("RPC unpacker, exception catched, skip digi")<<e.what(); 
	edm::LogInfo("Values")<< currentRMB<<" "<<currentTbLinkInputNumber<<" "<<lbData.lbNumber();
      continue;
    }
    // Creating RPC digi
    RPCDigi digi(geomStrip,currentBX-triggerBX);

    /// Committing digi to the product
    LogTrace("")<<" DIGI;  det: "<<rawDetId<<", strip: "<<digi.strip()<<", bx: "<<digi.bx();
    LogTrace("") << " ChamberRawDataSpec: " << eleIndex.print(); 
    prod->insertDigi(RPCDetId(rawDetId),digi);
  }
}
