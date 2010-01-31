{

gROOT->Reset();

gStyle->SetOptStat(0);


TFile DATA900GeV("output_DATA900GeV.root");
//TFile DATA2360GeV("output_DATA2360GeV.root");
//TFile MC900GeV("output_MC900GeV.root");
//TFile MC2360GeV("output_MC2360GeV.root");


TCanvas c_calomet_900GeV;

c_calomet_900GeV.SetLogy();
c_calomet_900GeV.SetGridx();
c_calomet_900GeV.SetGridy();

DATA900GeV.cd();
h_calometPt_baseSel->SetTitle("");

h_calometPt_baseSel->SetLineColor(kBlue);
h_calometPt_baseSel_HFPMTHitVeto_tcMET->SetLineColor(kRed);
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->SetLineColor(kGreen);

h_calometPt_baseSel->SetMarkerColor(kBlue);
h_calometPt_baseSel_HFPMTHitVeto_tcMET->SetMarkerColor(kRed);
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->SetMarkerColor(kGreen);

h_calometPt_baseSel->SetFillColor(kBlue);
h_calometPt_baseSel_HFPMTHitVeto_tcMET->SetFillColor(kRed);
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->SetFillColor(kGreen);


h_calometPt_baseSel->SetMarkerStyle(20);
h_calometPt_baseSel_HFPMTHitVeto_tcMET->SetMarkerStyle(20);
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->SetMarkerStyle(20);

h_calometPt_baseSel->SetMarkerSize(0.8);
h_calometPt_baseSel_HFPMTHitVeto_tcMET->SetMarkerSize(0.8);
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->SetMarkerSize(0.8);

h_calometPt_baseSel->GetXaxis()->SetTitle("CaloMET [GeV]");

h_calometPt_baseSel->Draw("hist");
h_calometPt_baseSel_HFPMTHitVeto_tcMET->Draw("histsames");
h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET->Draw("histsames");

gPad->RedrawAxis();

TLegend *legend_900 = new TLegend(0.420977,0.447034,0.864943,0.862288);
legend_900->AddEntry(h_calometPt_baseSel,"MinBias trigger + tracker selection","f");
legend_900->AddEntry(h_calometPt_baseSel_HFPMTHitVeto_tcMET,"After HF noise filter","f");
legend_900->AddEntry(h_calometPt_baseSel_HFPMTHitVeto_ECALSpikesVeto_tcMET,"After HF + ECAL noise filter","f");
//Draw legend
legend_900->Draw();


TLatex l_900;
l_900.SetTextAlign(12);
l_900.SetTextSize(0.04);
l_900.SetTextFont(62);
l_900.SetNDC();
l_900.DrawLatex(0.446839,0.347458,"CMS 2009 Preliminary - 900 GeV");


}
