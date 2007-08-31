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
    mETHistogramData = new double[4096];
    if ( !mETHistogramData ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for ET histogram");
      RAISE(lExc);
    }
    mLHCHistogramData = new double[4096];
    if ( !mLHCHistogramData ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for LHC histogram");
      RAISE(lExc);
    }
  }

  void GIFDistributor::CleanUp() { 
    if ( mETHistogramData ) {
      delete []mETHistogramData;
      mETHistogramData = 0;
    }
    if ( mLHCHistogramData ) {
      delete []mLHCHistogramData;
      mLHCHistogramData = 0;
    }
  }

  void GIFDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 
    //cout << "Begin " << __PRETTY_FUNCTION__ << endl;

    // Done before histogram fill - worried about
    // root using dynamic allocation of memory...slower
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      mETHistogramData[j] = 0.0;
      mLHCHistogramData[j] = 0.0;
      for ( u32 i = 0 ; i != lumiSection.hdr.numHLXs ; i++ ) {
	mETHistogramData[j] += static_cast<double>(lumiSection.etSum[i].data[j]);
	mLHCHistogramData[j] += static_cast<double>(lumiSection.lhc[i].data[j]);
      }
     //cout << j << "\t" << mETHistogramData[j] << endl;
    }

    TH1D histogramET("tempHist","HLX ET Distribution",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogramET.Fill(j,mETHistogramData[j]);
    }

    TCanvas canvas("tempCanvas","tempCanvas",200,10,600,400);
    canvas.cd();
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("ET Count");
    histogramET.Draw();
    canvas.SaveAs("tmp/et.gif");

    //TH1D histogramLHC("tempHist","HLX LHC Distribution",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    //for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
    //  histogramLHC.Fill(j,mLHCHistogramData[j]);
   // }

    //TCanvas canvas2("tempCanvas","tempCanvas",200,10,600,400);
    //canvas.cd();
    //canvas.SetFillColor(10);
    //canvas.SetLogy();
    //canvas.SetGridx();
    //canvas.SetGridy();
    //histogramLHC.SetLineWidth(2);
    //histogramLHC.GetXaxis()->SetTitle("BX Number");
    //histogramLHC.GetYaxis()->SetTitle("ET Count");
    //histogramLHC.Draw();
    //canvas2.SaveAs("tmp/lhc.gif");

    //cout << "End " << __PRETTY_FUNCTION__ << endl;
  }

}












