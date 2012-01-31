TChain *tteffAnalysis_runs_160431_167913_v428_1_V00_09_07(bool l1emu, bool highpurity) {
  TString tmp = "";
  if(l1emu)
    tmp = "-l1emu";
  if(highpurity)
    tmp += "-highpurity";
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_07/tteffAnalysis-hltpftautight-hpspftau"+tmp+".root");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptSkim_v4_v428_1_V00_09_07/tteffAnalysis-hltpftautight-hpspftau"+tmp+".root");
  return chain;
}
TChain *tteffAnalysis_runs_170722_173198_v428_1_V00_09_07(bool l1emu, bool highpurity) {
  TString tmp = "";
  if(l1emu)
    tmp = "-l1emu";
  if(highpurity)
    tmp += "-highpurity";
  TChain *chain = new TChain("TTEffTree");
  // FIXME: Aug05 is missing!
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptSkim_v6_v428_1_V00_09_07/tteffAnalysis-hltpftautight-hpspftau"+tmp+".root");
  return chain;
}
TChain *tteffAnalysis_runs_173236_173692_v428_1_V00_09_07(bool l1emu, bool highpurity) {
  TString tmp = "";
  if(l1emu)
    tmp = "-l1emu";
  if(highpurity)
    tmp += "-highpurity";
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptSkim_v6_v428_1_V00_09_07/tteffAnalysis-hltpftautight-hpspftau"+tmp+".root");
  return chain;
}
TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_07(bool highpurity){
  TChain *chain = new TChain("TTEffTree");
  TString tmp = "";
  if(highpurity)
    tmp += "-highpurity";
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_07/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau"+tmp+".root");
  return chain;
}
TChain *tteffAnalysis_data_v428(int runRange, bool l1emu=false) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_07(l1emu, false);
  else if(runRange == 2)
    return tteffAnalysis_runs_170722_173198_v428_1_V00_09_07(l1emu, false);
  else if(runRange == 3)
    return tteffAnalysis_runs_173236_173692_v428_1_V00_09_07(l1emu, false);

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}
TChain *tteffAnalysis_data_v428_highpurity(int runRange, bool l1emu=false) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_07(l1emu, true);
  else if(runRange == 2)
    return tteffAnalysis_runs_170722_173198_v428_1_V00_09_07(l1emu, true);
  else if(runRange == 3)
    return tteffAnalysis_runs_173236_173692_v428_1_V00_09_07(l1emu, true);

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}
TChain* tteffAnalysis_DYtoTauTau_v428(){
  return tteffAnalysis_DYtoTauTau_v428_1_V00_09_07(false);
}
TChain* tteffAnalysis_DYtoTauTau_v428_highpurity(){
  return tteffAnalysis_DYtoTauTau_v428_1_V00_09_07(true);
}




TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_05_v2(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_05_v2/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_05_v2_highpurity(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_05_v2/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_05(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_05/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_05_highpurity(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_05/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_167913_v428_1_V00_09_05() {
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_05/tteffAnalysis-hltpftautight-hpspftau.root");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptSkim_v4_v428_1_V00_09_05/tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_167913_v428_1_V00_09_05_highpurity() {
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_05/tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptSkim_v4_v428_1_V00_09_05/tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_163869_v428_1_V00_09_05() {
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_05/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_163869_v428_1_V00_09_05_highpurity() {
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_05/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_data_v428_1_V00_09_05(int runRange) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_05();

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}
TChain *tteffAnalysis_data_v428_1_V00_09_05_highpurity(int runRange) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_05_highpurity();

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}



TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_03(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_03/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain* tteffAnalysis_DYtoTauTau_v428_1_V00_09_03_highpurity(){
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_DYtoTauTau_M-20_TuneP0_7TeV-pythia6-tauola_Summer11-PU_S4_START42_V11-v2_v428_1_V00_09_03/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_167913_v428_1_V00_09_03() {
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_03/tteffAnalysis-hltpftautight-hpspftau.root");
  //chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptReco_v4_v428_1_V00_09_03/tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_167913_v428_1_V00_09_03_highpurity() {
  TChain *chain = new TChain("TTEffTree");
  chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_03/tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  //chain->Add("data/tteffAnalysis_SingleMu_Run2011A_Tau_PromptReco_v4_v428_1_V00_09_03/tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_163869_v428_1_V00_09_03() {
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_03/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau.root");
  return chain;
}
TChain *tteffAnalysis_runs_160431_163869_v428_1_V00_09_03_highpurity() {
  TChain *chain = new TChain("TTEffTree");
  TString path = "data/tteffAnalysis_SingleMu_Run2011A_Tau_May10ReReco_v1_v428_1_V00_09_03/";
  chain->Add(path + "tteffAnalysis-hltpftautight-hpspftau-highpurity.root");
  return chain;
}
TChain *tteffAnalysis_data_v428_1_V00_09_03(int runRange) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_03();

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}
TChain *tteffAnalysis_data_v428_1_V00_09_03_highpurity(int runRange) {
  if(runRange == 1)
    return tteffAnalysis_runs_160431_167913_v428_1_V00_09_03_highpurity();

  std::cout << "No data yet for run range " << runRange << std::endl;
  return 0;
}
