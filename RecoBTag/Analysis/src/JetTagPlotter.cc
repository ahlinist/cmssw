#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/src/Tools.h"

#include <iostream>

using namespace std;


JetTagPlotter::JetTagPlotter (const EtaPtBin & etaPtBin,
		       double discrStart, double discrEnd,
		       int nBinEffPur, double startEffPur, double endEffPur, bool update) :
		       BaseBTagPlotter(etaPtBin),
		       discrStart_(discrStart),
		       discrEnd_(discrEnd), nBinEffPur_(nBinEffPur),
		       startEffPur_(startEffPur ), endEffPur_(endEffPur)  {

  if (update){
    TString dir= theExtensionString.Strip( TString::kLeading, '_');
    gFile->cd();
    gFile->cd(dir);
  }
  // fixed now:
  discrBins = 2000;
  finalized = false;

  // to have a shorter name .....
  const TString & es = theExtensionString;

  // Discriminator for computations (fine binning)
  dDiscriminatorFC = new FlavourHistorgrams<double>
	("discrFC" + es, "Discriminator for computations" + es,
	discrBins, discrStart_, discrEnd_, true, true, true, "l", update);
  // jet flavour
  dJetFlav = new FlavourHistorgrams<int>
	("jetFlavour" + es, "Jet Flavour" + es, 22, -0.5, 21.5,
	false, false, false, "l", update);

  // track multiplicity in jet
  dJetMultiplicity = new FlavourHistorgrams<int>
	("jetMultiplicity" + es, "Jet Multiplicity" + es, 31, -0.5, 30.5,
	false, false, false, "l", update);

    // Discriminator: again with reasonable binning
  dDiscriminator = new FlavourHistorgrams<double>
	("discr" + es, "Discriminator" + es, 50, discrStart_, discrEnd_,
	false, false, false, "b", update);

    // reconstructed jet momentum
  dJetRecMomentum = new FlavourHistorgrams<double>
	("jetMomentum" + es, "jet momentum" + es, 200, 0.0, 200.0,
	false, false, false, "l", update);

  // reconstructed jet transverse momentum
  dJetRecPt = new FlavourHistorgrams<double>
	("jetPt" + es, "jet pt" + es, 200, 0.0, 200.0,
	false, false, false, "l", update);

  // reconstructed jet eta
  dJetRecPseudoRapidity = new FlavourHistorgrams<double>
	("jetEta" + es, "jet eta" + es, 100, -3.5, 3.5,
	false, false, false, "l", update);

  // reconstructed jet phi
  dJetRecPhi = new FlavourHistorgrams<double>
	("jetPhi" + es, "jet phi" + es, 100, -3.15, 3.15,
	false, false, false, "l", update);

  // associated parton momentum
  dJetPartonMomentum = new FlavourHistorgrams<double>
	("associatedPartonMomentum" + es, "associated parton momentum" + es,
	200, 0.0, 400.0, false, false, false, "l", update);

  // associated parton pt
  dJetPartonPt = new FlavourHistorgrams<double>
	("associatedPartonPt" + es, "associated parton pt" + es,
	200, 0.0, 400.0, false, false, false, "l", update);

  // associated parton eta
  dJetPartonPseudoRapidity = new FlavourHistorgrams<double>
	("associatedPartonEta" + es, "associated parton eta" + es,
	100, -3.5, 3.5, false, false, false, "l", update);
}



JetTagPlotter::~JetTagPlotter () {
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
    delete dDiscrCut;
    delete dDiscrCutCond;
    delete effPurFromHistos;
  }
}

void JetTagPlotter::epsPlot(const TString & name)
{
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
    dDiscrCut->epsPlot(name);
    dDiscrCutCond->epsPlot(name);
    effPurFromHistos->epsPlot(name);
  }
}

void JetTagPlotter::psPlot(const TString & name)
{
  TString cName = "JetTagPlots"+ theExtensionString;
  TCanvas canvas(cName,
  	"JetTagPlors"+ theExtensionString, 600, 900);
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  dJetFlav->plot((TPad*) canvas.GetPrimitive(cName+"_1"));
  canvas.cd(2);
  dJetMultiplicity->plot((TPad*) canvas.GetPrimitive(cName+"_2"));
  canvas.cd(3);
  dDiscriminator->plot((TPad*) canvas.GetPrimitive(cName+"_3"));
  canvas.cd(4);
  dJetRecMomentum->plot((TPad*) canvas.GetPrimitive(cName+"_4"));
  canvas.cd(5);
  dJetRecPt->plot((TPad*) canvas.GetPrimitive(cName+"_5"));
  canvas.cd(6);
  dJetRecPseudoRapidity->plot((TPad*) canvas.GetPrimitive(cName+"_6"));
  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);

  canvas.cd(1);
  dJetRecPhi->plot((TPad*) canvas.GetPrimitive(cName+"_1"));
  canvas.cd(2);
  dJetPartonMomentum->plot((TPad*) canvas.GetPrimitive(cName+"_2"));
  canvas.cd(3);
  dJetPartonPt->plot((TPad*) canvas.GetPrimitive(cName+"_3"));
  canvas.cd(4);
  dJetPartonPseudoRapidity->plot((TPad*) canvas.GetPrimitive(cName+"_4"));
  if (finalized) {
    canvas.cd(5);
    dDiscrCut->plot((TPad*) canvas.GetPrimitive(cName+"_5"));
    canvas.cd(6);
    dDiscrCutCond->plot((TPad*) canvas.GetPrimitive(cName+"_6"));
    canvas.Print(name + cName + ".ps");
    canvas.Clear();
    canvas.Divide(2,3);
    canvas.cd(1);
    effPurFromHistos->plot((TPad*) canvas.GetPrimitive(cName+"_1"));
  }
  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void JetTagPlotter::analyzeJetTag(const reco::JetTag & jetTag)
{
  //fixme:
  int jetFlav = 5;
  // "normal" variables once per event

  dDiscriminatorFC->fill(jetFlav, jetTag.discriminator());
  dJetFlav->fill(jetFlav, jetFlav);
  dJetMultiplicity->fill(jetFlav, jetTag.tracks().size()); //fixme
  dDiscriminator->fill(jetFlav, jetTag.discriminator() );
  dJetRecMomentum->fill(jetFlav, jetTag.jet().p() );
  dJetRecPt->fill(jetFlav, jetTag.jet().pt() );
  dJetRecPseudoRapidity->fill(jetFlav, jetTag.jet().eta() );
  dJetRecPhi->fill(jetFlav, jetTag.jet().phi());

//fixme
//   dJetPartonMomentum->fill(jetFlav, jetPartonMomentum );
//   dJetPartonPt->fill(jetFlav, jetPartonPt );
//   dJetPartonPseudoRapidity->fill(jetFlav, jetPartonPseudoRapidity );

}


