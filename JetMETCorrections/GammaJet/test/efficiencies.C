// List of signal bins and their cross sections
#include <map>
#include <string>
map<string, double> signal;
TFile *signalfile[7];
TFile *bkgfile[18];
 
TH1D* pt_true = new TH1D("pt_true","pt_true no cut",40,0.,2000.);                            pt_true->Sumw2();         
TH1D* pt_true_pt = new TH1D("pt_true_pt","pt_true pt cut",40,0.,2000.);		             pt_true_pt->Sumw2();      
TH1D* pt_true_eta = new TH1D("pt_true_eta","pt_true eta cut",40,0.,2000.);		     pt_true_eta->Sumw2();     
TH1D* pt_true_ntrk = new TH1D("pt_true_ntrk","pt_true ntrk cut ",40,0.,2000.);		     pt_true_ntrk->Sumw2();    
TH1D* pt_true_ptiso = new TH1D("pt_true_ptiso","pt_true ptiso cut",40,0.,2000.);	     pt_true_ptiso->Sumw2();   
TH1D* pt_true_emf = new TH1D("pt_true_emf","pt_true emf cut",40,0.,2000.);		     pt_true_emf->Sumw2();     
TH1D* pt_true_smaj = new TH1D("pt_true_smaj","pt_true smaj cut",40,0.,2000.);		     pt_true_smaj->Sumw2();    
TH1D* pt_true_smin = new TH1D("pt_true_smin","pt_true smin cut",40,0.,2000.);		     pt_true_smin->Sumw2();    
TH1D* pt_true_ecaliso = new TH1D("pt_true_ecaliso","pt_true ecaliso cut",40,0.,2000.);	     pt_true_ecaliso->Sumw2(); 
TH1D* pt_true_etajet = new TH1D("pt_true_etajet","pt_true eta jet cut",40,0.,2000.);	     pt_true_etajet->Sumw2();  
TH1D* pt_true_deltaphi = new TH1D("pt_true_deltaphi","pt_true deltaphi cut",40,0.,2000.);    pt_true_deltaphi->Sumw2();
TH1D* pt_true_2ndjet = new TH1D("pt_true_2ndjet","pt_true 2ndjet cut",40,0.,2000.);	     pt_true_2ndjet->Sumw2();  
TH1D* pt_true_deltaeta = new TH1D("pt_true_deltaeta","pt_true deltaeta cut",40,0.,2000.);    pt_true_deltaeta->Sumw2();

TH1D* pt_true_eta_int = new TH1D("pt_true_eta_int","pt_true eta cut cascade",40,0.,2000.);                   pt_true_eta_int->Sumw2();     
TH1D* pt_true_ntrk_int = new TH1D("pt_true_ntrk_int","pt_true ntrk cut cascade ",40,0.,2000.);		     pt_true_ntrk_int->Sumw2();    
TH1D* pt_true_ptiso_int = new TH1D("pt_true_ptiso_int","pt_true ptiso cut cascade",40,0.,2000.);	     pt_true_ptiso_int->Sumw2();   
TH1D* pt_true_emf_int = new TH1D("pt_true_emf_int","pt_true emf cut cascade",40,0.,2000.);		     pt_true_emf_int->Sumw2();     
TH1D* pt_true_smaj_int = new TH1D("pt_true_smaj_int","pt_true smaj cut cascade",40,0.,2000.);		     pt_true_smaj_int->Sumw2();    
TH1D* pt_true_smin_int = new TH1D("pt_true_smin_int","pt_true smin cut cascade",40,0.,2000.);		     pt_true_smin_int->Sumw2();    
TH1D* pt_true_ecaliso_int = new TH1D("pt_true_ecaliso_int","pt_true ecaliso cut cascade",40,0.,2000.);	     pt_true_ecaliso_int->Sumw2(); 
TH1D* pt_true_etajet_int = new TH1D("pt_true_etajet_int","pt_true eta jet cut cascade",40,0.,2000.);	     pt_true_etajet_int->Sumw2();  
TH1D* pt_true_deltaphi_int = new TH1D("pt_true_deltaphi_int","pt_true deltaphi cut cascade",40,0.,2000.);    pt_true_deltaphi_int->Sumw2();
TH1D* pt_true_2ndjet_int = new TH1D("pt_true_2ndjet_int","pt_true 2ndjet cut cascade",40,0.,2000.);	     pt_true_2ndjet_int->Sumw2();  
TH1D* pt_true_deltaeta_int = new TH1D("pt_true_deltaeta_int","pt_true deltaeta cut cascade",40,0.,2000.);    pt_true_deltaeta_int->Sumw2();

