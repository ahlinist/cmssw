#include "TauAnalysis/BgEstimationTools/plugins/KineEventReweightExtractor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TFile.h>

KineEventReweightExtractor::KineEventReweightExtractor(const edm::ParameterSet& cfg)
  : kineReweightLookupTable_(0),
    kineVarExtractor_(0),
    error_(0)
{
  //std::cout << "<KineEventReweightExtractor::KineEventReweightExtractor>:" << std::endl; 

  edm::ParameterSet cfgWeightLookupTable = cfg.getParameter<edm::ParameterSet>("weightLookupTable");
  fileName_ = cfgWeightLookupTable.getParameter<std::string>("fileName");
  meName_ = cfgWeightLookupTable.getParameter<std::string>("meName");
  
  TFile* inputFile = TFile::Open(fileName_.data());
  TH1* weightLookupTable = (TH1*)inputFile->Get(meName_.data());
  if ( !weightLookupTable ) {
    edm::LogError ("KineEventReweightExtractor") << " Failed to retrieve look-up Table = " << meName_
						 << " from input File = " << fileName_ << " !!";
    error_ = 1;
  }

  if ( weightLookupTable ) kineReweightLookupTable_ = (TH1*)weightLookupTable->Clone();

  inputFile->Close();
  delete inputFile;

  edm::ParameterSet cfgKineVarExtractor = cfg.getParameter<edm::ParameterSet>("kineVarExtractor");
  std::string pluginTypeKineVarExtractor = cfgKineVarExtractor.getParameter<std::string>("pluginType");

  kineVarExtractor_ = ObjValExtractorPluginFactory::get()->create(pluginTypeKineVarExtractor, cfgKineVarExtractor);
}

KineEventReweightExtractor::~KineEventReweightExtractor()
{
  delete kineReweightLookupTable_;

  delete kineVarExtractor_;
}

double KineEventReweightExtractor::operator()(const edm::Event& evt) const
{
  if ( error_ ) {
    edm::LogError ("operator()") << " Error in Configuration ParameterSet" 
				 << " --> kinematic Reweights will NOT be extracted !!";
    return 0.;
  }

  double kineVarValue = (*kineVarExtractor_)(evt);

  int binIndex = kineReweightLookupTable_->FindBin(kineVarValue);

  double kineEventReweight = kineReweightLookupTable_->GetBinContent(binIndex);

  return kineEventReweight;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, KineEventReweightExtractor, "KineEventReweightExtractor");
