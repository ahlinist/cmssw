#include "TauAnalysis/BgEstimationTools/plugins/SPlotAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <RooAddPdf.h>
#include <RooFit.h>

#include <TAxis.h>
#include <TLegend.h>

SPlotAnalyzer::dataEntryType::dataEntryType(const edm::ParameterSet& cfg, 
					    const std::string& tagNameHistograms)
  : processName_("data"),
    fitHistogram_(0),
    fitDataHist_(0),
    sPlotWeightVariable_(0),
    sPlotNtuple_(0),
    sPlotDataSet_(0),
    error_(0)
{
  edm::ParameterSet cfgHistograms = cfg.getParameter<edm::ParameterSet>(tagNameHistograms);
  vstring histogramNames = cfgHistograms.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator histogramName = histogramNames.begin();
	histogramName != histogramNames.end(); ++histogramName ) {
    edm::ParameterSet cfgHistogram = cfgHistograms.getParameter<edm::ParameterSet>(*histogramName);
    
    std::string meName = cfgHistogram.getParameter<std::string>("meName");
    meNames_.push_back(meName);
  }

  sPlotNtupleFileNames_ = cfg.getParameter<vstring>("fileNames");
  sPlotTreeName_ = cfg.getParameter<std::string>("treeName");

  sPlotWeightVariableName_ = cfg.exists("branchNameEventWeight") ? 
    cfg.getParameter<std::string>("branchNameEventWeight") : "";
}

SPlotAnalyzer::dataEntryType::~dataEntryType()
{
  //delete fitHistogram_;
  delete fitDataHist_;
  delete sPlotWeightVariable_;
  delete sPlotNtuple_;
  delete sPlotDataSet_;
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

void SPlotAnalyzer::dataEntryType::getHistogram()
{
  std::vector<const TH1*> histograms = getHistograms(meNames_, &error_);
  
  if ( histograms.size() == 1 ) {
    fitHistogram_ = histograms[0];
  } else {
    edm::LogError ("buildDataHist")
      << " Number of histograms = " << histograms.size() << ", exactly one histogram required !!";
    error_ = 1;
  }
}

void SPlotAnalyzer::dataEntryType::buildDataHist(const RooArgSet& fitVariables)
{
  std::string fitDataHistName = std::string(processName_).append("_").append("fitDataHist");
  fitDataHist_ = new RooDataHist(fitDataHistName.data(), fitDataHistName.data(), fitVariables, fitHistogram_);
}

void SPlotAnalyzer::dataEntryType::buildSPlotDataSet(const RooArgSet& fitVariables, const RooArgSet& sPlotVariables)
{
  sPlotNtuple_ = new TChain(sPlotTreeName_.data());
  for ( vstring::const_iterator sPlotNtupleFileName = sPlotNtupleFileNames_.begin();
	sPlotNtupleFileName != sPlotNtupleFileNames_.end(); ++sPlotNtupleFileName ) {
    sPlotNtuple_->Add(sPlotNtupleFileName->data());
  }
  
  RooArgSet allVariables;
  allVariables.add(fitVariables);
  allVariables.add(sPlotVariables);

  if ( sPlotWeightVariableName_ != "" ) {
    sPlotWeightVariable_ = new RooRealVar(sPlotWeightVariableName_.data(), sPlotWeightVariableName_.data(), 1.);
    allVariables.add(*sPlotWeightVariable_);
  }

  std::string sPlotDataSetName = std::string(processName_).append("_").append("sPlotDataSet");
  sPlotDataSet_ = new RooDataSet(sPlotDataSetName.data(), sPlotDataSetName.data(), 
				 sPlotNtuple_, allVariables, 0, sPlotWeightVariableName_.data());
}

//
//-------------------------------------------------------------------------------
//

SPlotAnalyzer::processEntryType::processEntryType(const edm::ParameterSet& cfg, const std::string& processName,
						  const std::string& tagNameHistograms)
  : dataEntryType(cfg, tagNameHistograms),
    modelHistPdf_(0),
    norm_(0)
{
  processName_ = processName;
}

SPlotAnalyzer::processEntryType::~processEntryType()
{
  delete modelHistPdf_;
  delete norm_;
}

void SPlotAnalyzer::processEntryType::buildPdf(const RooArgSet& fitVariables)
{
  dataEntryType::getHistogram();
  dataEntryType::buildDataHist(fitVariables);

  std::string modelHistPdfName = std::string(processName_).append("_").append("modelHistPdf");
  modelHistPdf_ = new RooHistPdf(modelHistPdfName.data(), modelHistPdfName.data(), fitVariables, *fitDataHist_);

  std::string normName = std::string(processName_).append("_").append("norm");
  norm_ = new RooRealVar(normName.data(), normName.data(), 0., 1.e+6);
}

//
//-------------------------------------------------------------------------------
//

SPlotAnalyzer::sPlotEntryType::sPlotEntryType(const edm::ParameterSet& cfg)
  : var_(0)
{
  branchName_ = cfg.getParameter<std::string>("branchName");
  
  title_ = cfg.getParameter<std::string>("title");

  xMin_ = cfg.getParameter<double>("xMin");
  xMax_ = cfg.getParameter<double>("xMax");

  var_ = new RooRealVar(branchName_.data(), title_.data(), xMin_, xMax_);

  selProcesses_ = cfg.getParameter<vstring>("selProcesses");
}

SPlotAnalyzer::sPlotEntryType::~sPlotEntryType()
{
  delete var_;
}

//
//-------------------------------------------------------------------------------
//

SPlotAnalyzer::SPlotAnalyzer(const edm::ParameterSet& cfg)
  : data_(0),
    sPlotAlgorithm_(0),
    canvas_(0),
    ps_(0),
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

    processEntryType* process = new processEntryType(cfgProcess, *processName);
    processes_.push_back(process);
  }

