#include "Plotter.cxx"

TGraphAsymmErrors *draw(Plotter *plotter, TCut sel, TCut sel2="") {
  TString variable("muon.Pt() >>hnum(50,0.0,150.)");
  TCut selection("matchL1JetDR < 0.5 && matchL1Jet.Et() > 15");
  selection = selection && sel;
  TCut denominator("TMath::Abs(muon.Eta()) < 2.4");

  TGraphAsymmErrors *gr = plotter->DrawHistogram(variable, selection && sel2, denominator && sel2);

  gr->SetMaximum(1.1);
  gr->SetMinimum(0.0);

  gr->GetXaxis()->SetTitle("MC Muon p_{T} (GeV/c)");
  gr->GetYaxis()->SetTitle("L1 jet reconstruction eff.");

  return gr;
}


void plotL1MuonEfficiency(TString format=".png", TString file="ntuple.root", bool rejectJets = false, bool rejectMothers = false) {
  Plotter *plotter = new Plotter(file, "analysis/tree");
  TCanvas *c = new TCanvas("foo1");

  //rejectJets = true;
  //rejectMothers = true;

  TCut rejection("");
  if(rejectJets)
    rejection = rejection && TCut("matchGenJetDR > 0.5 || matchGenJetIndex == -1");
  if(rejectMothers)
    rejection = rejection && TCut("abs(muonMothers[0]) == 24");

  TGraph *jetreco = draw(plotter, "(matchL1JetIsTau || matchL1JetIsCen)", rejection);
  TGraph *cenreco = draw(plotter, "matchL1JetIsCen", rejection);
  TGraph *taureco = draw(plotter, "matchL1JetIsTau", rejection);

  taureco->SetMarkerColor(kRed);
  cenreco->SetMarkerColor(kBlue);

  jetreco->Draw("AP");
  taureco->Draw("Psame");
  cenreco->Draw("Psame");

  TLatex l;
  l.SetNDC();
  float x=0.52, y=0.85;
  l.DrawLatex(x, y, "Numerator:"); y -= 0.05;
  l.DrawLatex(x, y, "#DeltaR(L1 jet, #mu) < 0.5"); y -= 0.05;
  l.DrawLatex(x, y, "L1 jet E_{T} > 15 GeV/c"); y -= 0.05;

  y -= 0.05;
  l.SetTextColor(kBlue);
  l.DrawLatex(x, y, "|#eta| < 2.4"); y -= 0.05;
  if(rejectJets) {
    l.DrawLatex(x, y, "#DeltaR(GenJet, #mu) > 0.5"); y -= 0.05;
  }
  if(rejectMothers) {
    l.DrawLatex(x, y, "W#rightarrow#mu#nu only"); y -= 0.05;
  }

  TLegend *legend = new TLegend(0.2, 0.7, 0.4, 0.9);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(1);
  legend->SetMargin(0.1);
  legend->AddEntry(jetreco, "L1 Jet", "p");
  legend->AddEntry(taureco, "L1 Tau jet", "p");
  legend->AddEntry(cenreco, "L1 Cen jet", "p");

  legend->Draw();

  c->SaveAs(Form("MCMuonEfficiency%s", format.Data()));

  delete plotter;
  TFile *f = TFile::Open(file);
  TTree *tree = dynamic_cast<TTree *>(f->Get("analysis/tree"));

  c = new TCanvas("foo2");
  TCut selection("(matchL1JetIsTau || matchL1JetIsCen) && TMath::Abs(muon.Eta()) < 2.4 && matchL1Jet.Et() > 15");
  selection = selection && rejection;
  tree->Draw("matchL1JetDR >>foo(50,0.,4.)", selection);
  TH1 *h = tree->GetHistogram();
  h->GetXaxis()->SetTitle("#DeltaR(L1 jet, #mu)");
  h->GetYaxis()->SetTitle("# of entries");

  y = 0.75;
  l.SetTextColor(kBlack);
  l.DrawLatex(x, y, "L1 jet E_{T} > 15 GeV/c"); y -=0.05;
  l.DrawLatex(x, y, "|#eta| < 2.4"); y -= 0.05;
  if(rejectJets) {
    l.DrawLatex(x, y, "#DeltaR(GenJet, #mu) > 0.5"); y -= 0.05;
  }
  if(rejectMothers) {
    l.DrawLatex(x, y, "W#rightarrow#mu#nu only"); y -= 0.05;
  }
  c->SaveAs(Form("MCMuonDR%s", format.Data()));

  c = new TCanvas("foo3");
  selection = TCut("TMath::Abs(muon.Eta()) < 2.4 && matchGenJetIndex >= 0");
  if(rejectMothers)
    selection = selection && rejection;
  tree->Draw("matchGenJetDR >>foo2(50.,0.,4.)", selection);
  h = tree->GetHistogram();

  h->GetXaxis()->SetTitle("#DeltaR(GenJet, #mu)");
  h->GetYaxis()->SetTitle("# of entries");

  y = 0.75;
  l.SetTextColor(kBlack);
  l.DrawLatex(x, y, "|#eta| < 2.4"); y -= 0.05;
  if(rejectMothers) {
    l.DrawLatex(x, y, "W#rightarrow#mu#nu only"); y -= 0.05;
  }

  c->SaveAs(Form("MCMuonGenJetDR%s", format.Data()));

  std::cout << "Tree has " << tree->GetEntries() << " entries" << std::endl;
}
