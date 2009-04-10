#ifndef ENERGYPROCESSING_H_
#define ENERGYPROCESSING_H_
#include <vector>
#include <TTree.h>
#include <TObject.h>
#include <string>
#include <TColor.h>
#include <TH1.h>
#include <TStyle.h>
#include <utility>
#include <THStack.h>
#include <TLegend.h>

#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"

class EnergyProcessing {
public:
	EnergyProcessing() {}

	EnergyProcessing(TTree* t, std::string graphicsFile, std::string macroFile, std::string directory = "", bool tb = true, long nEntries =1000000000);

	void reset(TTree* t, std::string graphicsFile, std::string macroFile, std::string directory, bool tb = true, long nEntries = 1000000000);

	void evaluatePlots(bool lowEnergy = true);

	void doParticleIdPlots();

	//Plots E_HB against E_EB
	void doBananaPlots();

	//Plots total E, E_EB, E_HB
	void doSpectrumPlots();

	//2D scatters of rechits vs. clusters etc.
	void doCorrelationPlots();

	//Histograms of E_emag/E_total
	void doEEmagOnETotPlots();

	void doElectronDiscriminationPlots();

	void doPFCandidatePlots();

	void doBeamCompositionPlots();

	void doLoopedPlots();

	void doBeamSpotPlots();

	void doCandidateTypePlots();

	void closeFiles();

	virtual ~EnergyProcessing();

private:

	TTree* tree_;
	TStyle* defaultStyle_;
	PlotUtil util_;

	long nEntries_;
	long zero_;

	std::string directory_;
	std::string graphicsFile_;
	std::string macroFile_;

	bool tb_;

	void adcPerBin(TH1* histo, unsigned adcPerBin);

};

#endif /*ENERGYPROCESSING_H_*/

