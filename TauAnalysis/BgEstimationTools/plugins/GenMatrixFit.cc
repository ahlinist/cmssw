#include "TauAnalysis/BgEstimationTools/plugins/GenMatrixFit.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/binningAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include <TObjArray.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TArrayD.h>

#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooFormulaVar.h>
#include <RooProdPdf.h>
#include <RooAddPdf.h>
#include <RooArgSet.h>
#include <RooFit.h>
#include <RooBinning.h>
#include <RooGaussian.h>
#include <RooDataHist.h>

#include <iomanip>
#include <fstream>

const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const double maxNorm = 1.e+6;

const double epsilon = 1.e-2;

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

double getProbability(double probDensity, const Double_t* binBoundaries)
{
//--- "probability" values used by RooParametricStepFunction are in fact probability densities,
//    so need to integrate probability density over bin-width 

  //std::cout << "<getProbability>:" << std::endl;
  //std::cout << " probDensity = " << probDensity << std::endl;

  double prob = (binBoundaries[1] - binBoundaries[0])*probDensity;
  //std::cout << " prob = " << prob << std::endl;

  return prob;
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
    applyProbConstraint_(applyProbConstraint),
    pdfProbConstraint_(0),
    meanProbConstraint_(0),
    sigmaProbConstraint_(0)
{
  std::cout << "<model1dEntryType::model1dEntryType>:" << std::endl;
  std::cout << " name = " << name_ << std::endl;
  std::cout << " prob = " << prob << std::endl;

  const Int_t numBins = 2;  
  TArrayD binBoundaries(numBins + 1);
  binBoundaries[0] = objVarEntry->xMin_;
  //std::cout << " binBoundaries[0] = " << binBoundaries[0] << std::endl;
  binBoundaries[1] = objVarEntry->xBoundary_;
  //std::cout << " binBoundaries[1] = " << binBoundaries[1] << std::endl;
  binBoundaries[2] = objVarEntry->xMax_;
  //std::cout << " binBoundaries[2] = " << binBoundaries[2] << std::endl;

//--- "probability" values used by RooParametricStepFunction are in fact probability densities,
//    so need to devide probability by bin-width (and renormalize)
  double probDensity1 = prob/(binBoundaries[1] - binBoundaries[0]);

  std::string probName = std::string(name_).append("_prob");
  std::string probTitle = probName;
  std::cout << "--> creating RooRealVar with name = " << probName << ", start-values:" 
	    << " probability = " << prob << ", prob. density = " << probDensity1 << "..." << std::endl;
  prob_ = new RooRealVar(probName.data(), probTitle.data(), probDensity1, 0., 1./(binBoundaries[1] - binBoundaries[0]));
  
  std::string pdf1dName = std::string(name_).append("_pdf1d");
  std::string pdf1dTitle = pdf1dName;
  TObjArray binProbabilities;
  binProbabilities.Add(prob_);
  std::string pdf1dArgsName = std::string(name_).append("_args");
  RooArgList pdf1dArgs(binProbabilities, pdf1dArgsName.data());
  std::cout << "--> creating RooParametricStepFunction with name = " << pdf1dName.data() << ","
	    << " depending on variables = " << prob_->GetName() << std::endl;
  pdf1d_ = new RooParametricStepFunction(pdf1dName.data(), pdf1dTitle.data(), *objVarEntry->x_, pdf1dArgs, binBoundaries, numBins);

  if ( applyProbConstraint_ ) {
    std::cout << "<model1dType>: constraining prob = " << valueProbConstraint << " +/- " << sigmaProbConstraint << ","
	      << " name = " << name_ << std::endl;
    
    std::string meanProbConstraintName = std::string(name_).append("_meanProbConstraint");
    meanProbConstraint_ = new RooConstVar(meanProbConstraintName.data(), meanProbConstraintName.data(), valueProbConstraint);
    std::string sigmaProbConstraintName = std::string(name_).append("_sigmaProbConstraint");
    sigmaProbConstraint_ = new RooConstVar(sigmaProbConstraintName.data(), sigmaProbConstraintName.data(), sigmaProbConstraint);
    
    std::string pdfProbConstraintName = std::string(name_).append("_pdfProbConstraint");
    pdfProbConstraint_ = new RooGaussian(pdfProbConstraintName.data(), pdfProbConstraintName.data(), 
					 *prob_, *meanProbConstraint_, *sigmaProbConstraint_);
  }
}

GenMatrixFit::model1dEntryType::~model1dEntryType()
{
  //std::cout << "<model1dEntryType::~model1dEntryType>:" << std::endl;

  delete pdf1d_;

  delete prob_;

  delete pdfProbConstraint_;
  delete meanProbConstraint_;
  delete sigmaProbConstraint_;
}

void GenMatrixFit::model1dEntryType::print(std::ostream& stream) const
{
  stream << "<model1dEntryType::print>:" << std::endl;
  stream << " name = " << name_ << std::endl;
  const Double_t* binBoundaries = pdf1d_->getLimits();
  double prob = getProbability(prob_->getVal(), binBoundaries);
  stream << " prob = " << prob << std::endl;
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
    applyNormConstraint_(applyNormConstraint),
    pdfNormConstraint_(0),
    meanNormConstraint_(0),
    sigmaNormConstraint_(0)
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

std::string GenMatrixFit::modelNdEntryType::getRegionTitle(unsigned region, const std::vector<string>& varNames)
{
  unsigned numVar = model1dEntries_.size();

  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));
  if ( region < numRegions ) {
    std::ostringstream regionTitle;
    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      const std::string& varName = varNames[iVar];
      
      unsigned bitValue = TMath::Nint(TMath::Power(2., (double)iVar));
      
      std::string operator_string;
      if ( region & bitValue ) {
	operator_string = "<";
      } else {
	operator_string = ">=";
      }

      regionTitle << varName << " " << operator_string << " " << model1dEntries_[iVar]->pdf1d_->getLimits()[1];
      if ( iVar < (numVar - 1) ) regionTitle << " && "; 
    }

    return regionTitle.str();
  } else {
    edm::LogError ("getRegionTitle") << " Invalid region = " << region << " !!";
    return std::string("undefined");
  }
}
 
