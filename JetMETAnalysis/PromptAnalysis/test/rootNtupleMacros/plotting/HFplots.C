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
h2_ET_vs_HFratio->SetTitle("");
h2_ET_vs_HFratio->GetXaxis()->SetTitle("R = (E_{L}-E_{S})/(E_{L}+E_{S})");
h2_ET_vs_HFratio->GetYaxis()->SetTitle("E_{T} of HF tower [GeV]");
h2_ET_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,1.1);
h2_ET_vs_HFratio->GetYaxis()->SetRangeUser(0,50);
// h2_ET_vs_HFratio->SetMarkerStyle(20);
// h2_ET_vs_HFratio->SetMarkerSize(0.6);
// h2_ET_vs_HFratio->SetMarkerColor(kBlack);
h2_ET_vs_HFratio->Draw("colz");

TLatex l_900;
l_900.SetTextAlign(12);
l_900.SetTextSize(0.04);
l_900.SetTextFont(62);
l_900.SetNDC();
l_900.DrawLatex(0.321839,0.927966,"CMS 2009 Preliminary - 900 GeV MinBias DATA");

c_Data_tcMET.SaveAs("hf_towerET_vs_ratio_DATA.eps");


TCanvas c_MC_tcMET;
c_MC_tcMET.SetGridy();
c_MC_tcMET.SetGridx();
c_MC_tcMET.SetLogz();
MC.cd();
h2_ET_vs_HFratio->SetTitle("");
h2_ET_vs_HFratio->GetXaxis()->SetTitle("R = (E_{L}-E_{S})/(E_{L}+E_{S})");
h2_ET_vs_HFratio->GetYaxis()->SetTitle("E_{T} of HF tower [GeV]");
h2_ET_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,1.1);
h2_ET_vs_HFratio->GetYaxis()->SetRangeUser(0,50);
// h2_ET_vs_HFratio->SetMarkerStyle(20);
// h2_ET_vs_HFratio->SetMarkerSize(0.6);
// h2_ET_vs_HFratio->SetMarkerColor(kRed);
h2_ET_vs_HFratio->Draw("colz");

l_900.DrawLatex(0.350575,0.927966,"CMS 2009 Preliminary - 900 GeV MinBias MC");

c_MC_tcMET.SaveAs("hf_towerET_vs_ratio_MC.eps");


/*

// TCanvas all;
// all.Divide(2,2);

Data.cd();

// all.cd(1);
// all.SetGridy();
TCanvas c_Data_Long;
c_Data_Long.SetGridy();
h2_ETL_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - Data");
h2_ETL_vs_HFratio->GetYaxis()->SetTitle("ET Long Fiber (GeV) - Data");
h2_ETL_vs_HFratio->GetXaxis()->SetRangeUser(0.5,1.1);
h2_ETL_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETL_vs_HFratio->Draw("colz");
c_Data_Long.SaveAs("hf_longfiberET_vs_ratio_DATA.eps");

// all.cd(2);
// all.SetGridy();
TCanvas c_Data_Short;
c_Data_Short.SetGridy();
h2_ETS_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - Data");
h2_ETS_vs_HFratio->GetYaxis()->SetTitle("ET Short Fiber (GeV) - Data");
h2_ETS_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,-0.5);
h2_ETS_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETS_vs_HFratio->Draw("colz");
c_Data_Short.SaveAs("hf_shortfiberET_vs_ratio_DATA.eps");

MC.cd();

// all.cd(3);
// all.SetGridy();
TCanvas c_MC_Long;
c_MC_Long.SetGridy();
h2_ETL_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - MC");
h2_ETL_vs_HFratio->GetYaxis()->SetTitle("ET Long Fiber (GeV) - MC");
h2_ETL_vs_HFratio->GetXaxis()->SetRangeUser(0.5,1.1);
h2_ETL_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETL_vs_HFratio->Draw("colz");
c_MC_Long.SaveAs("hf_longfiberET_vs_ratio_MC.eps");

// all.cd(4);
// all.SetGridy();
TCanvas c_MC_Short;
c_MC_Short.SetGridy();
h2_ETS_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) -  MC");
h2_ETS_vs_HFratio->GetYaxis()->SetTitle("ET Short Fiber (GeV) - MC");
h2_ETS_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,-0.5);
h2_ETS_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETS_vs_HFratio->Draw("colz");
c_MC_Short.SaveAs("hf_shortfiberET_vs_ratio_MC.eps");

*/

}
