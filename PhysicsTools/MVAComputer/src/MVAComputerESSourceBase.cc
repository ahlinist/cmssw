#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Entry.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "PhysicsTools/MVAComputer/interface/Calibration.h"
#include "PhysicsTools/MVAComputer/interface/MVAComputer.h"
#include "PhysicsTools/MVAComputer/interface/MVAComputerESSourceBase.h"

namespace PhysicsTools {

MVAComputerESSourceBase::MVAComputerESSourceBase(
					const edm::ParameterSet &params)
{
	std::vector<std::string> names = params.getParameterNames();
	for(std::vector<std::string>::const_iterator iter = names.begin();
	    iter != names.end(); iter++) {
		const edm::Entry *entry = params.retrieveUntracked(*iter);
		if (!entry)
			throw edm::Exception(edm::errors::Configuration,
			                     "MissingParameter")
				<< "The required parameter '" << *iter
				<< "' is missing." << std::endl;

		std::string path;
		if (entry->typeCode() == 'F')
			path = entry->getFileInPath().fullPath();
		else 
			path = entry->getString();

		mvaCalibrations[*iter] = path;
	}
}

MVAComputerESSourceBase::~MVAComputerESSourceBase()
{
}

MVAComputerESSourceBase::ReturnType
MVAComputerESSourceBase::produce() const
{
	ReturnType container(new Calibration::MVAComputerContainer);

	for(LabelFileMap::const_iterator iter = mvaCalibrations.begin();
	    iter != mvaCalibrations.end(); iter++) {
		std::auto_ptr<Calibration::MVAComputer> calibration(
			MVAComputer::readCalibration(iter->second.c_str()));

		container->add(iter->first) = *calibration;
	}

	return container;
}

} // namespace PhysicsTools
