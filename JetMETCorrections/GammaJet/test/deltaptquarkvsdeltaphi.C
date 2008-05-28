void deltaptquarkvsdeltaphi(){
  

  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,900);
  gStyle->SetOptStat(0);

  TFile deltapt("deltaptjetquarknormvsdeltaphi.root");
  deltapt.cd();
  resosum->SetMaximum(35);
  resosum->SetMarkerSize(.7);
  resosum->SetTitle("");
  resosum->SetXTitle("#Delta(#phi) #gamma-jet");
  resosum->SetYTitle("witdh(#Delta(p_{T}))%");
  resosum->SetTitleOffset(1.,"Y");
  resosum->Draw();
  TArrow *arr = new TArrow();
  arr -> SetArrowSize(0.03);
  arr -> DrawArrow(0.2,33,0.2,22);  
  
  c0->SaveAs("reso_deltaptjetquarknormvsdeltaphi.eps");
  c0->SaveAs("reso_deltaptjetquarknormvsdeltaphi.gif");

  deltapt.cd();
  biassum->SetMaximum(20);
  biassum->SetMinimum(-20);
  biassum->SetMarkerSize(.7);
  biassum->SetTitle("");
  biassum->SetXTitle("#Delta(#phi) #gamma-jet");
  biassum->SetYTitle("peak position(#Delta(p_{T}))%");
  biassum->SetTitleOffset(1.,"Y");
  biassum->Draw();
  arr -> DrawArrow(0.2,17,0.2,5);  

  c0->SaveAs("bias_deltaptjetquarknormvsdeltaphi.eps");
  c0->SaveAs("bias_deltaptjetquarknormvsdeltaphi.gif");
  
}