std::string GenMatrixFit::modelNdEntryType::getContrTitle(unsigned region)
{
  unsigned numVar = model1dEntries_.size();

  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));
  if ( region < numRegions ) {
    std::ostringstream contrTitle;
    contrTitle << "Norm * ";

    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      
      unsigned bitValue = TMath::Nint(TMath::Power(2., (double)iVar));

      std::ostringstream probDescription;
      if ( region & bitValue ) {
	probDescription << "(1 - P" << (iVar + 1) << ")";
      } else {
	probDescription << "P" << (iVar + 1);
      }

      contrTitle << probDescription.str();
      if ( iVar < (numVar - 1) ) contrTitle << " * "; 
    }

    return contrTitle.str();
  } else {
    edm::LogError ("getContrTitle") << " Invalid region = " << region << " !!";
    assert(0);
  }
}
 
double GenMatrixFit::modelNdEntryType::getContr(unsigned region)
{
  unsigned numVar = model1dEntries_.size();

  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));
  if ( region < numRegions ) {

    double contribution = norm_->getVal();

    for ( unsigned iVar = 0; iVar < numVar; ++iVar ) {
      model1dEntryType* model1dEntry = model1dEntries_[iVar];

      const Double_t* binBoundaries = model1dEntry->pdf1d_->getLimits();
      double prob = getProbability(model1dEntry->prob_->getVal(), binBoundaries);
            
      unsigned bitValue = TMath::Nint(TMath::Power(2., (double)iVar));    
      if ( region & bitValue ) 
	contribution *= prob;
      else 
	contribution *= (1. - prob);
    }
    
    return contribution;
  } else {
    edm::LogError ("getContr") << " Invalid region = " << region << " !!";
    return 0.;
  }
}
 
