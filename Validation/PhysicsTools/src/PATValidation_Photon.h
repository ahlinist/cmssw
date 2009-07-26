//Author Sudhir Malik 20 May 2009
#ifndef ValidationPhysicsToolsPATValidation_Photon_h
#define ValidationPhysicsToolsPATValidation_Photon_h

 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <map>
#include <vector>



class PATValidation_Photon : public edm::EDAnalyzer {

 public:
    
  PATValidation_Photon (const edm::ParameterSet&);
  ~PATValidation_Photon();

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void endJob() ;

  
 private:

  float  phiNormalization( float& a);
  void book2DHistoVector();
  
  std::string histname;
  double nPhoMin;
  double nPhoMax;
  int    nPhoBin;

  double eMin;
  double eMax;
  int    eBin;

  double etaMin;
  double etaMax;
  int    etaBin;
  
  double eopMin;
  double eopMax;
  int    eopBin; 

  double phiMin;
  double phiMax;
  int    phiBin;


 
  edm::InputTag recoPhoton_;
  edm::InputTag patPhoton_;
  std::string outputFile_;

  DQMStore* dbe;
   std::map<std::string, MonitorElement*> me;

   MonitorElement* me_nPho_[2][3];
   MonitorElement* me_scE_[2][3];
   MonitorElement* me_scEt_[2][3];
   MonitorElement* me_r9_[2][3];  
   MonitorElement* me_scEta_[2];
   MonitorElement* me_scPhi_[2];
   MonitorElement* me_phoE_[2][3];
   MonitorElement* me_phoEt_[2][3];
   MonitorElement* me_phoEta_[2];
   MonitorElement* me_phoPhi_[2];
   MonitorElement* me_nConv_[2][3];
   MonitorElement* me_convEta_[2];
   MonitorElement* me_convPhi_[2];
   MonitorElement* me_EoverPTracks_[2][3];
   MonitorElement* me_tkChi2_[2];
   MonitorElement* me_DPhiTracksAtVtx_[2][3];
   MonitorElement* me_DCotTracks_[2][3];
   MonitorElement* me_invMass_[2][3];
   MonitorElement* me_DPhiTracksAtEcal_[2][3];
   MonitorElement* me_DEtaTracksAtEcal_[2][3];
   MonitorElement* me_zPVFromTracks_[2];   

    
   MonitorElement* me_pat_nPho_[2][3];
   MonitorElement* me_pat_scE_[2][3];
   MonitorElement* me_pat_scEt_[2][3];
   MonitorElement* me_pat_r9_[2][3];  
   MonitorElement* me_pat_scEta_[2];
   MonitorElement* me_pat_scPhi_[2];
   MonitorElement* me_pat_phoE_[2][3];
   MonitorElement* me_pat_phoEt_[2][3];
   MonitorElement* me_pat_phoEta_[2];
   MonitorElement* me_pat_phoPhi_[2];
   MonitorElement* me_pat_nConv_[2][3];
   MonitorElement* me_pat_convEta_[2];
   MonitorElement* me_pat_convPhi_[2];
   MonitorElement* me_pat_EoverPTracks_[2][3];
   MonitorElement* me_pat_tkChi2_[2];
   MonitorElement* me_pat_DPhiTracksAtVtx_[2][3];
   MonitorElement* me_pat_DCotTracks_[2][3];
   MonitorElement* me_pat_invMass_[2][3];
   MonitorElement* me_pat_DPhiTracksAtEcal_[2][3];
   MonitorElement* me_pat_DEtaTracksAtEcal_[2][3];
   MonitorElement* me_pat_zPVFromTracks_[2];
   

   };
  
#endif
 

