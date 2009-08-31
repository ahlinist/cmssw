 #ifndef HcalChanMon_h
 #define HcalChanMon_h


// system include files
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h>

// user include files

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"

#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"

#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Candidate/interface/Candidate.h"

//ecal
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

//trigger
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TProfile.h"
#include <TROOT.h>
#include <TSystem.h>
#include "TFile.h"
#include <TCanvas.h>
#include <cmath>

#include "TTree.h"
#include "TStyle.h"
#include "TPaveStats.h"

class HcalChanMon : public edm::EDAnalyzer {
   public:
      explicit HcalChanMon(const edm::ParameterSet&);
      ~HcalChanMon();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();


     // ----------member data ---------------------------

  void bookHistograms ();
  TH1D* book1DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			 int fNbins, double fXmin, double fXmax) const;
  TH2D* book2DHistogram (TFileDirectory& fDir, const std::string& fName, const std::string& fTitle, 
			 int fNbinsX, double fXmin, double fXmax,
			 int fNbinsY, double fYmin, double fYmax) const;
      

  edm::Service<TFileService> fs;
  
  std::map<std::string, TH2D*> histo2D;
  std::map<std::string, TH2D*>::iterator histo2Diter;
  
  std::map<std::string, TH1D*> histo1D;
  std::map<std::string, TH1D*>::iterator histo1Diter;

  std::map<std::string, TH1D*> histo1DD;
  std::map<std::string, TH1D*>::iterator histo1DDiter;
  
  std::map<std::string, TH1D*> histo1DR;
  std::map<std::string, TH1D*>::iterator histo1DRiter;

  std::map<std::string, TH1D*> histo1DD_np;
  std::map<std::string, TH1D*>::iterator histo1DDiter_np;
  
  std::map<std::string, TH1D*> histo1DR_np;
  std::map<std::string, TH1D*>::iterator histo1DRiter_np;
    
        
  std::map<std::string, TH1D*> histo1DNormRaw;
  std::map<std::string, TH1D*>::iterator histo1DNormRawiter;
  
  std::map<std::string, TH1D*> histo1DNormDigit;
  std::map<std::string, TH1D*>::iterator histo1DNormDigititer;
  
  std::map<std::string, TH1D*> histo1DNormRecHit;
  std::map<std::string, TH1D*>::iterator histo1DNormRecHititer;

  std::map<std::string, TH1D*> histo1DNormDigit_np;
  std::map<std::string, TH1D*>::iterator histo1DNormDigititer_np;
  
  std::map<std::string, TH1D*> histo1DNormRecHit_np;
  std::map<std::string, TH1D*>::iterator histo1DNormRecHititer_np;
    
  std::vector<HcalElectronicsId> channelse;
  std::vector<HcalDetId> channelsd;
  const HcalElectronicsMap *readoutMap_;

  char channel[1024], name[1024], title[1024],channel_np[1024];
  
  double EnergyThres;
  double ChargeAbovePed;  
  double pedestal, pedDif;
  double evt,evts;
  bool   DoHtml, MultiJob;

  ifstream in;
  ofstream out, out1 , htmlPage;
  

  std::map<TString, double> myChannel;
  std::map<TString, double>::iterator mychIT;

  std::map<TString, double> m_HCAL_counter;
  std::map<TString, double>::iterator hcal_c;

  std::string runNumber_;
  std::string OutputFilePath_;
  std::string fileName;
  std::string subDet_;
  
  int runBegin;
  long runTime;

  typedef std::vector< edm::ParameterSet > Parameters;
  Parameters ChannelList_;

  std::map<int, short> CDFversionNumber_list;
  std::map<int, short>::iterator CDFvers_it;
  std::map<int, short> CDFEventType_list;
  std::map<int, short>::iterator CDFEvT_it;
  std::map<int, short> CDFReservedBits_list;
  std::map<int, short>::iterator CDFReservedBits_it;
  std::map<int, short> DCCEvtFormat_list;
  std::map<int, short>::iterator DCCEvtFormat_it;
  std::map<int, short> DCCRsvdBits_list;
  std::map<int, short>::iterator DCCRsvdBits_it;


  
  
};

#endif
