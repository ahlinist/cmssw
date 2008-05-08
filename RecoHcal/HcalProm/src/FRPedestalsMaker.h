#ifndef FRPedestalMaker_H
#define FRPedestalMaker_H


#include "FWCore/Framework/interface/EDAnalyzer.h"

class TH1F;
class TH2F;
class HcalDbService;
class TFile;

class FRPedestalsMaker: public edm::EDAnalyzer{

public:

/// Constructor
FRPedestalsMaker(const edm::ParameterSet& ps);

/// Destructor
~FRPedestalsMaker();

protected:
//void SampleAnalyzer_in(const edm::ParameterSet& ps);
//void SampleAnalyzer_out();

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

// BeginJob
void beginJob(const edm::EventSetup& c);

// EndJob
void endJob(void);

private:
 std::map <unsigned, TH1F*> mHists [4];
 TH1F* mTimeSlicesHist;
 TH2F* mPedDifs [4];
 std::string mPedestalsFile;
 int mPreSliceFirst;
 int mPreSliceLast;
 int mPostSliceFirst;
 int mPostSliceLast;
 const HcalDbService* mConditions;
 TFile* mRootFile;
};

#endif