double GenMatrixFit::modelNdEntryType::getContrErr(unsigned region)
{
//--- not implemented yet
  return 0.;
}

void GenMatrixFit::modelNdEntryType::print(std::ostream& stream) const
{
  stream << "<modelNdEntryType::print>:" << std::endl;
  stream << " name = " << name_ << std::endl;
  stream << " norm = " << norm_->getVal() << std::endl;
  
  for ( std::vector<model1dEntryType*>::const_iterator model1dEntry = model1dEntries_.begin();
	model1dEntry != model1dEntries_.end(); ++model1dEntry ) {
    (*model1dEntry)->print(stream);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

GenMatrixFit::GenMatrixFit(const edm::ParameterSet& cfg)
  : cfg_(cfg),
    moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    cfgError_(0)
{
  //std::cout << "<GenMatrixFit::GenMatrixFit>:" << std::endl;

//--- read values of configuration parameters controlling verbosity print-out
//    generated during fitting
  edm::ParameterSet cfgFit = cfg_.getParameter<edm::ParameterSet>("fit");
  printLevel_ = ( cfgFit.exists("printLevel") ) ? cfgFit.getParameter<int>("printLevel") : 1;
  printWarnings_  = ( cfgFit.exists("printWarnings") ) ? cfgFit.getParameter<bool>("printWarnings") : true;

//--- read configuration parameters specifying options for making control plots
  edm::ParameterSet cfgProcesses = cfg.getParameter<edm::ParameterSet>("processes");
  vstring processNames = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator processName = processNames.begin();
	processName != processNames.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);
    
    edm::ParameterSet cfgDrawOption = cfgProcess.getParameter<edm::ParameterSet>("drawOptions");
    drawOptions_[*processName] = new drawOptionsType(cfgDrawOption);
  }

  edm::ParameterSet cfgControlPlots = cfg_.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("controlPlots");
  controlPlotsFileName_ = cfgControlPlots.getParameter<std::string>("fileName");

//--- read configuration parameters specifying options for saving fit results in DQMStore
  edm::ParameterSet cfgSaveFitResults = cfg_.getParameter<edm::ParameterSet>("output").getParameter<edm::ParameterSet>("fitResults");
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

void GenMatrixFit::initialize()
{
//--- get binning results from DQMStore
  edm::ParameterSet cfgData = cfg_.getParameter<edm::ParameterSet>("data");

  edm::ParameterSet cfgBinningService = cfgData.getParameter<edm::ParameterSet>("binningService");
  std::string binningServicePluginType = cfgBinningService.getParameter<std::string>("pluginType");
  binningService_ = BinningServicePluginFactory::get()->create(binningServicePluginType, cfgBinningService);

  dqmDirectory_data_ = cfgData.getParameter<std::string>("dqmDirectory");

  dataBinningResults_ = binningService_->loadBinningResults(dqmDirectory_data_);
  dataBinGrid_ = dataBinningResults_->binGrid();

  if ( !dataBinGrid_ ) {
    edm::LogError ("GenMatrixFit") << " Failed to retrieve binningResults !!";
    assert(0);
  }

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

    std::string objVarEntryName = std::string(moduleLabel_).append("_").append(varName);
    objVarEntryType* objVarEntry = new objVarEntryType(objVarEntryName, xMin, xBoundary, xMax);
    objVarEntries_[varName] = objVarEntry;
    objVarEntries_vector[iDimension] = objVarEntry;
  }

//--- get model parameters
  edm::ParameterSet cfgProcesses = cfg_.getParameter<edm::ParameterSet>("processes");
  processNames_ = cfgProcesses.getParameterNamesForType<edm::ParameterSet>();

  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {
    edm::ParameterSet cfgProcess = cfgProcesses.getParameter<edm::ParameterSet>(*processName);

    double norm_initial = cfgProcess.getParameter<edm::ParameterSet>("norm").getParameter<double>("initial");

    std::vector<double> prob_initial(numDimensions);
    for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
      std::ostringstream probName;
      probName << "par" << (iDimension + 1);

      prob_initial[iDimension] = cfgProcess.getParameter<edm::ParameterSet>(probName.str()).getParameter<double>("initial");
      if ( prob_initial[iDimension] < epsilon ) {
	edm::LogError ("GenMatrixFit") 
	  << " Initial value = " << prob_initial[iDimension] 
	  << " for probName = " << probName.str() << ", processName = " << (*processName) << " below " << epsilon
	  << " --> setting initial value to " << epsilon << " !!";
	prob_initial[iDimension] = epsilon;
      } else if ( prob_initial[iDimension] > (1. - epsilon) ) {
	edm::LogError ("GenMatrixFit") 
	  << " Initial value = " << prob_initial[iDimension] 
	  << " for probName = " << probName.str() << ", processName = " << (*processName) << " above " << (1. - epsilon)
	  << " --> setting initial value to " << (1. - epsilon) << " !!";
	prob_initial[iDimension] = (1. - epsilon);
      } 

//--- WARNING: pdf does not depend on observables at all,
//             in case probability is exactly equal to 0.50,
//             potentially causing problems with RooFit (?)
//
//            --> avoid this "degenerate" case by adding random number
//                to probability value specified by configuration parameter
      while ( !(prob_initial[iDimension] > epsilon &&
		TMath::Abs(prob_initial[iDimension] - 0.50) > epsilon &&
		prob_initial[iDimension] < (1. - epsilon)) ) {
	prob_initial[iDimension] += 10.*epsilon*getRandom();
      }
    }

    bool applyNormConstraint = false;
    double meanNormConstraint = 0.;
    double sigmaNormConstraint = 0.;

    std::vector<bool> applyProbConstraint(numDimensions);
    std::vector<double> meanProbConstraint(numDimensions);
    std::vector<double> sigmaProbConstraint(numDimensions);

    edm::ParameterSet cfgFit = cfg_.getParameter<edm::ParameterSet>("fit");
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
	  probName << "par" << (iDimension + 1);

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
  }
}

