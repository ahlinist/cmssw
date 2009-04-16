#ifndef PLOTUTIL_H_
#define PLOTUTIL_H_

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
#include <TGraphErrors.h>
#include <TFile.h>

#include "RecoParticleFlow/PFAnalyses/interface/PlotSpecial.h"

class PlotUtil {
public:

	static Color_t tb_rechits;
	static Color_t pf_rechits;
	static Color_t pf_cluster;


	PlotUtil();
	virtual ~PlotUtil();

	void init();

	void setMacroFile(std::string s);
	void setGraphicsFile(std::string s);

	TStyle* makeStyle(const std::string& name);

	TH1* getHisto(const std::string& name);
	TH1* printHisto(const std::string& name, const std::string& title,
			const std::string& xtitle, Color_t line, Color_t fill,
			int thickness = 1);
	TH1* formatHisto(const std::string& name, const std::string& title,
			const std::string& xtitle, Color_t line, Color_t fill,
			int thickness = 1);

	void formatGraph(TGraph* graph, const std::string& title,
			const std::string& xtitle, const std::string ytitle, Color_t line, int size = 1.5,
			int thickness = 1);

	void flushPage();
	void newPage();
	void accumulateObjects(TObject* o, std::string options = "");
	void flushAccumulatedObjects(std::string filename);

	void accumulateSpecial(TObject* o, TStyle* s,
			std::string drawOptions, std::string preferredName);

	void flushSpecials(std::string directory = "");

	TLegend* legendForStack(THStack* theStack);

	Color_t nextColor();

	std::pair<double, double> fitStabilisedGaussian(TH1* histo);

	//void closeFiles();
private:
	std::string graphicsFile_;
	std::string macroFile_;
	TFile* rootFile_;

	std::vector<std::pair<TObject*, std::string> > accumulatedObjects_;
	std::vector<PlotSpecial> accumulatedSpecials_;
	std::vector<Color_t> colors_;

	bool amInitialised_;


};

#endif /*PLOTUTIL_H_*/
