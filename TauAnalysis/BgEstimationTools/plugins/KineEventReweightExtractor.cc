#include "TauAnalysis/BgEstimationTools/plugins/KineEventReweightExtractor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"

#include <TFile.h>

ObjValExtractorBase* makeExtractorPlugin(const edm::ParameterSet& cfg)
{
  std::string pluginTypeExtractor = cfg.getParameter<std::string>("pluginType");
  ObjValExtractorBase* pluginExtractor = ObjValExtractorPluginFactory::get()->create(pluginTypeExtractor, cfg);
  return pluginExtractor;
}

KineEventReweightExtractor::KineEventReweightExtractor(const edm::ParameterSet& cfg)
  : lut_(0),
    error_(0)
{
  //std::cout << "<KineEventReweightExtractor::KineEventReweightExtractor>:" << std::endl; 

  edm::ParameterSet cfgWeightLookupTable = cfg.getParameter<edm::ParameterSet>("weightLookupTable");

  edm::FileInPath inputFileName = cfgWeightLookupTable.getParameter<edm::FileInPath>("inputFileName");
  lutName_ = cfgWeightLookupTable.getParameter<std::string>("lutName");
  if ( !inputFileName.isLocal()) throw cms::Exception("KineEventReweightExtractor") 
    << " Failed to find File = " << inputFileName << " !!\n";

  TFile* inputFile_ = new TFile(inputFileName.fullPath().data());
  TH1* lut_temp = (TH1*)inputFile_->Get(lutName_.data());
  if ( !lut_temp ) {
    edm::LogError ("KineEventReweightExtractor") 
      << " Failed to retrieve look-up Table = " << lutName_ << " from input File = " << inputFileName << " !!";
    error_ = 1;
  }

  lut_ = (TH1*)lut_temp->Clone(TString(lut_temp->GetName()).Append("_cloned").Data());

  delete inputFile_;

  edm::ParameterSet cfgVariables = cfg.getParameter<edm::ParameterSet>("variables");
  std::vector<std::string> axisLabels;
  axisLabels.push_back("x");
  axisLabels.push_back("y");
  axisLabels.push_back("z");
  unsigned numAxisLabels = axisLabels.size();
  for ( unsigned iAxis = 0; iAxis < numAxisLabels; ++iAxis ) {
    const char* axisLabel = axisLabels[iAxis].data();

    if ( cfgVariables.exists(axisLabel) ) {
      edm::ParameterSet cfgExtractor = cfgVariables.getParameter<edm::ParameterSet>(axisLabel);
      kineVarExtractors_.push_back(makeExtractorPlugin(cfgExtractor));
    } else {
      break;
    }
  }

  if ( kineVarExtractors_.size() == 0 ) kineVarExtractors_.push_back(makeExtractorPlugin(cfgVariables));

  if ( (int)kineVarExtractors_.size() != lut_->GetDimension() ) {
    edm::LogError ("KineEventReweightExtractor") 
      << " Mismatch between dimensionality of weight look-up Table = " << lut_->GetDimension() 
      << " and number of Extractor plugin definitions = " << kineVarExtractors_.size() << " !!";
    error_ = 1;
  }
}

KineEventReweightExtractor::~KineEventReweightExtractor()
{
  for ( std::vector<ObjValExtractorBase*>::iterator it = kineVarExtractors_.begin();
	it != kineVarExtractors_.end(); ++it ) {
    delete (*it);
  }
}

double KineEventReweightExtractor::operator()(const edm::Event& evt) const
{
  //std::cout << "<KineEventReweightExtractor::operator()>:" << std::endl;

  if ( error_ ) {
    edm::LogError ("operator()") 
      << " Error in Configuration ParameterSet --> kinematic Reweights will NOT be extracted !!";
    return 0.;
  }

  unsigned numDimensions = lut_->GetDimension();

  std::vector<double> kineVarValues(numDimensions);
  for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
    kineVarValues[iDimension] = (*kineVarExtractors_[iDimension])(evt);
  }

  int binIndex = -1;
  switch ( numDimensions ) {
    case 1:
      binIndex = lut_->FindBin(kineVarValues[0]);
      break;
    case 2:
      binIndex = lut_->FindBin(kineVarValues[0], kineVarValues[1]);
      break;
    case 3:
      binIndex = lut_->FindBin(kineVarValues[0], kineVarValues[1], kineVarValues[2]);
      break;
  }

  double kineEventReweight = 0.;
  if ( binIndex != -1 ) {
    kineEventReweight = lut_->GetBinContent(binIndex);
  } else {
    edm::LogWarning ("operator()") 
      << " Failed to find kinematic Reweight for observable vector = " << format_vdouble(kineVarValues) << " !!";
  }

  //std::cout << " x = " << kineVarValues[0];
  //if ( kineVarValues.size() >= 2 ) std::cout << ", y = " << kineVarValues[1];
  //if ( kineVarValues.size() >= 3 ) std::cout << ", z = " << kineVarValues[2];
  //std::cout << ": weight = " << kineEventReweight << std::endl;

  return kineEventReweight;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, KineEventReweightExtractor, "KineEventReweightExtractor");
