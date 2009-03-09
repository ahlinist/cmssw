/**
 *  See header file for a description of this class.
 *
 */


#include "RecoMuon/SeedGenerator/src/RPCSeedFinder.h"
#include <iomanip>

using namespace std;
using namespace edm;


RPCSeedFinder::RPCSeedFinder() {

    // Initiate the member
    isrecHitsset = false;
    isConfigured = false;
    isOutputset = false;
    isEventSetupset = false;
    oneSeed.clear();
}

RPCSeedFinder::~RPCSeedFinder() {

}

void RPCSeedFinder::configure(const edm::ParameterSet& iConfig) {

    oneSeed.configure(iConfig);
    isConfigured = true;
}

void RPCSeedFinder::setOutput(std::vector<TrajectorySeed> *goodRef, std::vector<TrajectorySeed> *candidateRef) {

    goodSeedsRef = goodRef;
    candidateSeedsRef = candidateRef;
    isOutputset = true;
}

void RPCSeedFinder::setrecHits(ConstMuonRecHitContainer &recHits) {

    oneSeed.clear();
    for (ConstMuonRecHitContainer::const_iterator iter = recHits.begin(); iter!= recHits.end(); iter++)
        oneSeed.add(*iter);
    isrecHitsset = true;
}

void RPCSeedFinder::setEventSetup(const edm::EventSetup& iSetup) {

    eSetup = &iSetup;
    isEventSetupset = true;
}

void RPCSeedFinder::seed() {

    cout << "[RPCSeedFinder] --> seeds called" << endl;

    if(isrecHitsset == false || isOutputset == false || isConfigured == false || isEventSetupset == false)
    {
        cout << "Configuration or IO is not set yet" << endl;
        return;
    }
    
    TrajectorySeed theSeed;
    int isGoodSeed = 0;
    const edm::EventSetup &iSetup = *eSetup;
    theSeed = oneSeed.seed(iSetup, isGoodSeed);
    // Push back the good seed
    if(isGoodSeed == 1)
    {
        cout << "[RPCSeedFinder] --> Seeds from " << oneSeed.nrhit() << " recHits." << endl;
        goodSeedsRef->push_back(theSeed);
    }
    // Push back cadidate seed but not the fake seed
    if(isGoodSeed >= 0)
    {
        candidateSeedsRef->push_back(theSeed);
    }

    // Unset the signal
    oneSeed.clear();
    isrecHitsset = false;
}



