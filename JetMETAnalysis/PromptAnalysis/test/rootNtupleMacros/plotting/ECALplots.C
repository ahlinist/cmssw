{

gROOT->Reset();
gStyle->SetOptStat("0");

TFile Data("output_DATA900GeV.root");
TFile MC("output_MC900GeV.root");


TCanvas c_Data_tcMET;
c_Data_tcMET.SetGridy();
c_Data_tcMET.SetGridx();
c_Data_tcMET.SetLogz();
Data.cd();
h2_ECalSeedET_Vs_S4_tcMEToverS1->SetTitle("");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetXaxis()->SetTitle("S4/S1");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetYaxis()->SetTitle("E_{T} of seed crystal [GeV]");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetXaxis()->SetRangeUser(0,1.);
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetYaxis()->SetRangeUser(0,50);
// h2_ET_vs_HFratio->SetMarkerStyle(20);
// h2_ET_vs_HFratio->SetMarkerSize(0.6);
// h2_ET_vs_HFratio->SetMarkerColor(kBlack);
h2_ECalSeedET_Vs_S4_tcMEToverS1->Draw("colz");

TLatex l_900;
l_900.SetTextAlign(12);
l_900.SetTextSize(0.04);
l_900.SetTextFont(62);
l_900.SetNDC();
l_900.DrawLatex(0.492816,0.927966,"CMS Preliminary - 900 GeV DATA");

c_Data_tcMET.SaveAs("ecal_seedET_vs_S4oS1_DATA.eps");



TCanvas c_MC_tcMET;
c_MC_tcMET.SetGridy();
c_MC_tcMET.SetGridx();
c_MC_tcMET.SetLogz();
MC.cd();
h2_ECalSeedET_Vs_S4_tcMEToverS1->SetTitle("");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetXaxis()->SetTitle("S4/S1");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetYaxis()->SetTitle("E_{T} of seed crystal [GeV]");
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetXaxis()->SetRangeUser(0,1.);
h2_ECalSeedET_Vs_S4_tcMEToverS1->GetYaxis()->SetRangeUser(0,50);
// h2_ET_vs_HFratio->SetMarkerStyle(20);
// h2_ET_vs_HFratio->SetMarkerSize(0.6);
// h2_ET_vs_HFratio->SetMarkerColor(kBlack);
h2_ECalSeedET_Vs_S4_tcMEToverS1->Draw("colz");

TLatex l_900;
l_900.SetTextAlign(12);
l_900.SetTextSize(0.04);
l_900.SetTextFont(62);
l_900.SetNDC();
l_900.DrawLatex(0.41523,0.930085,"CMS Preliminary - 900 GeV MinBias MC");

c_MC_tcMET.SaveAs("ecal_seedET_vs_S4oS1_MC.eps");


}
