

#include "DQMServices/ProcessorUtilities/interface/MSService.h"


namespace dqmevf{

  MSService::MSService(const edm::ParameterSet& iPS, 
				       edm::ActivityRegistry& reg)
  {
  
    reg.watchPostBeginJob(this,&MSService::postBeginJob);
    reg.watchPostEndJob(this,&MSService::postEndJob);
  
    reg.watchPreProcessEvent(this,&MSService::preEventProcessing);
    reg.watchPostProcessEvent(this,&MSService::postEventProcessing);
    reg.watchPreSource(this,&MSService::preSource);
    reg.watchPostSource(this,&MSService::postSource);
  
    reg.watchPreModule(this,&MSService::preModule);
    reg.watchPostModule(this,&MSService::postModule);
    microstate1_ = "BJ";
    microstate2_ = "INIT";

  }


  MSService::~MSService()
  {
  }

  void MSService::postBeginJob()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "BJD";
  }

  void MSService::postEndJob()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "EJ";
    microstate2_ = "done";
  }

  void MSService::preEventProcessing(const edm::EventID& iID,
					     const edm::Timestamp& iTime)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate1_ = "PRO";
  }

  void MSService::postEventProcessing(const edm::Event& e, const edm::EventSetup&)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "INPUT";
  }
  void MSService::preSource()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "INPUT";
  }

  void MSService::postSource()
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "FWKOVH";
  }

  void MSService::preModule(const edm::ModuleDescription& desc)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = desc.moduleLabel();
  }

  void MSService::postModule(const edm::ModuleDescription& desc)
  {
    boost::mutex::scoped_lock sl(lock_);
    microstate2_ = "FWKOVH";
  }
  
  std::string MSService::getMicroState1()
  { 
	boost::mutex::scoped_lock sl(lock_);
	return microstate1_;
  }

  std::string MSService::getMicroState2()
  { 
	boost::mutex::scoped_lock sl(lock_);
	return microstate2_;
  }

  void MSService::setMicroState(std::string &in)
  {
    	boost::mutex::scoped_lock sl(lock_);
	microstate2_ = in;
  }

} //end namespace dqmevf

