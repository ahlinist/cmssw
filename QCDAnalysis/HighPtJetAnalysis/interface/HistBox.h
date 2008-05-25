#ifndef __HIST_BOX__
#define __HIST_BOX__

#include <TFile.h>
#include <TH1D.h>
#include <vector>
#include <string>
#include <TLorentzVector.h>

class EventHistBox{
 public:
  EventHistBox(){}
  ~EventHistBox(){}
  void initHistos(const std::vector<double> &ptMins);
  void write(TFile *file);
  TH1D *h_ptHat,*h_weight,*h_xSection,*h_ptHatStat;
 private:
  std::vector<TH1*> theHistos;
};

class ROOTContextSentinel {
public:
  ROOTContextSentinel() : dir(gDirectory), file(gFile) {}
  ~ROOTContextSentinel() { gDirectory = dir; gFile = file; }

private:
  TDirectory	*dir;
  TFile		*file;
};

class AlgoHistBox{
 public:
  AlgoHistBox(){}
  ~AlgoHistBox(){}
  void initHistos(const std::string &name,const std::vector<double> &ptBins,const std::vector<double> &rapBins,const double &ptMin);
  void fill(const TLorentzVector &lVEc,double weight);
  void write(TFile *file);
 private:
  void normToWidth(TH1D *histo);
  TH1D *h_pt_tot,*h_phi_tot,*h_rap_tot;
  std::vector<TH1D*> v_pt_rap, v_phi_rap;
  std::string theName;
  std::vector<TH1*> theHistos;
  std::vector<double> _rapBins,_rapBinsWidth,_ptBins,_ptBinsWidth;
  double _ptMin;
};



#endif
