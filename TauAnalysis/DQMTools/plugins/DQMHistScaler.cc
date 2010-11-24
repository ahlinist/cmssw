#include "TauAnalysis/DQMTools/plugins/DQMHistScaler.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// framework & common header files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>

const int verbosity = 0;

DQMHistScaler::cfgEntryScaleJob::cfgEntryScaleJob(const edm::ParameterSet& cfg)
  : meName_inputErr_(""),
    meName_outputErr_(""),
    scaleFactor_(-1),
    scaleFactorErr_(0.),
    meName_scaleFactor_(""),
    meName_scaleFactorErr_(""),    
    meName_numerator_(""),
    meName_numeratorErr_(""),
    meName_denominator_(""),
    meName_denominatorErr_(""),
    meType_(""),
    dqmDirectory_factorizedLooseSel_(""),
    dqmDirectory_factorizedTightSel_(""),
    cfgError_(0)
{
  //std::cout << "<cfgEntryScaleJob::cfgEntryScaleJob>:" << std::endl;
  
  unsigned numScales = 0;
  
  if ( cfg.exists("scaleFactor") ) {
    scaleFactor_ = cfg.getParameter<double>("scaleFactor");
    scaleFactorErr_ = ( cfg.exists("scaleFactorErr") ) ? cfg.getParameter<double>("scaleFactorErr") : 0.;

    //std::cout << " scaleFactor = " << scaleFactor_ << std::endl;
    //std::cout << " scaleFactorErr = " << scaleFactorErr_ << std::endl;

    ++numScales;
  } 

  if ( cfg.exists("meName_scaleFactor") &&
       cfg.exists("meType") ) {
    meName_scaleFactor_ = cfg.getParameter<std::string>("meName_scaleFactor"); 

    if ( cfg.exists("meName_scaleFactorErr") ) {
      meName_scaleFactorErr_ = cfg.getParameter<std::string>("meName_scaleFactorErr"); 
    }
    
    meType_ = cfg.getParameter<std::string>("meType"); 
    
    //std::cout << " meName_scaleFactor = " << meName_scaleFactor_ << std::endl;
    //std::cout << " meName_scaleFactorErr = " << meName_scaleFactorErr_ << std::endl;
    //std::cout << " meType = " << meType_ << std::endl;

    ++numScales;
  }
  
  if ( cfg.exists("meName_numerator") && cfg.exists("meName_denominator") && 
       cfg.exists("meType") ) {
    meName_numerator_ = cfg.getParameter<std::string>("meName_numerator");
    meName_denominator_ = cfg.getParameter<std::string>("meName_denominator");

    if ( cfg.exists("meName_numeratorErr") && cfg.exists("meName_denominatorErr") ) {
      meName_numeratorErr_ = cfg.getParameter<std::string>("meName_numeratorErr");
      meName_denominatorErr_ = cfg.getParameter<std::string>("meName_denominatorErr");
    }

    meType_ = cfg.getParameter<std::string>("meType");
    
    //std::cout << " meName_numerator = " << meName_numerator_ << std::endl;
    //std::cout << " meName_numeratorErr = " << meName_numeratorErr_ << std::endl;
    //std::cout << " meName_denominator = " << meName_denominator_ << std::endl;
    //std::cout << " meName_denominatorErr = " << meName_denominatorErr_ << std::endl;
    //std::cout << " meType = " << meType_ << std::endl;
   
    if ( cfg.exists("dqmDirectory_factorizedLooseSel") &&
	 cfg.exists("dqmDirectory_factorizedTightSel") ) {
      dqmDirectory_factorizedLooseSel_ = cfg.getParameter<std::string>("dqmDirectory_factorizedLooseSel");
      dqmDirectory_factorizedTightSel_ = cfg.getParameter<std::string>("dqmDirectory_factorizedTightSel");
    
      //std::cout << " dqmDirectory_factorizedLooseSel = " << dqmDirectory_factorizedLooseSel_ << std::endl;
      //std::cout << " dqmDirectory_factorizedTightSel = " << dqmDirectory_factorizedTightSel_ << std::endl;
    }
   
    ++numScales;
  } 

  if ( cfg.exists("meName_input") &&
       cfg.exists("meName_output") ) {
    meName_input_ = cfg.getParameter<std::string>("meName_input");
    meName_output_ = cfg.getParameter<std::string>("meName_output");

    //std::cout << " meName_input = " << meName_input_ << std::endl;
    //std::cout << " meName_output = " << meName_output_ << std::endl;
    
    if ( cfg.exists("meName_inputErr") &&
	 cfg.exists("meName_outputErr") ) {
      meName_inputErr_ = cfg.getParameter<std::string>("meName_inputErr");
      meName_outputErr_ = cfg.getParameter<std::string>("meName_outputErr");
      
      //std::cout << " meName_inputErr = " << meName_inputErr_ << std::endl;
      //std::cout << " meName_outputErr = " << meName_outputErr_ << std::endl;
    }
  } else {
    dqmDirectory_input_ = cfg.getParameter<std::string>("dqmDirectory_input");
    //std::cout << " dqmDirectory_input = " << dqmDirectory_input_ << std::endl;
    if ( cfg.exists("dqmSubDirectories_input") ) dqmSubDirectories_input_ = cfg.getParameter<vstring>("dqmSubDirectories_input");
    //std::cout << " dqmSubDirectories_input = " << format_vstring(dqmSubDirectories_input_) << std::endl;
   
    dqmDirectory_output_ = cfg.getParameter<std::string>("dqmDirectory_output");
    //std::cout << " dqmDirectory_output = " << dqmDirectory_output_ << std::endl;
  }

//--- check that either:
//     o scaleFactor
//    or
//     o numerator + denominator
//    configuration parameters are defined 
  if ( numScales != 1 ) {
    edm::LogError("DQMHistScaler") 
      << " Need to specify either Configuration parameter 'scaleFactor', 'meName_scaleFactor' or"
      << " 'meName_numerator' and 'meName_denominator' !!";
    cfgError_ = 1;
  } 
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistScaler::DQMHistScaler(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistScaler::DQMHistScaler>:" << std::endl;

  if ( cfg.exists("config") ) {
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgScaleJobs = cfg.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgScaleJob = cfgScaleJobs.begin();
	  cfgScaleJob != cfgScaleJobs.end(); ++cfgScaleJob ) {
      cfgEntryScaleJobs_.push_back(cfgEntryScaleJob(*cfgScaleJob));
    }
  } else {
    cfgEntryScaleJobs_.push_back(cfgEntryScaleJob(cfg));
  }
  
  dqmDirectories_drop_ = cfg.exists("drop") ? cfg.getParameter<vstring>("drop") : vstring();
  
  //std::cout << "done." << std::endl;
}

