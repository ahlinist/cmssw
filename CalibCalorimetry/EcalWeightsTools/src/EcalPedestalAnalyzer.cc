/**
 * \file EcalPedestalAnalyzer.h 
 * Dump in a file values of pedestal
 * 
 * $Date: 2006/07/19 22:01:19 $
 * $Revision: 1.3 $
 * \author R. Bruneliere'
 *
*/

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "CalibCalorimetry/EcalWeightsTools/interface/EcalPedestalAnalyzer.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

EcalPedestalAnalyzer::EcalPedestalAnalyzer(edm::ParameterSet const& pSet) : 
  verbosity_(pSet.getUntrackedParameter("verbosity", 1U)),
  nChannels_(0), iEvent_(0)
{
  //
}

EcalPedestalAnalyzer::~EcalPedestalAnalyzer()
{
  int gainValues[kGains] = {12, 6, 1};
  
  // Create output text file and root file
  std::ofstream fPedestal;
  fPedestal.open("pedestal.txt");
  TFile* fOut = new TFile("pedestal.root", "recreate");
  fOut->cd();
  TH1F** hNoiseDistri = new TH1F*[kGains];
  TH2F** hPedestalVsPos = new TH2F*[kGains];
  TH2F** hNoiseVsPos = new TH2F*[kGains];
  for (int gainId = 0; gainId < kGains; gainId++) {
    std::string hTitle = Form("Noise gain x%d", gainValues[gainId]);
    hNoiseDistri[gainId] = new TH1F(Form("hNoiseDistri_%d", gainId),
				    hTitle.c_str(), 500, 0., 5.);
    hTitle = Form("Pedestal vs (#eta, #phi) gain x%d", gainValues[gainId]);
    hPedestalVsPos[gainId] = new TH2F(Form("hPedestalVsPos_%d", gainId),
				      hTitle.c_str(),
				      85, 0., 85., 20, 0., 20.);
    hTitle = Form("Noise vs (#eta, #phi) gain x%d", gainValues[gainId]);
    hNoiseVsPos[gainId] = new TH2F(Form("hNoiseVsPos_%d", gainId),
				   hTitle.c_str(),
				   85, 0., 85., 20, 0., 20.);
  }
  
  // Loop over Ecal barrel channels
  for (int iChannel = 0; iChannel < nChannels_; iChannel++) {
    int ieta = indexToChannelPos_[iChannel].first;
    int iphi = indexToChannelPos_[iChannel].second;
    if (verbosity_)
      std::cout << "eta = " << ieta << ", phi = " << iphi 
		<< ":" << std::endl;
    fPedestal << std::setw(3) << ieta << " " << std::setw(3) << iphi << " ";
    
    // Loop over different gains
    for (int gainId = 0; gainId < kGains; gainId++) {
      int arrayId = gainId*nChannels_ + iChannel;	
      if (nEntries_[arrayId]) {
	pedestal_[arrayId] /= float(nEntries_[arrayId]);
	noise_[arrayId] = sqrt(noise_[arrayId]/float(nEntries_[arrayId]));
      }
      if (verbosity_)
	std::cout << "gain x" << gainValues[gainId]
		  << ": n = " << nEntries_[arrayId] 
		  << ", pedestal = " << pedestal_[arrayId] 
		  << ", noise = " << noise_[arrayId] 
		  << std::endl;
      fPedestal << std::setw(10) << std::setprecision(7) 
		<< pedestal_[arrayId] << " " 
		<< std::setw(10) << std::setprecision(7) 
		<< noise_[arrayId] << " ";
      hNoiseDistri[gainId]->Fill(noise_[arrayId]);
      hPedestalVsPos[gainId]->SetBinContent(ieta, iphi, pedestal_[arrayId]);
      hNoiseVsPos[gainId]->SetBinContent(ieta, iphi, noise_[arrayId]);
    }
    fPedestal << std::endl;
  }
  
  // Close files
  fOut->Write();
  delete fOut;
  fPedestal.close();
}

void EcalPedestalAnalyzer::analyze(const edm::Event& e, 
				   const edm::EventSetup& c) 
{
  if (verbosity_ > 1)
    std::cout << "Starting EcalPedestalAnalyzer::analyze for event "
	      << iEvent_ << std::endl;
  
  // Get ecal barrel digis
  edm::Handle<EBDigiCollection> digis;
  //e.getByLabel("ecalEBunpacker", digis);
  e.getByLabel("ecalTBunpack", digis);
  
  // Initialize vectors if not already done
  if (int(digis->size()) > nChannels_) {
    nChannels_ = digis->size();
    if (verbosity_)
      std::cout << "EcalPedestalAnalyzer::analyze: collection size = " 
		<< digis->size() << std::endl;
    nEntries_.resize(nChannels_*kGains, 0);
    pedestal_.resize(nChannels_*kGains, 0.);
    noise_.resize(nChannels_*kGains, 0.);
    indexToChannelPos_.resize(nChannels_);
  }
  
  // Loop over Ecal barrel digis
  for (EBDigiCollection::const_iterator digiItr = digis->begin(); 
       digiItr != digis->end(); ++digiItr) {
    
    // Get (ieta, iphi) coordinates
    std::pair<int, int> channelPos(EBDetId((*digiItr).id()).ieta(), 
				   EBDetId((*digiItr).id()).iphi());
    int iChannel = int(digiItr - digis->begin());
    if (!nEntries_[iChannel]) {
      indexToChannelPos_[iChannel] = channelPos;
      channelPosToIndex_[channelPos] = iChannel;
    }
    iChannel = channelPosToIndex_[channelPos];
    if (iChannel < 0 || iChannel >= nChannels_) {
      if (verbosity_)
	std::cout << "EcalPedestalAnalyzer::analyze: Warning: iChannel = "
		  << iChannel << " at eta = " << channelPos.first
		  << " and phi = " << channelPos.second
		  << " for event " << iEvent_ << std::endl;
      continue;
    }
    if (verbosity_ > 1)
      std::cout << "EcalPedestalAnalyzer::analyze: "
		<< "dump the ADC counts for eta = " << channelPos.first
		<< " and phi = " << channelPos.second << std::endl;
    
    // Loop over time samples
    float delta;
    for (int iSample = 0; iSample < digiItr->size(); ++iSample) {
      float adc = float((*digiItr).sample(iSample).adc());
      int gainId = (*digiItr).sample(iSample).gainId();
      if (verbosity_ > 1) 
	std::cout << gainId << " " << adc << " ";
      if (gainId < kFirstGainId || gainId >= kFirstGainId + kGains) {
	if (verbosity_)
	  std::cout << "EcalPedestalAnalyzer::analyze: Warning: gainId = "
		    << gainId << " at eta = " << channelPos.first
		    << " and phi = " << channelPos.second
		    << " for event " << iEvent_ << std::endl;
	continue;
      }
      int arrayId = (gainId - kFirstGainId)*nChannels_ + iChannel; 
      nEntries_[arrayId]++;
      pedestal_[arrayId] += adc;
      if (nEntries_[arrayId] > 1) {
	delta = float(nEntries_[arrayId])*adc - pedestal_[arrayId];
	noise_[arrayId] += delta*delta/float(nEntries_[arrayId]*
					     (nEntries_[arrayId] - 1));
      }
    }
    if (verbosity_ > 1)
      std::cout << std::endl;
  }
  
  if (verbosity_ > 1)
    std::cout << "Finishing EcalPedestalAnalyzer::analyze for event "
	      << iEvent_ << std::endl;
  iEvent_++;
}
