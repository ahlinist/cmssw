#ifndef OPERATIONS_H_
#define OPERATIONS_H_
#include <iostream>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


template<typename T> std::string obj2str(T n) {
	std::ostringstream oss;
	oss << n;
	std::string s(oss.str());
	return s;
}

template<typename T> T* vecToArr(std::vector<T> input) {
	T* a = new T[input.size()];
	std::copy(input.begin(), input.end(), a);
	return a;
}

//This does work.
template<typename u> std::vector<u> arrToVec(u a[], int size) {
	std::vector<u> ans(a, a + size);
	return ans;
}

namespace pftools {
double deltaR(const double& eta1, const double& eta2, const double& phi1,
		const double& phi2);

template<class T> void getCollection(edm::Handle<T>& c,
		const edm::InputTag& tag, const edm::Event& event) {

	try {
		event.getByLabel(tag, c);
		if (!c.isValid()) {
			edm::LogProblem("pftools::operations::getCollection")
					<< "Warning! Collection for label " << tag
					<< " is not valid!" << std::endl;
		}
	} catch (cms::Exception& err) {
		edm::LogError("pftools::operations::getCollection")
				<< "Couldn't get collection\n";
		throw err;

	}
}

template<typename T> std::vector<unsigned> findObjectsInDeltaR(
		const std::vector<T>& objects, const double centralEta,
		const double centralPhi, const double deltaRCut) {
//	using namespace edm;
//	using namespace reco;

	unsigned index(0);
	std::vector<unsigned> answers;
	for (typename std::vector<T>::const_iterator oit = objects.begin(); oit
			!= objects.end(); ++oit) {
		T obj = *oit;
		double rhEta = obj.positionREP().eta();
		double rhPhi = obj.positionREP().phi();

		if (pftools::deltaR(rhEta, centralEta, rhPhi, centralPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}
		++index;
	}
	return answers;
}



template<typename T> std::vector<unsigned> findCandidatesInDeltaR(
		const std::vector<T>& objects, const double centralEta,
		const double centralPhi, const double deltaRCut) {
//	using namespace edm;
//	using namespace reco;

	unsigned index(0);
	std::vector<unsigned> answers;
	for (typename std::vector<T>::const_iterator oit = objects.begin(); oit
			!= objects.end(); ++oit) {
		T obj = *oit;
		double rhEta = obj.eta();
		double rhPhi = obj.phi();

		if (pftools::deltaR(rhEta, centralEta, rhPhi, centralPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}
		++index;
	}
	return answers;

}

template<typename T> std::vector<unsigned> findCandidatesInDeltaRECAL(
		const std::vector<T>& objects, const double centralEta,
		const double centralPhi, const double deltaRCut) {
//	using namespace edm;
//	using namespace reco;

	unsigned index(0);
	std::vector<unsigned> answers;
	for (typename std::vector<T>::const_iterator oit = objects.begin(); oit
			!= objects.end(); ++oit) {
		T obj = *oit;
		double rhEta = obj.positionAtECALEntrance().eta();
		double rhPhi = obj.positionAtECALEntrance().phi();

		if (pftools::deltaR(rhEta, centralEta, rhPhi, centralPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}
		++index;
	}
	return answers;

}

}

#endif

