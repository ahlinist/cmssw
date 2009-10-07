// Adapted from D0 Experiment jetcorr/src/RjetCorr.cpp

#include "L3Corr.hpp"
#include <math.h>

using namespace std;

L3Corr::L3Corr(const JetAlg& jetalg, 
	       const jec::ErrorTypes& errType,
	       const PhotonID& id) :
  _jetalg(jetalg), _errType(errType), _phoID(id)
{

  _E0 = 100;

  this->SetJetAlg(jetalg); // initialize tables for jetalg
}

// Uncertainty is returned as _absolute_ uncertainty
double L3Corr::Rjet(const double pTprime, double& err)  {

  double rjet = this->_Rjet(pTprime);

  double err2 = 0;

  if (_errType & jec::kL3Sys || _errType & jec::kL3Extra) {
    // Systematics are a relative uncertainty so need to multiply by rjet
    double errSyst = this->_SystErr(pTprime);
    err2 += rjet * rjet * errSyst * errSyst;
  }

  if (_errType & jec::kL3Stat) {
    // Statistics are reported as relative so need to multiply by rjet
    double errStat = this->_StatErr(pTprime);
    err2 += rjet * rjet * errStat * errStat;
  }

  err = sqrt(err2);

  return rjet;
}

void L3Corr::SetJetAlg(const JetAlg& jetalg) {

  _jetalg = jetalg;

  switch (_jetalg) {

  // This uses fit to reconstructed MC
  case L3Corr::IC5_DATA:
    _isMC = false;
    // These are from signal reco MC (medium+deltaeta)
    _par[0] = 0.275; _par[1] = 0.4576; _par[2] = 0.9269;
    // These are from staterr.C toyMC study
    // TRandom3 seed 771156000
    /*
    // CSA07 Signal sample (toyMC 10 pb-1, 1.5-sigma peak):
    _pstat[0] = 0.3473; _pstat[1] = 0.6239; _pstat[2] = 1.004; // [2]+/-0.079
    _chi2 = 0.7393; // 8.9/12
    _eig[0][0] = -0.1097; _eig[1][0] = -0.1064; _eig[2][0]  = -0.1106;
    _eig[0][1] = -0.001698; _eig[1][1] = 0.008222; _eig[2][1]  = -0.006228;
    _eig[0][2] = 0.001132; _eig[1][2] = -0.000357; _eig[2][2]  = -0.00078;
    */
    // Summer08 Signal sample (toyMC 10 pb-1, binned xsec, arithmetic mean):
    _pstat[0] = 0.2675; _pstat[1] = 0.5318; _pstat[2] = 0.9552; // [2]+/-0.097
    _chi2 = 0.4365; // 4.4/10
    _eig[0][0] = -0.09631; _eig[1][0] = -0.1354; _eig[2][0]  = -0.09627;
    _eig[0][1] = -0.004067; _eig[1][1] = 0.008479; _eig[2][1]  = -0.007861;
    _eig[0][2] = 0.001375; _eig[1][2] = -0.0002672; _eig[2][2]  = -0.0009995;
    /*
    // These are from signal reco MC (medium+deltaeta)
    _par[0] = 0.275; _par[1] = 0.4576; _par[2] = 0.9269;
    _pstat[0] = 0.275; _pstat[1] = 0.4576; _pstat[2] = 0.9269;
    _chi2 = 0.5349;
    _eig[0][0] = 0.01217; _eig[1][0] = 0.03932; _eig[2][0]  = 0.01236;
    _eig[0][1] = 0.003087; _eig[1][1] = -0.001454; _eig[2][1]  = 0.001587;
    _eig[0][2] = -0.0003632; _eig[1][2] = -8.516e-05; _eig[2][2]  = 0.0006284;
    */
    /*
    // These are from mixed reco MC (medium+deltaeta)
    _par[0] = 0.2795; _par[1] = 0.4658; _par[2] = 0.931;
    _pstat[0] = 0.2795; _pstat[1] = 0.4658; _pstat[2] = 0.931;
    _chi2 = 0.8428;
    _eig[0][0] = 0.01693; _eig[1][0] = 0.05511; _eig[2][0]  = 0.01752;
    _eig[0][1] = 0.004658; _eig[1][1] = -0.002093; _eig[2][1]  = 0.002083;
    _eig[0][2] = -0.0004272; _eig[1][2] = -0.0001307; _eig[2][2]  = 0.0008241;
    */
    /*
    // This was also supposed to be mixed reco MC (medium) with same settings?
    _par[0] = 0.2848; _par[1] = 0.4885; _par[2] = 0.933;
    _pstat[0] = 0.2848; _pstat[1] = 0.4885; _pstat[2] = 0.933;
    _chi2 = 0.8486;
    _eig[0][0] = -0.02129; _eig[1][0] = -0.06299; _eig[2][0]  = -0.02191;
    _eig[0][1] = -0.00469; _eig[1][1] = 0.002374; _eig[2][1]  = -0.002268;
    _eig[0][2] = -0.0004994; _eig[1][2] = -0.0001395; _eig[2][2]  = 0.0008864;
    */
    break;
  // All other cases use IC5 MC truth until better available
  case L3Corr::SIS5_DATA:
  case L3Corr::SIS7_DATA:
  case L3Corr::KT5_DATA:
  case L3Corr::KT7_DATA:
  //
  case L3Corr::SIS5_MC:
  case L3Corr::SIS7_MC:
  case L3Corr::KT5_MC:
  case L3Corr::KT7_MC:
  //
  case L3Corr::IC5_MC:
    _isMC = true;
    // currently MC truth
    // "1 - 2.316*pow(x,0.6005-1)"
    // => "1 - 0.3679*pow(pT/100., 0.6005-1)
    _par[0] = 0.3679; _par[1] = 0.6005; _par[2] = 1.;
    // These are from signal reco MC (medium+deltaeta)
    _pstat[0] = 0.275; _pstat[1] = 0.4576; _pstat[2] = 0.9269;
    _chi2 = 0.5349;
    _eig[0][0] = 0.01217; _eig[1][0] = 0.03932; _eig[2][0]  = 0.01236;
    _eig[0][1] = 0.003087; _eig[1][1] = -0.001454; _eig[2][1]  = 0.001587;
    _eig[0][2] = -0.0003632; _eig[1][2] = -8.516e-05; _eig[2][2]  = 0.0006284;
    /*
    _pstat[0] = 0.2869; _pstat[1] = 0.4872; _pstat[2] = 0.9356;
    _chi2 = 0.8321;
    _eig[0][0] = -0.01227; _eig[1][0] = -0.03484; _eig[2][0]  = -0.01241;
    _eig[0][1] = -0.002727; _eig[1][1] = 0.001456; _eig[2][1]  = -0.001392;
    _eig[0][2] = -0.0003259; _eig[1][2] = -8.209e-05; _eig[2][2]  = 0.0005524;
    */
    break;
  default:
    cout << "Unknown jet algorithm!" << endl << flush;
    exit(1);
  };
}

