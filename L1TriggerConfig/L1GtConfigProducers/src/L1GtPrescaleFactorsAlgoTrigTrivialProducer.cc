/**
 * \class L1GtPrescaleFactorsAlgoTrigTrivialProducer
 * 
 * 
 * Description: ESProducer for L1 GT prescale factors for algorithm triggers.  
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 * $Date$
 * $Revision$
 *
 */

// this class header
#include "L1TriggerConfig/L1GtConfigProducers/interface/L1GtPrescaleFactorsAlgoTrigTrivialProducer.h"

// system include files
#include <memory>
#include "boost/shared_ptr.hpp"

#include <vector>

// user include files
//   base class
#include "FWCore/Framework/interface/ESProducer.h"

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1GtPrescaleFactorsAlgoTrigRcd.h"

// forward declarations

// constructor(s)
L1GtPrescaleFactorsAlgoTrigTrivialProducer::L1GtPrescaleFactorsAlgoTrigTrivialProducer(
        const edm::ParameterSet& parSet)
{
    // tell the framework what data is being produced
    setWhatProduced(this,
            &L1GtPrescaleFactorsAlgoTrigTrivialProducer::producePrescaleFactors);

    // now do what ever other initialization is needed

    // prescale factors
    m_prescaleFactors = 
            parSet.getParameter<std::vector<int> >("PrescaleFactors");

}

// destructor
L1GtPrescaleFactorsAlgoTrigTrivialProducer::~L1GtPrescaleFactorsAlgoTrigTrivialProducer()
{

    // empty

}

// member functions

// method called to produce the data
boost::shared_ptr<L1GtPrescaleFactors> 
    L1GtPrescaleFactorsAlgoTrigTrivialProducer::producePrescaleFactors(
        const L1GtPrescaleFactorsAlgoTrigRcd& iRecord)
{

    boost::shared_ptr<L1GtPrescaleFactors> pL1GtPrescaleFactors =
            boost::shared_ptr<L1GtPrescaleFactors>(
                    new L1GtPrescaleFactors(m_prescaleFactors) );

            return pL1GtPrescaleFactors;
}
