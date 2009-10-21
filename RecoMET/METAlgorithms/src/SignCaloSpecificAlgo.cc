#include "DataFormats/Math/interface/LorentzVector.h"
#include "RecoMET/METAlgorithms/interface/SignCaloSpecificAlgo.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "RecoMET/METAlgorithms/interface/SigInputObj.h"
#include "RecoMET/METAlgorithms/interface/significanceAlgo.h"
#include "RecoMET/METAlgorithms/interface/SignAlgoResolutions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"

#include <string>
using namespace reco;
using namespace std;
// -*- C++ -*-
//
// Package:    METAlgorithms
// Class:      SigInputObj
// 
/**\class METSignificance SigInputObj.cc RecoMET/METAlgorithms/src/SigInputObj.cc

 Description: <one line class summary>

 Implementation:

-------------------------------------------------------------------------
 This algorithm adds calorimeter specific global event information to 
 the MET object which may be useful/needed for MET Data Quality Monitoring
 and MET cleaning.  This list is not exhaustive and additional 
 information will be added in the future. 
-------------------------------------

*/
//
// Original Author:  Kyle Story, Freya Blekman (Cornell University)
//         Created:  Fri Apr 18 11:58:33 CEST 2008
// $Id: SignCaloSpecificAlgo.cc,v 1.6 2009/08/21 13:18:56 fblekman Exp $
//
//

SignCaloSpecificAlgo::SignCaloSpecificAlgo():
  significance_(0.),
  matrix_(2,2)
{
  matrix_(0,0)=matrix_(1,0)=matrix_(0,1)=matrix_(1,1)=0.;
}
SignCaloSpecificAlgo::~SignCaloSpecificAlgo()
{
}

void SignCaloSpecificAlgo::usePreviousSignif(const std::vector<double> &values)
{
  if(values.size()!=4)
    return;
  matrix_(0,0)=values[0];
  matrix_(0,1)=values[1];
  matrix_(1,0)=values[2];
  matrix_(1,1)=values[3];
  return;
}
std::vector<double>
SignCaloSpecificAlgo::getSignifMatrix()
{
  std::vector<double> result(4,0.);
  result[0]=matrix_(0,0);
  result[1]=matrix_(0,1);
  result[2]=matrix_(1,0);
  result[3]=matrix_(1,1);
  return result;
}
////////////////
//
// Convert a list of calo towers to objects that can be passed to the significance algo:

