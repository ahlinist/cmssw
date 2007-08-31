/*
  WedgeGIF distributor class
  Uses root to make histograms
  Copyright John Jones 2007
*/

// Prototype class definition
#include "WedgeGIFDistributor.hh"

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
  WedgeGIFDistributor::WedgeGIFDistributor(u32 wedge) {
    try {
      // TODO: CHECK NOT > 31
      // Copy the wedge number
      mWedge=wedge;
      // Initialise the data structures
      this->Init();
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  WedgeGIFDistributor::~WedgeGIFDistributor() {
    // Clean up the data structures
    this->CleanUp();
  }

  void WedgeGIFDistributor::Init() {
    mHistogramData = new double[4096];
    if ( !mHistogramData ) {
      //this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory for ET histogram");
      RAISE(lExc);
    }
  }

  void WedgeGIFDistributor::CleanUp() { 
    if ( mHistogramData ) {
      delete []mHistogramData;
      mHistogramData = 0;
    }
  }

  void WedgeGIFDistributor::GenerateETSumHistogram(const LUMI_SECTION & lumiSection) {

    TH1D *histogram = new TH1D("tempHist1","HLX ET Sum",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.etSum[mWedge].data[j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas1","tempCanvas1",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("ET Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/et_sum_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateLHCHistogram(const LUMI_SECTION & lumiSection) {

    TH1D *histogram = new TH1D("tempHist2",
			       "HLX LHC",
			       lumiSection.hdr.numBunches,
			       0,
			       lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.etSum[mWedge].data[j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas2","tempCanvas2",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("ET Count > threshold");
    histogram->Draw();

    std::ostringstream tempStringStream;
    std::string tempString = tempStringStream.str();
    tempStringStream << "tmp/lhc_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateAboveBottomOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist3","HLX Occupancy, Above Thresholds, Bottom FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[5][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas3","tempCanvas3",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_above_threshold_bottom_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateBetweenBottomOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist4","HLX Occupancy, Between Thresholds, Bottom FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[4][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas4","tempCanvas4",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_between_thresholds_bottom_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateBelowBottomOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist5","HLX Occupancy, Below Theshold, Bottom FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[3][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas5","tempCanvas5",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_below_threshold_bottom_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateAboveTopOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist6","HLX Occupancy, Above Thresholds, Top FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[2][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas6","tempCanvas6",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_above_threshold_top_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateBetweenTopOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist7","HLX Occupancy, Between Thresholds, Top FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[1][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas7","tempCanvas7",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_between_thresholds_top_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::GenerateBelowTopOccupancyHistogram(const LUMI_SECTION & lumiSection) {
    // CHECK occID < 6? - probablu doesn't matter as private and 'never touched' :)

    TH1D *histogram = new TH1D("tempHist8","HLX Occupancy, Below Theshold, Top FPGA",lumiSection.hdr.numBunches,0,lumiSection.hdr.numBunches);
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      histogram->Fill(j,static_cast<double>(lumiSection.occupancy[mWedge].data[0][j]));
    }

    TCanvas *canvas = new TCanvas("tempCanvas8","tempCanvas8",200,10,800,600);
    canvas->cd();
    canvas->SetFillColor(10);
    canvas->SetLogy();
    canvas->SetGridx();
    canvas->SetGridy();
    histogram->SetLineWidth(2);
    histogram->GetXaxis()->SetTitle("BX Number");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->Draw();

    std::ostringstream tempStringStream;
    tempStringStream << "tmp/occ_below_threshold_top_hlx_" << dec << mWedge << ".gif";
    canvas->SaveAs(tempStringStream.str().c_str());

    delete canvas;
    delete histogram;
  }

  void WedgeGIFDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 

    // Do ET sum
    this->GenerateETSumHistogram(lumiSection);

    // Do occupancies
    this->GenerateBelowTopOccupancyHistogram(lumiSection);
    this->GenerateBetweenTopOccupancyHistogram(lumiSection);
    this->GenerateAboveTopOccupancyHistogram(lumiSection);
    this->GenerateBelowBottomOccupancyHistogram(lumiSection);
    this->GenerateBetweenBottomOccupancyHistogram(lumiSection);
    this->GenerateAboveBottomOccupancyHistogram(lumiSection);

    // Do LHC
    this->GenerateLHCHistogram(lumiSection);

  }

}












