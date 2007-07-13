//
// Author:  Jan Heyninck
// Created: Tue Apr  3 17:33:23 PDT 2007
//
// $Id: MtopUncertaintyCalc.cc,v 1.1 2007/06/01 09:16:17 heyninck Exp $
//
#include "TopQuarkAnalysis/TopMassMeasurement/interface/MtopUncertaintyCalc.h"

MtopUncertaintyCalc::MtopUncertaintyCalc() {
}


// destructor
MtopUncertaintyCalc::~MtopUncertaintyCalc() {
}




void  MtopUncertaintyCalc::operator()(TtSemiMassSolution& s){
  
  // read the input objects
  readInput(s);
  
  // calculate, according to the parametrisation, the DM/dpar partial differentials
  calcDmtDpMatrices();

  // calculate the top mass uncertainty
  double dmtop = calcDMtop();
  s.setMtopUncertainty(dmtop);
}












//
// Function to read all neccessary input from the solution
//
void MtopUncertaintyCalc::readInput(TtSemiMassSolution& s){  
  
  //read the parametrisation for the jets used in the kinfit
  //1: EtEtaPhi, 2:EtThetaPhi, 3:EMom
  jetparam = s.getJetParametrisation();
  if( jetparam < 3  ) dim = 3;
  if( jetparam == 3 ) dim = 4;
  
  // get the fitted top mass and the fitted jet fourvectors
  mtop     = s.getFitHadt().mass();
  jet[0]   = s.getFitHadp();
  jet[1]   = s.getFitHadq();
  jet[2]   = s.getFitHadb();


  // get the covariance matrices for the three hadronic jets
  for(int j=0; j<3; j++){
    jCovM[j].ResizeTo(dim,dim);
    jCovM[j].Zero();
    for(int ii = 0; ii<dim; ii++){
      for(int jj = 0; jj<dim; jj++){
        jCovM[j](ii,jj) = jet[j].getCovM()[ii*dim+jj];
      }
    }
  }
}



//
// Function to read the covariance Matrix for the three hadronic jets
//

void MtopUncertaintyCalc::calcDmtDpMatrices(){
  
  double Esum  	 =   jet[0].energy() +   jet[1].energy() +   jet[2].energy();
  double pxsum 	 =   jet[0].px()     +   jet[1].px()     +   jet[2].px();
  double pysum 	 =   jet[0].py()     +   jet[1].py()     +   jet[2].py();
  double pzsum 	 =   jet[0].pz()     +   jet[1].pz()     +   jet[2].pz();
  
  for(int j=0; j<3; j++){
    dMdp[j].ResizeTo(1,dim);
    dMdp[j].Zero();
    if(jetparam==1){
      dMdp[j](0,0)  = (1./mtop) * ( Esum/sin(jet[j].theta()) - pxsum*cos(jet[j].phi()) - pysum*sin(jet[j].phi()) - pzsum/tan(jet[j].theta()) );
      dMdp[j](0,1)  = (1./mtop) * ( -1.*Esum*jet[j].et()*cos(jet[j].theta())/pow(sin(jet[j].theta()),2) + pzsum*jet[j].et()/pow(sin(jet[j].theta()),2) );
      dMdp[j](0,1)  *= -2. / ( exp(jet[j].eta()) + exp(-1.*jet[j].eta()) );
      dMdp[j](0,2)  = (1./mtop) * ( pysum*jet[j].et()*cos(jet[j].phi()) - pxsum*jet[j].et()*sin(jet[j].phi()) );
    }
    if(jetparam==2){
      dMdp[j](0,0)  = (1./mtop) * ( Esum/sin(jet[j].theta()) - pxsum*cos(jet[j].phi()) - pysum*sin(jet[j].phi()) - pzsum/tan(jet[j].theta()) );
      dMdp[j](0,1)  = (1./mtop) * ( -1.*Esum*jet[j].et()*cos(jet[j].theta())/pow(sin(jet[j].theta()),2) + pzsum*jet[j].et()/pow(sin(jet[j].theta()),2) );
      dMdp[j](0,2)  = (1./mtop) * ( pysum*jet[j].et()*cos(jet[j].phi()) - pxsum*jet[j].et()*sin(jet[j].phi()) );
    }
    if(jetparam==3){
      // Still to complete
    }
  }
}





//
// Function to calculate the top mass uncertainty from the cov matrices
//

double MtopUncertaintyCalc::calcDMtop(){
  double dmtop = 0.;
  for(int j=0; j<3; j++){
    TMatrixD dMdpInv(TMatrixD::kTransposed,dMdp[j]);
    //jCovM[j].Print();
    TMatrixD mult = dMdp[j] * jCovM[j] * dMdpInv;
    dmtop += mult(0,0);
  }
  
  return sqrt(dmtop);
}