TH1D* eff_pt_true = new TH1D("eff_pt_true","eff_pt_true no cut",40,0.,2000.);
TH1D* eff_pt_true_pt = new TH1D("eff_pt_true_pt","eff_pt_true pt cut",40,0.,2000.);
TH1D* eff_pt_true_eta = new TH1D("eff_pt_true_eta","eff_pt_true eta cut",40,0.,2000.);
TH1D* eff_pt_true_ntrk = new TH1D("eff_pt_true_ntrk","eff_pt_true ntrk cut ",40,0.,2000.);
TH1D* eff_pt_true_ptiso = new TH1D("eff_pt_true_ptiso","eff_pt_true ptiso cut",40,0.,2000.);
TH1D* eff_pt_true_emf = new TH1D("eff_pt_true_emf","eff_pt_true emf cut",40,0.,2000.);
TH1D* eff_pt_true_smaj = new TH1D("eff_pt_true_smaj","eff_pt_true smaj cut",40,0.,2000.);
TH1D* eff_pt_true_smin = new TH1D("eff_pt_true_smin","eff_pt_true smin cut",40,0.,2000.);
TH1D* eff_pt_true_ecaliso = new TH1D("eff_pt_true_ecaliso","eff_pt_true ecaliso cut",40,0.,2000.);
TH1D* eff_pt_true_etajet = new TH1D("eff_pt_true_etajet","eff_pt_true eta jet cut",40,0.,2000.);
TH1D* eff_pt_true_deltaphi = new TH1D("eff_pt_true_deltaphi","eff_pt_true deltaphi cut",40,0.,2000.);
TH1D* eff_pt_true_2ndjet = new TH1D("eff_pt_true_2ndjet","eff_pt_true 2ndjet cut",40,0.,2000.);
TH1D* eff_pt_true_deltaeta = new TH1D("eff_pt_true_deltaeta","eff_pt_true deltaeta cut",40,0.,2000.);

TH1D* eff_pt_true_eta_int = new TH1D("eff_pt_true_eta_int","eff_pt_true eta cut cascade",40,0.,2000.);
TH1D* eff_pt_true_ntrk_int = new TH1D("eff_pt_true_ntrk_int","eff_pt_true ntrk cut cascade ",40,0.,2000.);
TH1D* eff_pt_true_ptiso_int = new TH1D("eff_pt_true_ptiso_int","eff_pt_true ptiso cut cascade",40,0.,2000.);
TH1D* eff_pt_true_emf_int = new TH1D("eff_pt_true_emf_int","eff_pt_true emf cut cascade",40,0.,2000.);
TH1D* eff_pt_true_smaj_int = new TH1D("eff_pt_true_smaj_int","eff_pt_true smaj cut cascade",40,0.,2000.);
TH1D* eff_pt_true_smin_int = new TH1D("eff_pt_true_smin_int","eff_pt_true smin cut cascade",40,0.,2000.);
TH1D* eff_pt_true_ecaliso_int = new TH1D("eff_pt_true_ecaliso_int","eff_pt_true ecaliso cut cascade",40,0.,2000.);
TH1D* eff_pt_true_etajet_int = new TH1D("eff_pt_true_etajet_int","eff_pt_true eta jet cut cascade",40,0.,2000.);
TH1D* eff_pt_true_deltaphi_int = new TH1D("eff_pt_true_deltaphi_int","eff_pt_true deltaphi cut cascade",40,0.,2000.);
TH1D* eff_pt_true_2ndjet_int = new TH1D("eff_pt_true_2ndjet_int","eff_pt_true 2ndjet cut cascade",40,0.,2000.);
TH1D* eff_pt_true_deltaeta_int = new TH1D("eff_pt_true_deltaeta_int","eff_pt_true deltaeta cut cascade",40,0.,2000.);