std::vector<metsig::SigInputObj> 
SignCaloSpecificAlgo::makeVectorOutOfCaloTowers(edm::Handle<edm::View<reco::Candidate> > towers, const::metsig::SignAlgoResolutions& resolutions, bool noHF, double globalThreshold)
{

  edm::View<Candidate>::const_iterator towerCand = towers->begin();
  std::vector<metsig::SigInputObj> signInputVec;
  //iterate over all CaloTowers and record information
  for( ; towerCand != towers->end(); towerCand++ ) {
    const Candidate *candidate = &(*towerCand);
    if(candidate){
      const CaloTower * calotower = dynamic_cast<const CaloTower*> (candidate);
      if(calotower){
	if(calotower->et()<globalThreshold)
	  continue;
	bool wasused=false;
	double sign_tower_et = calotower->et();
	double sign_tower_phi = calotower->phi();
	double sign_tower_sigma_et = 0;
	double sign_tower_sigma_phi = 0;
	std::string sign_tower_type = "";
	
	bool hadIsDone = false;
	bool emIsDone = false;
	int cell = calotower->constituentsSize();

	while ( --cell >= 0 && (!hadIsDone || !emIsDone) ) 
	  {
	    DetId id = calotower->constituent( cell );
	    if( !hadIsDone && id.det() == DetId::Hcal ) 
	      {
		HcalSubdetector subdet = HcalDetId(id).subdet();
		if(subdet == HcalBarrel){
		  sign_tower_type = "hadcalotower";
		  sign_tower_et = calotower->hadEt();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloHB,metsig::ET,calotower->hadEt(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloHB,metsig::PHI,calotower->hadEt(),calotower->phi(),calotower->eta());
		}
		else if(subdet==HcalOuter){
		  sign_tower_type = "hadcalotower";
		  sign_tower_et = calotower->outerEt();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloHO,metsig::ET,calotower->outerEt(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloHO,metsig::PHI,calotower->outerEt(),calotower->phi(),calotower->eta());
		}
		else if(subdet==HcalEndcap){
		  sign_tower_type = "hadcalotower";
		  sign_tower_et = calotower->hadEt();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloHE,metsig::ET,calotower->hadEt(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloHE,metsig::PHI,calotower->hadEt(),calotower->phi(),calotower->eta());
		}
		else if(subdet == HcalForward){
		  sign_tower_type = "hadcalotower";
		  sign_tower_et = calotower->et();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloHF,metsig::ET,calotower->et(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloHF,metsig::PHI,calotower->et(),calotower->phi(),calotower->eta());
		}
		else{
		  edm::LogWarning("SignCaloSpecificAlgo") << " HCAL tower cell not assigned to an HCAL subdetector!!!" << std::endl;
		}
		// and book!
		metsig::SigInputObj temp(sign_tower_type,sign_tower_et,sign_tower_phi,sign_tower_sigma_et,sign_tower_sigma_phi);
		if(!noHF || subdet !=HcalForward)
		  signInputVec.push_back(temp);
		
		wasused=1;
		hadIsDone = true;
	      }
	    else if( !emIsDone && id.det() == DetId::Ecal )
	      {
		EcalSubdetector subdet = EcalSubdetector( id.subdetId() );
		
		if(subdet == EcalBarrel){
		  sign_tower_type = "emcalotower";
		  sign_tower_et = calotower->emEt();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloEB,metsig::ET,calotower->emEt(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloEB,metsig::PHI,calotower->emEt(),calotower->phi(),calotower->eta());
		}
		else if(subdet == EcalEndcap ){
		  sign_tower_type = "emcalotower";
		  sign_tower_et = calotower->emEt();
		  sign_tower_sigma_et = resolutions.eval(metsig::caloEE,metsig::ET,calotower->emEt(),calotower->phi(),calotower->eta());
		  sign_tower_sigma_phi = resolutions.eval(metsig::caloEE,metsig::PHI,calotower->emEt(),calotower->phi(),calotower->eta());
		    
		}
		else{
		  edm::LogWarning("SignCaloSpecificAlgo") << " ECAL tower cell not assigned to an ECAL subdetector!!!" << std::endl;
		}
		metsig::SigInputObj temp(sign_tower_type,sign_tower_et,sign_tower_phi,sign_tower_sigma_et,sign_tower_sigma_phi);
		signInputVec.push_back(temp);
		wasused=1;
		emIsDone = true;
	      }
	  }
	if(wasused==0)
	  edm::LogWarning("SignCaloSpecificAlgo") << "found non-assigned cell, " << std::endl;
      }
    }
  }
  return signInputVec;
}
////////////////
//
// Basic MET algorithm. gets towers, does sum. Very similar to standard MET.
void SignCaloSpecificAlgo::calculateBaseCaloMET(edm::Handle<edm::View<reco::Candidate> > towers, CommonMETData met,const metsig::SignAlgoResolutions& resolutions, bool noHF, double globalThreshold)
{ 

  //retreive calo tower information from candidates
  //start with the first element of the candidate list

  

  // use this container to calculate the significance. SigInputObj are objects that contain both directional and uncertainty information and are used as input to the significance calculation

  std::vector<metsig::SigInputObj> signInputVec = makeVectorOutOfCaloTowers(towers, resolutions, noHF, globalThreshold);
  
  // now run the significance algorithm.
  
  double sign_calo_met_total=0;
  double sign_calo_met_phi=0;
  double sign_calo_met_set=0;
  metsig::significanceAlgo signifalgo;
  // check the caloMET, if significance was already run continue with the matrix that is stored..
  signifalgo.addSignifMatrix(matrix_);
  signifalgo.addObjects(signInputVec);
  matrix_=signifalgo.getSignifMatrix();
  significance_ = signifalgo.significance( sign_calo_met_total, sign_calo_met_phi, sign_calo_met_set);
  // cleanup everything:
  signInputVec.clear();
  // and return
}

//-------------------------------------------------------------------------
