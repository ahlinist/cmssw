// ETThetaPhi parametrisation
#include "TopQuarkAnalysis/TopMassMeasurement/interface/TtSemiIdeogramScan.h"


//
// constructor - read in the fit functions from a root file
//
TtSemiIdeogramScan::TtSemiIdeogramScan() {}

TtSemiIdeogramScan::TtSemiIdeogramScan(vector<double> & mtvals) {
  mtvalues = mtvals;
}



//
// destructor
//
TtSemiIdeogramScan::~TtSemiIdeogramScan() {
}


vector<pair<double,double> > TtSemiIdeogramScan::getScanValues(TtSemiEvtSolution& asol) {

  // read three jets 4-vec's that are supposed to be the decay products of the hadronic top
  TMatrixD m1(3,3), m2(3,3), m3(3,3);
  m1.Zero(); m2.Zero(); m3.Zero();
  
  TLorentzVector hadpVec(asol.getCalHadp().px(),asol.getCalHadp().py(),
                         asol.getCalHadp().pz(),asol.getCalHadp().energy());
  TLorentzVector hadqVec(asol.getCalHadq().px(),asol.getCalHadq().py(),
                      	 asol.getCalHadq().pz(),asol.getCalHadq().energy());
  TLorentzVector hadbVec(asol.getCalHadb().px(),asol.getCalHadb().py(),
                         asol.getCalHadb().pz(),asol.getCalHadb().energy());
    
  // plus their jet resolutions
  m1(0,0) = pow(asol.getCalHadp().getResET(),  2);
  m1(1,1) = pow(asol.getCalHadp().getResTheta(), 2);
  m1(2,2) = pow(asol.getCalHadp().getResPhi(), 2);
  m2(0,0) = pow(asol.getCalHadq().getResET(),  2); 
  m2(1,1) = pow(asol.getCalHadq().getResTheta(), 2); 
  m2(2,2) = pow(asol.getCalHadq().getResPhi(), 2);
  m3(0,0) = pow(asol.getCalHadb().getResET(),  2); 
  m3(1,1) = pow(asol.getCalHadb().getResTheta(), 2); 
  m3(2,2) = pow(asol.getCalHadb().getResPhi(), 2);

  // loop over the vector of mtvalues
  vector<pair<double,double> > scan;
  for(unsigned int m=0; m<mtvalues.size(); m++){
      
    // setup the fitter
    TKinFitter * theFitter = new TKinFitter("TtFit", "TtFit");
    TMatrixD empty3(3,3);
    TFitParticleEtThetaPhi      * fitHadb = new TFitParticleEtThetaPhi("Jet1", "Jet1", 0, &empty3);
    TFitParticleEtThetaPhi      * fitHadp = new TFitParticleEtThetaPhi("Jet2", "Jet2", 0, &empty3);
    TFitParticleEtThetaPhi      * fitHadq = new TFitParticleEtThetaPhi("Jet3", "Jet3", 0, &empty3);
    theFitter->addMeasParticle(fitHadb);
    theFitter->addMeasParticle(fitHadp);
    theFitter->addMeasParticle(fitHadq);
    fitHadp->setIni4Vec(&hadpVec); fitHadp->setCovMatrix(&m1);
    fitHadq->setIni4Vec(&hadqVec); fitHadq->setCovMatrix(&m2);
    fitHadb->setIni4Vec(&hadbVec); fitHadb->setCovMatrix(&m3);
    TFitConstraintM  * cons1 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0 , 80.35);
    cons1->addParticles1(fitHadp, fitHadq);
    theFitter->addConstraint(cons1);
    TFitConstraintM  * cons2 = new TFitConstraintM("MassConstraint", "Mass-Constraint", 0, 0, mtvalues[m]);
    cons2->addParticles1(fitHadp, fitHadq, fitHadb);
    theFitter->addConstraint(cons2);
    theFitter->setMaxNbIter(200);
    theFitter->setMaxDeltaS(5e-5);
    theFitter->setMaxF(1e-4);
    theFitter->setVerbosity(0);
    
    // perform the fit
    theFitter->fit();
  
    // store the probability of the Chi2 for this scan value
    if ( theFitter->getStatus() == 0 ) {
      scan.push_back(pair<double,double>(mtvalues[m],TMath::Prob(theFitter->getS(), theFitter->getNDF())));
    }
    
    // delete the created objects
    delete cons1; delete cons2;
    delete fitHadb; delete fitHadp; delete fitHadq;
    delete theFitter;
  }
  return scan;
}