void L3Corr::SetErrType(const jec::ErrorTypes& errType) {
  _errType = errType;
}

void L3Corr::SetPhoID(const PhotonID& id) {
  _phoID = id;
}

jec::ErrorTypes L3Corr::GetErrType() {
  return _errType;
}


double L3Corr::_Rjet(const double pTprime) const {

  return _powerlaw(pTprime, _par);
}


// NB: Systematic errors are reported as _relative_ uncertainties
double L3Corr::_SystErr(const double pTprime) const {

  if (pTprime); // supress warning
  double err2 = 0;
 
  if (_errType & jec::kL3PhotonES) {

    // Photon energy scale error from the PhotonID group
    // The 2% estimate on EM scale mentioned at the photon ID workshop
    // in autumn 2007. Relative photon / electron scale is less than about 0.5%
    // Consult photon ID group again once we get data 
    double errEMScale = 0.02;
    double errPhotonRel = 0.005;

    err2 += errEMScale * errEMScale + errPhotonRel * errPhotonRel;
  }

  if (_errType & jec::kL3QCDBackground) {

    // Fit purity from output_mixed_all.root purity;2
    // - Take difference between medium, tight and loose as a conservative
    //   photon ID systematic for P and dC
    // - Assign 10% uncertainty on QCD and photon+jet relative
    //   NLO cross sections (better estimate from Tevatron papers) for P
    double P = _purity(pTprime, _phoID);//kMedium);//0.9;
    double errP = _SystPurity(pTprime, _phoID);//kMedium);//0.1
    double errdC; // 0.1
    double dC = _deltaC(pTprime, errdC, _phoID);//kMedium); // 0.1
    //errdC *= (1+dC) * errdC; // BUG, multiplied by errdC (fixed 12-19-2008)
    errdC *= (1+dC); // turn into absolute uncertainty

    double errQCD_P = errP * fabs(dC);
    double errQCD_dC = (1-P) * errdC;
    double errQCD_cross = errP * errdC;

    // Quadratic addition assuming errP and dC fully unforrelated
    err2 += errQCD_P * errQCD_P + errQCD_dC * errQCD_dC
      + errQCD_cross * errQCD_cross;
    // Linear addition assuming full anticorrelation dC=-errP
    //err2 += pow(-errQCD_P + errQCD_dC - errQCD_cross, 2);
  }

  if (_errType & jec::kL3Parton) {

    // photon+jet balanced at parton level: photon already at parton level,
    // but jet not, so account for uncertainty in jet parton correction
    // Best numbers probably from Attilio's study looking into difference
    // in parton correction between Pythia and Herwig, and between Pythia tunes
    // One piece is effectively coming from fragmentation modeling (showering),
    // the other from underlying event modeling
    // - temporarily use 100% of jet showering in photon+jets
    // NB: disagreement with Attilio's results, should be 2-3% bigger effect
    // [- estimate UE at ~0.5 GeV per 0.7 cone; take 100% as uncertainty
    //    (D0 estimated UE at ~0.2 GeV per 0.7 cone at reco level,
    //     which is around 2.5 times more at generator level)]
    // - QCD group's CR2008/032 gives dN/dphideta~1.7, dpT/dphideta~2.2-3
    //   for charged tracks (I guess charged+neutral is 1.5 times larger)
    //   in Fig. 3; however, Fig. 4 has significantly lower UE of
    //   dpT/dphideta~0.5-1.5, dN~0.5 for drell-Yan? See CMS Note 2006/067
    //   => on average one charged track with 0.75 GeV in 0.5 cone for dN~1.5
    //      guess dpT/dphideta~1.5, take 50% uncertainty
    // NB: UE apparently only affects the mean, not the peak!!
    if (_errType & jec::kL3PartonFrag) {

      //double kjets = -4.123 + 5.135 * pow(pTprime, -0.0006598);
      //double dkjet = 1.00 * (kjets - 1.);
      double dkjet = 0.50 * (_partonFrag(pTprime) - 1.);

      err2 += dkjet * dkjet;
    }
    if (_errType & jec::kL3PartonUE) {

      // 50% * [(charged+neutral)/charged=1.5] * [dpT/dphideta=1.5] * pi * R2
      //double due = 0.50 * 1.5 * 1.5 * 3.14 * 0.5 *0.5 / pTprime;
      double due = 0.50 * (_partonUE(pTprime) - 1.);

      err2 += due * due;
    }
  }

  if (_errType & jec::kL3HardGluon) {

    // This is the uncertainty on the cut on second jet pT
    // Due to different response in data and MC this cut may be
    // effectively different
    // Test the sensitivity by varying the cut by a large factor
    // (x2 for ratio, x1.5 for pT threshold)
    // These numbers are for mixed sample
  //double rjet  = 0.9310 - 0.2795*pow(pTprime, 0.4658-1.); //pT2<0.10*pT,10GeV
  //double rjet2 = 0.8977 - 0.2783*pow(pTprime, 0.3471-1.); //pT2<0.20*pT,15GeV
  //double rjet3 = 0.9477 - 0.2877*pow(pTprime, 0.4931-1.); //pT2<0.05*pT,6GeV
    // These are for pure signal sample (more stable for rjet2)
  //double rjet  = 0.9269 - 0.2750*pow(pTprime, 0.4576-1.); //pT2<0.10*pT,10GeV
  //double rjet2 = 0.9868 - 0.3509*pow(pTprime, 0.6204-1.); //pT2<0.20*pT,20GeV
  //double rjet3 = 0.9459 - 0.2859*pow(pTprime, 0.4928-1.); //pT2<0.05*pT,5GeV
    // These are for pure signal with deltaeta cut
    double x = 0.01*pTprime; // BUG, forgot 0.01 before (fixed 12-19-2008)
    //double rjet05 = 0.9391 - 0.2758*pow(x,0.4703-1.); //pT2<0.05*pT,5GeV
    //double rjet10 = 0.9269 - 0.2750*pow(x,0.4576-1.); //pT2<0.10*pT,10GeV
    double rjet05 = 0.9750 - 0.3069*pow(x,0.5749-1.); //pT2<0.05*pT,2.5GeV
    double rjet10 = 0.9603 - 0.3048*pow(x,0.5589-1.); //pT2<0.10*pT,2.5GeV
    //double rjet20 = 0.9853 - 0.3465*pow(x,0.6151-1.); //pT2<0.20*pT,20GeV
    //double eg = 0.5 * (fabs(rjet05/rjet10 - 1.) + fabs(rjet20/rjet10 - 1.));
    double eg = fabs(rjet05/rjet10 - 1.);
    // eg *= 0.5; // estimate for 1 fb-1

    err2 += eg * eg;
  }

  if (_errType & jec::kL3Flavor) {
    
    // This uncertainty covers the flavor mapping uncertainty
    // when transferring the photon+jet response (quark dominated)
    // to the default QCD response (gluon dominated)
    // Take 50% as uncertainty; at D0 the default MC was off by 100%
    // in this regard, but CMS should do better

    double drjet = 0.5 * (_flavorMap(pTprime) - 1.);
    // drjet *= 0.5; /// estimate for 1 fb-1

    err2 += drjet * drjet;
  }

  double err = sqrt(err2);

  return err;
}

