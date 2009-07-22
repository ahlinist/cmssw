#include "RecoParticleFlow/PFAnalyses/interface/TestbeamPreprocessor.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

using namespace std;
using namespace pftools;

#include <iostream>
#include <TF1.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream>
#include <ostream>
#include <sstream>

//template<typename T, int length> int theSize(T (&x)[length]) {
//	return length;
//}

//This doesn't work :-(
//template<typename u> std::vector<u> arrToVec(u a[]) {
//	std::vector<u> ans;
//	int size = sizeof(a)/sizeof(a[0]);
//	//int size = theSize(a);
//	std::cout << "Array size = " << size << std::endl;
//	for (int k(0); k < size; ++k) {
//		ans.push_back(a[k]);
//	}
//	return ans;
//}


TestbeamPreprocessor::TestbeamPreprocessor(bool withMonitoring) :
	withMonitoring_(withMonitoring) {
	//std::cout << __PRETTY_FUNCTION__ << ": initialising."<< std::endl;

	populateEnergiesAndRuns();
}

TestbeamPreprocessor::~TestbeamPreprocessor() {
	//std::cout << __PRETTY_FUNCTION__ << ": destructing."<< std::endl;

}

void TestbeamPreprocessor::processEvents(std::string filename, int runNumber,
		double energy) {
	std::cout << "*** Processing run " << runNumber << std::endl;
	TFile* run = TFile::Open(filename.c_str());
	if (!run) {
		std::cout << "Couldn't open run!" << std::endl;
		return;
	}
	TTree* events = (TTree *) gROOT->FindObject("Events");
	processEvents(runNumber, events, energy);
	std::cout << "Finished run " << runNumber << std::endl;
}

void TestbeamPreprocessor::processEvents(int runNumber, TTree* events,
		double energy) {
	RunInfo ri;
	ri.reset();
	ri.runNumber_ = runNumber;
	ri.particlePDG_ = 211;

	bool foundRun(false);
	if (energy == 0.0) {

		for (std::map<int, std::vector<int> >::iterator it =
				energiesAndRuns_.begin(); it != energiesAndRuns_.end(); ++it) {
			std::pair<int, std::vector<int> > keyVal = *it;
			for (std::vector<int>::iterator uit = keyVal.second.begin(); uit
					!= keyVal.second.end(); ++uit) {
				int run = *uit;
				if (run == runNumber) {
					ri.beamEnergy_ = keyVal.first;
					foundRun = true;
					break;
				}
			}
			if (foundRun)
				break;
		}

		if (!foundRun) {
			std::cout
					<< "!!! Couldn't find run in hard-coded dictionary. Update code?\n";
			return;
		}
	} else {
		ri.beamEnergy_ = energy;
	}

	processPositioning(events, ri);
	processBeamCounters(events, ri);
	processScintillatorCounters(events, ri);
	processMuonVetos(events, ri);
	processCerenkovCounters(events, ri);
	processTOF(events, ri);
	runInfos_.push_back(ri);
}

void TestbeamPreprocessor::finish(std::string filename) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	//TODO: Mini memory leak here :-(
	RunInfo* theRun = new RunInfo();
	TFile* output = new TFile(filename.c_str(), "update");
	//Update with new info:
	//See whether tree exists or not:
	TTree* tree = (TTree*) gROOT->FindObject("RunInfo");
	std::map<int, RunInfo> existingRuns;
	if (tree == 0) {
		std::cout << "\tCreating new tree..." << std::endl;
	} else {
		std::cout << "\tUpdating existing tree... " << std::endl;
		tree->SetBranchAddress("RunInfo", &theRun);
		for (unsigned entry(0); entry < tree->GetEntries(); ++entry) {
			tree->GetEntry(entry);
			existingRuns[theRun->runNumber_] = *theRun;
		}
	}
	//delete existing tree etc.
	output->Close();
	output = new TFile(filename.c_str(), "recreate");
	tree = new TTree("RunInfo", "");
	tree->Branch("RunInfo", &theRun, 32000, 2);
	std::cout << "\tWe have " << existingRuns.size()
			<< " already existing runs.\n";

	for (std::vector<RunInfo>::iterator it = runInfos_.begin(); it
			!= runInfos_.end(); ++it) {
		//Is there a better way of writing this line?!
		theRun = &(*it);
		std::cout << "*** Writing out run " << theRun->runNumber_ << "\n";
		tree->Fill();
		std::cout << "\tFilled tree." << std::endl;
		//remove object from already existing runs
		if (existingRuns.count(theRun->runNumber_) != 0) {
			existingRuns.erase(theRun->runNumber_);
		}

		if (withMonitoring_) {
			std::cout << "\tWriting out monitors..." << std::endl;
			output->cd("/");
			std::string runNumber = obj2str(theRun->runNumber_);
			output->mkdir(runNumber.c_str());
			output->cd((std::string("/") + runNumber).c_str());

			std::vector<TObject*> runObjs = monitoring_[theRun->runNumber_];
			for (std::vector<TObject*>::iterator objs = runObjs.begin(); objs
					!= runObjs.end(); ++objs) {
				TObject* ob = *objs;
				ob->Write();
			}
		}
	}

	if (existingRuns.size() != 0) {
		for (std::map<int, RunInfo>::iterator eit = existingRuns.begin(); eit
				!= existingRuns.end(); ++eit) {
			theRun = &((*eit).second);
			std::cout << "*** Writing out existing run " << theRun->runNumber_
					<< "\n";
			tree->Fill();
		}
	}

	std::cout << "*** Writing output..." << std::endl;
	output->cd("/");
	tree->Write();

	output->Write();
	output->Close();
	std::cout << __PRETTY_FUNCTION__ << ": done." << std::endl;
}

