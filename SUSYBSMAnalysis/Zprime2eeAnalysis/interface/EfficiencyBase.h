#ifndef TagProbeAnalysis_Trigger_EfficiencyBase_h
#define TagProbeAnalysis_Trigger_EfficiencyBase_h

// --------------------
// Base class for trigger efficiency analysis using common tag + probe code
// Require TFileService to be present at runtime

#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>

class EfficiencyBase
{
 protected:

  long _sameProbeEETagEE;
  long _sameProbeEETagEB;
  long _sameProbeEBTagEE;
  long _sameProbeEBTagEB;

  TH1F *_ptEBPass;
  TH1F *_ptEBTotal;
  TH1F *_ptEEPass;
  TH1F *_ptEETotal;
  TH1F *_ptPass;
  TH1F *_ptTotal;

  TH1F *_etaEBPass;
  TH1F *_etaEBTotal;
  TH1F *_etaEEPass;
  TH1F *_etaEETotal;
  TH1F *_etaPass;
  TH1F *_etaTotal;

  TH1F *_phiEBPass;
  TH1F *_phiEBTotal;
  TH1F *_phiEEPass;
  TH1F *_phiEETotal;
  TH1F *_phiPass;
  TH1F *_phiTotal;

  TH1F *_ptEBEff;
  TH1F *_ptEEEff;
  TH1F *_ptEff;
  TH1F *_etaEBEff;
  TH1F *_etaEEEff;
  TH1F *_etaEff;
  TH1F *_phiEBEff;
  TH1F *_phiEEEff;
  TH1F *_phiEff;

  int _ptBinsEB;
  double _ptMinEB;
  double _ptMaxEB;
  int _ptBinsEE;
  double _ptMinEE;
  double _ptMaxEE;
  int _ptBins;
  double _ptMin;
  double _ptMax;

  int _etaBinsEB;
  double _etaMinEB;
  double _etaMaxEB;
  int _etaBinsEE;
  double _etaMinEE;
  double _etaMaxEE;
  int _etaBins;
  double _etaMin;
  double _etaMax;

  int _phiBinsEB;
  double _phiMinEB;
  double _phiMaxEB;
  int _phiBinsEE;
  double _phiMinEE;
  double _phiMaxEE;
  int _phiBins;
  double _phiMin;
  double _phiMax;

// For tree
//

  TFile *_file;
  TTree *_tree;

  Int_t nProbe_;
//  Int_t *pass_, *os_;
//  Float_t *eta_, *phi_, *pt_, *weight_;
  Int_t pass_, os_;
  Float_t eta_, phi_, pt_, weight_;
  Float_t vx_, vy_, vz_;
  Float_t mass_;


  void BeginTree();
  void AddToTree(const EgEff::EmObjectRef, const EgEff::EmObjectRef, double, bool, bool);
  void FillTree();

//
//

  void InitialiseHistograms(const edm::ParameterSet&);
  void AddToTotalHistograms(const EgEff::EmObjectRef, double);
  void AddToPassHistograms(const EgEff::EmObjectRef, double);
  void AddToTotalHistograms(const EgEff::EmObjectRef);
  void AddToPassHistograms(const EgEff::EmObjectRef);
  void FillEfficiencyHistogram(TH1F*, TH1F*, TH1F*);
  double dR(double, double, double, double);
  double dPhi(double, double);
  void CalculateEventEfficiency(const double, const double, double&, double&);

  double ecalEta(double &, double &, double &);
  double ecalPhi(double &, double &, double &, int , double &);

  std::string _name;
  void setName(std::string &name) {_name = name;}

  bool _eventEfficiency;

 public:
   EfficiencyBase(const char* name) : _name(name){};
   EfficiencyBase() : _name(""){};
   virtual ~EfficiencyBase(){};
   virtual void Initialise(const edm::ParameterSet&);
   virtual void Add(const edm::Event&, const edm::EventSetup&) = 0;
   virtual void FinishHistograms();
};

#endif