// NB: Statistical uncertainty is reported as relative uncertainty
double L3Corr::_StatErr(const double pTprime) const {

  if (pTprime); // supress warning
  double err2 = 0;

  if (_errType & jec::kL3Stat) {

    /*
    // Statistical uncertainty per 25 GeV bin
    const double lumi = 0.010; // 10 pb-1 = 0.010 fb-1
    const double N = 1., S = 2., C = 0.06;
    double x = pTprime;
    double res = sqrt(N*N/(x*x) + S*S/x + C*C);
    double npfb = 7.729e+10*pow(x,-3.104)*exp(-0.003991*x); // events per fb-1
    double n = lumi * npfb;
    //double stat = res / sqrt(n);

    err2 += res * res / n;
    */

    // Calculate stat uncertainty from fit (provided as rel. eigenfunctions)
    double e0 = _eigpowerlaw(pTprime, _pstat, &_eig[0], 0);
    double e1 = _eigpowerlaw(pTprime, _pstat, &_eig[0], 1);
    double e2 = _eigpowerlaw(pTprime, _pstat, &_eig[0], 2);
    
    err2 += max(_chi2, 1.) * (e0 * e0 + e1 * e1 + e2 * e2); 

    /*
    // staterr directly
    double f
		 "sqrt(pow(0.01*x,2*[1]-2)*[2]"
		 "+2*pow(0.01*x,2*[1]-2)*[0]*log(0.01*x)*[3]"
		 "-2*pow(0.01*x,[1]-1)*[4]"
		 "+pow(0.01*x,2*[1]-2)*[0]*[0]*log(0.01*x)*log(0.01*x)*[5]"
		 "-2*pow(0.01*x,[1]-1)*[0]*log(0.01*x)*[6]"
		 "+[7])",
    */
  }

  double err = sqrt(err2);

  return err;
}

