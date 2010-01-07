void plotHitResStrip(char* name){
  setTDRStyle();
  gROOT->SetStyle("tdrStyle");

  TFile* f = TFile::Open(TString("overlaps_")+name+TString(".root"));
  //TFile* f = TFile::Open("output_test.root");


  int n_tot = layer->GetEntries();
  int n_TIB12 = 0;
  int n_TIB34 = 0;
  int n_TOB1234 = 0;
  int n_TOB56 = 0;

  float alignment_cut = 0.0008;
  float momentum_cut = 15.0;
  float path_median = 4.0;

  for (int i = 1; i<=n_tot; i++){
    //cout << "==========================" << endl;
    float alignX = ddXslope->GetBinContent(i)*x->GetBinError(i);
    float alignY = ddYslope->GetBinContent(i)*y->GetBinError(i);

    float alignDX = ddDxslope->GetBinContent(i)*dxdz->GetBinError(i);
    float alignDY = ddDxslope->GetBinContent(i)*dydz->GetBinError(i);

    //cout << " values of alignX and Y = " << alignX << " and " << alignY << endl;
    //cout << " values of alignDX and Y = " << alignDX << " and " << alignDY << endl;

    if ( fabs(alignX)<alignment_cut && fabs(alignY)<alignment_cut && fabs(alignDX)<alignment_cut && fabs(alignDY)<alignment_cut ) {
      
      if (layer->GetBinContent(i)==1||layer->GetBinContent(i)==2) n_TIB12++;
      if (layer->GetBinContent(i)==3||layer->GetBinContent(i)==4) n_TIB34++;
      if (layer->GetBinContent(i)==5||layer->GetBinContent(i)==6||
	  layer->GetBinContent(i)==7||layer->GetBinContent(i)==8) n_TOB1234++;
      if (layer->GetBinContent(i)==9||layer->GetBinContent(i)==10) n_TOB56++;
    } 
    //else cout << "cutting from misalignment: " << alignX << " or " << alignY << " or " << alignDX << " or " << alignDY << endl;
  }  
  
    cout << "number of entries: TIB12 = " << n_TIB12 << " TIB34 = " << n_TIB34 << 
      " TOB1234 = " << n_TOB1234 << " TOB56 = " << n_TOB56 << endl;

  float widthm, widthe;
  float predm, prede;
  float hitm, hite;
  float hitmdata, hitedata;
  float ratio, ratioe;
  float slopeX, slopeY;
  float rangeX, rangeY;


  TH1F* hitresTIB12 = new TH1F("hitresTIB12", "hit resolution TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* hitsigmaTIB12 = new TH1F("hitsigTIB12", "hit position uncertainty", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* ratio_hTIB12 = new TH1F("ratioTIB12", "ratio TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* ddTIB12 = new TH1F("ddTIB12", "double difference width TIB12", n_TIB12, -0.5, n_TIB12-0.5);
  TH1F* predTIB12 = new TH1F("predTIB12", "mean pred uncertainty TIB12", n_TIB12, -0.5, n_TIB12-0.5);

  TH1F* hitresTIB34 = new TH1F("hitresTIB34", "hit resolution TIB34", n_TIB34, -0.5, n_TIB34-0.5);
  TH1F* hitsigmaTIB34 = new TH1F("hitsigTIB34", "hit position uncertainty", n_TIB34, -0.5, n_TIB34-0.5);
  TH1F* ratio_hTIB34 = new TH1F("ratioTIB34", "ratio TIB34", n_TIB34, -0.5, n_TIB34-0.5);

  TH1F* hitresTOB1234 = new TH1F("hitresTOB1234", "hit resolution TOB1234", n_TOB1234, -0.5, n_TOB1234-0.5);
  TH1F* hitsigmaTOB1234 = new TH1F("hitsigTOB1234", "hit position uncertainty", n_TOB1234, -0.5, n_TOB1234-0.5);
  TH1F* ratio_hTOB1234 = new TH1F("ratioTOB1234", "ratio TOB1234", n_TOB1234, -0.5, n_TOB1234-0.5);

  TH1F* hitresTOB56 = new TH1F("hitresTOB56", "hit resolution TOB56", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* hitsigmaTOB56 = new TH1F("hitsigTOB56", "hit position uncertainty", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* ratio_hTOB56 = new TH1F("ratioTOB56", "ratio TOB56", n_TOB56, -0.5, n_TOB56-0.5);
  TH1F* hitres1DTIB12 = new TH1F("hitres1DTIB12", "hit resolution TIB12", 50, 0, 100);
  TH1F* hitres1DTIB34 = new TH1F("hitres1DTIB34", "hit resolution TIB34", 50, 0, 100);
  TH1F* hitres1DTOB1234 = new TH1F("hitres1DTOB1234", "hit resolution TOB1234", 50, 0, 100);
  TH1F* hitres1DTOB56 = new TH1F("hitres1DTOB56", "hit resolution TOB56", 100, 0, 100);
  TH1F* pred1DTIB12 = new TH1F("pred1DTIB12", "predicted CPE  TIB12", 100, 0, 100);
  TH1F* pred1DTIB34 = new TH1F("pred1DTIB34", "predicted CPE  TIB34", 100, 0, 100);
  TH1F* pred1DTOB1234 = new TH1F("pred1DTOB1234", "predicted CPE  TOB1234", 100, 0, 100);
  TH1F* pred1DTOB56 = new TH1F("pred1DTOB56", "predicted CPE  TOB56", 100, 0, 100);

  int iTIB12 = 1;
  int iTIB34 = 1;
  int iTOB1234 = 1;
  int iTOB56 = 1;
  for (int i = 1; i<=n_tot; i++){
    widthm = (width->GetBinContent(i));
    widthe = (width->GetBinError(i));
    predm = sigMean->GetBinContent(i);
    prede = sigMean->GetBinError(i);
    hitm = hitSigX->GetBinContent(i);
    hite = hitSigX->GetBinError(i);
    slopeX = ddXslope->GetBinContent(i);
    slopeY = ddYslope->GetBinContent(i);
    rangeX = x->GetBinError(i);
    rangeY = y->GetBinError(i);

    float alignX = ddXslope->GetBinContent(i)*x->GetBinError(i);
    float alignY = ddYslope->GetBinContent(i)*y->GetBinError(i);

    float alignDX = ddDxslope->GetBinContent(i)*dxdz->GetBinError(i);
    float alignDY = ddDxslope->GetBinContent(i)*dydz->GetBinError(i);

    //cout << "slopeX = " << slopeX << " and rangeX = " << rangeX << ", slopeY = " << slopeY << " and rangeY = " << rangeY << "               " << slopeX*rangeX << " and " << slopeY*rangeY <<   endl;
    
    if ( fabs(alignX)>alignment_cut || fabs(alignY)>alignment_cut || fabs(alignDX)>alignment_cut || fabs(alignDY)>alignment_cut ) {
      continue;
    }

    cout << "  errors layer " << layer->GetBinContent(i) << ": stat on width = " << widthe;

    //add on an extra uncertainty to the fitted width because of the alignment
    //take the single biggest misalignment uncertainty
    float alignUncert = fabs(alignX);
    if ( fabs(alignY) > alignUncert ) alignUncert = fabs(alignY);
    if ( fabs(alignDX) > alignUncert ) alignUncert = fabs(alignDX);
    if ( fabs(alignDY) > alignUncert ) alignUncert = fabs(alignDY);
    alignUncert = alignUncert*10000;

    cout << " align = " << alignUncert;
    widthe = sqrt(widthe*widthe+(alignUncert*alignUncert));
    
    cout << " stat on prediction = " << prede;
    //add on extra uncertainty to predicted mean from tracking due to mulple scattering uncertainty
    float msUncert = ((0.0136/momentum_cut)*sqrt(0.025)*path_median)*10000;
    prede = sqrt(prede*prede+(msUncert*msUncert));

    cout << " MS = " << msUncert;
    
    if (widthm>predm) {
      hitmdata = sqrt(widthm*widthm-predm*predm);
      hitedata = sqrt(widthe*widthe+prede*prede);
    } else {
      cout << "BAD point: pred mean > dd width with entry = " << i << " and mean = "<< predm << " and widthm = " << widthm << endl;
    }

    cout << " total = " << hitedata << endl;     

    //ratio = hitm/hitmdata;
    ratio = hitmdata/hitm;
    ratioe = sqrt( ((hite/hitm)*(hite/hitm)) + ((hitedata/hitmdata)*(hitedata/hitmdata)) )*ratio;

    if (layer->GetBinContent(i)==1||layer->GetBinContent(i)==2){
      hitresTIB12->SetBinContent(iTIB12,hitmdata/sqrt(2));
      hitresTIB12->SetBinError(iTIB12,hitedata/sqrt(2));
      hitres1DTIB12->Fill(hitmdata/sqrt(2));
      pred1DTIB12->Fill(hitm/sqrt(2));
      hitsigmaTIB12->SetBinContent(iTIB12,hitm/sqrt(2));
      hitsigmaTIB12->SetBinError(iTIB12,hite/sqrt(2));
      ratio_hTIB12->SetBinContent(iTIB12,ratio);
      ratio_hTIB12->SetBinError(iTIB12,ratioe);
      ddTIB12->SetBinContent(iTIB12,widthm/sqrt(2));
      ddTIB12->SetBinError(iTIB12,widthe/sqrt(2));
      predTIB12->SetBinContent(iTIB12, predm/sqrt(2));
      predTIB12->SetBinError(iTIB12, prede/sqrt(2));
      iTIB12++;
    }
    
    if (layer->GetBinContent(i)==3||layer->GetBinContent(i)==4){
      //cout << "ratio = " << ratio << " ratio error = " << ratioe << " hitm = " << hitm << "+-" << hite << " hit data = " << hitmdata << "+-" << hitedata << endl;
    //cout << "ratio fractional error = " << ratioe/ratio << " and data fractional error = " << hitedata/hitmdata << " ratio error = " << ratio*hitedata/hitmdata << endl;
      hitresTIB34->SetBinContent(iTIB34,hitmdata/sqrt(2));
      hitresTIB34->SetBinError(iTIB34,hitedata/sqrt(2));
      hitres1DTIB34->Fill(hitmdata/sqrt(2));
      pred1DTIB34->Fill(hitm/sqrt(2));
      hitsigmaTIB34->SetBinContent(iTIB34,hitm/sqrt(2));
      hitsigmaTIB34->SetBinError(iTIB34,hite/sqrt(2));
      ratio_hTIB34->SetBinContent(iTIB34,ratio);
      ratio_hTIB34->SetBinError(iTIB34,ratioe);
      iTIB34++;
    }
    
    if (layer->GetBinContent(i)==5||layer->GetBinContent(i)==6||layer->GetBinContent(i)==7||layer->GetBinContent(i)==8){
      hitresTOB1234->SetBinContent(iTOB1234,hitmdata/sqrt(2));
      hitresTOB1234->SetBinError(iTOB1234,hitedata/sqrt(2));
      hitres1DTOB1234->Fill(hitmdata/sqrt(2));
      pred1DTOB1234->Fill(hitm/sqrt(2));
      hitsigmaTOB1234->SetBinContent(iTOB1234,hitm/sqrt(2));
      hitsigmaTOB1234->SetBinError(iTOB1234,hite/sqrt(2));
      ratio_hTOB1234->SetBinContent(iTOB1234,ratio);
      ratio_hTOB1234->SetBinError(iTOB1234,ratioe);
      iTOB1234++;
    }
    
    if (layer->GetBinContent(i)==9||layer->GetBinContent(i)==10){
      hitresTOB56->SetBinContent(iTOB56,hitmdata/sqrt(2));
      hitresTOB56->SetBinError(iTOB56,hitedata/sqrt(2));
      hitres1DTOB56->Fill(hitmdata/sqrt(2));
      pred1DTOB56->Fill(hitm/sqrt(2));
      hitsigmaTOB56->SetBinContent(iTOB56,hitm/sqrt(2));
      hitsigmaTOB56->SetBinError(iTOB56,hite/sqrt(2));
      ratio_hTOB56->SetBinContent(iTOB56,ratio);
      ratio_hTOB56->SetBinError(iTOB56,ratioe);
      iTOB56++;
    }
  }

  hitres1DTIB12->Draw();
  hitres1DTIB12->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/hitres1DTIB12_")+name+TString(".png"));  
  c1->SaveAs(TString("HitResStripPlots/hitres1DTIB12_")+name+TString(".eps"));  

  hitres1DTIB34->Draw();
  hitres1DTIB34->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/hitres1DTIB34_")+name+TString(".png"));  
  c1->SaveAs(TString("HitResStripPlots/hitres1DTIB34_")+name+TString(".eps"));  

  hitres1DTOB1234->Draw();
  hitres1DTOB1234->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/hitres1DTOB1234_")+name+TString(".png"));  
  c1->SaveAs(TString("HitResStripPlots/hitres1DTOB1234_")+name+TString(".eps"));  

  hitres1DTOB56->Draw();
  hitres1DTOB56->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/hitres1DTOB56_")+name+TString(".png")); 
  c1->SaveAs(TString("HitResStripPlots/hitres1DTOB56_")+name+TString(".eps")); 

  pred1DTIB12->Draw();
  pred1DTIB12->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/pred1DTIB12_")+name+TString(".png"));
  c1->SaveAs(TString("HitResStripPlots/pred1DTIB12_")+name+TString(".eps")); 

  pred1DTIB34->Draw();
  pred1DTIB34->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/pred1DTIB34_")+name+TString(".png"));
  c1->SaveAs(TString("HitResStripPlots/pred1DTIB34_")+name+TString(".eps"));

  pred1DTOB1234->Draw();
  pred1DTOB1234->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/pred1DTOB1234_")+name+TString(".png"));  
  c1->SaveAs(TString("HitResStripPlots/pred1DTOB1234_")+name+TString(".eps"));  

  pred1DTOB56->Draw();
  pred1DTOB56->Fit("gaus");
  c1->SaveAs(TString("HitResStripPlots/pred1DTOB56_")+name+TString(".png"));
  c1->SaveAs(TString("HitResStripPlots/pred1DTOB56_")+name+TString(".eps"));

  //first do plot to show how resolution is found
  ddTIB12->SetLineColor(2);
  ddTIB12->GetYaxis()->SetTitle("Uncertainty [#mum]");
  ddTIB12->GetXaxis()->SetTitle("");
  ddTIB12->GetXaxis()->SetLabelSize(0);
  ddTIB12->GetXaxis()->SetTickLength(0);
  ddTIB12->SetMarkerStyle(24);    //20
  ddTIB12->SetMarkerColor(2);
  hitresTIB12->SetLineColor(4);
  hitresTIB12->SetMarkerStyle(25);  //21
  hitresTIB12->SetMarkerColor(4);
  predTIB12->SetLineColor(1);
  predTIB12->SetMarkerStyle(26);
  predTIB12->SetMarkerColor(1);
  ddTIB12->SetMaximum(60);
  ddTIB12->Draw();
  predTIB12->Draw("same");
  hitresTIB12->Draw("same");
  
  leg = new TLegend(0.30,0.72,0.64,0.91);
  leg->AddEntry(ddTIB12,"double difference width","p");
  leg->AddEntry(hitresTIB12,"Hit resolution from data","p");
  leg->AddEntry(predTIB12,"<#sigma(#Deltax_{pred})>","p");
  leg->SetTextSize(0.055);
  leg->SetFillColor(0);
  leg->Draw("same");

  c1->SaveAs("HitResStripPlots/showRes.png");

  hitresTIB12->Fit("pol0");  
  hitresTIB12->SetLineColor(2);
  hitsigmaTIB12->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTIB12->GetXaxis()->SetTitle("");
  hitsigmaTIB12->GetXaxis()->SetLabelSize(0);
  hitsigmaTIB12->GetXaxis()->SetTickLength(0);
  hitresTIB12->SetMarkerStyle(24);    //20
  hitresTIB12->SetMarkerColor(2);
  hitsigmaTIB12->SetLineColor(4);
  hitsigmaTIB12->SetMarkerStyle(25);  //21
  hitsigmaTIB12->SetMarkerColor(4);
  hitresTIB12->SetMaximum(45);
  hitresTIB12->SetMinimum(0);
  hitresTIB12->Draw();
  hitsigmaTIB12->Draw("same");
  
  leg_histTIB12 = new TLegend(0.30,0.72,0.64,0.91);
  leg_histTIB12->AddEntry(hitresTIB12,"Hit resolution from data","p");
  leg_histTIB12->AddEntry(hitsigmaTIB12,"<#sigma(#Deltax_{hit})>","p");
  leg_histTIB12->SetTextSize(0.055);
  leg_histTIB12->SetFillColor(0);
  leg_histTIB12->Draw("same");

  TString fileRes = TString("HitResStripPlots/Resolution_") + name;
  TString fileRatio = TString("HitResStripPlots/ratio_") + name;
  c1->SaveAs(fileRes+"_TIB12.png");
  c1->SaveAs(fileRes+"_TIB12.eps");
  
  ratio_hTIB12->Fit("pol0");
  ratio_hTIB12->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTIB12->GetXaxis()->SetTitle("");
  ratio_hTIB12->GetXaxis()->SetLabelSize(0);
  ratio_hTIB12->GetXaxis()->SetTickLength(0);
  ratio_hTIB12->Draw();
  c1->SaveAs(fileRatio+"_TIB12.png");
  c1->SaveAs(fileRatio+"_TIB12.eps");

  hitresTIB34->Fit("pol0");  
  hitresTIB34->SetLineColor(2);
  hitsigmaTIB34->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTIB34->GetXaxis()->SetTitle("");
  hitsigmaTIB34->GetXaxis()->SetLabelSize(0);
  hitsigmaTIB34->GetXaxis()->SetTickLength(0);
  hitresTIB34->SetMarkerStyle(24);    //20
  hitresTIB34->SetMarkerColor(2);
  hitsigmaTIB34->SetLineColor(4);
  hitsigmaTIB34->SetMarkerStyle(25);  //21
  hitsigmaTIB34->SetMarkerColor(4);
  //hitresTIB34->SetMaximum(200);
  hitresTIB34->Draw();
  hitsigmaTIB34->Draw("same");
  
  leg_histTIB34 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTIB34->AddEntry(hitresTIB34,"Hit resolution from data","p");
  leg_histTIB34->AddEntry(hitsigmaTIB34,"<#sigma(#Deltax_{hit})>","p");
  leg_histTIB34->SetTextSize(0.055);
  leg_histTIB34->SetFillColor(0);
  leg_histTIB34->Draw("same");
  c1->SaveAs(fileRes+"_TIB34.png");
  c1->SaveAs(fileRes+"_TIB34.eps");
  
  ratio_hTIB34->Fit("pol0");
  ratio_hTIB34->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTIB34->GetXaxis()->SetTitle("");
  ratio_hTIB34->GetXaxis()->SetLabelSize(0);
  ratio_hTIB34->GetXaxis()->SetTickLength(0);
  ratio_hTIB34->Draw();
  c1->SaveAs(fileRatio+"_TIB34.png");
  c1->SaveAs(fileRatio+"_TIB34.eps");

  hitresTOB1234->Fit("pol0");  
  hitresTOB1234->SetLineColor(2);
  hitsigmaTOB1234->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTOB1234->GetXaxis()->SetTitle("");
  hitsigmaTOB1234->GetXaxis()->SetLabelSize(0);
  hitsigmaTOB1234->GetXaxis()->SetTickLength(0);
  hitresTOB1234->SetMarkerStyle(24);    //20
  hitresTOB1234->SetMarkerColor(2);
  hitsigmaTOB1234->SetLineColor(4);
  hitsigmaTOB1234->SetMarkerStyle(25);  //21
  hitsigmaTOB1234->SetMarkerColor(4);
  //hitresTOB1234->SetMaximum(200);
  hitresTOB1234->Draw();
  hitsigmaTOB1234->Draw("same");
  
  leg_histTOB1234 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTOB1234->AddEntry(hitresTOB1234,"Hit resolution from data","p");
  leg_histTOB1234->AddEntry(hitsigmaTOB1234,"<#sigma(#Deltax_{hit})>","p");
  leg_histTOB1234->SetTextSize(0.055);
  leg_histTOB1234->SetFillColor(0);
  leg_histTOB1234->Draw("same");
  c1->SaveAs(fileRes+"_TOB1234.png");
  c1->SaveAs(fileRes+"_TOB1234.eps");
  
  ratio_hTOB1234->Fit("pol0");
  ratio_hTOB1234->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTOB1234->GetXaxis()->SetTitle("");
  ratio_hTOB1234->GetXaxis()->SetLabelSize(0);
  ratio_hTOB1234->GetXaxis()->SetTickLength(0);
  ratio_hTOB1234->Draw();
  c1->SaveAs(fileRatio+"_TOB1234.png");
  c1->SaveAs(fileRatio+"_TOB1234.eps");

  hitresTOB56->Fit("pol0");  
  hitresTOB56->SetLineColor(2);
  hitsigmaTOB56->GetYaxis()->SetTitle("Uncertainty [#mum]");
  hitsigmaTOB56->GetXaxis()->SetTitle("");
  hitsigmaTOB56->GetXaxis()->SetLabelSize(0);
  hitsigmaTOB56->GetXaxis()->SetTickLength(0);
  hitresTOB56->SetMarkerStyle(24);    //20
  hitresTOB56->SetMarkerColor(2);
  hitsigmaTOB56->SetLineColor(4);
  hitsigmaTOB56->SetMarkerStyle(25);  //21
  hitsigmaTOB56->SetMarkerColor(4);
  //hitresTOB56->SetMaximum(200);
  hitresTOB56->Draw();
  hitsigmaTOB56->Draw("same");
  
  leg_histTOB56 = new TLegend(0.60,0.72,0.94,0.91);
  leg_histTOB56->AddEntry(hitresTOB56,"Hit resolution from data","p");
  leg_histTOB56->AddEntry(hitsigmaTOB56,"<#sigma(#Deltax_{hit})>","p");
  leg_histTOB56->SetTextSize(0.055);
  leg_histTOB56->SetFillColor(0);
  leg_histTOB56->Draw("same");
  c1->SaveAs(fileRes+"_TOB56.png");
  c1->SaveAs(fileRes+"_TOB56.eps");
  
  ratio_hTOB56->Fit("pol0");
  ratio_hTOB56->GetYaxis()->SetTitle("Ratio of resolution from data and calculation");
  ratio_hTOB56->GetXaxis()->SetTitle("");
  ratio_hTOB56->GetXaxis()->SetLabelSize(0);
  ratio_hTOB56->GetXaxis()->SetTickLength(0);
  ratio_hTOB56->Draw();
  c1->SaveAs(fileRatio+"_TOB56.png");
  c1->SaveAs(fileRatio+"_TOB56.eps");

}

void setTDRStyle() {

   TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
   tdrStyle->SetCanvasBorderMode(0);
   tdrStyle->SetCanvasColor(kWhite);
   tdrStyle->SetCanvasDefH(600); //Height of canvas
   tdrStyle->SetCanvasDefW(1100); //Width of canvas
   tdrStyle->SetCanvasDefX(0);   //POsition on screen
   tdrStyle->SetCanvasDefY(0);

// For the Pad:
   tdrStyle->SetPadBorderMode(0);
   // tdrStyle->SetPadBorderSize(Width_t size = 1);
   tdrStyle->SetPadColor(kWhite);
   tdrStyle->SetPadGridX(false);
   tdrStyle->SetPadGridY(false);
   tdrStyle->SetGridColor(0);
   tdrStyle->SetGridStyle(3);
   tdrStyle->SetGridWidth(1);

// For the frame:
   tdrStyle->SetFrameBorderMode(0);
   tdrStyle->SetFrameBorderSize(1);
   tdrStyle->SetFrameFillColor(0);
   tdrStyle->SetFrameFillStyle(0);
   tdrStyle->SetFrameLineColor(1);
   tdrStyle->SetFrameLineStyle(1);
   tdrStyle->SetFrameLineWidth(1);

// For the histo:
   // tdrStyle->SetHistFillColor(1);
   // tdrStyle->SetHistFillStyle(0);
   //tdrStyle->SetHistLineColor(1);
   //tdrStyle->SetHistLineStyle(0);
   tdrStyle->SetHistLineWidth(1.5);
   // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
   // tdrStyle->SetNumberContours(Int_t number = 20);

//   tdrStyle->SetEndErrorSize(2);
//   tdrStyle->SetErrorMarker(20);
//   tdrStyle->SetErrorX(0.);

   //tdrStyle->SetMarkerStyle(20);

//For the fit/function:
   tdrStyle->SetOptFit(1);
   tdrStyle->SetFitFormat("5.4g");
   tdrStyle->SetFuncColor(2);
   tdrStyle->SetFuncStyle(1);
   tdrStyle->SetFuncWidth(1);

//For the date:
   tdrStyle->SetOptDate(0);
   // tdrStyle->SetDateX(Float_t x = 0.01);
   // tdrStyle->SetDateY(Float_t y = 0.01);

// // For the statistics box:
//   tdrStyle->SetOptFile(0);
//   tdrStyle->SetOptStat(0); // To display the mean and RMS:    
//   tdrStyle->SetOptStat("mr");
   tdrStyle->SetOptStat("");
   tdrStyle->SetStatColor(kWhite);
   tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
   tdrStyle->SetStatTextColor(1);
   tdrStyle->SetStatFormat("6.4g");
   tdrStyle->SetStatBorderSize(1);
   tdrStyle->SetStatH(0.1);
   tdrStyle->SetStatW(0.15);
   // tdrStyle->SetStatStyle(Style_t style = 1001);
   // tdrStyle->SetStatX(Float_t x = 0);
   // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
   tdrStyle->SetPadTopMargin(0.05);
   tdrStyle->SetPadBottomMargin(0.13);
   tdrStyle->SetPadLeftMargin(0.10);
   tdrStyle->SetPadRightMargin(0.02);

// For the Global title:

   tdrStyle->SetOptTitle(0);
   tdrStyle->SetTitleFont(42);
   tdrStyle->SetTitleColor(1);
   tdrStyle->SetTitleTextColor(1);
   tdrStyle->SetTitleFillColor(10);
   tdrStyle->SetTitleFontSize(0.05);
   // tdrStyle->SetTitleH(0); // Set the height of the title box
   // tdrStyle->SetTitleW(0); // Set the width of the title box
   // tdrStyle->SetTitleX(0); // Set the position of the title box
   // tdrStyle->SetTitleY(0.985); // Set the position of the title box
   // tdrStyle->SetTitleStyle(Style_t style = 1001);
   // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

   tdrStyle->SetTitleColor(1, "XYZ");
   tdrStyle->SetTitleFont(42, "XYZ");
   //tdrStyle->SetTitleSize(0.06, "XYZ");
   tdrStyle->SetTitleSize(0.052, "XYZ");
   // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
   // tdrStyle->SetTitleYSize(Float_t size = 0.02);
   //tdrStyle->SetTitleXOffset(0.9);
   tdrStyle->SetTitleXOffset(0.875);
   tdrStyle->SetTitleYOffset(0.90);
   //tdrStyle->SetTitleYOffset(1.05);
   // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the  Offset

// For the axis labels:

   tdrStyle->SetLabelColor(1, "XYZ");
   tdrStyle->SetLabelFont(42, "XYZ");
   tdrStyle->SetLabelOffset(0.007, "XYZ");
   tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

   tdrStyle->SetAxisColor(1, "XYZ");
   tdrStyle->SetStripDecimals(kTRUE);
   tdrStyle->SetTickLength(0.03, "XYZ");
   tdrStyle->SetNdivisions(510, "XYZ");
   tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
   tdrStyle->SetPadTickY(1);

// Change for log plots:
   tdrStyle->SetOptLogx(0);
   tdrStyle->SetOptLogy(0);
   tdrStyle->SetOptLogz(0);

// Postscript options:
   tdrStyle->SetPaperSize(20.,20.);
   // tdrStyle->SetLineScalePS(Float_t scale = 3);
   // tdrStyle->SetLineStyleString(Int_t i, const char* text);
   // tdrStyle->SetHeaderPS(const char* header);
   // tdrStyle->SetTitlePS(const char* pstitle);

   // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
   // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
   // tdrStyle->SetPaintTextFormat(const char* format = "g");
   // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
   // tdrStyle->SetTimeOffset(Double_t toffset);
   // tdrStyle->SetHistMinimumZero(kTRUE);

   tdrStyle->cd();

}
