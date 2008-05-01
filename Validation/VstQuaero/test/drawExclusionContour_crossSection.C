{
mystyle1 = new TStyle("mystyle1","mystyle1");
mystyle1->SetPadRightMargin(.15);
mystyle1->SetPadLeftMargin(.15);
mystyle1->SetPadBottomMargin(.15);

TCanvas *ca = new TCanvas("ca","",500,500);
mystyle1->cd();
ca->UseCurrentStyle();
ca->SetFillColor(0);

double x[9] = { 100, 125, 150, 175, 200, 225, 250, 275, 300 };
double y[9] = { 0.0008, 0.0012, 0.0022, 0.0027, 0.0035, 0.005, 0.0065, 0.018, 0.027 };
TGraph* gr = new TGraph(9,x,y);
gr->SetTitle("");
gr->GetXaxis()->SetTitleOffset(1.5);
gr->GetXaxis()->CenterTitle();
gr->GetXaxis()->SetLabelSize(0.03);
gr->GetYaxis()->SetTitleOffset(1.5);
gr->GetYaxis()->CenterTitle();
gr->GetYaxis()->SetLabelSize(0.03);
gr->GetXaxis()->SetLimits(100,300);
gr->GetYaxis()->SetLimits(0.0001,0.1);
gr->SetMinimum(0.0001);
gr->SetMaximum(0.1);
gr->GetXaxis()->SetTitle("m_{q^{*}} (GeV)");
gr->GetYaxis()->SetTitle("f/#Lambda (GeV^{-1})");
gr->SetLineWidth(2);
gPad->SetLogy();
gr->Draw("LA");
ca->Print("model_tag.eps");
gSystem->Exit(0);
}




/*
{
#include <fstream>
#include <string>
#include "TF1.h"
#include "TGraphErrors.h"
using namespace std;

ifstream fin("tmp.txt");
double x1=0, x2=0;
const int N=100;
double crossSection[N] = new double[N], coupling[N] = new double[N], logLikelihoodRatio[N] = new double[N], logLikelihoodRatioError[N] = new double[N], binWidth[N] = new double[N];
double x[20] = new double[20], y[20] = new double[20];
int n=0, k=0;
while(fin)
{
  if((!(fin >> x1))||((x2!=x1)&&(x2!=0)))
    {
      TGraphErrors* g = new TGraphErrors(n,crossSection,logLikelihoodRatio,binWidth,logLikelihoodRatioError);
      double* parabolaParameters = new double[3];
      g->Fit("pol2");
      g->GetFunction("pol2")->GetParameters(parabolaParameters);
      g->GetFunction("pol2")->Draw();
      //assert(parabolaParameters[0]<0); // concave downwards
      TF1* posteriorDistribution = TH1::GetFunction("gaus");
      double sigma = 1/sqrt(2*parabolaParameters[0]/log10(exp(1.)));
      double mu = parabolaParameters[1]/(2*parabolaParameters[0]);
      posteriorDistribution->SetParameters(mu,sigma,1);
      x[k] = x2; y[k] = mu + 2*sigma;
      k++;
      n=0;
    }
  if(fin)
    {
      x2=x1;
      fin >> coupling[n] >> logLikelihoodRatio[n];
      coupling[n] *= 1000;
      crossSection[n] = coupling[n]*coupling[n];
      logLikelihoodRatioError[n] = 1;
      binWidth[n] = 0.01;
      if(logLikelihoodRatio[n]>-5)
	n++;
    }
}
TGraph exclusionContour(k,x,y);
exclusionContour->Draw();
}
*/
