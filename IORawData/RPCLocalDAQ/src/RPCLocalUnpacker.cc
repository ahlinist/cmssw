/** \file
 * Implementation of class RPCLocalUnpacker
 *
 *  $Date: 2006/06/30 19:27:58 $
 *  $Revision: 1.17 $
 *
 * \author Ilaria Segoni
 */

#include "IORawData/RPCLocalDAQ/interface/RPCLocalUnpacker.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCRollSpecs.h"

#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"


#include <iostream>


RPCLocalUnpacker::RPCLocalUnpacker(const edm::ParameterSet& config):maxLBs(33){
  std::cout <<"Initialize the Event Dump"<<std::endl;
  produces<RPCDigiCollection>();

  std::vector<std::string> inputFiles=config.getParameter<std::vector<std::string> >("EventDumps");
  
  rpcdigiprint = config.getParameter<bool>("printOut"); 
  std::string wordBeforeEventData = config.getParameter<std::string>("LastWordBeforeEvent"); 
  std::string wordAfterEventData = config.getParameter<std::string>("FirstWordAfterEvent"); 
  
  if (rpcdigiprint) {
    std::cout<<"Event Dump Digi Creation"<<std::endl;
    std::cout<<"Number of Input files ="<<inputFiles.size()<<std::endl;
  }
    
  fileReader = new RPCFileProcessor(inputFiles, wordBeforeEventData, wordAfterEventData);
}


void
RPCLocalUnpacker::produce(edm::Event & e, const edm::EventSetup& c)
{
  if (rpcdigiprint){
    std::cout <<" Beginning Event : "
	      <<" run="<<e.id().run()
	      <<" event="<<e.id().event()<<std::endl;
  }
  
  std::auto_ptr<RPCDigiCollection> pDigis(new RPCDigiCollection());
  
  fileReader->nextEvent(pDigis);

  if (rpcdigiprint){
    std::cout <<" Event Done : "
	      <<" run="<<e.id().run()
	      <<" event="<<e.id().event()<<std::endl;
  }
  
    
 e.put(pDigis);

  
}

DEFINE_FWK_MODULE(RPCLocalUnpacker)
 
