/** \class EcalTBH2LocalReadout
 *   produce ECAL uncalibrated rechits from dataframes
 *
 *
 */

#include "RecoTBCalo/EcalTBH2LocalReadout/interface/EcalTBH2LocalReadout.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalMGPASample.h"
#include "FWCore/Framework/interface/Handle.h"

#include <iostream>
#include <iomanip>
#include <cmath>

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include <vector>

EcalTBH2LocalReadout::EcalTBH2LocalReadout(const edm::ParameterSet& ps):theTargetCrystal_(-1) {

   EBdigiCollection_ = ps.getParameter<std::string>("EBdigiCollection");
   digiProducer_   = ps.getParameter<std::string>("digiProducer");
   tableEtaPosition_  = ps.getParameter<int>("tableEtaPosition");
   tablePhiPosition_  = ps.getParameter<int>("tablePhiPosition");
   
   //   EBreducedDigiCollection_  = ps.getParameter<std::string>("EBreducedDigiCollection");
   produces< EBDigiCollection >();

   theTTlist_.reserve(1);
   theDetIds.clear();
   theDetIds.reserve(1700);
   //Constructing a fake list of EBDetId in SM4
   for (int icry=0;icry<1700;icry++)
     {
       theDetIds.push_back(DetId(EBDetId(1,icry+1,EBDetId::SMCRYSTALMODE).rawId()));
     }
}

EcalTBH2LocalReadout::~EcalTBH2LocalReadout() {
}

void
EcalTBH2LocalReadout::produce(edm::Event& evt, const edm::EventSetup& es) {

   
  edm::Handle< EBDigiCollection > pEBDigis;
  const EBDigiCollection* EBdigis =0;
  
  try {
    //     evt.getByLabel( digiProducer_, EBdigiCollection_, pEBDigis);
    evt.getByLabel( digiProducer_, pEBDigis);
    EBdigis = pEBDigis.product(); // get a ptr to the produc
    LogDebug("EcalUncalibRecHitInfo") << "total # EBdigis: " << EBdigis->size() ;
  } catch ( std::exception& ex ) {
    edm::LogError("EcalUncalibRecHitError") << "Error! can't get the product " << EBdigiCollection_.c_str() ;
  }
  
   edm::ESHandle<EcalTrigTowerConstituentsMap> eTTmap;
   es.get<IdealGeometryRecord>().get(eTTmap);     

   if (!EBdigis)
     return;



   std::auto_ptr< EBDigiCollection > EBreducedDigis( new EBDigiCollection );

   performReadout(*eTTmap, *EBdigis, *EBreducedDigis);

   evt.put( EBreducedDigis );
}

void EcalTBH2LocalReadout::findTTlist(const int & crysId, const EcalTrigTowerConstituentsMap& etmap) {

  // search for the TT involved in the NCRYMATRIX x NCRYMATRIX
  // around the target crystal if a new target, otherwise use
  // the list already filled

  if ( crysId == theTargetCrystal_ ) { return; }

  theTTlist_.clear();

  /// step 1:
  /// find the correspondence between the crystal identified in module and its DetId
  /// assuming only 1 SM present
  
  EBDetId theTargetId;
  std::vector<DetId>::const_iterator idItr = theDetIds.begin();
  unsigned int ncount = 0;
  bool found = false;
  
  while  ( (ncount < theDetIds.size()) && !found ) 
  {
    EBDetId thisEBdetid(idItr->rawId());
    if (thisEBdetid.ic() == crysId) {
      theTargetId = thisEBdetid;
      found = true;
    }
    ++idItr;
    ++ncount;
  }

  if ( !found ) {
    throw cms::Exception("ObjectNotFound", "Ecal TB target crystal not found in geometry");
    return;
  }
  theTargetCrystal_ = theTargetId.ic();

  /// step 2:
  /// find the crystals in the matrix and fill the TT list

  int myEta = theTargetId.ieta();
  int myPhi = theTargetId.iphi();


  for ( int icrysEta = (myEta-(NCRYMATRIX-1)/2) ; icrysEta <= (myEta+(NCRYMATRIX-1)/2) ; ++icrysEta ) {
    for ( int icrysPhi = (myPhi-(NCRYMATRIX-1)/2) ; icrysPhi <= (myPhi+(NCRYMATRIX-1)/2) ; ++icrysPhi ) {
      
      /// loop on all the valid DetId and search for the good ones

      EBDetId thisEBdetid;

      idItr = theDetIds.begin();
      ncount = 0;
      found = false;
  
      while  ( (ncount < theDetIds.size()) && !found ) 
        {
          EBDetId myEBdetid(idItr->rawId());
          if ( (myEBdetid.ieta() == icrysEta) && (myEBdetid.iphi() == icrysPhi) ) {
            thisEBdetid = myEBdetid;
            found = true;
          }
          ++idItr;
          ++ncount;
        }

      if ( found ) {

        EcalTrigTowerDetId thisTTdetId=etmap.towerOf(thisEBdetid);

        LogDebug("EcalDigi") << "Crystal to be readout: sequential id = " << thisEBdetid.ic() << " eta = " << icrysEta << " phi = " << icrysPhi << " from TT = " << thisTTdetId;

        if ( theTTlist_.size() == 0 || ( theTTlist_.size() == 1 && theTTlist_[0] != thisTTdetId )) {
          theTTlist_.push_back(thisTTdetId);
        }
        else {
          std::vector<EcalTrigTowerDetId>::iterator ttFound = find(theTTlist_.begin(), theTTlist_.end(), thisTTdetId);
          if ( theTTlist_.size() > 1 && ttFound == theTTlist_.end() && *(theTTlist_.end()) != thisTTdetId ) { 
            theTTlist_.push_back(thisTTdetId);
          }
        }
      }

    }
  }

  edm::LogInfo("EcalDigi") << " TT to be read: ";
  for ( unsigned int i = 0 ; i < theTTlist_.size() ; ++i ) {
    edm::LogInfo("EcalDigi") << " TT " << i << " " << theTTlist_[i];
  }

}