//--- read configuration parameters for dataset used in likelihood fit
  edm::ParameterSet cfgData = cfg.getParameter<edm::ParameterSet>("data");

  data_ = new dataEntryType(cfgData);

//--- read configuration parameters for variables used in likelihood fit
  fitVariableNames_ = cfg.getParameter<vstring>("fitVariables");

//--- read configuration parameters for sPlot object/
//    branch names of control variables
  edm::ParameterSet cfgSPlots = cfg.getParameter<edm::ParameterSet>("sPlots");
  vstring sPlotNames = cfgSPlots.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator sPlotName = sPlotNames.begin();
	sPlotName != sPlotNames.end(); ++sPlotName ) {
    edm::ParameterSet cfgSPlot = cfgSPlots.getParameter<edm::ParameterSet>(*sPlotName);

    sPlotEntryType* sPlot = new sPlotEntryType(cfgSPlot);
    sPlots_.push_back(sPlot);
  }

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
  for ( std::vector<processEntryType*>::iterator it = processes_.begin();
	it != processes_.end(); ++it ) {
    delete (*it);
  }

  delete data_;   

  for ( std::vector<RooRealVar*>::iterator it = fitVariableList_.begin();
	it != fitVariableList_.end(); ++it ) {
    delete (*it);
  }

  //delete sPlotAlgorithm_; // CV: calling RooStats::SPlot destructor causes segmentation violation ?!

  for ( std::vector<sPlotEntryType*>::iterator it = sPlots_.begin();
	it != sPlots_.end(); ++it ) {
    delete (*it);
  }

  delete canvas_;
  delete ps_;
}

void SPlotAnalyzer::endJob()
{
  std::cout << "<SPlotAnalyzer::endJob>:" << std::endl;

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("endJob") 
      << " Error in Configuration ParameterSet --> SPlot will NOT be run !!";
    return;
  }

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") 
      << " Failed to access dqmStore --> SPlot will NOT be run !!";
    dqmError_ = 1;
    return;
  }

//--- initialize variables used in likelihood fit
  data_->getHistogram();

  unsigned numFitVariableNames = fitVariableNames_.size();
  if ( (int)numFitVariableNames == data_->fitHistogram_->GetDimension() ) {
    for ( unsigned iFitVariable = 0; iFitVariable < numFitVariableNames; ++iFitVariable ) {
      const std::string& fitVariableName = fitVariableNames_[iFitVariable];
      
      TAxis* axis = 0;
      switch ( iFitVariable ) {
      case 0:
	axis = data_->fitHistogram_->GetXaxis();
	break;
      case 1:
	axis = data_->fitHistogram_->GetYaxis();
	break;
      case 2:
	axis = data_->fitHistogram_->GetZaxis();
	break;
      default:
	edm::LogError ("endJob") 
	  << " Unsupported dimension = " << iFitVariable << " --> skipping !!";
	cfgError_ = 1;
	continue;
      }

      double fitRangeMin = axis->GetXmin();
      double fitRangeMax = axis->GetXmax();

      RooRealVar* fitVariable = new RooRealVar(fitVariableName.data(), fitVariableName.data(), fitRangeMin, fitRangeMax);
      fitVariableList_.push_back(fitVariable);

      fitVariables_.add(*fitVariable);
    }
  }

