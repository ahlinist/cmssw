
  TGraphAsymmErrors *g = new TGraphAsymmErrors(Npoints);
  for (int i = 0;i < Npoints;i++)
    {
      g->SetPoint(i,steps[i],res[i]);
    }
g->SetTitle(""); //Quaero results for MODEL, tag TAG
  g->SetMarkerStyle(20);
  g->SetMarkerColor(4);
  TCanvas *cg = new TCanvas("cg","",400,700);
  cg.Divide(1,2);
  cg.cd(1);
gPad->SetLogx(1);
g->SetMinimum(-5.0);
  g->Draw("ALP");

  g->GetXaxis()->SetTitle("#sigma_{H}/#sigma_{Theory}");
  g->GetYaxis()->SetTitle("log_{10}(L)");

 
  
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
      //      cout << "Slice " << prob[i] << " or " << (0.5*(prevprob+prob[i])) << endl;
      tot += prob[i]*(steps[i] - prev);
      
    }

cout << "Total area: " << totold << " or " << tot << endl;
  
  for (int i = 0;i < Npoints;i++)
    {
      prob[i] /= tot;
    }
  
  for (int i = 0;i < Npoints;i++)
    {
      gp->SetPoint(i,steps[i],prob[i]);
    }

  double excl = 0;
int ilim = Npoints-5;
for (int i = 0;i < Npoints;i++)
    {
      if (steps[i] <= 1.0)
	{
	  double prev = 0;
	  double prevprob = prob[i];
	  if (i != 0) 
	    {
	      prev = steps[i-1];
	      prevprob = prob[i-1];
	    }
	  //cout << prob[i] << ", " << (steps[i] - prev) << endl;
	  //cout << "Exclusion at " << excl << endl;
	  excl += prob[i]*(steps[i] - prev);
	  
	}
      if (steps[i] == 1.0) ilim=i;
    }

  cout << "Exclude SM at " << excl << endl;

 double excl95 = 0;
 double prevexcl = 0;
double step95 = 0.0;
int i95 = 0;
for (int i = 0;i < Npoints && excl95 < 0.95;i++)
    {
      {
	double prev = 0;
	double prevprob = prob[i];

	if (i != 0) 
	  {
	    prev = steps[i-1];
	    prevprob = prob[i-1];
	  }
	//cout << prob[i] << ", " << (steps[i] - prev) << endl;
	//cout << "Exclusion at " << excl << endl;
	
	prevexcl = excl95;
	excl95 += prob[i]*(steps[i] - prev);
	
      }
      if (excl95 > 0.95) 
	{
	  i95=i;
	  double intdiff = 0.95-prevexcl;
	  double diff = excl95-prevexcl;
	  if (i>0)
	    step95 = steps[i-1] + (intdiff/diff)*(steps[i]-steps[i-1]);
	  else
	    step95 = steps[0];
	}
    }

cout << "Exclude " << steps[i] << " at " << excl95 << endl;
cout << "Exclude " << step95 << " at 0.95" <<  endl;

TLatex *ts = new TLatex(0.0,-4.0,"SIGMAS");  
ts->Draw();

  cg.cd(2);
gPad->SetLogx(1);
  gp->Draw("AP");
  char ress[100];
  sprintf(ress,"#int_{0}^{1}P(#sigma)d#sigma = %1.2f",excl);

double g_max = gp->GetYaxis()->GetXmax();
g_max = g_max*0.85;
  TLatex *t = new TLatex(0.0,g_max,ress);
t->SetTextSize(0.04);
t->SetTextColor(2);
  char ress952[100];
  sprintf(ress952," #int_{0}^{%2.1f}P(#sigma)d#sigma = .95",step95);
printf("res952 = %s\n",ress952);
  TLatex *t952 = new TLatex(log10(step95),g_max,ress952);
t952->SetTextSize(0.04);
t952->SetTextColor(4);
  gp->SetMarkerStyle(20);
  gp->SetMarkerColor(4);

TLine *tl1 = new TLine(0.0,0.0,0.0,g_max*0.98);
tl1->SetLineColor(2);
tl1->SetLineStyle(2);

TLine *tl2 = new TLine(log10(step95),0.0,log10(step95),g_max*0.98);
tl2->SetLineColor(4);
tl2->SetLineStyle(4);
tl1->Draw();
tl2->Draw();
  gp->SetTitle("");
  gp->GetXaxis()->SetTitle("#sigma_{H}/#sigma_{Theory}");
  gp->GetYaxis()->SetTitle("P(H(#sigma_{H})|D)");

t->Draw();  t952->Draw(); 


  cg->Print("log/MODEL_TAG.eps");
}