// Photon+jet purity (currently for CSA07 => update)
double L3Corr::_purity(const double pTprime, const PhotonID& id) const {

  double P = 1.;
  double x = log(0.01*pTprime);
  // The 'no deltaeta' were fitted by hand from output_mixed_all.root
  // from plots in the range 30-700 GeV
  if (id == kLoose) {
    P = 0.688 + x*(0.126 + x*-0.0265); // Summer08 - May 31 fakerate
    //P = 0.7094 + x*(-0.03399 + x*0.06281); // Summer08 - May 31
    //P = 0.6339 + x*(0.213 + x*-0.06296); // deltaeta, no rebin
    //P = 0.5514 + x*(0.2773 + x*-0.06944); // deltaeta, automatic
    //P = 6.11287e-01 + x*(1.80336e-01 + x*-1.69342e-02); // no deltaeta?
  }
  if (id == kMedium) {
    P =  0.837 + x*(0.105 + x*-0.0408); // Summer08 - May 31 fakerate
    //P = 0.984 + x*(-0.02309 + x*-0.006556); // Summer08 - May30
    //P = 0.9673 + x*(-0.01242 + x*0.01187); // deltaeta, no rebin
    //P = 9.66655e-01 + x*(-1.28295e-02 + x*1.29304e-02); // no deltaeta?
  }
  if (id == kTight) {
    P = 0.864 + x*(0.11 + x*-0.0477); // Summer08 - May 31 fakerate
    //P = 0.846 + x*(0.0995 + x*-0.0392); // Summer08 - May 31 fakerate
    //P = 0.9977 + x*(-0.001804 + x*-0.009779); // Summer08 - May 31
    //P = 0.9797 + x*(-0.006598 + x*0.004506); // deltaeta, no rebin
    //P = 9.78925e-01 + x*(-7.17726e-03 + x*5.88710e-03); // no deltaeta?
  }
  if (id == kMedium005) {
    P = 0.9921 + x*(-0.0128 + x*0.005335); // deltaeta, no rebin
    //P = 0.9876 + x*(-0.03873 + x*0.01767);
  }
  if (id == kMedium010) { // copy of kMedium
    P = 0.9673 + x*(-0.01242 + x*0.01187); // deltaeta, no rebin
  }
  if (id == kMedium020) {
    P = 0.9581 + x*(-0.01365 + x*0.007251); // deltaeta, no rebin
    //P = 0.6982 + x*(0.2757 + x*-0.08513);
  }

  if (P<0) P = 0;
  if (P>1) P = 1;

  return P;
}

// NB: all purity uncertainties are _absolute_ values
double L3Corr::_SystPurity(const double pTprime, const PhotonID& id) const {

  double err2 = 0;
  
  if (_errType & jec::kL3PurityStat)
    err2 += pow(_StatPurity(pTprime, id),2);

  if (!_isMC && (_errType & jec::kL3PurityID))
    err2 += pow(_SystPurityID(pTprime, id),2);

  if (!_isMC && (_errType & jec::kL3PurityXsec))
    err2 += pow(_SystPurityXsec(pTprime, id), 2);

  /*
  if (!_isMC && (_errType & jec::kL3Purity2ndJet)) {
    err2 += pow(_SystPurity2ndJet(pTprime, id), 2);
  }
  */

  double err = sqrt(err2);
		    
  return err;
}

// Systematic on photon+jet purity from photon ID
// (average difference between medium, loose and tight, but could use
//  more detailed estimates for each cut also)
// => Change the impact to 30% of the difference, now it seems too large
//    for medium ID
double L3Corr::_SystPurityID(const double pTprime, const PhotonID& id) const {

  //double dP = 0.5*(fabs(_purity(pTprime, id) - _purity(pTprime, kLoose))
  //	   + fabs(_purity(pTprime, id) - _purity(pTprime, kTight)));

  double dP = 0.30*(fabs(_purity(pTprime, id) - _purity(pTprime, kLoose))
		    + fabs(_purity(pTprime, id) - _purity(pTprime, kTight)));
  
  return dP;
}

// Systematic on photon+jet purity from NLO cross sections
// Assume ratio of QCD and photon+jet is known to 10%
// See e.g. arXiv:/0804.1107 [hep-ex] (photon+jet)
//          arXiv:/0802.2400 [hep-ex] (inclusive jet~dijet)
// => 20% uncertainty on B/S (theory has 10%, data/theory discrepancy 20%)
// P' = P / (P + x*(1-P)), where x = B'/B
// dP'/dx(at x=1) = -(1-P)*P
double L3Corr::_SystPurityXsec(const double pTprime, const PhotonID& id) const{

  double P = _purity(pTprime, id);
  double dxsec = 0.2;

  return (dxsec * (1-P) * P);
}