//-----------------------------------------------------------------------------------------------------------------------
// build RooFit structure
//-----------------------------------------------------------------------------------------------------------------------

void GenMatrixFit::buildFitData()
{
//--- build dataset

  std::cout << "<GenMatrixFit::buildFitData>:" << std::endl;

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

  std::cout << "<GenMatrixFit::buildFitModel>:" << std::endl;

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

//--- load binning results from DQMStore;
//    build structure of variables used in fit
  initialize();

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
  std::cout << " Chi2red = " << this->compChi2red() << std::endl;
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

double GenMatrixFit::compChi2red() 
{
  //std::cout << "<compChi2red>:" << std::endl;
  
  double chi2 = 0.;
  int numDoF = 0;

  unsigned numVar = objVarEntries_.size();

  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));

  for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
    std::vector<binResultType> binResults = dataBinningResults_->getBinResults(iRegion);

    double numData = getBinContent(binResults, "rec");
    double numDataErr2 = getBinSumw2(binResults, "rec");

    double numFit = 0.;
    double numFitErr2 = 0.;
    for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	  modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
      numFit += modelNdEntry->second->getContr(iRegion);
      numFitErr2 += TMath::Power(modelNdEntry->second->getContrErr(iRegion), 2.);
    }
    
    double diff = numData - numFit;
    double diffErr2 = numDataErr2 + numFitErr2;
    
    if ( diffErr2 > 0. ) {
      chi2 += (diff*diff/diffErr2);
      ++numDoF;
    }
  }

