#ifndef TREECOPY_H_
#define TREECOPY_H_

#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include <string>
#include <TTree.h>
#include <TFile.h>

namespace pftools {

class TreeCopy {
public:
	TreeCopy() {
	}
	virtual ~TreeCopy() {
	}

	void doCopy(std::string sourceFile, std::string destFile) {
		std::cout << "Opening source file " << sourceFile << std::endl;
		TFile source(sourceFile.c_str());
		TTree* sourceTree = (TTree*) gDirectory->Get("extraction/Extraction");

		std::cout << "Opening destination file " << destFile << std::endl;
		TFile dest(destFile.c_str(), "RECREATE");
		dest.mkdir("extraction")->cd();
		TTree* destTree = new TTree("Extraction", "");
		//init destTree

		Calibratable* sourceCalib = new Calibratable();

		sourceTree->SetBranchAddress("Calibratable", &sourceCalib);

		destTree->Branch("Calibratable", "pftools::Calibratable", &sourceCalib,
				32000, 2);

		std::cout << "Looping over source tree's " << sourceTree->GetEntries()
				<< " entries..." << std::endl;
		for (unsigned u(0); u < sourceTree->GetEntries(); ++u) {
			sourceCalib->reset();
			sourceTree->GetEntry(u);
			sourceCalib->recompute();
			destTree->Fill();
			if(u % 1000 == 0)
				std::cout << *sourceCalib << "\n";
			if(sourceCalib->cand_energyEvent_ > 160)
				std::cout << "Exceptional event: " << *sourceCalib << "\n";
		}

		destTree->Write();
		dest.Close();
		std::cout << "Done." << std::endl;
		delete sourceCalib;

	}

};

}

#endif /* TREECOPY_H_ */
