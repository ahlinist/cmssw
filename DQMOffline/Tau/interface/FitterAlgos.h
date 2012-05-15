#ifndef FitterAlgos_h
#define FitterAlgos_h

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooBreitWigner.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooMsgService.h"
#include "RooCBShape.h"
#include "RooFFTConvPdf.h"
#include "RooChebychev.h"
#include "TH1F.h"
#include "TMath.h"
#include <iostream>
#include <string>
using namespace std;
using namespace RooFit ;

struct fitinfo{
  fitinfo(double d,double e, double f){
    nsig = d;
    err = e;
    nllMin = f;
  }
  ~fitinfo(){
    //delete result;
  }
  double nsig,err,nllMin;
};

class FitterAlgoExpBW {

public:
  static fitinfo Fit(TH1F *h) {
    // Create observables
    double entries = 0;
    double entries80to120 = 0;
    int rmin = h->GetXaxis()->FindBin(80.);
    int rmax = h->GetXaxis()->FindBin(120.);
    for(int i = 1; i <= h->GetNbinsX(); i++){
      entries += h->GetBinContent(i);
      if(i >= rmin && i <= rmax)
	entries80to120 += h->GetBinContent(i);
    }
    //std::cout << h->GetTitle() << ": entries  " << entries << "   entries80to120  " << entries80to120 << "   " << h->GetTitle()<< std::endl;
    if(entries80to120==0){
      std::cout << "retutning nothing"<< std::endl;
      return fitinfo(0.,0.,-100000.);
    }

    double hmin = h->GetXaxis()->GetBinLowEdge(1);
    double hmax = h->GetXaxis()->GetBinLowEdge(h->GetNbinsX()) + h->GetXaxis()->GetBinWidth(h->GetNbinsX());

    RooRealVar nsig("nsig","nsig",entries80to120,0, entries); //MUST be called this way!
    RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
    //cout<<"init"<< endl;
    RooRealVar x("x","x",hmin,hmax);
    
    // Construct signal pdf
    //std::cout << "Construct signal pdf" << std::endl;
    RooRealVar mean("mean","mean",91.188); //PDG Value
    RooRealVar width("width","width",2.49,2.49,20); //At least larger than pdg value
    RooBreitWigner bw("bw","bw",x,mean,width);

    // Construct background pdf
    //std::cout << "background pdf"<< std::endl;
    RooRealVar c0("c0","c0",0.,-1000.,0.) ; //Absolutely no idea!
    RooExponential exp("exp","exp",x,c0) ;

    // Construct composite pdf
    RooRealVar nbkg("nbkg","nbkg", entries - entries80to120, 0., entries);
    RooAddPdf model("model","model",RooArgList(bw,exp),RooArgList(nsig,nbkg)) ; 

    RooDataHist data("hdataset","hdataset", RooArgList(x),h);

    RooFitResult* result = model.fitTo(data,Extended(),Save(),Minos(false),Verbose(false),PrintLevel(-1),PrintEvalErrors(-1),Warnings(false));//we don't care about asymmetric errors, we want it fast! (and silent, a ninja :P)

    /*/USED FOR DEBUGGING
      RooPlot* plot = x.frame();
    plot->GetYaxis()->SetTitleOffset(1.1);
    plot->GetXaxis()->SetTitleOffset(1.1);
    data.plotOn(plot);
    model.plotOn(plot);

    TCanvas * can = new TCanvas("can", "can", 600,500);    
    string png(h->GetTitle());
    plot->Draw();
    //h->SetMarkerStyle(20);
    //h->Draw();
    //can->SetLogy();
    can->Print(("dbgFit/"+png+"_rooplot.png").c_str());
    delete can; //   */

    return fitinfo(nsig.getVal(),nsig.getError(),result->minNll());
  }
};

class NoFit {

public:
  static fitinfo Fit(const TH1F *h) {
    // C r e a t e   m o d e l   f o r   p h y s i c s   s a m p l e
    // -------------------------------------------------------------

    // Create observables
    double entries = h->GetEntries();
    RooRealVar nsig("nsig","nsig",entries);
    nsig.setVal(entries);
    nsig.setError(TMath::Sqrt(entries));

    double chi2 = 0.; //Always accept state!
    double ndof = 1;
    return fitinfo(nsig.getVal(),nsig.getError(),-100000);
  }
};

#endif
