#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/EventSetupRecordKey.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronLikelihood.h"
#include "PhysicsTools/StatPatternRecognition/interface/SprClassifierReader.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTrainedFisher.hh"
#include <iostream>


ElectronLikelihood::ElectronLikelihood (const ElectronLikelihoodCalibration *calibration,
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
					LikelihoodSwitches eleIDSwitches,
					std::string signalWeightSplitting,
					std::string backgroundWeightSplitting,
					bool splitSignalPdfs,
					bool splitBackgroundPdfs) :
  _EBlt15lh (new LikelihoodPdfProduct ("electronID_EB_ptLt15_likelihood",0,0)) ,
  _EElt15lh (new LikelihoodPdfProduct ("electronID_EE_ptLt15_likelihood",1,0)) ,
  _EBgt15lh (new LikelihoodPdfProduct ("electronID_EB_ptGt15_likelihood",0,1)) ,
  _EEgt15lh (new LikelihoodPdfProduct ("electronID_EE_ptGt15_likelihood",1,1)) ,
  m_eleIDSwitches (eleIDSwitches) ,
  m_signalWeightSplitting (signalWeightSplitting), 
  m_backgroundWeightSplitting (backgroundWeightSplitting),
  m_splitSignalPdfs (splitSignalPdfs), 
  m_splitBackgroundPdfs (splitBackgroundPdfs)  
{
  Setup (calibration,
	 fisherEBFileName, fisherEEFileName,
	 eleFracsEBlt15, piFracsEBlt15,
	 eleFracsEElt15, piFracsEElt15,
	 eleFracsEBgt15, piFracsEBgt15,
	 eleFracsEEgt15, piFracsEEgt15,
	 eleWeight, piWeight,
	 signalWeightSplitting, backgroundWeightSplitting,
	 splitSignalPdfs, splitBackgroundPdfs) ;
}



// --------------------------------------------------------



ElectronLikelihood::~ElectronLikelihood () {
  delete _EBlt15lh ;
  delete _EElt15lh ;
  delete _EBgt15lh ;
  delete _EEgt15lh ;
}



// --------------------------------------------------------


