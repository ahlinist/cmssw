#include "TStyle.h"

void setMultPlotStyle()
{
    TStyle *MultPlotStyle = new TStyle("MultPlotStyle","Style for P-TDR");

// For the canvas:
    MultPlotStyle->SetCanvasBorderMode(0);
    MultPlotStyle->SetCanvasColor(kWhite);
    MultPlotStyle->SetCanvasDefH(600); //Height of canvas
    MultPlotStyle->SetCanvasDefW(600); //Width of canvas
    MultPlotStyle->SetCanvasDefX(0);   //POsition on screen
    MultPlotStyle->SetCanvasDefY(0);

// For the Pad:
    MultPlotStyle->SetPadBorderMode(0);
    // MultPlotStyle->SetPadBorderSize(Width_t size = 1);
    MultPlotStyle->SetPadColor(kWhite);
    MultPlotStyle->SetPadGridX(false);
    MultPlotStyle->SetPadGridY(false);
    MultPlotStyle->SetGridColor(0);
    MultPlotStyle->SetGridStyle(3);
    MultPlotStyle->SetGridWidth(1);

// For the frame:
    MultPlotStyle->SetFrameBorderMode(0);
    MultPlotStyle->SetFrameBorderSize(1);
    MultPlotStyle->SetFrameFillColor(0);
    MultPlotStyle->SetFrameFillStyle(0);
    MultPlotStyle->SetFrameLineColor(1);
    MultPlotStyle->SetFrameLineStyle(1);
    MultPlotStyle->SetFrameLineWidth(1);

// For the histo:
    // MultPlotStyle->SetHistFillColor(1);
    // MultPlotStyle->SetHistFillStyle(0);
    MultPlotStyle->SetHistLineColor(1);
    MultPlotStyle->SetHistLineStyle(0);
    MultPlotStyle->SetHistLineWidth(1);
    // MultPlotStyle->SetLegoInnerR(Float_t rad = 0.5);
    // MultPlotStyle->SetNumberContours(Int_t number = 20);

    MultPlotStyle->SetEndErrorSize(2);
//  MultPlotStyle->SetErrorMarker(20);
    MultPlotStyle->SetErrorX(0.);

    MultPlotStyle->SetMarkerStyle(20);

//For the fit/function:
    MultPlotStyle->SetOptFit(1);
    MultPlotStyle->SetFitFormat("5.4g");
    MultPlotStyle->SetFuncColor(2);
    MultPlotStyle->SetFuncStyle(1);
    MultPlotStyle->SetFuncWidth(1);

//For the date:
    MultPlotStyle->SetOptDate(0);
    // MultPlotStyle->SetDateX(Float_t x = 0.01);
    // MultPlotStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
    MultPlotStyle->SetOptFile(0);
    MultPlotStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    MultPlotStyle->SetStatColor(kWhite);
    MultPlotStyle->SetStatFont(42);
    MultPlotStyle->SetStatFontSize(0.025);
    MultPlotStyle->SetStatTextColor(1);
    MultPlotStyle->SetStatFormat("6.4g");
    MultPlotStyle->SetStatBorderSize(1);
    MultPlotStyle->SetStatH(0.1);
    MultPlotStyle->SetStatW(0.15);
    // MultPlotStyle->SetStatStyle(Style_t style = 1001);
    // MultPlotStyle->SetStatX(Float_t x = 0);
    // MultPlotStyle->SetStatY(Float_t y = 0);

// Margins:
    MultPlotStyle->SetPadTopMargin(0.05);
    MultPlotStyle->SetPadBottomMargin(0.13);
    MultPlotStyle->SetPadLeftMargin(0.16);
    MultPlotStyle->SetPadRightMargin(0.05);

// For the Global title:

    MultPlotStyle->SetOptTitle(1);
    MultPlotStyle->SetTitleFont(42);
    MultPlotStyle->SetTitleColor(1);
    MultPlotStyle->SetTitleTextColor(1);
    MultPlotStyle->SetTitleFillColor(10);
    MultPlotStyle->SetTitleFontSize(0.05);
    // MultPlotStyle->SetTitleH(0); // Set the height of the title box
    // MultPlotStyle->SetTitleW(0); // Set the width of the title box
    MultPlotStyle->SetTitleX(0.07); // Set the position of the title box
    MultPlotStyle->SetTitleY(0.95); // Set the position of the title box
    // MultPlotStyle->SetTitleStyle(Style_t style = 1001);
    MultPlotStyle->SetTitleBorderSize(0);

// For the axis titles:

    MultPlotStyle->SetTitleColor(1, "XYZ");
    MultPlotStyle->SetTitleFont(42, "XYZ");
    MultPlotStyle->SetTitleSize(0.05, "XYZ");
    // MultPlotStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // MultPlotStyle->SetTitleYSize(Float_t size = 0.02);
    //MultPlotStyle->SetTitleXOffset(0.9);
    //MultPlotStyle->SetTitleYOffset(1.25);
    MultPlotStyle->SetTitleOffset(2., "X"); // Another way to set the Offset
    MultPlotStyle->SetTitleOffset(2., "Y"); // Another way to set the Offset
    MultPlotStyle->SetTitleOffset(2., "Z"); // Another way to set the Offset


// For the axis labels:

    MultPlotStyle->SetLabelColor(1, "XYZ");
    MultPlotStyle->SetLabelFont(42, "XYZ");
    MultPlotStyle->SetLabelOffset(0.007, "XYZ");
    MultPlotStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

    MultPlotStyle->SetAxisColor(1, "XYZ");
    MultPlotStyle->SetStripDecimals(kTRUE);
    MultPlotStyle->SetTickLength(0.03, "XYZ");
    MultPlotStyle->SetNdivisions(510, "XYZ");
    MultPlotStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    MultPlotStyle->SetPadTickY(1);

// Change for log plots:
    MultPlotStyle->SetOptLogx(0);
    MultPlotStyle->SetOptLogy(0);
    MultPlotStyle->SetOptLogz(0);

// Postscript options:
    MultPlotStyle->SetPaperSize(20.,20.);
    // MultPlotStyle->SetLineScalePS(Float_t scale = 3);
    // MultPlotStyle->SetLineStyleString(Int_t i, const char* text);
    // MultPlotStyle->SetHeaderPS(const char* header);
    // MultPlotStyle->SetTitlePS(const char* pstitle);

    // MultPlotStyle->SetBarOffset(Float_t baroff = 0.5);
    // MultPlotStyle->SetBarWidth(Float_t barwidth = 0.5);
    // MultPlotStyle->SetPaintTextFormat(const char* format = "g");
    // MultPlotStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // MultPlotStyle->SetTimeOffset(Double_t toffset);
    // MultPlotStyle->SetHistMinimumZero(kTRUE);

    MultPlotStyle->cd();
}



