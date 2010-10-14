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

#include "CalibCalorimetry/EcalTiming/interface/EcalTimePi0TreeContent.h"

#include "TTree.h"
#include "TFile.h"

class EcalTimeCalibrationValidator : public edm::EDAnalyzer {

        public:
                explicit EcalTimeCalibrationValidator(const edm::ParameterSet& ps);
                ~EcalTimeCalibrationValidator();

                virtual void analyze(edm::Event const&, edm::EventSetup const&);
                virtual void beginJob();
                virtual void beginRun(edm::EventSetup const&);
                virtual void endJob();

        private:

                std::string inputTreeFileName_; // file name of input ttree
                std::string outputTreeFileName_; // file name of output ttree
                TFile* inputTreeFile_;
                TFile* outputTreeFile_;
                TTree* myInputTree_;
                TTree* myOutputTree_;
                bool produce_;
                int maxEntries_;
                int startingEntry_;
                edm::ESHandle<EcalTimeCalibConstants> itime_;
                EcalTimePi0TreeContent ttreeMembersInput_;

};
#endif