void TestbeamPreprocessor::processPositioning(TTree* events, RunInfo& ri) {

	events->Draw("HcalTBEventPosition_tbunpacker__RECO.obj.hbheTableEta_>>eta");
	events->Draw("HcalTBEventPosition_tbunpacker__RECO.obj.hbheTablePhi_>>phi");
	TH1* eta = (TH1*) gROOT->FindObject("eta");
	TH1* phi = (TH1*) gROOT->FindObject("phi");
	//These positions are approximate
	double ecalImpactPhi(0.015);
	if (ri.runNumber_ > 30000)
		ecalImpactPhi = 0.045;

	ri.ecalEta_ = eta->GetMean();
	ri.ecalPhi_ = ecalImpactPhi;
	ri.hcalEta_ = eta->GetMean();
	ri.hcalPhi_ = ecalImpactPhi;
	ri.tableEta_ = eta->GetMean();
	ri.tablePhi_ = phi->GetMean();
	if (withMonitoring_) {
		std::cout << "\tTable eta, phi = (" << eta->GetMean() << ", "
				<< phi->GetMean() << ")\n";
		std::cout << "\tECAL/HCAL eta, phi= (" << eta->GetMean() << ", "
				<< ecalImpactPhi << ")\n";
	}
}

void TestbeamPreprocessor::processScintillatorCounters(TTree* events,
		RunInfo& ri) {
	//runinfo_s124_minmax = cms.vint32(120, 210, 250, 360, 200, 370),
	ri.s1Min_ = 120;
	ri.s1Max_ = 210;
	ri.s2Min_ = 250;
	ri.s2Max_ = 360;
	ri.s4Min_ = 200;
	ri.s4Max_ = 370;
}

void TestbeamPreprocessor::processCerenkovCounters(TTree* events, RunInfo& ri) {
	if (ri.beamEnergy_ < 10) {
		ri.ck2Max_ = 48;
		ri.ck2Min_ = 0;
		ri.applyCK2_ = true;
		//CK3 doesn't discriminate well for 4 and 5 GeV
		if (ri.beamEnergy_ < 4) {
			ri.applyCK3_ = true;
			ri.ck3Min_ = 0;
			ri.ck3Max_ = 480;
		} else if (ri.beamEnergy_ > 5) {
			ri.applyCK3_ = true;
			ri.ck3Min_ = 480;
			ri.ck3Max_ = 4000;
		} else if (ri.beamEnergy_ == 4 && ri.runNumber_ > 29975
				&& ri.runNumber_ < 29992) {
			//4 GeV need special attention, only certain run in this range had a suitable double
			//e-tag rejection.
			ri.applyCK3_ = true;
			ri.ck3Min_ = 0;
			ri.ck3Max_ = 480;
		}
	} else {
		ri.applyCK3_ = false;
		ri.applyCK2_ = false;
	}

	if (withMonitoring_) {
		events->Draw(
				"HcalTBBeamCounters_tbunpacker__RECO.obj.CK2adc_>>ck2(100,0,1000)");
		TH1* ck2 = (TH1*) gROOT->FindObject("ck2");
		events->Draw(
				"HcalTBBeamCounters_tbunpacker__RECO.obj.CK3adc_>>ck3(100,0,1000)");
		TH1* ck3 = (TH1*) gROOT->FindObject("ck3");
		monitoring_[ri.runNumber_].push_back(ck2);
		monitoring_[ri.runNumber_].push_back(ck3);
	}

}

