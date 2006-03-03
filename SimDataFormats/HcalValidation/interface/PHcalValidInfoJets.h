///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoJets.h
// Persistent Jet informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoJets_H
#define  PHcalValidInfoJets_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;

class PHcalValidInfoJets {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoJets(): nJetHit(0), nJet(0), ecalJet(0.), hcalJet(0.),
			hoJet(0.), etotJet(0.), detaJet(0.), dphiJet(0.),
			drJet(0.), dijetM(0.) {}
  virtual ~PHcalValidInfoJets() {}

  // acceess

  std::vector<float> jethite() const {return jetHite;}
  std::vector<float> jethitr() const {return jetHitr;}
  std::vector<float> jethitt() const {return jetHitt;}

  std::vector<float> jete()    const {return jetE;}
  std::vector<float> jeteta()  const {return jetEta;}
  std::vector<float> jetphi()  const {return jetPhi;}

  float              ecaljet() const {return ecalJet;}
  float              hcaljet() const {return hcalJet;}
  float                hojet() const {return   hoJet;}
  float              etotjet() const {return etotJet;}

  float              detajet() const {return detaJet;}
  float              dphijet() const {return dphiJet;}
  float                drjet() const {return   drJet;}
  float               dijetm() const {return  dijetM;}

  // fill
  void fillTProfileJet      (double e, double r, double t);
  void fillEcollectJet      (double ee, double he, double hoe, double etot);
  void fillEtaPhiProfileJet (double eta0, double phi0, double eta,
                             double phi, double dist);
  void fillJets             (std::vector<double> enj, std::vector<double> etaj,
			     std::vector<double> phij);
  void fillDiJets           (double mass);

private:

  int                 nJetHit, nJet;
  float               ecalJet, hcalJet, hoJet, etotJet;
  float               detaJet, dphiJet, drJet, dijetM;
  std::vector<float>  jetHite;
  std::vector<float>  jetHitr;
  std::vector<float>  jetHitt;
  std::vector<float>  jetE;
  std::vector<float>  jetEta;
  std::vector<float>  jetPhi;

};

#endif
