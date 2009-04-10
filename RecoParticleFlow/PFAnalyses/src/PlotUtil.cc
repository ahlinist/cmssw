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

Color_t PlotUtil::tb_rechits = kGreen;
Color_t PlotUtil::pf_rechits = kRed;
Color_t PlotUtil::pf_cluster = kViolet + 7;


PlotUtil::PlotUtil() :
	rootFile_(0), amInitialised_(false) {

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
	//rootFile_->Write();
	//	if(rootFile_ != 0) {
	//		rootFile_->Close();
	//		delete rootFile_;
	//	}
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

void PlotUtil::accumulateObjects(TObject* o, std::string options) {
	std::pair<TObject*, std::string> k(o, options);
	accumulatedObjects_.push_back(k);
}

void PlotUtil::flushAccumulatedObjects(std::string filename) {

	//how big to divide canvas?
	//For up to 9 objects, I suppose it should be X x Y big...
	//				  {1  2  3  4 ... objects}
	int optimalsX[] = { 1, 2, 2, 2, 3, 3, 3, 3, 3 };
	int optimalsY[] = { 1, 1, 2, 2, 2, 2, 3, 3, 3 };
	int dimX, dimY;
	if (accumulatedObjects_.size() <= 9) {
		dimX = optimalsX[accumulatedObjects_.size() -1];
		dimY = optimalsY[accumulatedObjects_.size() -1];
	} else {
		dimX = dimY = static_cast<int>(ceil(sqrt(accumulatedObjects_.size())));
	}

	TCanvas canv("Accumulated", "Accumulated");
	canv.Divide(dimX, dimY);
	canv.Update();
	unsigned count(1);
	for (std::vector<std::pair<TObject*, std::string> >::iterator it =
			accumulatedObjects_.begin(); it != accumulatedObjects_.end(); ++it) {
		canv.cd(count);
		TObject* o = (*it).first;
		std::string options = (*it).second;
		//gPad->UseCurrentStyle();
		if (o->InheritsFrom("THStack")) {
			TLegend* l = legendForStack((THStack*) o);
			o->Draw(options.c_str());
			l->Draw();
		} else {
			o->Draw(options.c_str());
		}

		canv.Update();
		canv.Modified();
		o->Write();
		++count;
	}

	canv.cd();
	canv.Update();
	canv.Print(filename.c_str());
}

TH1* PlotUtil::printHisto(const std::string& name, const std::string& title,
		const std::string& xtitle, Color_t line = kBlack,
		Color_t fill = kWhite, int thickness) {
	TH1* histo = formatHisto(name, title, xtitle, line, fill, thickness);
	histo->Draw();
	gPad->Print(graphicsFile_.c_str());
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
	while (hist = (TH1*)next() )
		legend->AddEntry(hist, "", "F");

	return legend;

}

TH1* PlotUtil::formatHisto(const std::string& name, const std::string& title,
		const std::string& xtitle, Color_t line = kBlack,
		Color_t fill = kWhite, int thickness) {
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
	}
	return histo;

}

void PlotUtil::formatGraph(TGraph* graph, const std::string& title,
		const std::string& xtitle, const std::string ytitle, Color_t marker,
		int thickness) {
	graph->SetTitle(title.c_str());
	graph->GetXaxis()->SetTitle(xtitle.c_str());
	graph->GetYaxis()->SetTitle(ytitle.c_str());
	graph->SetMarkerColor(marker);
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
	style->SetFrameFillColor(TColor::GetColor("#e7e6ed"));
	//gStyle->SetTitleX(0.08);
	style->SetTitleY(0.97);
	style->SetStatBorderSize(1);
	style->SetPadGridX(true);
	style->SetPadGridY(true);
	return style;
}

std::pair<double, double> PlotUtil::fitStabilisedGaussian(TH1* histo) {
	TF1 g("g", "gaus", histo->GetMean() - histo->GetRMS()/2, histo->GetMean()
			+ histo->GetRMS()/2);
	histo->Fit(&g, "RQ");
	TF1 improved("imp", "gaus", g.GetParameter(1) - 5*g.GetParameter(2),
			g.GetParameter(1) + 5*g.GetParameter(2));
	histo->Fit(&improved, "RQ");
	std::pair<double, double> ans(improved.GetParameter(1),
			improved.GetParameter(2));
	double quality = improved.GetChisquare()/improved.GetNDF();
	if (quality > 50) {
		std::cout << __PRETTY_FUNCTION__ << "WARNING: Fit quality is poor. Chisq/NDF = " << quality
				<< "\n";
	}
	return ans;
}