void kinplots (string folder, string histo, int log = 0)
{

    setTDRStyle();

    TFile *signal = TFile::Open("PomWigDiJets.root");
    TFile *background = TFile::Open("QCD.root");
//    TFile *all = TFile::Open("ALL.root");




///////////////////
//////////////////
//Legenda
    new TCanvas;


    TH1D signalLeg("signalLeg", "signalLeg", 2, 0.0, 1.0);
    signalLeg.Fill(0.2);
    signalLeg.SetLineColor(1);
    signalLeg.Draw();

    TH1D backgroundLeg("backgroundLeg", "backgroundLeg", 2, 0.0, 1.0);
    backgroundLeg.Fill(0.8);
    backgroundLeg.SetLineStyle(7);
    backgroundLeg.Draw("same");




    TLegend standardLegend(0.6,0.85,0.9,0.9);
    standardLegend.SetFillColor(0);
    standardLegend.AddEntry("signalLeg","POMWIG SD Dijets","L");
    standardLegend.AddEntry("backgroundLeg","PYTHIA ND Dijets","L");

///////////////////
//////////////////



    new TCanvas;
    gPad->SetLogy(log);





    signal->cd();
    TDirectoryFile *folder_;
    signal->GetObject(folder.c_str(),folder_);
    TH1 *histo_ = (TH1*)folder_->Get(histo.c_str());
    histo_->DrawNormalized();
//    signal->Close("R");


    background->cd();
    background->GetObject(folder.c_str(),folder_);
    histo_ = (TH1*)folder_->Get(histo.c_str());
    histo_->SetLineStyle(7);
    histo_->DrawNormalized("same");
    //background->Close("R");

    standardLegend.Draw("same");

    string command = "mkdir -p kinplots/" + folder;
    gSystem->Exec(command.c_str());

    string saveas = "kinplots/" + folder + "/" + histo + ".eps";
    gPad->SaveAs(saveas.c_str());

//    signal->Close("R");
//    background->Close("R");
//    all->Close("R");
}


