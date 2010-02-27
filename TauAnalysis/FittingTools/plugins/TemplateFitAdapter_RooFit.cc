#include "TauAnalysis/FittingTools/plugins/TemplateFitAdapter_RooFit.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <TCanvas.h>
#include <TMath.h>
#include <TH1.h>
#include <TLegend.h>
#include <TF1.h>
#include <TStyle.h>

#include <RooAddPdf.h>
#include <RooGaussian.h>
#include <RooProdPdf.h>
#include <RooArgSet.h>
#include <RooGlobalFunc.h>
#include <RooPlot.h>
#include <RooFit.h>
#include <RooLinkedList.h>
#include <RooCmdArg.h>
#include <RooTFnPdfBinding.h>
#include <RooParametricStepFunction.h>
#include <RooFormulaVar.h>

#include <iostream>
#include <fstream>

const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const double maxNorm = 1.e+6;
const double normStartValue = 1.e+3;

const int fitStatus_converged_RooFit = 0;

TemplateFitAdapter_RooFit::model1dTypeRooFitSpecific::model1dTypeRooFitSpecific(
  const std::string& processName, const std::string& varName,
  RooRealVar* var, RooRealVar* norm,
  bool fitSimultaneously, bool applySmoothing, const edm::ParameterSet& cfgSmoothing,
  const TemplateFitAdapterBase::model1dType* model1dEntryBase)
  : processName_(processName),
    varName_(varName),
    model1dEntryBase_(model1dEntryBase),
    varRef_(var),
    normRef_(norm),
    fittedEventFractionName_(std::string(processName).append("_").append(varName).append("_").append("fittedEventFraction")),
    fittedEventFraction_(0),
    numEventsFittedName_(std::string(processName).append("_").append(varName).append("_").append("numEventsFitted")),
    numEventsFitted_(0),
    pdfName_(std::string(processName).append("_").append(varName).append("_").append("pdf")),
    pdf_(0),
    fitSimultaneously_(fitSimultaneously),
    auxNormTemplateShapeName_(std::string(processName).append("_").append(varName).append("_").append("normTemplateShape")),
    auxNormTemplateShape_(0),
    auxPdfTemplateShapeSumName_(std::string(processName).append("_").append(varName).append("_").append("pdfTemplateShapeSum")),
    auxPdfTemplateShapeSum_(0),
    dataHistName_(std::string(processName_).append("_").append(varName_).append("_rooDataHist")),
    dataHist_(0),
    applySmoothing_(applySmoothing), 
    cfgSmoothing_(cfgSmoothing),
    auxTF1Wrapper_(0),
    pdfBinning_(0),
    pdfCoeffCollection_(0),
    pdfCoeffArgs_(0)
{}

TemplateFitAdapter_RooFit::model1dTypeRooFitSpecific::~model1dTypeRooFitSpecific()
{
  delete fittedEventFraction_;
  delete numEventsFitted_;
  delete pdf_;
  delete auxNormTemplateShape_;
  delete auxPdfTemplateShapeSum_;
  delete dataHist_;
  delete auxTF1Wrapper_;
  delete pdfBinning_;
  delete pdfCoeffCollection_;
  delete pdfCoeffArgs_;
}