/*
double L3Corr::_SystPurity2ndJet(const double pTprime,
				 const PhotonID& id) const {

  if (id); // Only for medium now, but suppress error
  double P = _purity(pTprime, kMedium);
  double P2 = _purity(pTprime, kMedium005);
  double P3 = _purity(pTprime, kMedium020);
  double err = 0.5 * (fabs(P2/P-1.) + fabs(P3/P-1.));

  return err;
}
*/

// Statistical uncertainty from MC fit correlation matrix
// (quadratic logarithmic fit)
double L3Corr::_StatPurity(const double pTprime, const PhotonID& id) const {

  double x = log(0.01*pTprime); if (x);
  double stat = 0;
  if (id == kLoose)
    stat = 0.07743; // top of err.bars / Summer08 - May 31
  //stat = 0.08077; // top of medium err.bars
//     stat = sqrt(0.0009192 + 2*-0.0002421*x + 2*-0.0002416*x*x
// 		+ 0.002093*x*x + 2*-0.001193*x*x*x
// 		+ 0.0009278*x*x*x*x);
  if (id == kMedium)
    stat = 0.05699; // top of err.bars, Summer08 - May 30
    //stat = 0.08077; // top of err.bars
  //stat = sqrt(0.0003825 + 2*2.183e-05*x + 2*-0.0001547*x*x
  //	+ 0.0005671*x*x + 2*-0.0003245*x*x*x
  //	+ 0.0002734*x*x*x*x); // deltaeta, no rebin
  if (id == kTight)
    stat = 0.05923; // top of err.bars / Summer08 - May 31
    //stat = 0.08077; // top of medium err.bars
//     stat = sqrt(0.0004265 + 2*-1.484e-05*x + 2*-0.0001517*x*x
// 		+ 0.0008268*x*x + 2*-0.0004603*x*x*x
// 		+ 0.0003639*x*x*x*x); // deltaeta, no rebin
  if (id == kMedium005)
    stat = 0.08077; // top of medium err.bars
//     stat = sqrt(0.0005035 + 2*-2.595e-05*x + 2*-0.0001639*x*x
// 		+ 0.001006*x*x + 2*-0.0005597*x*x*x
// 		+ 0.0004097*x*x*x*x); // deltaeta, no rebin
  if (id == kMedium010) // copy of kMedium
    stat = 0.08077; // top of err.bars
  if (id == kMedium020)
    stat = 0.08077; // top of medium err.bars
//     stat = sqrt(0.0003208 + 2*2.855e-05*x + 2*-0.0001461*x*x
// 		+ 0.000454*x*x + 2*-0.0002643*x*x*x
// 		+ 0.0002498*x*x*x*x); // deltaeta, no rebin

  return stat;
}

