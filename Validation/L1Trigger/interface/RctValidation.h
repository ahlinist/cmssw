/*Rct Validation from MC
Author : Michail Bachtis
University of Wisconsin-Madison
bachtis@hep.wisc.edu
*/

 
#include <memory>
#include <unistd.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"



//Include DQM core
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"


#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"
#include "CondFormats/DataRecord/interface/L1CaloGeometryRecord.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/DataRecord/interface/L1EmEtScaleRcd.h" 

class RctValidation : public edm::EDAnalyzer {
public:
  RctValidation( const edm::ParameterSet& );
  ~RctValidation();

protected:
   

  /// BeginRun
  void beginRun(const edm::Run& r, const edm::EventSetup& c);

  /// Fake Analyze
  void analyze(const edm::Event& e, const edm::EventSetup& c) ;

  ///Luminosity Block 
  void beginLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                            const edm::EventSetup& context) ;
  /// DQM Client Diagnostic
  void endLuminosityBlock(const edm::LuminosityBlock& lumiSeg, 
                          const edm::EventSetup& c);
  /// EndRun
  void endRun(const edm::Run& r, const edm::EventSetup& c);

  /// Endjob
  void endJob();



private:
  DQMStore* dbe_;  

  edm::InputTag egamma_;
  edm::InputTag genEGamma_;
  std::string directory_;

  double maxEt_;
  int binsEt_; 
  int binsEta_;
  int binsPhi_;
  double matchDR_;
  double egammaThreshold_;

  //MEs
  MonitorElement* egamma_rank;
  MonitorElement* egamma_et;
  MonitorElement* egamma_eta;
  MonitorElement* egamma_phi;

  MonitorElement* egamma_deltaEt;

  MonitorElement* egamma_et_eff_num;
  MonitorElement* egamma_et_eff_denom;
  MonitorElement* egamma_eta_eff_num;
  MonitorElement* egamma_eta_eff_denom;
  MonitorElement* egamma_phi_eff_num;
  MonitorElement* egamma_phi_eff_denom;

  MonitorElement* egamma_et_eff_num2D;
  MonitorElement* egamma_et_eff_denom2D;


  MonitorElement* isoegamma_et_eff_num;
  MonitorElement* isoegamma_eta_eff_num;
  MonitorElement* isoegamma_phi_eff_num;
  MonitorElement* isoegamma_et_eff_num2D;



  MonitorElement* region_rank;



  


};





