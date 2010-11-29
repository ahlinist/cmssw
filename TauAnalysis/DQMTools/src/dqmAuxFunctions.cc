#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <TH1.h>

#include <TPRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TString.h>
#include <TMath.h>

#include <iostream>

std::string terminate_dqmDirectory(const std::string& dqmDirectory)
{
  std::string dqmDirectory_terminated = dqmDirectory;

//--- add tailing '/'
  if ( dqmDirectory_terminated != "" && 
       dqmDirectory_terminated.find_last_of(dqmSeparator) != (dqmDirectory_terminated.length() - 1) ) 
    dqmDirectory_terminated.append(dqmSeparator);

  return dqmDirectory_terminated;
}

float* getBinning(const TAxis* axis)
{
  unsigned numBins = axis->GetNbins();

  float* binEdges_float = new float[numBins + 1];

  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    binEdges_float[iBin] = axis->GetBinLowEdge(iBin + 1);
  }

  binEdges_float[numBins] = axis->GetBinUpEdge(numBins);

  return binEdges_float;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TH1* getHistogram(DQMStore& dqmStore, const std::string& meName, bool& dqmError)
{
  std::string histogramName, histogramDirectory;
  separateMonitorElementFromDirectoryName(meName, histogramName, histogramDirectory);

  TH1* histogram = 0;

  MonitorElement* me = dqmStore.get(terminate_dqmDirectory(histogramDirectory).append(histogramName));
  if ( me ) {
    int meType = me->kind();
    if ( meType == MonitorElement::DQM_KIND_TH1F      ||
	 meType == MonitorElement::DQM_KIND_TH1S      ||
	 meType == MonitorElement::DQM_KIND_TH2F      ||
	 meType == MonitorElement::DQM_KIND_TH2S      ||
	 meType == MonitorElement::DQM_KIND_TH3F      ||
	 meType == MonitorElement::DQM_KIND_TPROFILE  ||
	 meType == MonitorElement::DQM_KIND_TPROFILE2D ) {
      try {
	histogram = me->getTH1();
      } catch ( cms::Exception& ex ) {
	edm::LogError ("getHistogram") 
	  << " Exception triggered when trying to access histogram associated to meName = " << meName << ","
	  << " MonitorElement type = " << meType << ":" << ex.what() << " !!";
	return histogram;
      } 
      if ( !histogram ) {
        edm::LogError ("getHistogram") 
  	  << " Failed to access histogram associated to meName = " << meName << " in DQMStore" << " --> skipping !!";
        return histogram;
      }

      //std::cout << "meName = " << meName << ": integral = " << histogram->Integral() << std::endl;
      
      if ( !histogram->GetSumw2N() ) histogram->Sumw2();
    } else {
      edm::LogError("getHistogram") << " MonitorElement name = " << meName << " is not of type histogram !!";
    }
  } else {
    edm::LogError("getHistogram") << " Failed to retrieve MonitorElement = " << meName << " !!";
    dqmError = true;
  }

  return histogram;
}

std::vector<TH1*> getHistograms(DQMStore& dqmStore, const std::vector<std::string>& meNames, bool& dqmError)
{
  std::vector<TH1*> histograms;

  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    TH1* histogram = getHistogram(dqmStore, *meName, dqmError);

    if ( histogram ) histograms.push_back(histogram);
  } 

  return histograms;
}

double getValue(DQMStore& dqmStore, const std::string& meName_full, bool& error)
{
  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_full, meName, dqmDirectory);
  MonitorElement* me = dqmStore.get(terminate_dqmDirectory(dqmDirectory).append(meName));
  if ( me ) {
    int meType = me->kind();
    if ( meType == MonitorElement::DQM_KIND_REAL ) return me->getFloatValue();
    else if ( meType == MonitorElement::DQM_KIND_INT  ) return me->getIntValue();
    else {
      edm::LogError ("getValue") << " MonitorElement = " << meName_full << " is of invalid Type !!";
      error = true;
    }
  } else {
    edm::LogError ("getValue") << " Failed to retrieve MonitorElement = " << meName_full << " !!";
    error = true;
  }
  
  return -1.;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::string dqmDirectoryName(const std::string& directory)
{
//--- add leading '/'
  std::string dirName = dqmRootDirectory;

  dirName += directory;

//--- add tailing '/'
  dirName = terminate_dqmDirectory(dirName);

//--- replace all instances of '//' by '/'
  while ( dirName.find(dqmSeparator2) != std::string::npos ) {
    dirName.replace(dirName.find(dqmSeparator2), dqmSeparator2.length(), dqmSeparator);
  }

  return dirName;
}

