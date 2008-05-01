{
//gROOT->Reset();
#include <cstdlib>
#include <string>
using namespace std;

string example = getenv("example");
string exampleSuffix = getenv("exampleSuffix");
string exp = getenv("exp");
string logLdirectory = "logL/";

// set bounds of plot

int d = 2; // dimensionality of space

// create new palette

mystyle1 = new TStyle("mystyle1","mystyle1");

const int n = 5;
int colors[2*n] = new int[2*n];
for(int i=0; i<n; i++) 
{
  colors[i]= i+100;
  double q = .8*i/n+.2;
  gROOT->GetColor(colors[i])->SetRGB(1,q,q);
  if(i==4)
    gROOT->GetColor(colors[i])->SetRGB(1,1,1);
}
for(int i=0; i<n; i++) 
{
  colors[n+i]= n+i+100;
  double q = .8*(n-i)/n+.2;
  gROOT->GetColor(colors[n+i])->SetRGB(q,1,q);
}

mystyle1->SetPalette(2*n,colors);
mystyle1->SetPadRightMargin(.15);
mystyle1->SetPadLeftMargin(.15);
mystyle1->SetPadBottomMargin(.15);

TCanvas *ca = new TCanvas("ca","",500,500);
mystyle1->cd();
ca->UseCurrentStyle();
ca->SetFillColor(0);
//ca->Divide(2,2);
if((example=="rpvSusy")
   ||(example=="excitedQuark")
   ||(example=="lq")   
   ||(example=="smHiggsGammaGamma")
   )
  gPad->SetLogy();

ifstream fin1((logLdirectory+exp+"_"+example+exampleSuffix+"_logL.txt").c_str());
string s;
int N = 0;
while(fin1 >> s)
{ N++; fin1 >> s >> s; }
fin1.close();
cout << "N = " << N << endl;
ifstream fin2((logLdirectory+exp+"_"+example+exampleSuffix+"_logL.txt").c_str());
double x[N] = new double[N], y[N] = new double[N], z[N] = new double[N];
string s;
for(int i=0; i<N; i++)
{
  fin2 >> x[i] >> y[i] >> z[i];
  if(z[i]<-5)
    z[i]=-5;
}
fin2.close();

//ca.cd(1);
TGraph2D *gr = new TGraph2D(N,x,y,z);
gr->SetTitle("");
gr->SetNpx(100);
gr->SetNpy(500);
gr->GetXaxis()->SetTitleOffset(1.5);
gr->GetXaxis()->CenterTitle();
gr->GetXaxis()->SetLabelSize(0.03);
gr->GetYaxis()->SetTitleOffset(1.5);
gr->GetYaxis()->CenterTitle();
gr->GetYaxis()->SetLabelSize(0.03);
gr->GetZaxis()->SetTitle("log_{10} L");
gr->SetMaximum(5);
gr->SetMinimum(-5);

if((exp=="h1")&&
   (example=="doublyChargedHiggs"))
  {
    gr->GetXaxis()->SetTitle("M_{H^{#pm#pm}}");
    gr->GetYaxis()->SetTitle("h_{ee}");
  }
if(example=="smHiggsGammaGamma")
  {
    gr->GetXaxis()->SetTitle("m_{h}");
    gr->GetYaxis()->SetTitle("Br(h#rightarrow#gamma#gamma)");
  }
if((exp=="h1")&&
   (example=="rpvSusy"))
  {
    gr->GetXaxis()->SetTitle("M_{#tilde{q}}");
    gr->GetYaxis()->SetTitle("#lambda'_{111}");
    gr->GetYaxis()->SetLimits(0.01,1);
  }
if(example=="mSugra")
  {
    gr->GetXaxis()->SetTitle("M_{0}");
    gr->GetYaxis()->SetTitle("M_{#frac{1}{2}}");
  }
if(example=="excitedElectron")
  {
    gr->GetXaxis()->SetTitle("m_{e^{*}}");
    gr->GetYaxis()->SetTitle("#Lambda");
  }
if(example=="singleTop")
  {
    gr->GetXaxis()->SetTitle("#kappa_{#gamma}");
    gr->GetYaxis()->SetTitle("#kappa_{Z}");
  }
if(example=="excitedQuark")
  {
    gr->GetXaxis()->SetTitle("m_{q^{*}} (GeV)");
    gr->GetYaxis()->SetTitle("f/#Lambda (GeV^{-1})");
  }
if(example=="lq")
  {
    gr->GetXaxis()->SetTitle("m_{LQ} (GeV)");
    gr->GetYaxis()->SetTitle("#lambda");
  }
if(example=="top")
  {
    if(exampleSuffix=="gRgL")
      {
	gr->GetXaxis()->SetTitle("g_{R}");
	gr->GetYaxis()->SetTitle("g_{L}");
      }
  }    
if(example=="top_massXsec")
{
  gr->GetXaxis()->SetTitle("m_{t}");
  gr->GetYaxis()->SetTitle("#sigma_{t#bar{t}}");
}
if(example=="rsGraviton")
{
  gr->GetXaxis()->SetTitle("m_{G^{*}}");
  gr->GetYaxis()->SetTitle("k/M_{Pl}");
}


gr->Draw("COLZ");
if((exp=="h1")&&
   (example=="doublyChargedHiggs"))
  {
    TLine *tl = new TLine(100,0.18,135,0.3);
    tl->SetLineColor(11);
    tl->SetLineWidth(4)
    tl->SetLineStyle(1);
    tl->Draw();
  }

ca->Print("model_tag.eps");

gSystem->Exit(0);

}
