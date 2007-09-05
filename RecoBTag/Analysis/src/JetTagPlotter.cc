#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/Tools.h"

#include <iostream>

using namespace std;
using namespace RecoBTag;


JetTagPlotter::JetTagPlotter (const TString & tagName, const EtaPtBin & etaPtBin,
		       double discrStart, double discrEnd,
		       int nBinEffPur, double startEffPur, double endEffPur,
		       bool update, BaseBTagPlotter *extTagPlotter) :
		       BaseBTagPlotter(tagName, etaPtBin, nBinEffPur, startEffPur, endEffPur),
		       extTagPlotter_(extTagPlotter), discrStart_(discrStart),
		       discrEnd_(discrEnd) {

  if (update){
    TString dir= "JetTag"+theExtensionString;
    gFile->cd();
    gFile->cd(dir);
  }
  // fixed now:
  discrBins = 400;
  finalized = false;

  // to have a shorter name .....
  const TString & es = theExtensionString;

  // Discriminator for computations (fine binning)
  dDiscriminatorFC = new FlavourHistorgrams<double>
	("discrFC" + es, "Discriminator for computations" + es,
	discrBins, discrStart_, discrEnd_, true, true, true, "b", update);
  // jet flavour
  dJetFlav = new FlavourHistorgrams<int>
	("jetFlavour" + es, "Jet Flavour" + es, 22, -0.5, 21.5,
	false, false, false, "b", update);

  // track multiplicity in jet
  dJetMultiplicity = new FlavourHistorgrams<int>
	("jetMultiplicity" + es, "Jet Multiplicity" + es, 31, -0.5, 30.5,
	false, true, true, "b", update);

    // Discriminator: again with reasonable binning
  dDiscriminator = new FlavourHistorgrams<double>
	("discr" + es, "Discriminator" + es, 50, discrStart_, discrEnd_,
	false, true, true, "b", update);

    // reconstructed jet momentum
  dJetRecMomentum = new FlavourHistorgrams<double>
	("jetMomentum" + es, "jet momentum" + es, 200, 0.0, 200.0,
	false, false, true, "b", update);

  // reconstructed jet transverse momentum
  dJetRecPt = new FlavourHistorgrams<double>
	("jetPt" + es, "jet pt" + es, 200, 0.0, 200.0,
	false, false, true, "b", update);

  // reconstructed jet eta
  dJetRecPseudoRapidity = new FlavourHistorgrams<double>
	("jetEta" + es, "jet eta" + es, 100, -3.5, 3.5,
	false, false, true, "b", update);

  // reconstructed jet phi
  dJetRecPhi = new FlavourHistorgrams<double>
	("jetPhi" + es, "jet phi" + es, 100, -3.15, 3.15,
	false, false, true, "b", update);

  // associated parton momentum
  dJetPartonMomentum = new FlavourHistorgrams<double>
	("associatedPartonMomentum" + es, "associated parton momentum" + es,
	200, 0.0, 400.0, false, false, true, "b", update);

  // associated parton pt
  dJetPartonPt = new FlavourHistorgrams<double>
	("associatedPartonPt" + es, "associated parton pt" + es,
	200, 0.0, 400.0, false, false, true, "b", update);

  // associated parton eta
  dJetPartonPseudoRapidity = new FlavourHistorgrams<double>
	("associatedPartonEta" + es, "associated parton eta" + es,
	100, -3.5, 3.5, false, false, true, "b", update);
}



JetTagPlotter::~JetTagPlotter () {
  if (extTagPlotter_) delete extTagPlotter_;
  delete dDiscriminatorFC;
  delete dJetFlav;
  delete dJetMultiplicity;
  delete dDiscriminator;
  delete dJetRecMomentum;
  delete dJetRecPt;
  delete dJetRecPseudoRapidity;
  delete dJetRecPhi;
  delete dJetPartonMomentum;
  delete dJetPartonPt;
  delete dJetPartonPseudoRapidity;
  if (finalized) {
    delete effPurFromHistos;
  }
}