std::string dqmDirectoryName_dqmRootDirectoryOmitted(const std::string& directory)
{
  std::string dirName = dqmDirectoryName(directory);
  
  if ( dqmRootDirectory != "" && dirName.find(dqmRootDirectory) == 0 ) 
    dirName = std::string(dirName, dqmRootDirectory.length());

  return dirName;
}

std::string dqmSubDirectoryName(const std::string& directory, const std::string& subdirectory)
{ 
  std::string subDirName = subdirectory;

//--- remove characters specifying directory part from name of subdirectory
  if ( subDirName.find(directory) <= 1 ) subDirName.replace(subDirName.find(directory), directory.length(), "");

//--- remove tailing '/'s
  while ( subDirName.find(dqmSeparator) == 0 ) {
    subDirName.replace(subDirName.find(dqmSeparator), dqmSeparator.length(), "");
  }

//--- replace all instances of '//' by '/'
  while ( subDirName.find(dqmSeparator2) != std::string::npos ) {
    subDirName.replace(subDirName.find(dqmSeparator2), dqmSeparator2.length(), dqmSeparator);
  }

  return subDirName;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void dqmCheckExistence(DQMStore& dqmStore, const std::string& directoryName, const std::string& meName, int mode, int& errorFlag)
{
  MonitorElement* meOutput = dqmStore.get(dqmDirectoryName(directoryName).append(meName));
 
  if ( meOutput ) {
    switch ( mode ) {
    case 1: // print error message 
      edm::LogError ("dqmCheckExistence") 
	<< " meName = " << meName << " already exists in directoryName = " << directoryName << " --> skipping !!";
      errorFlag = 1;
      break;
    case 2:
      break;
    default:
      edm::LogError ("dqmCheckExistence") << " Undefined mode = " << mode << " !!";
      errorFlag = 1;
    }
  }
}

void dqmRegisterHistogram(DQMStore& dqmStore, TH1* histogram, const std::string& meName_full)
{
  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_full, meName, dqmDirectory);
  if ( dqmDirectory != "" ) dqmStore.setCurrentFolder(dqmDirectory);

  histogram->SetName(histogram->GetName());  
  if ( TH1F* h = dynamic_cast<TH1F*>(histogram) ) {
    dqmStore.book1D(meName, h);
  } else if ( TH1S* h = dynamic_cast<TH1S*>(histogram) ) {
    dqmStore.book1S(meName, h);
  } else if ( TH2F* h = dynamic_cast<TH2F*>(histogram) ) {
    dqmStore.book2D(meName, h);
  } else if ( TH2S* h = dynamic_cast<TH2S*>(histogram) ) {
    dqmStore.book2S(meName, h);
  } else if ( TH3F* h = dynamic_cast<TH3F*>(histogram) ) {
    dqmStore.book3D(meName, h);
  } else if ( TProfile* h = dynamic_cast<TProfile*>(histogram) ) {
    dqmStore.bookProfile(meName, h);
  } else if ( TProfile2D* h = dynamic_cast<TProfile2D*>(histogram) ) {
    dqmStore.bookProfile2D(meName, h);
  }
}

void dqmRegisterFloat(DQMStore& dqmStore, double meValue, const std::string& meName_full)
{ 
  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_full, meName, dqmDirectory);
  if ( dqmDirectory != "" ) dqmStore.setCurrentFolder(dqmDirectory);

  MonitorElement* me = dqmStore.bookFloat(meName);
  me->Fill(meValue);
}

double getPower_general(const std::string& meName, const std::string& arg, double defaultValue)
{
  std::string arg_full = std::string(meOptionsSeparator).append(arg);

  unsigned posStart = meName.find(arg_full);

  if ( posStart != std::string::npos ) {
    posStart += arg_full.length();
    
    unsigned posEnd = meName.find(std::string(meOptionsSeparator), posStart);
    
    std::string power_string = ( posEnd != std::string::npos ) ?
      std::string(meName, posStart, posEnd - posStart) : std::string(meName, posStart);
    
    //std::cout << "meName = " << meName << ": power_string = " << power_string << std::endl; 
    
    return atof(power_string.data());
  }

  edm::LogWarning ("getPower_general") 
    << " Failed to decode power for argument " << arg << " in meName = " << meName
    <<  " --> returning default value = " << defaultValue << " !!";
  return defaultValue;
}