DQMHistScaler::~DQMHistScaler() 
{
// nothing to be done yet...
}

void DQMHistScaler::analyze(const edm::Event&, const edm::EventSetup&)
{
// nothing to be done yet...
}

double getMonitorElementNorm(DQMStore& dqmStore, 
			     const std::string& meName_full, const std::string& meType, 
			     double& normErr, int& errorFlag)
{
  //std::cout << "<getMonitorElementNorm>:" << std::endl;
  //std::cout << " meName_full = " << meName_full << std::endl;

  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_full, meName, dqmDirectory);

  dqmStore.setCurrentFolder(dqmDirectory);
  MonitorElement* me = dqmStore.get(meName_full);

  if ( !me ) {						
    edm::LogError("getMonitorElementNorm") 
      << " Failed to retrieve Monitor Element = " << meName << " from dqmDirectory = " << dqmDirectory << " !!";
    errorFlag = 1;
    return -1.;
  }

  if ( meType == "real" ) {
    normErr = 0.;
    return me->getFloatValue();
  } else if ( meType == "int" ) {
    normErr = 0.;
    return me->getIntValue();
  } else if ( meType == "hist" ) {
    if ( me->getTH1() ) {
      TH1* histogram = me->getTH1();

      //return histogram->IntegralAndError(xAxis->GetFirst(), xAxis->GetLast(), normErr);

      double integral = 0.;
      double integralErr2 = 0.;

      int numBinsX = histogram->GetXaxis()->GetNbins();
      for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	int numBinsY = histogram->GetYaxis()->GetNbins();
	for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  int numBinsZ = histogram->GetZaxis()->GetNbins();
	  for ( int iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	    double binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	    double binError = histogram->GetBinError(iBinX, iBinY, iBinZ);

	    integral += binContent;
	    integralErr2 += (binError*binError);
	  }
	}
      }

      normErr = TMath::Sqrt(integralErr2);

      return integral;
    } else {
      edm::LogError("getMonitorElementNorm") << " No histogram associated to Monitor Element = " << meName << " !!";
      errorFlag = 1;
      return -1.;
    } 
  } else {
    edm::LogError("getMonitorElementNorm") << " Undefined type = " << meType << " !!";
    errorFlag = 1;
    return -1.;
  }
}