void JetTagPlotter::finalize() {
  //
  // final processing
  //
  double discrWidth = ( discrEnd_ - discrStart_ ) / discrBins;
  double discrFirstBin = discrStart_ + 0.5*discrWidth;

  dDiscrCut = new FlavourHistorgrams<double> (
	"discrCut" + theExtensionString, "Discriminator Cut" + theExtensionString,
	discrBins, discrStart_, discrEnd_, true, false, false, "l" );

  // conditional discriminator cut for efficiency histos

  dDiscrCutCond = new FlavourHistorgrams<double> (
	"discrCutCond" + theExtensionString, "Conditional Discriminator Cut" + theExtensionString,
	discrBins, discrStart_, discrEnd_, true, false, false, "l" );

  // use the EffPurFromHistos class to compute the "2D" histograms from the ones defined above
  effPurFromHistos = new EffPurFromHistos ( theExtensionString,
					     dDiscrCutCond->histo_d   (),
					     dDiscrCutCond->histo_u   (),
					     dDiscrCutCond->histo_s   (),
					     dDiscrCutCond->histo_c   (),
					     dDiscrCutCond->histo_b   (),
					     dDiscrCutCond->histo_g   (),
					     dDiscrCutCond->histo_ni  (),
					     dDiscrCutCond->histo_dus (),
					     dDiscrCutCond->histo_dusg(),
					     nBinEffPur_, startEffPur_, endEffPur_ );

  // discr. for computation
  vector<TH1F*> discrCfHistos;
  discrCfHistos.push_back ( dDiscriminatorFC->histo_all  () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_d    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_u    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_s    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_c    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_b    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_g    () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_ni   () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_dus  () );
  discrCfHistos.push_back ( dDiscriminatorFC->histo_dusg () );

  // discr no cut
  vector<TH1F*> discrNoCutHistos;
  discrNoCutHistos.push_back ( dDiscrCut->histo_all  () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_d    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_u    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_s    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_c    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_b    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_g    () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_ni   () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_dus  () );
  discrNoCutHistos.push_back ( dDiscrCut->histo_dusg () );

  // discr no cut
  vector<TH1F*> discrCutHistos;
  discrCutHistos.push_back ( dDiscrCutCond->histo_all  () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_d    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_u    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_s    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_c    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_b    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_g    () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_ni   () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_dus  () );
  discrCutHistos.push_back ( dDiscrCutCond->histo_dusg () );

  int dimHistos = discrCfHistos.size(); // they all have the same size

  // DISCR-CUT LOOP:
  // fill the histos for eff-pur computations by scanning the discriminatorFC histogram
  // do it in a way not to need to change the EffPur..... interface
  discrCut = discrFirstBin;

  // better to loop over bins -> discrCut no longer needed
  for ( int iDiscr = 0; iDiscr < discrBins; iDiscr++ ) {
    //
    // loop over flavours
    for ( int iFlav = 0; iFlav < dimHistos; iFlav++ ) {
      // fill all jets into NoCut histo
//       int    iBin      = discrNoCutHistos[iFlav]->FindBin    ( discrCut );
      double nJetsFlav = discrCfHistos   [iFlav]->GetEntries ()          ;
      discrNoCutHistos[iFlav]->SetBinContent ( iDiscr, nJetsFlav );
      discrNoCutHistos[iFlav]->SetBinError   ( iDiscr, sqrt(nJetsFlav) );
      // fill jets fulfilling discriminator >= discriminatorCut in Cut histo
      double sum = 0.0;
      for ( int i = iDiscr; i < discrCfHistos[iFlav]->GetSize(); i++ ) {
	sum += discrCfHistos[iFlav]->GetBinContent( i );
      }
      discrCutHistos[iFlav]->SetBinContent ( iDiscr, sum );
      discrCutHistos[iFlav]->SetBinError   ( iDiscr, sqrt(sum) );

    }

    // increase
//     discrCut += discrWidth;
  }


  // divide to get efficiency vs. discriminator cut from absolute numbers
  dDiscrCutCond->divide ( *dDiscrCut );  // does: histos including discriminator cut / flat histo
  // produce the misid. vs. eff histograms
  effPurFromHistos->compute();
  finalized = true;
}
void JetTagPlotter::write()
{
  TString dir= theExtensionString.Strip( TString::kLeading, '_');

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
    dDiscrCut->write();
    dDiscrCutCond->write();
    effPurFromHistos->write();
  }
  gFile->cd();
}
