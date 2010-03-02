#include "TauAnalysis/FittingTools/plugins/TemplateFitAdapter_TFractionFitter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <TCanvas.h>
#include <TMath.h>
#include <TH1.h>

#include <iostream>
#include <fstream>

const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const int fitStatus_converged_TFractionFitter = 0;

typedef std::pair<double, double> double_pair;

TemplateFitAdapter_TFractionFitter::modelNdTypeTFractionFitterSpecific::modelNdTypeTFractionFitterSpecific(const std::string& processName)
  : processName_(processName),
    auxConcatenatedHistogram_(0)
{}

TemplateFitAdapter_TFractionFitter::modelNdTypeTFractionFitterSpecific::~modelNdTypeTFractionFitterSpecific()
{
  delete auxConcatenatedHistogram_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapter_TFractionFitter::dataNdTypeTFractionFitterSpecific::dataNdTypeTFractionFitterSpecific()
  : auxConcatenatedHistogram_(0)
{}

TemplateFitAdapter_TFractionFitter::dataNdTypeTFractionFitterSpecific::~dataNdTypeTFractionFitterSpecific()
{
  delete auxConcatenatedHistogram_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapter_TFractionFitter::TemplateFitAdapter_TFractionFitter(const edm::ParameterSet& cfg)
  : TemplateFitAdapterBase(cfg),
    fitAlgorithmImp_(0)
{
  std::cout << "<TemplateFitAdapter_TFractionFitter::TemplateFitAdapter_TFractionFitter>:" << std::endl;

  dataNdEntryImpSpecific_ = new dataNdTypeTFractionFitterSpecific();

//--- read configuration parameters specifying signal and background processes
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin(); 
	processName != processNames.end(); ++processName ) {
    modelNdTypeTFractionFitterSpecific* modelNdEntryImpSpecific = new modelNdTypeTFractionFitterSpecific(*processName);
    modelNdEntriesImpSpecific_[*processName] = modelNdEntryImpSpecific;
  }
}

TemplateFitAdapter_TFractionFitter::~TemplateFitAdapter_TFractionFitter()
{
  std::cout << "<TemplateFitAdapter_TFractionFitter::~TemplateFitAdapter_TFractionFitter>:" << std::endl;

  // CV: do not delete fitAlgorithmImp 
  //    - doing so causes a segmentation violation ?!
  //delete fitAlgorithmImp_;
  
  delete dataNdEntryImpSpecific_;

  for ( modelNdEntryMapImpSpecific::iterator it = modelNdEntriesImpSpecific_.begin();
	it != modelNdEntriesImpSpecific_.end(); ++it ) {
    delete it->second;
  }
}

void TemplateFitAdapter_TFractionFitter::fitImp(int, int) 
{
//-------------------------------------------------------------------------------
// fit template shapes of different signal and background processes
// to distribution observed in (pseudo)data using TFractionFitter algorithm
//-------------------------------------------------------------------------------

//--- build single concatenated histogram of all distribution observed in (pseudo)data
  std::vector<const TH1*> data1dHistograms;
  std::vector<double> dataNormCorrFactors;
  
  unsigned numVariables = varNames_.size();
  for ( unsigned iVariable = 0; iVariable < numVariables; ++iVariable ) {
    const std::string& varName = varNames_[iVariable];
    
    const TemplateFitAdapterBase::data1dType* data1dEntryBase = dataNdEntry_->data1dEntries_[varName];

    data1dHistograms.push_back(data1dEntryBase->fluctHistogram_);
    double normCorrFactor = ( data1dEntryBase->fittedFraction_ > 0. ) ? (1./data1dEntryBase->fittedFraction_) : 0.;
    dataNormCorrFactors.push_back(normCorrFactor);
  }

  delete dataNdEntryImpSpecific_->auxConcatenatedHistogram_;
  dataNdEntryImpSpecific_->auxConcatenatedHistogram_ 
    = makeConcatenatedHistogram("auxConcatenatedHistogram_data", data1dHistograms, &dataNormCorrFactors);
  std::cout << "integral(auxConcatenatedHistogram_data) = " 
	    << dataNdEntryImpSpecific_->auxConcatenatedHistogram_->Integral() << std::endl;

//--- build concatenated histogram of all template shapes 
//    for different signal and background processes
  unsigned numProcesses = processNames_.size();

  TObjArray auxConcatenatedTemplates(numProcesses);

  for ( unsigned iProcess = 0; iProcess < numProcesses; ++iProcess ) {
    const std::string& processName = processNames_[iProcess];
    
    std::vector<const TH1*> model1dHistograms;
    std::vector<double> modelNormCorrFactors;

    unsigned numVariables = varNames_.size();
    for ( unsigned iVariable = 0; iVariable < numVariables; ++iVariable ) {
      const std::string& varName = varNames_[iVariable];

      const TemplateFitAdapterBase::model1dType* model1dEntryBase = modelNdEntries_[processName]->model1dEntries_[varName];

      model1dHistograms.push_back(model1dEntryBase->fluctHistogram_);
      double normCorrFactor = ( model1dEntryBase->fittedFraction_ > 0. ) ? (1./model1dEntryBase->fittedFraction_) : 0.;
      modelNormCorrFactors.push_back(normCorrFactor);
    }

    delete modelNdEntriesImpSpecific_[processName]->auxConcatenatedHistogram_;
    std::string auxConcatenatedHistogramName_model = std::string("auxConcatenatedHistogram_").append(processName);
    modelNdEntriesImpSpecific_[processName]->auxConcatenatedHistogram_
      = makeConcatenatedHistogram(auxConcatenatedHistogramName_model, model1dHistograms, &modelNormCorrFactors);
    std::cout << "integral(auxConcatenatedHistogram_model, process = " << processName << ")" 
	      << " = " << modelNdEntriesImpSpecific_[processName]->auxConcatenatedHistogram_->Integral() << std::endl;
    
    auxConcatenatedTemplates.AddAt(modelNdEntriesImpSpecific_[processName]->auxConcatenatedHistogram_, iProcess);
  }
  
  delete fitAlgorithmImp_;
  fitAlgorithmImp_ = new TFractionFitter(dataNdEntryImpSpecific_->auxConcatenatedHistogram_, &auxConcatenatedTemplates);
  
//--- constrain coefficients to "physical" range 0..100%
  for ( unsigned iProcess = 0; iProcess < numProcesses; ++iProcess ) {
    fitAlgorithmImp_->Constrain(iProcess, 0., 1.);
  }

//--- perform fit
  int fitStatus = fitAlgorithmImp_->Fit(); 
  fitResult_->status_ = ( fitStatus == fitStatus_converged_TFractionFitter ) ? 0 : 1;
}

void TemplateFitAdapter_TFractionFitter::unpackFitResultsImpSpecific()
{  
//--- save normalization factors determined by TFractionFitter

  TVirtualFitter* fitResultImpSpecific = fitAlgorithmImp_->GetFitter();
  
  unsigned numProcesses = processNames_.size();
  fitResult_->normValues_.ResizeTo(numProcesses);
  fitResult_->normCov_.ResizeTo(numProcesses, numProcesses);
  for ( unsigned iProcess = 0; iProcess < numProcesses; ++iProcess ) {
    const std::string& processName = processNames_[iProcess];

    unsigned numVariables = varNames_.size();
    double numEvents_data = getIntegral(dataNdEntryImpSpecific_->auxConcatenatedHistogram_)/numVariables;
    
    double normValue = fitResultImpSpecific->GetParameter(iProcess)*numEvents_data;
    double normError = fitResultImpSpecific->GetParError(iProcess)*numEvents_data;

    TemplateFitAdapterBase::fitResultType::normEntryType& norm = fitResult_->normalizations_[processName];
    norm.value_ = normValue;
    norm.errUp_ = norm.errDown_ = normError;

    fitResult_->normValues_(iProcess) = fitResultImpSpecific->GetParameter(iProcess)*numEvents_data;

    for ( unsigned jProcess = 0; jProcess < numProcesses; ++jProcess ) {
      double fitCovarianceXY = fitResultImpSpecific->GetCovarianceMatrixElement(iProcess, jProcess)*numEvents_data*numEvents_data;
      fitResult_->normCov_(iProcess, jProcess) = ( fitCovarianceXY > 0. ) ? TMath::Sqrt(fitCovarianceXY) : 0.;
    }
  }
}

void TemplateFitAdapter_TFractionFitter::makeControlPlotsImpSpecific() 
{
//--- produce technical control plot of template variations 
//    in case TFractionFitter algorithm is used to perform the actual fitting

  std::cout << "<TemplateFitAdapter_TFractionFitter::makeControlPlotsImpSpecific>:" << std::endl;

//--- check fit status;
//    "template prediction" histogram obtained from TFractionFitter::GetPlot method
//    only exists in case fit converted (status = 0)
  if ( fitResult_->status_ != 0 ) {
    edm::LogWarning("makeControlPlotsImpSpecific") 
      << " Fit failed to converge --> skipping !!";
    return;
  }

  TCanvas canvas("TemplateFitAdapter_TFractionFitter", "TemplateFitAdapter_TFractionFitter", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);
  canvas.SetFrameFillColor(10);

  TH1* histo_smSum = (TH1*)fitAlgorithmImp_->GetPlot();
  //std::cout << " histo_smSum = " << histo_smSum << ": integral = " << histo_smSum->Integral() << std::endl;  
  histo_smSum->SetLineWidth(2);
  histo_smSum->SetLineColor(2);
  histo_smSum->Draw("hist");

  TH1* histo_Data = dataNdEntryImpSpecific_->auxConcatenatedHistogram_;
  //std::cout << " histo_Data = " << histo_Data << ": integral = " << histo_Data->Integral() << std::endl; 
  histo_Data->SetLineWidth(2);
  histo_Data->SetMarkerStyle(20);
  histo_Data->Draw("epsame");

  canvas.Update();

  int errorFlag = 0;
  std::string fileNameParam = std::string("modConcatenatedHistogram");
  std::string fileName = replace_string(controlPlotsFileName_, plotKeyword, fileNameParam, 1, 1, errorFlag);
  if ( !errorFlag ) {
    canvas.Print(fileName.data());
  } else {
    edm::LogError("makeControlPlotsImpSpecific") 
      << " Failed to decode controlPlotsFileName = " << controlPlotsFileName_ << " --> skipping !!";
    return;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(TemplateHistFitAdapterPluginFactory, TemplateFitAdapter_TFractionFitter, "TemplateFitAdapter_TFractionFitter");

