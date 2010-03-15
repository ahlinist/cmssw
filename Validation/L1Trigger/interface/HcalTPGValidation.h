#ifndef __Validation_L1Trigger_HcalTPGValidation_h__
#define __Validation_L1Trigger_HcalTPGValidation_h__

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

class HcalTPGValidation : public edm::EDAnalyzer {
   public:
      explicit HcalTPGValidation(const edm::ParameterSet&);
      ~HcalTPGValidation();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      std::string dqm_folder_;
      edm::InputTag simhits_label_;
      edm::InputTag tp_label_;
      double factor_HB_;
      double factor_HE_;
      double factor_HF1_;
      double factor_HF2_;
      double threshold_;
      HcalTrigTowerGeometry geometry_;
      std::map<int, double> cosh_ieta_;
      DQMStore * dbe_;
      MonitorElement* histo_Et_corr_HB;
      MonitorElement* histo_Et_corr_HE;
      MonitorElement* histo_Et_corr_HF;
      MonitorElement* histo_Et_ratio_HB;
      MonitorElement* histo_Et_ratio_HE;
      MonitorElement* histo_Et_ratio_HF;
};
#endif
