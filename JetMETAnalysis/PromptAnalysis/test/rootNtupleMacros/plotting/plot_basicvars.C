using namespace std;
#include "tdrstyle.C"

void plot_basicvars(){
   
  // specify the DATA/MC root file!!!
  // --------------------------------------------------------------------------
   TFile *data = TFile::Open("V00-00-25-MinimumBias-Commissioning10-PromptReco-v7_test2/root/V00-00-25-MinimumBias-Commissioning10-PromptReco-v7_test2.root");

  char *legDataTitle = "Data";
  char *legMCTitle = "Simulation";
  TString dataset="";
  TString overallTitle="CMS preliminary 2010";
  TString CMen="#sqrt{s}=7TeV";
  TString cuts1="";
  TString cuts2="";
  double rebin=4.;
  bool isptplot;
  //---------------------------------------------------------------------------
  setTDRStyle();
  gROOT->ForceStyle();
  TIter next (data->GetListOfKeys ());

  TString name;
  TRegexp jpt("JPT");  
  TRegexp pf("PF");
  int colour;
  TRegexp twodim("map");
  //non log plots 
  TRegexp eta("eta");
  TRegexp phi("phi");
  TRegexp hits("hits");
  //
  TRegexp pt("pt");
  TRegexp dphi("dphi");
  TRegexp mass("mass");


  int islog=1;

  for (TKey* key = 0; (key = (TKey *) next());) {
    isptplot=false;
    //determine wether it is a Calo/JPT or PF plots to make
    name=key->GetName();
    //    if(name.Index(twodim)>-1) continue; //dont want comparisons for 2D histos
    colour=5;
      cuts1="p_{T}(jet)> 10 GeV";
      cuts2="|#eta(jet)| < 3";
    if(name.Index(jpt)>-1) {
      colour=896;
      cuts1="p_{T}(jet)> 8 GeV";
      cuts2="|#eta(jet)| < 2";
    }
    if(name.Index(pf)>-1){
      colour=38;
      cuts1="p_{T}(jet)> 8 GeV";
      cuts2="|#eta(jet)| < 3";
    } 
   

    islog=1;
    if(name.Index(eta)>-1||name.Index(phi)>-1||name.Index(hits)>-1) islog=0;

    char histname_clean [256];
    TH1D * h_data= data->Get(key->GetName());
    sprintf (histname_clean, "%sJIDloose", key->GetName());
    cout << histname_clean<< endl;
    TH1D * h_data_clean= data->Get(histname_clean); 
    // only for filled histos
    if( h_data->GetEntries()!=0) {
      //
      h_data->SetTitle(overallTitle);
      if(name.Index(pt)>-1) {
	h_data->SetYTitle("Jets/GeV");
	isptplot=true;  
	//	cout << 	h_data->GetXaxis()->GetBinWidth(1)<< endl;  
      }
      if(name.Index(eta)>-1||name.Index(phi)>-1) {
	h_data->SetYTitle("Jets");
      }
      if(name.Index(dphi)>-1) {
	h_data->SetYTitle("Events");
      }
      if(name.Index(mass)>-1){
	h_data->SetYTitle("Events/GeV");
	isptplot=true;
      }

    TCanvas *c = new TCanvas("c","",600,600); 
    if(( h_data->GetEntries()!=0)&&( h_data_clean)) { 
        h_data->SetYTitle("number of jets / bin");
        h_data->SetLineWidth(2);
        h_data->SetLineStyle(1);
        h_data_clean->SetLineStyle(2);
        h_data_clean->SetLineWidth(2);
        //      h_data->Rebin(rebin);
        //h_data_clean->Rebin(rebin);
        dataset="Data, cleaned jets loose 7 TeV"; 
        std::cout << "Plotting: " << key->GetName()  <<  std::endl;
        MoveStatsAndDraw(h_data , h_data_clean, legDataTitle, dataset, 1);
        char histname [256];
        sprintf (histname, "Overlay%sLoose.gif", key->GetName());
        c.Print (histname);
    }//if Entries != 0 


    sprintf (histname_clean, "%sJIDtight", key->GetName());
    cout << histname_clean<< endl;
    TH1D * h_data_tight= data->Get(histname_clean); 
    if(( h_data->GetEntries()!=0)&&( h_data_tight)) { 
        h_data->SetYTitle("number of jets / bin");
        h_data->SetLineWidth(2);
        h_data->SetLineStyle(1);
        h_data_tight->SetLineStyle(2);
        h_data_tight->SetLineWidth(2);
        //      h_data->Rebin(rebin);
        //h_data_clean->Rebin(rebin);
        dataset="Data, cleaned jets tight7 TeV"; 
        std::cout << "Plotting: " << key->GetName()  <<  std::endl;
        MoveStatsAndDraw(h_data , h_data_tight, legDataTitle, dataset, 1);
        char histname [256];
        sprintf (histname, "Overlay%sTight.gif", key->GetName());
        c.Print (histname);
    }//if Entries != 0 

    delete c;
    } //if Entries != 0 
  } // loop over keys

  cout << "------ Calo Jets --------" <<endl;
   TH1D * temp =data->Get("dijetptall");
   cout << "CaloJets       " << temp->GetEntries() <<endl;
   TH1D *temp2 =data->Get("dijetptallJIDloose");
   cout << "Loose CaloJets " << temp2->GetEntries() <<endl;
   TH1D *temp3 =data->Get("dijetptallJIDtight");
   cout << "Tight CaloJets " << temp3->GetEntries() <<endl;
  
   cout << "------ PF Jets --------" <<endl;
   TH1D * temp5 =data->Get("diPFjetptall");
   cout << "PFJets       " << temp5->GetEntries() <<endl;
   TH1D *temp6 =data->Get("diPFjetptallJIDloose");
   cout << "Loose PFJets " << temp6->GetEntries() <<endl;
   TH1D *temp7 =data->Get("diPFjetptallJIDtight");
   cout << "Tight PFJets " << temp7->GetEntries() <<endl;  

   cout << "------ JPT Jets --------" <<endl;
   TH1D * temp8 =data->Get("diJPTjetptall");
   cout << "JPTJets       " << temp8->GetEntries() <<endl;
   TH1D *temp9 =data->Get("diJPTjetptallJIDloose");
   cout << "Loose JPTJets " << temp9->GetEntries() <<endl;
   TH1D *temp10 =data->Get("diJPTjetptallJIDtight");
   cout << "Tight JPTJets " << temp10->GetEntries() <<endl;
}


//-------------------------------------------------------------------------------------
void MoveStatsAndDraw (TH1 *data, TH1 *mc, char *dataTitle, char* mcTitle, int log){
  c->SetLogy(log);
  mc->SetTitle("");
  mc->Draw();
  data->Draw("sames");   
  gStyle->SetOptStat(00000000);  
  // gStyle->SetOptStat(1111111111); 
  TLegend *leg = new TLegend(0.35,0.79,0.75,0.89);
  leg->SetFillStyle(4000);
  leg->SetFillColor(kWhite);
  leg->SetBorderSize(0);
  leg->SetTextFont(42);
  leg->AddEntry(data,dataTitle);
  leg->AddEntry(mc,mcTitle);
  leg->Draw();
  c->Modified();
  c->Update();
 
}
 
