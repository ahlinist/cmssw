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

double tauTriggerCorrectionForMuTau(double pt, double muTauLooseTau10Weight, double muTauLooseTau15Weight, double muTauLooseTau20Weight) {
    double muTauLooseTau10 = efficiency(pt, 16.7854, -0.69382, 4.57285, 94.1275, 0.883126);
    double muTauLooseTau15 = efficiency(pt, 14.6677, 0.408165, 0.551949, 1.4477, 0.961277);
    double muTauLooseTau20 = efficiency(pt, 19.1898, -1.35938, 2.82724, 1.02721, 1.50862);
    double muTauLooseTau15MC = efficiency(pt, 14.5311, -0.0151916, 0.0022261, 1.6761, 0.891203);
    double correction = (muTauLooseTau10*muTauLooseTau10Weight + muTauLooseTau15*muTauLooseTau15Weight + muTauLooseTau20*muTauLooseTau20Weight) / muTauLooseTau15MC;
    return correction;
}

void makeTauTriggerCorrectionForMuTau(double muTauLooseTau10Weight, double muTauLooseTau15Weight, double muTauLooseTau20Weight) {
  TH1F *hist= new TH1F("tauTriggerEfficiencyCorrectionForMuTau", "tauTriggerEfficiencyCorrectionForMuTau", 485, 15, 500);
  // set under flow bin to value for pt 15
  hist->SetBinContent(0,tauTriggerCorrectionForMuTau(15, muTauLooseTau10Weight, muTauLooseTau15Weight, muTauLooseTau20Weight));
  for(double pt =15; pt <= 500; pt++) {
    double correction = tauTriggerCorrectionForMuTau(pt, muTauLooseTau10Weight, muTauLooseTau15Weight, muTauLooseTau20Weight);
    hist->Fill(pt, correction);
  }
  TFile file("../data/tauTriggerEfficiencyCorrectionForMuTau.root","RECREATE");
  hist->Write();
  file.Close();
}

double tauTriggerCorrectionForElecTau(double pt, double elecTauLooseTau20Weight, double elecTauMediumTau20Weight, double elecTauTightTau20Weight) {
  double elecTauLooseTau20 = efficiency(pt, 19.6319, -0.986354, 1.94272, 1.02398, 1.91094);
  double elecTauMediumTau20 = efficiency(pt, 19.3535, 0.369967, 0.158178, 3.31129, 0.76279);
  double elecTauTightTau20 = efficiency(pt, 19.7197, 0.844386, 1.16726, 1.00747, 9.35089);
  double elecTauMediumTau20MC = efficiency(pt, 19.451, -0.0554166, 0.0518694, 1.24892, 0.950397);
  double correction = (elecTauLooseTau20*elecTauLooseTau20Weight + elecTauMediumTau20*elecTauMediumTau20Weight + elecTauTightTau20*elecTauTightTau20Weight) / elecTauMediumTau20MC;
  return correction;
}

void makeTauTriggerCorrectionForElecTau(double elecTauLooseTau20Weight, double elecTauMediumTau20Weight, double elecTauTightTau20Weight) {
  TH1F *hist= new TH1F("tauTriggerEfficiencyCorrectionForElecTau", "tauTriggerEfficiencyCorrectionForElecTau", 480, 20, 500);
  hist->SetBinContent(0,tauTriggerCorrectionForElecTau(20, elecTauLooseTau20Weight, elecTauMediumTau20Weight, elecTauTightTau20Weight));
  for(double pt =20; pt <= 500; pt++) {
    double correction = tauTriggerCorrectionForElecTau(pt, elecTauLooseTau20Weight, elecTauMediumTau20Weight, elecTauTightTau20Weight);
    hist->Fill(pt, correction);
  }
  TFile file("../data/tauTriggerEfficiencyCorrectionForElecTau.root","RECREATE");
  hist->Write();
  file.Close();
}

void makeEfficiencyCorrectionRootFile() {

  std::cout << "Making tau trigger efficiency correction for mu tau" << std::endl;
  double muTauLooseTau10Luminosity = 0.017;
  double muTauLooseTau15Luminosity = 1.97;
  double muTauLooseTau20Luminosity = 2.46;
  double totalLuminosity = muTauLooseTau10Luminosity + muTauLooseTau15Luminosity + muTauLooseTau20Luminosity;
 makeTauTriggerCorrectionForMuTau(muTauLooseTau10Luminosity/totalLuminosity, muTauLooseTau15Luminosity/totalLuminosity, muTauLooseTau20Luminosity/totalLuminosity);

  std::cout << "Making tau trigger efficiency correction for elec tau" << std::endl;
  double elecTauLooseTau20Luminosity = 1.1;
  double elecTauTightTau20Luminosity = 0.7;
  double elecTauMediumTau20Luminosity = 1.7 + 0.9 + 0.0023;
  double totalLuminosity = elecTauLooseTau20Luminosity + elecTauTightTau20Luminosity + elecTauMediumTau20Luminosity;  
  makeTauTriggerCorrectionForElecTau(elecTauLooseTau20Luminosity/totalLuminosity, elecTauTightTau20Luminosity/totalLuminosity, elecTauMediumTau20Luminosity/totalLuminosity);

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
