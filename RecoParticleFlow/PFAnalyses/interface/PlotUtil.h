#ifndef PLOTUTIL_H_
#define PLOTUTIL_H_

#include <vector>
#include <TTree.h>
#include <TObject.h>
#include <string>
#include <TColor.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <utility>
#include <THStack.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TFile.h>
#include <TText.h>
#include <TCanvas.h>
#include <exception>

#include "RecoParticleFlow/PFAnalyses/interface/PlotSpecial.h"

#include "RecoParticleFlow/PFAnalyses/interface/JGraph.h"

class PlotUtil {
public:

	PlotUtil();

	virtual ~PlotUtil();

	void init();

	void setMacroFile(std::string s);

	void setGraphicsFile(std::string s);

	TStyle* makeStyle(const std::string& name);

	TStyle* makeSquashedStyle(const std::string& name);

	/**
	 * Get a histogram pointer
	 * @param name in ROOT namespace to find
	 * @return null pointer if histogram couln't be found
	 */
	TH1* getHisto(const std::string& name);

	template <typename T> T* getType(const std::string& name);

	TH1* printHisto(const std::string& name, const std::string& title, const std::string& xtitle, Color_t line,
			Color_t fill, int thickness = 1) throw(std::exception);

	TH1* formatHisto(const std::string& name, const std::string& title, const std::string& xtitle, Color_t line,
			Color_t fill, int thickness = 1) throw(std::exception);

	void formatGraph(TGraph* graph, const std::string& title, const std::string& xtitle, const std::string ytitle,
			Color_t line, int size = 1.5, int thickness = 1);

	void addTitle(const std::string& title);

	void enableAutoFlush(bool autoFlush, unsigned count = 0) {
		autoFlush_ = autoFlush;
		flushCount_ = count;
	}

	void flushPage(bool squashedTogether = false);

	void newPage();

	void accumulateObjects(TObject* o, std::string options = "");

	void flushAccumulatedObjects(std::string filename, bool squashedTogether = false);

	unsigned numberAccumulated() {
		return accumulatedObjects_.size();
	}

	void setSquareCanvasDimension(unsigned edgeSize) {
		edgeSize_ = edgeSize;
	}

	void accumulateSpecial(TObject* o, TStyle* s, std::string drawOptions, std::string preferredName);

	void accumulateTable(JGraph q, std::string title);

	void flushTables();

	void flushSpecials(std::string directory = "");

	void clearSpecials() {
		accumulatedSpecials_.clear();
	}

	TLegend* legendForStack(THStack* theStack);

	TLegend* legendForMultiGraph(TMultiGraph* multi);

	Color_t nextColor();

	std::vector<Color_t> colorMap() {
		return colors_;
	}

	std::pair<double, double> fitStabilisedGaussian(TH1* histo, double lowRange = 0.0, double highRange = 0.0);

	//Loops over histogram and writes out literal entries of the form
	//<Bin center>	|	<Content>
	bool streamTH1ToGraphFile(std::string filename, TH1* histo, bool recreate = true);

	bool streamTH2ToGraphFile(std::string filename, TH2* histo, bool recreate = true);

	//void closeFiles();
private:
	std::string graphicsFile_;
	std::string macroFile_;
	TFile* rootFile_;

	std::vector<std::pair<TObject*, std::string> > accumulatedObjects_;

	std::vector<PlotSpecial> accumulatedSpecials_;
	std::vector<Color_t> colors_;

	std::vector<std::pair<JGraph, std::string> > accumulatedTables_;

	std::vector<TObject*> deleteOnDestruction_;

	bool amInitialised_;
	bool autoFlush_;
	unsigned flushCount_;
	unsigned edgeSize_;

	unsigned pageCount_;
	TCanvas* workPad_;

};

template <typename T> T* PlotUtil::getType(const std::string& name) {
	T* o = (T*) gDirectory->FindObjectAny(name.c_str());
	return o;
}

#endif /*PLOTUTIL_H_*/
