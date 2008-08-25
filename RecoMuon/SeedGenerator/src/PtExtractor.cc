#include "RecoMuon/SeedGenerator/src/PtExtractor.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

#include "TMath.h"

PtExtractor::PtExtractor(const edm::ParameterSet& par){

  // load pT seed parameters
  // DT combinations
  DT12 = par.getParameter<std::vector<double> >("DT_12");
  DT13 = par.getParameter<std::vector<double> >("DT_13");
  DT14 = par.getParameter<std::vector<double> >("DT_14");
  DT23 = par.getParameter<std::vector<double> >("DT_23");
  DT24 = par.getParameter<std::vector<double> >("DT_24");
  DT34 = par.getParameter<std::vector<double> >("DT_34");
  // CSC combinations
  CSC01 = par.getParameter<std::vector<double> >("CSC_01");
  CSC12 = par.getParameter<std::vector<double> >("CSC_12");
  CSC02 = par.getParameter<std::vector<double> >("CSC_02");
  CSC13 = par.getParameter<std::vector<double> >("CSC_13");
  CSC03 = par.getParameter<std::vector<double> >("CSC_03");
  CSC14 = par.getParameter<std::vector<double> >("CSC_14");
  CSC23 = par.getParameter<std::vector<double> >("CSC_23");
  CSC24 = par.getParameter<std::vector<double> >("CSC_24");
  CSC34 = par.getParameter<std::vector<double> >("CSC_34");

  // Overlap combinations
  OL1213 = par.getParameter<std::vector<double> >("OL_1213");
  OL1222 = par.getParameter<std::vector<double> >("OL_1222");
  OL1232 = par.getParameter<std::vector<double> >("OL_1232");
  OL2213 = par.getParameter<std::vector<double> >("OL_2213");
  OL2222 = par.getParameter<std::vector<double> >("OL_2222");

  // Single segments (CSC)
  SME11 =  par.getParameter<std::vector<double> >("SME_11");
  SME12 =  par.getParameter<std::vector<double> >("SME_12");
  SME13 =  par.getParameter<std::vector<double> >("SME_13");
  SME21 =  par.getParameter<std::vector<double> >("SME_21");
  SME22 =  par.getParameter<std::vector<double> >("SME_22");
  SME31 =  par.getParameter<std::vector<double> >("SME_31");
  SME32 =  par.getParameter<std::vector<double> >("SME_32");
  SME41 =  par.getParameter<std::vector<double> >("SME_41");

  // Single segments (DT)
  SMB10 =  par.getParameter<std::vector<double> >("SMB_10");
  SMB11 =  par.getParameter<std::vector<double> >("SMB_11");
  SMB12 =  par.getParameter<std::vector<double> >("SMB_12");
  SMB20 =  par.getParameter<std::vector<double> >("SMB_20");
  SMB21 =  par.getParameter<std::vector<double> >("SMB_21");
  SMB22 =  par.getParameter<std::vector<double> >("SMB_22");
  SMB30 =  par.getParameter<std::vector<double> >("SMB_30");
  SMB31 =  par.getParameter<std::vector<double> >("SMB_31");
  SMB32 =  par.getParameter<std::vector<double> >("SMB_32");

}
PtExtractor::~PtExtractor(){
}


