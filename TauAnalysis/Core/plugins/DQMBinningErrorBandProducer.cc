#include "TauAnalysis/Core/plugins/DQMBinningErrorBandProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

const std::string meNamePattern_binContent = "binContent_region";
const std::string meNamePattern_binError = "binError_region";

DQMBinningErrorBandProducer::DQMBinningErrorBandProducer(const edm::ParameterSet& cfg)
  : DQMErrorBandProducerBase(cfg)
{
// nothing to be done yet...
}

DQMBinningErrorBandProducer::~DQMBinningErrorBandProducer() 
{
// nothing to be done yet...
}

void DQMBinningErrorBandProducer::computeErrorBandImp(DQMStore& dqmStore, 
						   const std::string& dqmDirectory_centralValue, 
						   const std::vector<std::string>& dqmDirectories_variance, 
						   const std::string& dqmDirectory_output, 
						   const std::string& method, int nDoF)
{
  //std::cout << "<DQMBinningErrorBandProducer::computeErrorBand>:" << std::endl;
  //std::cout << " inputDirectory_centralValue = " << inputDirectory_centralValue << std::endl;
  //std::cout << " inputDirectories_variance = " << format_vstring(inputDirectories_variance) << std::endl;
  //std::cout << " outputDirectory = " << outputDirectory << std::endl;
  //std::cout << " nDoF = " << nDoF << std::endl;

  dqmStore.setCurrentFolder(dqmDirectory_centralValue);

//--- compute error-band for all MonitorElements stored in current DQM directory
  std::vector<std::string> meNames = dqmStore.getMEs();
  for ( std::vector<std::string>::const_iterator meName_binContent = meNames.begin();
	meName_binContent != meNames.end(); ++meName_binContent ) {
    
    if ( !meName_binContent->find(meNamePattern_binContent) == 0 ) continue;
    
    std::string meName_abbrevBinContent = meName_binContent->substr(0, meName_binContent->find(meOptionsSeparator));
    
    int errorFlag = 0;
    std::string meName_abbrevBinError = 
      replace_string(meName_abbrevBinContent, meNamePattern_binContent, meNamePattern_binError, 0, 1, errorFlag); 

    std::string meName_binError = "";
    for ( std::vector<std::string>::const_iterator meName_i = meNames.begin();
	  meName_i != meNames.end(); ++meName_i ) {
      if ( meName_i->find(meName_abbrevBinError) == 0 ) meName_binError = (*meName_i);
    }
    
    if ( meName_binError == "" ) {
      edm::LogWarning ("computeErrorBandImp") 
	<< " Failed to find MonitorElement representing binError matching binContent" 
	<< " represented by meName = " << (*meName_binContent) << " --> skipping !!";
    }
    
    bool dqmError = false;
    
    std::string meName_binContent_centralValue = dqmDirectoryName(dqmDirectory_centralValue).append(*meName_binContent);
    double binContent_centralValue = getValue(dqmStore, meName_binContent_centralValue, dqmError);

    //std::string meName_binError_centralValue = dqmDirectoryName(dqmDirectory_centralValue).append(meName_binError);
    //double binError_centralValue = getValue(dqmStore, meName_binError_centralValue, dqmError);

    std::vector<double> binContents_variance;
    std::vector<double> binErrors_variance;
    for ( std::vector<std::string>::const_iterator dqmDirectory_variance = dqmDirectories_variance.begin();
	  dqmDirectory_variance != dqmDirectories_variance.end(); ++dqmDirectory_variance ) {
      std::string meName_binContent_variance = dqmDirectoryName(*dqmDirectory_variance).append(*meName_binContent);
      double binContent_variance = getValue(dqmStore, meName_binContent_variance, dqmError);
      binContents_variance.push_back(binContent_variance);

      std::string meName_binError_variance = dqmDirectoryName(*dqmDirectory_variance).append(meName_binError);
      double binError_variance = getValue(dqmStore, meName_binError_variance, dqmError);
      binErrors_variance.push_back(binError_variance);
    }
    
    std::string meName_binContent_output = dqmDirectoryName(dqmDirectory_output).append(*meName_binContent);
    MonitorElement* me_binContent_output = dqmStore.get(meName_binContent_output);
    
    std::string meName_binError_output = dqmDirectoryName(dqmDirectory_output).append(meName_binError);
    MonitorElement* me_binError_output = dqmStore.get(meName_binError_output);
          
    if ( dqmError ) {
      edm::LogWarning ("computeErrorBandImp") 
	<< " Failed to retrieve values for meName_binContent = " << (*meName_binContent) 
	<< " and/or meName_binError = " << meName_binError << " --> skipping !!";
      continue;
    }
     
    double variance = 0.;
	    
    double binContent_min = 0.;
    double binContent_max = 0.;
    bool isFirstValue = true;
	    
    assert(binContents_variance.size() == binErrors_variance.size());

    size_t numEntries_variance = binContents_variance.size();
    for ( size_t iVariance = 0; iVariance < numEntries_variance; ++iVariance ) {
      double binContent_variance = binContents_variance[iVariance];
      //double binError_variance = binErrors_variance[iVariance];

      double diffBinContent = binContent_variance - binContent_centralValue;
      variance += diffBinContent*diffBinContent;
	      
      if ( binContent_variance < binContent_min || isFirstValue ) binContent_min = binContent_variance;
      if ( binContent_variance > binContent_max || isFirstValue ) binContent_max = binContent_variance;
      isFirstValue = false;
    }
	    
    if ( method == method_sqrt ) {
      double binError = ( nDoF > 0 ) ? TMath::Sqrt(variance/nDoF) : 0.;
      me_binError_output->Reset();
      me_binError_output->Fill(binError);
    } else if ( method == method_min_max ) {
      double binContent_centralValue = 0.5*(binContent_max + binContent_min);
      me_binContent_output->Reset();
      me_binContent_output->Fill(binContent_centralValue);
      
      double binError = 0.5*(binContent_max - binContent_min);
      me_binError_output->Reset();
      me_binError_output->Fill(binError);
    } else {
      edm::LogWarning ("computeErrorBandImp") 
	<< " Unsupported method = " << method << " for computing error-band --> skipping !!";
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMBinningErrorBandProducer);