double getPower_scale(const std::string& meName)
{
  return getPower_general(meName, "s", 1.);
}

double getPower_add(const std::string& meName)
{
  return getPower_general(meName, "a", 1.);
}

void dqmCopyMonitorElement(DQMStore& dqmStore, const std::string& inputDirectory, const std::string& meName_input,
			   const std::string& outputDirectory, const std::string& meName_output, 
			   double scaleFactor, double scaleFactorErr, int mode)
{
  //std::cout << "<dqmCopyMonitorElement>:" << std::endl;
  //std::cout << " inputDirectory = " << inputDirectory << std::endl;
  //std::cout << " meName_input = " << meName_input << std::endl;
  //std::cout << " outputDirectory = " << outputDirectory << std::endl;
  //std::cout << " meName_output = " << meName_output << std::endl;
  //std::cout << " scaleFactor = " << scaleFactor << std::endl;
  //std::cout << " scaleFactorErr = " << scaleFactorErr << std::endl;
/*
  std::string meName_full = dqmDirectoryName(inputDirectory).append(meName_input);
  //std::cout << " meName_full = " <<  meName_full << std::endl;
  std::string meName_full_dqmRootDirectoryOmitted = dqmDirectoryName_dqmRootDirectoryOmitted(inputDirectory).append(meName_input);
  //std::cout << " meName_full_dqmRootDirectoryOmitted = " <<  meName_full_dqmRootDirectoryOmitted << std::endl;

  MonitorElement* meInput = dqmStore.get(meName_full);
  if ( !meInput ) meInput = dqmStore.get(meName_full_dqmRootDirectoryOmitted);
 */
  std::string meName_full = terminate_dqmDirectory(inputDirectory).append(meName_input);

  MonitorElement* meInput = dqmStore.get(meName_full);
  //std::cout << " meInput = " << meInput << std::endl;
  if ( !meInput ) {
    edm::LogError ("dqmCopyMonitorElement") 
      << " Failed to access meName = " << meName_input << " in DQMStore" << " --> skipping !!";
    return;
  }
      
  //-----------------------------------------------------begin of TauAnalysis specific code 
  //
  // WARNING: the following code is handles MonitorElements of different type differently;
  //          the handling is specific to the TauAnalysis !!
  //
  //           o histograms are scaled then added
  //           o strings are simply copied from input to output
  //             (strings are used to encode run + event numbers in the TauAnalysis)
  //           o integer numbers are added
  //             (integer numbers are used in the TauAnalysis to count the number 
  //              of **unweighted** events passing different stages in the event selection)
  //           o real numbers are scaled then added
  //             (real numbers are used in the TauAnalysis to count the number 
  //              of **weighted** events passing different stages in the event selection)
  //
      
//--- skip "invalid" MonitorElements
  if ( meInput->kind() == MonitorElement::DQM_KIND_INVALID ) {
    edm::LogWarning ("dqmCopyMonitorElement") 
      << " MonitorElement meName = " << meName_full << " marked as invalid" << " --> skipping !!";
    return;
  }
  
  int meType = meInput->kind();
  if ( meType == MonitorElement::DQM_KIND_TH1F      ||
       meType == MonitorElement::DQM_KIND_TH1S      ||
       meType == MonitorElement::DQM_KIND_TH2F      ||
       meType == MonitorElement::DQM_KIND_TH2S      ||
       meType == MonitorElement::DQM_KIND_TH3F      ||
       meType == MonitorElement::DQM_KIND_TPROFILE  ||
       meType == MonitorElement::DQM_KIND_TPROFILE2D ) {
    TH1* histogram = 0;
    try {
      histogram = meInput->getTH1();
    } catch ( cms::Exception& ex ) {
      edm::LogError ("dqmCopyMonitorElement") 
	<< " Exception triggered when trying to access histogram associated to meName = " << meName_full << ","
	<< " MonitorElement type = " << meType << ":" << ex.what() << " !!";
      return;
    }
    if ( !histogram ) {
      edm::LogError ("dqmCopyMonitorElement") 
	<< " Failed to access histogram associated to meName = " << meName_full << " in DQMStore" << " --> skipping !!";
      return;
    }
	
    std::auto_ptr<TH1> clone(dynamic_cast<TH1*>(histogram->Clone()));

//--- do not scale profile histograms
//    (as their y-axis represents average values of observables,
//     **not** number of events)
    if ( !(meType == MonitorElement::DQM_KIND_TPROFILE   ||
	   meType == MonitorElement::DQM_KIND_TPROFILE2D) ) {
      if ( scaleFactorErr > 0. ) {
	unsigned numBinsX = clone->GetNbinsX();
	for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	  unsigned numBinsY = clone->GetNbinsY();
	  for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	    unsigned numBinsZ = clone->GetNbinsZ();
	    for ( unsigned iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	      double binContent = clone->GetBinContent(iBinX, iBinY, iBinZ);
	      double binError = clone->GetBinError(iBinX, iBinY, iBinZ);
	      
	      clone->SetBinContent(iBinX, iBinY, iBinZ, scaleFactor*binContent);
	      clone->SetBinError(iBinX, iBinY, iBinZ, 
				 TMath::Sqrt(TMath::Power(scaleFactor*binError, 2) + TMath::Power(scaleFactorErr*binContent, 2)));
	    }
	  }
	}
      } else {
	clone->Scale(scaleFactor);
      }
    }
	
    dqmStore.setCurrentFolder(outputDirectory);
	
