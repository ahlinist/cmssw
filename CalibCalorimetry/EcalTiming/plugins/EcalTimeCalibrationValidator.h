#ifndef EcalTimeCalibrationValidator_hh
#define EcalTimeCalibrationValidator_hh

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/EcalObjects/interface/EcalTimeCalibConstants.h"

#include "CalibCalorimetry/EcalTiming/interface/EcalTimeTreeContent.h"

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

#include <vector>

class EcalTimeCalibrationValidator : public edm::EDAnalyzer {

        public:
                explicit EcalTimeCalibrationValidator(const edm::ParameterSet& ps);
                ~EcalTimeCalibrationValidator();

                virtual void analyze(edm::Event const&, edm::EventSetup const&);
                virtual void beginJob();
                virtual void beginRun(edm::EventSetup const&);
                virtual void endJob();
                void set(edm::EventSetup const&);

        private:
                edm::ESHandle<EcalTimeCalibConstants> origTimeCalibConstHandle;
                std::vector<std::string> inputFiles_;
                std::string outputTreeFileName_; // file name of output ttree
                std::string calibConstantFileName_; // file name of XML calib constants
                TFile* outputTreeFile_;
                TChain* myInputTree_;
                TTree* myOutputTree_;
                bool produce_;
                int maxEntries_;
                int startingEntry_;
                EcalTimeTreeContent ttreeMembersInput_;

};
#endif


