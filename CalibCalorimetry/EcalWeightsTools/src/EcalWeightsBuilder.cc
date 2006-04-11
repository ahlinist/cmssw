/**
 * \file EcalWeightsBuilder.cc 
 * Dump in a root file pulse shapes
 * 
 * 
 * $Date: 2005/08/12 18:06:48 $
 * $Revision: 1.1 $
 * \author R. Bruneliere'
 *
 * $Date: 2006/10/04 14:31:31 $
 * $Revision: 1.2 $
 * Updated by Alex Zabi.
*/

#include "TFile.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TF1.h"
#include "TMath.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/ComputeWeights.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalWeightsBuilder.h"

#include <iostream>
#include <iomanip>
#include <fstream>

EcalWeightsBuilder::EcalWeightsBuilder(edm::ParameterSet const& pSet) : 
  verbosity_(pSet.getUntrackedParameter("verbosity", 1U)),
  nBinsHisto_(pSet.getUntrackedParameter<int>("nBinsHisto", 10)),
  xMinHisto_(pSet.getUntrackedParameter<double>("xMinHisto", 0.)),
  xMaxHisto_(pSet.getUntrackedParameter<double>("xMaxHisto", 10.)),
  pedestalFileName_(pSet.getUntrackedParameter<std::string>("pedestalFile","pedestal.txt")),
  nChannels_(0), iEvent_(0)
{
  if (verbosity_ > 0)
    std::cout << "Constructing EcalWeightsBuilder::EcalWeightsBuilder" << std::endl;
  
  //READ PEDESTAL FILE
  std::ifstream pedestalFile;
  pedestalFile.open(pedestalFileName_.c_str());
  if (pedestalFile.is_open()) {
    int ieta, iphi;
    std::vector<float> pedestal(kGains, 0.);
    std::vector<float> noise(kGains, 0.);
    int index = 0;
    while (!pedestalFile.eof()) {
      pedestalFile >> ieta >> iphi;
      for (int gainId = 0; gainId < kGains; gainId++)
	pedestalFile >> pedestal[gainId] >> noise[gainId];
      std::pair<int, int> channelPos(ieta, iphi);
      if (channelPosToIndexPed_[channelPos] < 1) {
	channelPosToIndexPed_[channelPos] = index + 1;
	pedestal_.push_back(pedestal);
	index++;
      }//loop channel
    }//loop file
    pedestalFile.close();
  } else {
    std::cout << "EcalWeightsBuilder::EcalWeightsBuilder: Error: "
	      << "can not open pedestal file " << pedestalFileName_
	      << std::endl;
    abort();
  }
  
  // Open output root file
  if (verbosity_ > 1)
    std::cout << "EcalWeightsBuilder::EcalWeightsBuilder: opening root file" << std::endl;
  fOut_ = new TFile("pulse.root", "recreate");
  if (verbosity_ > 1)
    std::cout << "End Constructing EcalWeightsBuilder::EcalWeightsBuilder" << std::endl;
}//CONSTRUCTOR