//--- correct number of degrees of freedom
//    for number of fitted parameters
//    (numVar probability values plus norm per process)
  unsigned numProcesses = modelNdEntries_.size();
  numDoF -= (numProcesses*(numVar + 1));

  //std::cout << " chi2 = " << chi2 << std::endl;
  //std::cout << " numDoF = " << numDoF << std::endl;
  
  if ( numDoF > 0 ) {
    return (chi2/numDoF);
  } else {
    edm::LogWarning ("compChi2red") 
      << " numDoF = " << numDoF << " must not be negative --> returning Chi2red = 1.e+3 !!";
    return 1.e+3;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void getFitParameter(RooAbsReal* par, double& parValue, double& parErrUp, double& parErrDown)
{
  if ( par ) {
    parValue = par->getVal();

    if ( dynamic_cast<const RooRealVar*>(par) ) {
      const RooRealVar* par_derived = dynamic_cast<const RooRealVar*>(par);
      
      if ( par_derived->hasAsymError() ) {
	parErrUp = par_derived->getAsymErrorHi();
	parErrDown = par_derived->getAsymErrorLo();
      } else {
	parErrUp = parErrDown = par_derived->getError();
      }
    } 
  } else {
    edm::LogError ("getFitParameter") << " par = NULL --> setting parameter value and uncertainty to zero !!";
    parValue = parErrUp = parErrDown = 0.;
  }
}

void GenMatrixFit::saveFitResults()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("saveFitResults") << " Failed to access dqmStore --> fit results will NOT be saved !!";
    return;
  }
  
  DQMStore& dqmStore = (*edm::Service<DQMStore>());
  
  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    std::string normName = std::string(modelNdEntry->second->name_).append("_norm");
    double normValue, normErrUp, normErrDown;
    getFitParameter(modelNdEntry->second->norm_, normValue, normErrUp, normErrDown);
    saveFitParameter(dqmStore, dqmDirectory_fitResult_, modelNdEntry->first, normName, normValue, normErrUp, normErrDown);
    
    for ( std::vector<model1dEntryType*>::iterator model1dEntry = modelNdEntry->second->model1dEntries_.begin();
	  model1dEntry != modelNdEntry->second->model1dEntries_.end(); ++model1dEntry ) {
      std::string probName = std::string((*model1dEntry)->name_).append("_prob");

//--- parameter useds to fir RooParametricStepFunction is actually probability density,
//    not probability, so need to multiply by bin-width
      double probDensity, probDensityErrUp, probDensityErrDown;
      getFitParameter((*model1dEntry)->prob_, probDensity, probDensityErrUp, probDensityErrDown);

      const Double_t* binBoundaries = (*model1dEntry)->pdf1d_->getLimits();
      double prob = getProbability(probDensity, binBoundaries);
      double probErrUp = getProbability(probDensityErrUp, binBoundaries);
      double probErrDown = getProbability(probDensityErrDown, binBoundaries);

      saveFitParameter(dqmStore, dqmDirectory_fitResult_, modelNdEntry->first, probName, prob, probErrUp, probErrDown);
    }
  }
}

