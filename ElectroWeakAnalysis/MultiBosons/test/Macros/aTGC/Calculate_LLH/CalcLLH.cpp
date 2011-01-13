#include <iostream>
#include <sstream>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "LLH.hpp"
#include "TMath.h"
#include <vector>
#include <fstream>
using namespace std;

TString mode;

string DoubleToStr(double num) {
  std::ostringstream ss;
  if (!(ss<<num))
    return "";
  return ss.str();
}

int main(int argc, char *argv[]){

  //  bool debug = true;
  bool debug = false;

  if (debug) cout << argv[1] << endl;
  mode = argv[1];
  if ( mode != "eeg" &&
       mode != "mmg" ) return -1;


  vector <string> List;
  ifstream FileList("../SortedOutput/" + mode + "/inputScaled.sorted");
  while (!(FileList.eof())){
    string temp;
    if (!(FileList>>temp)) break;
    List.push_back(temp);
    //cout << "File: " << temp << endl;
  }

  vector <double> coupling_1;
  vector <double> coupling_2;
  vector <double> XSec;
  vector <double> Scale;
  vector <double> XSecErr;
  vector <double> XSecNlo;
  vector <double> XSecNloErr;
  vector <double> Acc;
  vector <double> AccErr;
  vector <double> LIKELIHOOD;

  double Coup1, Coup2, Cross, Err,CrossNlo, ErrNlo, Coup1sec,Coup2sec,Scal=0,Accc, AccErrr=0;
  ifstream CrossSecList("../SortedOutput/" + mode + "/scaler_LO.sorted");

  while (!(CrossSecList.eof())) {

    if  (!(CrossSecList >> Coup1)) break;
    if  (!(CrossSecList >> Coup2)) break;
    if  (!(CrossSecList >> Cross)) break;
    if  (!(CrossSecList >> Err)) break;

    coupling_1.push_back(Coup1);
    coupling_2.push_back(Coup2);
    XSec.push_back(Cross);    
    XSecErr.push_back(Err);
    if (debug) cout << "Grid: " << Coup1 << " " << Coup2 << " " << Cross << endl;
  }

  ifstream AccList("../SortedOutput/" + mode + "/acceptances.sorted"); 
  
  while (!(AccList.eof())){
    if (!(AccList >> Coup1sec)) break;
    if (!(AccList >> Coup2sec)) break;
    if (!(AccList >> Accc)) break;
    if (!(AccList >> AccErrr)) break;
    
    //    cout << " ACC "<<Accc<<endl; 

    Acc.push_back(Accc);
    AccErr.push_back(AccErrr);
  }
  if (!((Coup1==Coup1sec)&&(Coup2==Coup2sec)))
    cout << "AHHHHH not same" << Coup1<< " " << Coup1sec <<" " << Coup2<<" " << Coup2sec << endl;

  double bkgerr = 0.12;
  // 10% on the uncertainty
    
  // Checks:
  for (int i=0; i<List.size(); ++i) 
    if (debug) cout << List[i] << " " << coupling_1[i] << " " << coupling_2[i] << endl;
  
  for (int i=0;i<List.size();++i){

    Float_t TotBkg =0,TotCand=0; 
    Float_t TotSig=0;    
    
    TFile *q = new TFile("../data_" + mode + ".root","READ");   // This is my data file
    TH1F* Data    = (TH1F*)q->Get("data_" + mode);
    TH1F* BkgHist = (TH1F*)q->Get("background_" + mode);

    int nBins = Data->GetXaxis()->GetNbins();

    vector <double> Sig;
    vector <double> Bkg;
    vector <int> Obs;
    Float_t EvSelErr=0;
    {  
      if (debug){
	cout << "Now in file: " << List[i] << endl;
      }
      //Rebin MC histogram
      TFile* a = new TFile(List[i].c_str(),"READ");   
      if (debug) cout << List[i].c_str() << endl;
      TH1F* SigHist = (TH1F*)a->Get("signal_Et");

      //      Float_t TempXSec = XSec[i];
      //      Float_t TempXSecNlo = 1.*XSec[i];
      //      Float_t TempXSecErr = XSecErr[i]; 
      //      Float_t TempXSecNloErr = 1.*XSecErr[i]; 
      //      Float_t TempXSecPDF = 0.05*XSec[i];
    
      //      TempXSecNloErr = TMath::Sqrt(TempXSecNloErr*TempXSecNloErr+TempXSecPDF*TempXSecPDF);
      
      //      Float_t TempAccE = Acc[i];
      //      Float_t TempAccErr = AccErr[i];
      //if (debug) cout << TempAccE <<" "<<TempAccErr<<endl;

      //      EvSelErr = TMath::Sqrt((TempAccErr/TempAccE)*(TempAccErr/TempAccE) + 
      //			     (TempXSecNloErr/TempXSecNlo)*(TempXSecNloErr/TempXSecNlo) + 0.15*0.15);
      EvSelErr = 0.05;

      if (debug) cout << "==========SIGNAL=========" << endl;
      Float_t TotSigEvt = SigHist->Integral();
      
      if (debug) cout << "TotSigEvt " << TotSigEvt << endl;

      if (debug) cout << "==========DATA===========" << endl;
      Float_t TotDataEvt = Data->Integral();
      if (debug) cout << "TotDataEvt " << TotDataEvt << endl;
      if (debug) cout << "=========================" << endl;

      for (int j=1;j<=nBins+1;++j){
	Sig.push_back(SigHist->GetBinContent(j));
	Bkg.push_back(BkgHist->GetBinContent(j));
	Obs.push_back((int)Data->GetBinContent(j));
	TotCand += Data->GetBinContent(j);
	TotBkg += BkgHist->GetBinContent(j);
	TotSig+=SigHist->GetBinContent(j);
      }
      a->Close();
      
    }
    if (debug) {
      cout << "Ready for like: " << endl;
      cout << "Input to like: " << endl;
      cout << "EvSelErr: " << EvSelErr << endl;
      cout << "bkgerr: " << bkgerr << endl;
    }
    for (int j=0;j<Obs.size();++j){
      if (debug) cout << "Signal: " << Sig[j] << "\t Bkg: " << Bkg[j] 
		      << "\t Obs: " << Obs[j] << "\t EvSelError: "
		      << EvSelErr <<"\t Bkg err: " <<bkgerr<< endl;
    }
    if (debug) cout << "Summed: " << TotSig << " " << TotBkg  << " " << TotCand<< endl;

    Float_t Lumi = 1.; // already normalised input!
    Float_t LumiErr = 0.11 * Lumi;
    bkgerr = 0.12;
    
    LogLikelihood llh(Sig, Bkg, Obs, EvSelErr , bkgerr, LumiErr, 1, 3, 51);
    
    double Like = llh.calculate();

    cout 
      << "Working on "<< coupling_1[i] << " " << coupling_2[i]
      << "\t Likelihood of point\t"  << Like
      << endl;


    //cout << "Likelihood of this point: " << Like << endl;
    LIKELIHOOD.push_back(Like);
    if (debug) cout << "\n\n" << endl;

  }

  
  ofstream OutFile(mode + ".out");
  for (int i=0;i<List.size();++i){
    OutFile << coupling_1[i] << " " << coupling_2[i] << " " << LIKELIHOOD[i] << endl;
  }
  return 0;

}
