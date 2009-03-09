/**
 *  See header file for a description of this class.
 *
 */


#include "RecoMuon/SeedGenerator/src/RPCSeedOverlapper.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CommonDetUnit/interface/GeomDetUnit.h>

using namespace std;
using namespace edm;


RPCSeedOverlapper::RPCSeedOverlapper() {

    isConfigured = false; 
    isIOset = false;
    isEventSetupset = false;
}

RPCSeedOverlapper::~RPCSeedOverlapper() {

}

void RPCSeedOverlapper::configure(const edm::ParameterSet& iConfig) {

    isCheckgoodOverlap = iConfig.getParameter<bool>("isCheckgoodOverlap");
    isCheckcandidateOverlap = iConfig.getParameter<bool>("isCheckcandidateOverlap");
    ShareRecHitsNumberThreshold = iConfig.getParameter<unsigned int>("ShareRecHitsNumberThreshold");
    isConfigured = true;
}

void RPCSeedOverlapper::setIO(std::vector<TrajectorySeed> *goodRef, std::vector<TrajectorySeed> *candidateRef) {

    goodSeedsRef = goodRef;
    candidateSeedsRef = candidateRef;
    isIOset = true;
}

void RPCSeedOverlapper::unsetIO() {

    isIOset = false;
}

void RPCSeedOverlapper::setEventSetup(const edm::EventSetup& iSetup) {

    eSetup = &iSetup;
    isEventSetupset = true;
}

void RPCSeedOverlapper::run() {

    if(isConfigured == false || isIOset == false || isEventSetupset == false)
    {
        cout << "Configuration or IO is not set yet" << endl;
        return;
    }
    if(isCheckgoodOverlap == true)
    {
        CheckOverlap(*eSetup, goodSeedsRef);
    }
    if(isCheckcandidateOverlap == true)
    {
        CheckOverlap(*eSetup, candidateSeedsRef);
    }

}

