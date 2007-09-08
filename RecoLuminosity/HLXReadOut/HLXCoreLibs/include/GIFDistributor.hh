/*
  GIF distributor class
  Copyright John Jones 2007

  Output lumi section data to gif file
*/

#ifndef GIFDISTRIBUTOR_HH
#define GIFDISTRIBUTOR_HH

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// ROOT mutex
#include "ROOTMutex.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class GIFDistributor : public AbstractDistributor {

  public:

    // Constructor
    GIFDistributor();

    // Destructor
    ~GIFDistributor();

    // Processing function
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:

    // ROOT mutex object
    ROOTMutex mROOTMutex;

    // Processing functions
    void DoETSumHistogram(u32 numBunches);
    void DoLHCHistogram(u32 numBunches);
    void DoBelowOccTopHistogram(u32 numBunches);
    void DoBetweenOccTopHistogram(u32 numBunches);
    void DoAboveOccTopHistogram(u32 numBunches);
    void DoBelowOccBottomHistogram(u32 numBunches);
    void DoBetweenOccBottomHistogram(u32 numBunches);
    void DoAboveOccBottomHistogram(u32 numBunches);

    // Init and cleanup
    void Init();
    void CleanUp();

    // Buffers for insertion into DIP data structure
    double *mETHistogramData;
    double *mLHCHistogramData;
    double *mBelowBottomHistogramData;
    double *mBetweenBottomHistogramData;
    double *mAboveBottomHistogramData;
    double *mBelowTopHistogramData;
    double *mBetweenTopHistogramData;
    double *mAboveTopHistogramData;

  };

} //~namespace HCAL_HLX

#endif //GIFDISTRIBUTOR_HH
