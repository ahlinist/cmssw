/*
 * Initialisable.h
 *
 *  Created on: 22-Apr-2009
 *      Author: jamie
 */

#ifndef INITIALISABLE_H_
#define INITIALISABLE_H_
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace pftools {

/**
 * @class Initialisable
 * @brief For any class that should be initialised with some edm::ParameterSet
 * before usage
 *
 * I found I needed this for some multiple inheritance case...
 *
 * Oh how I miss Java interfaces.
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class Initialisable {
public:
	Initialisable();

	virtual ~Initialisable();

	virtual void init(const edm::ParameterSet& parameters) = 0;
};

}

#endif /* INITIALISABLE_H_ */
