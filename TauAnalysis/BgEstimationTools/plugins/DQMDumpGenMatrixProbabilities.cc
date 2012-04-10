#include "TauAnalysis/BgEstimationTools/plugins/DQMDumpGenMatrixProbabilities.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/Core/interface/binningAuxFunctions.h"

#include <TMath.h>

DQMDumpGenMatrixProbabilities::DQMDumpGenMatrixProbabilities(const edm::ParameterSet& cfg)
  : binningService_(0), 
    cfgError_(0)
{
  //std::cout << "<DQMDumpGenMatrixProbabilities::DQMDumpGenMatrixProbabilities>:" << std::endl;

  edm::ParameterSet cfgBinningService = cfg.getParameter<edm::ParameterSet>("binningService");

  edm::ParameterSet dqmDirectoryEntries = cfgBinningService.getParameter<edm::ParameterSet>("dqmDirectories");
  vstring dqmDirectoryEntryNames = dqmDirectoryEntries.getParameterNamesForType<std::string>();
  for ( vstring::const_iterator dqmDirectoryEntryName = dqmDirectoryEntryNames.begin(); 
	dqmDirectoryEntryName != dqmDirectoryEntryNames.end(); ++dqmDirectoryEntryName ) {
    std::string dqmDirectoryEntry = dqmDirectoryEntries.getParameter<std::string>(*dqmDirectoryEntryName);
    
    processes_.push_back(*dqmDirectoryEntryName);
    
    dqmDirectories_[*dqmDirectoryEntryName] = dqmDirectoryEntry;
  }
  
  if ( processes_.size() == 0 ) {
    edm::LogError("DQMDumpGenMatrixProbabilities") << " Configuration Parameter dqmDirectories contains no Entries --> skipping !!";
    cfgError_ = 1;
  }
  
  std::string binningService_pluginType = cfgBinningService.getParameter<std::string>("pluginType");
  binningService_ = BinningServicePluginFactory::get()->create(binningService_pluginType, cfgBinningService);
}

DQMDumpGenMatrixProbabilities::~DQMDumpGenMatrixProbabilities() 
{
  for ( std::map<std::string, BinningBase*>::iterator it = binningResults_.begin();
	it != binningResults_.end(); ++it ) {
    delete it->second;
  }

  delete binningService_;
}

void DQMDumpGenMatrixProbabilities::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void printGenMatrixProbabilities(std::ostream& stream, BinningBase* binningResult)
{
  const BinGrid* binGrid = binningResult->binGrid();

//--- check that exactly two bins are defined in each dimension,
//    as expected for binning used by generalized matrix method
  if ( (int)binGrid->numBins() != TMath::Nint(TMath::Power(2., (double)binGrid->dimensions())) ) {
    edm::LogError ("printGenMatrixProbabilities") << "Invalid binning for use by generalized matrix method !!";
    std::cout << "(numBins = " << binGrid->numBins() << ", numDimensions = " << binGrid->dimensions() << ")" << std::endl;
    return;
  }

  const std::vector<std::string>& objVarNames = binGrid->objVarNames();

  unsigned numDimensions = binGrid->dimensions();
  for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
    std::vector<double> binCenter0 = binGrid->binCenter(0);
    std::vector<double> binCenter1 = binGrid->binCenter(TMath::Nint(TMath::Power(2., (double)iDimension)));

    stream << objVarNames[iDimension] << ": " << binCenter0[iDimension] << " --> " << binCenter1[iDimension] << std::endl;

    unsigned numBins = binGrid->numBins();
    for ( unsigned iBin_ref = 0; iBin_ref < numBins; ++iBin_ref ) {

      if ( ((int)iBin_ref & TMath::Nint(TMath::Power(2., (double)iDimension))) != 0 ) continue;

      std::vector<binResultType> binResult_ref = binningResult->getBinResults(iBin_ref);
      double binContent_ref = getBinContent(binResult_ref, "rec");
      double binErr2_ref = getBinSumw2(binResult_ref, "rec");

      unsigned iBin_flip = iBin_ref + TMath::Nint(TMath::Power(2., (double)iDimension));

      std::vector<binResultType> binResult_flip = binningResult->getBinResults(iBin_flip);
      double binContent_flip = getBinContent(binResult_flip, "rec");
      double binErr2_flip = getBinSumw2(binResult_flip, "rec");

      double p_flip, pErr_flip;
      if ( (binContent_ref + binContent_flip) != 0 ) {
	p_flip = binContent_flip/(binContent_ref + binContent_flip);
	
	double err2Sum = 0;
	if ( binContent_ref  != 0 ) err2Sum += binErr2_ref/(binContent_ref*binContent_ref);
	if ( binContent_flip != 0 ) err2Sum += binErr2_flip/(binContent_flip*binContent_flip);

	pErr_flip = p_flip*TMath::Sqrt(err2Sum);
      } else {
	p_flip = 0.5;
	pErr_flip = 0.5;
      }

      std::vector<double> binCenter_ref = binGrid->binCenter(iBin_ref);

      printBinCenterPosition(stream, binGrid, iBin_ref, false);
      stream << " p = " << p_flip << " +/- " << pErr_flip << std::endl;
    }
  }
}

BinningBase* DQMDumpGenMatrixProbabilities::loadBinningResults(const std::string& dqmDirectory)
{
  std::cout << "<DQMDumpGenMatrixProbabilities::loadBinningResults>:" << std::endl;
  std::cout << " dqmDirectory = " << dqmDirectory << std::endl;

//--- check that BinningService is available
  if ( !binningService_ ) {
    edm::LogError ("loadBinning") << " BinningService not initialized --> binning results will NOT be loaded !!";
    return 0;
  }

  return binningService_->loadBinningResults(dqmDirectory);
}

void DQMDumpGenMatrixProbabilities::endJob()
{
  //std::cout << "<DQMDumpGenMatrixProbabilities::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endjob") << " Error in Configuration ParameterSet --> binning results will NOT be printed-out !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> binning results will NOT be printed-out !!";
    return;
  }

//--- load objects inheriting from BinningBase class from DQM directories
  for ( vstring::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    const std::string& dqmDirectory = dqmDirectories_[*process];
    std::cout << "retrieving Binning results for process = " << (*process) 
	      << " from dqmDirectory = " << dqmDirectory << "..." << std::endl;

    BinningBase* binningResult = loadBinningResults(dqmDirectory); 

    if ( binningResult ) {
      binningResults_[*process] = binningResult;
    } else {
      edm::LogError ("DQMDumpGenMatrixProbabilities") << " Failed to load Binning result from dqmDirectory = " << dqmDirectory
					      << " --> Binning results will NOT be printed-out !!";
      return;
    }
  }

//--- print objects inheriting from BinningBase class 
  for ( std::vector<std::string>::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    BinningBase* binningResult = binningResults_[*process];
    std::cout << "Probabilities for process = " << (*process) << ":" << std::endl;
    std::cout << "(generalized Matrix Method requires that all probabilities are about the same)" << std::endl;
    printGenMatrixProbabilities(std::cout, binningResult);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMDumpGenMatrixProbabilities);
