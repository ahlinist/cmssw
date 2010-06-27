#include "TauAnalysis/BgEstimationTools/plugins/GenMatrixFit.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TObjArray.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TROOT.h>

#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <RooProdPdf.h>
#include <RooAddPdf.h>
#include <RooArgSet.h>
#include <RooGlobalFunc.h>
#include <RooPlot.h>
#include <RooFit.h>
#include <RooBinning.h>
#include <RooGaussian.h>
#include <RooDataHist.h>

#include <iomanip>
#include <fstream>

const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const double maxNorm = 1.e+6;

const double epsilon = 1.e+2;

double getRandom()
{
  static TRandom3 u;

  double value = -10.;
  while ( value < -3 || value > +3 ) {
    value = u.Rndm();
  }

  return value;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void getProbability(double probDensity1, double probDensity2, const Double_t* binBoundaries,
		    double& prob1, double& prob2)
{
//--- "probability" values used by GenMatrix1dPdf are in fact probability densities,
//    so need to integrate probability density over bin-width 

  //std::cout << "<getProbability>:" << std::endl;
  //std::cout << " probDensity1 = " << probDensity1 << std::endl;
  //std::cout << " probDensity2 = " << probDensity2 << std::endl;

  double prob1_unnormalized = (binBoundaries[1] - binBoundaries[0])*probDensity1;
  double prob2_unnormalized = (binBoundaries[2] - binBoundaries[1])*probDensity2;

  //std::cout << " prob1_unnormalized = " << prob1_unnormalized << std::endl;
  //std::cout << " prob2_unnormalized = " << prob2_unnormalized << std::endl;
  
  prob1 = prob1_unnormalized/(prob1_unnormalized + prob2_unnormalized);
  prob2 = prob2_unnormalized/(prob1_unnormalized + prob2_unnormalized);

  //std::cout << " prob1 = " << prob1 << std::endl;
  //std::cout << " prob2 = " << prob2 << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenMatrixFit::objVarEntryType::objVarEntryType(const std::string& name, double xMin, double xBoundary, double xMax)
  : name_(name), xMin_(xMin), xBoundary_(xBoundary), xMax_(xMax) 
{ 
  //std::cout << "<objVarEntryType::objVarEntryType>:" << std::endl;
  //std::cout << " name = " << name_ << std::endl;
  //std::cout << " xMin = " << xMin_ << std::endl;
  //std::cout << " xBoundary = " << xBoundary_ << std::endl;
  //std::cout << " xMax = " << xMax_ << std::endl;

  std::string xName = std::string(name_);
  std::string xTitle = xName;
  std::cout << "--> creating RooRealVar with name = " << xName << ","
	    << " range of validity = [" << xMin_ << ", " << xMax_ << "]..." << std::endl;
  x_ = new RooRealVar(xName.data(), xTitle.data(), xMin, xMax);

  const Int_t numBins = 2;  
  Double_t binBoundaries[numBins + 1];
  binBoundaries[0] = xMin_;
  binBoundaries[1] = xBoundary_;
  binBoundaries[2] = xMax_;
  x_->setBinning(RooBinning(numBins, binBoundaries));
}

GenMatrixFit::objVarEntryType::~objVarEntryType()
{
  //std::cout << "<objVarEntryType::~objVarEntryType>:" << std::endl;

  delete x_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenMatrixFit::model1dEntryType::model1dEntryType(const std::string& name, 
						 objVarEntryType* objVarEntry, 
						 double prob,
						 bool applyProbConstraint, 
						 double valueProbConstraint, 
						 double sigmaProbConstraint)
  : name_(name), 
    applyProbConstraint_(applyProbConstraint)
{
  //std::cout << "<model1dEntryType::model1dEntryType>:" << std::endl;
  //std::cout << " name = " << name_ << std::endl;
  
  const Int_t numBins = 2;  
  Double_t binBoundaries[numBins + 1];
  binBoundaries[0] = objVarEntry->xMin_;
  binBoundaries[1] = objVarEntry->xBoundary_;
  binBoundaries[2] = objVarEntry->xMax_;

//--- "probability" values used by GenMatrix1dPdf are in fact probability densities,
//    so need to devide probability by bin-width (and renormalize)
//
//    NOTE: from the condition 
//
//         1 = prob + (1 - prob) = prob1 + prob2
//           = (binBoundaries[1] - binBoundaries[0])*probDensity1 + (binBoundaries[2] - binBoundaries[1])*probDensity2 
// 
//          it follows that
//
//         probDensity2 =                 1.                    binBoundaries[1] - binBoundaries[0] 
//                        ----------------------------------- - ----------------------------------- * probDensity1 
//                        binBoundaries[2] - binBoundaries[1]   binBoundaries[2] - binBoundaries[1]
//
  double probDensity1 = prob/(binBoundaries[1] - binBoundaries[0]);
  double probDensity2_c0 = 1./(binBoundaries[2] - binBoundaries[1]);
  double probDensity2_c1 = (binBoundaries[1] - binBoundaries[0])/(binBoundaries[2] - binBoundaries[1]);

  std::string prob1Name = std::string(name_).append("_prob1");
  std::string prob1Title = prob1Name;
  std::cout << "--> creating RooRealVar with name = " << prob1Name << ", start-values:" 
	    << " probability = " << prob << ", prob. density = " << probDensity1 << "..." << std::endl;
  prob1_ = new RooRealVar(prob1Name.data(), prob1Title.data(), probDensity1, 0., 1./(binBoundaries[1] - binBoundaries[0]));
  
  std::string prob2Name = std::string(name_).append("_prob2");
  std::string prob2Title = prob2Name;
  std::ostringstream prob2Function;
  prob2Function << probDensity2_c0 << "-" << "(" << probDensity2_c1 << "*" << prob1Name << ")";
  std::cout << "--> creating RooFormulaVar with name = " << prob2Name << "," 
	    << " depending on variable = " << prob1_->GetName() 
	    << " via function = " << prob2Function.str() << "..." << std::endl;
  prob2_ = new RooFormulaVar(prob2Name.data(), prob2Title.data(), prob2Function.str().data(), RooArgList(*prob1_));
  
  std::string pdf1dName = std::string(name_).append("_pdf1d");
  std::string pdf1dTitle = pdf1dName;
  TObjArray binProbabilities;
  binProbabilities.Add(prob1_);
  binProbabilities.Add(prob2_);
  std::cout << "--> creating GenMatrix1dPdf with name = " << pdf1dName.data() << ","
	    << " depending on variables = " << prob1_->GetName() << ", " << prob2_->GetName() << std::endl;
  pdf1d_ = new GenMatrix1dPdf(pdf1dName.data(), pdf1dTitle.data(), *objVarEntry->x_, numBins, binBoundaries, binProbabilities);

  if ( applyProbConstraint_ ) {
    std::cout << "<model1dType>: constraining prob = " << valueProbConstraint << " +/- " << sigmaProbConstraint << ","
	      << " name = " << name_ << std::endl;
    
    std::string meanProbConstraintName = std::string(name_).append("_meanProbConstraint");
    meanProbConstraint_ = new RooConstVar(meanProbConstraintName.data(), meanProbConstraintName.data(), valueProbConstraint);
    std::string sigmaProbConstraintName = std::string(name_).append("_sigmaProbConstraint");
    sigmaProbConstraint_ = new RooConstVar(sigmaProbConstraintName.data(), sigmaProbConstraintName.data(), sigmaProbConstraint);
    
    std::string pdfProbConstraintName = std::string(name_).append("_pdfProbConstraint");
    pdfProbConstraint_ = new RooGaussian(pdfProbConstraintName.data(), pdfProbConstraintName.data(), 
					 *prob1_, *meanProbConstraint_, *sigmaProbConstraint_);
  }
}

GenMatrixFit::model1dEntryType::~model1dEntryType()
{
  //std::cout << "<model1dEntryType::~model1dEntryType>:" << std::endl;

  delete pdf1d_;

  delete prob1_;
  delete prob2_;

  delete pdfProbConstraint_;
  delete meanProbConstraint_;
  delete sigmaProbConstraint_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenMatrixFit::modelNdEntryType::modelNdEntryType(const std::string& name, 
						 const std::vector<objVarEntryType*>& objVarEntries,
						 double norm,
						 bool applyNormConstraint, 
						 double valueNormConstraint, 
						 double sigmaNormConstraint,
						 const std::vector<double>& prob, 
						 const std::vector<bool>& applyProbConstraint, 
						 const std::vector<double>& valueProbConstraint, 
						 const std::vector<double>& sigmaProbConstraint)
  : name_(name),
    applyNormConstraint_(applyNormConstraint)
{
  //std::cout << "<modelNdEntryType::modelNdEntryType>:" << std::endl;
  //std::cout << " name = " << name_ << std::endl;
  //std::cout << " objVarEntries = " << objVarEntries.size() << std::endl;
  
  unsigned numVariables = objVarEntries.size();

  if ( !(prob.size()                == numVariables &&
	 applyProbConstraint.size() == numVariables &&
	 valueProbConstraint.size() == numVariables &&
	 sigmaProbConstraint.size() == numVariables) ) {
    edm::LogError ("modelNdEntryType") << " Number of objVarEntries does not match number of defined probability variables !!";
    assert(0);
  }

  unsigned numVar = objVarEntries.size();
  for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
    objVarEntryType* objVarEntry = objVarEntries[iVar];
    std::string model1dEntryName = std::string(name_).append("_").append(objVarEntry->name_);    
    model1dEntryType* model1dEntry 
      = new model1dEntryType(model1dEntryName.data(), objVarEntry, prob[iVar], 
			     applyProbConstraint[iVar], valueProbConstraint[iVar], sigmaProbConstraint[iVar]);
    model1dEntries_.push_back(model1dEntry);
  }

  std::string normName = std::string(name_).append("_norm");
  std::string normTitle = normName;
  std::cout << "--> creating RooRealVar with name = " << normName << ":" 
            << " start-value = " << norm << "..." << std::endl;
  norm_ = new RooRealVar(normName.data(), normTitle.data(), norm, 0., maxNorm);

  std::string pdfNdName = std::string(name_).append("_pdfNd");
  std::string pdfNdTitle = pdfNdName;
  TObjArray pdf1dCollection;
  for ( std::vector<model1dEntryType*>::iterator model1dEntry = model1dEntries_.begin();
	model1dEntry != model1dEntries_.end(); ++model1dEntry ) {
    pdf1dCollection.Add((*model1dEntry)->pdf1d_);
  }
  std::string pdf1dArgsName = std::string(name_).append("_args");
  RooArgList pdf1dArgs(pdf1dCollection, pdf1dArgsName.data());
  std::cout << "--> creating RooProdPdf with name = " << pdfNdName << std::endl;
  pdfNd_ = new RooProdPdf(pdfNdName.data(), pdfNdTitle.data(), pdf1dArgs);
  
  if ( applyNormConstraint_ ) {
    std::cout << "<modelNdType>: constraining norm = " << valueNormConstraint << " +/- " << sigmaNormConstraint << ","
	      << " name = " << name_ << std::endl;
    
    std::string meanNormConstraintName = std::string(name_).append("_meanNormConstraint");
    meanNormConstraint_ = new RooConstVar(meanNormConstraintName.data(), meanNormConstraintName.data(), valueNormConstraint);
    std::string sigmaNormConstraintName = std::string(name_).append("_sigmaNormConstraint");
    sigmaNormConstraint_ = new RooConstVar(sigmaNormConstraintName.data(), sigmaNormConstraintName.data(), sigmaNormConstraint);
    
    std::string pdfNormConstraintName = std::string(name_).append("_pdfNormConstraint");
    pdfNormConstraint_ = new RooGaussian(pdfNormConstraintName.data(), pdfNormConstraintName.data(), 
					 *norm_, *meanNormConstraint_, *sigmaNormConstraint_);
  }
}

GenMatrixFit::modelNdEntryType::~modelNdEntryType()
{
  delete pdfNd_;

  for ( std::vector<model1dEntryType*>::iterator it = model1dEntries_.begin();
	it != model1dEntries_.end(); ++it ) {
    delete (*it);
  }

  delete norm_;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenMatrixFit::GenMatrixFit(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    cfgError_(0)
{
  //std::cout << "<GenMatrixFit::GenMatrixFit>:" << std::endl;

//--- get binning results from DQMStore
  edm::ParameterSet cfgData = cfg.getParameter<edm::ParameterSet>("data");

  edm::ParameterSet cfgBinningService = cfgData.getParameter<edm::ParameterSet>("binningService");
  std::string binningServicePluginType = cfgBinningService.getParameter<std::string>("pluginType");
  binningService_ = BinningServicePluginFactory::get()->create(binningServicePluginType, cfgBinningService);

  dqmDirectory_data_ = cfgData.getParameter<std::string>("dqmDirectory");

  dataBinningResults_ = binningService_->loadBinningResults(dqmDirectory_data_);
  dataBinGrid_ = dataBinningResults_->binGrid();

//--- determine number of variables;
//    for each variable, get name and and bin-edges
  unsigned numDimensions = dataBinGrid_->dimensions();

  varNames_ = dataBinGrid_->objVarNames();
  if ( varNames_.size() != numDimensions ) {
    edm::LogError ("GenMatrixFit") << " Number of objVarNames does not match number of dimensions !!";
    assert(0);
  }

  std::vector<objVarEntryType*> objVarEntries_vector(numDimensions);

  for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
    const std::string& varName = varNames_[iDimension];

    std::vector<double> binEdges = dataBinGrid_->binEdges(iDimension);

    if ( binEdges.size() != 3 ) {
      edm::LogError ("GenMatrixFit") << " Number of binEdges not equal to three !!";
      assert(0);
    }

    double xMin = binEdges[0];
    double xBoundary = binEdges[1];
    double xMax = binEdges[2]; 

    objVarEntryType* objVarEntry = new objVarEntryType(varName, xMin, xBoundary, xMax);
    objVarEntries_[varName] = objVarEntry;
    objVarEntries_vector[iDimension] = objVarEntry;
  }

//--- get model parameters
//    and draw options
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  processNames_ = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();

  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);

    double norm_initial = cfgProcess.getParameter<edm::ParameterSet>("norm").getParameter<double>("initial");

    std::vector<double> prob_initial(numDimensions);
    for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
      std::ostringstream probName;
      probName << "par" << " " << (iDimension + 1);

      prob_initial[iDimension] = cfgProcess.getParameter<edm::ParameterSet>(probName.str()).getParameter<double>("initial");

//--- WARNING: pdf does not depend on observables at all,
//             in case probability is exactly equal to 0.50,
//             potentially causing problems with RooFit (?)
//
//            --> avoid this "degenerate" case by adding random number
//                to probability value specified by configuration parameter
      while ( TMath::Abs(prob_initial[iDimension] - 0.50) < epsilon ) {
	prob_initial[iDimension] += 10.*epsilon*getRandom();
      }
    }

    bool applyNormConstraint = false;
    double meanNormConstraint = 0.;
    double sigmaNormConstraint = 0.;

    std::vector<bool> applyProbConstraint(numDimensions);
    std::vector<double> meanProbConstraint(numDimensions);
    std::vector<double> sigmaProbConstraint(numDimensions);

    edm::ParameterSet cfgFit = cfg.getParameter<edm::ParameterSet>("fit");
    if ( cfgFit.exists("constraints") ) {
      edm::ParameterSet cfgConstraints = cfgFit.getParameter<edm::ParameterSet>("constraints");
      if ( cfgConstraints.exists(*processName) ) {
	edm::ParameterSet cfgConstraint = cfgConstraints.getParameter<edm::ParameterSet>(*processName);

	if ( cfgConstraint.exists("norm") ) {	  
	  edm::ParameterSet cfgNormConstraint = cfgConstraint.getParameter<edm::ParameterSet>("norm");

	  applyNormConstraint = true;
	  meanNormConstraint = cfgNormConstraint.getParameter<double>("value");
	  sigmaNormConstraint = cfgNormConstraint.getParameter<double>("uncertainty");
	}

	for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
	  std::ostringstream probName;
	  probName << "par" << " " << (iDimension + 1);

	  if ( cfgConstraint.exists(probName.str()) ) {
	    edm::ParameterSet cfgProbConstraint = cfgConstraint.getParameter<edm::ParameterSet>(probName.str());
	    
	    applyProbConstraint[iDimension] = true;
	    meanProbConstraint[iDimension] = cfgProbConstraint.getParameter<double>("value");
	    sigmaProbConstraint[iDimension] = cfgProbConstraint.getParameter<double>("uncertainty");
	  }
	}
      }
    }

    modelNdEntries_[*processName] = new modelNdEntryType(std::string(moduleLabel_).append("_").append(*processName),
							 objVarEntries_vector, 
							 norm_initial, applyNormConstraint, meanNormConstraint, sigmaNormConstraint,
							 prob_initial, applyProbConstraint, meanProbConstraint, sigmaProbConstraint);

    edm::ParameterSet cfgDrawOption = cfgProcess.getParameter<edm::ParameterSet>("drawOptions");
    drawOptions_[*processName] = new drawOptionsType(cfgDrawOption);
  }

//--- read values of configuration parameters controlling verbosity print-out
//    generated during fitting
  edm::ParameterSet cfgFit = cfg.getParameter<edm::ParameterSet>("fit");
  printLevel_ = ( cfgFit.exists("printLevel") ) ? cfgFit.getParameter<int>("printLevel") : 1;
  printWarnings_  = ( cfgFit.exists("printWarnings") ) ? cfgFit.getParameter<bool>("printWarnings") : true;

//--- read configuration parameters specifying options for making control plots
  edm::ParameterSet cfgControlPlots = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("controlPlots");
  controlPlotsFileName_ = cfgControlPlots.getParameter<std::string>("fileName");

//--- read configuration parameters specifying options for saving fit results in DQMStore
  edm::ParameterSet cfgSaveFitResults = cfg.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("fitResults");
  dqmDirectory_fitResult_ = dqmDirectoryName(cfgSaveFitResults.getParameter<std::string>("dqmDirectory"));
}

GenMatrixFit::~GenMatrixFit()
{
  std::cout << "<GenMatrixFit::~GenMatrixFit>:" << std::endl;

  //std::cout << " deleting objVarEntries..." << std::endl;
  for ( std::map<std::string, objVarEntryType*>::iterator it = objVarEntries_.begin();
	it != objVarEntries_.end(); ++it ) {
    delete it->second;
  }

  //std::cout << " deleting drawOptionEntries..." << std::endl;
  for ( std::map<std::string, drawOptionsType*>::iterator it = drawOptions_.begin();
	it != drawOptions_.end(); ++it ) {
    delete it->second;
  }

  //std::cout << " deleting dataSet..." << std::endl;
  delete dataBinningResults_;
  delete binningService_;
  delete dataSet_;

  //std::cout << " deleting pdfModel..." << std::endl;
  delete pdfModel_;

  for ( modelNdEntryMap::iterator it = modelNdEntries_.begin();
	it != modelNdEntries_.end(); ++it ) {
    delete it->second;
  }

  delete fitResult_;
}

//-----------------------------------------------------------------------------------------------------------------------
// build RooFit structure
//-----------------------------------------------------------------------------------------------------------------------

void GenMatrixFit::buildFitData()
{
//--- build dataset
  std::string dataSetName = std::string(moduleLabel_).append("_dataSet");
  std::string dataSetTitle = dataSetName;
  TObjArray objVarCollection;
  for ( std::map<std::string, objVarEntryType*>::iterator objVarEntry = objVarEntries_.begin();
	objVarEntry != objVarEntries_.end(); ++objVarEntry ) {
    objVarCollection.Add(objVarEntry->second->x_);
  }
  dataSet_ = new RooDataHist(dataSetName.data(), dataSetName.data(), RooArgSet(objVarCollection));

  unsigned numBins = dataBinGrid_->numBins();
  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    std::vector<double> binCenter = dataBinGrid_->binCenter(iBin);

    unsigned numDimensions = binCenter.size();
    for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
      const std::string& varName = varNames_[iDimension];

      objVarEntries_[varName]->x_->setVal(binCenter[iDimension]);
    }

    std::vector<binResultType> binResults = dataBinningResults_->getBinResults(iBin);
    double binContent = getBinContent(binResults, "rec");
    double binSumw2 = getBinSumw2(binResults, "rec");
    
    dataSet_->add(RooArgSet(objVarCollection), binContent, binSumw2);
  }
}

void GenMatrixFit::buildFitModel()
{
//--- build probability density function for model

  std::string pdfModelName = std::string(moduleLabel_).append("_pdfModel");
  std::string pdfModelTitle = pdfModelName;
  
  TObjArray pdfCollection;
  TObjArray normCollection;
  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    pdfCollection.Add(modelNdEntry->second->pdfNd_);
    normCollection.Add(modelNdEntry->second->norm_);
  }
  
  std::string pdfArgsName = std::string(moduleLabel_).append("_pdfArgs");
  RooArgList pdfArgs(pdfCollection, pdfArgsName.data());
  std::string normArgsName = std::string(moduleLabel_).append("_normArgs");
  RooArgList normArgs(normCollection, normArgsName.data());
  
  std::cout << "--> creating RooAddPdf with name = " << pdfModelName << std::endl;
  pdfModel_ = new RooAddPdf(pdfModelName.data(), pdfModelTitle.data(), pdfArgs, normArgs); 
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenMatrixFit::fit(int printLevel, int printWarnings)
{
//-----------------------------------------------------------------------------------------------------------------------
// fit number of (pseudo)data events observed in different bins of "generalized matrix"
// with sum of probability density functions for signal and background processes
//-----------------------------------------------------------------------------------------------------------------------

//--- configure RooFit model and data structures
  buildFitData();

  buildFitModel();

//--- print-out structure of RooFit model
  if ( printLevel > 0 ) {
    std::cout << ">>> RootFit model used for Template method Fit <<<" << std::endl;
    pdfModel_->printCompactTree();

    std::cout << ">>> RootFit Parameters <<<" << std::endl;
    for ( vstring::const_iterator processName = processNames_.begin();
	  processName != processNames_.end(); ++processName ) {
      std::cout << "for Process = " << (*processName) << ":" << std::endl;
      RooAbsPdf* pdfNd = modelNdEntries_[*processName]->pdfNd_;
      RooArgSet* pdfNdParameters = pdfNd->getParameters(*pdfNd->getComponents());
      pdfNdParameters->Print("v");
      delete pdfNdParameters;
    }
    
    std::cout << ">>> RootFit Observables <<<" << std::endl;
    for ( vstring::const_iterator processName = processNames_.begin();
	  processName != processNames_.end(); ++processName ) {
      std::cout << "for Process = " << (*processName) << ":" << std::endl;
      RooAbsPdf* pdfNd = modelNdEntries_[*processName]->pdfNd_;
      RooArgSet* pdfNdObservables = pdfNd->getObservables(*pdfNd->getComponents());
      pdfNdObservables->Print("v");
      delete pdfNdObservables;
    }
  }
  
//--- build list of fit options
  RooLinkedList fitOptions;

//--- perform extended likelihood fit
  fitOptions.Add(new RooCmdArg(RooFit::Extended()));

//--- check if "external" constraints exist on normalization factor or probability values to be determined by fit
//    (specified by Gaussian probability density functions with mean and sigma obtained
//     e.g. by level of agreement between Monte Carlo simulation and number of events observed in background enriched control samples)
  TObjArray externalConstraints_pdfCollection;
  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    if ( modelNdEntry->second->applyNormConstraint_ )
      externalConstraints_pdfCollection.Add(modelNdEntry->second->pdfNormConstraint_);

    for ( std::vector<model1dEntryType*>::iterator model1dEntry = modelNdEntry->second->model1dEntries_.begin();
	  model1dEntry != modelNdEntry->second->model1dEntries_.end(); ++model1dEntry ) {
      if ( (*model1dEntry)->applyProbConstraint_ )
	externalConstraints_pdfCollection.Add((*model1dEntry)->pdfProbConstraint_);
    }
  }

  if ( externalConstraints_pdfCollection.GetEntries() > 0 ) {
    std::string externalConstraints_pdfArgName = std::string(moduleLabel_).append("_externalConstraints_pdfArgs");
    RooArgSet externalConstraints_pdfArgs(externalConstraints_pdfCollection, externalConstraints_pdfArgName.data());
    
    fitOptions.Add(new RooCmdArg(RooFit::ExternalConstraints(externalConstraints_pdfArgs)));
  }

//--- save results of fit for later analysis
  fitOptions.Add(new RooCmdArg(RooFit::Save(true)));

//--- stop Minuit from printing lots of information 
//    about progress on fit and warnings
  fitOptions.Add(new RooCmdArg(RooFit::PrintLevel(printLevel)));
  fitOptions.Add(new RooCmdArg(RooFit::PrintEvalErrors(printWarnings)));
  fitOptions.Add(new RooCmdArg(RooFit::Warnings(printWarnings)));

//--- set "precision" of fitted (pseudo)data 
//    to precision which would be obtained with "real" data
//    (unweighted events; works with "real" data as well)
  fitOptions.Add(new RooCmdArg(RooFit::SumW2Error(false)));

//--- perform fit
  fitResult_ = pdfModel_->fitTo(*dataSet_, fitOptions);

  //for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
  //	  modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
  //  modelNdEntry->second->print(std::cout);
  //}

//--- delete fit option objects
  int numCmdArgs = fitOptions.GetSize();
  for ( int iCmdArg = 0; iCmdArg < numCmdArgs; ++iCmdArg ) {
    delete fitOptions.At(iCmdArg);
  }
}

void GenMatrixFit::endJob()
{
  //std::cout << "<GenMatrixFit::endJob>:" << std::endl; 

  std::cout << "<TemplateHistFitter::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("TemplateHistFitter::endJob") 
      << " Failed to access dqmStore" << " --> histogram will NOT be fitted !!";
    return;
  }

//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError ("GenMatrixFit::endJob") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }

//--- perform actual fit
  fit(printLevel_, printWarnings_);

//--- print-out fit results
  std::cout << ">>> Fit Results <<<" << std::endl;
  std::cout << " fitStatus = " << fitResult_->status() << std::endl;
  //std::cout << " Chi2red = " << compChi2red(fitResult_) << std::endl;
  print(std::cout);

//--- store fit results in DQMStore
  if ( dqmDirectory_fitResult_ != "" ) saveFitResults();

//--- produce plot of different signal and background processes
//    using scale factors determined by fit
//    compared to distribution of (pseudo)data
  if ( controlPlotsFileName_ != "" ) makeControlPlots();

  std::cout << "done." << std::endl;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenMatrixFit::saveFitResults()
{

}

void GenMatrixFit::makeControlPlots()
{

}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenMatrixFit::print(std::ostream& stream)
{
  stream << "<GenMatrixFit::print>:" << std::endl;

  unsigned numVar = objVarEntries_.size();

  stream << "Fit Parameter:" << std::endl;
  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    stream << " Process = " << modelNdEntry->first << std::endl;
    stream << "  norm = " << modelNdEntry->second->norm_->getVal() << std::endl;
    std::vector<model1dEntryType*>& model1dEntries = modelNdEntry->second->model1dEntries_;
    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      std::ostringstream probName;
      probName << "P" << (iVar + 1);
      const Double_t* binBoundaries = model1dEntries[iVar]->pdf1d_->getBinBoundaries();
      double prob1, prob2;
      getProbability(model1dEntries[iVar]->prob1_->getVal(), model1dEntries[iVar]->prob2_->getVal(), binBoundaries, prob1, prob2);
      stream << "  " << probName.str() << " = " << prob1 << std::endl;
     }
  }

  stream << "Contributions to Regions:" << std::endl;
  unsigned numRegions = TMath::Nint(TMath::Power(2, numVar));
  for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
    std::ostringstream regionDescription1;
    std::ostringstream regionDescription2;
    regionDescription2 << "Norm * ";
    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      const std::string& varName = varNames_[iVar];

      unsigned bitValue = TMath::Nint(TMath::Power(2, iVar));

      std::string operator_string;
      std::ostringstream probDescription;
      if ( iRegion & bitValue ) {
	operator_string = "<";
	probDescription << "(1 - P" << (iVar + 1) << ")";
      } else {
	operator_string = ">=";
	probDescription << "P" << (iVar + 1);
      }

      regionDescription1 << varName << " " << operator_string << " " << objVarEntries_[varName]->xBoundary_;
      regionDescription2 << probDescription.str();
      if ( iVar < (numVar - 1) ) {
	regionDescription1 << " && "; 
	regionDescription2 << " * "; 
      }
    }

    stream << " " << regionDescription1.str() << std::endl;
    stream << " " << regionDescription2.str() << std::endl;

    for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	  modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
      const std::string& processName = modelNdEntry->first;

      double processContribution = modelNdEntry->second->norm_->getVal();
      std::vector<model1dEntryType*>& model1dEntries = modelNdEntry->second->model1dEntries_;
      for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
	unsigned bitValue = TMath::Nint(TMath::Power(2, iVar));
	
	const Double_t* binBoundaries = model1dEntries[iVar]->pdf1d_->getBinBoundaries();
	double prob1, prob2;
	getProbability(model1dEntries[iVar]->prob1_->getVal(), model1dEntries[iVar]->prob2_->getVal(), binBoundaries, prob1, prob2);

	if ( iRegion & bitValue ) 
	  processContribution *= prob1;
	else 
	  processContribution *= prob2;
      }

      stream << "  " << processName << " = " 
	     << std::setprecision(3) << std::fixed << processContribution << std::endl;
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