void EcalTBH2LocalReadout::readOut(const EBDigiCollection & input, EBDigiCollection & output, const EcalTrigTowerConstituentsMap& etmap) {

  for(EBDigiCollection::const_iterator digiItr = input.begin();
      digiItr != input.end(); ++digiItr)
    {
      EcalTrigTowerDetId thisTTdetId=etmap.towerOf(digiItr->id());
      std::vector<EcalTrigTowerDetId>::iterator ttFound = find(theTTlist_.begin(), theTTlist_.end(), thisTTdetId);
      if ((ttFound != theTTlist_.end()) || *(theTTlist_.end()) == thisTTdetId) { 
        output.push_back(*digiItr);
      }
    }
  edm::LogInfo("EcalDigi") << "Read EB Digis: " << output.size();
  
}

void EcalTBH2LocalReadout::performReadout(const EcalTrigTowerConstituentsMap & theTTmap, const EBDigiCollection & input, EBDigiCollection & output) {

  //Embed code from Efe Yazgan to search for Hcal TT corresponding to table position
  // convert to ETA/PHI/THETA
  double eta, phi, theta;
  phi=0.263-0.259e-5*tablePhiPosition_;
  phi=2*atan(phi);
  theta=-0.273+0.006417*tableEtaPosition_;
  // cout<<"theta: "<<theta<<" phi: "<<phi<<endl;  

  eta=theta;
  eta=(90-eta)*3.14159265358979323846/180.0;
  eta=-log(tan(eta/2));

  int etaInt, phiInt;
  //  float etaTower, phiTower;

  //  etaInt = TMath::Nint(eta/0.087);
  //  phiInt = TMath::Nint(phi/0.087);
  etaInt = int(eta/0.087) + 1; //[should be comprised in 1-17] 
  phiInt = int(phi/0.087) + 1; //[should be comprised in 1-4] 
  //  etaTower = TMath::Nint(eta_seg*(0.5+eta/0.087))/(1.*eta_seg);
  //  phiTower= 10 + TMath::Nint(phi_seg*(0.5+phi/0.087))/(1.*phi_seg);

  int crysId = ((etaInt - 1) * 5 + 3 - 1)*20 + (4 - phiInt) * 5 + 3;
  
  //  edm::LogVerbatim("EcalTBH2LocalReadout") << "eta " << eta << " phi " << phi;
  //  edm::LogVerbatim("EcalTBH2LocalReadout") << "etaInt " << etaInt << " phiInt " << phiInt  << " crysId " << crysId ;

  // step 2: update (if needed) the TT list to be read

  findTTlist(crysId, theTTmap);

  // step 3: perform the readout

  readOut(input, output, theTTmap);

}