void RPCSeedOverlapper::CheckOverlap(const edm::EventSetup& iSetup, std::vector<TrajectorySeed> *SeedsRef) {

    std::vector<TrajectorySeed> sortSeeds;
    std::vector<TrajectorySeed> tempSeeds;
    edm::OwnVector<TrackingRecHit> tempRecHits;

    edm::ESHandle<RPCGeometry> rpcGeometry;
    iSetup.get<MuonGeometryRecord>().get(rpcGeometry);

    while(SeedsRef->size() != 0)
    {
        cout << "Finding the good seeds group from " << SeedsRef->size() << " seeds which share some recHits" << endl; 
        // Take 1st seed in SeedsRef as referrence and find a collection which always share some recHits with some other
        tempRecHits.clear();
        tempSeeds.clear();
        int N = 0;
        for(vector<TrajectorySeed>::iterator itseed = SeedsRef->begin(); itseed != SeedsRef->end(); N++)
        {
            TrajectorySeed::range RecHitsRange = itseed->recHits();
            if(N == 0)
            {
                cout << "Always take the 1st good seed to be the referrence." << endl;
                for(TrajectorySeed::const_iterator it = RecHitsRange.first; it != RecHitsRange.second; it++)
                {
                    cout << "Put its recHits to tempRecHits" << endl;
                    tempRecHits.push_back(it->clone());
                }
                cout << "Put it to tempSeeds" << endl;
                tempSeeds.push_back(*itseed);
                cout << "Then erase from SeedsRef->" << endl;
                itseed = SeedsRef->erase(itseed);
            }
            else
            {
                cout << "Come to other good seed for checking " << itseed->nHits() << " recHits from " << tempRecHits.size() << " temp recHits" << endl;
                unsigned int ShareRecHitsNumber = 0;
                for(TrajectorySeed::const_iterator it = RecHitsRange.first; it != RecHitsRange.second; it++)
                {
                    if(isShareHit(tempRecHits, *it, rpcGeometry))
                        ShareRecHitsNumber++;
                }
                if(ShareRecHitsNumber >= ShareRecHitsNumberThreshold)
                {
                    cout <<"This seed is found to belong to current share group" << endl;
                    for(TrajectorySeed::const_iterator it = RecHitsRange.first; it != RecHitsRange.second; it++)
                    {
                        if(!isShareHit(tempRecHits, *it, rpcGeometry))
                        {
                            cout << "Put its extra recHits to tempRecHits" << endl;
                            tempRecHits.push_back(it->clone());
                        }
                    }
                    cout << "Put it to tempSeeds" << endl;
                    tempSeeds.push_back(*itseed);
                    cout << "Then erase from SeedsRef" << endl;
                    itseed = SeedsRef->erase(itseed);
                }
                else
                    itseed++;
            }
        }
        // Find the best seed and kick out those share recHits with it
        // The best seed save in GoodSeeds, those don't share recHits with it will be push back to SeedsRef for next while loop
        TrajectorySeed bestSeed;
        vector<TrajectorySeed>::iterator bestiter;
        // Find the min Spt wrt Pt as the best Seed
        double minSpt = 1000000;
        unsigned NumberofHits = 0;
        cout << "Find " << tempSeeds.size() << " seeds into one trajectory group" << endl;
        for(vector<TrajectorySeed>::iterator itseed = tempSeeds.begin(); itseed != tempSeeds.end(); itseed++)
        {
            unsigned int nHits = itseed->nHits();
            std::vector<float> seed_error = itseed->startingState().errorMatrix();
            double Spt = seed_error[0];
            cout << "Find a good seed with Spt " << Spt << endl;
            if((NumberofHits < nHits) || (NumberofHits == nHits && Spt < minSpt))
            {
                NumberofHits = nHits;
                minSpt = Spt;
                bestSeed = *itseed;
                bestiter = itseed;
            }
        }
        cout << "Best good temp seed's Spt is " << minSpt <<endl;
        sortSeeds.push_back(bestSeed);
        tempSeeds.erase(bestiter);
        tempRecHits.clear();

        for(TrajectorySeed::const_iterator it = bestSeed.recHits().first; it != bestSeed.recHits().second; it++)
        {
            tempRecHits.push_back(it->clone());
        }
        for(vector<TrajectorySeed>::iterator itseed = tempSeeds.begin(); itseed != tempSeeds.end(); )
        {
            cout << "Checking the temp seed's " << itseed->nHits() << " hits to " << tempRecHits.size() << " temp recHits" << endl;
            TrajectorySeed::range RecHitsRange = itseed->recHits();
            bool isShare = false;
            for(TrajectorySeed::const_iterator it = RecHitsRange.first; it != RecHitsRange.second; it++)
            {
                if(isShareHit(tempRecHits, *it, rpcGeometry))
                    isShare = true;
            }
            if(isShare == true)
            {
                cout << "Find one temp seed share some recHits with best seed" << endl;
                itseed = tempSeeds.erase(itseed);
            }
            else
            {
                cout << "This seed has no relation with best seed" << endl;
                SeedsRef->push_back(*itseed);
                itseed = tempSeeds.erase(itseed);
            }
        }
    }
    // At the end exchange SeedsRef with sortSeeds
    *SeedsRef = sortSeeds;
}

bool RPCSeedOverlapper::isShareHit(const edm::OwnVector<TrackingRecHit> &RecHits, const TrackingRecHit& hit, edm::ESHandle<RPCGeometry> rpcGeometry) {

    bool istheSame = false;
    unsigned int n = 1;
    cout << "Checking from " << RecHits.size() << " temp recHits" << endl;

    LocalPoint lpos1 = hit.localPosition();
    DetId RPCId1 = hit.geographicalId();
    const GeomDetUnit *rpcroll1 = rpcGeometry->idToDetUnit(RPCId1);
    GlobalPoint gpos1 = rpcroll1->toGlobal(lpos1);
    cout << "The hit's position: " << gpos1.x() << ", " << gpos1.y() << ", " << gpos1.z() << endl;
    for(edm::OwnVector<TrackingRecHit>::const_iterator it = RecHits.begin(); it !=RecHits.end(); it++, n++)
    {
        cout << "Checking the " << n << " th recHit from tempRecHits" << endl;
        LocalPoint lpos2 = it->localPosition();
        DetId RPCId2 = it->geographicalId();
        const GeomDetUnit *rpcroll2 = rpcGeometry->idToDetUnit(RPCId2);
        GlobalPoint gpos2 = rpcroll2->toGlobal(lpos2);
        cout << "The temp hit's position: " << gpos2.x() << ", " << gpos2.y() << ", " << gpos2.z() << endl;

        if((gpos1.x() == gpos2.x()) && (gpos1.y() == gpos2.y()) && (gpos1.z() == gpos2.z()))
        {
            cout << "This hit is found to be the same" << endl;
            istheSame = true;
        }
    }
    return istheSame;
}