//--- check if output MonitorElement already exists
    int error = 0;
    dqmCheckExistence(dqmStore, outputDirectory, meName_output, mode, error);
    if ( error ) return;
	
    std::string meNameOutput_full = dqmDirectoryName(outputDirectory).append(meName_output);
    MonitorElement* meOutput = dqmStore.get(meNameOutput_full);
    if ( meOutput ) {
//--- add histogram to outputHistogram
      //std::cout << "--> adding to existing histogram." << std::endl;
      try {
        meOutput->getTH1()->Add(clone.get());
      } catch ( cms::Exception& ex ) {
        edm::LogError ("dqmCopyMonitorElement") 
	  << " Exception triggered when trying to add histogram associated to meName = " << meName_full
          << " to existing histogram associated to meName = " << meNameOutput_full << ":" << ex.what() << " !!";
	return;
      }
    } else {
//--- create new outputHistogram
      //std::cout << "--> registering as new histogram." << std::endl;
      dqmRegisterHistogram(dqmStore, clone.release(), meNameOutput_full);
    }
  } else if ( meType == MonitorElement::DQM_KIND_INT ) {
    int intValue = meInput->getIntValue();
    
    dqmStore.setCurrentFolder(outputDirectory);
	
//--- check if output MonitorElement already exists
    int error = 0;
    dqmCheckExistence(dqmStore, outputDirectory, meName_output, mode, error);
    if ( error ) return;
    
    MonitorElement* meOutput = dqmStore.get(dqmDirectoryName(outputDirectory).append(meName_output));
    if ( meOutput ) {
      int intSum = meOutput->getIntValue();
      meOutput->Fill(intSum + intValue);
    } else {
      meOutput = dqmStore.bookInt(meName_output);
      meOutput->Fill(intValue);
    }
  } else if ( meType == MonitorElement::DQM_KIND_REAL ) {
    double realValue = meInput->getFloatValue();
    //std::cout << "realValue = " << realValue << std::endl;
    double power_scale = getPower_scale(meName_input);
    //std::cout << "power_scale = " << power_scale << std::endl;
    realValue *= TMath::Power(scaleFactor, power_scale);
    
    dqmStore.setCurrentFolder(outputDirectory);

//--- check if output MonitorElement already exists
    int error = 0;
    dqmCheckExistence(dqmStore, outputDirectory, meName_output, mode, error);
    if ( error ) return;
    
    MonitorElement* meOutput = dqmStore.get(dqmDirectoryName(outputDirectory).append(meName_output));
    if ( meOutput ) {
      double power_add = getPower_add(meName_input);
      //std::cout << "power_add = " << power_add << std::endl;
      double realSum = meOutput->getFloatValue();
      //std::cout << "realSum = " << realSum << std::endl;
      meOutput->Fill(TMath::Power(TMath::Power(realSum, power_add) + TMath::Power(realValue, power_add), 1./power_add));
    } else {
      meOutput = dqmStore.bookFloat(meName_output);
      meOutput->Fill(realValue);
    }
  } else if ( meType == MonitorElement::DQM_KIND_STRING ) {
    std::string stringValue = meInput->getStringValue();
    
    dqmStore.setCurrentFolder(outputDirectory);

//--- check if output MonitorElement already exists
    int error = 0;
    dqmCheckExistence(dqmStore, outputDirectory, meName_output, mode, error);
    if ( error ) return;
    
    //std::cout << " --> calling DQMStore::bookString" << std::endl;
    dqmStore.bookString(meName_output, stringValue);
  } else {
    edm::LogError ("dqmCopyMonitorElement") 
      << " MonitorElement meName = " << meName_input << " of unknown type" << " --> skipping !!";
    return;
  }
  //
  //-------------------------------------------------------end of TauAnalysis specific code 
}