void TemplateFitAdapter_RooFit::model1dTypeRooFitSpecific::buildPdf()
{
  std::cout << "<model1dTypeRooFitSpecific::buildPdf>:" << std::endl;

  if ( applySmoothing_ ) {
    bool isFirstFit = (!auxTF1Wrapper_);
    
    if ( isFirstFit ) {
      std::string pluginTypeTF1Wrapper = cfgSmoothing_.getParameter<std::string>("pluginType");
      auxTF1Wrapper_ = TF1WrapperPluginFactory::get()->create(pluginTypeTF1Wrapper, cfgSmoothing_);
    } else {
      auxTF1Wrapper_->reinitializeTF1Parameter();
    }

    std::string fitOption = ( isFirstFit ) ? "RB0" : "RB0Q";
    
    model1dEntryBase_->fluctHistogram_->Fit(auxTF1Wrapper_->getTF1(), fitOption.data());

    delete pdf_;
    pdf_ = new RooTFnPdfBinding(pdfName_.data(), pdfName_.data(), auxTF1Wrapper_->getTF1(), RooArgList(*varRef_));
  } else if ( fitSimultaneously_ ) {
    bool isFirstFit = (!pdfCoeffCollection_);

    if ( isFirstFit ) {
      pdfBinning_ = new TArrayD(getBinning(model1dEntryBase_->fluctHistogram_->GetXaxis()));
      pdfCoeffCollection_ = new TObjArray();

      const RooAbsBinning& xRange = varRef_->getBinning();
      double xMin = xRange.lowBound();
      double xMax = xRange.highBound();
      std::cout << "pdfName = " << pdfName_ << ": xMin = " << xMin << ", xMax = " << xMax << std::endl;

      unsigned numBins = pdfBinning_->GetSize() - 1;
      for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
	std::ostringstream pdfCoeffName;
	pdfCoeffName << processName_ << "_" << varName_ << "_coeff" << iBin;	

	RooAbsReal* pdfCoeff = 0;
	if ( pdfBinning_->At(iBin + 1) > xMin && pdfBinning_->At(iBin) < xMax ) {
	  pdfCoeff = new RooRealVar(pdfCoeffName.str().data(), pdfCoeffName.str().data(), 0., 1.);
	} else {
	  double pdfCoeffValue = model1dEntryBase_->fluctHistogram_->GetBinContent(iBin);
	  pdfCoeff = new RooConstVar(pdfCoeffName.str().data(), pdfCoeffName.str().data(), pdfCoeffValue);
	}

	pdfCoeffCollection_->Add(pdfCoeff);
      }
      
      pdfCoeffCollection_->SetOwner();

      std::string pdfCoeffArgName = std::string(processName_).append("_").append(varName_).append("_pdfCoeffArgs");
      pdfCoeffArgs_ = new RooArgList(*pdfCoeffCollection_, pdfCoeffArgName.data());
    }

    unsigned numBins = pdfCoeffCollection_->GetEntries();
    for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
      RooAbsRealLValue* pdfCoeff = dynamic_cast<RooAbsRealLValue*>(pdfCoeffCollection_->At(iBin));
      if ( pdfCoeff && !pdfCoeff->isConstant() ) {
	double pdfCoeffValue = 0.25*(3.*model1dEntryBase_->fluctHistogram_->GetBinContent(iBin) + 1./numBins);
	pdfCoeff->setVal(pdfCoeffValue);
      }
    }
    
    delete pdf_;
    pdf_ = new RooParametricStepFunction(pdfName_.data(), pdfName_.data(), *varRef_, *pdfCoeffArgs_, *pdfBinning_, numBins);
  } else {
    delete dataHist_;
    dataHist_ = new RooDataHist(dataHistName_.data(), dataHistName_.data(), *varRef_, model1dEntryBase_->fluctHistogram_);

    delete pdf_;
    pdf_ = new RooHistPdf(pdfName_.data(), pdfName_.data(), *varRef_, *dataHist_);
  }

  delete fittedEventFraction_;
  double fittedFraction = model1dEntryBase_->fittedFraction_;
  fittedEventFraction_ = new RooConstVar(fittedEventFractionName_.data(), fittedEventFractionName_.data(), fittedFraction);

  delete numEventsFitted_;
  numEventsFitted_ = new RooFormulaVar(numEventsFittedName_.data(), numEventsFittedName_.data(), 
				       "@0*@1", RooArgList(*normRef_, *fittedEventFraction_));
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TemplateFitAdapter_RooFit::modelNdTypeRooFitSpecific::modelNdTypeRooFitSpecific(const std::string& processName, 
  double norm_initial, bool applyNormConstraint, double valueNormConstraint, double sigmaNormConstraint)
  : processName_(processName),
    norm_(0),
    applyNormConstraint_(applyNormConstraint),
    pdfNormConstraint_(0), 
    meanNormConstraint_(0),
    sigmaNormConstraint_(0)
{
  std::string normName = std::string(processName_).append("_").append("norm");
  norm_ = new RooRealVar(normName.data(), normName.data(), norm_initial, 0., maxNorm);

  if ( applyNormConstraint_ ) {
    std::cout << "<modelNdType>: constraining norm = " << valueNormConstraint << " +/- " << sigmaNormConstraint << ","
	      << " process = " << processName_ << std::endl;
    
    std::string meanNormConstraintName = std::string(processName_).append("_").append("meanNormConstraint");
    meanNormConstraint_ = new RooConstVar(meanNormConstraintName.data(), meanNormConstraintName.data(), valueNormConstraint);
     std::string sigmaNormConstraintName = std::string(processName_).append("_").append("sigmaNormConstraint");
    sigmaNormConstraint_ = new RooConstVar(sigmaNormConstraintName.data(), sigmaNormConstraintName.data(), sigmaNormConstraint);
    
    std::string pdfNormConstraintName = std::string(processName_).append("_").append("pdfNormConstraint");
    pdfNormConstraint_ = new RooGaussian(pdfNormConstraintName.data(), pdfNormConstraintName.data(), 
					 *norm_, *meanNormConstraint_, *sigmaNormConstraint_);
  }
}

