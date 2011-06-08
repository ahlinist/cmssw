TChain* tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltcalotau_hpspftau() {

        TChain* chain = new TChain("TTEffTree");

        TString path = "/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/RealTaus/tteffAnalysis_muTau_HLT-AOD_Run2011A_cmssw416_V00-07-03/";
        chain->AddFile(path + "tteffAnalysis-hltcalotau-hpspftau.root");
        return chain;
}

TChain* tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftau_hpspftau() {

        TChain* chain = new TChain("TTEffTree");

        TString path = "/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/RealTaus/tteffAnalysis_muTau_HLT-AOD_Run2011A_cmssw416_V00-07-03/";
        chain->AddFile(path + "tteffAnalysis-hltpftau-hpspftau.root");
        return chain;
}

TChain* tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftau() {

        TChain* chain = new TChain("TTEffTree");

        TString path = "/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/RealTaus/tteffAnalysis_muTau_HLT-AOD_Run2011A_cmssw416_V00-07-03/";
        chain->AddFile(path + "tteffAnalysis-hltpftau-pftau.root");
        return chain;
}

TChain* tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftautight_hpspftau() {

        TChain* chain = new TChain("TTEffTree");

        TString path = "/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/RealTaus/tteffAnalysis_muTau_HLT-AOD_Run2011A_cmssw416_V00-07-03/";
        chain->AddFile(path + "tteffAnalysis-hltpftautight-hpspftau.root");
        return chain;
}

