#include "GeneratorInterface/MadGraphInterface/interface/STLesHouchesMixerSource.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;

STLesHouchesMixerSource::STLesHouchesMixerSource( const ParameterSet & pset, InputSourceDescription const& desc) : ExternalInputSource (pset, desc), inputLHEFile22_ (fileNames()[0]), inputLHEFile23_ (fileNames()[1]){
  std::cout<<"Generator|MadGraph "<<" initializing STLesHouchesMixerSource"<<std::endl;
  std::cout<<"Generator|MadGraph "<<" fileNames().size() "<<fileNames().size()<<std::endl;
  // strip the input file name
  if ( inputLHEFile22_.find("file:") || inputLHEFile22_.find("rfio:")){ inputLHEFile22_.erase(0,5); }
  if ( inputLHEFile23_.find("file:") || inputLHEFile23_.find("rfio:")){ inputLHEFile23_.erase(0,5); }
  std::cout<<"Generator|MadGraph "<<" inputLHEFile22_ = "<<inputLHEFile22_<<std::endl;
  std::cout<<"Generator|MadGraph "<<" inputLHEFile23_ = "<<inputLHEFile23_<<std::endl;
}


STLesHouchesMixerSource::~STLesHouchesMixerSource(){
  edm::LogInfo("Generator|STLesHouchesMixerSource")<<"ending STLesHouchesMixerSource";
}

bool STLesHouchesMixerSource::produce(Event & e) {
  std::cout<<" event() = "<<event()<<std::endl;
  if (event() > 10 ) return false; // stop after 10 events
  else  return true;
}

