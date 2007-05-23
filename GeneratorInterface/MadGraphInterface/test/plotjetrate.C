{
TFile f("eventtree.root");
gStyle->SetOptStat(0);
TCanvas *myCanvas = new TCanvas("zc","zC",500,0,900,700);
myCanvas->SetLogy(1);
myCanvas->SetLogx(1);
TLegend *legend1 =  new TLegend(0.60,0.65,0.85,0.82);
hq2_all->SetTitle("Diff. 1->2 jet rates for Zmumu + jets at LHC, MadGraph + Pythia");
hq2_all->SetXTitle("Log(Qpar2)");
hq2_all->SetYTitle("Cross Section (arbitrary units)");
hq2_all->SetLineColor(5); hq2_all->SetLineWidth(3); hq2_all->SetLineStyle(0); hq2_all->SetMarkerStyle(20); hq2_all->SetMarkerColor(3); legend1->AddEntry(hq2_all,"Zmumu matched","l");
hq2_0->SetLineColor(2);   hq2_0->SetLineWidth(2);   hq2_0->SetLineStyle(2);   legend1->AddEntry(hq2_0,"Zmumu + 0 jets","l");
hq2_1->SetLineColor(3);   hq2_1->SetLineWidth(2);   hq2_1->SetLineStyle(3);   legend1->AddEntry(hq2_1,"Zmumu + 1 jets","l");
hq2_2->SetLineColor(1);   hq2_2->SetLineWidth(2);   hq2_2->SetLineStyle(4);   legend1->AddEntry(hq2_2,"Zmumu + 2 jets","l");
hq2_all->Draw("");
hq2_0->Draw("same");
hq2_1->Draw("same");
hq2_2->Draw("same");
legend1->Draw();
zc->Draw();
zc->Print("jetrate12.gif");
}