void TestbeamPreprocessor::processMuonVetos(TTree* events, RunInfo& ri) {
	ri.vmbMax_ = 160;

	if (ri.beamEnergy_ < 10) {
		events->Draw(
				"HcalTBBeamCounters_tbunpacker__RECO.obj.VMFadc_>>vmf(500, 0, 500");
		TH1* vmf = (TH1*) gROOT->FindObject("vmf");
		std::pair<double, double> vmfFit = fitStabilisedGaussian(vmf);
		ri.vmfMax_ = static_cast<int> (1.05 * vmfFit.first + 3 * vmfFit.second);

		//Iterate over muon vetos and find thresholds
		std::string vmxNamesTemp[] = { "VM1adc", "VM2adc", "VM3adc", "VM4adc",
				"VM5adc", "VM6adc", "VM7adc", "VM8adc" };
		std::vector<std::string> vmxNames = arrToVec(vmxNamesTemp, 8);

		std::vector<int> thresholds;

		for (std::vector<std::string>::iterator rit = vmxNames.begin(); rit
				!= vmxNames.end(); ++rit) {
			std::string query("HcalTBBeamCounters_tbunpacker__RECO.obj.");
			query.append(*rit);
			query.append("_>>");
			query.append(*rit);
			query.append("(500,0,500)");

			events->Draw(query.c_str());
			TH1* vmx = (TH1*) gROOT->FindObject((*rit).c_str());
			std::pair<double, double> ans = fitStabilisedGaussian(vmx);
			//Threshold is 3 sigma away from the mean.
			thresholds.push_back(static_cast<int> (ans.first + 3* ans .second));
			if (withMonitoring_) {
				std::cout << "\t" << *rit << ": threshold = "
						<< static_cast<int> (ans.first + 3 * ans.second)
						<< "\n";
				monitoring_[ri.runNumber_].push_back(vmx);
			}
		}
		ri.vmx = thresholds;
		ri.applyVMF_ = true;
	}

}

void TestbeamPreprocessor::processTOF(TTree* events, RunInfo& ri) {
	if (ri.beamEnergy_ < 10) {
		ri.applyTOF_ = true;
		if (ri.beamEnergy_ < 6) {
			ri.tofMin_ = -1000;
			ri.tofMax_ = -545;
		} else {
			ri.tofMin_ = -1000;
			//rather handily, we can parameterise this sausage :-)
			ri.tofMax_ = static_cast<int> (-545 - 5* (ri .beamEnergy_ - 6));
		}
		if (withMonitoring_) {
			std::cout << "\tTOF min: " << ri.tofMin_ << ", tofMax: "
			<< ri.tofMax_ << "\n";
			events->Draw("HcalTBTiming_tbunpacker__RECO.obj.TOF1Stime_-HcalTBTiming_tbunpacker__RECO.obj.TOF2Stime_>>stime(1000,-1000,0)");
			TH1* stime = (TH1*) gROOT->FindObject("stime");
			monitoring_[ri.runNumber_].push_back(stime);

			events->Draw("HcalTBTiming_tbunpacker__RECO.obj.TOF1Jtime_-HcalTBTiming_tbunpacker__RECO.obj.TOF2Jtime_>>jtime(1000,-1000,0)");
			TH1* jtime = (TH1*) gROOT->FindObject("jtime");
			monitoring_[ri.runNumber_].push_back(jtime);
		}
	}
		}

void TestbeamPreprocessor::processBeamCounters(TTree* events, RunInfo& ri) {

	events->Draw(
			"HcalTBBeamCounters_tbunpacker__RECO.obj.BH1adc_>>bh1(500,0,500)");
	TH1* bh1 = (TH1*) gROOT->FindObject("bh1");
	std::pair<double, double> ans1 = fitStabilisedGaussian(bh1);
	ri.bh1Max_ = static_cast<int> (1.05 * ans1.first + 3 * ans1.second);

	events->Draw(
			"HcalTBBeamCounters_tbunpacker__RECO.obj.BH2adc_>>bh2(500,0,500)");
	TH1* bh2 = (TH1*) gROOT->FindObject("bh2");
	std::pair<double, double> ans2 = fitStabilisedGaussian(bh2);
	ri.bh2Max_ = static_cast<int> (1.05 * ans2.first + 3 * ans2.second);

	events->Draw(
			"HcalTBBeamCounters_tbunpacker__RECO.obj.BH3adc_>>bh3(500,0,500)");
	TH1* bh3 = (TH1*) gROOT->FindObject("bh3");
	std::pair<double, double> ans3 = fitStabilisedGaussian(bh3);
	ri.bh3Max_ = static_cast<int> (1.05 * ans3.first + 3 * ans3.second);

	events->Draw(
			"HcalTBBeamCounters_tbunpacker__RECO.obj.BH4adc_>>bh4(500,0,500)");
	TH1* bh4 = (TH1*) gROOT->FindObject("bh4");
	std::pair<double, double> ans4 = fitStabilisedGaussian(bh4);
	ri.bh4Max_ = static_cast<int> (1.05 * ans4.first + 3 * ans4.second);

	if (withMonitoring_) {
		std::cout << "\tBH1: threshold = " << ri.bh1Max_ << "\n";
		std::cout << "\tBH2: threshold = " << ri.bh2Max_ << "\n";
		std::cout << "\tBH3: threshold = " << ri.bh3Max_ << "\n";
		std::cout << "\tBH4: threshold = " << ri.bh4Max_ << "\n";
		monitoring_[ri.runNumber_].push_back(bh1);
		monitoring_[ri.runNumber_].push_back(bh2);
		monitoring_[ri.runNumber_].push_back(bh3);
		monitoring_[ri.runNumber_].push_back(bh4);
	}
}

