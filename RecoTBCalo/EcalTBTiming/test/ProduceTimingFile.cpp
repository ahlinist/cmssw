// c++ -o ProduceTimingFile.exe `root-config --libs` `root-config --cflags` ProduceTimingFile.cpp

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>

#include<vector>
#include<string>


//! ROOT includes
#include "TROOT.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TFile.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "TPad.h"
#include "TLegend.h"
#include "TMath.h"
#include "TF1.h"

using namespace std;

int main(int argc,  char * argv[]){
  if(argc < 3){cout<<" Usage: executable input_file.root outfile.txt"<<endl; return -4;}
  int sm_ = 18;
  TFile f(argv[1]);
  TProfile* pr = 0;
  pr = (TProfile*) f.Get("rel_timing_prof_conv_blu");
  if( !pr ){cout<<" timing profile not found in the root file"<<endl; return -1;}
  
  float time[1701]; for(int i=0;i<1701;i++){time[i]=-100;}
  float half_mean  = 0;

  for(int ch=1;ch<1701;ch++){ 
    if ( ch<101 || (ch-1)%20 > 9){half_mean=137.4/25.;}// half1
    else {half_mean=137.1/25.;}
    time[ch] = pr->GetBinContent(ch)+half_mean;
    if( fabs(pr->GetBinContent(ch))>0.2 || pr->GetBinEntries(ch)<500 ){
      cout<<"cry: "<<ch<<" entries: "<<pr->GetBinEntries(ch)<<" rel timing: "<<pr->GetBinContent(ch)<<endl;
    }
  }
  f.Close();
  
  ofstream txt_channels;
  txt_channels.open(argv[2],ios::out);
  for(int i=1;i<1701;i++){
    txt_channels <<sm_<<"   "<<setw(4)<<i<<" \t "<<setw(8)<<setprecision(7)<<time[i]<< endl;
  }
  txt_channels.close();
  
  return 0;
}
