/*
  GIF distributor class
  Uses root to make histograms
  Copyright John Jones 2007
*/

// Prototype class definition
#include "GIFDistributor.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"

// Root includes
#include "TCanvas.h"
#include "TH1D.h"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  GIFDistributor::GIFDistributor() {
    try {
      // Initialise the data structures
      this->Init();
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  GIFDistributor::~GIFDistributor() {
    // Clean up the data structures
    this->CleanUp();
  }

  void GIFDistributor::Init() {
    mHistogramData = new u32[4096];
    if ( !mHistogramData ) {
      this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory");
      RAISE(lExc);
    }
  }

  void GIFDistributor::CleanUp() { 
    if ( mHistogramData ) {
      delete []mHistogramData;
      mHistogramData = 0;
    }
  }

  void GIFDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 

    // Done before histogram fill - worried about
    // root using dynamic allocation of memory...slower
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      mHistogramData[j] = 0; 
      for ( u32 i = 0 ; i != lumiSection.hdr.numHLXs ; i++ ) {
	mHistogramData[j] += lumiSection.etSum[i].data[j];
      }
    }

    TH1D histogram("tempHist","HLX ET Distribution",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram.Fill(j,mHistogramData[j]);
    }

    TCanvas canvas("tempCanvas","tempCanvas",200,10,600,400);
    canvas.cd();
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogram.SetLineWidth(2);
    histogram.GetXaxis()->SetTitle("BX Number");
    histogram.GetYaxis()->SetTitle("ET Count");
    histogram.Draw();
    canvas.SaveAs("tmp/et.gif");

  }

}












