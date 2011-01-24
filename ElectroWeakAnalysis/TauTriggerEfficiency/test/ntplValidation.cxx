// macro for checking empty branches in TTEffTree
// 19.3.2010/S.Lehti

void ntplValidation(){

//	TFile* fIN = TFile::Open("tteffAnalysis-pftau.root");
//	TFile* fIN = TFile::Open("/castor/cern.ch/user/s/slehti/TauTriggerEfficiencyMeasurementData/tteffAnalysis-pftau_MinimumBias_Commissioning10_GOODCOLL_v8_RAW_RECO_run2.root");
//	TFile* fIN = TFile::Open("tteffAnalysis-l1cen.root");
	TFile* fIN = TFile::Open("tteffAnalysis-hltpftau-pftau.root");

	fIN->ls();
	cout << " TTEffTree entries " << TTEffTree->GetEntries() << endl;

	TTree* TTEffTree = (TTree*)fIN->Get("TTEffTree");

	int runMin = 0,
            runMax = 0;

	TObjArray* branches = TTEffTree->GetListOfBranches();
	//GetNbranches()
	for(int i = 0; i < branches->GetEntries(); ++i){

		TTEffTree->Draw(branches->At(i)->GetName(),"","ng");
		TH1F* htemp = (TH1F*)gPad->GetPrimitive("htemp");

		string branchName = string(branches->At(i)->GetName());
                while(branchName.size() < 35) branchName+=" ";
                cout << branchName << " ";
/*
		cout << htemp->GetEntries() << " "
		     << htemp->GetMinimum() << " " 
                     << htemp->GetMaximum() << endl;
*/

		if(htemp->GetEntries() == 0) {
			cout << "Warning, No entries! " << endl;
			htemp->DrawClone();
			continue;
		}
		if(htemp->GetMaximum() <= htemp->GetMinimum()){
			cout << "Warning, max <= min" << endl;
			htemp->DrawClone();
			continue;
		}
		int nFilledBins  = 0;
		int iNonEmptyBin = -1;
		for(int iBin = 1; iBin <= htemp->GetNbinsX(); ++iBin){
			if(htemp->GetBinContent(iBin) != 0) {
				nFilledBins++;
				iNonEmptyBin = iBin;
				if(string(branches->At(i)->GetName()) == "run"){
					if(runMin == 0) runMin = htemp->GetBinLowEdge(iBin);
					runMax = htemp->GetBinLowEdge(iBin);
				}
			}
		}
		if(nFilledBins < 2){
			cout << "WARNING, only one bin filled, value = " << htemp->GetBinLowEdge(iNonEmptyBin) << endl;
			htemp->DrawClone();
                        continue;
		}
		cout << "Ok." << endl;
	}
	cout << "Run range " << runMin << " - " << runMax << endl;
	exit(0);
}
