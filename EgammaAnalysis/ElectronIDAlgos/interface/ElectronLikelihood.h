// -*- C++ -*-
//-----------------------------------------------------------------------
//
// Package:    
//      EgammaAnalysis/ElectronIDAlgos
// Description:
//      Class ElectronLikelihood
//      class defining the eID selector (likelihood ratio)
//      specialized likelihoods for: barrel/endcap, <15 GeV/>15 GeV 
//      
// Original Authors:  Emanuele Di Marco, 
//                    Pietro Govoni,
//                    Paolo Meridiani
//                    Chiara Ilaria Rovelli, 
// Universita' di Roma "La Sapienza" and INFN Roma
// Universita' di Milano "Bicocca" and INFN Milano
//
// Created:  Fri Jun  1 13:37:18 CEST 2007
//
//-----------------------------------------------------------------------

#ifndef ElectronLikelihood_H
#define ElectronLikelihood_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodSwitches.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodPdfProduct.h"
#include "CondFormats/DataRecord/interface/ElectronLikelihoodRcd.h"
#include "CondFormats/EgammaObjects/interface/ElectronLikelihoodCalibration.h"
#include "FWCore/Framework/interface/data_default_record_trait.h"
#include <TDirectory.h>
#include <vector>


class ElectronLikelihood : ElectronIDAlgo {

 public:
  
  //! ctor, not used for this algo (need initialization from ES)
  ElectronLikelihood () {} ;

  //! ctor
  ElectronLikelihood (const ElectronLikelihoodCalibration *calibration,
		      const std::vector<double> & fisherEBLt15,
		      const std::vector<double> & fisherEBGt15,
		      const std::vector<double> & fisherEELt15,
		      const std::vector<double> & fisherEEGt15,
		      const std::vector<double> & eleFracsEBlt15,
		      const std::vector<double> & piFracsEBlt15,
		      const std::vector<double> & eleFracsEElt15,
		      const std::vector<double> & piFracsEElt15,
		      const std::vector<double> & eleFracsEBgt15,
		      const std::vector<double> & piFracsEBgt15,
		      const std::vector<double> & eleFracsEEgt15,
		      const std::vector<double> & piFracsEEgt15,
		      double eleWeight,
		      double piWeight,
		      LikelihoodSwitches eleIDSwitches,
		      std::string signalWeightSplitting,
		      std::string backgroundWeightSplitting,
		      bool splitSignalPdfs,
		      bool splitBackgroundPdfs) ;

  //! dtor
  virtual ~ElectronLikelihood () ;

  //! not used for this algo
  void setup (const edm::ParameterSet& conf) {} ;

  //! get the result of the algorithm
  float result (const reco::GsfElectron &electron, 
                const reco::ClusterShape &sClShape) const ;

 private:

  //! build the likelihood model from histograms 
  //! in Barrel file and Endcap file
  void Setup (const ElectronLikelihoodCalibration *calibration,
	      const std::vector<double> & fisherEBLt15,
	      const std::vector<double> & fisherEBGt15,
	      const std::vector<double> & fisherEELt15,
	      const std::vector<double> & fisherEEGt15,
	      const std::vector<double> & eleFracsEBlt15,
	      const std::vector<double> & piFracsEBlt15,
	      const std::vector<double> & eleFracsEElt15,
	      const std::vector<double> & piFracsEElt15,
	      const std::vector<double> & eleFracsEBgt15,
	      const std::vector<double> & piFracsEBgt15,
	      const std::vector<double> & eleFracsEEgt15,
	      const std::vector<double> & piFracsEEgt15,  
              double eleWeight,
              double piWeight,
	      std::string signalWeightSplitting,
	      std::string backgroundWeightSplitting,
	      bool splitSignalPdfs,
	      bool splitBackgroundPdfs) ;


  //! get the input variables from the electron and the e-Setup
  void getInputVar (const reco::GsfElectron &electron, 
                    std::vector<float> &measuremnts, 
                    const reco::ClusterShape &sClShape) const ;

  //! evaluate the shape Fisher discriminant
  double CalculateFisher(const reco::GsfElectron &electron,
			 const reco::ClusterShape &sClShape) const;
  
  //! likelihood below 15GeV/c
  LikelihoodPdfProduct *_EBlt15lh, *_EElt15lh;
  //! likelihood above 15GeV/c
  LikelihoodPdfProduct *_EBgt15lh, *_EEgt15lh;

  //! fisher coefficients
  std::vector<double> m_fisherEBLt15, m_fisherEBGt15, m_fisherEELt15, m_fisherEEGt15;

  //! general parameters of all the ele id algorithms
  LikelihoodSwitches m_eleIDSwitches ;

  //! splitting rule for PDF's
  std::string m_signalWeightSplitting;
  std::string m_backgroundWeightSplitting;
  bool m_splitSignalPdfs;
  bool m_splitBackgroundPdfs;

};


EVENTSETUP_DATA_DEFAULT_RECORD (ElectronLikelihood, ElectronLikelihoodRcd)

#endif // ElectronLikelihood_H