EcalWeightsBuilder::~EcalWeightsBuilder()
{
  if (verbosity_ > 0)
    std::cout << "Destructing EcalWeightsBuilder::~EcalWeightsBuilder"
	      << std::endl;
  
  //
  // COMPUTING WEIGHTS
  //
  // Create object used to compute weights
  int verbosity = 1;
  bool doFitBaseline = true;
  bool doFitTime = false;
  int nPulseSamples = 5;
  int nPrePulseSamples = 3;
  ComputeWeights weights(verbosity, doFitBaseline, doFitTime, nPulseSamples, nPrePulseSamples);

  // Create output files
  std::ofstream fAmpWeights, fPedWeights, fChi2;
  fAmpWeights.open("ampWeights.txt");
  if (doFitBaseline) fPedWeights.open("pedWeights.txt");
  fChi2.open("chi2Matrix.txt");

  // Loop over histograms
  std::map< std::pair<int, int>, int >::const_iterator it;
  //it = channelPosToIndexHist_.begin();
  for (it = channelPosToIndexHist_.begin(); 
       it != channelPosToIndexHist_.end(); it++) {
    //for (int ii = 0; ii < 85*19; ii++) {
    //it++;
    int index = (*it).second - 1;
    int ieta = (*it).first.first;
    int iphi = (*it).first.second;
    if (verbosity_ > 1)
      std::cout << "EcalWeightsBuilder::~EcalWeightsBuilder: computing weights "
		<< "for ieta = " << ieta << ", iphi = " << iphi << std::endl;
    if (!hPulseProf_[index]) {
      if (verbosity)
	std::cout << "EcalWeightsBuilder::~EcalWeightsBuilder: Warning:"
		  << " histogram is not existing for ieta = " << ieta
		  << ", iphi = " << iphi << std::endl;
      continue;
    }// check histogram
    int nSamples = TMath::Nint(xMaxHisto_ - xMinHisto_);
    if (nSamples <= 3) {
      if (verbosity)
	std::cout << "EcalWeightsBuilder::~EcalWeightsBuilder: Warning:"
		  << " too few histogram samples for ieta = " << ieta
		  << ", iphi = " << iphi << std::endl;
      continue;
    }// check sample

    // Get histogram maximum by a pol3 fit
    //double xMax = hPulseProf_[index]->
    //GetBinCenter(hPulseProf_[index]->GetMaximumBin());
    //hPulseProf_[index]->Fit("pol3", "0", "", xMax - 1., xMax + 2.5);
    //TF1* fPol3 = hPulseProf_[index]->GetFunction("pol3");
    double pulseMax = hPulseProf_[index]->GetMaximum(); // fPol3->GetMaximum(xMax - 1., xMax + 2.);
    std::vector<double> pulseShape(nSamples);
    std::vector<double> pulseShapeDerivative(nSamples);
    int nTdcBins = nBinsHisto_/nSamples;

    // Loop over tdc bins
    for (int iTdcBin = 0; iTdcBin < nTdcBins; iTdcBin++) {
      for (int iSample = 0; iSample < nSamples; iSample++) {
	int bin = iSample*nTdcBins + iTdcBin + 1;

	// NORMALIZING PROFILE HISTOGRAMS
	if (pulseMax > 100.)
	  pulseShape[iSample] = hPulseProf_[index]->GetBinContent(bin)/pulseMax;
	else
	  pulseShape[iSample] = 0.;
	pulseShapeDerivative[iSample] = 0.;
      }//loop sample
      
      //COMPUTING WEIGHTS
      if (!weights.compute(pulseShape, pulseShapeDerivative)) {
	if (verbosity_)
	  std::cout << "EcalWeightsBuilder::~EcalWeightsBuilder: Warning:"
		    << " Impossible to compute weights for ieta = " << ieta
		    << ", iphi = " << iphi << " and iTdcBin = " << iTdcBin
		    << std::endl;
	continue;
      }

      //WRITING WEIGHTS INTO OUTPUT FILE
      if (!iTdcBin) {
	fAmpWeights << ieta << " " << iphi << " " << nSamples << " " << nTdcBins << std::endl;
	if (doFitBaseline)
	  fPedWeights << ieta << " " << iphi << " " << nSamples << " " << nTdcBins << std::endl;
	fChi2 << ieta << " " << iphi << " " << nSamples << " " << nTdcBins << std::endl;
      }
      for (int iSample = 0; iSample < nSamples; iSample++) {
	fAmpWeights << std::setw(10) << std::setprecision(7) << weights.getAmpWeight(iSample) << " "; 
	if (doFitBaseline)
	  fPedWeights << std::setw(10) << std::setprecision(7) << weights.getPedWeight(iSample) << " "; 
	for (int iSample2 = 0; iSample2 < nSamples; iSample2++)
	  fChi2 << std::setw(10) << std::setprecision(7) << weights.getChi2Matrix(iSample, iSample2) << " "; 
	fChi2 << std::endl;
      }
      fAmpWeights << std::endl;
      if (doFitBaseline) fPedWeights << std::endl;

    }//loop TDC
  }//loop Channels.

  //CLOSING FILES
  fAmpWeights.close();
  if (doFitBaseline) fPedWeights.close();
  fChi2.close();

  // Close output root file
  fOut_->Write();
  //delete fOut_;
  // ====>
  // Problem during destruction between root and clhep to solve.... maybe....
}//DESTRUCTOT

