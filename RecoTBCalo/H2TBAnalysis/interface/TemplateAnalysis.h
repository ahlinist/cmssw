#ifndef TEMPLATE_ANALYSIS_H
#define TEMPLATE_ANALYSIS_H

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TNamed.h"
#include <map>
#include <string>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetfwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "RecoTBCalo/H2TBAnalysis/interface/CaloTowerBoundries.h"

#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"

class TemplateAnalysis {
public:
  TemplateAnalysis(); 
  /** Setup the analysis to put the histograms into HistoFile and focus on
      ieta,iphi for analysis.
  */
  void setup(const edm::ParameterSet& pSet);
  void fillHist(const TString& histName, const Double_t& value, const Double_t& wt=1.0);
  void fillHist(const TString& histName, const Double_t& x,const Double_t& y,const Double_t& wt=1.0);

  /** Analyze the Data */
  void analyze(const CaloJetCollection& rjets,
	       const HBHERecHitCollection& hbhe_hits, 
	       const HORecHitCollection& ho_hits, 
	       const HFRecHitCollection& hf_hits, 
	       const HcalTBTriggerData& trigger,
	       const HcalTBEventPosition& epos,
	       const HcalTBBeamCounters& bc,
	       const HcalTBTiming& time);

  /** Finalization (close files, etc) */
  void done();

  void bookHistograms();
  void bookGeneralHistograms();

  void bookTBHists();
  void fillTBTriggerHists(const HcalTBTriggerData& trigger);
  void fillTBBeamHists(const HcalTBEventPosition& epos,
		       const HcalTBBeamCounters& bc,
		       const HcalTBTiming& time);

  void bookJetHistograms(const TString& prefix);

  void bookForId(const HcalDetId& id);

  void fillJetHists(const CaloJetCollection& jets, const TString& prefx);
  //Templated to accommodate HB/HF/HO/EB rechits with one method....
  template <typename T> void fillRecHits(const T& hits);

private:

  // input variables
  std::string _HistName; // Name of histogram file
  double _EtaMin,_EtaMax;

  int evtCounter;
  bool doTBTrigger;

  const float etaBarrel() {return 1.4;}

  TFile* m_file; // pointer to Histogram file
  const TString EnergyDir() { return "Channel Energies";}
  const TString PulseDir(){ return "Pulse Shapes";}

  // Trigger histogram labels
  static const char* trigBeam(){return "Beam";}
  static const char* trigIped(){return "In-Spill Ped";}
  static const char* trigOped(){return "Out-Spill Ped";}
  static const char* trigLED(){return "LED";}
  static const char* trigLaser(){return "Laser";}

  // histogram declarations
  TH1* m_Cntr; // Simple single histogram

  // use the map function to access the rest of the histograms
  std::map<TString, TH1*> m_HistNames;
  std::map<TString, TH1*>::iterator hid_iter;  //used to search through the histo name map

  std::map<TString, TH2*> m_HistNames2D;
  std::map<TString, TH2*>::iterator hid2D_iter;  //used to search through the 2D histo name map

  //create maps linking histogram pointers to HCAL Channel hits and digis
  std::map<HcalDetId, TH1*> rechitmap;  
  std::map<HcalDetId, TH1*>::iterator rechit_iter;

};

#endif
