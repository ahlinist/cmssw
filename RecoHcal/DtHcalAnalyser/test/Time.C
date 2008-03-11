// Draw HB timing distributions for different types of trigger.
// (takes data from DtHcalAnalyser output file)

void Time(char *FileName="none"){
   TFile* f = new TFile(FileName,"READ"); 
   if(!f->IsOpen()){
      printf("Usage: .x Time.C(\"filename\")\n");
      printf("Can't open file \"%s\"\n",FileName);
      return;
   } 
   gStyle->SetOptStat(0); 
   TCanvas *can=new TCanvas("HB Timing plots","HB Timing plots");
   can->Divide(1,5,0.0001,0.0001);
   
   TH1F*  top_hb = (TH1F*)f->Get("HCAL Plots/HB Top Timing");
   TH1F*  bot_hb = (TH1F*)f->Get("HCAL Plots/HB Bottom Timing");
   top_hb->SetLineColor(kRed);
   top_hb->SetLineWidth(2);
   bot_hb->SetLineColor(kBlue);
   bot_hb->SetLineWidth(2);
   can->cd(1);
   top_hb->SetTitle("HB Timing, blue - bottom,red - top");
   top_hb->Draw();
   bot_hb->Draw("Same");
   
   TH1F*  top_dt = (TH1F*)f->Get("HCAL Plots/HB Top Timing (DT trigger)");
   TH1F*  bot_dt = (TH1F*)f->Get("HCAL Plots/HB Bottom Timing (DT trigger)");
   top_dt->SetLineColor(kRed);
   top_dt->SetLineWidth(2);
   bot_dt->SetLineColor(kBlue);
   bot_dt->SetLineWidth(2);
   can->cd(2);
   top_dt->SetTitle("HB Timing (DT trigger), blue - bottom,red - top");
   top_dt->Draw();
   bot_dt->Draw("Same");

   TH1F*  top_rpc = (TH1F*)f->Get("HCAL Plots/HB Top Timing (RPC trigger)");
   TH1F*  bot_rpc = (TH1F*)f->Get("HCAL Plots/HB Bottom Timing (RPC trigger)");
   top_rpc->SetLineColor(kRed);
   top_rpc->SetLineWidth(2);
   bot_rpc->SetLineColor(kBlue);
   bot_rpc->SetLineWidth(2);
   can->cd(3);
   top_rpc->SetTitle("HB Timing (RPC trigger), blue - bottom,red - top");
   top_rpc->Draw();
   bot_rpc->Draw("Same");

   TH1F*  top_rpcdt = (TH1F*)f->Get("HCAL Plots/HB Top Timing (RPC+DT trigger)");
   TH1F*  bot_rpcdt = (TH1F*)f->Get("HCAL Plots/HB Bottom Timing (RPC+DT trigger)");
   top_rpcdt->SetLineColor(kRed);
   top_rpcdt->SetLineWidth(2);
   bot_rpcdt->SetLineColor(kBlue);
   bot_rpcdt->SetLineWidth(2);
   can->cd(4);
   top_rpcdt->SetTitle("HB Timing (RPC+DT trigger), blue - bottom,red - top");
   top_rpcdt->Draw();
   bot_rpcdt->Draw("Same");

   TH1F*  top_hbmip = (TH1F*)f->Get("HCAL Plots/HB Top Timing (HB trigger)");
   TH1F*  bot_hbmip = (TH1F*)f->Get("HCAL Plots/HB Bottom Timing (HB trigger)");
   top_hbmip->SetLineColor(kRed);
   top_hbmip->SetLineWidth(2);
   bot_hbmip->SetLineColor(kBlue);
   bot_hbmip->SetLineWidth(2);
   can->cd(5);
   top_hbmip->SetTitle("HB Timing (HBmip trigger), blue - bottom,red - top");
   top_hbmip->Draw();
   bot_hbmip->Draw("Same");
   
   can->cd();
}