TemplateFitAdapter_RooFit::modelNdTypeRooFitSpecific::~modelNdTypeRooFitSpecific()
{
  for ( std::map<std::string, model1dTypeRooFitSpecific*>::iterator it = model1dEntriesImpSpecific_.begin();
	it != model1dEntriesImpSpecific_.end(); ++it ) {
    delete it->second;
  }

  delete norm_;

  delete pdfNormConstraint_;
  delete meanNormConstraint_;
  delete sigmaNormConstraint_;
}

void TemplateFitAdapter_RooFit::modelNdTypeRooFitSpecific::addVar(const std::string& varName, RooRealVar* var,
								  bool fitSimultaneously, 
								  bool applySmoothing, const edm::ParameterSet& cfgSmoothing,
								  const TemplateFitAdapterBase::model1dType* model1dEntryBase)
{
  model1dEntriesImpSpecific_[varName] 
    = new model1dTypeRooFitSpecific(processName_, varName, var, norm_, fitSimultaneously, applySmoothing, cfgSmoothing, model1dEntryBase);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

template<class T>
const T* getMapValue(const std::map<std::string, T*>& mapArg, const std::string& keyArg)
{
  if ( mapArg.find(keyArg) != mapArg.end() ) {
    return mapArg.find(keyArg)->second;
  } else {
    edm::LogError ("getMapValue") << " Failed to find Object for key = " << keyArg << "!!";
    return 0;
  }
}

TemplateFitAdapter_RooFit::TemplateFitAdapter_RooFit(const edm::ParameterSet& cfg)
  : TemplateFitAdapterBase(cfg),
    fitData_(0),
    fitModel_(0),
    fitCategories_(0),
    fitResultImpSpecific_(0)
{
  std::cout << "<TemplateFitAdapter_RooFit::TemplateFitAdapter_RooFit>:" << std::endl;

  edm::ParameterSet cfgFit = cfg.getParameter<edm::ParameterSet>("fit");

//--- read list of variables to be used in fit;
//    create RooRealVar for each
  edm::ParameterSet cfgVariables = cfgFit.getParameter<edm::ParameterSet>("variables");
  for ( vstring::const_iterator varName = varNames_.begin();
	varName != varNames_.end(); ++varName ) {
    edm::ParameterSet cfgVariable = cfgVariables.getParameter<edm::ParameterSet>(*varName);

    std::string name = cfgVariable.getParameter<std::string>("name");
    std::string title = ( cfgVariable.exists("title") ) ? cfgVariable.getParameter<std::string>("title") : name;
    
    // CV: use dummy range for now, to be initialized with correct values later,
    //     once histograms that are fitted have been retrieved from DQMStore
    fitVariables_[*varName] = new RooRealVar(name.data(), title.data(), 0., 1.);
  }

//--- read configuration parameters specifying signal and background templates
//    and options for fitting the templates to the distributions observed in (pseudo)data
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {
    
    const TemplateFitAdapterBase::modelNdType* modelNdEntryBase = modelNdEntries_[*processName];

    edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);

    double norm_initial = ( cfgProcess.exists("norm") ) ? 
      cfgProcess.getParameter<edm::ParameterSet>("norm").getParameter<double>("initial") : normStartValue;

    bool applyNormConstraints = false;
    double meanNormConstraints = 0.;
    double sigmaNormConstraints = 0.;
    if ( cfgFit.exists("constraints") ) {
      edm::ParameterSet cfgConstraints = cfgFit.getParameter<edm::ParameterSet>("constraints");
      if ( cfgConstraints.exists(*processName) ) {
	edm::ParameterSet cfgConstraint = cfgConstraints.getParameter<edm::ParameterSet>(*processName);
	edm::ParameterSet cfgNormConstraint = cfgConstraint.getParameter<edm::ParameterSet>("norm");
      
	applyNormConstraints = true;
	meanNormConstraints = cfgNormConstraint.getParameter<double>("value");
	sigmaNormConstraints = cfgNormConstraint.getParameter<double>("uncertainty");
      }
    }

    modelNdTypeRooFitSpecific* modelNdEntryImpSpecific 
      = new modelNdTypeRooFitSpecific(*processName, norm_initial, applyNormConstraints, meanNormConstraints, sigmaNormConstraints);

    edm::ParameterSet cfgTemplates = cfgProcess.getParameter<edm::ParameterSet>("templates");
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      if ( !cfgTemplates.exists(*varName) ) {
	edm::LogError ("TemplateFitAdapter_RooFit") << " No Template of variable = " << (*varName) 
						    << " defined for process = " << (*processName) << " !!";
	error_ = 1;
	continue;
      }
      
      edm::ParameterSet cfgTemplate = cfgTemplates.getParameter<edm::ParameterSet>(*varName);

      std::string meName = cfgTemplate.getParameter<std::string>("meName");

      bool fitSimultaneously = ( cfgTemplate.exists("fitSimultaneously") ) ? 
	cfgTemplate.getParameter<bool>("fitSimultaneously") : false;

      bool applySmoothing = cfgTemplate.exists("smoothing");
      edm::ParameterSet cfgSmoothing = ( applySmoothing ) ? 
	cfgTemplate.getParameter<edm::ParameterSet>("smoothing") : edm::ParameterSet();

      if ( fitSimultaneously && applySmoothing ) {
	edm::LogError ("TemplateFitAdapter_RooFit") << " Parameters 'fitSimultaneously' and 'applySmoothing' are mutually exclusive !!";
	error_ = 1;
	continue;
      }

      const TemplateFitAdapterBase::model1dType* model1dEntryBase = getMapValue(modelNdEntryBase->model1dEntries_, *varName);
      if ( model1dEntryBase ) {
	modelNdEntryImpSpecific->addVar(*varName, fitVariables_[*varName], 
					fitSimultaneously, applySmoothing, cfgSmoothing, model1dEntryBase);
      } else {
	error_ = 1;
      }
    }

    modelNdEntriesImpSpecific_[*processName] = modelNdEntryImpSpecific;
  }
}

