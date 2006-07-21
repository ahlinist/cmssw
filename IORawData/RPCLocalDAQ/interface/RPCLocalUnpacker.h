#ifndef RPCLocalUnpacker_h
#define RPCLocalUnpacker_h

/** \class RPCLocalUnpacker
 *   Unpacking of Link Board Event Dump
 *
 *  \author Ilaria Segoni 
 */
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <IORawData/RPCLocalDAQ/interface/RPCFileProcessor.h>
#include <DataFormats/RPCDigi/interface/RPCDigiCollection.h>

class RPCLocalUnpacker : public edm::EDProducer {
 public:
  RPCLocalUnpacker(const edm::ParameterSet& config);
  ~RPCLocalUnpacker(){}
  void produce(edm::Event& e, const edm::EventSetup& c);
  
 private:

  bool rpcdigiprint;
  int maxLBs;
  
  RPCFileProcessor * fileReader;


};
#endif
