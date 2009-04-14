#ifndef DELTAC_CLASS
#define DELTAC_CLASS
// header
#include <string>
#include <vector>

class bkgbias {

public:
  bkgbias(string idtype);
  Double_t deltac(const Double_t *ptgamma, const Double_t *p);
  Double_t deltac(const Double_t *ptgamma);
  Double_t bkgresp(const Double_t *ptgamma, const Double_t *p);
  Double_t bkgresp(const Double_t *ptgamma);

private:
  std::vector<double> idpars;
};

// code part

// Background bias, depending on photon ID cuts (parameterization in p)
// p has 20 parameters: jr (2+2), jk (3'+3), pr (2+2), pk (3'+3)
Double_t bkgbias::deltac(const Double_t *ptgamma, const Double_t *p) {

  // In data one sees only the calo-gamma pT
  // The parametrizations are functions of either
  // GenJet pT (parton-jet pT) or calogamma pT, so need to map
  double x = *ptgamma;
  double pk1 = (p[14] + log(0.01*x)*(p[15] + log(0.01*x)*p[16]));
  double pk2 = (p[19] - p[17]*pow(x, p[18]-1));
  double pr1 = (1 - p[10]*pow(x, p[11]-1));
  double pr2 = (1 - p[12]*pow(x, p[13]-1));
  double z1 = x / pr1; // GenJet pT~GenGamma pT; change jk vs parton-jet
  double z2 = x / pr2; // GenJet pT~GenGamma pT; change jk vs parton-jet
  double jk1 = (p[4] + log(0.01*z1) * (p[5] + log(0.01*z1)*p[6]));
  double jk2 = (p[9] - p[7]*pow(z2, p[8]-1)); // jk
  double y1 = x / (pr1*pk1) * jk1; // GenJet pT
  double y2 = x / (pr2*pk2) * jk2; // GenJet pT
  double jr1 = (1 - p[0]*pow(y1, p[1]-1));
  double jr2 =  (1 - p[2]*pow(y2, p[3]-1));

  /*
  double b =
    ((1 - p[0]*pow(x, p[1]-1)) / (1 - p[2]*pow(x, p[3]-1))) //jr
    * ((p[4] + log(0.01*x) * (p[5] + log(0.01*x)*p[6]))
       / (p[9] - p[7]*pow(x, p[8]-1))) //jk
    / (((1 - p[10]*pow(x, p[11]-1)) / (1 - p[12]*pow(x, p[13]-1))) //pr
       * ((p[14] + log(0.01*x)*(p[15] + log(0.01*x)*p[16]))
	  / (p[19] - p[17]*pow(x, p[18]-1)))); //pk
  */
  double dc =
    ((jr1 / jr2) * (jk1 / jk2))
    / ((pr1 / pr2) * (pk1 /pk2));

  return dc;
} // deltac

Double_t bkgbias::deltac(const Double_t *ptgamma) {

  return deltac(ptgamma, &idpars[0]);
}

// Return signal response multiplied by pre-fixed DeltaC
Double_t bkgbias::bkgresp(const Double_t *ptgamma, const Double_t *p) {

  double x = *ptgamma;
  double r = p[2] - p[0]*pow(0.01*x, p[1]-1);
  double dc = deltac(ptgamma); 
  
  return (r*dc);
}

Double_t bkgbias::bkgresp(const Double_t *ptgamma) {
  
  double x = *ptgamma; // GenJet pT~CaloGamma pT for signal
  double jr2 = (1 - idpars[2]*pow(x, idpars[3]-1));
  double dc = deltac(ptgamma);

  return (jr2*dc);
}

bkgbias::bkgbias(std::string idtype) {

   const double idpars_loose[20] =
     {2.3319, 0.5995, 1.8806, 0.6198, 0.9239, 0.0313, -0.0047,
      -5.1348, 0.9993, -4.1229, 0.8911, 0.5995, 0.2216, 0.5113,
      0.9236, 0.0313, -0.0047, -0.5847, 0.2653, 0.9924};
   const double idpars_medium[20] =
     {2.3319, 0.5995, 1.8806, 0.6198, 0.9239, 0.0313, -0.0047,
      -5.1348, 0.9993, -4.1229, 0.2260, 0.5995, 0.1008, 0.6009,
      0.8957, 0.0313, -0.0047, -0.1381, 0.5992, 0.9865,};
   const double idpars_tight[20] =
     {2.3319, 0.5995, 1.8806, 0.6198, 0.9239, 0.0313, -0.0047,
      -5.1348, 0.9993, -4.1229, 0.0567, 0.5995, 0.0980, 0.4698,
      0.8618, 0.0313, -0.0047, -0.1707, 0.3690, 0.9922};
   const double idpars_nn[20] =
     {2.3319, 0.5995, 1.8806, 0.6198, 0.9239, 0.0313, -0.0047,
      -5.1348, 0.9993, -4.1229, 0.6667, 0.5995, 1.3657, 0.0457,
      0.9313, 0.0313, -0.0047, -0.2619, 0.4732, 0.9872};
   
   this->idpars.resize(20);
   for (unsigned int i = 0; i != idpars.size(); ++i) {
     if (idtype == "loose")  idpars[i] = idpars_loose[i];
     if (idtype == "medium") idpars[i] = idpars_medium[i];
     if (idtype == "tight")  idpars[i] = idpars_tight[i];
     if (idtype == "NN")     idpars[i] = idpars_nn[i];
   }
} // bkgbias
   
#endif
