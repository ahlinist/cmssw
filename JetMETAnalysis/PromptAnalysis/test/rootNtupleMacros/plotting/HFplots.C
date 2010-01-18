{

TFile Data("output_DATA900GeV.root");
TFile MC("output_MC900GeV.root");

TCanvas all;
all.Divide(2,2);

Data.cd();

all.cd(1);
all.SetGridy();
// TCanvas c_Data_Long;
// c_Data_Long.SetGridy();
h2_ETL_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - Data");
h2_ETL_vs_HFratio->GetYaxis()->SetTitle("ET Long Fiber (GeV) - Data");
h2_ETL_vs_HFratio->GetXaxis()->SetRangeUser(0.8,1.1);
h2_ETL_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETL_vs_HFratio->Draw("colz");

all.cd(2);
all.SetGridy();
// TCanvas c_Data_Short;
// c_Data_Short.SetGridy();
h2_ETS_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - Data");
h2_ETS_vs_HFratio->GetYaxis()->SetTitle("ES Short Fiber(GeV) - Data");
h2_ETS_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,-0.8);
h2_ETS_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETS_vs_HFratio->Draw("colz");


MC.cd();

all.cd(3);
all.SetGridy();
// TCanvas c_MC_Long;
// c_MC_Long.SetGridy();
h2_ETL_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) - MC");
h2_ETL_vs_HFratio->GetYaxis()->SetTitle("ET Long Fiber (GeV) - MC");
h2_ETL_vs_HFratio->GetXaxis()->SetRangeUser(0.8,1.1);
h2_ETL_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETL_vs_HFratio->Draw("colz");

all.cd(4);
all.SetGridy();
// TCanvas c_MC_Short;
// c_MC_Short.SetGridy();
h2_ETS_vs_HFratio->GetXaxis()->SetTitle("HF ratio (L-S/L+S) -  MC");
h2_ETS_vs_HFratio->GetYaxis()->SetTitle("ES Short Fiber(GeV) - MC");
h2_ETS_vs_HFratio->GetXaxis()->SetRangeUser(-1.1,-0.8);
h2_ETS_vs_HFratio->GetYaxis()->SetRangeUser(0,20);
h2_ETS_vs_HFratio->Draw("colz");


}