//--- determine normalization of different signal/background processes
  buildFitData();

  buildFitModel();

  RooLinkedList fitOptions;
  fitOptions.Add(new RooCmdArg(RooFit::Extended()));
//--- save results of fit for later analysis
  fitOptions.Add(new RooCmdArg(RooFit::Save(true)));
//--- set "precision" of fitted (pseudo)data 
//    to precision which would be obtained with "real" data
//    (unweighted events; works with "real" data as well)
  fitOptions.Add(new RooCmdArg(RooFit::SumW2Error(false)));
  fitModel_->fitTo(*data_->fitDataHist_, fitOptions);

//--- create sPlot weights
  buildSPlotWeights();

//--- check sPlot weights
  for ( std::vector<processEntryType*>::const_iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    std::cout << "process = " << (*process)->processName_ << ":"
	      << " norm = " << (*process)->norm_->getVal() << " +/- " << (*process)->norm_->getError() << "," 
	      << " sum(sPlot weights) = " << sPlotAlgorithm_->GetYieldFromSWeight((*process)->norm_->GetName()) << std::endl;
  }

//--- create plots of control variables
  canvas_ = new TCanvas("canvas", "canvas", 800, 600);
  canvas_->SetFillColor(10);
  canvas_->SetFrameFillColor(10);

  if ( outputFileName_ != "" ) {
    std::string psFileName = ( outputFilePath_ != "" ) ? 
      std::string(outputFilePath_).append("/").append(outputFileName_) : outputFileName_;
    ps_ = new TPostScript(psFileName.data(), 112);
  }

  for ( std::vector<sPlotEntryType*>::const_iterator sPlot = sPlots_.begin();
	sPlot != sPlots_.end(); ++sPlot ) {
    for ( vstring::const_iterator selProcessName = (*sPlot)->selProcesses_.begin();
	  selProcessName != (*sPlot)->selProcesses_.end(); ++selProcessName ) {
      makeControlPlot(*selProcessName, **sPlot);
    }
  }

//--- close postscript file
  canvas_->Clear();
  std::cout << "done." << std::endl;
  if ( ps_ ) ps_->Close();
}

//
//-------------------------------------------------------------------------------
//

void SPlotAnalyzer::buildFitData()
{
  data_->buildDataHist(fitVariables_);
}

void SPlotAnalyzer::buildFitModel()
{
  TObjArray fitModel_pdfCollection;
  TObjArray fitModel_normCollection;
  
  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    (*process)->buildPdf(fitVariables_);
    
    fitModel_pdfCollection.Add((*process)->modelHistPdf_);
    fitModel_normCollection.Add((*process)->norm_);
  }
  
  std::string fitModelName = std::string("fitModel");
  fitModel_ = new RooAddPdf(fitModelName.data(), fitModelName.data(), 
			    RooArgList(fitModel_pdfCollection), RooArgList(fitModel_normCollection));
}

void SPlotAnalyzer::buildSPlotWeights()
{
  std::cout << "<buildSPlotWeights>:" << std::endl;
  
  for ( std::vector<sPlotEntryType*>::iterator sPlot = sPlots_.begin();
	sPlot != sPlots_.end(); ++sPlot ) {
    sPlotVariables_.add(*(*sPlot)->var_);
  }

  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    (*process)->buildSPlotDataSet(fitVariables_, sPlotVariables_);
  }
  
  data_->buildSPlotDataSet(fitVariables_, sPlotVariables_);
  std::cout << "--> isWeighted(sPlotDataSet) = " << data_->sPlotDataSet_->isWeighted() << std::endl;
  
  std::string sPlotName = std::string("sPlot");
  TObjArray fitModel_normCollection;
  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    std::cout << " process = " << (*process)->processName_ << ":" 
	      << " norm = " << (*process)->norm_->getVal() << " +/- " << (*process)->norm_->getError() << std::endl;
    fitModel_normCollection.Add((*process)->norm_);
  }
  std::cout << "creating sPlot algorithm..." << std::endl;
  sPlotAlgorithm_ = new RooStats::SPlot(sPlotName.data(), sPlotName.data(), 
					*data_->sPlotDataSet_, fitModel_, RooArgList(fitModel_normCollection));  
}