std::pair<double, double> TestbeamPreprocessor::fitStabilisedGaussian(
		TH1* histo) {
	TF1 g("g", "gaus", histo->GetMean() - histo->GetRMS() / 2, histo->GetMean()
			+ histo->GetRMS() / 2);
	histo->Fit(&g, "RQ");
	TF1 improved("imp", "gaus", g.GetParameter(1) - 5* g .GetParameter(2),
			g.GetParameter(1) + 5* g .GetParameter(2));
	histo->Fit(&improved, "RQ");
	std::pair<double, double> ans(improved.GetParameter(1),
			improved.GetParameter(2));
	double quality = improved.GetChisquare() / improved.GetNDF();
	if (quality > 50) {
		std::cout << "\tWARNING: Fit quality is poor. Chisq/NDF = " << quality
				<< "\n";
	}
	return ans;
}

void TestbeamPreprocessor::populateEnergiesAndRuns() {
	//Filthy utility method!

	//If any of these change, don't forget to update the sizes in the call
	//arrToVec below!
	int a2GeVs[] = { 30139, 30140, 30141, 30146, 29739, 29740, 29950, 29951,
			29953, 29954, 29955, 29956, 29957, 29958, 29959 };
	int a3GeVs[] = { 29689, 29690, 29691, 29694, 29695, 29703, 29704, 29705,
			29706, 29707, 30127, 30128, 30129, 30131, 30133 };
	int a4GeVs[] = { 29976, 29977, 29983, 29984, 29986, 29988, 29989, 29990,
			29991 };
	int a5GeVs[] = { 29713, 29728, 29729, 29730, 29731, 29754, 29755, 30114,
			30115, 30116, 30117, 30120 };
	int a6GeVs[] = { 29721, 29722, 29725, 29726, 29757, 29758, 29759, 30111,
			30112, 30113 };
	int a7GeVs[] = { 29732, 29733, 29734, 29760, 29761, 29762, 30110 };
	int a8GeVs[] = { 29735, 29736, 29763, 29764, 29767 };
	int a9GeVs[] = { 29650, 29737, 29738, 30109, 15507, 15508 };
	int a20GeVs[] = { 30379 };
	int a30GeVs[] = { 30553 };
	int a50GeVs[] = { 30384, 16031 };
	int a100GeVs[] = { 30386 };
	int a150GeVs[] = { 30387 };
	int a200GeVs[] = { 30388 };
	int a300GeVs[] = { 30389, 30390 };

	energiesAndRuns_[2] = arrToVec(a2GeVs, 15);
	energiesAndRuns_[3] = arrToVec(a3GeVs, 15);
	energiesAndRuns_[4] = arrToVec(a4GeVs, 9);
	energiesAndRuns_[5] = arrToVec(a5GeVs, 12);
	energiesAndRuns_[6] = arrToVec(a6GeVs, 10);
	energiesAndRuns_[7] = arrToVec(a7GeVs, 7);
	energiesAndRuns_[8] = arrToVec(a8GeVs, 5);
	energiesAndRuns_[9] = arrToVec(a9GeVs, 7);
	energiesAndRuns_[20] = arrToVec(a20GeVs, 1);
	energiesAndRuns_[30] = arrToVec(a30GeVs, 1);
	energiesAndRuns_[50] = arrToVec(a50GeVs, 2);
	energiesAndRuns_[100] = arrToVec(a100GeVs, 1);
	energiesAndRuns_[150] = arrToVec(a150GeVs, 1);
	energiesAndRuns_[200] = arrToVec(a200GeVs, 1);
	energiesAndRuns_[300] = arrToVec(a300GeVs, 2);

}

