#include "TauAnalysis/BgEstimationTools/plugins/SPlotAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <RooAddPdf.h>
#include <RooFit.h>

#include <TAxis.h>

SPlotAnalyzer::processEntryType::processEntryType(const std::string& processName, const vstring& meNames)
  : processName_(processName),
    meNames_(meNames),
    fitVarRef_(0),
    auxConcatenatedHistogram_(0),
    modelDataHist_(0),
    modelHistPdf_(0),
    norm_(0),
    error_(0)
{}

SPlotAnalyzer::processEntryType::~processEntryType()
{
  delete auxConcatenatedHistogram_;
  delete modelDataHist_;
  delete modelHistPdf_;
  delete norm_;
}

std::vector<const TH1*> getHistograms(const std::vector<std::string>& meNames, int* error)
{
  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  std::vector<const TH1*> histograms;
  for ( std::vector<std::string>::const_iterator meName = meNames.begin();
	meName != meNames.end(); ++meName ) {
    MonitorElement* me = dqmStore.get(*meName);

    TH1* histogram = ( me ) ? me->getTH1() : 0;

    if ( !histogram ) {
      edm::LogError ("getHistograms") 
	<< " Failed to access dqmMonitorElement = " << (*meName) << " !!";
      if ( error ) (*error) = 1;
      continue;
    }

    histograms.push_back(histogram);
  }

  return histograms;
}

void SPlotAnalyzer::processEntryType::buildPdf()
{
  std::string auxConcatenatedHistogramName = std::string(processName_).append("_").append("auxConcatenatedHistogram");
  std::vector<const TH1*> histograms = getHistograms(meNames_, &error_);
  auxConcatenatedHistogram_ = makeConcatenatedHistogram(auxConcatenatedHistogramName, histograms);

  std::string modelDataHistName = std::string(processName_).append("_").append("modelDataHist");
  modelDataHist_ = new RooDataHist(modelDataHistName.data(), modelDataHistName.data(), *fitVarRef_, auxConcatenatedHistogram_);

  std::string modelHistPdfName = std::string(processName_).append("_").append("modelHistPdf");
  modelHistPdf_ = new RooHistPdf(modelHistPdfName.data(), modelHistPdfName.data(), *fitVarRef_, *modelDataHist_);

  std::string normName = std::string(processName_).append("_").append("normName");
  norm_ = new RooRealVar(normName.data(), normName.data(), 0., 1.e+6);
}

//
//-------------------------------------------------------------------------------
//

SPlotAnalyzer::SPlotAnalyzer(const edm::ParameterSet& cfg)
  : auxConcatenatedHistogram_(0),
    fitDataHist_(0),
    fitVar_(0),
    sPlotNtuple_(0),
    sPlotDataSet_(0),
    sPlot_(0),
    cfgError_(0),
    dqmError_(0)
{
  std::cout << "<SPlotAnalyzer::SplotAnalyzer>:" << std::endl;

//--- read configuration parameters for probability density function (pdf) model
//    in likelihood fit that determines yields of different signal/background processes
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin();
	processName != processNames.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);

    vstring meNames;

    edm::ParameterSet cfgTemplates = cfgProcess.getParameter<edm::ParameterSet>("templates");
    vstring templateNames = cfgTemplates.getParameterNamesForType<edm::ParameterSet>();
    for ( vstring::const_iterator templateName = templateNames.begin();
	  templateName != templateNames.end(); ++templateName ) {
      edm::ParameterSet cfgTemplate = cfgTemplates.getParameter<edm::ParameterSet>(*templateName);

      std::string meName = cfgTemplate.getParameter<std::string>("meName");
      meNames.push_back(meName);
    }

    processes_.push_back(processEntryType(*processName, meNames));
  }

//--- read configuration parameters for dataset used in likelihood fit
  edm::ParameterSet cfgData = cfg.getParameter<edm::ParameterSet>("data");

  edm::ParameterSet cfgDistributions = cfgData.getParameter<edm::ParameterSet>("distributions");
  vstring distributionNames = cfgDistributions.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator distributionName = distributionNames.begin();
	distributionName != distributionNames.end(); ++distributionName ) {
    edm::ParameterSet cfgDistribution = cfgDistributions.getParameter<edm::ParameterSet>(*distributionName);
    
    std::string meName = cfgDistribution.getParameter<std::string>("meName");
    meNamesFitVariables_.push_back(meName);
  }

//--- read configuration parameters for sPlot object/
//    branch names of control variables
  edm::ParameterSet cfgSPlot = cfg.getParameter<edm::ParameterSet>("sPlot");

  sPlotFileNames_ = cfgSPlot.getParameter<vstring>("fileNames");
  sPlotTreeName_ = cfgSPlot.getParameter<std::string>("treeName");
  controlVariableNames_ = cfgSPlot.getParameter<vstring>("branchNames");

