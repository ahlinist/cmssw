#include "TauAnalysis/BgEstimationTools/plugins/KineEventReweightExtractor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <TFile.h>

ObjValExtractorBase* makeExtractorPlugin(const edm::ParameterSet& cfg)
{
  std::string pluginTypeExtractor = cfg.getParameter<std::string>("pluginType");
  ObjValExtractorBase* pluginExtractor = ObjValExtractorPluginFactory::get()->create(pluginTypeExtractor, cfg);
  return pluginExtractor;
}

KineEventReweightExtractor::KineEventReweightExtractor(const edm::ParameterSet& cfg)
  : kineReweightLookupTable_(0),
    error_(0)
{
  //std::cout << "<KineEventReweightExtractor::KineEventReweightExtractor>:" << std::endl; 

  edm::ParameterSet cfgWeightLookupTable = cfg.getParameter<edm::ParameterSet>("weightLookupTable");
  fileName_ = cfgWeightLookupTable.getParameter<std::string>("fileName");
  //std::cout << " fileName = " << fileName_ << std::endl;
  meName_ = cfgWeightLookupTable.getParameter<std::string>("meName");
  //std::cout << " meName = " << meName_ << std::endl;
  
  TFile* inputFile = TFile::Open(fileName_.data());
  TH1* weightLookupTable = (TH1*)inputFile->Get(meName_.data());
  if ( !weightLookupTable ) {
    edm::LogError ("KineEventReweightExtractor") 
      << " Failed to retrieve look-up Table = " << meName_ << " from input File = " << fileName_ << " !!";
    error_ = 1;
  }

  if ( weightLookupTable ) kineReweightLookupTable_ = (TH1*)weightLookupTable->Clone();

  inputFile->Close();
  delete inputFile;

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

  if ( (int)kineVarExtractors_.size() != weightLookupTable->GetDimension() ) {
    edm::LogError ("KineEventReweightExtractor") 
      << " Mismatch between dimensionality of weight look-up Table = " << weightLookupTable->GetDimension() 
      << " and number of Extractor plugin definitions = " << kineVarExtractors_.size() << " !!";
    error_ = 1;
  }
}

KineEventReweightExtractor::~KineEventReweightExtractor()
{
  delete kineReweightLookupTable_;

  for ( std::vector<ObjValExtractorBase*>::iterator it = kineVarExtractors_.begin();
	it != kineVarExtractors_.end(); ++it ) {
    delete (*it);
  }
}

double KineEventReweightExtractor::operator()(const edm::Event& evt) const
{
  if ( error_ ) {
    edm::LogError ("operator()") 
      << " Error in Configuration ParameterSet --> kinematic Reweights will NOT be extracted !!";
    return 0.;
  }

  unsigned numDimensions = kineReweightLookupTable_->GetDimension();

  std::vector<double> kineVarValues(numDimensions);
  for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
    kineVarValues[iDimension] = (*kineVarExtractors_[iDimension])(evt);
  }

  int binIndex = -1;
  switch ( numDimensions ) {
    case 1:
      binIndex = kineReweightLookupTable_->FindBin(kineVarValues[0]);
      break;
    case 2:
      binIndex = kineReweightLookupTable_->FindBin(kineVarValues[0], kineVarValues[1]);
      break;
    case 3:
      binIndex = kineReweightLookupTable_->FindBin(kineVarValues[0], kineVarValues[1], kineVarValues[2]);
      break;
  }

  double kineEventReweight = 0.;
  if ( binIndex != -1 ) {
    kineEventReweight = kineReweightLookupTable_->GetBinContent(binIndex);
  } else {
    edm::LogWarning ("operator()") 
      << " Failed to find kinematic Reweight for observable vector = " << format_vdouble(kineVarValues) << " !!";
  }

  return kineEventReweight;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, KineEventReweightExtractor, "KineEventReweightExtractor");
