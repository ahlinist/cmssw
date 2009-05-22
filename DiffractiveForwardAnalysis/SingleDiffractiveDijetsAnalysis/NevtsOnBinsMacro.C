
{
    TFile *pomwig = new TFile("PomWigDiJets_NevtsOnBins.root");
    TFile *pythia = new TFile("QCD_NevtsOnBins.root");


    cout << "" << endl;
    cout << "############ POMWIG ############" << endl;

    pomwig->cd();
    NetvOnBins->cd();

    cout << "30 --> 80: " << (int)h_NevtsOnBins->GetBinContent(2) << endl;
    cout << "80 --> Inf: " << (int)(h_NevtsOnBins->GetBinContent(3) + h_NevtsOnBins->GetBinContent(4) + h_NevtsOnBins->GetBinContent(5) + h_NevtsOnBins->GetBinContent(6) + h_NevtsOnBins->GetBinContent(7) + h_NevtsOnBins->GetBinContent(8) + h_NevtsOnBins->GetBinContent(9)) << endl;

    cout << "" << endl;
    cout << "############ PPYTHIA ############" << endl;

    pythia->cd();
    NetvOnBins->cd();

    cout << "15 --> 30: " << (int)h_NevtsOnBins->GetBinContent(1) << endl;
    cout << "30 --> 80: " << (int)h_NevtsOnBins->GetBinContent(2) << endl;
    cout << "80 --> 170: " << (int)h_NevtsOnBins->GetBinContent(3) << endl;
    cout << "170 --> 300: " << (int)h_NevtsOnBins->GetBinContent(4) << endl;
    cout << "300 --> 470: " << (int)h_NevtsOnBins->GetBinContent(5) << endl;
    cout << "470 --> 800: " << (int)h_NevtsOnBins->GetBinContent(6) << endl;
    cout << "800 --> 1400: " << (int)h_NevtsOnBins->GetBinContent(7) << endl;
    cout << "1400 --> 2200: " << (int)h_NevtsOnBins->GetBinContent(8) << endl;
    cout << "2200 --> Inf: " << (int)h_NevtsOnBins->GetBinContent(9) << endl;

    cout << "" << endl;
}