//--- read configuration parameters specifying file names of control plots
  outputFilePath_ = ( cfg.exists("outputFilePath") ) ? cfg.getParameter<std::string>("outputFilePath") : "";
  if ( outputFilePath_.rbegin() != outputFilePath_.rend() ) {
    if ( (*outputFilePath_.rbegin()) == '/' ) outputFilePath_.erase(outputFilePath_.length() - 1);
  }

  outputFileName_ = ( cfg.exists("outputFileName") ) ? cfg.getParameter<std::string>("outputFileName") : "";

  indOutputFileName_ = ( cfg.exists("indOutputFileName") ) ? cfg.getParameter<std::string>("indOutputFileName") : "";
  if ( indOutputFileName_ != "" && indOutputFileName_.find('.') == std::string::npos ) {
    edm::LogError ("SPlotAnalyzer") 
      << " Failed to determine type of graphics format from indOutputFileName = " << indOutputFileName_ << " !!";
    cfgError_ = 1;
  }

//--- check that exactly one type of output is specified for the plots
//    (either separate graphics files displaying one plot each 
//     or postscript file displaying all plots on successive pages;
//     cannot create both types of output simultaneously,
//     as TCanvas::Print seems to interfere with TPostScript::NewPage)
  if ( outputFileName_ == "" && indOutputFileName_ == "" ) {
    edm::LogError ("SPlotAnalyzer") 
      << " Either outputFileName or indOutputFileName must be specified !!";
    cfgError_ = 1;
  }

  if ( outputFileName_ != "" && indOutputFileName_ != "" ) {
    edm::LogError ("SPlotAnalyzer") 
      << " Must not specify outputFileName and indOutputFileName simultaneously !!";
    cfgError_ = 1;
  }
}

SPlotAnalyzer::~SPlotAnalyzer()
{
  delete auxConcatenatedHistogram_;   
  delete fitDataHist_;  
  delete fitVar_;

  delete sPlotNtuple_;
  delete sPlotDataSet_;
  delete sPlot_;
}

void SPlotAnalyzer::beginJob()
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("beginJob") 
      << " Error in Configuration ParameterSet --> SPlot will NOT be run !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("beginJob") 
      << " Failed to access dqmStore --> SPlot will NOT be run !!";
    dqmError_ = 1;
    return;
  }

  buildFitData();

  buildFitModel();

  fitModel_->fitTo(*fitDataHist_, RooFit::Extended());

  buildSPlotWeights();
}

void SPlotAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
// nothing to be done yet...
}

void SPlotAnalyzer::endJob()
{
// nothing to be done yet...
}

//
//-------------------------------------------------------------------------------
//

void SPlotAnalyzer::buildFitData()
{
  std::string auxConcatenatedHistogramName = std::string("data").append("_").append("auxConcatenatedHistogram");
  std::vector<const TH1*> histograms = getHistograms(meNamesFitVariables_, &dqmError_);
  auxConcatenatedHistogram_ = makeConcatenatedHistogram(auxConcatenatedHistogramName, histograms);

  std::string fitVarName = std::string("fitVar");
  TAxis* fitRange = auxConcatenatedHistogram_->GetXaxis();
  fitVar_ = new RooRealVar(fitVarName.data(), fitVarName.data(), fitRange->GetXmin(), fitRange->GetXmax());

  std::string fitDataHistName = std::string("fitDataHist");
  fitDataHist_ = new RooDataHist(fitDataHistName.data(), fitDataHistName.data(), *fitVar_, auxConcatenatedHistogram_);
}

void SPlotAnalyzer::buildFitModel()
{
  TObjArray fitModel_pdfCollection;
  TObjArray fitModel_normCollection;
  
  for ( std::vector<processEntryType>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    process->setAuxVar(fitVar_);
    process->buildPdf();
    
    fitModel_pdfCollection.Add(process->modelHistPdf_);
    fitModel_normCollection.Add(process->norm_);
  }
  
  std::string fitModelName = std::string("fitModel");
  fitModel_ = new RooAddPdf(fitModelName.data(), fitModelName.data(), 
			    RooArgList(fitModel_pdfCollection), RooArgList(fitModel_normCollection));
}

void SPlotAnalyzer::buildSPlotWeights()
{
  sPlotNtuple_ = new TChain(sPlotTreeName_.data());
  for ( vstring::const_iterator sPlotFileName = sPlotFileNames_.begin();
	sPlotFileName != sPlotFileNames_.end(); ++sPlotFileName ) {
    sPlotNtuple_->Add(sPlotFileName->data());
  }
  
  for ( vstring::const_iterator controlVariableName = controlVariableNames_.begin();
	controlVariableName != controlVariableNames_.end(); ++controlVariableName ) {
    RooRealVar controlVariable(controlVariableName->data(), controlVariableName->data(), 0.);
    controlVariables_.addOwned(controlVariable);
  }

  std::string sPlotDataSetName = std::string("sPlotDataSet");
  sPlotDataSet_ = new RooDataSet(sPlotDataSetName.data(), sPlotDataSetName.data(), sPlotNtuple_, controlVariables_);

  std::string sPlotName = std::string("sPlot");
  TObjArray fitModel_normCollection;
  for ( std::vector<processEntryType>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    fitModel_normCollection.Add(process->norm_);
  }
  sPlot_ = new RooStats::SPlot(sPlotName.data(), sPlotName.data(), *sPlotDataSet_, fitModel_, RooArgList(fitModel_normCollection));

  
}

void SPlotAnalyzer::makeControlPlot()
{

}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SPlotAnalyzer);