// Returns deltaC and _relative_ uncertainties (curretly for CSA07 => update)
double L3Corr::_deltaC(const double pTprime, double& syserr,
		       const PhotonID& id,
		       double (*syserrs)[6]) const {
  
  double x = log(0.01*pTprime);
  double y = pTprime;
  //double rjets = 1. - 1.881 * pow(y, -0.3802);
  //double rjetb = 1. - 2.332 * pow(y, -0.4005);
  //double kjets = -4.123 + 5.135 * pow(y, -0.0006598);
  //double kjetb = 0.9239 + x * (0.0313 + x * -0.004696);
  //double kjetb_p = 0.9726 + x * (0.01841 + x * -0.003336);
  //double ktopos = 0.9973 + x * (-0.0005675 + x * 0);
  //double ktopob = 1.002 + x * (-0.005414 + x * 0.001552);
  // Summer08 - May 30
  double rjets = 1 - 2.063 * pow(y, -0.4141);
  double rjetb = 1 - 2.506 * pow(y, -0.4241);
  double kjets = 0.9844 + x*(0.0007429 + x*-0.0004443);
  //double kjets_p = 0.9984 - 26.29 * pow(y, -1.901);
  double kjetb = 0.9357 + x * (0.04055 + x * -0.00881);
  double kjetb_p = 0.967 + x * (0.02833 + x * -0.006476);
  double ktopos = 0.9911 + x * (0.002791 + x * 0);
  double ktopob = 0.9854 + x * (0.01565 + x * -0.004435);

  
  double rphos=1., rphob=1., kphos=1., kphob=1.;
  if (id == kLoose) {
    //rphos = 1 - 0.007997 * pow(y, 0.07654);
    //rphob = 1 - 0.8911 * pow(y, -0.4005);
    //kphos = 1 - 1.877e-12 * pow(y, -0.4549);
    //kphob = 0.9236 + x * (0.0313 + x * -0.004696);
    // Summer08 - May30
    rphos = 1 - 0.003092 * pow(y, 0.306);
    rphob = 1 - 0.6651 * pow(y, -0.4241);
    kphos = 1 - 2.538e-09 * pow(y, -0.4449);
    kphob = 0.9381 + x * (0.04055 + x * -0.00881);
  }
  if (id == kMedium
      || id == kMedium005 || id == kMedium010 || id == kMedium020) {
    //rphos = 1 - 0.003445 * pow(y, 0.1838);
    //rphob = 1 - 0.226 * pow(y, -0.4005);
    //kphos = 1 - 1.397e-12 * pow(y, -0.4546);
    //kphob = 0.8957 + x * (0.0313 + x * -0.004696);
    // Summer08 - May 30
    rphos = 1 - 0.002257 * pow(y, 0.3319);
    rphob = 1 - 0.3692 * pow(y, -0.4241);
    kphos = 1.;//1 - -1.271e-08 * pow(y, -0.4451);
    kphob = 0.9124 + x * (0.04055 + x * -0.00881);
  }
  if (id == kTight) {
    //rphos = 1 - 0.07228 * pow(y, -0.4365);
    //rphob = 1 - 0.6667 * pow(y, -0.4005);
    //kphos = 1 - 3.707e-12 * pow(y, -0.4573);
    //kphob = 0.9313 + x * (0.0313 + x * -0.004696);
    // Summer08 - May 30
    rphos = 1 - 0.001986 * pow(y, 0.3362);
    rphob = 1 - 0.2638 * pow(y, -0.4241);
    kphos = 1 - -1.395e-08 * pow(y, -0.4443);
    kphob = 0.8993 + x * (0.04055 + x * -0.00881);
  }
  
  double dC = ((rjetb * kjetb * ktopob) / (rphob * kphob))
    / ((rjets * kjets * ktopos) / (rphos * kphos)) - 1.;
  
  double err2 = 0;

  // Statistical uncertainty of the DeltaC fit in MC
  // Get the shape from factorized corrections which has good
  // effective statistics and fit to the total deltaC
  // Allow some scaling in both energy dependence and magnitude:
  // dC = a*dC0 + b
  // SOURCE: MC statistics
  // NB: Currently just placeholder
  if (_errType & jec::kL3DeltaCStat) {
    double stat = 0.02;//5;
    err2 += stat * stat;
    if (syserrs) (*syserrs)[0] = stat;
  }

  // EM-jet/photon response
  // => 50% of the difference between EM-jet and a real photon in MC
  //    (CMS EM shower MC simulation not yet certified on data)
  // SOURCE: detector simulation of EM shower (+photonID cuts)
  if (_errType & jec::kL3DeltaCRphot) {
    double drpho = 0.50 * (rphob / rphos - 1.);
    err2 += drpho * drpho;
    if (syserrs) (*syserrs)[1] = drpho;
  }

  // EM-jet/photon showering compared to jet showering
  // - normally they'd cancel for QCD, but not with very tight ID
  // => 100% of the difference observed in dijet MC (not well understood)
  // SOURCE: photon ID cuts and fragmentation simulation?
  // NB: Could study this by only applying isolation cuts in PhotonID,
  //     but not H/E (would need hollow cones...) and cluster shape
  if (_errType & jec::kL3DeltaCKphot) {
    double dkb = 1.00 * (kphob / kjetb - 1.);
    err2 += dkb * dkb;
    if (syserrs) (*syserrs)[2] = dkb;
  }
  /*
  //    + 50% of the jet showering observed in photon+jet MC (not corrected)
  // NB: photon showering = 0 by definition and jet absolute showering goes
  //     to another signal uncertainty source => avoid double counting
  if (_errType & jec::kL3DeltaCKphotSig) {
    double dks = 0.50 * (kphos / kjets - 1.);
    if (syserrs) (*syserrs)[3] = dks;
    err2 += dks * dks;
  }
  */

  // Jet response (quark and gluon jets different)
  // => 50% of the difference between photon+jet and dijet MC
  // (at D0 data showed twice as big an effect as MC, but CMS MC
  //  should work better)
  // SOURCE: detector simulation of single pion response
  if (_errType & jec::kL3DeltaCRjet) {
    double drjet = 0.50 * (rjetb / rjets - 1.);
    err2 += drjet * drjet;
    if (syserrs) (*syserrs)[3] = drjet;
  }

  // Jet showering (quark and gluon jets different)
  // => 50% of the difference between photon+jet and dijet MC
  // (replace with a Pythia/Herwig difference from Attilio's
  //  L7 parton corrections)
  // NB: jet showering for QCD cancels with EM-jet, except for
  //     uncertainty already counted in kL3DeltaCKphot;
  //     signal showering is a separate uncertainty
  //     => set this to 0. to avoid double-counting
  // SOURCE: fragmentation simulation
  if (_errType & jec::kL3DeltaCKjet) {
    double dkjet = 0.;//0.50 * (kjetb / kjets - 1.);
    err2 += dkjet * dkjet;
    if (syserrs) (*syserrs)[4] = dkjet;
  }

  // Difference between mean and peak (MPV) for dijet showering
  // While the difference should survive for the probe jet, it is
  // not obvious that it is fully preserved for the EM-jet due
  // to bin-to-bin migration effects => increases showering difference
  // => 100% of the difference between mean and peak
  // SOURCE: fragmentation simulation + accounting of migration effects?
  if (_errType & jec::kL3DeltaCKjetPeak) {
    double dpeak = kjetb_p / kjetb - 1.;
    err2 += dpeak * dpeak;
    if (syserrs) (*syserrs)[5] = dpeak;
  }
  
  syserr = sqrt(err2);

  return dC;
}

double L3Corr::_deltaRjet(const double pTprime, const PhotonID& id) const {

  double P = _purity(pTprime, id);
  double errdC = 0;
  double dC = _deltaC(pTprime, errdC, id);
  double deltaRjet = (1-P)*dC;

  return deltaRjet;
}