std::vector<double> PtExtractor::pT_extract(MuonTransientTrackingRecHit::ConstMuonRecHitPointer firstHit,
                                                                      MuonTransientTrackingRecHit::ConstMuonRecHitPointer secondHit){

  //---- Having two hits a pT is calculated; it uses a parametrization from the STA Seeding code
  // RPCs shoudl not be used (no parametrization)!
  //std::vector<double> vPara;
  
  std::map <std::string, std::vector<double> > chamberCombination;

  // DT station-station
  chamberCombination["DT12"] = DT12;
  chamberCombination["DT13"] = DT13;
  chamberCombination["DT14"] = DT14;
  chamberCombination["DT23"] = DT23;
  chamberCombination["DT24"] = DT24;
  chamberCombination["DT34"] = DT34;
  
  // CSC station-station (O is ME11)
  chamberCombination["CSC01"] = CSC01;
  chamberCombination["CSC12"] = CSC12;
  chamberCombination["CSC02"] = CSC02;
  chamberCombination["CSC13"] = CSC13;
  chamberCombination["CSC03"] = CSC03;
  chamberCombination["CSC14"] = CSC14;
  chamberCombination["CSC23"] = CSC23;
  chamberCombination["CSC24"] = CSC24;
  chamberCombination["CSC34"] = CSC34;

  // DT-CSC overlap 
  chamberCombination["OL1213"] = OL1213;
  chamberCombination["OL1222"] = OL1222;
  chamberCombination["OL1232"] = OL1232;
  chamberCombination["OL2213"] = OL2213;
  chamberCombination["OL2222"] = OL2222;

  // CSC single (ststion/ring)
  chamberCombination["SME11"] = SME11;
  chamberCombination["SME12"] = SME12;
  chamberCombination["SME13"] = SME13;
  chamberCombination["SME21"] = SME21;
  chamberCombination["SME22"] = SME22;
  chamberCombination["SME31"] = SME31;
  chamberCombination["SME32"] = SME32;
  chamberCombination["SME41"] = SME41;

  // DT single (?)
  chamberCombination["SMB10"] = SMB10;
  chamberCombination["SMB11"] = SMB11;
  chamberCombination["SMB12"] = SMB12;
  chamberCombination["SMB20"] = SMB20;
  chamberCombination["SMB21"] = SMB21;
  chamberCombination["SMB22"] = SMB22;
  chamberCombination["SMB30"] = SMB30;
  chamberCombination["SMB31"] = SMB31;
  chamberCombination["SMB32"] = SMB32;


  double phiInner = firstHit->globalPosition().phi();
  double phiOuter = secondHit->globalPosition().phi();

  double etaInner = firstHit->globalPosition().eta();
  double etaOuter = secondHit->globalPosition().eta();

  // if some of the segments is missing r-phi measurement then we should
  // use only the 4D phi estimate (also use 4D eta estimate only)
  // the direction is not so important (it will be corrected) 

  double dPhi =  phiInner - phiOuter;
  if(dPhi < -TMath::Pi()){
    dPhi += 2*TMath::Pi();
  }
  else if(dPhi >  TMath::Pi()){
    dPhi -= 2*TMath::Pi();
  }
  int sign = 1;
  if ( dPhi< 0.) {
    dPhi = -dPhi;
    sign = -1;
  }

  if (dPhi < 1.0e-6){
    dPhi = 1.0e-6;
  }
  double eta = fabs(etaOuter);// what if it is 2D segment? use inner?


  std::vector <int> stationCoded(2);
  stationCoded[0] = stationCoded[1] = 999;

  DetId  detId_first = firstHit->hit()->geographicalId();
  DetId  detId_second = secondHit->hit()->geographicalId();

  // use some encoding for first/second hit and for DT/CSC
  if( firstHit->isDT() ){
    // first hit is DT
    DTChamberId dtCh(detId_first);
    //std::cout<<"first (DT) St/W/S = "<<dtCh.station()<<"/"<<dtCh.wheel()<<"/"<<dtCh.sector()<<"/"<<std::endl;
    switch (dtCh.station()){
    case 1:
      stationCoded[0] = -1; // negative means DT
      break;
    case 2:
      stationCoded[0] = -2;
      break;
    case 3:
      stationCoded[0] = -3;
      break;
    case 4:
      stationCoded[0] = -4;
      break;
    default:
      stationCoded[0] = -99;
      break;
    }
  }
  else if( firstHit->isCSC() ){
    // first hit is CSC
    CSCDetId cscID(detId_first);
    //std::cout<<"first (CSC) E/S/R/C = "<<cscID.endcap()<<"/"<<cscID.station()<<"/"<<cscID.ring()<<"/"<<cscID.chamber()<<std::endl;
    switch ( cscID.station() ){
    case 1:
      if ( 1 == cscID.ring() ||  4 == cscID.ring()){
        stationCoded[0] = 0;
      }
      else{
        stationCoded[0] = 1;
      }
      break;
    case 2:
      stationCoded[0] = 2;
      break;
    case 3:
      stationCoded[0] = 3;
      break;
    case 4:
      stationCoded[0] = 4;
      break;
    default:
      stationCoded[0] = 99;
      break;
    }
  }
  else if(firstHit->isRPC()){
    // should not be the case
  }
  if(secondHit->isDT()){
    // second hit is DT
    DTChamberId dtCh(detId_second);
    //std::cout<<"second (DT) St/W/S = "<<dtCh.station()<<"/"<<dtCh.wheel()<<"/"<<dtCh.sector()<<std::endl;
    switch (dtCh.station()){
    case 1:
      stationCoded[1] = -1;
      break;
    case 2:
      stationCoded[1] = -2;
      break;
    case 3:
      stationCoded[1] = -3;
      break;
    case 4:
      stationCoded[1] = -4;
      break;
    default:
      stationCoded[1] = -99;
      break;
    }
  }
  else if( secondHit->isCSC() ){
    // second hit is CSC
    CSCDetId cscID(detId_second);
    //std::cout<<"second (CSC) E/S/R/C = "<<cscID.endcap()<<"/"<<cscID.station()<<"/"<<cscID.ring()<<"/"<<cscID.chamber()<<std::endl;
    switch ( cscID.station() ){
    case 1:
      if ( 1 == cscID.ring() ||  4 == cscID.ring()){
        stationCoded[1] = 0;
      }
      else{
        stationCoded[1] = 1;
      }
      break;
    case 2:
      stationCoded[1] = 2;
      break;
    case 3:
      stationCoded[1] = 3;
      break;
    case 4:
      stationCoded[1] = 4;
      break;
    default:
      stationCoded[1] = 99;
      break;
    }
  }
  else if(secondHit->isRPC()){
    // should not be the case
  }

  std::ostringstream os0 ;
  std::ostringstream os1 ;
  os0 << abs(stationCoded[0]);
  os1 << abs(stationCoded[1]);

  std::string  combination = "0";
  std::string init_combination = combination;
  bool singleSegment = false;
  if( stationCoded[0] == stationCoded[1]){
    // single segment - DT or CSC
    singleSegment = true;
    GlobalPoint segPos = firstHit->globalPosition();
    GlobalVector gv = firstHit->globalDirection();

    // Psi is angle between the segment origin and segment direction
    // Use dot product between two vectors to get Psi in global x-y plane
    double cosDpsi  = (gv.x()*segPos.x() + gv.y()*segPos.y());
    cosDpsi /= sqrt(segPos.x()*segPos.x() + segPos.y()*segPos.y());
    cosDpsi /= sqrt(gv.x()*gv.x() + gv.y()*gv.y());

    double axb = ( segPos.x()*gv.y() ) - ( segPos.y()*gv.x() ) ;
    sign = (axb < 0.) ? 1 : -1;

    double dpsi = fabs(acos(cosDpsi)) ;
    if ( dpsi > TMath::Pi()/2.) {
      dpsi = TMath::Pi() - dpsi;
    }
    if (fabs(dpsi) < 0.00005) {
      dpsi = 0.00005;
    }
    dPhi = dpsi;
    double etaAbs = fabs(eta);
    switch(stationCoded[0]){
      // the 1st layer
    case -1:
      // MB10
      if ( etaAbs < 0.3 ) {
	combination = "SMB10";
      }
      // MB11
      else if(etaAbs >= 0.3 && etaAbs < 0.82){
	combination = "SMB11";
      }
      // MB12
      else if(etaAbs >= 0.82 && etaAbs < 1.2){
	combination = "SMB12";
      }
      else{
	// can not be
      }
      break;
    case 1:
      // ME13
      if(etaAbs > 0.92 && etaAbs < 1.16){
        combination = "SME13";
      }
      // ME12
      else if(etaAbs >= 1.16 && etaAbs <= 1.6){
      }
      else{
        // can not be 
      }
      break;
    case 0:
      // ME11
      if ( etaAbs > 1.6 && etaAbs < 2.45 ) {
	combination = "SME11";
      }
      break;
      // the 2nd layer
    case -2:
      // MB20
      if ( etaAbs < 0.25 ) {
        combination = "SMB20";
      }
      // MB21
      else if ( etaAbs >= 0.25 && etaAbs < 0.72 ) {
        combination = "SMB21";
      }
      // MB22
      else if ( etaAbs >= 0.72 && etaAbs < 1.04 ) {
        combination = "SMB22";
      }
      else{
        // can not be 
	combination = "SMB22"; // in case
      }
      break;
    case 2:
      // ME22
      if ( etaAbs > 0.95 && etaAbs <= 1.6 ) {
	combination = "SME22";
      }
      // ME21
      else if ( etaAbs > 1.6 && etaAbs < 2.45 ) {
	combination = "SME21";
      }
      else{
	// can not be
      }
      break;
      // the 3rd layer
    case -3:
      // MB30
      if ( fabs(eta) <= 0.22 ) {
	combination = "SMB30";
      }
      // MB31
      if ( fabs(eta) > 0.22 && fabs(eta) <= 0.6 ) {
	combination = "SMB31";
      }
      // MB32
      if ( fabs(eta) > 0.6 && fabs(eta) < 0.95 ) {
	combination = "SMB32";
      }
      else{
	// can not be
      }
      
      break;
    case 3:
      // missing?? 
      break;
    case 4:
      // missing??
      break;
      // -4? miising?
    default :
      break;
    }
  }
  else{
    if(stationCoded[0]<0){
      if(stationCoded[1]<0){
        // DT-DT
        combination = "DT" + os0.str() + os1.str();
      }
      else{
        // DT-CSC
        eta = fabs(etaInner);
        if(-1==stationCoded[0]){
          switch (stationCoded[1]){
          case 1:
            combination = "OL1213";
            break;
          case 2:
            combination = "OL1222";
            break;
          case 3:
            combination = "OL1232";
            break;
          default:
            // can not be 
            break;
          }
        }
        else if (-2==stationCoded[0]){
          if(1==stationCoded[1]){
            combination = "OL2213";
          }
          else{
	    combination = "OL2222";
          }
        }
        else{
          // can not be
        }
      }
    }
    else{
      if(stationCoded[1]<0){
        // CSC-DT
        // can not be
      }
      else{
        // CSC-CSC
        combination = "CSC" + os0.str() + os1.str();
        if("CSC04" == combination){
          combination = "CSC14";
        }
      }
    }
  }

  std::vector<double> pTestimate(2);// 
  if(init_combination!=combination){
    // get the pT havin the chosen hit combination
    pTestimate = getPt(chamberCombination[combination], eta, dPhi); // dPhi has a different meaning 
    //for single segment combination
    if(singleSegment){
      pTestimate[0] = fabs(pTestimate[0]);
      pTestimate[1] = fabs(pTestimate[1]);
    }
    pTestimate[0] = pTestimate[0] * double(sign);
  }
  else{
    pTestimate[0] = pTestimate[1] = 100;// harcoded - remove it! do we need that at all?
    // hmm
  }
  return pTestimate;
}


// it is a copy from the Seed code - call it from there?
std::vector<double> PtExtractor::getPt(std::vector<double> vPara, double eta, double dPhi ) {
  std::cout<<" eta = "<<eta<<" dPhi = "<<dPhi<<" vPara[0] = "<<vPara[0]<<" vPara[1] = "<<vPara[1]<<" vPara[2] = "<<vPara[2]<<std::endl;
  double h  = fabs(eta);
  double estPt  = ( vPara[0] + vPara[1]*h + vPara[2]*h*h ) / dPhi;
  double estSPt = ( vPara[3] + vPara[4]*h + vPara[5]*h*h ) * estPt;
  std::cout<<"estPt = "<<estPt<<std::endl;
  std::vector<double> paraPt ;
  paraPt.push_back( estPt );
  paraPt.push_back( estSPt ) ;
  return paraPt ;
}