void EcalWeightsBuilder::analyze(const edm::Event& e, const edm::EventSetup& c) 
{
  if (verbosity_ > 1)
    std::cout << "Starting EcalWeightsBuilder::analyze for event "
	      << iEvent_ << std::endl;
  
  // Get ecal barrel digis
  edm::Handle<EBDigiCollection> digis;
  e.getByLabel("ecalEBunpacker", digis);
  
  // Loop over Ecal barrel digis
  for (EBDigiCollection::const_iterator digiItr = digis->begin(); 
       digiItr != digis->end(); ++digiItr) {
    
    // Get (ieta, iphi) coordinates
    std::pair<int, int> channelPos((*digiItr).id().ieta(),(*digiItr).id().iphi());

    if (!channelPosToIndexHist_[channelPos]) {
      nChannels_++;
      channelPosToIndexHist_[channelPos] = nChannels_;
      std::string hName = Form("hPulse_%d_%d", (*digiItr).id().ieta(),
			       (*digiItr).id().iphi());
      std::string hTitle = Form("Pulse shape at i#eta = %d and i#phi = %d", 
				(*digiItr).id().ieta(),
				(*digiItr).id().iphi());
      fOut_->cd();
      hPulse_.push_back(new TH2F(hName.c_str(), hTitle.c_str(),
				 nBinsHisto_, xMinHisto_, xMaxHisto_,
				 1025, -100., 4000.));
      hName = Form("hPulseProf_%d_%d",
		   (*digiItr).id().ieta(), (*digiItr).id().iphi());
      hTitle = Form("Pulse shape profile at i#eta = %d and i#phi = %d", 
		    (*digiItr).id().ieta(), (*digiItr).id().iphi());
      hPulseProf_.push_back(new TProfile(hName.c_str(), hTitle.c_str(),
					 nBinsHisto_, 
					 xMinHisto_, xMaxHisto_));
    }

    int iChannel = channelPosToIndexHist_[channelPos] - 1;
    if (iChannel < 0 || iChannel >= nChannels_) {
      if (verbosity_)
	std::cout << "EcalWeightsBuilder::analyze: Warning: iChannel = "
		  << iChannel << " at eta = " << channelPos.first
		  << " and phi = " << channelPos.second
		  << " for event " << iEvent_ << std::endl;
      continue;
    }
    if (verbosity_ > 1)
      std::cout << "EcalWeightsBuilder::analyze: "
		<< "dump the ADC counts for eta = " << channelPos.first
		<< " and phi = " << channelPos.second << std::endl;
    

    //CHECKING GAINS
    // Loop over samples and look gainId
    bool isSameGain = true;
    int gainId0 = kFirstGainId - 1;
    for (int iSample = 0; iSample < digiItr->size(); ++iSample) {
      int gainId = (*digiItr).sample(iSample).gainId();
      if (!iSample) gainId0 = gainId;
      if (gainId != gainId0) isSameGain = false;
    }//loop samples
    if (gainId0 < kFirstGainId || gainId0 >= kFirstGainId + kGains) isSameGain = false;

    // Keep only pulses where gain is the same for all samples
    if (!isSameGain) {
      if (verbosity_ > 1) 
	std::cout << "EcalWeightsBuilder::analyze: Warning: gainId "
		  << " was not constant at eta = " << channelPos.first
		  << " and phi = " << channelPos.second
		  << " for event " << iEvent_ << std::endl;
      continue;
    }//check gain
    
    //CHECKING PEDESTAL FILE
    // Keep only channels where pedestal was measured
    if (!channelPosToIndexPed_[channelPos]) {
      if (verbosity_ > 1) 
	std::cout << "EcalWeightsBuilder::analyze: Warning: no pedestal "
		  << "available at eta = " << channelPos.first
		  << " and phi = " << channelPos.second
		  << " for event " << iEvent_ << std::endl;
      continue;
    }// check pedestal

    float pedestal = pedestal_[channelPosToIndexPed_[channelPos] - 1][gainId0 - kFirstGainId];
    
    //WRITING HISTOS
    // Loop over samples to fill histo
    for (int iSample = 0; iSample < digiItr->size(); ++iSample) {
      float adc = float((*digiItr).sample(iSample).adc());
      hPulse_[iChannel]->Fill(float(iSample) + 0.5,float(adc) - pedestal);
      hPulseProf_[iChannel]->Fill(float(iSample) + 0.5,float(adc) - pedestal);
    }//loop samples
  }//loop digis

  if (verbosity_ > 1)
    std::cout << "Finishing EcalWeightsBuilder::analyze for event " << iEvent_ << std::endl;
  iEvent_++;
}//analyze
