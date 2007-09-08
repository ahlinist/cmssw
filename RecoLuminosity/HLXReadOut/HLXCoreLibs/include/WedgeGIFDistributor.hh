/*
  Wedge GIF distributor class
  Copyright John Jones 2007

  Output lumi section data to gif file
  for individual HF wedges
*/

#ifndef WEDGEGIFDISTRIBUTOR_HH
#define WEDGEGIFDISTRIBUTOR_HH

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
  class WedgeGIFDistributor : public AbstractDistributor {

  public:

    // Constructor
    WedgeGIFDistributor(u32 wedge);

    // Destructor
    ~WedgeGIFDistributor();

    // Processing function
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:

    // Generate the histograms
    void GenerateBelowTopOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateBetweenTopOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateAboveTopOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateBelowBottomOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateBetweenBottomOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateAboveBottomOccupancyHistogram(const LUMI_SECTION & lumiSection);
    void GenerateETSumHistogram(const LUMI_SECTION & lumiSection);
    void GenerateLHCHistogram(const LUMI_SECTION & lumiSection);

    // Init and cleanup
    void Init();
    void CleanUp();

    // Wedge number
    u32 mWedge;

    // Buffers for insertion into DIP data structure
    double *mHistogramData;

    // ROOT mutex
    ROOTMutex mROOTMutex;

  };

} //~namespace HCAL_HLX

#endif //WEDGEGIFDISTRIBUTOR_HH