void dqmCopyRecursively(DQMStore& dqmStore, const std::string& inputDirectory, const std::string& outputDirectory, 
			double scaleFactor, double scaleFactorErr, int mode, 
			bool rmInputDirectory, std::vector<outputCommandEntry>* outputCommands)
{
  //std::cout << "<dqmCopyRecursively>:" << std::endl;
  //std::cout << " inputDirectory = " << inputDirectory << std::endl;
  //std::cout << " outputDirectory = " << outputDirectory << std::endl;
  //std::cout << " scaleFactor = " << scaleFactor << std::endl;
  //std::cout << " scaleFactorErr = " << scaleFactorErr << std::endl;
  //std::cout << " rmInputDirectory = " << rmInputDirectory << std::endl;

  dqmStore.setCurrentFolder(inputDirectory);

//--- check if current inputDirectory is to be kept or dropped
  bool copyMonitorElements = true;
  if ( outputCommands ) {
    TString outputDirectory_tstring = outputDirectory.data();

    for ( std::vector<outputCommandEntry>::iterator outputCommand = outputCommands->begin();
	  outputCommand != outputCommands->end(); ++outputCommand ) {
      int statement = outputCommand->first;
      //std::cout << " statement = " << statement << std::endl;

      TPRegexp& dqmDirectory_regexp = outputCommand->second;
      //std::cout << " dqmDirectory_regexp = " << dqmDirectory_regexp.GetPattern() << std::endl;

      //std::cout << "--> dqmDirectory_regexp.Match(outputDirectory_tstring) = " 
      //          << dqmDirectory_regexp.Match(outputDirectory_tstring) << std::endl;

      if ( dqmDirectory_regexp.Match(outputDirectory_tstring) == 1 ) {
	if ( statement == kKeep ) copyMonitorElements = true;
	if ( statement == kDrop ) copyMonitorElements = false;
      }
    }
  }

//--- copy all monitor elements in current inputDirectory to the outputDirectory
  bool meInput_copied = false;
  if ( copyMonitorElements ) {    
    dqmStore.setCurrentFolder(inputDirectory);
    std::vector<std::string> meNames = dqmStore.getMEs();
    //std::cout << " #meNames = " << meNames.size() << std::endl;
    for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	  meName != meNames.end(); ++meName ) {
      dqmCopyMonitorElement(dqmStore, inputDirectory, *meName, outputDirectory, *meName, scaleFactor, scaleFactorErr, mode);
      meInput_copied = true;
    }
  }

//--- call function recursively for all sub-directories
  dqmStore.setCurrentFolder(inputDirectory);
  std::vector<std::string> dirNames = dqmStore.getSubdirs();
  for ( std::vector<std::string>::const_iterator dirName = dirNames.begin();
	dirName != dirNames.end(); ++dirName ) {
    std::string subDirName = dqmSubDirectoryName(inputDirectory, *dirName);

    std::string inputDirName_full = terminate_dqmDirectory(inputDirectory).append(subDirName);
    std::string outputDirName_full = dqmDirectoryName(outputDirectory).append(subDirName);

    dqmCopyRecursively(dqmStore, inputDirName_full, outputDirName_full, 
		       scaleFactor, scaleFactorErr, mode, rmInputDirectory, outputCommands);
  }

//--- delete inputDirectory 
//    (if requested to do so and inputDirectory is **not empty**;
//     otherwise, common parent directories of inputDirectory and outputDirectory might get deleted !!)
  //std::cout << "--> meInput_copied = " << meInput_copied << std::endl;
  if ( rmInputDirectory && meInput_copied ) dqmStore.rmdir(inputDirectory);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void printMonitorElementValue(MonitorElement* me, std::ostream& stream)
{
  int meType = me->kind();
  if ( meType == MonitorElement::DQM_KIND_INT ) {
    stream << me->getIntValue();
  } else if ( meType == MonitorElement::DQM_KIND_REAL ) {
    stream << me->getFloatValue();
  } else if ( meType == MonitorElement::DQM_KIND_STRING ) {
    stream << me->getStringValue();
  } 
}

