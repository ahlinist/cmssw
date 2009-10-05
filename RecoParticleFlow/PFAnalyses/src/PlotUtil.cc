#include "RecoParticleFlow/PFAnalyses/interface/PlotUtil.h"

#include <iostream>
#include <TROOT.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TList.h>
#include <TPaveText.h>
#include <cmath>
#include <TF1.h>
#include <TText.h>
#include <TMultiGraph.h>
#include <stdexcept>
#include <utility>

using namespace std;

PlotUtil::PlotUtil() :
	rootFile_(0), amInitialised_(false), autoFlush_(false), edgeSize_(0),
			flushCount_(0) {

	colors_.push_back(kBlue + 3);
	colors_.push_back(kRed);
	colors_.push_back(kGray + 3);
	colors_.push_back(kOrange + 1);
	colors_.push_back(kPink - 9);
	colors_.push_back(kCyan + 3);
	colors_.push_back(kAzure);

}

void PlotUtil::init() {

	rootFile_ = new TFile("PlotUtil.root", "recreate");
	amInitialised_ = true;

}

Color_t PlotUtil::nextColor() {
	return colors_[accumulatedObjects_.size() % colors_.size()];
}

PlotUtil::~PlotUtil() {
	for (vector<TObject*>::iterator del = deleteOnDestruction_.begin(); del
			!= deleteOnDestruction_.end(); ++del) {
		if (!*del)
			delete *del;
	}

}

void PlotUtil::setGraphicsFile(std::string graphicsFile) {
	graphicsFile_ = graphicsFile;
}

void PlotUtil::setMacroFile(std::string macroFile) {
	macroFile_ = macroFile;
}

TH1* PlotUtil::getHisto(const std::string& name) {
	TH1* o = (TH1*) gDirectory->FindObjectAny(name.c_str());

	return o;

}

void PlotUtil::newPage() {
	accumulatedObjects_.clear();
}

void PlotUtil::flushPage() {
	flushAccumulatedObjects(graphicsFile_);
	flushAccumulatedObjects(macroFile_);
}

void PlotUtil::accumulateTable(JGraph q, std::string title) {
	pair<JGraph, string> p = make_pair<JGraph, string> (q, title);
	accumulatedTables_.push_back(p);

}

void PlotUtil::flushTables() {
	cout << __PRETTY_FUNCTION__ << "\n";
	vector<pair<JGraph, string> >::const_iterator c =
			accumulatedTables_.begin();
	for (; c != accumulatedTables_.end(); ++c) {
		cout << "*** PlotUtil Table ***\n";
		cout << c->second << "\n";
		cout << c->first << "\n";
	}
	accumulatedTables_.clear();
}

void PlotUtil::addTitle(const std::string& theTitle) {
	//	auto_ptr<TText> title(new TText(0, 0, theTitle.c_str()));
	TText* title = new TText(0, 0, theTitle.c_str());
	title->SetNDC();
	title->SetX(0.1);
	title->SetY(0.9);
	accumulateObjects(title, "");
	deleteOnDestruction_.push_back(title);
}

void PlotUtil::accumulateObjects(TObject* o, std::string options) {
	std::pair<TObject*, std::string> k(o, options);
	accumulatedObjects_.push_back(k);
	if (autoFlush_ && accumulatedObjects_.size() > flushCount_) {
		std::cout << __PRETTY_FUNCTION__ << ": autoflushing... " << std::endl;
		flushPage();
		newPage();
	}
}

