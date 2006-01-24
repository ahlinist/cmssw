///////////////////////////////////////////////////////////////////////////////
// File: PHcalValidInfoJets.h
// Persistent Jet informaion from analysis in SimG4HcalValidation
///////////////////////////////////////////////////////////////////////////////
#ifndef  PHcalValidInfoJets_H
#define  PHcalValidInfoJets_H

#include <string>
#include <vector>
#include <memory>

class PHcalValidInfoJets {

public:
       
  PHcalValidInfoJets(): nJetHit(0), ecalJet(0), hcalJet(0), hoJet(0),
    etotJet(0), detaJet(0), dphiJet(0), drJet(0), dijetM(0) {}
  virtual ~PHcalValidInfoJets() {}

  void fillTProfileJet      (double e, double r, double t);
  void fillEcollectJet      (double ee, double he, double hoe, double etot);
  void fillEtaPhiProfileJet (double eta0, double phi0, double eta,
                             double phi, double dist);
  void fillJets             (std::vector<double> en, std::vector<double> eta,
			     std::vector<double> fi);
  void fillDiJets           (double mass);

  struct JetHit {
    JetHit() {}
    float e;
    float r;
    float t;
  };

  struct Jet {
    Jet() {}
    float e;
    float eta;
    float phi;
  };

private:

  int                 nJetHit;
  float               ecalJet, hcalJet, hoJet, etotJet;
  float               detaJet, dphiJet, drJet, dijetM;
  std::vector<JetHit> jetHit;
  std::vector<Jet>    jets;

};

#endif
