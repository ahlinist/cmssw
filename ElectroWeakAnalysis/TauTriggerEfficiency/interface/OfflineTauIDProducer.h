#ifndef OfflineTauIDProducer_H
#define OfflineTauIDProducer_H

/** \class OfflineTauIDProducer
 *
 *
 *  This class is an EDProducer for selecting events
 *  passing tau identification
 *
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/TauReco/interface/CaloTau.h"
#include "RecoTauTag/TauTagTools/interface/CaloTauElementsOperators.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"

//#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

#include <string>

using namespace edm;
using namespace reco;
using namespace math;
using namespace std;

class OfflineTauIDProducer : public edm::EDProducer {

    public:
        explicit OfflineTauIDProducer(const edm::ParameterSet&);
        ~OfflineTauIDProducer();

        virtual void produce(edm::Event&, const edm::EventSetup&);
	virtual bool tauTag(reco::CaloTau&);
        virtual bool tauTag(reco::PFTau&);

   private:
	InputTag tauProducer;
        double jetPtMin,
               matchingConeSize,
               isolationConeSize,
               ptLeadingTrackMin,
               ptOtherTracksMin;
        string signalConeSizeFormula;
        double signalConeSizeMin;
        double signalConeSizeMax;
        string metric;
        unsigned int isolationAnnulus_Tracksmaxn;

        int nEvents,
            nSelectedEvents;
};
#endif
