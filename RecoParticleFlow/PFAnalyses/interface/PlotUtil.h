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
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <TFile.h>
#include <TText.h>
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

	/**
	 * Get a histogram pointer
	 * @param name in ROOT namespace to find
	 * @return null pointer if histogram couln't be found
	 */
	TH1* getHisto(const std::string& name);

	TH1* printHisto(const std::string& name, const std::string& title,
			const std::string& xtitle, Color_t line, Color_t fill,
			int thickness = 1) throw(std::exception);

	TH1* formatHisto(const std::string& name, const std::string& title,
			const std::string& xtitle, Color_t line, Color_t fill,
			int thickness = 1) throw(std::exception);

	void formatGraph(TGraph* graph, const std::string& title,
			const std::string& xtitle, const std::string ytitle, Color_t line,
			int size = 1.5, int thickness = 1);

	void addTitle(const std::string& title);

	void enableAutoFlush(bool autoFlush, unsigned count = 0) {
		autoFlush_ = autoFlush;
		flushCount_ = count;
	}

	void flushPage();

	void newPage();

	void accumulateObjects(TObject* o, std::string options = "");

	void flushAccumulatedObjects(std::string filename);

	unsigned numberAccumulated() {
		return accumulatedObjects_.size();
	}

	void setSquareCanvasDimension(unsigned edgeSize) {
		edgeSize_ = edgeSize;
	}

	void accumulateSpecial(TObject* o, TStyle* s, std::string drawOptions,
			std::string preferredName);

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

	std::pair<double, double> fitStabilisedGaussian(TH1* histo);

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

};

#endif /*PLOTUTIL_H_*/