void sum(char* histo, bool issig = 1){

  int nbins = 8;
  if(!issig) nbins = 7;

  for (int i=0; i<nbins; i++) {
    
    if(issig) signalfile[i]->cd();
    else bkgfile[i]->cd();
    for(int j=1; j<pt_true->GetNbinsX()+1; j++){
      ((TH1D*)gROOT->Get(histo))->SetBinContent(j,((TH1D*)gROOT->Get(histo))->GetBinContent(j)+((TH1D*)gDirectory->Get(histo))->GetBinContent(j));
      ((TH1D*)gROOT->Get(histo))->SetBinError(j,sqrt(pow(((TH1D*)gROOT->Get(histo))->GetBinError(j),2)+pow(((TH1D*)gDirectory->Get(histo))->GetBinError(j),2)));
    }    
  }

}

void eff(char* histo1, char* histo2, bool issig = 1){
    
  gROOT->cd();
  char name[100];
  double eff1(0), erreff1(0), eff2(0), erreff2(0), eff3(0), erreff3(0), eff4(0), erreff4(0) ;
  double num1(0), den1(0), errden1(0), num2(0), den2(0), errden2(0), num3(0), den3(0), errden3(0), num4(0), den4(0), errden4(0);
  sprintf(name,"%s%s","eff_",histo1);

  for(int j=1; j<((TH1D*)gROOT->Get(histo2))->GetNbinsX()+1; j++){

    double eff(0), erreff(0);
    if(((TH1D*)gROOT->Get(histo2))->GetBinContent(j)>0 && ((TH1D*)gROOT->Get(histo1))->GetBinContent(j)>0){
      eff = ((TH1D*)gROOT->Get(histo1))->GetBinContent(j) / ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      erreff = sqrt(eff*(1-eff)) * ((TH1D*)gROOT->Get(histo2))->GetBinError(j) / ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      ((TH1D*)gROOT->Get(name))->SetBinContent(j,eff);
      ((TH1D*)gROOT->Get(name))->SetBinError(j,erreff);    
    }else{
      ((TH1D*)gROOT->Get(name))->SetBinContent(j,.001);
      ((TH1D*)gROOT->Get(name))->SetBinError(j,0.001);          
    }
  }
  
  for(int j=1; j<((TH1D*)gROOT->Get(histo1))->GetNbinsX()+1; j++){
    if(j<3) {
      num1 += ((TH1D*)gROOT->Get(histo1))->GetBinContent(j);
      den1 += ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      errden1 = sqrt(pow(errden1,2)+pow(((TH1D*)gDirectory->Get(histo2))->GetBinError(j),2));
    }
    if(j>2 && j<5) {
      num2 += ((TH1D*)gROOT->Get(histo1))->GetBinContent(j);
      den2 += ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      errden2 = sqrt(pow(errden2,2)+pow(((TH1D*)gDirectory->Get(histo2))->GetBinError(j),2));
    }
    if(j>4 && j<9) {
      num3 += ((TH1D*)gROOT->Get(histo1))->GetBinContent(j);
      den3 += ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      errden3 = sqrt(pow(errden3,2)+pow(((TH1D*)gDirectory->Get(histo2))->GetBinError(j),2));
    }
    if(j>8) {
      num4 += ((TH1D*)gROOT->Get(histo1))->GetBinContent(j);
      den4 += ((TH1D*)gROOT->Get(histo2))->GetBinContent(j);
      errden4 = sqrt(pow(errden4,2)+pow(((TH1D*)gDirectory->Get(histo2))->GetBinError(j),2));
    }
  }
  
  eff1 = num1/den1;
  erreff1 = sqrt(eff1*(1-eff1)) * errden1/den1 * 100;
  eff2 = num2/den2;
  erreff2 = sqrt(eff2*(1-eff2)) * errden2/den2 * 100;
  eff3 = num3/den3;
  erreff3 = sqrt(eff3*(1-eff3)) * errden3/den3 * 100;
  eff4 = num4/den4;
  erreff4 = sqrt(eff4*(1-eff4)) * errden4/den4 * 100;

  eff1 *= 100; eff2 *= 100; eff3 *= 100; eff4 *= 100;
  
  char myline[1000];
  if(erreff1>0.1)  sprintf(myline, "%s & $%2.1f \pm %2.1f$ & $%2.1f \pm %2.1f$ & $%2.1f \pm %2.1f$ & $%2.1f \pm %2.1f$", histo1, eff1, erreff1, eff2, erreff2, eff3, erreff3, eff4, erreff4);
  if(erreff1>0.01 &&erreff1<0.1 )   sprintf(myline, "%s & $%2.2f \pm %2.2f$ & $%2.2f \pm %2.2f$ & $%2.2f \pm %2.2f$ & $%2.2f \pm %2.2f$", histo1, eff1, erreff1, eff2, erreff2, eff3, erreff3, eff4, erreff4);  
  if(erreff1>0.001 &&erreff1<0.01 )   sprintf(myline, "%s & $%2.3f \pm %2.3f$ & $%2.3f \pm %2.3f$ & $%2.3f \pm %2.3f$ & $%2.3f \pm %2.3f$", histo1, eff1, erreff1, eff2, erreff2, eff3, erreff3, eff4, erreff4);  
  if(erreff1<0.001 &&erreff1<0.001 )   sprintf(myline, "%s & $%2.4f \pm %2.4f$ & $%2.4f \pm %2.4f$ & $%2.4f \pm %2.4f$ & $%2.4f \pm %2.4f$", histo1, eff1, erreff1, eff2, erreff2, eff3, erreff3, eff4, erreff4);  
  if(erreff1<0.0001 )   sprintf(myline, "%s & $%2.5f \pm %2.5f$ & $%2.5f \pm %2.5f$ & $%2.5f \pm %2.5f$ & $%2.5f \pm %2.5f$", histo1, eff1, erreff1, eff2, erreff2, eff3, erreff3, eff4, erreff4);  
  cout << myline << endl;
  
  }

