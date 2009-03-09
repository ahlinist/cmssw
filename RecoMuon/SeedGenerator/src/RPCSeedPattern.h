#ifndef RecoMuon_SeedGenerator_RPCSeedPattern_H
#define RecoMuon_SeedGenerator_RPCSeedPattern_H

/**  \class RPCSeedPattern
 *
 *  \author Haiyun.Teng - Peking University
 *
 *
 */


#include "FWCore/Framework/interface/EventSetup.h"
#include <DataFormats/TrajectorySeed/interface/TrajectorySeed.h>
#include <RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h>
#include <MagneticField/Engine/interface/MagneticField.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <vector>

#ifndef upper_limit_pt
#define upper_limit_pt 1000
#endif

#ifndef lower_limit_pt
#define lower_limit_pt 3.0
#endif


class RPCSeedPattern {

    typedef MuonTransientTrackingRecHit::MuonRecHitPointer MuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitPointer ConstMuonRecHitPointer;
    typedef MuonTransientTrackingRecHit::MuonRecHitContainer MuonRecHitContainer;
    typedef MuonTransientTrackingRecHit::ConstMuonRecHitContainer ConstMuonRecHitContainer;
    typedef std::pair<ConstMuonRecHitPointer, ConstMuonRecHitPointer> RPCSegment;

    public:
    RPCSeedPattern(); 
    ~RPCSeedPattern();
    void configure(const edm::ParameterSet& iConfig);
    void clear() { theRecHits.clear(); }
    void add(const ConstMuonRecHitPointer& hit) { theRecHits.push_back(hit); }   
    unsigned int nrhit() const { return theRecHits.size(); }

    private:
    friend class RPCSeedFinder;
    TrajectorySeed seed(const edm::EventSetup& eSetup, int& isGoodSeed); 
    void clearPattern();
    void ThreePointsAlgorithm();
    void MiddlePointsAlgorithm();
    void SegmentAlgorithm();
    void SegmentAlgorithmSpecial(const edm::EventSetup& eSetup);
    bool checkSegment() const;
    ConstMuonRecHitPointer FirstRecHit() const; 
    ConstMuonRecHitPointer BestRefRecHit() const;
    TrajectorySeed createFakeSeed(int& isGoodSeed, const edm::EventSetup& eSetup);
    TrajectorySeed createSeed(int& isGoodSeed, const edm::EventSetup& eSetup);
    double getDistance(const ConstMuonRecHitPointer& precHit, const GlobalVector& Center) const;
    bool checkStraightwithThreerecHits(ConstMuonRecHitPointer (&precHit)[3], double MinDeltaPhi) const;
    GlobalVector computePtwithThreerecHits(double& pt, double& pt_err, ConstMuonRecHitPointer (&precHit)[3]) const;
    bool checkStraightwithSegment(const RPCSegment& Segment1, const RPCSegment& Segment2, double MinDeltaPhi) const;
    GlobalVector computePtwithSegment(const RPCSegment& Segment1, const RPCSegment& Segment2) const;
    bool checkStraightwithThreerecHits(double (&x)[3], double (&y)[3], double MinDeltaPhi) const;
    GlobalVector computePtWithThreerecHits(double& pt, double& pt_err, double (&x)[3], double (&y)[3]) const;

    void checkSimplePattern(const edm::EventSetup& eSetup);
    void checkSegmentAlgorithmSpecial(const edm::EventSetup& eSetup);
    double extropolateStep(const GlobalPoint& startPosition, const GlobalVector& startMomentum, ConstMuonRecHitContainer::const_iterator iter, const int ClockwiseDirection, double& tracklength, const edm::EventSetup& eSetup);
    void computeBestPt(double* pt, double* spt, double& ptmean0, double& sptmean0, unsigned int NumberofPt) const;

    // ----------member data ---------------------------

    // parameters for configuration
    double MaxRSD;
    double deltaRThreshold;
    unsigned int AlgorithmType;
    bool autoAlgorithmChoose;
    double ZError;
    double MinDeltaPhi;
    double stepLength;
    unsigned int sampleCount;
    // Signals for run seed()
    bool isConfigured;
    // recHits of a pattern
    ConstMuonRecHitContainer theRecHits;
    // Complex pattern
    std::vector<bool> isStraight2;
    std::vector<GlobalVector> Center2;
    std::vector<double> meanRadius2;
    std::vector<GlobalVector> meanMagneticField2;
    GlobalPoint entryPosition;
    GlobalPoint leavePosition;
    double lastPhi;
    double S;
    // Simple pattern
    bool isStraight;
    GlobalVector Center;
    double meanRadius;
    double meanBz;
    double deltaBz;
    // Pattern estimation part
    bool isPatternChecked;
    int isGoodPattern;
    int isClockwise;
    int isParralZ;
    int Charge;
    double meanPt;
    double meanSpt;
};

#endif
