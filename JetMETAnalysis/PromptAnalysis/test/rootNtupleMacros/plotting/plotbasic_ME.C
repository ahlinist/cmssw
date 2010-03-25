using namespace std;
#include "tdrstyle.C"

void plotbasic_ME(){
   
  // specify the DATA file!!!
  // --------------------------------------------------------------------------
  TFile *data = TFile::Open("out.root");
  char *legDataTitle = "Data, jets 7 TeV";
  TString dataset="Data, cleaned jets 7 TeV"; 
  double rebin=1.;
  //---------------------------------------------------------------------------
  setTDRStyle();
  gROOT->ForceStyle();
  TIter next (data->GetListOfKeys ());
  TCanvas c("c","",600,600); 
 
  for (TKey* key = 0; (key = (TKey *) next());) {
     // Plot loose cleaned jets in one plot
    char histname_clean [256];
    sprintf (histname_clean, "%sJIDloose", key->GetName());
    cout << histname_clean<< endl;
    TH1D * h_data= data->Get(key->GetName());
    TH1D * h_data_clean= data->Get(histname_clean); 
    // only for filled histos
    
    if(( h_data->GetEntries()!=0)&&( h_data_clean)) { 
	h_data->SetYTitle("number of jets / bin");
	h_data->SetLineWidth(2);
	h_data->SetLineStyle(1);
	h_data_clean->SetLineStyle(2);
	h_data_clean->SetLineWidth(2);
	//	h_data->Rebin(rebin);
	//h_data_clean->Rebin(rebin);
 	dataset="Data, cleaned jets loose 7 TeV"; 
	std::cout << "Plotting: " << key->GetName()  <<  std::endl;
	MoveStatsAndDraw(h_data , h_data_clean, legDataTitle, dataset, 1);
	char histname [256];
	sprintf (histname, "Overlay%sLoose.gif", key->GetName());
	c.Print (histname);
    }//if Entries != 0 

    // Plot tight cleaned jets in one plot
    sprintf (histname_clean, "%sJItight", key->GetName());
    h_data_clean= data->Get(histname_clean); 
    // only for filled histos
    if(( h_data->GetEntries()!=0)&&( h_data_clean)) { 
	h_data->SetYTitle("number of jets / bin");
	h_data->SetLineWidth(2);
	h_data->SetLineStyle(1);
	h_data_clean->SetLineStyle(2);
	h_data_clean->SetLineWidth(2);
	//	h_data->Rebin(rebin);
	//h_data_clean->Rebin(rebin);
	dataset="Data, cleaned jets tight 7 TeV"; 
	std::cout << "Plotting: " << key->GetName()  <<  std::endl;
	MoveStatsAndDraw(h_data , h_data_clean, legDataTitle, dataset, 1);
	char histname [256];
	sprintf (histname, "Overlay%sTight.gif", key->GetName());
	c.Print (histname);
    }//if Entries != 0

    else { 
      if (h_data->GetEntries()!=0){
      	h_data->SetYTitle("number of jets / bin");
	h_data->SetLineWidth(2);
	h_data->SetLineStyle(1);
	//	h_data->Rebin(rebin);
	std::cout << "Plotting: " << key->GetName()  <<  std::endl;
	h_data->SetTitle("");
	h_data->Draw();
	char histname [256];
	sprintf (histname, "Simple_%s.gif", key->GetName());
	c.Print (histname);
      } 
    }
  } // loop over keys
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
