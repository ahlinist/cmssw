#include "TauAnalysis/RecoTools/interface/ZllRecoilCorrectionAlgorithm.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

#include <string>

const double sigmaU1min = 5.0;
const double sigmaU2min = 5.0;

ZllRecoilCorrectionAlgorithm::ZllRecoilCorrectionAlgorithm(const edm::ParameterSet& cfg)
  : corrParameterData_(0),
    corrParameterMC_(0)
{
  edm::ParameterSet cfgCorrParameter = cfg.getParameter<edm::ParameterSet>("parameter"); 
  corrParameterData_ = new ZllRecoilCorrectionParameterSet(cfgCorrParameter.getParameter<edm::ParameterSet>("data"));
  corrParameterMC_   = new ZllRecoilCorrectionParameterSet(cfgCorrParameter.getParameter<edm::ParameterSet>("mc"));

  //std::cout << "corr. Parameter(Data):" << std::endl;
  //corrParameterData_->print(std::cout);
  //std::cout << "corr. Parameter(MC):" << std::endl;
  //corrParameterMC_->print(std::cout);

  shiftByUncertainty_ = ( cfg.exists("shiftByUncertainty") ) ?
    cfg.getParameter<double>("shiftByUncertainty") : 0.;
}

ZllRecoilCorrectionAlgorithm::~ZllRecoilCorrectionAlgorithm()
{
  delete corrParameterData_;
  delete corrParameterMC_;
}

std::pair<double, double> ZllRecoilCorrectionAlgorithm::compSigma(const ZllRecoilCorrectionParameterSet& corrParameter, double qT) const
{
  double sigmaU1 = corrParameter.sigma1()*(1. + corrParameter.b1()*qT + corrParameter.c1()*qT*qT);
  double sigmaU2 = corrParameter.sigma2()*(1. + corrParameter.b2()*qT + corrParameter.c2()*qT*qT);
  if ( sigmaU1 < sigmaU1min ) sigmaU1 = sigmaU1min;
  if ( sigmaU2 < sigmaU2min ) sigmaU2 = sigmaU2min;
  return std::pair<double, double> (sigmaU1, sigmaU2);
}

double square(double x)
{
  return x*x;
}