double L3Corr::_parton(const double pTprime) const {

  return (_partonFrag(pTprime) * _partonUE(pTprime));
}

double L3Corr::_partonFrag(const double pTprime) const {

  // kjets = hadronization * underlying event
  //double kjets = -4.123 + 5.135 * pow(pTprime, -0.0006598);
  double y = log(0.01*pTprime);
  double kjets = 0.9844 + y*(0.0007429 + y*-0.0004443); // Summer 08, May 30

  return kjets / _partonUE(pTprime);
}

double L3Corr::_partonUE(const double pTprime) const {

  // [(charged+neutral)/charged=1.5] * [dpT/dphideta=1.5] * pi * R2
  //double due = 0.50 * 1.5 * 1.5 * 3.14 * 0.5 *0.5 / pTprime; //BUG 26-01-2009
  double due = 1.5 * 1.5 * 3.14 * 0.5 *0.5 / pTprime; // estimated UE

  return (1. + due);
}

double L3Corr::_flavorMap(const double pTprime) const {

  // These should be the same as inside DeltaC
  //double rjets = 1. - 1.881 * pow(pTprime, -0.3802);
  //double rjetb = 1. - 2.332 * pow(pTprime, -0.4005);  

  //return (rjetb / rjets);
  return (_RjetTruthQCD(pTprime) / _RjetTruth(pTprime));
}

double L3Corr::_powerlaw(const double& pTprime, const double* par) const {  

  return (_par[2] - par[0] * pow(0.01*pTprime, par[1]-1));
}

double L3Corr::_eigpowerlaw(const double& pTprime, const double* par,
			    const double (*eig)[npar], const int& ieig) const {

  // Partial derivatives of 'p2 - p0*pow(0.01*pTprime, p1-1)'
  // w.r.t. the parameters times eigenvectors give the eigenfunctions
  double f = _powerlaw(pTprime, par);
  double df0 = -pow(0.01*pTprime, par[1]-1);
  double df1 = -par[0] * log(0.01*pTprime) * pow(0.01*pTprime, par[1]-1);
  double df2 = 1.;

  //return (eig[ieig][0]*df0 + eig[ieig][1]*df1 + eig[ieig][2]*df2) / f;
  return (eig[0][ieig]*df0 + eig[1][ieig]*df1 + eig[2][ieig]*df2) / f;
  
}

double L3Corr::_Rbias(const double pT, const PhotonID& id) const {
  
  return (_RbiasBkg(pT, id) * _RbiasPhot(pT, id) * _RbiasTopo(pT, id)
	  * _RbiasParton(pT, id) * _RbiasBalance(pT) * _RbiasPeak(pT));
}

// This equals in principle _deltaRjet, but is not factorized
double L3Corr::_RbiasBkg(const double pT, const PhotonID& id) const {

  return (1+_deltaRjet(pT, id));
  //return (_RjetMix(pT,id) / _RjetReco(pT,id));
}

// Correct for residual photon scale in summer08, to make sure Rphot==1.000
// (This applies for the mean of the photon response, not peak)
double L3Corr::_RbiasPhot(const double pT, const PhotonID& id) const {

  return (1. / _Rphoton(pT, id) );
}

// This could be factorized more, now its just a fit of the difference
// between photon+jet MC truth and reconstructed MC
// Unfortunately, separate fits don't extrapolate too well at pT<60 GeV...
double L3Corr::_RbiasTopo(const double pT, const PhotonID& id) const {

  //return (_RjetReco(pT,id) / _RjetTruth(pT) * _Rphoton(pT,id) );
  return ( (_RjetReco(pT,id) / _RjetTruth(pT)) / _RbiasPhot(pT,id)
	   / _RbiasParton(pT, id) / _RbiasBalance(pT));
}

// Correct for the ratio of GenJet and jet parton pT. This folds together
// underlying event, hadronization and final state radiation. The first
// two are considered in the systematics for parton corrections, the
// latter one is implicitly included in the topology bias systematics
// through sensitivity to secondary jet cut
double L3Corr::_RbiasParton(const double pT, const PhotonID &id) const {
  
  // use the medium ID value for now, will have to later update
  // at least for different secondary jet cuts
  if (id);
  //double x = log(0.01*pT);
  //double kjets = 0.9844 + x*(0.0007429 + x*-0.0004443); // Summer08 - May 30

  //return kjets;
  return _parton(pT);
}

// Ratio of jet and photon parton pT in bins of pThat
// Leave this to 1.00 for now so the effect will be automatically
// included in the topology bias
double L3Corr::_RbiasBalance(const double pT) const {

  if (pT);
  return 1;
}

// Difference between peak and arithmetic mean in photon+jet MC truth
double L3Corr::_RbiasPeak(const double pT) const {

  if (pT);
  return 1.; // comparing to mean for now
}