void kinPlotProducer ()
{




/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////

    vector< string > kinPlotsFolders;
    kinPlotsFolders.push_back("RecoKinPlots");
    kinPlotsFolders.push_back("HLTKinPlots");
    kinPlotsFolders.push_back("OfflineKinPlotsHf00Castor00");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf0Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf1Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf2Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf3Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf4Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf5Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf6Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf7Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf8Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf9Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf10Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf11Castor100");
    kinPlotsFolders.push_back("OfflineKinPlotsHf12Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf13Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf14Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf15Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf16Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf17Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf18Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf19Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf20Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf21Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf22Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf23Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf24Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf25Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf26Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf27Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf28Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf29Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf30Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf31Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf32Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf33Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf34Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf35Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf36Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf37Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf38Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf39Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf40Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf41Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf42Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf43Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf44Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf45Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf46Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf47Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf48Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf49Castor100");
//    kinPlotsFolders.push_back("OfflineKinPlotsHf50Castor100");


    for (int i=0;i < kinPlotsFolders.size(); i++)
    {

        kinplots (kinPlotsFolders.at(i), "h_EtaJet1");
        kinplots (kinPlotsFolders.at(i), "h_EtaJet2");
        kinplots (kinPlotsFolders.at(i), "h_DeltaEtaJets");
        kinplots (kinPlotsFolders.at(i), "h_EtJet1",1);
        kinplots (kinPlotsFolders.at(i), "h_EtJet2",1);
        kinplots (kinPlotsFolders.at(i), "h_AveEtJets",1);
        kinplots (kinPlotsFolders.at(i), "h_DeltaEtJets",1);
        kinplots (kinPlotsFolders.at(i), "h_PhiJet1");
        kinplots (kinPlotsFolders.at(i), "h_PhiJet2");
        kinplots (kinPlotsFolders.at(i), "h_DeltaPhiJets");
        kinplots (kinPlotsFolders.at(i), "h_DeltaRJets");
        kinplots (kinPlotsFolders.at(i), "h_MultTracks");
        kinplots (kinPlotsFolders.at(i), "h_EnergyWeightedEta",1);

    }

}


void multplots (string folder, string folderRandom, string title, string histo)
{

    //OffSets
    double xOffSet = 1.5;
    double yOffSet = 1.7;
    double zOffSet = 2.;

    setMultPlotStyle();

    TFile *signal = TFile::Open("PomWigDiJets.root");
    TFile *background = TFile::Open("QCD.root");
    TFile *all = TFile::Open("ALL.root");

    TFile * fileSources[3];
    fileSources[0] = signal;
    fileSources[1] = background;
    fileSources[2] = all;

    string sigbackall;
    for (int i=0;i < 3; i++)
    {
        if (i == 0)
        {
            sigbackall = "signal";
        }
        if (i == 1)
        {
            sigbackall = "background";
        }
        if (i == 2)
        {
            sigbackall = "signal_plus_background";
        }

        //open file to save errors on (0,0) bin
        string stat_errors_file = "multplots/" + folder + "/" +sigbackall + "/" + histo + "_stat_errors.txt";
        ofstream stat_errors;
        stat_errors.open(stat_errors_file.c_str(),ios::out);


        //propper
        new TCanvas;
        fileSources[i]->cd();
        TDirectoryFile *folder_;
        fileSources[i]->GetObject(folder.c_str(),folder_);
        TH2 *histo_ = (TH1*)folder_->Get(histo.c_str());
        histo_->SetTitle(title.c_str());
        histo_->SetTitleOffset(xOffSet, "X");
        histo_->SetTitleOffset(yOffSet, "Y");
        histo_->SetTitleOffset(zOffSet, "Z");
        histo_->Draw("LEGO");

        string command = "mkdir -p multplots/" + folder + "/" +sigbackall;
        gSystem->Exec(command.c_str());

        string saveas = "multplots/" + folder + "/" + sigbackall + "/"  + histo + ".eps";
        gPad->SaveAs(saveas.c_str());

        stat_errors << histo << " : " << histo_->GetBinContent(1,1) << " +/- " << sqrt(histo_->GetBinContent(1,1)) << endl;


        //random
        new TCanvas;
        fileSources[i]->cd();
        TDirectoryFile *folder_;
        fileSources[i]->GetObject(folderRandom.c_str(),folder_);
        TH2 *histoRandom_ = (TH1*)folder_->Get(histo.c_str());
        histoRandom_->SetTitle(title.c_str());
        histoRandom_->SetTitleOffset(xOffSet, "X");
        histoRandom_->SetTitleOffset(yOffSet, "Y");
        histoRandom_->SetTitleOffset(zOffSet, "Z");
        histoRandom_->Draw("LEGO");

        string command = "mkdir -p multplots/" + folder + "/" +sigbackall;
        gSystem->Exec(command.c_str());

        string saveas = "multplots/" + folder + "/" + sigbackall + "/" + histo + "Random.eps";
        gPad->SaveAs(saveas.c_str());

        stat_errors << histo << "Random" << " : " << histoRandom_->GetBinContent(1,1) << " +/- " << sqrt(histoRandom_->GetBinContent(1,1)) << endl;


        //propper - random
        new TCanvas;
//    TH2 histoPropperToRandom(*histo_) ;
        histo_->Clone("histoPropperToRandom");

        histoPropperToRandom->Add(histoRandom_,-1.0);
        histoPropperToRandom->SetTitle(title.c_str());
        histoPropperToRandom->SetTitleOffset(xOffSet, "X");
        histoPropperToRandom->SetTitleOffset(yOffSet, "Y");
        histoPropperToRandom->SetTitleOffset(zOffSet, "Z");
        histoPropperToRandom->Draw("LEGO");

        string command = "mkdir -p multplots/" + folder + "/" +sigbackall;
        gSystem->Exec(command.c_str());

        string saveas = "multplots/" + folder + "/" + sigbackall + "/"  + histo + "ProperToRandom.eps";
        gPad->SaveAs(saveas.c_str());

        stat_errors << histo << "PropperToRandom" << " : " << histoPropperToRandom->GetBinContent(1,1) << " +/- " << sqrt(fabs(histoPropperToRandom->GetBinContent(1,1))) << endl;


        stat_errors.close();
    }

    signal->Close("R");
    background->Close("R");
    all->Close("R");

}