/*
void GenMatrixFit::makeControlPlot(const RooRealVar* x, 
					const std::string& variableName, const std::string& variableTitle,
					const std::string& outputFileName)
{
//--- stop ROOT from opening X-window for canvas output
//    (in order to be able to run in batch mode) 
  gROOT->SetBatch(true);

  TCanvas canvas("GenMatrixFit", "GenMatrixFit", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);

  RooPlot* plotFrame = x->frame();
  std::string plotTitle = std::string("GenMatrixFit - Control Distribution of ").append(variableName);
  plotFrame->SetTitle(plotTitle.data());
  dataSet_->plotOn(plotFrame, RooFit::MarkerColor(kBlack), RooFit::MarkerStyle(2));
  const std::vector<pdfSingleProcessEntryType*>& pdfSingleProcessEntries = pdfProcessSumEntry_->pdfSingleProcessEntries_;
  for ( std::vector<pdfSingleProcessEntryType*>::const_iterator pdfSingleProcessEntry = pdfSingleProcessEntries.begin();
	pdfSingleProcessEntry != pdfSingleProcessEntries.end(); ++pdfSingleProcessEntry ) {
    std::string componentName = (*pdfSingleProcessEntry)->pdfSingleProcess_->GetName();
    pdfProcessSumEntry_->pdfProcessSum_->plotOn(plotFrame, RooFit::Components(componentName.data()), 
						RooFit::LineColor((*pdfSingleProcessEntry)->lineColor_), 
						RooFit::LineStyle((*pdfSingleProcessEntry)->lineStyle_), 
						RooFit::LineWidth((*pdfSingleProcessEntry)->lineWidth_));
  }
  pdfProcessSumEntry_->pdfProcessSum_->plotOn(plotFrame, RooFit::LineColor(kBlack), RooFit::LineStyle(kSolid), RooFit::LineWidth(2));
  plotFrame->SetXTitle(variableTitle.data());
  //plotFrame->SetTitleOffset(1.2, "X");
  plotFrame->SetYTitle("");
  //plotFrame->SetTitleOffset(1.2, "Y");
  plotFrame->Draw();
  
  canvas.Update();
  
  canvas.Print(outputFileName.data());
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenMatrixFit::makeScaleFactors()
{
//--- check that dimensionality of point specifying signal region
//    matches number of observables
  if ( scaleFactorSignalRegion_.size() != numVar_ ) {
    edm::LogError ("makeScaleFactors") << " Dimensionality of point specifying signal region = " << scaleFactorSignalRegion_.size()
				       << " does not match Number of observables = " << numVar_ << " --> skipping !!";
    return;
  }

//--- check that point specifying signal point
//    is within defined bin-grid
  unsigned numRegions = TMath::Nint(TMath::Power(2, numVar_));
  unsigned iRegion = dataBinGrid_->binNumber(scaleFactorSignalRegion_);
  if ( !(iRegion >= 1 && iRegion <= numRegions) ) {
    edm::LogError ("makeScaleFactors") << " Point specifying signal region not within defined bin-grid --> skipping !!";
    return;
  }

//--- open output file
  std::ostream* outputFile = new std::ofstream(scaleFactorFileName_.data(), std::ios::out);

  std::vector<pdfSingleProcessEntryType*>& pdfSingleProcessEntries = pdfProcessSumEntry_->pdfSingleProcessEntries_;
  for ( std::vector<pdfSingleProcessEntryType*>::const_iterator pdfSingleProcessEntry = pdfSingleProcessEntries.begin(); 
	pdfSingleProcessEntry != pdfSingleProcessEntries.end(); ++pdfSingleProcessEntry ) {

//--- compute normalization constant
    double processContribution = (*pdfSingleProcessEntry)->norm_->getVal();
    std::vector<model1dEntryType*>& model1dEntries = (*pdfSingleProcessEntry)->model1dEntries_;
    for ( unsigned iVar = 0; iVar < numVar_; ++iVar ) {
      unsigned bitValue = TMath::Nint(TMath::Power(2, iVar));

      const Double_t* binBoundaries = model1dEntries[iVar]->pdf1d_->getBinBoundaries();
      double prob1, prob2;
      getProbability(model1dEntries[iVar]->prob1_->getVal(), model1dEntries[iVar]->prob2_->getVal(), binBoundaries, prob1, prob2);

      if ( iRegion & bitValue ) 
	processContribution *= prob1;
      else 
	processContribution *= prob2;
    }

//--- write normalization constant for process into output file
    (*outputFile) << (*pdfSingleProcessEntry)->name_ << ".normalization = cms.double(" 
		  << std::setprecision(3) << std::fixed << processContribution << ")" << std::endl;
  }

//--- close output file
  delete outputFile;
}
 */

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GenMatrixFit);