void efficiencies(bool issig = 1){

  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  gStyle->SetOptStat(0);
  gStyle->SetStatY(.9);
  gStyle->SetStatX(.9);
  

  using namespace std;


  // PROCESS GAMMA+JET
  // Set pT hat bin cross sections, needed for event weights
  //signal["0_15"] = 1.6e6; // what's the xsect?
  //signal["15_20"] = 2.5e5;// what's the xsect?
  //signal["20_30"] = 
//   signal["30_50"] = 41140.;
//   signal["50_80"] = 7210.;
//   signal["80_120"] = 1307.;
//   signal["120_170"] = 276.;
//   signal["170_300"] = 87.1;
//   signal["300_500"] = 8.29;
//   signal["500_7000"] = 0.878;
  signal["15"] = 2.887E+05;//E-04;
  signal["30"] = 3.222E+04;//E-05;
  signal["80"] = 1.010E+03;//E-06;
  signal["170"] = 5.143E+01;//E-08;
  signal["300"] = 4.193E+00;//E-09;
  signal["470"] = 4.515E-01;//E-10;
  signal["800"] = 2.003E-02;//E-11;
  signal["1400"] = 2.686E-04;//E-13;
  
  // PROCESS QCD
  // Set pT hat bin cross sections, needed for event weights
  map<string, double> qcd;
 //  qcd["30_50"] = 163000000.;
//   qcd["50_80"]  =  21600000.;
//   qcd["80_120"] =   3080000.;
//   qcd["120_170"]  =    494000.;
//   qcd["170_230"]  =    101000.;
//   qcd["230_300"]  =     24500.;
//   qcd["300_380"]  =      6240.;
//   qcd["380_470"]  =      1780.;
//   qcd["470_600"]  =       683.;
//   qcd["600_800"]  =       204.;
//   qcd["800_1000"] =       35.1;
//   qcd["1000_1400"]=       10.9;
//   qcd["1400_1800"]=       1.06;
//   qcd["1800_2200"]=       0.145;
//   qcd["2200_2600"]=       0.0238;
//   qcd["2600_3000"]=       0.00429;
//   qcd["3000_3500"]=       0.000844;
//   qcd["3500_inf"] =       0.000108;
  qcd["30"] = 109057220.4;
  qcd["80"] = 1934639.567;
  qcd["170"] = 62562.87713;
  qcd["300"] = 3664.608301;
  qcd["470"] = 315.5131272;
  qcd["800"] = 11.9419745;
  qcd["1400"] = 0.1720187189;
  
  int i=0;
  for (map<string, double>::iterator it = signal.begin();
       it != signal.end(); ++it) {
    signalfile[i] = new TFile(Form("output_fix_PhotonJets_ite/output_PhotonJets_%s.root", it->first));
    cout << i << "   " << Form("output_fix_PhotonJets_ite/output_PhotonJets_%s.root", it->first) << endl;
    i++;
  } // for signal
  
  i=0;
  for (map<string, double>::iterator it = qcd.begin();
       it != qcd.end(); ++it) {
    bkgfile[i] = new TFile(Form("output_fix_QCD_ite/output_QCD_%s.root", it->first));
    cout << i << "   " << Form("output_fix_QCD_ite/output_QCD_%s.root", it->first) << endl;
    i++;
  } // for signal
  
  gROOT->cd();
  cout << "criterion (one-by-one efficiency) & $p_T<100GeV$  & $100GeV<p_T<200GeV$ & $200GeV<p_T<400GeV$ & $p_T>400GeV$" << endl;
  cout << "hline" << endl;
  sum("pt_true",issig);                
  sum("pt_true_eta_int",issig);	 eff("pt_true_eta_int","pt_true",issig);     
  sum("pt_true_ntrk",issig);		 eff("pt_true_ntrk","pt_true",issig);	      
  sum("pt_true_ptiso",issig);		 eff("pt_true_ptiso","pt_true",issig);	      
  sum("pt_true_emf",issig);		 eff("pt_true_emf","pt_true",issig);	      
  sum("pt_true_smaj",issig);		 eff("pt_true_smaj","pt_true",issig);	      
  sum("pt_true_smin",issig);		 eff("pt_true_smin","pt_true",issig);	      
  sum("pt_true_ecaliso",issig);	 eff("pt_true_ecaliso","pt_true",issig);     
  cout << "hline" << endl;
  cout << "criterion (cascade efficiency) & $p_T<100GeV$  & $100GeV<p_T<200GeV$ & $200GeV<p_T<400GeV$ & $p_T>400GeV$" << endl;
  cout << "hline" << endl;				                             
  eff("pt_true_eta_int","pt_true");     
  sum("pt_true_ntrk_int",issig);	 eff("pt_true_ntrk_int","pt_true",issig);    
  sum("pt_true_ptiso_int",issig);	 eff("pt_true_ptiso_int","pt_true",issig);   
  sum("pt_true_emf_int",issig);	 eff("pt_true_emf_int","pt_true",issig);     
  sum("pt_true_smaj_int",issig);	 eff("pt_true_smaj_int","pt_true",issig);    
  sum("pt_true_smin_int",issig);	 eff("pt_true_smin_int","pt_true",issig);    
  sum("pt_true_ecaliso_int",issig);	 eff("pt_true_ecaliso_int","pt_true",issig); 
  cout << "hline" << endl;

  cout << "hline" << endl;
  cout << "criterion (one-by-one efficiency) & $p_T<100GeV$  & $100GeV<p_T<200GeV$ & $200GeV<p_T<400GeV$ & $p_T>400GeV$" << endl;
  cout << "hline" << endl;				                             
  sum("pt_true_etajet",issig);	 eff("pt_true_etajet","pt_true_ecaliso_int",issig);      
  sum("pt_true_deltaphi",issig);	 eff("pt_true_deltaphi","pt_true_ecaliso_int",issig);    
  sum("pt_true_2ndjet",issig);	 eff("pt_true_2ndjet","pt_true_ecaliso_int",issig);      
  sum("pt_true_deltaeta",issig);	 eff("pt_true_deltaeta","pt_true_ecaliso_int",issig);    

  cout << "hline" << endl;
  cout << "criterion (cascade efficiency) & $p_T<100GeV$  & $100GeV<p_T<200GeV$ & $200GeV<p_T<400GeV$ & $p_T>400GeV$" << endl;
  cout << "hline" << endl;				                             
  sum("pt_true_etajet_int",issig);	 eff("pt_true_etajet_int","pt_true_ecaliso_int",issig);  
  sum("pt_true_deltaphi_int",issig);	 eff("pt_true_deltaphi_int","pt_true_ecaliso_int",issig);
  sum("pt_true_2ndjet_int",issig);	 eff("pt_true_2ndjet_int","pt_true_ecaliso_int",issig);  
  sum("pt_true_deltaeta_int",issig);	 eff("pt_true_deltaeta_int","pt_true_ecaliso_int",issig);



  gROOT->cd();
 
  double min(0.1), max(1.05);
  c0->SetLogy(0);
  char namex[100];
  sprintf(namex,"true #gamma p_{T} [GeV]");
  if(!issig){ min = 0.0000001;max = 1.05; c0->SetLogy(1);sprintf(namex,"true parton p_{T} [GeV]");}
  //  if(!issig){ min = 0.0000001;max = .01; c0->SetLogy(0);sprintf(namex,"true parton p_{T} [GeV]");}
  eff_pt_true_eta_int->SetAxisRange(0.,700.);
  eff_pt_true_eta_int->SetMaximum(max);
  eff_pt_true_eta_int->SetMinimum(min);
  eff_pt_true_eta_int->SetTitle("");
  eff_pt_true_eta_int->SetXTitle(namex);
  eff_pt_true_eta_int->SetYTitle("efficiency");
  eff_pt_true_eta_int->SetMarkerStyle(29);
  eff_pt_true_eta_int->SetMarkerColor(kBlack);
  eff_pt_true_eta_int->SetMarkerSize(1.5);
  eff_pt_true_eta_int->Draw("pe");
  eff_pt_true_ntrk_int->SetMarkerColor(kRed);
  eff_pt_true_ntrk_int->SetMarkerStyle(20);
  eff_pt_true_ntrk_int->SetMarkerSize(1.5);
  eff_pt_true_ntrk_int->Draw("pesame");
  eff_pt_true_ptiso_int->SetMarkerColor(kBlue);
  eff_pt_true_ptiso_int->SetMarkerStyle(21);
  eff_pt_true_ptiso_int->SetMarkerSize(1.5);
  eff_pt_true_ptiso_int->Draw("pesame");
  eff_pt_true_emf_int->SetMarkerColor(kGreen);
  eff_pt_true_emf_int->SetMarkerStyle(22);
  eff_pt_true_emf_int->SetMarkerSize(1.5);
  eff_pt_true_emf_int->Draw("pesame");
  eff_pt_true_smaj_int->SetMarkerColor(kMagenta);
  eff_pt_true_smaj_int->SetMarkerStyle(23);
  eff_pt_true_smaj_int->SetMarkerSize(1.5);
  eff_pt_true_smaj_int->Draw("pesame");
  eff_pt_true_smin_int->SetMarkerColor(kCyan);
  eff_pt_true_smin_int->SetMarkerStyle(24);
  eff_pt_true_smin_int->SetMarkerSize(1.5);
  eff_pt_true_smin_int->Draw("pesame");
  eff_pt_true_ecaliso_int->SetMarkerColor(kOrange);
  eff_pt_true_ecaliso_int->SetMarkerStyle(25);
  eff_pt_true_ecaliso_int->SetMarkerSize(1.5);
  eff_pt_true_ecaliso_int->Draw("pesame");
  TLegend *leg;
  leg = new TLegend(0.54,0.12,0.88,0.5);
  //  leg->SetFillStyle(0); 
  leg->SetBorderSize(2.); leg->SetTextSize(0.045);
  leg->SetFillColor(0);
  legge = leg->AddEntry(eff_pt_true_eta_int, "#gamma reco", "p");
  legge = leg->AddEntry(eff_pt_true_ntrk_int, "N_{trk} iso", "p");
  legge = leg->AddEntry(eff_pt_true_ptiso_int, "#Sigma_{trk} p_{T} iso", "p");
  legge = leg->AddEntry(eff_pt_true_emf_int, "HCAL/ECAL", "p");
  legge = leg->AddEntry(eff_pt_true_smaj_int, "Major Axis", "p");
  legge = leg->AddEntry(eff_pt_true_smin_int, "Minor Axis", "p");
  legge = leg->AddEntry(eff_pt_true_ecaliso_int, "#Sigma E_{ECAL} iso", "p");
  leg->Draw();
  if(issig) c0->SaveAs("eff_cascade_id_signal.eps");
  else  c0->SaveAs("eff_cascade_id_bkg.eps");
  
  eff_pt_true_eta_int->SetAxisRange(0.,700.);
  eff_pt_true_eta_int->SetMaximum(max);
  eff_pt_true_eta_int->SetMinimum(min);
  eff_pt_true_eta_int->SetTitle("");
  eff_pt_true_eta_int->SetXTitle(namex);
  eff_pt_true_eta_int->SetYTitle("efficiency");
  eff_pt_true_eta_int->SetMarkerStyle(29);
  eff_pt_true_eta_int->SetMarkerColor(kBlack);
  eff_pt_true_eta_int->SetMarkerSize(1.5);
  eff_pt_true_eta_int->Draw("pe");
  eff_pt_true_ntrk->SetMarkerColor(kRed);
  eff_pt_true_ntrk->SetMarkerStyle(20);
  eff_pt_true_ntrk->SetMarkerSize(1.5);
  eff_pt_true_ntrk->Draw("pesame");
  eff_pt_true_ptiso->SetMarkerColor(kBlue);
  eff_pt_true_ptiso->SetMarkerStyle(21);
  eff_pt_true_ptiso->SetMarkerSize(1.5);
  eff_pt_true_ptiso->Draw("pesame");
  eff_pt_true_emf->SetMarkerColor(kGreen);
  eff_pt_true_emf->SetMarkerStyle(22);
  eff_pt_true_emf->SetMarkerSize(1.5);
  eff_pt_true_emf->Draw("pesame");
  eff_pt_true_smaj->SetMarkerColor(kMagenta);
  eff_pt_true_smaj->SetMarkerStyle(23);
  eff_pt_true_smaj->SetMarkerSize(1.5);
  eff_pt_true_smaj->Draw("pesame");
  eff_pt_true_smin->SetMarkerColor(kCyan);
  eff_pt_true_smin->SetMarkerStyle(24);
  eff_pt_true_smin->SetMarkerSize(1.5);
  eff_pt_true_smin->Draw("pesame");
  eff_pt_true_ecaliso->SetMarkerColor(kOrange);
  eff_pt_true_ecaliso->SetMarkerStyle(25);
  eff_pt_true_ecaliso->SetMarkerSize(1.5);
  eff_pt_true_ecaliso->Draw("pesame");
  leg->Draw();
  if(issig) c0->SaveAs("eff_sing_id_signal.eps");
  elsec0->SaveAs("eff_sing_id_bkg.eps");

  max = .95;
  if(!issig){c0->SetLogy(1);max = 1.05;}
  eff_pt_true_etajet_int->SetAxisRange(0.,700.);
  eff_pt_true_etajet_int->SetMaximum(max);
  eff_pt_true_etajet_int->SetMinimum(0.00005);
  eff_pt_true_etajet_int->SetTitle("");
  eff_pt_true_etajet_int->SetXTitle(namex);
  eff_pt_true_etajet_int->SetYTitle("efficiency");
  eff_pt_true_etajet_int->SetMarkerStyle(29);
  eff_pt_true_etajet_int->SetMarkerColor(kBlack);
  eff_pt_true_etajet_int->SetMarkerSize(1.5);
  eff_pt_true_etajet_int->Draw("pe");
  eff_pt_true_deltaphi_int->SetMarkerColor(kRed);
  eff_pt_true_deltaphi_int->SetMarkerStyle(20);
  eff_pt_true_deltaphi_int->SetMarkerSize(1.5);
  eff_pt_true_deltaphi_int->Draw("pesame");
  eff_pt_true_2ndjet_int->SetMarkerColor(kBlue);
  eff_pt_true_2ndjet_int->SetMarkerStyle(21);
  eff_pt_true_2ndjet_int->SetMarkerSize(1.5);
  eff_pt_true_2ndjet_int->Draw("pesame");
  eff_pt_true_deltaeta_int->SetMarkerColor(kGreen);
  eff_pt_true_deltaeta_int->SetMarkerStyle(22);
  eff_pt_true_deltaeta_int->SetMarkerSize(1.5);
  eff_pt_true_deltaeta_int->Draw("pesame");
  TLegend *leg2;
  if(issig) leg2 = new TLegend(0.56,0.14,0.76,0.37);
//   else  leg2 = new TLegend(0.15,0.9,0.35,0.62);
  else  leg2 = new TLegend(0.2,0.14,0.4,0.37);
  //  leg2->SetFillStyle(10); 
  leg2->SetBorderSize(2); leg2->SetTextSize(0.045);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(eff_pt_true_2ndjet_int, "2^{nd} jet", "p");
  legge2 = leg2->AddEntry(eff_pt_true_deltaphi_int, "#Delta #phi", "p");
  legge2 = leg2->AddEntry(eff_pt_true_deltaeta_int, "#Delta #eta", "p");
  legge2 = leg2->AddEntry(eff_pt_true_etajet_int, "#eta jet", "p");
  leg2->Draw();    
  if(issig) c0->SaveAs("eff_cascade_event_signal.eps");
  else c0->SaveAs("eff_cascade_event_bkg.eps");

  eff_pt_true_etajet->SetAxisRange(0.,700.);
  eff_pt_true_etajet->SetMaximum(max);
  eff_pt_true_etajet->SetMinimum(0.0005);
  eff_pt_true_etajet->SetTitle("");
  eff_pt_true_etajet->SetXTitle(namex);
  eff_pt_true_etajet->SetYTitle("efficiency");
  eff_pt_true_etajet->SetMarkerStyle(29);
  eff_pt_true_etajet->SetMarkerColor(kBlack);
  eff_pt_true_etajet->SetMarkerSize(1.5);
  eff_pt_true_etajet->Draw("pe");
  eff_pt_true_deltaphi->SetMarkerColor(kRed);
  eff_pt_true_deltaphi->SetMarkerStyle(20);
  eff_pt_true_deltaphi->SetMarkerSize(1.5);
  eff_pt_true_deltaphi->Draw("pesame");
  eff_pt_true_2ndjet->SetMarkerColor(kBlue);
  eff_pt_true_2ndjet->SetMarkerStyle(21);
  eff_pt_true_2ndjet->SetMarkerSize(1.5);
  eff_pt_true_2ndjet->Draw("pesame");
  eff_pt_true_deltaeta->SetMarkerColor(kGreen);
  eff_pt_true_deltaeta->SetMarkerStyle(22);
  eff_pt_true_deltaeta->SetMarkerSize(1.5);
  eff_pt_true_deltaeta->Draw("pesame");

  leg2->Draw();    
  if(issig) c0->SaveAs("eff_sing_event_signal.eps");
  else c0->SaveAs("eff_sing_event_bkg.eps"); 

  c0->SetLogy();
  pt_true->SetMinimum(0.0001);
  pt_true->Draw("");
  pt_true_pt->Draw("histsame");
  pt_true_eta_int->Draw("histsame");
  pt_true_ntrk_int->Draw("histsame");
  pt_true_ptiso_int->Draw("histsame");
  pt_true_emf_int->Draw("histsame");
  pt_true_smaj_int->Draw("histsame");
  pt_true_smin_int->Draw("histsame");
  pt_true_ecaliso_int->Draw("histsame");
  pt_true_etajet_int->Draw("histsame");
  pt_true_deltaphi_int->Draw("histsame");
  pt_true_2ndjet_int->Draw("histsame");
  pt_true_deltaeta_int->Draw("histsame");

  if(issig) c0->SaveAs("pt_sig_all.eps");
  else c0->SaveAs("pt_bkg_all.eps");

}

