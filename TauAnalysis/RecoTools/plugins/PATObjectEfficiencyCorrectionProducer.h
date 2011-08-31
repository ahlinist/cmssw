#ifndef TauAnalysis_RecoTools_PATObjectEfficiencyCorrectionProducer_h
#define TauAnalysis_RecoTools_PATObjectEfficiencyCorrectionProducer_h

/** \class PATObjectEfficiencyCorrectionProducer
 *
 * Produce downgrade weights to correct for differences in lepton reconstruction, id., 
 * isolation and trigger efficiencies between data and Monte Carlo simulation
 *
 * NOTE:
 *      o weight > 1: efficiency is underestimated by Monte Carlo simulation
 *      o weight = 1: efficiency is well modeled by Monte Carlo simulation
 *      o weight < 1: efficiency is overestimated by Monte Carlo simulation
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATObjectEfficiencyCorrectionProducer.h,v 1.2 2010/12/22 10:03:05 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/CandidateTools/interface/FetchCollection.h"

#include <TFile.h>
#include <TH1.h>

#include <vector>
#include <string>

template<typename T>
class PATObjectEfficiencyCorrectionProducer : public edm::EDProducer 
{
  typedef std::vector<edm::InputTag> vInputTag;

 public:

  explicit PATObjectEfficiencyCorrectionProducer(const edm::ParameterSet& cfg)
    : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
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
      throw cms::Exception("PATObjectEfficiencyCorrectionProducer") 
	<< " Failed to find file = " << inputFileName << " !!";
    }

    edm::ParameterSet cfgParametrization = cfg.getParameter<edm::ParameterSet>("parametrization");
    srcObjects_ = cfgParametrization.getParameter<vInputTag>("src");

    stringFunctionX_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("x"));
    if ( lut_->GetDimension() >= 2 ) stringFunctionY_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("y"));
    if ( lut_->GetDimension() >= 3 ) stringFunctionZ_ = new StringObjectFunction<T>(cfgParametrization.getParameter<std::string>("z"));
    
    noObjectSubstituteValue_ = cfg.getParameter<double>("noObjectSubstituteValue");

    produces<double>("");
  }

  ~PATObjectEfficiencyCorrectionProducer() 
  {
    delete inputFile_;

    delete stringFunctionX_;
    delete stringFunctionY_;
    delete stringFunctionZ_;
  }

  int getBin(TAxis* axis, double objX)
  {
    int bin = axis->FindFixBin(objX);
    int nBins = axis->GetNbins();
    if ( bin < 1     ) bin = 1;
    if ( bin > nBins ) bin = nBins;
    return bin;
  }

  double getEfficiencyCorrection(const T& obj)
  {
    double objX, objY, objZ;
    int binX, binY, binZ;
    switch ( lut_->GetDimension() ) {
    case 1:
      objX = (*stringFunctionX_)(obj);
      binX = getBin(lut_->GetXaxis(), objX);
      return lut_->GetBinContent(binX);
    case 2:
      objX = (*stringFunctionX_)(obj);
      binX = getBin(lut_->GetXaxis(), objX);
      objY = (*stringFunctionY_)(obj);
      binY = getBin(lut_->GetYaxis(), objY);
      return lut_->GetBinContent(binX, binY);
    case 3:
      objX = (*stringFunctionX_)(obj);
      binX = getBin(lut_->GetXaxis(), objX);
      objY = (*stringFunctionY_)(obj);
      binY = getBin(lut_->GetYaxis(), objY);
      objZ = (*stringFunctionZ_)(obj);
      binZ = getBin(lut_->GetZaxis(), objZ);
      return lut_->GetBinContent(binX, binY, binZ);
    } 

    edm::LogError ("getEfficiencyCorrection") 
      << " Invalid dimensionality = " << lut_->GetDimension() << " of LUT histogram !!";
    return 1.;
  }

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    //std::cout << "<PATObjectEfficiencyCorrectionProducer::produce>:" << std::endl;
    //std::cout << " moduleLabel = " << moduleLabel_ << std::endl;

    double weight = 0.;

    typedef edm::View<T> TView;

    bool foundObj = false;
    for ( vInputTag::const_iterator src = srcObjects_.begin();
	  src != srcObjects_.end(); ++src ) {
      edm::Handle<TView> objCollection;
      pf::fetchCollection(objCollection, *src, evt);

      for ( typename TView::const_iterator obj = objCollection->begin();
	    obj != objCollection->end(); ++obj ) {
	weight = getEfficiencyCorrection(*obj);
	foundObj = true;
	break;
      }
    }

    if ( !foundObj ) weight = noObjectSubstituteValue_;
    //std::cout << "--> weight = " << weight << std::endl;
  
    std::auto_ptr<double> weightPtr(new double(weight));
    evt.put(weightPtr);
  }

 private:

  std::string moduleLabel_;

  TFile* inputFile_;
  TH1* lut_;

  vInputTag srcObjects_;

  StringObjectFunction<T>* stringFunctionX_;
  StringObjectFunction<T>* stringFunctionY_;
  StringObjectFunction<T>* stringFunctionZ_;

  double noObjectSubstituteValue_;
};

#endif