void PlotUtil::flushAccumulatedObjects(std::string filename) {

	//how big to divide canvas?
	//For up to 9 objects, I suppose it should be X x Y big...
	//				  {1  2  3  4 ... objects}
	int optimalsX[] = { 1, 2, 2, 2, 3, 3, 3, 3, 3 };
	int optimalsY[] = { 1, 1, 2, 2, 2, 2, 3, 3, 3 };
	int dimX, dimY;
	if (edgeSize_ > 0) {
		dimX = dimY = edgeSize_;
	} else if (accumulatedObjects_.size() <= 9) {
		dimX = optimalsX[accumulatedObjects_.size() - 1];
		dimY = optimalsY[accumulatedObjects_.size() - 1];
	} else {
		dimX = dimY = static_cast<int> (ceil(sqrt(accumulatedObjects_.size())));
	}
	//check edgeSize_; if > 0 divide canvas appropriately
	//if more objects exist that edgeSize_^2, flush each page separately

	TCanvas canv("Accumulated", "Accumulated");
	canv.Divide(dimX, dimY);
	canv.Update();
	//count which subcanvas we're on
	unsigned count(1);
	//how many objects printed so far?
	unsigned seen(0);
	for (std::vector<std::pair<TObject*, std::string> >::iterator it =
			accumulatedObjects_.begin(); it != accumulatedObjects_.end(); ++it) {
		canv.cd(count);
		//Auto flush when canvas gets full
		if (seen >= edgeSize_ * edgeSize_ && edgeSize_ > 0) {
			std::cout << "Flushing page..." << std::endl;
			canv.cd();
			canv.Update();
			canv.Print(filename.c_str());
			//D clears subpads too, allegedly
			canv.Clear("D");
			canv.cd();
			canv.Divide(dimX, dimY);
			//reset subcanvas count
			count = 1;
		}
		TObject* o = (*it).first;
		std::string options = (*it).second;
		//gPad->UseCurrentStyle();
		if (o->InheritsFrom("THStack")) {
			TLegend* l = legendForStack((THStack*) o);
			o->Draw(options.c_str());
			l->Draw();
		} else if (o->InheritsFrom("TMultiGraph")) {
			TMultiGraph* g = (TMultiGraph*) o;
			g->Draw(options.c_str());
			TLegend* l = legendForMultiGraph(g);
			l->Draw();

		} else {
			o->Draw(options.c_str());
		}

		canv.Update();
		canv.Modified();
		o->Write();
		++count;
		++seen;
	}

	canv.cd();
	canv.Update();
	canv.Print(filename.c_str());
}

TH1* PlotUtil::printHisto(const std::string& name, const std::string& title,
		const std::string& xtitle, Color_t line = kBlack,
		Color_t fill = kWhite, int thickness) throw(exception) {
	TH1* histo = formatHisto(name, title, xtitle, line, fill, thickness);
	if (histo) {
		histo->Draw();
		gPad->Print(graphicsFile_.c_str());
	} else {
		invalid_argument e("Can't get histogram");
		throw e;
	}
	return histo;
}

void PlotUtil::accumulateSpecial(TObject* o, TStyle* s,
		std::string drawOptions, std::string preferredName) {
	PlotSpecial ps(o, s, drawOptions, preferredName);
	accumulatedSpecials_.push_back(ps);
}

void PlotUtil::flushSpecials(std::string directory) {
	for (std::vector<PlotSpecial>::iterator i = accumulatedSpecials_.begin(); i
			!= accumulatedSpecials_.end(); ++i) {
		PlotSpecial ps = *i;
		//TODO: this should be an option really
		ps.style_->SetOptTitle(false);
		ps.style_->cd();
		TCanvas temp("temp", "temp");
		if (ps.obj_->InheritsFrom("THStack")) {
			TLegend* l = legendForStack((THStack*) ps.obj_);
			ps.obj_->Draw(ps.options_.c_str());
			l->Draw();
		} else if (ps.obj_->InheritsFrom("TMultiGraph")) {
			TMultiGraph* g = (TMultiGraph*) ps.obj_;
			g->Draw(ps.options_.c_str());
			TLegend* l = legendForMultiGraph(g);
			l->Draw();
		} else {
			ps.obj_->Draw(ps.options_.c_str());
		}
		std::string destination(directory);
		destination.append("/");
		destination.append(ps.preferredName_);
		std::string macro(destination);
		macro.append(".C");
		std::string postscript(destination);
		postscript.append(".eps");
		temp.Update();
		temp.Modified();
		temp.Print(macro.c_str());
		temp.Print(postscript.c_str());
	}

}

