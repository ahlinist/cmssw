{
TFile f("HcalZS.root");

f.ls();
f.cd("hcalZS/Hcal");

TCanvas c1;
c1.Divide(2,3);

c1.cd(1);
c1_1.SetGridx();
c1_1.SetGridy();
c1_1.SetLogz();
h_hcal_hbhfd1->Draw("colz");
h_hcal_hbhfd1->SetStats(0);

c1.cd(2);
c1_2.SetGridx();
c1_2.SetGridy();
c1_2.SetLogz();
h_hcal_hbhfd2->Draw("colz");
h_hcal_hbhfd2->SetStats(0);

c1.cd(3);
c1_3.SetGridx();
c1_3.SetGridy();
c1_3.SetLogz();
h_hcal_hed1->Draw("colz");
h_hcal_hed1->SetStats(0);

c1.cd(4);
c1_4.SetGridx();
c1_4.SetGridy();
c1_4.SetLogz();
h_hcal_hed2->Draw("colz");
h_hcal_hed2->SetStats(0);

c1.cd(5);
c1_5.SetGridx();
c1_5.SetGridy();
c1_5.SetLogz();
h_hcal_hed3->Draw("colz");
h_hcal_hed3->SetStats(0);

c1.cd(6);
c1_6.SetGridx();
c1_6.SetGridy();
c1_6.SetLogz();
h_hcal_hod4->Draw("colz");
h_hcal_hod4->SetStats(0);


}
