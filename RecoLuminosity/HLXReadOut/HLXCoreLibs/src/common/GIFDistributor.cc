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
#include "TDatime.h"
#include "TStyle.h"
#include "TROOT.h"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;

  // Possibly add as configuration setting in future
#define HCAL_HLX_CANVAS_OUTPUT_DIR "/opt/gifoutput/"
//"/cms/mon/data/dqm/lumi/pic/"
  
  // Default constructor
  GIFDistributor::GIFDistributor() : mCanvasOutputDir(HCAL_HLX_CANVAS_OUTPUT_DIR) {
    mETHistogramData = 0;
    mLHCHistogramData = 0;
    mBelowTopHistogramData = 0;
    mBetweenTopHistogramData = 0;
    mAboveTopHistogramData = 0;
    mBelowBottomHistogramData = 0;
    mBetweenBottomHistogramData = 0;
    mAboveBottomHistogramData = 0;
    try {
      // Initialise the data structures
      this->Init();
      // Get rid of rms / mean display in pictures
      gStyle->SetOptStat(0);
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
    if ( mETHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for ET histogram");
      RAISE(lExc);
    }
    mLHCHistogramData = new double[4096];
    if ( mLHCHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for LHC histogram");
      RAISE(lExc);
    }
    mBelowTopHistogramData = new double[4096];
    if ( mBelowTopHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for below-top histogram");
      RAISE(lExc);
    }
    mBetweenTopHistogramData = new double[4096];
    if ( mBetweenTopHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for between-top histogram");
      RAISE(lExc);
    }
    mAboveTopHistogramData = new double[4096];
    if ( mAboveTopHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for above-top histogram");
      RAISE(lExc);
    }
    mBelowBottomHistogramData = new double[4096];
    if ( mBelowBottomHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for below-bottom histogram");
      RAISE(lExc);
    }
    mBetweenBottomHistogramData = new double[4096];
    if ( mBetweenBottomHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for between-bottom histogram");
      RAISE(lExc);
    }
    mAboveBottomHistogramData = new double[4096];
    if ( mAboveBottomHistogramData == 0 ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for above-bottom histogram");
      RAISE(lExc);
    }
  }

  void GIFDistributor::CleanUp() { 
    if ( mETHistogramData != 0 ) {
      delete []mETHistogramData;
      mETHistogramData = 0;
    }
    if ( mLHCHistogramData != 0 ) {
      delete []mLHCHistogramData;
      mLHCHistogramData = 0;
    }
    if ( mBelowTopHistogramData != 0 ) {
      delete []mBelowTopHistogramData;
      mBelowTopHistogramData = 0;
    }
    if ( mBetweenTopHistogramData != 0 ) {
      delete []mBetweenTopHistogramData;
      mBetweenTopHistogramData = 0;
    }
    if ( mAboveTopHistogramData != 0 ) {
      delete []mAboveTopHistogramData;
      mAboveTopHistogramData = 0;
    }
    if ( mBelowBottomHistogramData != 0 ) {
      delete []mBelowBottomHistogramData;
      mBelowBottomHistogramData = 0;
    }
    if ( mBetweenBottomHistogramData != 0 ) {
      delete []mBetweenBottomHistogramData;
      mBetweenBottomHistogramData = 0;
    }
    if ( mAboveBottomHistogramData != 0 ) {
      delete []mAboveBottomHistogramData;
      mAboveBottomHistogramData = 0;
    }
  }

  void GIFDistributor::DoETSumHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "ET on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mETHistogramData[j]);
    }

    TCanvas canvas("gifCanvas","gifCanvas",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("ET Sum");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "et.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoLHCHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "LHC on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist2",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mLHCHistogramData[j]);
    }

    TCanvas canvas("gifCanvas2","gifCanvas2",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "lhc.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoBelowOccTopHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Below Threshold Inner Ring on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist3",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mBelowTopHistogramData[j]);
    }

    TCanvas canvas("gifCanvas3","gifCanvas3",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "occ_below_threshold_inner.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoBetweenOccTopHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Between Thresholds Inner Ring on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist4",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mBetweenTopHistogramData[j]);
    }

    TCanvas canvas("gifCanvas4","gifCanvas4",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "occ_between_thresholds_inner.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoAboveOccTopHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Above Threshold Inner Ring on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist5",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mAboveTopHistogramData[j]);
    }

    TCanvas canvas("gifCanvas5","gifCanvas5",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "occ_above_threshold_inner.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoBelowOccBottomHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Below Threshold Outer Ring on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist6",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mBelowBottomHistogramData[j]);
    }

    TCanvas canvas("gifCanvas6","gifCanvas6",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "occ_below_threshold_outer.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoBetweenOccBottomHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Between Thresholds Outer Ring on ";
    tStr += curTime.AsString();

    TH1D histogramET("gifHist7",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogramET.Fill(j,mBetweenBottomHistogramData[j]);
    }

    TCanvas canvas("gifCanvas7","gifCanvas7",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    histogramET.SetLineWidth(2);
    histogramET.GetXaxis()->SetTitle("BX Number");
    histogramET.GetYaxis()->SetTitle("Count");
    histogramET.Draw();

    std::string outputName = mCanvasOutputDir + "occ_between_thresholds_outer.png";
    canvas.SaveAs(outputName.c_str());
  }

  void GIFDistributor::DoAboveOccBottomHistogram(u32 numBunches) {

    TDatime curTime;
    std::string tStr = "Above Threshold Outer Ring on ";
    tStr += curTime.AsString();

    TH1D histogram("gifHist8",tStr.c_str(),numBunches,0,numBunches);
    for ( u32 j = 0 ; j != numBunches ; j++ ) {
      histogram.Fill(j,mAboveBottomHistogramData[j]);
    }

    TCanvas canvas("gifCanvas8","gifCanvas8",200,10,800,600);
    canvas.SetFillColor(10);
    canvas.SetLogy();
    canvas.SetGridx();
    canvas.SetGridy();
    
    histogram.SetLineWidth(2);
    histogram.GetXaxis()->SetTitle("BX Number");
    histogram.GetYaxis()->SetTitle("Count");
    histogram.Draw();

    std::string outputName = mCanvasOutputDir + "occ_above_threshold_outer.png";
    canvas.SaveAs(outputName.c_str());
  }

  bool GIFDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 
    // Safety check
    if ( lumiSection.hdr.numBunches > HCAL_HLX_MAX_BUNCHES ) {
      cerr << "Too many bunches in " << __PRETTY_FUNCTION__ << endl;
      // Discard
      return true;
    }
    if ( lumiSection.hdr.numHLXs > HCAL_HLX_MAX_HLXS ) {
      cerr << "Too many HLXs in " << __PRETTY_FUNCTION__ << endl;
      cerr << lumiSection.hdr.numHLXs << endl;
      cerr << HCAL_HLX_MAX_HLXS << endl;
      // Discard
      return true;
    }

    // todo - migrate later to use lumicalc data...

    // Done before histogram fill - worried about
    // root using dynamic allocation of memory...slower
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      mETHistogramData[j] = 0.0;
      mLHCHistogramData[j] = 0.0;
      mBelowBottomHistogramData[j] = 0.0;
      mBetweenBottomHistogramData[j] = 0.0;
      mAboveBottomHistogramData[j] = 0.0;
      mBelowTopHistogramData[j] = 0.0;
      mBetweenTopHistogramData[j] = 0.0;
      mAboveTopHistogramData[j] = 0.0;
      for ( u32 i = 0 ; i != lumiSection.hdr.numHLXs ; i++ ) {
	mETHistogramData[j] += static_cast<double>(lumiSection.etSum[i].data[j]);
	mLHCHistogramData[j] += static_cast<double>(lumiSection.lhc[i].data[j]);
	mBelowTopHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[0][j]);
	mBetweenTopHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[1][j]);
	mAboveTopHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[2][j]);
	mBelowBottomHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[3][j]);
	mBetweenBottomHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[4][j]);
	mAboveBottomHistogramData[j] += static_cast<double>(lumiSection.occupancy[i].data[5][j]);
      }
    }

    // Lock the ROOT mutex to stop the other distributors
    // accessing ROOT at the same time...
    mROOTMutex.Lock();

    this->DoETSumHistogram(lumiSection.hdr.numBunches);
    this->DoLHCHistogram(lumiSection.hdr.numBunches);
    this->DoBelowOccTopHistogram(lumiSection.hdr.numBunches);
    this->DoBetweenOccTopHistogram(lumiSection.hdr.numBunches);
    this->DoAboveOccTopHistogram(lumiSection.hdr.numBunches);
    this->DoBelowOccBottomHistogram(lumiSection.hdr.numBunches);
    this->DoBetweenOccBottomHistogram(lumiSection.hdr.numBunches);
    this->DoAboveOccBottomHistogram(lumiSection.hdr.numBunches);

    // Unlock the ROOT mutex
    mROOTMutex.Unlock();

    return true;
  }

}












