#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "EgammaAnalysis/ElectronIDAlgos/interface/ElectronLikelihood.h"
#include "PhysicsTools/StatPatternRecognition/interface/SprClassifierReader.hh"
#include "PhysicsTools/StatPatternRecognition/interface/SprTrainedFisher.hh"
#include <iostream>


ElectronLikelihood::ElectronLikelihood (TDirectory *EBlt15dir, TDirectory *EElt15dir,
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
					LikelihoodSwitches eleIDSwitches) :
  _EBlt15lh (new LikelihoodPdfProduct ("electronID_EB_ptLt15_likelihood")) ,
  _EElt15lh (new LikelihoodPdfProduct ("electronID_EE_ptLt15_likelihood")) ,
  _EBgt15lh (new LikelihoodPdfProduct ("electronID_EB_ptGt15_likelihood")) ,
  _EEgt15lh (new LikelihoodPdfProduct ("electronID_EE_ptGt15_likelihood")) ,
  m_eleIDSwitches (eleIDSwitches) 
{
  Setup (EBlt15dir, EElt15dir,
	 EBgt15dir, EEgt15dir,
	 fisherEBFileName, fisherEEFileName,
	 eleFracsEBlt15, piFracsEBlt15,
	 eleFracsEElt15, piFracsEElt15,
	 eleFracsEBgt15, piFracsEBgt15,
	 eleFracsEEgt15, piFracsEEgt15,
	 eleWeight, piWeight) ;
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
ElectronLikelihood::Setup (TDirectory *EBlt15dir, TDirectory *EElt15dir,
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
			   double piWeight) 
{
  // Set up the Fisher coefficients
  m_fisherEBFileName = fisherEBFileName;
  m_fisherEEFileName = fisherEEFileName;

  // ECAL BARREL LIKELIHOOD - Pt < 15 GeV region
  _EBlt15lh->initFromFile (EBlt15dir) ;

  _EBlt15lh->addSpecies ("electrons",eleWeight) ;
  _EBlt15lh->addSpecies ("hadrons",piWeight) ;

  _EBlt15lh->setSplitFrac ("electrons", "GsfClass0", eleFracsEBlt15[0]) ;
  _EBlt15lh->setSplitFrac ("electrons", "GsfClass1", eleFracsEBlt15[1]) ;
  _EBlt15lh->setSplitFrac ("electrons", "GsfClass2", eleFracsEBlt15[2]) ;
  _EBlt15lh->setSplitFrac ("electrons", "GsfClass3", eleFracsEBlt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBlt15lh->addPdf ("electrons", "dPhiVtx",     true) ;   
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBlt15lh->addPdf ("electrons", "dEtaCalo",    true) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBlt15lh->addPdf ("electrons", "EoPout",      true) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBlt15lh->addPdf ("electrons", "HoE",         true) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBlt15lh->addPdf ("electrons", "shapeFisher", true) ;

  _EBlt15lh->setSplitFrac ("hadrons", "GsfClass0", piFracsEBlt15[0]) ;
  _EBlt15lh->setSplitFrac ("hadrons", "GsfClass1", piFracsEBlt15[1]) ;
  _EBlt15lh->setSplitFrac ("hadrons", "GsfClass2", piFracsEBlt15[2]) ;
  _EBlt15lh->setSplitFrac ("hadrons", "GsfClass3", piFracsEBlt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBlt15lh->addPdf ("hadrons", "dPhiVtx",       false) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBlt15lh->addPdf ("hadrons", "dEtaCalo",      false) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBlt15lh->addPdf ("hadrons", "EoPout",        false) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBlt15lh->addPdf ("hadrons", "HoE",           false) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBlt15lh->addPdf ("hadrons", "shapeFisher",   false) ;


  // ECAL BARREL LIKELIHOOD - Pt >= 15 GeV region
  _EBgt15lh->initFromFile (EBgt15dir) ;

  _EBgt15lh->addSpecies ("electrons",eleWeight) ;  
  _EBgt15lh->addSpecies ("hadrons",piWeight) ;

  _EBgt15lh->setSplitFrac ("electrons", "GsfClass0", eleFracsEBgt15[0]) ;
  _EBgt15lh->setSplitFrac ("electrons", "GsfClass1", eleFracsEBgt15[1]) ;
  _EBgt15lh->setSplitFrac ("electrons", "GsfClass2", eleFracsEBgt15[2]) ;
  _EBgt15lh->setSplitFrac ("electrons", "GsfClass3", eleFracsEBgt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBgt15lh->addPdf ("electrons", "dPhiVtx",     true) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBgt15lh->addPdf ("electrons", "dEtaCalo",    true) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBgt15lh->addPdf ("electrons", "EoPout",      true) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBgt15lh->addPdf ("electrons", "HoE",         true) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBgt15lh->addPdf ("electrons", "shapeFisher", true) ;

  _EBgt15lh->setSplitFrac ("hadrons", "GsfClass0", piFracsEBgt15[0]) ;
  _EBgt15lh->setSplitFrac ("hadrons", "GsfClass1", piFracsEBgt15[1]) ;
  _EBgt15lh->setSplitFrac ("hadrons", "GsfClass2", piFracsEBgt15[2]) ;
  _EBgt15lh->setSplitFrac ("hadrons", "GsfClass3", piFracsEBgt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EBgt15lh->addPdf ("hadrons", "dPhiVtx",       false) ; 
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EBgt15lh->addPdf ("hadrons", "dEtaCalo",      false) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EBgt15lh->addPdf ("hadrons", "EoPout",        false) ;
  if (m_eleIDSwitches.m_useHoverE)       _EBgt15lh->addPdf ("hadrons", "HoE",           false) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EBgt15lh->addPdf ("hadrons", "shapeFisher",   false) ;


  // ECAL ENDCAP LIKELIHOOD - Pt < 15 GeV
  _EElt15lh->initFromFile (EElt15dir) ;

  _EElt15lh->addSpecies ("electrons",eleWeight) ;
  _EElt15lh->addSpecies ("hadrons",piWeight) ;

  _EElt15lh->setSplitFrac ("electrons", "GsfClass0", eleFracsEElt15[0]) ;
  _EElt15lh->setSplitFrac ("electrons", "GsfClass1", eleFracsEElt15[1]) ;
  _EElt15lh->setSplitFrac ("electrons", "GsfClass2", eleFracsEElt15[2]) ;
  _EElt15lh->setSplitFrac ("electrons", "GsfClass3", eleFracsEElt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EElt15lh->addPdf ("electrons", "dPhiVtx",     true) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EElt15lh->addPdf ("electrons", "dEtaCalo",    true) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EElt15lh->addPdf ("electrons", "EoPout",      true) ;
  if (m_eleIDSwitches.m_useHoverE)       _EElt15lh->addPdf ("electrons", "HoE",         true) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EElt15lh->addPdf ("electrons", "shapeFisher", true) ;

  _EElt15lh->setSplitFrac ("hadrons", "GsfClass0", piFracsEElt15[0]) ;
  _EElt15lh->setSplitFrac ("hadrons", "GsfClass1", piFracsEElt15[1]) ;
  _EElt15lh->setSplitFrac ("hadrons", "GsfClass2", piFracsEElt15[2]) ;
  _EElt15lh->setSplitFrac ("hadrons", "GsfClass3", piFracsEElt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EElt15lh->addPdf ("hadrons", "dPhiVtx",     false) ;  
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EElt15lh->addPdf ("hadrons", "dEtaCalo",    false) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EElt15lh->addPdf ("hadrons", "EoPout",      false) ;
  if (m_eleIDSwitches.m_useHoverE)       _EElt15lh->addPdf ("hadrons", "HoE",         false) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EElt15lh->addPdf ("hadrons", "shapeFisher", false) ;


  // ECAL ENDCAP LIKELIHOOD - Pt >= 15 GeV
  _EEgt15lh->initFromFile (EEgt15dir) ;

  _EEgt15lh->addSpecies ("electrons",eleWeight) ;
  _EEgt15lh->addSpecies ("hadrons",piWeight) ;

  _EEgt15lh->setSplitFrac ("electrons", "GsfClass0", eleFracsEEgt15[0]) ;
  _EEgt15lh->setSplitFrac ("electrons", "GsfClass1", eleFracsEEgt15[1]) ;
  _EEgt15lh->setSplitFrac ("electrons", "GsfClass2", eleFracsEEgt15[2]) ;
  _EEgt15lh->setSplitFrac ("electrons", "GsfClass3", eleFracsEEgt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EEgt15lh->addPdf ("electrons", "dPhiVtx",     true) ;
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EEgt15lh->addPdf ("electrons", "dEtaCalo",    true) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EEgt15lh->addPdf ("electrons", "EoPout",      true) ;
  if (m_eleIDSwitches.m_useHoverE)       _EEgt15lh->addPdf ("electrons", "HoE",         true) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EEgt15lh->addPdf ("electrons", "shapeFisher", true) ;

  _EEgt15lh->setSplitFrac ("hadrons", "GsfClass0", piFracsEEgt15[0]) ;
  _EEgt15lh->setSplitFrac ("hadrons", "GsfClass1", piFracsEEgt15[1]) ;
  _EEgt15lh->setSplitFrac ("hadrons", "GsfClass2", piFracsEEgt15[2]) ;
  _EEgt15lh->setSplitFrac ("hadrons", "GsfClass3", piFracsEEgt15[3]) ;

  if (m_eleIDSwitches.m_useDeltaPhiIn)   _EEgt15lh->addPdf ("hadrons", "dPhiVtx",     false) ; 
  if (m_eleIDSwitches.m_useDeltaEtaCalo) _EEgt15lh->addPdf ("hadrons", "dEtaCalo",    false) ;
  if (m_eleIDSwitches.m_useEoverPOut)    _EEgt15lh->addPdf ("hadrons", "EoPout",      false) ;
  if (m_eleIDSwitches.m_useHoverE)       _EEgt15lh->addPdf ("hadrons", "HoE",         false) ;
  if (m_eleIDSwitches.m_useShapeFisher)  _EEgt15lh->addPdf ("hadrons", "shapeFisher", false) ;
}



// --------------------------------------------------------



void 
ElectronLikelihood::getInputVar (const PixelMatchGsfElectron *electron, 
                                 std::vector<float> &measurements, 
                                 const edm::Event& iEvent) const 
{

  // the variables entering the likelihood
  measurements.push_back ( electron->deltaPhiSuperClusterTrackAtVtx () ) ;
  measurements.push_back ( electron->deltaEtaSeedClusterTrackAtCalo () ) ;
  measurements.push_back ( electron->eSeedClusterOverPout () ) ;
  measurements.push_back ( electron->hadronicOverEm () ) ;
  measurements.push_back ( CalculateFisher(electron, iEvent) ) ;

}



// --------------------------------------------------------



double 
ElectronLikelihood::CalculateFisher(const PixelMatchGsfElectron *electron,
				    const edm::Event& iEvent) const
{

  // the variables entering the shape fisher
  double s9s25, sigmaEtaEta, lat, a20;
  bool hasBarrel=true ;
  bool hasEndcap=true ;

  edm::Handle<BasicClusterShapeAssociationCollection> barrelClShpHandle ;
  try { iEvent.getByLabel ("hybridSuperClusters","hybridShapeAssoc", barrelClShpHandle) ; }
  catch ( cms::Exception& ex ) { edm::LogWarning ("ElectronLikelihood") << "Can't get ECAL barrel Cluster Shape Collection" ; }
  const reco::BasicClusterShapeAssociationCollection& barrelClShpMap = *barrelClShpHandle ;
  
  edm::Handle<BasicClusterShapeAssociationCollection> endcapClShpHandle ;
  try { iEvent.getByLabel ("islandBasicClusters","islandEndcapShapeAssoc", endcapClShpHandle) ; }
  catch ( cms::Exception& ex ) { edm::LogWarning ("ElectronLikelihood") << "Can't get ECAL endcap Cluster Shape Collection" ; }
  const reco::BasicClusterShapeAssociationCollection& endcapClShpMap = *endcapClShpHandle ;

  SuperClusterRef sclusRef = electron->get<SuperClusterRef> () ;
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;
  seedShpItr = barrelClShpMap.find (sclusRef->seed ()) ;
  if (seedShpItr==barrelClShpMap.end ()) {
    hasBarrel=false ;
    seedShpItr=endcapClShpMap.find (sclusRef->seed ()) ;
    if (seedShpItr==endcapClShpMap.end ()) hasEndcap=false ;
  }
  if (hasBarrel || hasEndcap) {
    const ClusterShapeRef& sClShape = seedShpItr->val ;  
    s9s25=sClShape->e3x3 ()/sClShape->e5x5 () ;
    sigmaEtaEta=sqrt (sClShape->covEtaEta ()) ;
    lat=sClShape->lat () ;
    a20=sClShape->zernike20 () ;
  }
  else { 
    edm::LogWarning ("ElectronLikelihood") << "Cannot find hits in ECAL barrel or ECAL encap."
					   << "Why are you requesting filling ECAL infos?";
  }

  vector<double> inputs;
  inputs.push_back(s9s25);
  inputs.push_back(sigmaEtaEta);
  inputs.push_back(lat);
  inputs.push_back(a20);

  // evaluate the Fisher discriminant
  double clusterShapeFisher;
  std::vector<DetId> vecId=sclusRef->getHitsByDetId () ;
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
    edm::LogWarning ("ElectronLikelihood") << "Undefined electron, eta = " << electron->eta () << "!" ;
  }
  return clusterShapeFisher;
}



// --------------------------------------------------------



float 
ElectronLikelihood::result (const PixelMatchGsfElectron *electron, 
                            const edm::Event& iEvent) const 
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
  getInputVar (electron, measurements, iEvent) ;

  // Split using only the 10^1 bit (golden/big brem/narrow/showering)
  int bitVal=-1 ;
  int gsfclass=electron->classification () ;
  if (gsfclass<99) 
    bitVal=int (gsfclass)/10 ; 
  else
    bitVal=int (int (gsfclass)%100)/10 ;
  
  // temporary: crack electrons goes into Class3 (showering)
  if (bitVal==4) bitVal=3 ;
  if (bitVal<0 || bitVal>3)
    throw cms::Exception ("ElectronLikelihood") << "ERROR! electron class " << gsfclass << " is not accepted!\n" ;

  char className[20] ;
  sprintf (className,"GsfClass%d",bitVal) ;

  SuperClusterRef sclusRef = electron->get<SuperClusterRef> () ;
  std::vector<DetId> vecId=sclusRef->getHitsByDetId () ;
  EcalSubdetector subdet = EcalSubdetector (vecId[0].subdetId ()) ;
  float thisPt = sqrt ( (electron->momentum ().x ()*electron->momentum ().x ()) + (electron->momentum ().y ()*electron->momentum ().y ()) ) ;

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

