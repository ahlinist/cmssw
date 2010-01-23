{

gROOT->Reset();

TFile DATA("output_DATA900.root");
TFile MC("output_MC900.root");
//cut functions from http://indico.cern.ch/getFile.py/access?contribId=5&resId=0&materialId=slides&confId=81128

float E_cut_EM = 80.;
TF1 f_Cut_HF_EM("Cut_HF_EM","0.11*log10(1/55 * 1/x)",0,0.02);
TLine l_Cut_HF_EM( 1 / E_cut_EM , 0 , 1 / E_cut_EM , f_Cut_HF_EM.Eval( 1/E_cut_EM ) );

float E_cut_HAD = 120.;
TF1 f_Cut_HF_HAD("f_Cut_HF_HAD","0.045*log10(1/60 * 1/x)",0,0.02);
TLine l_Cut_HF_HAD( 1 / E_cut_HAD , 0 , 1 / E_cut_HAD , f_Cut_HF_HAD.Eval( 1/E_cut_HAD ) );

DATA.cd();

//*************
TCanvas c_EM_DATA;
c_EM_DATA.SetGridx();
c_EM_DATA.SetGridy();


h2_S9S1_vs_1overE_em->SetTitle("f_{Neighbours} vs 1/E_{seed} (HF EM) - DATA");
h2_S9S1_vs_1overE_em->GetXaxis()->SetRangeUser(0,0.02);
h2_S9S1_vs_1overE_em->GetYaxis()->SetRangeUser(0,0.2);
h2_S9S1_vs_1overE_em->GetXaxis()->SetTitle("1/E_{EM}");
h2_S9S1_vs_1overE_em->GetYaxis()->SetTitle("S_{9}/S_{1}");
h2_S9S1_vs_1overE_em->SetMarkerStyle(20);
h2_S9S1_vs_1overE_em->SetMarkerSize(0.5);
h2_S9S1_vs_1overE_em->SetMarkerColor(kBlue);
h2_S9S1_vs_1overE_em->Draw();
f_Cut_HF_EM->SetLineColor(kRed);
f_Cut_HF_EM->SetLineWidth(3);
f_Cut_HF_EM->SetLineStyle(2);
f_Cut_HF_EM->Draw("same");
l_Cut_HF_EM->SetLineColor(kRed);
l_Cut_HF_EM->SetLineWidth(3);
l_Cut_HF_EM->SetLineStyle(2);
l_Cut_HF_EM->Draw("same");

c_EM_DATA.SaveAs("S9S1_vs_1overE_em_DATA.eps");

//*************
TCanvas c_HAD_DATA;
c_HAD_DATA.SetGridx();
c_HAD_DATA.SetGridy();

h2_S9S1_vs_1overE_had->SetTitle("f_{Neighbours} vs 1/E_{seed} (HF HAD) - DATA");
h2_S9S1_vs_1overE_had->GetXaxis()->SetRangeUser(0,0.02);
h2_S9S1_vs_1overE_had->GetYaxis()->SetRangeUser(0,0.2);
h2_S9S1_vs_1overE_had->GetXaxis()->SetTitle("1/E_{HAD}");
h2_S9S1_vs_1overE_had->GetYaxis()->SetTitle("S_{9}/S_{1}");
h2_S9S1_vs_1overE_had->SetMarkerStyle(20);
h2_S9S1_vs_1overE_had->SetMarkerSize(0.5);
h2_S9S1_vs_1overE_had->SetMarkerColor(kBlue);
h2_S9S1_vs_1overE_had->Draw();
f_Cut_HF_HAD->SetLineColor(kRed);
f_Cut_HF_HAD->SetLineWidth(3);
f_Cut_HF_HAD->SetLineStyle(2);
f_Cut_HF_HAD->Draw("same");
l_Cut_HF_HAD->SetLineColor(kRed);
l_Cut_HF_HAD->SetLineWidth(3);
l_Cut_HF_HAD->SetLineStyle(2);
l_Cut_HF_HAD->Draw("same");

c_HAD_DATA.SaveAs("S9S1_vs_1overE_had_DATA.eps");


MC.cd();

//*************
TCanvas c_EM_MC;
c_EM_MC.SetGridx();
c_EM_MC.SetGridy();


h2_S9S1_vs_1overE_em->SetTitle("f_{Neighbours} vs 1/E_{seed} (HF EM) - MC");
h2_S9S1_vs_1overE_em->GetXaxis()->SetRangeUser(0,0.02);
h2_S9S1_vs_1overE_em->GetYaxis()->SetRangeUser(0,0.2);
h2_S9S1_vs_1overE_em->GetXaxis()->SetTitle("1/E_{EM}");
h2_S9S1_vs_1overE_em->GetYaxis()->SetTitle("S_{9}/S_{1}");
h2_S9S1_vs_1overE_em->SetMarkerStyle(20);
h2_S9S1_vs_1overE_em->SetMarkerSize(0.5);
h2_S9S1_vs_1overE_em->SetMarkerColor(kBlue);
h2_S9S1_vs_1overE_em->Draw();
f_Cut_HF_EM->SetLineColor(kRed);
f_Cut_HF_EM->SetLineWidth(3);
f_Cut_HF_EM->SetLineStyle(2);
f_Cut_HF_EM->Draw("same");
l_Cut_HF_EM->SetLineColor(kRed);
l_Cut_HF_EM->SetLineWidth(3);
l_Cut_HF_EM->SetLineStyle(2);
l_Cut_HF_EM->Draw("same");

c_EM_MC.SaveAs("S9S1_vs_1overE_em_MC.eps");

//*************
TCanvas c_HAD_MC;
c_HAD_MC.SetGridx();
c_HAD_MC.SetGridy();

h2_S9S1_vs_1overE_had->SetTitle("f_{Neighbours} vs 1/E_{seed} (HF HAD) - MC");
h2_S9S1_vs_1overE_had->GetXaxis()->SetRangeUser(0,0.02);
h2_S9S1_vs_1overE_had->GetYaxis()->SetRangeUser(0,0.2);
h2_S9S1_vs_1overE_had->GetXaxis()->SetTitle("1/E_{HAD}");
h2_S9S1_vs_1overE_had->GetYaxis()->SetTitle("S_{9}/S_{1}");
h2_S9S1_vs_1overE_had->SetMarkerStyle(20);
h2_S9S1_vs_1overE_had->SetMarkerSize(0.5);
h2_S9S1_vs_1overE_had->SetMarkerColor(kBlue);
h2_S9S1_vs_1overE_had->Draw();
f_Cut_HF_HAD->SetLineColor(kRed);
f_Cut_HF_HAD->SetLineWidth(3);
f_Cut_HF_HAD->SetLineStyle(2);
f_Cut_HF_HAD->Draw("same");
l_Cut_HF_HAD->SetLineColor(kRed);
l_Cut_HF_HAD->SetLineWidth(3);
l_Cut_HF_HAD->SetLineStyle(2);
l_Cut_HF_HAD->Draw("same");

c_HAD_MC.SaveAs("S9S1_vs_1overE_had_MC.eps");


}
