#include "RecoBTag/Analysis/interface/SoftLeptonTagPlotter.h"

static const char* ordinal[] = { "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th" };

SoftLeptonTagPlotter::SoftLeptonTagPlotter(
    JetTagPlotter *jetTagPlotter,
    bool update
) :
    BaseBTagPlotter( jetTagPlotter->etaPtBin()), m_plotter(jetTagPlotter )
{
  if (update) {
    TString dir = "SoftLepton" + theExtensionString;
    gFile->cd();
    gFile->cd(dir);
  }

  m_discriminant = new FlavourHistorgrams<double> (
      "jet discriminant", "Jet b tagging discriminant",
      50, 0.0, 1.0, false, false, true, "b", update);

  for (int i = 0; i < s_leptons; i++) {
    m_leptonId[i] = new FlavourHistorgrams<double> (
        Form("%s lepton %s", ordinal[i], "id" ),
        "Lepton identification discriminaint",
        50, 0.0, 1.0, false, false, true, "b", update);
    m_leptonPt[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "pT" ),
        "Lepton transverse moementum",
        100, 0.0, 20.0, false, false, true, "b", update);
    m_sip3d[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "sip3d" ),
        "Lepton signed 3D impact parameter significance",
        100, -20.0, 30.0, false, false, true, "b", update);
    m_ptRel[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "pT rel" ),
        "Lepton transverse moementum relative to jet axis",
        100, 0.0, 10.0, false, false, true, "b", update);
    m_etaRel[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "eta rel" ),
        "Lepton pseudorapidity relative to jet axis",
        100, -5.0, 25.0, false, false, true, "b", update);
    m_deltaR[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "delta R" ),
        "Lepton pseudoangular distance from jet axis",
        100, 0.0, 0.6, false, false, true, "b", update);
    m_ratio[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "energy ratio" ),
        "Ratio of lepton momentum to jet energy",
        100, 0.0, 2.0, false, false, true, "b", update);
    m_ratioRel[i] = new FlavourHistorgrams<double> (
        Form( "%s lepton %s", ordinal[i], "" ),
        "Ratio of lepton momentum along the jet axis to jet energy",
        100, 0.0, 2.0, false, false, true, "b", update);
  }
}

SoftLeptonTagPlotter::~SoftLeptonTagPlotter ()
{
  delete m_plotter;

  delete m_discriminant;
  for (int i = 0; i < s_leptons; i++) {
    delete m_leptonId[i];
    delete m_leptonPt[i];
    delete m_sip3d[i];
    delete m_ptRel[i];
    delete m_etaRel[i];
    delete m_deltaR[i];
    delete m_ratio[i];
    delete m_ratioRel[i];
  }
}

void SoftLeptonTagPlotter::analyzeTag(
    const reco::SoftLeptonTagInfo & tagInfo,
    const reco::JetTag &            jetTag,
    const JetFlavour &              jetFlavour )
{

  int flavour = jetFlavour.flavour();
  m_plotter->analyzeJetTag(jetTag, jetFlavour);

  // check if properly initialized

  int n_leptons = tagInfo.leptons();

  if (n_leptons)
    m_discriminant->fill( flavour, tagInfo.properties(0).tag );
  for (int i = 0; i < n_leptons && i < s_leptons; i++) {
    m_leptonId[i]->fill( flavour, 1.0 );
    m_leptonPt[i]->fill( flavour, tagInfo.lepton(i)->pt() );
    m_sip3d[i]->fill(    flavour, tagInfo.properties(i).sip3d );
    m_ptRel[i]->fill(    flavour, tagInfo.properties(i).ptRel );
    m_etaRel[i]->fill(   flavour, tagInfo.properties(i).etaRel );
    m_deltaR[i]->fill(   flavour, tagInfo.properties(i).deltaR );
    m_ratio[i]->fill(    flavour, tagInfo.properties(i).ratio );
    m_ratioRel[i]->fill( flavour, tagInfo.properties(i).ratioRel );
  }
}

void SoftLeptonTagPlotter::psPlot(const TString & name)
{
  m_plotter->psPlot(name);

  TString cName = "SoftLeptonPlots" + theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "SoftLeptonPlots" + theExtensionString, 600, 900);
  canvas.UseCurrentStyle();
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  m_discriminant->plot((TPad*) canvas.GetPrimitive( cName + "_1" ));
  for (int i = 0; i < s_leptons; i++) {
    canvas.cd(1);
    m_leptonId[i]->plot((TPad*) canvas.GetPrimitive( cName + "_1" ));
    canvas.cd(2);
    m_leptonPt[i]->plot((TPad*) canvas.GetPrimitive( cName + "_2" ));
    canvas.cd(1);
    m_sip3d[i]->plot((TPad*) canvas.GetPrimitive( cName + "_1" ));
    canvas.cd(2);
    m_ptRel[i]->plot((TPad*) canvas.GetPrimitive( cName + "_2" ));
    canvas.cd(3);
    m_etaRel[i]->plot((TPad*) canvas.GetPrimitive( cName + "_3" ));
    canvas.cd(4);
    m_deltaR[i]->plot((TPad*) canvas.GetPrimitive( cName + "_4" ));
    canvas.cd(5);
    m_ratio[i]->plot((TPad*) canvas.GetPrimitive( cName + "_5" ));
    canvas.cd(6);
    m_ratioRel[i]->plot((TPad*) canvas.GetPrimitive( cName + "_6" ));
  }
  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void SoftLeptonTagPlotter::write()
{
  m_plotter->write();

  TString dir= "SoftLepton" + theExtensionString;
  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);
  m_discriminant->write();
  for (int i = 0; i < s_leptons; i++) {
    m_leptonId[i]->write();
    m_leptonPt[i]->write();
    m_sip3d[i]->write();
    m_ptRel[i]->write();
    m_etaRel[i]->write();
    m_deltaR[i]->write();
    m_ratio[i]->write();
    m_ratioRel[i]->write();
  }
  gFile->cd();
}

void SoftLeptonTagPlotter::epsPlot(const TString & name)
{
  m_plotter->epsPlot( name );
  m_discriminant->epsPlot( name );
  for (int i=0; i < s_leptons; i++) {
    m_leptonId[i]->epsPlot( name );
    m_leptonPt[i]->epsPlot( name );
    m_sip3d[i]->epsPlot( name );
    m_ptRel[i]->epsPlot( name );
    m_etaRel[i]->epsPlot( name );
    m_deltaR[i]->epsPlot( name );
    m_ratio[i]->epsPlot( name );
    m_ratioRel[i]->epsPlot( name );
  }
}

