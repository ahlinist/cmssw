#ifndef DQMTau_h
#define DQMTau_h

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <iostream>

class DQMTau {

public:
	 DQMTau(const reco::Candidate*, std::map<std::string,MonitorElement *>::const_iterator, std::string, double, std::map<std::string,MonitorElement *>);
	~DQMTau();

private:

};

DQMTau::DQMTau(const reco::Candidate* gen_particle_, std::map<std::string,MonitorElement *>::const_iterator element_, std::string currentDiscriminatorLabel_, double tauPtRes_, std::map<std::string,MonitorElement *> plotMapPrivate_)
{
//	std::cout << __PRETTY_FUNCTION__ << "]\t" << std::endl << __LINE__ << "]\t" << "dump" << std::endl ;
}

DQMTau::~DQMTau()
{
  
}

#endif