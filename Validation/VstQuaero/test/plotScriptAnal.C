

double lo = -5.0;
double hi = 2.0;
for (int i = 0;i < Npoints;i++)
  if(res[i]>hi)
    hi = (int)(res[i]+2);

  TGraphAsymmErrors *g = new TGraphAsymmErrors(Npoints);
  for (int i = 0;i < Npoints;i++)
    g->SetPoint(i,steps[i],res[i]);
  TGraphAsymmErrors *gSensitivityH = new TGraphAsymmErrors(Npoints);
  TGraphAsymmErrors *gSensitivitySM = new TGraphAsymmErrors(Npoints);
  for (int i = 0;i < Npoints;i++)
    {
      gSensitivityH->SetPoint(i,steps[i],sensitivityH[i]);
      gSensitivitySM->SetPoint(i,steps[i],sensitivitySM[i]);
    }

g->SetTitle("");
gSensitivityH->SetTitle("");
gSensitivitySM->SetTitle("");
  g->SetMarkerStyle(20);
  g->SetMarkerColor(4);
  TCanvas *cg = new TCanvas("cg","",400,350);
//cg->Divide(1,2);
  cg->cd(1);
g->SetMinimum(lo);
g->SetMaximum(hi);
  g->Draw("ALP");
gSensitivityH->SetLineStyle(2);
gSensitivityH->SetLineColor(3);
gSensitivityH->Draw();
gSensitivitySM->SetLineStyle(2);
gSensitivitySM->SetLineColor(2);
gSensitivitySM->Draw();

//  g->GetXaxis()->SetTitle("#sigma [pb]");
//  g->GetXaxis()->SetTitle("M_{#frac{1}{2}} (GeV)");
//  g->GetXaxis()->SetTitle("m_{H^{#pm#pm}} (GeV)");
//  g->GetXaxis()->SetTitle("m_{H^{#pm}} (GeV)");
//  g->GetXaxis()->SetTitle("m_{h} (GeV)");
//  g->GetXaxis()->SetTitle("#kappa_{tu#gamma}");
  g->GetXaxis()->SetTitle("XAXISLABEL");
  g->GetYaxis()->SetTitle("log_{10} L");

 
  
  double low = steps[0]/2.0;
  double high = steps[Npoints-1]*2.0;

  double fval = 1.0/(high-low);


  double prob[Npoints];

  TGraphAsymmErrors *gp = new TGraphAsymmErrors(Npoints);


  for (int i = 0;i < Npoints;i++)
    {
      prob[i] = pow(10,res[i]) * fval;
    }

  double tot=0;
  double totold=0;
  for (int i = 0;i < Npoints;i++)
    {
      double prev = 0;
      double prevprob = prob[i];
      if (i != 0) 
	{
	  prev = steps[i-1];
	  prevprob = prob[i-1];
	}
      //cout << "Slice " << prob[i] << " or " << (0.5*(prevprob+prob[i])) << endl;
      tot += prob[i]*(steps[i] - prev);
      
    }

cout << "Total area: " << totold << " or " << tot << endl;
  
double max = -1.0;
  for (int i = 0;i < Npoints;i++)
    {

      prob[i] /= tot;
      if (prob[i] > max) { max = prob[i]; }
    }
  
  for (int i = 0;i < Npoints;i++)
    {
      gp->SetPoint(i,steps[i],prob[i]);
    }

  
  TLine *tl = new TLine(SMVALUE,lo,SMVALUE,0.0);
  tl->SetLineColor(11);
  tl->SetLineStyle(3);
  tl->Draw();
  TLine *tl2 = new TLine(g->GetXaxis()->GetXmin(),-1.0,g->GetXaxis()->GetXmax(),-1.0);
  tl2->SetLineColor(9);
  tl2->SetLineStyle(3);
  tl2->Draw();
  

//cg->cd(2);

gp->SetMarkerStyle(20);
gp->SetMarkerColor(4);
//gp->Draw("ALP");
  gp->SetTitle("");
gp->GetXaxis()->SetTitle("m_{LQ} [GeV]");
//gp->GetXaxis()->SetTitle("#sigma [pb]");
gp->GetYaxis()->SetTitle("P(H|D)");

  cg->Print("MODEL_TAG.eps");

cout << "Max = " << max << endl;
max = max * 0.9;
  TLine *tl = new TLine(SMVALUE,0.0,SMVALUE,1.0);
  tl->SetLineColor(2);
  tl->SetLineStyle(2);
  tl->Draw();
}