TLegend* PlotUtil::legendForStack(THStack* stack) {
	TLegend* legend = new TLegend(0.55, 0.7, 0.95, 0.95);
	TList* list = stack->GetHists();
	TIter next(list);
	TH1* hist;
	while (hist = (TH1*) next())
		legend->AddEntry(hist, "", "LPF");

	deleteOnDestruction_.push_back(legend);
	return legend;

}

TLegend* PlotUtil::legendForMultiGraph(TMultiGraph* multi) {
	TLegend* legend = new TLegend(0.55, 0.7, 0.95, 0.95);
	TList* list = multi->GetListOfGraphs();
	TIter next(list);
	TGraph* graph;
	while (graph = (TGraph*) next())
		legend->AddEntry(graph, "", "LP");

	deleteOnDestruction_.push_back(legend);
	return legend;

}

TH1* PlotUtil::formatHisto(const std::string& name, const std::string& title,
		const std::string& xtitle, Color_t line = kBlack,
		Color_t fill = kWhite, int thickness) throw(exception) {
	//do things
	TH1* histo = getHisto(name);
	if (histo) {
		histo->SetTitle(title.c_str());
		histo->SetXTitle(xtitle.c_str());
		histo->SetLineColor(line);
		histo->SetFillColor(fill);
		histo->SetLineWidth(thickness);

		//		TPaveText t(0.7, 0.8, 0.9, 0.9);
		//		t.AddText(graphicsFile_.c_str());
		//		t.Draw();

	} else {
		std::cout << "Can't format histogram, returning null pointer!"
				<< std::endl;
		invalid_argument e("Can't get histogram");
		throw e;
	}
	return histo;

}

void PlotUtil::formatGraph(TGraph* graph, const std::string& title,
		const std::string& xtitle, const std::string ytitle, Color_t marker,
		int size, int thickness) {
	graph->SetTitle(title.c_str());
	graph->GetXaxis()->SetTitle(xtitle.c_str());
	graph->GetYaxis()->SetTitle(ytitle.c_str());
	graph->SetMarkerColor(marker);
	graph->SetMarkerStyle(4);
	graph->SetMarkerSize(size);
	graph->SetLineWidth(thickness);
}

TStyle* PlotUtil::makeStyle(const std::string& name) {
	TStyle* style = new TStyle(name.c_str(), name.c_str());
	style->SetCanvasColor(kWhite); // background is no longer mouse-dropping white
	style->SetPalette(1, 0); // blue to red false color palette. Use 9 for b/w
	style->SetCanvasBorderMode(0); // turn off canvas borders
	style->SetPadBorderMode(0);
	style->SetPaintTextFormat("5.2f");
	style->SetTitleBorderSize(0);
	style->SetOptStat(1111111);
	style->SetCanvasDefW(500);
	style->SetFrameBorderMode(0);
	style->SetTitleFillColor(kWhite);
	style->SetStatColor(21);
	//style->SetFrameFillColor(TColor::GetColor("#e7e6ed"));
	style->SetFrameFillColor(kWhite);
	//gStyle->SetTitleX(0.08);
	style->SetTitleY(0.97);
	style->SetStatBorderSize(1);
	style->SetPadGridX(true);
	style->SetPadGridY(true);
	deleteOnDestruction_.push_back(style);
	return style;
}

std::pair<double, double> PlotUtil::fitStabilisedGaussian(TH1* histo) {
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
		std::cout << __PRETTY_FUNCTION__
				<< "WARNING: Fit quality is poor. Chisq/NDF = " << quality
				<< "\n";
	}
	double gaussianess = fabs(ans.first / histo->GetMean());
	if (gaussianess > 2.0 || gaussianess < 0.5) {
		std::cout << __PRETTY_FUNCTION__
				<< "WARNING: Histogram isn't gaussian at all!\n";
	}
	return ans;
}

