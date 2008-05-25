#include "QCDAnalysis/HighPtJetAnalysis/interface/HistBox.h"
#include <TLorentzVector.h>
#include <sstream>

using namespace std;

void AlgoHistBox::initHistos(const std::string &name,const std::vector<double> &ptBins,const std::vector<double> &rapBins,const double &ptMin){
  _ptMin=ptMin;
  theName=name;
  //get pt and rap bins, make ROOT happy... 
  unsigned int nPts=ptBins.size();
  double *RptBins=new double[nPts];
  for (unsigned int i=0;i<nPts;++i){
    RptBins[i]=ptBins[i];
  }
  double last=0;
  _ptBins=ptBins;
  _rapBins=rapBins;
  for (vector<double>::const_iterator iter=rapBins.begin();iter!=rapBins.end();++iter){
    double width=(*iter)*2-2*last;
    last=*iter;
    _rapBinsWidth.push_back(width);
  }
  last=0;
  for (vector<double>::const_iterator iter=ptBins.begin();iter!=ptBins.end();++iter){
    if (*iter==0) continue;
    double width=(*iter)-last;
    last=*iter;
    _ptBinsWidth.push_back(width);
  }
  

  
  string text=(name+"pt_tot");
  h_pt_tot=new TH1D(text.c_str(),text.c_str(),4000,0,4000);
  theHistos.push_back(h_pt_tot);
  
  text=name+"phi_tot";
  h_phi_tot=new TH1D(text.c_str(),text.c_str(),256,-M_PI,M_PI);
  theHistos.push_back(h_phi_tot);
  text=name+"rap_tot";
  h_rap_tot=new TH1D(text.c_str(),text.c_str(),1000,-8,+8);
  theHistos.push_back(h_rap_tot);
  
  unsigned int nRaps=rapBins.size();
  for (unsigned int i=0;i<nRaps;++i){
    ostringstream os;
    os.str("");
    os<<name<<"pt_rap_"<<i;
    TH1D *tmpHist1=new TH1D(os.str().c_str(),os.str().c_str(),nPts-1,RptBins);
    v_pt_rap.push_back(tmpHist1);
    theHistos.push_back(tmpHist1);
    os.str("");
    os<<name<<"phi_rap_"<<i;
    TH1D *tmpHist2=new TH1D(os.str().c_str(),os.str().c_str(),nPts-1,RptBins);
    v_phi_rap.push_back(tmpHist2);
    theHistos.push_back(tmpHist2);
  }
  
  //Directory management, and errors:
  for (vector<TH1*>::iterator iter=theHistos.begin();iter!=theHistos.end();++iter){
    (*iter)->SetDirectory(0);
    (*iter)->Sumw2();
  }

  delete [] RptBins;

}



void AlgoHistBox::write(TFile *file){
  ROOTContextSentinel c;
  file->cd();
  //normalize Histos to binWidth and Rapidity:
  normToWidth(h_pt_tot);
  unsigned int counter=0;
  for (vector<TH1D*>::iterator iter=v_pt_rap.begin();iter!=v_pt_rap.end();++iter){
    normToWidth(*iter);
    (*iter)->Scale(1/_rapBinsWidth.at(counter));
    ++counter;
  }
  for (vector<TH1*>::iterator iter=theHistos.begin();iter!=theHistos.end();++iter){
    (*iter)->Write();
  }
}

void AlgoHistBox::normToWidth(TH1D *histo){
  unsigned int nbins=histo->GetNbinsX();
  for (unsigned int i=1;i<=nbins;++i){
    double width=histo->GetBinWidth(i);
    double value=histo->GetBinContent(i);
    double error=histo->GetBinError(i);
    histo->SetBinContent(i,value/width);
    // Be Aware: is this what we want?
    histo->SetBinError(i,error/width);
  }
  
}

void AlgoHistBox::fill(const TLorentzVector &jet,double weight){
  if (jet.Pt()<_ptMin) return;

  h_pt_tot->Fill(jet.Pt(),weight);
  h_phi_tot->Fill(jet.Phi(),weight);
  h_rap_tot->Fill(jet.Phi(),weight);
  
  for (unsigned int i=0;i<_rapBins.size();++i){
    if (fabs(jet.Rapidity())<_rapBins.at(i)) {
      v_pt_rap.at(i)->Fill(jet.Pt(),weight);
      v_phi_rap.at(i)->Fill(jet.Phi(),weight);
      break;
    }
  }
}


void EventHistBox::initHistos(const vector<double> &v_ptHatMin){
  string text("Event.");
  h_ptHat=new TH1D((text+"ptHat").c_str(),(text+"ptHat").c_str(),1000,0,7000);
  theHistos.push_back(h_ptHat);
  h_weight=new TH1D((text+"weight").c_str(),(text+"weight").c_str(),1000,0,1000);
  theHistos.push_back(h_weight);
  h_xSection=new TH1D((text+"xSection").c_str(),(text+"xSection").c_str(),1000,0,1E10);
  theHistos.push_back(h_xSection);

  unsigned int nPtHats=v_ptHatMin.size();
  double *pHatMins=new double[nPtHats];
  for (unsigned int i=0;i<nPtHats;++i){
    pHatMins[i]=v_ptHatMin.at(i);
  }
  h_ptHatStat=new TH1D((text+"ptHatMin").c_str(),(text+"ptHatMin").c_str(),nPtHats-1,pHatMins);
  theHistos.push_back(h_ptHatStat);
  
  for (vector<TH1*>::iterator iter=theHistos.begin();iter!=theHistos.end();++iter){
    (*iter)->SetDirectory(0);
    (*iter)->Sumw2();
  }
  
}


void EventHistBox::write(TFile *file){
  ROOTContextSentinel c;
  file->cd();
  for (vector<TH1*>::iterator iter=theHistos.begin();iter!=theHistos.end();++iter){
    (*iter)->Write();
  }
}