void SPlotAnalyzer::makeControlPlot(const std::string& selProcessName, const sPlotEntryType& sPlot)
{
  std::cout << "<makeControlPlot>:" << std::endl;
  std::cout << " process = " << selProcessName << std::endl;
  std::cout << " distribution = " << sPlot.branchName_ << std::endl;

  processEntryType* selProcess = 0;
  for ( std::vector<processEntryType*>::iterator process = processes_.begin();
	process != processes_.end(); ++process ) {
    if ( (*process)->processName_ == selProcessName ) selProcess = (*process);
  }

  if ( !selProcess ) {
    edm::LogError ("makeControlPlot") 
      << " No process with name = " << selProcessName << " defined --> skipping !!";
    return;
  }

  canvas_->Clear();

  RooPlot* frame = sPlot.var_->frame();
  frame->SetTitle(sPlot.title_.data());

  std::string sPlotWeightName = std::string(selProcess->norm_->GetName()).append("_sw"); // follow RooStats::SPlot naming convention
  RooDataSet* sPlotWeightedDataSet = new RooDataSet(data_->sPlotDataSet_->GetName(), data_->sPlotDataSet_->GetTitle(), 
						    data_->sPlotDataSet_, *data_->sPlotDataSet_->get(), 0, sPlotWeightName.data());

  RooPlot* plotTrueDistr = selProcess->sPlotDataSet_->plotOn(frame);
  std::string histTrueDistrName = std::string("h_").append(selProcess->sPlotDataSet_->GetName()); // follow RooPlot naming convention
  RooHist* histTrueDistr = plotTrueDistr->getHist(histTrueDistrName.data());
  assert(histTrueDistr != 0);
  histTrueDistr->SetLineColor(kRed);
  histTrueDistr->SetMarkerColor(kRed);
  histTrueDistr->SetMarkerStyle(24);
  histTrueDistr->SetMarkerSize(0.8);

  RooPlot* plotEstDistr = sPlotWeightedDataSet->plotOn(frame, RooFit::DataError(RooAbsData::SumW2));
  std::string histEstDistrName = std::string("h_").append(data_->sPlotDataSet_->GetName()); // follow RooPlot naming convention
  RooHist* histEstDistr = plotEstDistr->getHist(histEstDistrName.data());
  assert(histEstDistr != 0);
  histEstDistr->SetLineColor(kBlack);
  histEstDistr->SetMarkerColor(kBlack);
  histEstDistr->SetMarkerStyle(20);
  histEstDistr->SetMarkerSize(1.0);

  frame->Draw();

  TLegend legend(0.64, 0.71, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(histEstDistr, "sPlot Estimate", "p");
  legend.AddEntry(histTrueDistr, "MC Truth", "p");
  legend.Draw();

  canvas_->Update();

  if ( indOutputFileName_ != "" ) {
    std::string plotName = std::string(selProcessName).append("_").append(sPlot.branchName_);
    int errorFlag = 0;
    std::string modIndOutputFileName = replace_string(indOutputFileName_, plotKeyword, plotName, 1, 1, errorFlag);
    if ( !errorFlag ) {
      std::string fullFileName = ( outputFilePath_ != "" ) ? 
	std::string(outputFilePath_).append("/").append(modIndOutputFileName) : modIndOutputFileName;
      //std::cout << " fullFileName = " << fullFileName << std::endl;
      canvas_->Print(fullFileName.data());
    } else {
      edm::LogError("makeControlPlot") 
	<< " Failed to decode indOutputFileName = " << indOutputFileName_ << " --> skipping !!";
    }
  }

  if ( ps_ ) ps_->NewPage();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(SPlotAnalyzer);