pat::MET ZllRecoilCorrectionAlgorithm::buildZllCorrectedMEt(
           const pat::MET& uncorrectedMEt, 
	   const reco::Candidate::LorentzVector& genMEt, const reco::Candidate::LorentzVector& qGenBoson) const
{
  //std::cout << " uncorrected MEt: px = " << uncorrectedMEt.px() << ", py = " << uncorrectedMEt.py() << std::endl;

  pat::MET correctedMEt(uncorrectedMEt);

  double qX = qGenBoson.px();
  double qY = qGenBoson.py();
  double qT = qGenBoson.pt();

  //std::cout << " qT = " << qT << " (qX = " << qX << ", qY = " << qY << ")" << std::endl;

  if ( qT > 0. ) {

    int errorFlag = 0;
    std::pair<double, double> uTgen = compMEtProjU(qGenBoson, genMEt.px(), genMEt.py(), errorFlag);
    double u1gen = uTgen.first;
    double u2gen = uTgen.second;
    //std::cout << " gen: u1 = " << u1gen << ", u2 = " << u2gen << std::endl;

    std::pair<double, double> uTrec = compMEtProjU(qGenBoson, uncorrectedMEt.px(), uncorrectedMEt.py(), errorFlag);
    double u1rec = uTrec.first;
    double u2rec = uTrec.second;
    //std::cout << " rec: u1 = " << u1rec << ", u2 = " << u2rec << std::endl;

    std::pair<double, double> uTsigmaData = compSigma(*corrParameterData_, qT);
    double u1sigmaData = uTsigmaData.first;
    double u2sigmaData = uTsigmaData.second;
    //std::cout << " sigma(Data): u1 = " << u1sigmaData << ", u2 = " << u2sigmaData << std::endl;
    
    std::pair<double, double> uTsigmaMC = compSigma(*corrParameterMC_, qT);
    double u1sigmaMC = uTsigmaMC.first;
    double u2sigmaMC = uTsigmaMC.second;    
    //std::cout << " sigma(MC): u1 = " << u1sigmaMC << ", u2 = " << u2sigmaMC << std::endl;
    
    double u1sigmaCorr = ( u1sigmaMC != 0. ) ? (u1sigmaData/u1sigmaMC) : 1.;
    double u2sigmaCorr = ( u2sigmaMC != 0. ) ? (u2sigmaData/u2sigmaMC) : 1.;
    
    double u1rec_corrected = (u1rec - u1gen)*u1sigmaCorr + u1gen;         
    double u2rec_corrected = (u2rec - u2gen)*u2sigmaCorr + u2gen;
    
    //std::cout << "--> rec(resolution corrected): u1 = " << u1rec_corrected << ", u2 = " << u2rec_corrected << std::endl;

    double dDiff = corrParameterData_->d() - corrParameterMC_->d();
    double kDiff = corrParameterData_->k1()*0.5*(1.0 - TMath::Erf(-corrParameterData_->k2()*TMath::Power(qT, corrParameterData_->k3()))) 
                  - corrParameterMC_->k1()*0.5*(1.0 - TMath::Erf(-corrParameterMC_->k2()*TMath::Power(qT, corrParameterMC_->k3())));
    u1rec_corrected += dDiff + kDiff*qT;
    
    //std::cout << "--> rec(resolution + response corrected): u1 = " << u1rec_corrected << ", u2 = " << u2rec_corrected << std::endl;
    
    if ( shiftByUncertainty_ != 0. ) {
      //std::cout << " shiftByUncertainty = " << shiftByUncertainty_ << std::endl;
      // CV: uncertainties on "response turn-on" parameters k2 and k3 neglected for now...
      double u1Err2 = 
  	  square(u1rec - u1gen)*(square(corrParameterData_->sigma1Err()/u1sigmaMC)
	*(square(corrParameterData_->b1Err()*qT) + square(corrParameterData_->c1Err()*qT*qT))
       + square(u1sigmaData*corrParameterMC_->sigma1()/square(u1sigmaMC))
        *(square(corrParameterMC_->b1Err()*qT) + square(corrParameterMC_->c1Err()*qT*qT))
       + square(u1sigmaCorr)
        *(square(corrParameterData_->sigma1Err()/corrParameterData_->sigma1()) 
       + square(corrParameterMC_->sigma1Err()/corrParameterMC_->sigma1())));
      u1Err2 += square(corrParameterData_->dErr()) + square(corrParameterMC_->dErr())
       + square(corrParameterData_->k1Err()*qT) + square(corrParameterMC_->k1Err()*qT);
      u1rec_corrected += shiftByUncertainty_*TMath::Sqrt(u1Err2);
      double u2Err2 = 
	 square(u2rec - u2gen)*(square(corrParameterData_->sigma2()/u2sigmaMC)
	*(square(corrParameterData_->b2Err()*qT) + square(corrParameterData_->c2Err()*qT*qT))
       + square(u2sigmaData*corrParameterMC_->sigma2()/square(u2sigmaMC))
	*(square(corrParameterMC_->b2Err()*qT) + square(corrParameterMC_->c2Err()*qT*qT))
       + square(u2sigmaCorr)
	*(square(corrParameterData_->sigma2Err()/corrParameterData_->sigma2()) 
       + square(corrParameterMC_->sigma2Err()/corrParameterMC_->sigma2())));
      u2rec_corrected += shiftByUncertainty_*TMath::Sqrt(u2Err2);
    }
    
    //std::cout << "--> rec(corrected, after shift): u1 = " << u1rec_corrected << ", u2 = " << u2rec_corrected << std::endl;

    double uXrec_corrected = (u1rec_corrected*qX + u2rec_corrected*qY)/qT;
    double uYrec_corrected = (u1rec_corrected*qY - u2rec_corrected*qX)/qT;
    
    double uXrec = (u1rec*qX + u2rec*qY)/qT;
    double uYrec = (u1rec*qY - u2rec*qX)/qT;    
    //std::cout << " uXrec = " << uXrec << ", corrected = " << uXrec_corrected << std::endl;
    //std::cout << " uYrec = " << uYrec << ", corrected = " << uYrec_corrected << std::endl;
    
    double correctedMETpx = correctedMEt.px() - (uXrec_corrected - uXrec);
    double correctedMETpy = correctedMEt.py() - (uYrec_corrected - uYrec);
    double correctedMETpt = TMath::Sqrt(correctedMETpx*correctedMETpx + correctedMETpy*correctedMETpy);
    
    //std::cout << "--> corrected MEt: px = " << correctedMETpx << ", py = " << correctedMETpy << std::endl;

    correctedMEt.setP4(math::XYZTLorentzVector(correctedMETpx, correctedMETpy, 0., correctedMETpt));
  }

  return correctedMEt;
}
