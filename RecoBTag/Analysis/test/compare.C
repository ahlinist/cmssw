#include <vector>
#include <string>
using namespace std;

int colorList[] = {1,2,3,4};  
int markerStyleList[] = {21,22,23,21,22,23};  

TObject * getHistogram(TFile * f, string algo,string histoName, string range = "GLOBAL",string suffix ="")
{
string prefix = "JetTag";
string d = prefix+"_"+algo+"_"+range;
TDirectory * dir  =(TDirectory *) f->Get(d.c_str());
cout << histoName+"_"+algo+"_"+range << endl;
return dir->Get((histoName+"_"+algo+"_"+range+suffix).c_str());
}


void setStyle(int i, TH1F *obj)
{
obj->SetMarkerColor(colorList[i]);
obj->SetMarkerStyle(markerStyleList[i]);
}



void drawAll(string suf = "")
{
 // TFile *_file0 = TFile::Open("jetTagAnalysisA-test-4-menomeno.root"); //jetTagAnalysisA-test-4-menomeno-abs.root");
  TFile *_file0 = TFile::Open("jetTagAnalysisB-test-2.0.root");
  TFile *_file1 = TFile::Open("jetTagAnalysisB-test.root");
  
  vector<TFile *> files;
  vector<string> algos;
/*  algos.push_back("jetProbabilityJetTags");
  files.push_back(_file0);

*/
  algos.push_back("jetProbabilityJetTagsTestMass");
  files.push_back(_file0);
  algos.push_back("jetProbabilityJetTags");
  files.push_back(_file1);
  algos.push_back("jetProbabilityJetTagsTest");
  files.push_back(_file1);
  algos.push_back("jetProbabilityJetTagsTestMass");
  files.push_back(_file1);
  
/*algos.push_back("trackCountingHighEffJetTags");
  files.push_back(_file0);
  
  algos.push_back("trackCountingHighPurJetTags");
  files.push_back(_file0);
  
  algos.push_back("jetProbabilityJetTags");
  files.push_back(_file1);

  algos.push_back("trackCountingHighEffJetTags");
  files.push_back(_file1);
  
  algos.push_back("trackCountingHighPurJetTags");
  files.push_back(_file1);
*/


  TLegend * leg = new TLegend(0.4,0.4,0.6,0.6);
  TCanvas * c1 = new TCanvas();
  c1->SetLogy();  
  c1->SetGridy();  
  c1->SetGridx();  
  for(int i = 0 ; i < algos.size() ; i++)
   {
      cout << algos[i] << endl;
//     TH1F * h = (TH1F *) getHistogram(files[i],algos[i],"FlavEffVsBEff_DUS_discrFC","ETA_0-1.4_PT_50-80");
     TH1F * h = (TH1F *) getHistogram(files[i],algos[i],"FlavEffVsBEff_C_discrFC","GLOBAL");
//     TH1F * h = (TH1F *) getHistogram(files[i],algos[i],"FlavEffVsBEff_G_discrFC","GLOBAL"); //ETA_0-1.4_PT_50-80");
//     TH1F * h = (TH1F *) getHistogram(files[i],algos[i],"discrCutCond_discrFC","ETA_0-1.4_PT_50-80",suf);
     cout << h << endl;
     if(i==0) h->Draw(); else h->Draw("same"); 
     setStyle(i,h);
     leg->AddEntry(h,algos[i].c_str(),"p");
   }
  leg->Draw("same");

}