void multPlotProducer ()
{

    //std::vector< std::pair < std::string , std::string > > multPlotsFolders;
    vector <string> multPlotsFolders;
    vector <string> multPlotsFoldersRandom;
    vector <string> multPlotsTitles;
    string data_[3];

//
    data_[0] = "sdDijetsPlotterInfHf00Castor00";
    data_[1] = "sdDijetsPlotterInfRandomHf00Castor00";
//    data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf20Castor50";
//    data_[1] = "sdDijetsPlotterInfRandomHf20Castor50";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf45Castor50";
//    data_[1] = "sdDijetsPlotterInfRandomHf45Castor50";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf20Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf20Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf45Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf45Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//
//
//
//
//
//
//
//
//
//
//    data_[0] = "sdDijetsPlotter5Hf00Castor00";
//    data_[1] = "sdDijetsPlotter5RandomHf00Castor00";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf20Castor50";
//    data_[1] = "sdDijetsPlotter5RandomHf20Castor50";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf45Castor50";
//    data_[1] = "sdDijetsPlotter5RandomHf45Castor50";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf20Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf20Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf45Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf45Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//
//
//
//
//
//
//    data_[0] = "sdDijetsPlotter1Hf00Castor00";
//    data_[1] = "sdDijetsPlotter1RandomHf00Castor00";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf20Castor50";
//    data_[1] = "sdDijetsPlotter1RandomHf20Castor50";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf45Castor50";
//    data_[1] = "sdDijetsPlotter1RandomHf45Castor50";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf20Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf20Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf45Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf45Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//
//
//
//    data_[0] = "sdDijetsPlotterInfHf47Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf47Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf48Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf48Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf49Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf49Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf50Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf50Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf51Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf51Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf52Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf52Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf53Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf53Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf54Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf54Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf55Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf55Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf56Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf56Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotterInfHf57Castor100";
//    data_[1] = "sdDijetsPlotterInfRandomHf57Castor100";
////    data_[2] = "N^{ max}_{ track} = Inf";
//    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//
//    data_[0] = "sdDijetsPlotter5Hf47Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf47Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf48Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf48Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf49Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf49Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf50Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf50Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf51Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf51Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf52Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf52Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf53Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf53Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf54Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf54Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf55Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf55Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf56Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf56Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter5Hf57Castor100";
//    data_[1] = "sdDijetsPlotter5RandomHf57Castor100";
//    data_[2] = "N^{ max}_{ track} = 5";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//
//
//
//
//    data_[0] = "sdDijetsPlotter1Hf47Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf47Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf48Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf48Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf49Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf49Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf50Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf50Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf51Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf51Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf52Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf52Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf53Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf53Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf54Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf54Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf55Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf55Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf56Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf56Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//    data_[0] = "sdDijetsPlotter1Hf57Castor100";
//    data_[1] = "sdDijetsPlotter1RandomHf57Castor100";
//    data_[2] = "N^{ max}_{ track} = 1";
////    data_[2] = "No N^{ max}_{ track} cut";
//    multPlotsFolders.push_back(data_[0]);
//    multPlotsFoldersRandom.push_back(data_[1]);
//    multPlotsTitles.push_back(data_[2]);
//
//
//




    data_[0] = "sdDijetsPlotterInfHf1Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf1Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf2Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf2Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf3Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf3Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf4Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf4Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf5Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf5Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf6Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf6Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf7Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf7Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf8Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf8Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf9Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf9Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf10Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf10Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf11Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf11Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf12Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf12Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf13Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf13Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf14Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf14Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf15Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf15Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf16Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf16Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf17Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf17Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf18Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf18Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf19Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf19Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf20Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf20Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf21Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf21Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf22Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf22Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf23Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf23Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf24Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf24Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf25Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf25Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf26Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf26Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf27Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf27Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf28Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf28Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf29Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf29Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf30Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf30Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf31Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf31Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf32Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf32Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf33Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf33Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf34Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf34Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf35Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf35Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf36Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf36Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf37Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf37Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf38Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf38Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf39Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf39Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf40Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf40Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf41Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf41Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf42Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf42Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf43Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf43Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf44Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf44Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf45Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf45Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf46Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf46Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf47Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf47Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf48Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf48Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf49Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf49Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotterInfHf50Castor100";
    data_[1] = "sdDijetsPlotterInfRandomHf50Castor100";
    //data_[2] = "N^{ max}_{ track} = Inf";
    data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf1Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf1Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf2Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf2Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf3Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf3Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf4Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf4Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf5Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf5Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf6Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf6Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf7Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf7Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf8Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf8Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf9Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf9Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf10Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf10Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf11Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf11Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf12Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf12Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf13Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf13Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf14Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf14Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf15Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf15Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf16Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf16Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf17Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf17Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf18Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf18Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf19Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf19Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf20Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf20Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf21Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf21Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf22Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf22Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf23Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf23Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf24Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf24Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf25Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf25Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf26Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf26Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf27Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf27Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf28Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf28Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf29Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf29Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf30Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf30Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf31Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf31Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf32Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf32Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf33Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf33Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf34Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf34Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf35Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf35Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf36Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf36Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf37Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf37Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf38Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf38Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf39Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf39Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf40Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf40Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf41Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf41Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf42Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf42Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf43Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf43Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf44Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf44Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf45Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf45Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf46Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf46Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf47Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf47Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf48Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf48Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf49Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf49Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter5Hf50Castor100";
    data_[1] = "sdDijetsPlotter5RandomHf50Castor100";
    data_[2] = "N^{ max}_{ track} = 5";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf1Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf1Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf2Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf2Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf3Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf3Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf4Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf4Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf5Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf5Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf6Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf6Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf7Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf7Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf8Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf8Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf9Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf9Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf10Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf10Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf11Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf11Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf12Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf12Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf13Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf13Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf14Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf14Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf15Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf15Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf16Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf16Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf17Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf17Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf18Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf18Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf19Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf19Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf20Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf20Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf21Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf21Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf22Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf22Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf23Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf23Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf24Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf24Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf25Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf25Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf26Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf26Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf27Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf27Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf28Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf28Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf29Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf29Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf30Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf30Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf31Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf31Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf32Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf32Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf33Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf33Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf34Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf34Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf35Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf35Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf36Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf36Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf37Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf37Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf38Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf38Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf39Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf39Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf40Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf40Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf41Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf41Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf42Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf42Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf43Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf43Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf44Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf44Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf45Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf45Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf46Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf46Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf47Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf47Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf48Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf48Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf49Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf49Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    data_[0] = "sdDijetsPlotter1Hf50Castor100";
    data_[1] = "sdDijetsPlotter1RandomHf50Castor100";
    data_[2] = "N^{ max}_{ track} = 1";
    //data_[2] = "No N^{ max}_{ track} cut";
    multPlotsFolders.push_back(data_[0]);
    multPlotsFoldersRandom.push_back(data_[1]);
    multPlotsTitles.push_back(data_[2]);







    if (multPlotsFolders.size() == multPlotsFoldersRandom.size())
    {
        for (int i=0;i < multPlotsFolders.size(); i++)
        {
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HFlow_HFhigh");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HF_CASTOR");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HF_GENCASTOR");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_GENCASTOR_CASTOR");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HFlow_HFhigh_Opp");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HF_CASTOR_Opp");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_HF_GENCASTOR_Opp");
            multplots (multPlotsFolders.at(i), multPlotsFoldersRandom.at(i), multPlotsTitles.at(i), "h_Mult_GENCASTOR_CASTOR_Opp");
        }
    }
    else
    {
        cout << "Propper size != Random size!!!" << endl;
    }

}


void plotProducer ()
{
    gSystem->Exec("rm -r multplots");
    gSystem->Exec("rm -r kinplots");
    kinPlotProducer ();
    multPlotProducer ();
}


