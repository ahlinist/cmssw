void plot_deltapt_MC(char* inputfile){
  
  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  gStyle->SetOptStat(1110);
  gStyle->SetStatY(.9);
  gStyle->SetStatX(.9);
  
  TFile resojet(inputfile);
  
  deltaptjetquarknorm->SetTitle("");
  deltaptjetquarknorm->SetXTitle("#Delta(p_{T})(#gamma-quark)/p_{T}");
  deltaptjetquarknorm->SetYTitle("events/bin");
  deltaptjetquarknorm->SetTitleOffset(1.3,"Y");
  deltaptjetquarknorm->SetStats(1111);
  deltaptjetquarknorm->Draw();
  c0->SaveAs("deltapt_MC.eps");
  c0->SaveAs("deltapt_MC.gif");

  deltaptjetquarknorm_etaptcut->SetTitle("");
  deltaptjetquarknorm_etaptcut->SetXTitle("#Delta(p_{T})(#gamma-quark)/p_{T}");
  deltaptjetquarknorm_etaptcut->SetYTitle("events/bin");
  deltaptjetquarknorm_etaptcut->SetTitleOffset(1.3,"Y");
  deltaptjetquarknorm_etaptcut->SetStats(1111);
  deltaptjetquarknorm_etaptcut->Draw();
  c0->SaveAs("deltapt_MC_etaptcut.eps");
  c0->SaveAs("deltapt_MC_etaptcut.gif");

  deltaptjetquarknorm_deltaphi->SetTitle("");
  deltaptjetquarknorm_deltaphi->SetXTitle("#Delta(p_{T})(#gamma-quark)/p_{T}");
  deltaptjetquarknorm_deltaphi->SetYTitle("events/bin");
  deltaptjetquarknorm_deltaphi->SetTitleOffset(1.3,"Y");
  deltaptjetquarknorm_deltaphi->SetStats(1111);
  deltaptjetquarknorm_deltaphi->Draw();
  c0->SaveAs("deltapt_MC_deltaphi.eps");
  c0->SaveAs("deltapt_MC_deltaphi.gif");

  resojetMC->SetTitle("");
  resojetMC->SetXTitle("#Delta(p_{T})(jet-quark)/p_{T}");
  resojetMC->SetYTitle("events/bin");
  resojetMC->SetTitleOffset(1.3,"Y");
  resojetMC->SetStats(1111);
  resojetMC->Draw();
  c0->SaveAs("deltapt_quark-jet.eps");
  c0->SaveAs("deltapt_quark-jet.gif");
  

}
