#include <string>
class Plotter {
    public:
	Plotter(){

	        gROOT->LoadMacro("./tdrstyle.cxx");
	        setTDRStyle();

		inFile = new TFile("tteffAnalysis.root");
		tree = (TTree *) (inFile->Get("TTEffTree"));
	}
	~Plotter(){
		delete tree;
		delete inFile;
	};

	void DrawHistogram(const char* varexp, const TCut& selection);
	void SetXTitle(char* title) {plotXtitle   = title;}
	void SetFileName(char* name){plotFileName = name;}

    private:
	TFile* inFile;
	TTree* tree;
	char* plotXtitle;
	char* plotFileName;
};

void Plotter::DrawHistogram(const char* varexp, const TCut& selection){

	char* varexp1 = varexp;

	string s_varexp = string(varexp);

	size_t posbegin = s_varexp.find(">>");
	size_t posend   = s_varexp.find("(");

	s_varexp = s_varexp.substr(0,posbegin+2) + "hden" + s_varexp.substr(posend,s_varexp.length()-posend);

	char* varexp2 = s_varexp.c_str();

        tree->Draw(varexp1,selection,"e");
        tree->Draw(varexp2,"","h");

        TH1F *hnum = (TH1F*)gDirectory->Get("hnum");
        TH1F *hden = (TH1F*)gDirectory->Get("hden");

	hnum->SetStats(0);
        hnum->SetTitle("");
        //hnum->SetAxisRange(0.,1.,"X");
        hnum->GetXaxis()->SetTitle(plotXtitle);
        hnum->GetYaxis()->SetTitle("Efficiency");
        hnum->SetMarkerColor(kBlack);
        hnum->SetMarkerSize(1.);
        hnum->SetLineWidth(1);
        hnum->SetLineColor(kBlack);
        hnum->SetMarkerStyle(kFullDotLarge);

        hnum->Divide(hden);
        hnum->Draw("e");

        gPad->SaveAs(plotFileName);
}
