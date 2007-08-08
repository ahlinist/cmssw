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
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronIDAlgo.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodSwitches.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/LikelihoodPdfProduct.h"
#include "CondFormats/DataRecord/interface/ElectronLikelihoodRcd.h"
#include "FWCore/Framework/interface/data_default_record_trait.h"
#include <TDirectory.h>
#include <vector>


class ElectronLikelihood : ElectronIDAlgo {

 public:
  
  //! ctor, not used for this algo (need initialization from ES)
  ElectronLikelihood () {} ;

  //! ctor
  ElectronLikelihood (TDirectory *EBlt15dir, TDirectory *EElt15dir,
		      TDirectory *EBgt15dir, TDirectory *EEgt15dir,
		      edm::FileInPath fisherEBFileName, edm::FileInPath fisherEEFileName,
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
		      LikelihoodSwitches eleIDSwitches) ;

  //! dtor
  virtual ~ElectronLikelihood () ;

  //! not used for this algo
  void setup (const edm::ParameterSet& conf) {} ;

  //! get the result of the algorithm
  float result (const reco::PixelMatchGsfElectron *, 
                const edm::Event&) const ;

 private:

  //! build the likelihood model from histograms 
  //! in Barrel file and Endcap file
  void Setup (TDirectory *EBlt15dir, TDirectory *EElt15dir,
	      TDirectory *EBgt15dir, TDirectory *EEgt15dir,
	      edm::FileInPath fisherEBFileName, edm::FileInPath fisherEEFileName,
	      const std::vector<double> & eleFracsEBlt15,
	      const std::vector<double> & piFracsEBlt15,
	      const std::vector<double> & eleFracsEElt15,
	      const std::vector<double> & piFracsEElt15,
	      const std::vector<double> & eleFracsEBgt15,
	      const std::vector<double> & piFracsEBgt15,
	      const std::vector<double> & eleFracsEEgt15,
	      const std::vector<double> & piFracsEEgt15,  
              double eleWeight,
              double piWeight) ;


  //! get the input variables from the electron and the e-Setup
  void getInputVar (const PixelMatchGsfElectron *electron, 
                    std::vector<float> &measuremnts, 
                    const edm::Event& iEvent) const ;

  //! evaluate the shape Fisher discriminant
  double CalculateFisher(const PixelMatchGsfElectron *electron,
			 const edm::Event& iEvent) const;
  
  //! likelihood below 15GeV/c
  LikelihoodPdfProduct *_EBlt15lh, *_EElt15lh;
  //! likelihood above 15GeV/c
  LikelihoodPdfProduct *_EBgt15lh, *_EEgt15lh;

  //! fisher configuration files
  edm::FileInPath m_fisherEBFileName;
  edm::FileInPath m_fisherEEFileName;

  //! general parameters of all the ele id algorithms
  LikelihoodSwitches m_eleIDSwitches ;
};


EVENTSETUP_DATA_DEFAULT_RECORD (ElectronLikelihood, ElectronLikelihoodRcd)

#endif // ElectronLikelihood_H