// Response in reco MC for mixed sample
double L3Corr::_RjetMix(const double pT, const PhotonID& id) const {

  double p[3] = {0, 0, 0};
  //p[0] = 0.2795; p[1] = 0.4658; p[2] = 0.931; // 50 GeV-> fit, pT2>10 GeV
  //p[0] = 0.3266; p[1] = 0.5940; p[2] = 0.9817; // 30 GeV-> fit, pT2>2.5 GeV
  
  // All fits from 33 GeV upwards, with pTlead>10 GeV
  if (id==kLoose) {
    //p[0] = 0.2606; p[1] = 0.5256; p[2] = 0.9409; // pT2>8 GeV
    p[0] = 0.3512; p[1] = 0.5963; p[2] = 1.027; // Summer08 - May 31
  }
  if (id==kMedium) {
    //p[0] = 0.2986; p[1] = 0.5242; p[2] = 0.9510; // pT2>8 GeV, CSA07
    //p[0] = 0.2908; p[1] = 0.4725; p[2] = 0.9673; // Summer08
    p[0] = 0.3201; p[1] = 0.5477; p[2] = 0.9894; // Summer08 - May 30
  }
  if (id==kTight) {
    //p[0] = 0.2811; p[1] = 0.4970; p[2] = 0.9318; // pT2>8 GeV
    p[0] = 0.3458; p[1] = 0.5786; p[2] = 1.007; // Summer08 - May 30
  }
  if (id==kMedium005) {
    p[0] = 0.3048; p[1] = 0.5616; p[2] = 0.9722; // pT2>2.5 GeV
  }
  if (id==kMedium010) {
    p[0] = 0.3125; p[1] = 0.5668; p[2] = 0.9676; // pT2>2.5 GeV
  }
  if (id==kMedium020) {
    p[0] = 0.3602; p[1] = 0.6246; p[2] = 0.9989; // pT2>2.5 GeV
  }

  return (p[2] - p[0] * pow(0.01*pT, p[1]-1));
}

// Response in reco MC for pure photon+jet sample
double L3Corr::_RjetReco(const double pT, const PhotonID& id) const {

  double p[3] = {0, 0, 0};
  //p[0] = 0.2751; p[1] = 0.4577; p[2] = 0.927; // 50 GeV-> fit, pT2>10 GeV
  //p[0] = 0.3048; p[1] = 0.5589; p[2] = 0.9603; // 30 GeV-> fit, pT2>10 GeV

  // All fits from 33 GeV upwards, with pTlead>10 GeV
  if (id==kLoose) {
    //p[0] = 0.2902; p[1] = 0.5211; p[2] = 0.9466; // pT2>8 GeV
    p[0] = 0.3024; p[1] = 0.5188; p[2] = 0.9746; // Summer08 - May31
  }
  if (id==kMedium) {
    //p[0] = 0.2853; p[1] = 0.5021; p[2] = 0.9387; // pT2>8 GeV, CSA07
    //p[0] = 0.2958; p[1] = 0.4875; p[2] = 0.9724; // Summer08
    p[0] = 0.3063; p[1] = 0.5251; p[2] = 0.9758; // Summer08 - May 30
  }
  if (id==kTight) {
    //p[0] = 0.2803; p[1] = 0.4927; p[2] = 0.9306; // pT2>8 GeV
    p[0] = 0.3202; p[1] = 0.5440; p[2] = 0.9830; // Summer08 - May 31
  }
  if (id==kMedium005) {
    //p[0] = 0.2954; p[1] = 0.5480; p[2] = 0.9630; // pT2>2.5 GeV 
    p[0] = 0.281; p[1] = 0.466; p[2] = 0.961; // Summer08 - May 30
  }
  if (id==kMedium010) {
    //p[0] = 0.2958; p[1] = 0.5377; p[2] = 0.951; // pT2>2.5 GeV
    p[0] = 0.298; p[1] = 0.500; p[2] = 0.967; // Summer08 - May 30
  }
  if (id==kMedium020) {
    //p[0] = 0.3354; p[1] = 0.5989; p[2] = 0.9732; // pT2>2.5 GeV
    p[0] = 0.322; p[1] = 0.540; p[2] = 0.972; // Summer08 - May 30
  }

  return (p[2] - p[0] * pow(0.01*pT, p[1]-1));
}

// Response in MC truth for photon+jet (pThat binning, vs <pTgamma>)
// Currently arithmetic mean, not peak value
double L3Corr::_RjetTruth(const double pT) const {

  // This should be the same as inside DeltaC
  //return (1. - 1.881 * pow(pT, -0.3802)); // CSA07
  //return (1. - 2.228 * pow(pT, -0.4267)); // Summer08
  return (1 - 2.063 * pow(pT, -0.4141)); // Summer08 - May 30
}

// Response in MC truth for QCD dijet (pThat binning, vs <pTgamma>)
// Currently arithmetic mean, not peak value
double L3Corr::_RjetTruthQCD(const double pT) const {

  // This should be the same as inside DeltaC
  //return (1. - 2.332 * pow(pT, -0.4005)); // CSA07
  //return (1. - 2.833 * pow(pT, -0.4516)); // Summer08
  return (1 - 2.506 * pow(pT, -0.4241)); // Summer08 - May 30
}

// Photon response in Summer08 MC
// Currently arithmetic mean, not peak value
double L3Corr::_Rphoton(const double pT, const PhotonID& id) const {

  if (id); // Maybe use later, now only medium cuts
  //return (1. - 0.005502 * pow(pT, 0.232));
  return (1 - 0.002257 * pow(pT, 0.3319)); // Summer08 - May 30
}