TemplateFitAdapter_RooFit::~TemplateFitAdapter_RooFit()
{
  std::cout << "<TemplateFitAdapter_RooFit::~TemplateFitAdapter_RooFit>:" << std::endl;

  delete fitData_;

  for ( modelNdEntryMapImpSpecific::iterator it = modelNdEntriesImpSpecific_.begin();
	it != modelNdEntriesImpSpecific_.end(); ++it ) {
    delete it->second;
  }

  for ( pdfModelSumMap::iterator it = pdfModelSums_.begin();
	it != pdfModelSums_.end(); ++it ) {
    delete it->second;
  }

  // CV: do not delete fitModel 
  //    - doing so causes a segmentation violation ?!
  //delete fitModel_;
  
  delete fitCategories_;
	
  for ( varMap::iterator it = fitVariables_.begin();
	it != fitVariables_.end(); ++it ) {
    delete it->second;
  }

  delete fitResultImpSpecific_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

std::string getCategoryName_data(const std::string& varName)
{
  return std::string(varName).append("_data");
}

std::string getCategoryName_template(const std::string& processName, const std::string& varName)
{
  return std::string(processName).append("_").append(varName).append("_template");
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TemplateFitAdapter_RooFit::buildFitData()
{
  std::cout << "<TemplateFitAdapter_RooFit::buildFitData>:" << std::endl;

  std::string fitDataName = "fitData";

  if ( fitCategories_->numTypes() == 1 ) {
    delete fitData_;
    RooRealVar* var = fitVariables_[varNames_.front()];
    TH1* hist = dataNdEntry_->data1dEntries_[varNames_.front()]->fluctHistogram_;
    fitData_ = new RooDataHist(fitDataName.data(), fitDataName.data(), *var, hist);
  } else {
    TObjArray varCollection;
    
    std::map<std::string, TH1*> histMap;
    
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      
      varCollection.Add(fitVariables_[*varName]);
      
      std::string categoryName_data = getCategoryName_data(*varName);
      histMap[categoryName_data] = dataNdEntry_->data1dEntries_[*varName]->fluctHistogram_;
      
      for ( vstring::const_iterator processName = processNames_.begin();
	    processName != processNames_.end(); ++processName ) {
	if ( modelNdEntriesImpSpecific_[*processName]->model1dEntriesImpSpecific_[*varName]->fitSimultaneously_ ) {
	  std::string categoryName_template = getCategoryName_template(*processName, *varName);
	  histMap[categoryName_template] = modelNdEntries_[*processName]->model1dEntries_[*varName]->fluctHistogram_;
	}
      }
    }

    std::cout << "num. variables = " << varCollection.GetEntries() << std::endl;
    
    for ( std::map<std::string, TH1*>::const_iterator histMapEntry = histMap.begin();
	  histMapEntry != histMap.end(); ++histMapEntry ) {
      std::cout << "histMap[" << histMapEntry->first << "] = " << histMapEntry->second->GetName() << ":"
		<< " dimension = " << histMapEntry->second->GetDimension() << std::endl;
    }

    // CV: RooFit seems to produce error message
    //    "ERROR: dimension of input histogram must match number of continuous variables"
    //     and aborts with an assert statement in case more than one histogram is fitted simultaneously
    //     and the histograms depend on different variables/are fitted in different ranges ?!
    //
    //    (see posting in http://root.cern.ch/phpBB2/viewtopic.php?t=9518 RootTalk forum)
    //
    if ( varCollection.GetEntries() > 1 ) {
      edm::LogError ("buildFitData") << " Simultaneous fits of more than one variable not supported yet !!";
      assert(0);
    }

    delete fitData_;
    fitData_ = new RooDataHist(fitDataName.data(), fitDataName.data(), RooArgList(varCollection), *fitCategories_, histMap); 
  }
}

void TemplateFitAdapter_RooFit::buildFitModel()
{
  std::cout << "<TemplateFitAdapter_RooFit::buildFitModel>:" << std::endl;

  for ( vstring::const_iterator varName = varNames_.begin();
	varName != varNames_.end(); ++varName ) {
    std::string pdfModelSumName = std::string(*varName).append("_pdfModelSum");

    TObjArray pdfModelSum_pdfCollection;
    TObjArray pdfModelSum_normCollection;

    for ( vstring::const_iterator processName = processNames_.begin();
	  processName != processNames_.end(); ++processName ) {
      modelNdTypeRooFitSpecific* modelNdEntryImpSpecific = modelNdEntriesImpSpecific_[*processName];
      model1dTypeRooFitSpecific* model1dEntryImpSpecific = modelNdEntryImpSpecific->model1dEntriesImpSpecific_[*varName];
      
      model1dEntryImpSpecific->buildPdf();

      pdfModelSum_pdfCollection.Add(model1dEntryImpSpecific->pdf_);
      pdfModelSum_normCollection.Add(model1dEntryImpSpecific->numEventsFitted_);
    }
    
    //std::cout << "--> creating RooAddPdf with name = " << pdfModelSumName << std::endl;
    //std::cout << " #pdfs = " << RooArgList(pdfModelSum_pdfCollection).getSize() << std::endl;
    //std::cout << " #coefficients = " << RooArgList(pdfModelSum_normCollection).getSize() << std::endl;
    delete pdfModelSums_[*varName];
    pdfModelSums_[*varName] = new RooAddPdf(pdfModelSumName.data(), pdfModelSumName.data(), 
					    RooArgList(pdfModelSum_pdfCollection), RooArgList(pdfModelSum_normCollection));
  }
  
  std::string fitModelName = "fitModel";

  if ( fitCategories_->numTypes() == 1 ) {
    fitModel_ = pdfModelSums_[varNames_.front()];
  } else {
    delete fitModel_;
    fitModel_ = new RooSimultaneous(fitModelName.data(), fitModelName.data(), *fitCategories_);
    
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      
      std::string categoryName_data = getCategoryName_data(*varName);
      ((RooSimultaneous*)fitModel_)->addPdf(*pdfModelSums_[*varName], categoryName_data.data());
      
      for ( vstring::const_iterator processName = processNames_.begin();
	    processName != processNames_.end(); ++processName ) {
	modelNdTypeRooFitSpecific* modelNdEntryImpSpecific = modelNdEntriesImpSpecific_[*processName];
	model1dTypeRooFitSpecific* model1dEntryImpSpecific = modelNdEntryImpSpecific->model1dEntriesImpSpecific_[*varName];

	if ( model1dEntryImpSpecific->fitSimultaneously_ ) {
	  delete model1dEntryImpSpecific->auxNormTemplateShape_;
	  const char* normTemplateShapeName = model1dEntryImpSpecific->auxNormTemplateShapeName_.data();
	  double normTemplateShapeValue = getIntegral(modelNdEntries_[*processName]->model1dEntries_[*varName]->fluctHistogram_);
	  model1dEntryImpSpecific->auxNormTemplateShape_ = new RooRealVar(normTemplateShapeName, normTemplateShapeName, 
									  normTemplateShapeValue, 0., maxNorm);
	  
	  delete model1dEntryImpSpecific->auxPdfTemplateShapeSum_;
	  const char* pdfTemplateShapeSumName = model1dEntryImpSpecific->auxPdfTemplateShapeSumName_.data();
	  RooAbsPdf* pdfTemplateShape = model1dEntryImpSpecific->pdf_;
	  RooRealVar* normTemplateShape = model1dEntryImpSpecific->auxNormTemplateShape_;
	  //std::cout << "--> creating RooAddPdf with name = " << model1dEntryImpSpecific->pdfTemplateShapeSumName_ << std::endl;
	  //std::cout << " #pdfs = " << RooArgList(*pdfTemplateShape).getSize() << std::endl;
	  //std::cout << " #coefficients = " << RooArgList(*normTemplateShape).getSize() << std::endl;
	  model1dEntryImpSpecific->auxPdfTemplateShapeSum_ = new RooAddPdf(pdfTemplateShapeSumName, pdfTemplateShapeSumName, 
									   RooArgList(*pdfTemplateShape), RooArgList(*normTemplateShape));
	  
	  std::string categoryName_template = getCategoryName_template(*processName, *varName);
	  ((RooSimultaneous*)fitModel_)->addPdf(*model1dEntryImpSpecific->auxPdfTemplateShapeSum_, categoryName_template.data());
	}
      }
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void TemplateFitAdapter_RooFit::fitImp(int printLevel, int printWarnings) 
{
//-------------------------------------------------------------------------------
// fit template shapes of different signal and background processes
// to distribution observed in (pseudo)data using RooFit algorithms
//-------------------------------------------------------------------------------

//--- initialize ranges of variables used in fit
  for ( vstring::const_iterator varName = varNames_.begin();
	varName != varNames_.end(); ++varName ) {
    const TAxis* axis = dataNdEntry_->data1dEntries_[*varName]->histogram_->GetXaxis();
    double min = axis->GetXmin();
    double max = axis->GetXmax();
    if ( printLevel > 0 ) {
      std::cout << " range fitted for variable = " << (*varName) << ": " << min << ".." << max << std::endl;
    }
    
    fitVariables_[*varName]->setRange(min, max);
  }
  
//--- configure categories used in simultaneous fits
  fitCategories_ = new RooCategory("fitCategories", "fitCategories");
  for ( vstring::const_iterator varName = varNames_.begin();
	varName != varNames_.end(); ++varName ) {
    std::string categoryName_data = getCategoryName_data(*varName);
    fitCategories_->defineType(categoryName_data.data());
    
    for ( vstring::const_iterator processName = processNames_.begin();
	  processName != processNames_.end(); ++processName ) {
      if ( modelNdEntriesImpSpecific_[*processName]->model1dEntriesImpSpecific_[*varName]->fitSimultaneously_ ) {
	std::string categoryName_template = getCategoryName_template(*processName, *varName);
	fitCategories_->defineType(categoryName_template.data());
      }
    }
  }
  
  if ( printLevel > 0 ) {
    std::cout << "number of Categories = " << fitCategories_->numTypes() << std::endl;
    unsigned numCategories = fitCategories_->numTypes();
    for ( unsigned iCategory = 0; iCategory < numCategories; ++iCategory ) {
      std::cout << "Category[" << iCategory << "] = " << fitCategories_->lookupType(iCategory)->GetName() << std::endl;
    }
  }
  
//--- configure RooFit model and data structures
  buildFitData();

  buildFitModel();

//--- print-out structure of RooFit model
  if ( printLevel > 0 ) {
    std::cout << ">>> RootFit model used for Template method Fit <<<" << std::endl;
    fitModel_->printCompactTree();

    std::cout << ">>> RootFit Parameters <<<" << std::endl;
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      std::cout << "for Variable = " << (*varName) << ":" << std::endl;
      RooAbsPdf* pdfModelSum = pdfModelSums_[*varName];
      RooArgSet* pdfModelSumParameters = pdfModelSum->getParameters(*pdfModelSum->getComponents());
      pdfModelSumParameters->Print("v");
      delete pdfModelSumParameters;
    }
    
    std::cout << ">>> RootFit Observables <<<" << std::endl;
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {
      std::cout << "for Variable = " << (*varName) << ":" << std::endl;
      RooAbsPdf* pdfModelSum = pdfModelSums_[*varName];
      RooArgSet* pdfModelSumObservables = pdfModelSum->getObservables(*pdfModelSum->getComponents());
      pdfModelSumObservables->Print("v");
      delete pdfModelSumObservables;
    }
  }
  
//--- build list of fit options
  RooLinkedList fitOptions;

//--- perform extended likelihood fit
  fitOptions.Add(new RooCmdArg(RooFit::Extended()));

//--- check if "external" constraints exist on normalization factors to be determined by fit
//    (specified by Gaussian probability density functions with mean and sigma obtained
//     e.g. by level of agreement between Monte Carlo simulation and number of events observed in background enriched samples)
  TObjArray normConstraints_pdfCollection;
  for ( modelNdEntryMapImpSpecific::iterator modelNdEntryImpSpecific = modelNdEntriesImpSpecific_.begin();
	modelNdEntryImpSpecific != modelNdEntriesImpSpecific_.end(); ++modelNdEntryImpSpecific ) {
    if ( modelNdEntryImpSpecific->second->applyNormConstraint_ ) 
      normConstraints_pdfCollection.Add(modelNdEntryImpSpecific->second->pdfNormConstraint_);
  }

  if ( normConstraints_pdfCollection.GetEntries() > 0 ) {
    std::string normConstraints_pdfArgName = std::string("normConstraints").append("_pdfArgs");
    RooArgSet normConstraints_pdfArgs(normConstraints_pdfCollection, normConstraints_pdfArgName.data());
    
    fitOptions.Add(new RooCmdArg(RooFit::ExternalConstraints(normConstraints_pdfArgs)));
  }

//--- save results of fit for later analysis
  fitOptions.Add(new RooCmdArg(RooFit::Save(true)));

//--- stop Minuit from printing lots of information 
//    about progress on fit and warnings
  fitOptions.Add(new RooCmdArg(RooFit::PrintLevel(printLevel)));
  fitOptions.Add(new RooCmdArg(RooFit::PrintEvalErrors(printWarnings)));
  fitOptions.Add(new RooCmdArg(RooFit::Warnings(printWarnings)));

//--- perform fit
  RooFitResult* fitResult = fitModel_->fitTo(*fitData_, fitOptions);
  fitResult_->status_ = ( fitResult->status() == fitStatus_converged_RooFit ) ? 0 : 1;
  fitResultImpSpecific_ = fitResult;

//--- delete fit option objects
  int numCmdArgs = fitOptions.GetSize();
  for ( int iCmdArg = 0; iCmdArg < numCmdArgs; ++iCmdArg ) {
    delete fitOptions.At(iCmdArg);
  }
}

void TemplateFitAdapter_RooFit::unpackFitResultsImpSpecific()
{
//--- save normalization factors determined by RooFit

  if ( !fitResultImpSpecific_ ) return;

  const RooArgList& fitParameter = fitResultImpSpecific_->floatParsFinal();
  
  unsigned numProcesses = processNames_.size();
  fitResult_->normValues_.ResizeTo(numProcesses);
  fitResult_->normCov_.ResizeTo(numProcesses, numProcesses);
  for ( unsigned iProcess = 0; iProcess < numProcesses; ++iProcess ) {
    const std::string& processName = processNames_[iProcess];

    const RooAbsArg* paramI_arg = fitParameter.at(iProcess);
    const RooRealVar* paramI = dynamic_cast<const RooRealVar*>(paramI_arg);
    assert(paramI != NULL);
    double sigmaI = paramI->getError();

    TemplateFitAdapterBase::fitResultType::normEntryType& norm = fitResult_->normalizations_[processName];
    norm.value_ = paramI->getVal();
    if ( paramI->hasAsymError() ) {
      norm.errUp_ = paramI->getAsymErrorHi();
      norm.errDown_ = paramI->getAsymErrorLo();
    } else {
      norm.errUp_ = norm.errDown_ = sigmaI;
    }
    
    for ( unsigned jProcess = 0; jProcess < numProcesses; ++jProcess ) {
      if ( jProcess == iProcess ) {
	fitResult_->normCov_(iProcess, jProcess) = sigmaI*sigmaI;
      } else {
	const RooAbsArg* paramJ_arg = fitParameter.at(jProcess);
	const RooRealVar* paramJ = dynamic_cast<const RooRealVar*>(paramJ_arg);
	assert(paramJ != NULL);
	double sigmaJ = paramJ->getError();
	double corrIJ = fitResultImpSpecific_->correlation(*paramI_arg, *paramJ_arg);
	fitResult_->normCov_(iProcess, jProcess) = sigmaI*sigmaJ*corrIJ;
      }
    }
  }
}

void TemplateFitAdapter_RooFit::saveFitResultsImpSpecific() 
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("saveFitResultsImpSpecific") << " Failed to access dqmStore --> fit results will NOT be saved !!";
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {   
    modelNdTypeRooFitSpecific* modelNdEntryImpSpecific = modelNdEntriesImpSpecific_[*processName];

    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {      
      model1dTypeRooFitSpecific* model1dEntryImpSpecific = modelNdEntryImpSpecific->model1dEntriesImpSpecific_[*varName];

      if ( model1dEntryImpSpecific->pdfCoeffCollection_ ) {
	int numCoefficients = model1dEntryImpSpecific->pdfCoeffCollection_->GetEntries();
	for ( int iCoefficient = 0; iCoefficient < numCoefficients; ++iCoefficient ) {
	  const RooAbsReal* coefficient_base = dynamic_cast<RooAbsReal*>(model1dEntryImpSpecific->pdfCoeffCollection_->At(iCoefficient));

	  std::string coeffName = coefficient_base->GetName();
	  double coeffValue = coefficient_base->getVal();
	  double coeffErrUp = 0.;
	  double coeffErrDown = 0.;
	  
	  if ( dynamic_cast<const RooRealVar*>(coefficient_base) ) {
	    const RooRealVar* coefficient_derived = dynamic_cast<const RooRealVar*>(coefficient_base);

	    if ( coefficient_derived->hasAsymError() ) {
	      coeffErrUp = coefficient_derived->getAsymErrorHi();
	      coeffErrDown = coefficient_derived->getAsymErrorLo();
	    } else {
	      coeffErrUp = coeffErrDown = coefficient_derived->getError();
	    }
	  } 

	  saveFitParameter(dqmStore, dqmDirectory_fitResult_, *processName, coeffName, coeffValue, coeffErrUp, coeffErrDown);
	}
      }
    }
  }
}

void TemplateFitAdapter_RooFit::makeControlPlotsImpSpecific()
{
//--- produce control plots of smoothing functions
//    fitted to shape templates in order to reduce effect of statistical fluctuations
//    of shape templates on fit results
//    (and in particular effect of different statistical precision with which shape templates
//     are determined for different background processes in background enriched samples)

  TCanvas canvas("TemplateFitAdapter_RooFit", "TemplateFitAdapter_RooFit", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);
  canvas.SetFrameFillColor(10);

  int defStyle_optStat = gStyle->GetOptStat();
  int defStyle_optFit = gStyle->GetOptStat();
  float defStyle_labelSizeX = gStyle->GetLabelSize("x");
  float defStyle_labelSizeY = gStyle->GetLabelSize("y");

  //gStyle->SetOptStat(1111);
  //gStyle->SetOptFit(111);
  gStyle->SetLabelSize(0.03, "x");
  gStyle->SetLabelSize(0.03, "y");
  
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {   
    const modelNdTypeRooFitSpecific* modelNdEntryImpSpecific = modelNdEntriesImpSpecific_[*processName];
    
    for ( vstring::const_iterator varName = varNames_.begin();
	  varName != varNames_.end(); ++varName ) {      
      const model1dTypeRooFitSpecific* model1dEntryImpSpecific 
	= getMapValue(modelNdEntryImpSpecific->model1dEntriesImpSpecific_, *varName);

      if ( !(model1dEntryImpSpecific && model1dEntryImpSpecific->applySmoothing_) ) continue;

      const TemplateFitAdapterBase::model1dType* model1dEntryBase = model1dEntryImpSpecific->model1dEntryBase_;

      std::string histogramName = std::string(model1dEntryBase->fluctHistogram_->GetName()).append("_cloned");
      TH1* histogram_cloned = (TH1*)model1dEntryBase->fluctHistogram_->Clone(histogramName.data());
      histogram_cloned->SetStats(false);
      histogram_cloned->GetXaxis()->SetTitle(varName->data());
      histogram_cloned->SetLineStyle(1);
      histogram_cloned->SetLineColor(1);
      histogram_cloned->SetLineWidth(2);

      std::string tf1Name = std::string(model1dEntryImpSpecific->auxTF1Wrapper_->getTF1()->GetName()).append("_cloned");
      TF1* tf1_cloned = (TF1*)model1dEntryImpSpecific->auxTF1Wrapper_->getTF1()->Clone(tf1Name.data());
      tf1_cloned->SetLineStyle(1);
      tf1_cloned->SetLineColor(2);
      tf1_cloned->SetLineWidth(2);
      
      histogram_cloned->SetMinimum(0.);
      double yMax = histogram_cloned->GetMaximum();
      histogram_cloned->SetMaximum(1.4*yMax);

      TLegend legend(0.63, 0.38, 0.89, 0.54);
      legend.SetBorderSize(0);
      legend.SetFillColor(0);

      legend.AddEntry(histogram_cloned, "Template Histogram", "p");
      legend.AddEntry(tf1_cloned, "Smoothing Function", "l");

      histogram_cloned->Draw("e1p");
      tf1_cloned->Draw("lsame");
      
      //legend.Draw();

      canvas.Update();

      int errorFlag = 0;
      std::string fileNameParam = std::string(*processName).append("_").append(*varName).append("_smoothing");
      std::string fileName = replace_string(controlPlotsFileName_, plotKeyword, fileNameParam, 1, 1, errorFlag);
      if ( !errorFlag ) {
	canvas.Print(fileName.data());
      } else {
	edm::LogError("makeControlPlotsSmoothing") << " Failed to decode controlPlotsFileName = " 
						   << controlPlotsFileName_ << " --> skipping !!";
	return;
      }

      delete histogram_cloned;
      delete tf1_cloned;
    }
  }

  gStyle->SetOptStat(defStyle_optStat);
  gStyle->SetOptFit(defStyle_optFit);
  gStyle->SetLabelSize(defStyle_labelSizeX, "x");
  gStyle->SetLabelSize(defStyle_labelSizeY, "y");
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(TemplateHistFitAdapterPluginFactory, TemplateFitAdapter_RooFit, "TemplateFitAdapter_RooFit");

