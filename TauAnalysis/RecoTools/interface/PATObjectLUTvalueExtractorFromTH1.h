#ifndef TauAnalysis_RecoTools_PATObjectLUTvalueExtractorFromTH1_h
#define TauAnalysis_RecoTools_PATObjectLUTvalueExtractorFromTH1_h

/** \class PATObjectLUTvalueExtractorFromTH1
 *
 * Extract efficiency/fake-rate values for pat::Electron, pat::Muon and pat::Tau objects
 *
 * NOTE: The values to be extracted need to be stored in a N-dimensional histogram
 *      (of type derived from TH1/TH2/TH3, depending on dimensionality)
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATObjectLUTvalueExtractorFromTH1.h,v 1.1 2011/10/21 16:24:11 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/RecoTools/interface/PATObjectLUTvalueExtractorBase.h"

#include <TFile.h>
#include <TH1.h>

#include <vector>
#include <string>

namespace PATObjectLUTvalueExtractorFromTH1_namespace
{
  int getBin(TAxis* axis, double objX)
  {
    int bin = axis->FindFixBin(objX);
    int nBins = axis->GetNbins();
    if ( bin < 1     ) bin = 1;
    if ( bin > nBins ) bin = nBins;
    return bin;
  }
}

template<typename T>
class PATObjectLUTvalueExtractorFromTH1 : public PATObjectLUTvalueExtractorBase<T>
{
 public:

  explicit PATObjectLUTvalueExtractorFromTH1(const edm::ParameterSet& cfg)
    : PATObjectLUTvalueExtractorBase<T>(cfg),
      inputFile_(0),
      lut_(0),
      stringFunctionX_(0),
      stringFunctionY_(0),
      stringFunctionZ_(0)
  {
    edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
    std::string lutName = cfg.getParameter<std::string>("lutName");

    if ( inputFileName.isLocal()) {       
      inputFile_ = new TFile(inputFileName.fullPath().data());
      lut_ = (TH1*)inputFile_->Get(lutName.data());
    } else {
      throw cms::Exception("PATObjectLUTvalueExtractorFromTH1") 
	<< " Failed to find file = " << inputFileName << " !!\n";
    }

    edm::ParameterSet cfgParametrization = cfg.getParameter<edm::ParameterSet>("parametrization");

    stringFunctionX_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("x"));
    if ( lut_->GetDimension() >= 2 ) stringFunctionY_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("y"));
    if ( lut_->GetDimension() >= 3 ) stringFunctionZ_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("z"));
  }

  ~PATObjectLUTvalueExtractorFromTH1() 
  {
    delete inputFile_;

    delete stringFunctionX_;
    delete stringFunctionY_;
    delete stringFunctionZ_;
  }

 protected:

  double extract_value(const T& obj) const
  {
    double objX, objY, objZ;
    int binX, binY, binZ;
    switch ( lut_->GetDimension() ) {
    case 1:
      objX = (*stringFunctionX_)(obj);
      binX = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetXaxis(), objX);
      return lut_->GetBinContent(binX);
    case 2:
      objX = (*stringFunctionX_)(obj);
      binX = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetXaxis(), objX);
      objY = (*stringFunctionY_)(obj);
      binY = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetYaxis(), objY);
      return lut_->GetBinContent(binX, binY);
    case 3:
      objX = (*stringFunctionX_)(obj);
      binX = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetXaxis(), objX);
      objY = (*stringFunctionY_)(obj);
      binY = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetYaxis(), objY);
      objZ = (*stringFunctionZ_)(obj);
      binZ = PATObjectLUTvalueExtractorFromTH1_namespace::getBin(lut_->GetZaxis(), objZ);
      return lut_->GetBinContent(binX, binY, binZ);
    } 

    throw cms::Exception("PATObjectLUTvalueExtractorFromTH1") 
      << " Invalid dimensionality = " << lut_->GetDimension() << " of LUT histogram !!\n";
  }

 private:

  TFile* inputFile_;
  TH1* lut_;

  StringObjectFunction<T>* stringFunctionX_;
  StringObjectFunction<T>* stringFunctionY_;
  StringObjectFunction<T>* stringFunctionZ_;
};

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATObjectLUTvalueExtractorFromTH1<pat::Electron> PATElectronLUTvalueExtractorFromTH1;
typedef PATObjectLUTvalueExtractorFromTH1<pat::Muon> PATMuonLUTvalueExtractorFromTH1;
typedef PATObjectLUTvalueExtractorFromTH1<pat::Tau> PATTauLUTvalueExtractorFromTH1;

#endif