void 
ElectronLikelihood::Setup (const ElectronLikelihoodCalibration *calibration,
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
			   std::string signalWeightSplitting,
			   std::string backgroundWeightSplitting,
			   bool splitSignalPdfs,
			   bool splitBackgroundPdfs) 
{

  // Set up the Fisher coefficients
  m_fisherEBFileName = fisherEBFileName;
  m_fisherEEFileName = fisherEEFileName;

  // ECAL BARREL LIKELIHOOD - Pt < 15 GeV region
  _EBlt15lh->initFromDB (calibration) ;

  _EBlt15lh->addSpecies ("electrons",eleWeight) ;
  _EBlt15lh->addSpecies ("hadrons",piWeight) ;

  if(signalWeightSplitting.compare("fullclass")==0) {
    _EBlt15lh->setSplitFrac ("electrons", "fullclass0", eleFracsEBlt15[0]) ;
    _EBlt15lh->setSplitFrac ("electrons", "fullclass1", eleFracsEBlt15[1]) ;
    _EBlt15lh->setSplitFrac ("electrons", "fullclass2", eleFracsEBlt15[2]) ;
    _EBlt15lh->setSplitFrac ("electrons", "fullclass3", eleFracsEBlt15[3]) ;
  }
  else if(signalWeightSplitting.compare("class")==0) {
    _EBlt15lh->setSplitFrac ("electrons", "class0", eleFracsEBlt15[0]+eleFracsEBlt15[1]+eleFracsEBlt15[2]) ;
    _EBlt15lh->setSplitFrac ("electrons", "class1", eleFracsEBlt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBlt15lh->addPdf ("electrons", "dPhiVtx",     splitSignalPdfs) ;   
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBlt15lh->addPdf ("electrons", "dEtaCalo",    splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBlt15lh->addPdf ("electrons", "EoPout",      splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBlt15lh->addPdf ("electrons", "HoE",         splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBlt15lh->addPdf ("electrons", "shapeFisher", splitSignalPdfs) ;

  if(backgroundWeightSplitting.compare("fullclass")==0) {
    _EBlt15lh->setSplitFrac ("hadrons", "fullclass0", piFracsEBlt15[0]) ;
    _EBlt15lh->setSplitFrac ("hadrons", "fullclass1", piFracsEBlt15[1]) ;
    _EBlt15lh->setSplitFrac ("hadrons", "fullclass2", piFracsEBlt15[2]) ;
    _EBlt15lh->setSplitFrac ("hadrons", "fullclass3", piFracsEBlt15[3]) ;
  }
  else if(backgroundWeightSplitting.compare("class")==0) {
    _EBlt15lh->setSplitFrac ("hadrons", "class0", piFracsEBlt15[0]+piFracsEBlt15[1]+piFracsEBlt15[2]) ;
    _EBlt15lh->setSplitFrac ("hadrons", "class1", piFracsEBlt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBlt15lh->addPdf ("hadrons", "dPhiVtx",       splitBackgroundPdfs) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBlt15lh->addPdf ("hadrons", "dEtaCalo",      splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBlt15lh->addPdf ("hadrons", "EoPout",        splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBlt15lh->addPdf ("hadrons", "HoE",           splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBlt15lh->addPdf ("hadrons", "shapeFisher",   splitBackgroundPdfs) ;


  // ECAL BARREL LIKELIHOOD - Pt >= 15 GeV region
  _EBgt15lh->initFromDB (calibration) ;

  _EBgt15lh->addSpecies ("electrons",eleWeight) ;  
  _EBgt15lh->addSpecies ("hadrons",piWeight) ;

  if(signalWeightSplitting.compare("fullclass")==0) {
    _EBgt15lh->setSplitFrac ("electrons", "fullclass0", eleFracsEBgt15[0]) ;
    _EBgt15lh->setSplitFrac ("electrons", "fullclass1", eleFracsEBgt15[1]) ;
    _EBgt15lh->setSplitFrac ("electrons", "fullclass2", eleFracsEBgt15[2]) ;
    _EBgt15lh->setSplitFrac ("electrons", "fullclass3", eleFracsEBgt15[3]) ;
  }
  else if(signalWeightSplitting.compare("class")==0) {
    _EBgt15lh->setSplitFrac ("electrons", "class0", eleFracsEBgt15[0]+eleFracsEBgt15[1]+eleFracsEBgt15[2]) ;
    _EBgt15lh->setSplitFrac ("electrons", "class1", eleFracsEBgt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBgt15lh->addPdf ("electrons", "dPhiVtx",     splitSignalPdfs) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBgt15lh->addPdf ("electrons", "dEtaCalo",    splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBgt15lh->addPdf ("electrons", "EoPout",      splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBgt15lh->addPdf ("electrons", "HoE",         splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBgt15lh->addPdf ("electrons", "shapeFisher", splitSignalPdfs) ;

  if(backgroundWeightSplitting.compare("fullclass")==0) {
    _EBgt15lh->setSplitFrac ("hadrons", "fullclass0", piFracsEBgt15[0]) ;
    _EBgt15lh->setSplitFrac ("hadrons", "fullclass1", piFracsEBgt15[1]) ;
    _EBgt15lh->setSplitFrac ("hadrons", "fullclass2", piFracsEBgt15[2]) ;
    _EBgt15lh->setSplitFrac ("hadrons", "fullclass3", piFracsEBgt15[3]) ;
  }
  else if(backgroundWeightSplitting.compare("class")==0) {
    _EBgt15lh->setSplitFrac ("hadrons", "class0", piFracsEBgt15[0]+piFracsEBgt15[1]+ piFracsEBgt15[2]) ;
    _EBgt15lh->setSplitFrac ("hadrons", "class1", piFracsEBgt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBgt15lh->addPdf ("hadrons", "dPhiVtx",       splitBackgroundPdfs) ; 
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBgt15lh->addPdf ("hadrons", "dEtaCalo",      splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBgt15lh->addPdf ("hadrons", "EoPout",        splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBgt15lh->addPdf ("hadrons", "HoE",           splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBgt15lh->addPdf ("hadrons", "shapeFisher",   splitBackgroundPdfs) ;


  // ECAL ENDCAP LIKELIHOOD - Pt < 15 GeV
  _EElt15lh->initFromDB (calibration) ;

  _EElt15lh->addSpecies ("electrons",eleWeight) ;
  _EElt15lh->addSpecies ("hadrons",piWeight) ;

  if(signalWeightSplitting.compare("fullclass")==0) {
    _EElt15lh->setSplitFrac ("electrons", "fullclass0", eleFracsEElt15[0]) ;
    _EElt15lh->setSplitFrac ("electrons", "fullclass1", eleFracsEElt15[1]) ;
    _EElt15lh->setSplitFrac ("electrons", "fullclass2", eleFracsEElt15[2]) ;
    _EElt15lh->setSplitFrac ("electrons", "fullclass3", eleFracsEElt15[3]) ;
  }
  else if(signalWeightSplitting.compare("class")==0) {
    _EElt15lh->setSplitFrac ("electrons", "class0", eleFracsEElt15[0]+eleFracsEElt15[1]+eleFracsEElt15[2]) ;
    _EElt15lh->setSplitFrac ("electrons", "class1", eleFracsEElt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EElt15lh->addPdf ("electrons", "dPhiVtx",     splitSignalPdfs) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EElt15lh->addPdf ("electrons", "dEtaCalo",    splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EElt15lh->addPdf ("electrons", "EoPout",      splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EElt15lh->addPdf ("electrons", "HoE",         splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EElt15lh->addPdf ("electrons", "shapeFisher", splitSignalPdfs) ;

  if(backgroundWeightSplitting.compare("fullclass")==0) {
    _EElt15lh->setSplitFrac ("hadrons", "fullclass0", piFracsEElt15[0]) ;
    _EElt15lh->setSplitFrac ("hadrons", "fullclass1", piFracsEElt15[1]) ;
    _EElt15lh->setSplitFrac ("hadrons", "fullclass2", piFracsEElt15[2]) ;
    _EElt15lh->setSplitFrac ("hadrons", "fullclass3", piFracsEElt15[3]) ;
  }
  else if(backgroundWeightSplitting.compare("class")==0) {
    _EElt15lh->setSplitFrac ("hadrons", "class0", piFracsEElt15[0]+piFracsEElt15[1]+piFracsEElt15[2]) ;
    _EElt15lh->setSplitFrac ("hadrons", "class1", piFracsEElt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }


  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EElt15lh->addPdf ("hadrons", "dPhiVtx",     splitBackgroundPdfs) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EElt15lh->addPdf ("hadrons", "dEtaCalo",    splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EElt15lh->addPdf ("hadrons", "EoPout",      splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EElt15lh->addPdf ("hadrons", "HoE",         splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EElt15lh->addPdf ("hadrons", "shapeFisher", splitBackgroundPdfs) ;


  // ECAL ENDCAP LIKELIHOOD - Pt >= 15 GeV
  _EEgt15lh->initFromDB (calibration) ;

  _EEgt15lh->addSpecies ("electrons",eleWeight) ;
  _EEgt15lh->addSpecies ("hadrons",piWeight) ;

  if(signalWeightSplitting.compare("fullclass")==0) {
    _EEgt15lh->setSplitFrac ("electrons", "fullclass0", eleFracsEEgt15[0]) ;
    _EEgt15lh->setSplitFrac ("electrons", "fullclass1", eleFracsEEgt15[1]) ;
    _EEgt15lh->setSplitFrac ("electrons", "fullclass2", eleFracsEEgt15[2]) ;
    _EEgt15lh->setSplitFrac ("electrons", "fullclass3", eleFracsEEgt15[3]) ;
  }
  else if(signalWeightSplitting.compare("class")==0) {
    _EEgt15lh->setSplitFrac ("electrons", "class0", eleFracsEEgt15[0]+eleFracsEEgt15[1]+eleFracsEEgt15[2]) ;
    _EEgt15lh->setSplitFrac ("electrons", "class1", eleFracsEEgt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EEgt15lh->addPdf ("electrons", "dPhiVtx",     splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EEgt15lh->addPdf ("electrons", "dEtaCalo",    splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EEgt15lh->addPdf ("electrons", "EoPout",      splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EEgt15lh->addPdf ("electrons", "HoE",         splitSignalPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EEgt15lh->addPdf ("electrons", "shapeFisher", splitSignalPdfs) ;

  if(backgroundWeightSplitting.compare("fullclass")==0) {
    _EEgt15lh->setSplitFrac ("hadrons", "fullclass0", piFracsEEgt15[0]) ;
    _EEgt15lh->setSplitFrac ("hadrons", "fullclass1", piFracsEEgt15[1]) ;
    _EEgt15lh->setSplitFrac ("hadrons", "fullclass2", piFracsEEgt15[2]) ;
    _EEgt15lh->setSplitFrac ("hadrons", "fullclass3", piFracsEEgt15[3]) ;
  }
  else if(backgroundWeightSplitting.compare("class")==0) {
    _EEgt15lh->setSplitFrac ("hadrons", "class0", piFracsEEgt15[0]+piFracsEEgt15[1]+piFracsEEgt15[2]) ;
    _EEgt15lh->setSplitFrac ("hadrons", "class1", piFracsEEgt15[3]) ;
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EEgt15lh->addPdf ("hadrons", "dPhiVtx",     splitBackgroundPdfs) ; 
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EEgt15lh->addPdf ("hadrons", "dEtaCalo",    splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EEgt15lh->addPdf ("hadrons", "EoPout",      splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useHoverE)       _EEgt15lh->addPdf ("hadrons", "HoE",         splitBackgroundPdfs) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EEgt15lh->addPdf ("hadrons", "shapeFisher", splitBackgroundPdfs) ;
}



// --------------------------------------------------------



void 
ElectronLikelihood::getInputVar (const PixelMatchGsfElectron &electron, 
                                 std::vector<float> &measurements, 
                                 const ClusterShape &sClShape) const 
{

  // the variables entering the likelihood
  measurements.push_back ( electron.deltaPhiSuperClusterTrackAtVtx () ) ;
  measurements.push_back ( electron.deltaEtaSeedClusterTrackAtCalo () ) ;
  measurements.push_back ( electron.eSeedClusterOverPout () ) ;
  measurements.push_back ( electron.hadronicOverEm () ) ;
  measurements.push_back ( CalculateFisher(electron, sClShape) ) ;

}



// --------------------------------------------------------



double 
ElectronLikelihood::CalculateFisher(const PixelMatchGsfElectron &electron,
				    const ClusterShape& sClShape) const
{

  // the variables entering the shape fisher
  double s9s25, sigmaEtaEta, lat, a20;

  s9s25=sClShape.e3x3 ()/sClShape.e5x5 () ;
  sigmaEtaEta=sqrt (sClShape.covEtaEta ()) ;
  lat=sClShape.lat () ;
  a20=sClShape.zernike20 () ;


  vector<double> inputs;
  inputs.push_back(s9s25);
  inputs.push_back(sigmaEtaEta);
  inputs.push_back(lat);
  inputs.push_back(a20);

  // evaluate the Fisher discriminant
  double clusterShapeFisher;
  std::vector<DetId> vecId=electron.superCluster()->getHitsByDetId () ;
  EcalSubdetector subdet = EcalSubdetector (vecId[0].subdetId ()) ;
  
  if (subdet==EcalBarrel) {
    SprTrainedFisher *FisherEB = 
      SprClassifierReader::readTrained<SprTrainedFisher>(m_fisherEBFileName.fullPath().c_str(), "Fisher");
    FisherEB->useStandard();
    clusterShapeFisher = FisherEB->response(inputs);
  }
  else if (subdet==EcalEndcap) {
    SprTrainedFisher *FisherEE = 
      SprClassifierReader::readTrained<SprTrainedFisher>(m_fisherEEFileName.fullPath().c_str(), "Fisher");
    FisherEE->useStandard();
    clusterShapeFisher = FisherEE->response(inputs);
  }
  else {
    clusterShapeFisher = -999 ;
    edm::LogWarning ("ElectronLikelihood") << "Undefined electron, eta = " << electron.eta () << "!" ;
  }
  return clusterShapeFisher;
}



// --------------------------------------------------------



float 
ElectronLikelihood::result (const PixelMatchGsfElectron &electron, 
                            const ClusterShape &sClShape) const 
{

  //=======================================================
  // used classification:
  // golden                     =>  0
  // big brem                   => 10
  // narrow                     => 20
  // showering nbrem 0          => 30
  // showering nbrem 1          => 31
  // showering nbrem 2          => 32
  // showering nbrem 3          => 33
  // showering nbrem 4 ou plus  => 34
  // cracks                     => 40
  // endcap                     => barrel + 100
  //=======================================================

  std::vector<float> measurements ;
  getInputVar (electron, measurements, sClShape) ;

  // Split using only the 10^1 bit (golden/big brem/narrow/showering)
  int bitVal=-1 ;
  int gsfclass=electron.classification () ;
  if (gsfclass<99) 
    bitVal=int (gsfclass)/10 ; 
  else
    bitVal=int (int (gsfclass)%100)/10 ;
  
  // temporary: crack electrons goes into Class3 (showering)
  if (bitVal==4) bitVal=3 ;
  if (bitVal<0 || bitVal>3)
    throw cms::Exception ("ElectronLikelihood") << "ERROR! electron class " << gsfclass << " is not accepted!\n" ;

  char className[20] ;
  if(m_signalWeightSplitting.compare("fullclass")==0) {
    sprintf (className,"fullclass%d",bitVal) ;
  }
  else if(m_signalWeightSplitting.compare("class")==0) {
    int classVal = (bitVal<3) ? 0 : 1;
    sprintf (className,"class%d",classVal);
  }
  else {
    throw cms::Exception("BadConfig") << "Only class (non-showering / showering)"
				      << " and fullclass (golden / bigbrem / narrow / showering)" 
				      << " splitting is implemented right now";
  }

  SuperClusterRef sclusRef = electron.superCluster() ;
  std::vector<DetId> vecId=sclusRef->getHitsByDetId () ;
  EcalSubdetector subdet = EcalSubdetector (vecId[0].subdetId ()) ;
  float thisPt =  electron.pt();

  if (subdet==EcalBarrel && thisPt<15.)
    return _EBlt15lh->getRatio ("electrons",measurements,std::string (className)) ;
  else if (subdet==EcalBarrel && thisPt>=15.)
    return _EBgt15lh->getRatio ("electrons",measurements,std::string (className)) ;
  else if (subdet==EcalEndcap && thisPt<15.)
    return _EElt15lh->getRatio ("electrons",measurements,std::string (className)) ;
  else if (subdet==EcalEndcap && thisPt>=15.)
    return _EEgt15lh->getRatio ("electrons",measurements,std::string (className)) ;
  else return -999. ;
}

