void read2(TString filename = "/data1/consumer/results/XMon/XMon_xsec194337.root")
{
  TFile file (filename);
  TTree *tree = (TTree*) file.Get("l1names");
  tree->ls();
  Long64_t NumberOfEntries = tree->GetOfEntries();
  
  cout << "NumberOfEntries " << NumberOfEntries << endl;

  int Number;
  char name[300];
  tree->SetBranchAddress ("index", &Number);
  tree->SetBranchAddress ("name", name);
  
  for (int loop = 0; loop < NumberOfEntries; ++loop)
    {
      tree->GetEntry(loop);
      cout << loop << " : " << Number << ") " << name << endl;   
    }  
}