double getRatio(DQMStore& dqmStore, const std::string& dqmDirectory,
		const std::string& meName_numerator, const std::string& meName_numeratorErr,
		const std::string& meName_denominator, const std::string& meName_denominatorErr,
		const std::string& meType,
		double& ratioErr, int& errorFlag)
{
  //std::cout << "<getRatio>:" << std::endl;
  //std::cout << " meName_numerator = " << meName_numerator << std::endl;
  //std::cout << " meName_numeratorErr = " << meName_numeratorErr << std::endl;
  //std::cout << " meName_denominator = " << meName_denominator << std::endl;
  //std::cout << " meName_denominatorErr = " << meName_denominatorErr << std::endl;
    
  double dummy;

  double numerator, numeratorErr;
  std::string meName_numerator_full = dqmDirectoryName(dqmDirectory).append(meName_numerator);
  numerator = getMonitorElementNorm(dqmStore, meName_numerator_full, meType, numeratorErr, errorFlag);
  if ( meName_numeratorErr != "" ) {
    std::string meName_numeratorErr_full = dqmDirectoryName(dqmDirectory).append(meName_numeratorErr);
    numeratorErr = getMonitorElementNorm(dqmStore, meName_numeratorErr_full, meType, dummy, errorFlag);
  }
  //std::cout << " numerator = " << numerator << " +/- " << numeratorErr << std::endl;

  double denominator, denominatorErr;
  std::string meName_denominator_full = dqmDirectoryName(dqmDirectory).append(meName_denominator);
  denominator = getMonitorElementNorm(dqmStore, meName_denominator_full, meType, denominatorErr, errorFlag);
  if ( meName_denominatorErr != "" ) {
    std::string meName_denominatorErr_full = dqmDirectoryName(dqmDirectory).append(meName_denominatorErr);
    denominatorErr = getMonitorElementNorm(dqmStore, meName_denominatorErr_full, meType, dummy, errorFlag);
  }
  //std::cout << " denominator = " << denominator << " +/- " << denominatorErr << std::endl;

  double ratio = ( denominator > 0. ) ? numerator/denominator : 0.;
  double relErr2 = 0.;
  if ( numerator   > 0. ) relErr2 += TMath::Power(numeratorErr/numerator, 2);
  if ( denominator > 0. ) relErr2 += TMath::Power(denominatorErr/denominator, 2);
  ratioErr = ratio*TMath::Sqrt(relErr2);
  //std::cout << " ratio = " << ratio << " +/- " << ratioErr << std::endl;

  return ratio;
}

std::string dqmDirectoryName_full(const std::string& dqmDirectory, const std::string& dqmSubDirectory)
{
  std::string dqmDirectoryName_full = dqmDirectoryName(dqmDirectory).append(dqmSubDirectory);
  return dqmDirectoryName_full;
}