void JetTagPlotter::epsPlot(const TString & name)
{
  if (extTagPlotter_) extTagPlotter_->epsPlot(name);
  dDiscriminatorFC->epsPlot(name);
  dJetFlav->epsPlot(name);
  dJetMultiplicity->epsPlot(name);
  dDiscriminator->epsPlot(name);
  dJetRecMomentum->epsPlot(name);
  dJetRecPt->epsPlot(name);
  dJetRecPseudoRapidity->epsPlot(name);
  dJetRecPhi->epsPlot(name);
  dJetPartonMomentum->epsPlot(name);
  dJetPartonPt->epsPlot(name);
  dJetPartonPseudoRapidity->epsPlot(name);
  if (finalized) {
    effPurFromHistos->epsPlot(name);
  }
}

void JetTagPlotter::psPlot(const TString & name)
{
  if (extTagPlotter_) extTagPlotter_->psPlot(name);

  TString cName = "JetTagPlots"+ theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "JetTagPlors"+ theExtensionString, 600, 900);
  canvas.UseCurrentStyle();

  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  dJetFlav->plot();
  canvas.cd(2);
  dJetMultiplicity->plot();
  canvas.cd(3);
  dDiscriminator->plot();
  canvas.cd(4);
  dJetRecMomentum->plot();
  canvas.cd(5);
  dJetRecPt->plot();
  canvas.cd(6);
  dJetRecPseudoRapidity->plot();
  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);

  canvas.cd(1);
  dJetRecPhi->plot();
  canvas.cd(2);
  dJetPartonMomentum->plot();
  canvas.cd(3);
  dJetPartonPt->plot();
  canvas.cd(4);
  dJetPartonPseudoRapidity->plot();
  if (finalized) {
    canvas.cd(5);
    effPurFromHistos->discriminatorNoCutEffic()->plot();
    canvas.cd(6);
    effPurFromHistos->discriminatorCutEfficScan()->plot();
    canvas.Print(name + cName + ".ps");
    canvas.Clear();
    canvas.Divide(2,3);
    canvas.cd(1);
    effPurFromHistos->plot();
  }
  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void JetTagPlotter::analyzeTag(const reco::JetTag & jetTag,
	const JetFlavour & jetFlavour)
{
  if (extTagPlotter_) extTagPlotter_->analyzeTag(jetTag, jetFlavour);

  int jetFlav = jetFlavour.flavour();

  dDiscriminatorFC->fill(jetFlav, jetTag.discriminator());
  dJetFlav->fill(jetFlav, jetFlav);
  dJetMultiplicity->fill(jetFlav, jetTag.tracks().size()); //fixme
  dDiscriminator->fill(jetFlav, jetTag.discriminator() );
  dJetRecMomentum->fill(jetFlav, jetTag.jet()->p() );
  dJetRecPt->fill(jetFlav, jetTag.jet()->pt() );
  dJetRecPseudoRapidity->fill(jetFlav, jetTag.jet()->eta() );
  dJetRecPhi->fill(jetFlav, jetTag.jet()->phi());

  dJetPartonMomentum->fill(jetFlav, jetFlavour.underlyingParton4Vec().P() );
  dJetPartonPt->fill(jetFlav, jetFlavour.underlyingParton4Vec().Pt() );
  dJetPartonPseudoRapidity->fill(jetFlav, jetFlavour.underlyingParton4Vec().Eta() );
}


void JetTagPlotter::finalize()
{
  if (extTagPlotter_) extTagPlotter_->finalize();
  //
  // final processing:
  // produce the misid. vs. eff histograms
  //
  effPurFromHistos = new EffPurFromHistos ( dDiscriminatorFC,
				nBinEffPur_, startEffPur_, endEffPur_);
  effPurFromHistos->compute();
  finalized = true;
}


void JetTagPlotter::write()
{
  if (extTagPlotter_) extTagPlotter_->write();

  TString dir= "JetTag"+theExtensionString;

  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);

  dDiscriminatorFC->write();
  dJetFlav->write();
  dJetMultiplicity->write();
  dDiscriminator->write();
  dJetRecMomentum->write();
  dJetRecPt->write();
  dJetRecPseudoRapidity->write();
  dJetRecPhi->write();
  dJetPartonMomentum->write();
  dJetPartonPt->write();
  dJetPartonPseudoRapidity->write();
  if (finalized) {
    effPurFromHistos->write();
  }
  gFile->cd();
}
