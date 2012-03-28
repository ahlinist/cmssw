#include <vector>
#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include "TFile.h"
#include <string>

double efficiency(double m, double m0, double sigma, double alpha,
     double n, double norm)
{
  const double sqrtPiOver2 = 1.2533141373;
  const double sqrt2 = 1.4142135624;
  double sig = fabs((double) sigma);
  double t = (m - m0)/sig;
  if(alpha < 0)
    t = -t;
  double absAlpha = fabs(alpha/sig);
  double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
  double b = absAlpha - n/absAlpha;
  double ApproxErf;
  double arg = absAlpha / sqrt2;
  if (arg > 5.) ApproxErf = 1;
  else if (arg < -5.) ApproxErf = -1;
  else ApproxErf = TMath::Erf(arg);
  double leftArea = (1 + ApproxErf) * sqrtPiOver2;
  double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
  double area = leftArea + rightArea;
  if( t <= absAlpha ){
    arg = t / sqrt2;
    if(arg > 5.) ApproxErf = 1;
    else if (arg < -5.) ApproxErf = -1;
    else ApproxErf = TMath::Erf(arg);
    return norm * (1 + ApproxErf) * sqrtPiOver2 / area;
  }
  else{
    return norm * (leftArea + a * (1/TMath::Power(t-b,n-1) -
          1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area;
  }
}

struct TriggerEfficiencyParametersStruct {
  double m0;
  double sigma;
  double alpha;
  double n;
  double norm;
  double luminosity;
};

typedef struct TriggerEfficiencyParametersStruct TriggerEfficiencyParameters;


TriggerEfficiencyParameters makeTriggerEfficiencyParameters(double m0, double sigma, double alpha, double n, double norm, double luminosity) {
  TriggerEfficiencyParameters parameters;
  parameters.m0 = m0;
  parameters.m0 = m0;
  parameters.sigma = sigma;
  parameters.alpha = alpha;
  parameters.n = n;
  parameters.norm = norm;
  parameters.luminosity = luminosity;
  return parameters;
}

double triggerCorrection(double m, TriggerEfficiencyParameters dataParameters, TriggerEfficiencyParameters mcParameters) {
  double dataEfficiency = efficiency(m, dataParameters.m0, dataParameters.sigma, dataParameters.alpha, dataParameters.n, dataParameters.norm);
  double mcEfficiency = efficiency(m, mcParameters.m0, mcParameters.sigma, mcParameters.alpha, mcParameters.n, mcParameters.norm);
  return dataEfficiency / mcEfficiency;
}

void makeTriggerEfficiencyCorrections(std::string name, 
                                      int numberOfBins,
                                      double minValue,
                                      double maxValue,
                                      TriggerEfficiencyParameters mcTriggerEfficiencyParameters, 
                                      std::vector<TriggerEfficiencyParameters> dataTriggerEfficiencyParameters) {
  TH1F *hist = new TH1F(name.c_str(), name.c_str(), numberOfBins, minValue, maxValue);
  double totalLuminosity = 0;
  for(unsigned int i=0; i!=dataTriggerEfficiencyParameters.size(); i++) {
    totalLuminosity += dataTriggerEfficiencyParameters[i].luminosity;
  }
  for(double m=minValue; m<=maxValue; m+= (maxValue-minValue)/numberOfBins) {
    double correction = 0;
    for(unsigned int i=0; i!=dataTriggerEfficiencyParameters.size(); i++) {
      correction += triggerCorrection(m,dataTriggerEfficiencyParameters[i], mcTriggerEfficiencyParameters)*dataTriggerEfficiencyParameters[i].luminosity/totalLuminosity;
    }
    if(m == minValue)
      hist->SetBinContent(0, correction);
    hist->Fill(m, correction);
//    std::cout << "pt: " << m << " correction: " << correction << std::endl;
  }
  TFile file(std::string("../data/"+name+".root").c_str(),"RECREATE");
  hist->Write();
  file.Close();
}

void makeTriggerEfficiencyCorrections2D(std::string name, 
        int numberOfBins,
        double minValue,
        double maxValue,
        TriggerEfficiencyParameters mcTriggerEfficiencyParametersEB, 
        TriggerEfficiencyParameters mcTriggerEfficiencyParametersEE, 
        std::vector<TriggerEfficiencyParameters> dataTriggerEfficiencyParametersEB,
        std::vector<TriggerEfficiencyParameters> dataTriggerEfficiencyParametersEE) {
    
    TH2F *hist = new TH2F(name.c_str(), name.c_str(), 4,-1.479*2,1.479*2,numberOfBins, minValue, maxValue);
    
    double totalLuminosity = 0;
    for(unsigned int i=0; i!=dataTriggerEfficiencyParametersEB.size(); i++) {
        totalLuminosity += dataTriggerEfficiencyParametersEB[i].luminosity;
    }
    
    for(double m=minValue; m<=maxValue; m+= (maxValue-minValue)/numberOfBins) {
        double correctionEB = 0;
        for(unsigned int i=0; i!=dataTriggerEfficiencyParametersEB.size(); i++) {
            correctionEB += triggerCorrection(m,dataTriggerEfficiencyParametersEB[i], mcTriggerEfficiencyParametersEB)*dataTriggerEfficiencyParametersEB[i].luminosity/totalLuminosity;
        }
        double correctionEE = 0;
        for(unsigned int i=0; i!=dataTriggerEfficiencyParametersEE.size(); i++) {
            correctionEE += triggerCorrection(m,dataTriggerEfficiencyParametersEE[i], mcTriggerEfficiencyParametersEE)*dataTriggerEfficiencyParametersEE[i].luminosity/totalLuminosity;
        }
        if(m == minValue) {
            hist->SetBinContent(1,0,correctionEE);
            hist->SetBinContent(2,0,correctionEB);
            hist->SetBinContent(3,0,correctionEB);
            hist->SetBinContent(4,0,correctionEE);
        }
        hist->Fill(-2,m,correctionEE);
        hist->Fill(-1,m,correctionEB);
        hist->Fill(1,m,correctionEB);
        hist->Fill(2,m,correctionEE);
        //    std::cout << "pt: " << m << " correction: " << correction << std::endl;
    }
    TFile file(std::string("../data/"+name+".root").c_str(),"RECREATE");
    hist->Write();
    file.Close();
}

void makeEfficiencyCorrectionRootFile() {

  std::cout << "Making tau trigger efficiency correction for mu tau" << std::endl;
  std::vector<TriggerEfficiencyParameters> muTauParameters;
  // muTauLooseTau10
  muTauParameters.push_back(makeTriggerEfficiencyParameters(16.7854, -0.69382, 4.57285, 94.1275, 0.883126, 0.017));
  muTauParameters.push_back(makeTriggerEfficiencyParameters(14.6677, 0.408165, 0.551949, 1.4477, 0.961277, 1.97));
  muTauParameters.push_back(makeTriggerEfficiencyParameters(19.1898, -1.35938, 2.82724, 1.02721, 1.50862, 2.46));

  TriggerEfficiencyParameters muTauLooseTau15MCParameters = 
    makeTriggerEfficiencyParameters(14.5311, -0.0151916, 0.0022261, 1.6761, 0.891203, 0);
  makeTriggerEfficiencyCorrections("tauTriggerEfficiencyCorrectionForMuTau", 485, 15, 500, muTauLooseTau15MCParameters, muTauParameters);

  std::cout << "Making tau trigger efficiency correction for elec tau" << std::endl;
  std::vector<TriggerEfficiencyParameters> elecTauParametersEB;
  // elecTauLoose20
  elecTauParametersEB.push_back(makeTriggerEfficiencyParameters(19.3916, 0.996964, 1.70131, 1.38002, 0.903245, 1.1));
  // elecTauMedium20
  elecTauParametersEB.push_back(makeTriggerEfficiencyParameters(19.5667, 1.15203, 1.68126, 1.40025, 0.848033, 0.7));
  // elecTauTight20
  elecTauParametersEB.push_back(makeTriggerEfficiencyParameters(19.6013, 0.987317, 1.08015, 1.88592, 0.776894, 1.7 + 0.9 + 0.0023));
  TriggerEfficiencyParameters elecTauMediumTau20MCParametersEB = 
    makeTriggerEfficiencyParameters(19.468, 0.0615381, 0.0349325, 1.59349, 0.860096, 0);
  std::vector<TriggerEfficiencyParameters> elecTauParametersEE;
  // elecTauLoose20
  elecTauParametersEE.push_back(makeTriggerEfficiencyParameters(18.8166, 0.526632, 0.20666, 6.80392, 0.903245, 1.1));
  // elecTauMedium20
  elecTauParametersEE.push_back(makeTriggerEfficiencyParameters(18.8476, 0.528963, 0.16717, 3.65814, 0.749759, 0.7));
  // elecTauTight20
  elecTauParametersEE.push_back(makeTriggerEfficiencyParameters(18.8859, 0.271301, 0.128008, 1.50993, 0.825122, 1.7 + 0.9 + 0.0023));
  TriggerEfficiencyParameters elecTauMediumTau20MCParametersEE = 
    makeTriggerEfficiencyParameters(19.3862, 0.247148, 0.123187, 2.87108, 0.790894, 0);
  makeTriggerEfficiencyCorrections2D("tauTriggerEfficiencyCorrectionForElecTau", 480, 20, 500, 
          elecTauMediumTau20MCParametersEB, elecTauMediumTau20MCParametersEE, elecTauParametersEB, elecTauParametersEE);
/*
  std::cout << "Making electron trigger efficiency correction for elec tau" << std::endl;
  std::vector<TriggerEfficiencyParameters> elecTauParameters;
  // elecTauLoose20
  electronParameters.push_back(makeTriggerEfficiencyParameters(19.6319, -0.986354, 1.94272, 1.02398, 1.91094, 1.1));
  // elecTauMedium20
  electronParameters.push_back(makeTriggerEfficiencyParameters(19.3535, 0.369967, 0.158178, 3.31129, 0.76279, 0.7));
  // elecTauTight20
  electronParameters.push_back(makeTriggerEfficiencyParameters(19.7197, 0.844386, 1.16726, 1.00747, 9.35089, 1.7 + 0.9 + 0.0023));
  TriggerEfficiencyParameters electron18MCParameters = 
    makeTriggerEfficiencyParameters(19.451, -0.0554166, 0.0518694, 1.24892, 0.950397, 0);
  makeTriggerEfficiencyCorrections("electronTriggerEfficiencyCorrection", 480, 20, 500, electron18MCParameters, electronParameters);
*/

    // numbers from AN-11-390 v6, Table 13

  // do electron isolation/ID efficiency corrections

  std::cout << "Making electron isolation efficiency correction for elec tau" << std::endl;
    TH2F* elecIsoCorr = new TH2F("electronIsolationEfficiencyCorrection","electronIsolationEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    elecIsoCorr->SetBinContent(1,1,1.19); // EE-, 10 < Pt < 15
    elecIsoCorr->SetBinContent(1,2,1.06); // EE-, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(2,1,1.11); // EB-, 10 < Pt < 15
    elecIsoCorr->SetBinContent(2,2,0.98); // EB-, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(3,1,1.11); // EB+, 10 < Pt < 15
    elecIsoCorr->SetBinContent(3,2,0.98); // EB+, 15 < Pt < 20
    
    elecIsoCorr->SetBinContent(4,1,1.19); // EE+, 10 < Pt < 15
    elecIsoCorr->SetBinContent(4,2,1.06); // EE+, 15 < Pt < 20
    
    for ( int i = 3; i <= 500; i++ ) {
        elecIsoCorr->SetBinContent(1,i,1.05); // EE-, 20 < Pt 
        elecIsoCorr->SetBinContent(2,i,0.99); // EB-, 20 < Pt 
        elecIsoCorr->SetBinContent(3,i,0.99); // EB+, 20 < Pt 
        elecIsoCorr->SetBinContent(4,i,1.05); // EE+, 20 < Pt 
    }

    TFile elecIsoFile("../data/electronIsolationEfficiencyCorrection.root","RECREATE");
    elecIsoCorr->Write();
    elecIsoFile.Close();


    // do electron trigger efficiency corrections

  std::cout << "Making electron trigger efficiency correction for elec tau" << std::endl;
    TH2F* elecTriggCorr = new TH2F("electronTriggerEfficiencyCorrection","electronTriggerEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    elecTriggCorr->SetBinContent(1,1,0.97); // EE-, 10 < Pt < 15
    elecTriggCorr->SetBinContent(1,2,1.05); // EE-, 15 < Pt < 20
    elecTriggCorr->SetBinContent(1,3,1.00); // EE-, 20 < Pt < 25
    elecTriggCorr->SetBinContent(1,4,1.00); // EE-, 25 < Pt < 30
    
    elecTriggCorr->SetBinContent(2,1,0.98); // EB-, 10 < Pt < 15
    elecTriggCorr->SetBinContent(2,2,1.00); // EB-, 15 < Pt < 20
    elecTriggCorr->SetBinContent(2,3,1.001); // EB-, 20 < Pt < 25
    elecTriggCorr->SetBinContent(2,4,1.001); // EB-, 25 < Pt < 30
    
    elecTriggCorr->SetBinContent(3,1,0.98); // EB+, 10 < Pt < 15
    elecTriggCorr->SetBinContent(3,2,1.00); // EB+, 15 < Pt < 20
    elecTriggCorr->SetBinContent(3,3,1.001); // EB+, 20 < Pt < 25
    elecTriggCorr->SetBinContent(3,4,1.001); // EB+, 25 < Pt < 30

    elecTriggCorr->SetBinContent(4,1,0.97); // EE+, 10 < Pt < 15
    elecTriggCorr->SetBinContent(4,2,1.05); // EE+, 15 < Pt < 20
    elecTriggCorr->SetBinContent(4,3,1.00); // EE+, 20 < Pt < 25
    elecTriggCorr->SetBinContent(4,4,1.00); // EE+, 25 < Pt < 30
    
    for ( int i = 5; i <= 500; i++ ) {
        elecTriggCorr->SetBinContent(1,i,1.008); // EE-, 30 < Pt 
        elecTriggCorr->SetBinContent(2,i,1.003); // EB-, 30 < Pt 
        elecTriggCorr->SetBinContent(3,i,1.003); // EB+, 30 < Pt 
        elecTriggCorr->SetBinContent(4,i,1.008); // EE+, 30 < Pt 
    }

    TFile elecTrigFile("../data/electronTriggerEfficiencyCorrection.root","RECREATE");
    elecTriggCorr->Write();
    elecTrigFile.Close();



    // do muon isolation/ID efficiency corrections

  std::cout << "Making muon isolation efficiency correction for mu tau" << std::endl;
    TH2F* muIsoCorr = new TH2F("muonIsolationEfficiencyCorrection","muonIsolationEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    muIsoCorr->SetBinContent(1,1,0.98); // EE-, 10 < Pt < 15
    muIsoCorr->SetBinContent(1,2,0.962); // EE-, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(2,1,0.92); // EB-, 10 < Pt < 15
    muIsoCorr->SetBinContent(2,2,0.948); // EB-, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(3,1,0.92); // EB+, 10 < Pt < 15
    muIsoCorr->SetBinContent(3,2,0.948); // EB+, 15 < Pt < 20
    
    muIsoCorr->SetBinContent(4,1,0.98); // EE+, 10 < Pt < 15
    muIsoCorr->SetBinContent(4,2,0.962); // EE+, 15 < Pt < 20
    
    for ( int i = 3; i <= 500; i++ ) {
        muIsoCorr->SetBinContent(1,i,0.9982); // EE-, 20 < Pt 
        muIsoCorr->SetBinContent(2,i,0.9932); // EB-, 20 < Pt 
        muIsoCorr->SetBinContent(3,i,0.9932); // EB+, 20 < Pt 
        muIsoCorr->SetBinContent(4,i,0.9982); // EE+, 20 < Pt 
    }

    TFile muIsoFile("../data/muonIsolationEfficiencyCorrection.root","RECREATE");
    muIsoCorr->Write();
    muIsoFile.Close();


    // do muon trigger efficiency corrections

  std::cout << "Making muon trigger efficiency correction for mu tau" << std::endl;
    TH2F* muTrigCorr = new TH2F("muonTriggerEfficiencyCorrection","muonTriggerEfficiencyCorrection",4,-1.479*2,1.479*2,98,10,500);
    
    muTrigCorr->SetBinContent(1,1,1.03); // EE-, 10 < Pt < 15
    muTrigCorr->SetBinContent(1,2,1.07); // EE-, 15 < Pt < 20
    muTrigCorr->SetBinContent(1,3,1.04); // EE-, 20 < Pt < 25
    muTrigCorr->SetBinContent(1,4,1.04); // EE-, 25 < Pt < 30
    
    muTrigCorr->SetBinContent(2,1,1.01); // EB-, 10 < Pt < 15
    muTrigCorr->SetBinContent(2,2,0.99); // EB-, 15 < Pt < 20
    muTrigCorr->SetBinContent(2,3,0.99); // EB-, 20 < Pt < 25
    muTrigCorr->SetBinContent(2,4,0.99); // EB-, 25 < Pt < 30
    
    muTrigCorr->SetBinContent(3,1,1.01); // EB+, 10 < Pt < 15
    muTrigCorr->SetBinContent(3,2,0.99); // EB+, 15 < Pt < 20
    muTrigCorr->SetBinContent(3,3,0.99); // EB+, 20 < Pt < 25
    muTrigCorr->SetBinContent(3,4,0.99); // EB+, 25 < Pt < 30

    muTrigCorr->SetBinContent(4,1,1.03); // EE+, 10 < Pt < 15
    muTrigCorr->SetBinContent(4,2,1.07); // EE+, 15 < Pt < 20
    muTrigCorr->SetBinContent(4,3,1.04); // EE+, 20 < Pt < 25
    muTrigCorr->SetBinContent(4,4,1.04); // EE+, 25 < Pt < 30
    
    for ( int i = 5; i <= 500; i++ ) {
        muTrigCorr->SetBinContent(1,i,1.06); // EE-, 30 < Pt 
        muTrigCorr->SetBinContent(2,i,0.992); // EB-, 30 < Pt 
        muTrigCorr->SetBinContent(3,i,0.992); // EB+, 30 < Pt 
        muTrigCorr->SetBinContent(4,i,1.06); // EE+, 30 < Pt 
    }

    TFile muTrigFile("../data/muonTriggerEfficiencyCorrection.root","RECREATE");
    muTrigCorr->Write();
    muTrigFile.Close();


}