void dqmDumpMonitorElement(DQMStore& dqmStore, const std::string& meName, const std::string& meName_err, 
			   const std::string& label, std::ostream& stream)
{
  MonitorElement* me = dqmStore.get(meName);
  MonitorElement* me_err = ( meName_err != "" ) ? dqmStore.get(meName_err) : 0;

  if ( !me ) {
    edm::LogError ("dqmDumpMonitorElement") 
      << " Failed to retrieve MonitorElement " << meName << " from dqmStore !!";
    return;
  }

  int meType = me->kind();
  if ( meType == MonitorElement::DQM_KIND_TH1F      ||
       meType == MonitorElement::DQM_KIND_TH1S      ||
       meType == MonitorElement::DQM_KIND_TH2F      ||
       meType == MonitorElement::DQM_KIND_TH2S      ||
       meType == MonitorElement::DQM_KIND_TH3F      ||
       meType == MonitorElement::DQM_KIND_TPROFILE  ||
       meType == MonitorElement::DQM_KIND_TPROFILE2D ) {
    TH1* histogram = 0;
    try {
      histogram = me->getTH1();
    } catch ( cms::Exception& ex ) {
      edm::LogError ("dqmDumpMonitorElement") 
	<< " Exception triggered when trying to access histogram associated to meName = " << meName << ","
	<< " MonitorElement type = " << meType << ":" << ex.what() << " !!";
      return;
    }
    if ( !histogram ) {
      edm::LogError ("dqmDumpMonitorElement") 
	<< " Failed to access histogram associated to meName = " << meName << " in DQMStore" << " --> skipping !!";
      return;
    }

    stream << " " << label << ":" << std::endl;

    unsigned numBinsX = histogram->GetNbinsX();
    for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      unsigned numBinsY = histogram->GetNbinsY();
      for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	unsigned numBinsZ = histogram->GetNbinsZ();
	for ( unsigned iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	  stream << "  binX = " << iBinX;
	  if ( histogram->GetNbinsY() > 1 ) stream << ", binY = " << iBinY;
	  if ( histogram->GetNbinsZ() > 1 ) stream << ", binZ = " << iBinZ;
	  stream << " (x = " << histogram->GetXaxis()->GetBinCenter(iBinX);
	  if ( histogram->GetNbinsY() > 1 ) stream << ", y = " << histogram->GetYaxis()->GetBinCenter(iBinY);
	  if ( histogram->GetNbinsZ() > 1 ) stream << ", z = " << histogram->GetZaxis()->GetBinCenter(iBinZ);
	  stream << "): " << histogram->GetBinContent(iBinX, iBinY, iBinZ) 
		 << " +/- " << histogram->GetBinError(iBinX, iBinY, iBinZ) << std::endl;
	}
      }
    }
  } else if ( meType == MonitorElement::DQM_KIND_INT ) {
    stream << " " << label << ": " << me->getIntValue();
    if ( me_err ) {
      stream << " +/- ";
      printMonitorElementValue(me_err, stream);
    }
    stream << std::endl;
  } else if ( meType == MonitorElement::DQM_KIND_REAL ) {
    stream << " " << label << ": " << me->getFloatValue();
    if ( me_err ) {
      stream << " +/- ";
      printMonitorElementValue(me_err, stream);
    }
    stream << std::endl;
  } else if ( meType == MonitorElement::DQM_KIND_STRING ) {
    stream << " " << label << ": " << me->getStringValue() << std::endl;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void separateMonitorElementFromDirectoryName(const std::string& meName_full, std::string& meName, std::string& dqmDirectoryName)
{
  std::string tempName = meName_full;

//--- remove DQM root directory from histogram name
  std::string::size_type dqmRootDirectoryPos = tempName.find(dqmRootDirectory);
  if ( dqmRootDirectoryPos != std::string::npos ) {  
    tempName.replace(dqmRootDirectoryPos, dqmRootDirectory.size(), "");  
  }  
 
//--- extract directory from histogram name
  std::string::size_type lastPos;
  std::string::size_type nextPos = tempName.find(dqmSeparator);  
  do {
    lastPos = nextPos;
    nextPos = tempName.find(dqmSeparator, lastPos + 1);
  } while ( nextPos != std::string::npos );

  meName = ( lastPos != std::string::npos ) ? std::string(tempName, lastPos + 1, tempName.length()) : tempName;
  dqmDirectoryName = dqmRootDirectory;
  if ( lastPos != std::string::npos ) dqmDirectoryName += std::string(tempName, 0, lastPos);
}