void DQMHistScaler::endJob()
{
  std::cout << "<DQMHistScaler::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> histograms will NOT be scaled !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
//--- compute scale factor
  for ( std::vector<cfgEntryScaleJob>::const_iterator cfgScaleJob = cfgEntryScaleJobs_.begin();
	cfgScaleJob != cfgEntryScaleJobs_.end(); ++cfgScaleJob ) {

//--- check that configuration parameters contain no errors
    if ( cfgScaleJob->cfgError_ ) {
      edm::LogError ("endJob") << " Error in Configuration ParameterSet --> histograms will NOT be scaled !!";
      continue;
    }

//--- compute scale factor
    double scaleFactor = 0.;
    double scaleFactorErr = 0.;
    if ( cfgScaleJob->scaleFactor_ != -1. ) {
      scaleFactor = cfgScaleJob->scaleFactor_;
      scaleFactorErr = cfgScaleJob->scaleFactorErr_;

      std::cout << " scaleFactor = " << scaleFactor << " +/- " << scaleFactorErr << std::endl;
    
	} else if ( cfgScaleJob->meName_scaleFactor_ != "" ) { 
      int errorFlag = 0;
      scaleFactor = getMonitorElementNorm(dqmStore, cfgScaleJob->meName_scaleFactor_, cfgScaleJob->meType_, scaleFactorErr, errorFlag);
      if ( cfgScaleJob->meName_scaleFactorErr_ != "" ) {
	double dummy;
	scaleFactorErr = getMonitorElementNorm(dqmStore, cfgScaleJob->meName_scaleFactorErr_, cfgScaleJob->meType_, dummy, errorFlag);
      }
      
      if ( errorFlag ) {
	edm::LogError ("endJob") 
	  << " Failed to access scale factor Monitor Element --> histograms will NOT be scaled !!";
	return;
      }
    } else {
      int errorFlag = 0;
      
      const std::string& meName_numerator = cfgScaleJob->meName_numerator_;
      const std::string& meName_numeratorErr = cfgScaleJob->meName_numeratorErr_;
      const std::string& meName_denominator = cfgScaleJob->meName_denominator_;
      const std::string& meName_denominatorErr = cfgScaleJob->meName_denominatorErr_;
      const std::string& meType = cfgScaleJob->meType_;

      if ( cfgScaleJob->dqmDirectory_factorizedLooseSel_ != "" &&
	   cfgScaleJob->dqmDirectory_factorizedTightSel_ != "" ) {
	double efficiencyLooseSel, efficiencyErrLooseSel;
	efficiencyLooseSel = getRatio(dqmStore, cfgScaleJob->dqmDirectory_factorizedLooseSel_,
				      meName_numerator, meName_numeratorErr, meName_denominator, meName_denominatorErr, meType,
				      efficiencyErrLooseSel, errorFlag);
	
	double efficiencyTightSel, efficiencyErrTightSel;
	efficiencyTightSel = getRatio(dqmStore, cfgScaleJob->dqmDirectory_factorizedTightSel_,
				      meName_numerator, meName_numeratorErr, meName_denominator, meName_denominatorErr, meType,
				      efficiencyErrTightSel, errorFlag);
	
	scaleFactor = ( efficiencyLooseSel > 0. ) ? efficiencyTightSel/efficiencyLooseSel : 0.;
	double relErr2 = 0.;
	if ( efficiencyLooseSel > 0. ) relErr2 += TMath::Power(efficiencyErrLooseSel/efficiencyLooseSel, 2);
	if ( efficiencyTightSel > 0. ) relErr2 += TMath::Power(efficiencyErrTightSel/efficiencyTightSel, 2);
	scaleFactorErr = scaleFactor*TMath::Sqrt(relErr2);
      } else {
	scaleFactor = getRatio(dqmStore, "",
			       meName_numerator, meName_numeratorErr, meName_denominator, meName_denominatorErr, meType,
			       scaleFactorErr, errorFlag);
      }

      std::cout << " scaleFactor = " << scaleFactor << " +/- " << scaleFactorErr << std::endl;

      if ( errorFlag ) {
	edm::LogError ("endJob") 
	  << " Failed to access numerator and denominator Monitor Elements --> histograms will NOT be scaled !!";
	return;
      }
    }

//--- scale histograms;
//    in case specific subdirectories have been specified in configuration parameters, 
//    copy the specified subdirectories only, else copy all subdirectories 
//    from dqmDirectory_input to dqmDirectory_output
    //std::cout << "--> scaling histograms..." << std::endl;
    if ( cfgScaleJob->meName_input_ != "" && cfgScaleJob->meName_output_ != "" ) {
      std::string meName_input, dqmDirectory_input;
      separateMonitorElementFromDirectoryName(cfgScaleJob->meName_input_, meName_input, dqmDirectory_input);

      std::string meName_output, dqmDirectory_output;
      separateMonitorElementFromDirectoryName(cfgScaleJob->meName_output_, meName_output, dqmDirectory_output);

      dqmCopyMonitorElement(dqmStore, dqmDirectory_input, meName_input, dqmDirectory_output, meName_output, 
			    scaleFactor, scaleFactorErr);

      if ( cfgScaleJob->meName_inputErr_ != "" && cfgScaleJob->meName_outputErr_ != "" ) {
	std::string meName_inputErr, dqmDirectory_inputErr;
	separateMonitorElementFromDirectoryName(cfgScaleJob->meName_inputErr_, meName_inputErr, dqmDirectory_inputErr);

	std::string meName_outputErr, dqmDirectory_outputErr;
	separateMonitorElementFromDirectoryName(cfgScaleJob->meName_outputErr_, meName_outputErr, dqmDirectory_outputErr);

	dqmCopyMonitorElement(dqmStore, dqmDirectory_inputErr, meName_inputErr, dqmDirectory_outputErr, meName_outputErr, 
			      scaleFactor, scaleFactorErr);

	MonitorElement* meInput = dqmStore.get(cfgScaleJob->meName_input_);
	MonitorElement* meInputErr = dqmStore.get(cfgScaleJob->meName_inputErr_);
	
	//std::cout << " meInput = " << meInput << std::endl;
	//std::cout << " meInputErr = " << meInputErr << std::endl;

	MonitorElement* meOutput = dqmStore.get(cfgScaleJob->meName_output_);
	MonitorElement* meOutputErr = dqmStore.get(cfgScaleJob->meName_outputErr_);
	
	//std::cout << " meOutput = " << meOutput << std::endl;
	//std::cout << " meOutputErr = " << meOutputErr << std::endl;

	if ( meInput->kind() == MonitorElement::DQM_KIND_REAL && meInputErr->kind() == MonitorElement::DQM_KIND_REAL ) {
	  double relErr2 = 0.;
	  if ( meInput->getFloatValue() != 0. ) relErr2 += TMath::Power(meInputErr->getFloatValue()/meInput->getFloatValue(), 2);
	  if ( scaleFactor              != 0. ) relErr2 += TMath::Power(scaleFactorErr/scaleFactor, 2);

	  double output = meOutput->getFloatValue();
	  double outputErr = output*TMath::Sqrt(relErr2);

	  //std::cout << " outputErr = " << outputErr << std::endl;

	  meOutputErr->Reset();	  
	  meOutputErr->Fill(outputErr);
	} else {
	  edm::LogError ("endJob") 
	    << " Handling of errors implemented for Monitor Elements of type DQM_KIND_REAL only !!";
	}
      }
    } else if ( cfgScaleJob->dqmSubDirectories_input_.size() > 0 ) {
      for ( vstring::const_iterator dqmSubDirectory = cfgScaleJob->dqmSubDirectories_input_.begin();
	    dqmSubDirectory != cfgScaleJob->dqmSubDirectories_input_.end(); ++dqmSubDirectory ) {

	std::string inputDirectory = dqmDirectoryName_full(cfgScaleJob->dqmDirectory_input_, *dqmSubDirectory);
	std::string outputDirectory = dqmDirectoryName_full(cfgScaleJob->dqmDirectory_output_, *dqmSubDirectory);
	
	std::cout << " scaling MonitorElements in input Directory = " << inputDirectory << ","
		  << " copying scaled MonitorElements to output Directory = " << outputDirectory << std::endl;
	
	dqmCopyRecursively(dqmStore, inputDirectory, outputDirectory, scaleFactor, scaleFactorErr, 1, false);
      }
    } else {
      std::string inputDirectory = dqmDirectoryName(cfgScaleJob->dqmDirectory_input_);
      std::string outputDirectory = dqmDirectoryName(cfgScaleJob->dqmDirectory_output_);
      
      std::cout << " scaling MonitorElements in input Directory = " << inputDirectory << ","
		<< " copying scaled MonitorElements to output Directory = " << outputDirectory << std::endl;
      
      dqmCopyRecursively(dqmStore, inputDirectory, outputDirectory, scaleFactor, scaleFactorErr, 1, false);
    }
  }

//--- delete all MonitorElements in input directories
//    specified to be dropped after applying scale factors and copying to output directories
  for ( vstring::const_iterator dqmDirectory_drop = dqmDirectories_drop_.begin();
	dqmDirectory_drop != dqmDirectories_drop_.end(); ++dqmDirectory_drop ) {
    
    std::cout << " dropping all MonitorElements in directory = " << (*dqmDirectory_drop) << std::endl;

    dqmStore.rmdir(*dqmDirectory_drop);
  }

  std::cout << "done." << std::endl; 
  if ( verbosity ) dqmStore.showDirStructure();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistScaler);