void GenMatrixFit::makeControlPlots()
{
//--- stop ROOT from opening X-window for canvas output
//    (in order to be able to run in batch mode) 
  gROOT->SetBatch(true);

  TCanvas canvas("GenMatrixFit", "GenMatrixFit", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);

  unsigned numVar = varNames_.size();

  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));

  int numBinsX = numRegions;
  double minX = -0.5;
  double maxX = numRegions - 0.5;
  
  std::string dataHistogramName = std::string(moduleLabel_).append("_dataHistogram");
  std::string histogramTitle = "Num. Events in different Regions";
  TH1* dataHistogram = new TH1D(dataHistogramName.data(), histogramTitle.data(), numBinsX, minX, maxX);
  for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
    std::vector<binResultType> binResults = dataBinningResults_->getBinResults(iRegion);

    double binContent = getBinContent(binResults, "rec");
    double binErr2 = getBinSumw2(binResults, "rec");

    int binIndex = dataHistogram->FindBin(iRegion);

    dataHistogram->SetBinContent(binIndex, binContent);
    dataHistogram->SetBinError(binIndex, TMath::Sqrt(binErr2));
  }

  dataHistogram->SetMarkerColor(1);
  dataHistogram->SetMarkerStyle(8);
  dataHistogram->SetLineColor(1);

  dataHistogram->SetXTitle("Region");
  dataHistogram->SetTitleOffset(1.2, "X");
  dataHistogram->SetYTitle("");
  dataHistogram->SetTitleOffset(1.2, "Y");

  double yMax = dataHistogram->GetMaximum();

  std::map<std::string, TH1*> modelHistograms; // key = processName
  for ( vstring::const_iterator processName = processNames_.begin();
	processName != processNames_.end(); ++processName ) {
    modelNdEntryType* modelNdEntry = modelNdEntries_[*processName];
    
    std::string modelHistogramName = std::string(moduleLabel_).append("_").append(*processName).append("_modelHistogram");
    TH1* modelHistogram = new TH1D(modelHistogramName.data(), histogramTitle.data(), numBinsX, minX, maxX);
    for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
      double binContent = modelNdEntry->getContr(iRegion);
      double binErr = modelNdEntry->getContrErr(iRegion);

      int binIndex = modelHistogram->FindBin(iRegion);
      
      modelHistogram->SetBinContent(binIndex, binContent);
      modelHistogram->SetBinError(binIndex, binErr);
    }

    drawOptionsType* drawOption = drawOptions_[*processName];
    modelHistogram->SetLineColor(drawOption->lineColor_);
    modelHistogram->SetLineStyle(drawOption->lineStyle_);
    modelHistogram->SetLineWidth(drawOption->lineWidth_);
    
    modelHistograms[*processName] = modelHistogram;

    if ( modelHistogram->GetMaximum() > yMax ) yMax = modelHistogram->GetMaximum(); 
  }

  TLegend legend(0.60, 0.64, 0.89, 0.89, "", "brNDC"); 
  legend.SetBorderSize(0);
  legend.SetFillColor(0);

  dataHistogram->SetMaximum(yMax);
  dataHistogram->Draw("e1p");
  legend.AddEntry(dataHistogram, "Data", "p");
  
  for ( std::map<std::string, TH1*>::iterator modelHistogram = modelHistograms.begin();
	modelHistogram != modelHistograms.end(); ++modelHistogram ) {
    modelHistogram->second->SetMaximum(yMax);
    modelHistogram->second->Draw("histsame");
    legend.AddEntry(modelHistogram->second, modelHistogram->first.data(), "l");
  }

  dataHistogram->Draw("e1psame");

  legend.Draw();

  canvas.Update();
  
  canvas.Print(controlPlotsFileName_.data());

  delete dataHistogram;
  for ( std::map<std::string, TH1*>::iterator it = modelHistograms.begin();
	it != modelHistograms.end(); ++it ) {
    delete it->second;
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void GenMatrixFit::print(std::ostream& stream)
{
  stream << "<GenMatrixFit::print>:" << std::endl;

  for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
    modelNdEntry->second->print(stream);
  }

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

      const Double_t* binBoundaries = model1dEntries[iVar]->pdf1d_->getLimits();
      double prob = getProbability(model1dEntries[iVar]->prob_->getVal(), binBoundaries);
      
      stream << "  " << probName.str() << " = " << prob << std::endl;
     }
  }

  stream << "Contributions to Regions:" << std::endl;
  unsigned numRegions = TMath::Nint(TMath::Power(2., (double)numVar));
  for ( unsigned iRegion = 0; iRegion < numRegions; ++iRegion ) {
    if ( modelNdEntries_.begin() != modelNdEntries_.end() ) {
      stream << " " << modelNdEntries_.begin()->second->getRegionTitle(iRegion, varNames_) << std::endl;
      stream << " " << modelNdEntries_.begin()->second->getContrTitle(iRegion) << std::endl;
    }
    
    for ( modelNdEntryMap::iterator modelNdEntry = modelNdEntries_.begin();
	  modelNdEntry != modelNdEntries_.end(); ++modelNdEntry ) {
      stream << "  " << modelNdEntry->first << " = " 
	     << std::setprecision(3) << std::fixed << modelNdEntry->second->getContr(iRegion) << std::endl;
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(GenMatrixFit);
