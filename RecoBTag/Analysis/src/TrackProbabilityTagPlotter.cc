#include "RecoBTag/Analysis/interface/TrackProbabilityTagPlotter.h"

TrackProbabilityTagPlotter::TrackProbabilityTagPlotter(const TString & tagName,
	const EtaPtBin & etaPtBin, int nBinEffPur, double startEffPur, double endEffPur,
	bool update) :
    BaseBTagPlotter(tagName, etaPtBin, nBinEffPur, startEffPur, endEffPur)
{
  finalized = false;
  if (update){
  TString dir= "TrackProbability"+theExtensionString;
  gFile->cd();
  gFile->cd(dir);
  }
  tkcntHistosSig3D[4] = new FlavourHistorgrams<double>
       ("ips_3D" + theExtensionString, "3D Probability of impact parameter",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig3D[0] = new FlavourHistorgrams<double>
       ("ips1_3D" + theExtensionString, "3D Probability of impact parameter 1st trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig3D[1] = new FlavourHistorgrams<double>
       ("ips2_3D" + theExtensionString, "3D Probability of impact parameter 2nd trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig3D[2] = new FlavourHistorgrams<double>
       ("ips3_3D" + theExtensionString, "3D Probability of impact parameter 3rd trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig3D[3] = new FlavourHistorgrams<double>
       ("ips4_3D" + theExtensionString, "3D Probability of impact parameter 4th trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig2D[4] = new FlavourHistorgrams<double>
       ("ips_2D" + theExtensionString, "2D Probability of impact parameter",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig2D[0] = new FlavourHistorgrams<double>
       ("ips1_2D" + theExtensionString, "2D Probability of impact parameter 1st trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig2D[1] = new FlavourHistorgrams<double>
       ("ips2_2D" + theExtensionString, "2D Probability of impact parameter 2nd trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig2D[2] = new FlavourHistorgrams<double>
       ("ips3_2D" + theExtensionString, "2D Probability of impact parameter 3rd trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

  tkcntHistosSig2D[3] = new FlavourHistorgrams<double>
       ("ips4" + theExtensionString, "2D Probability of impact parameter 4th trk",
	50, -1.0, 1.0, false, true, true, "b", update) ;

}


TrackProbabilityTagPlotter::~TrackProbabilityTagPlotter ()
{

  for(int n=0; n <= 4; n++) {
    delete tkcntHistosSig2D[n];
    delete tkcntHistosSig3D[n];
  }
  if (finalized) {
    for(int n=0; n < 4; n++) delete effPurFromHistos[n];
  }
}


void TrackProbabilityTagPlotter::analyzeTag (const reco::JetTag & jetTag,
	const JetFlavour & jetFlavour)
{
  const reco::TrackProbabilityTagInfo * tagInfo = 
	dynamic_cast<const reco::TrackProbabilityTagInfo *>(jetTag.tagInfoRef().get());

  if (!tagInfo) {
    throw cms::Exception("Configuration")
      << "BTagPerformanceAnalyzer: Extended TagInfo attached to selected JetTag not of type TrackProbabilityTagInfo. " << endl
      << "Select a different JetTag collection or change the algorithm parameter to TrackProbability in the configuration\n";
  }

  int jetFlav = jetFlavour.flavour();

  int numberOfTracks = jetTag.tracks().size();

  for(int n=0; n < tagInfo->selectedTracks(1) && n < 4; n++)
    tkcntHistosSig2D[n]->fill(jetFlav, tagInfo->probability(n,1));
  for(int n=0; n < tagInfo->selectedTracks(0) && n < 4; n++)
    tkcntHistosSig3D[n]->fill(jetFlav, tagInfo->probability(n,0));

  for(int n=0; n < tagInfo->selectedTracks(1); n++)
    tkcntHistosSig2D[4]->fill(jetFlav, tagInfo->probability(n,1));
  for(int n=0; n < tagInfo->selectedTracks(0); n++)
    tkcntHistosSig3D[4]->fill(jetFlav, tagInfo->probability(n,0));
}

void TrackProbabilityTagPlotter::finalize ()
{
  //
  // final processing:
  // produce the misid. vs. eff histograms
  //
  effPurFromHistos[0] = new EffPurFromHistos (tkcntHistosSig3D[1],
		nBinEffPur(), startEffPur(), endEffPur());
  effPurFromHistos[1] = new EffPurFromHistos (tkcntHistosSig3D[2],
		nBinEffPur(), startEffPur(), endEffPur());
  effPurFromHistos[2] = new EffPurFromHistos (tkcntHistosSig2D[1],
		nBinEffPur(), startEffPur(), endEffPur());
  effPurFromHistos[3] = new EffPurFromHistos (tkcntHistosSig2D[2],
		nBinEffPur(), startEffPur(), endEffPur());
  for(int n=0; n < 4; n++) effPurFromHistos[n]->compute();
  finalized = true;
}

void TrackProbabilityTagPlotter::psPlot(const TString & name)
{
  TString cName = "TrackProbabilityPlots"+ theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "TrackProbabilityPlots"+ theExtensionString, 600, 900);
  canvas.UseCurrentStyle();
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);

  tkcntHistosSig3D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(2+n);
    tkcntHistosSig3D[n]->plot();
  }

  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);

  canvas.cd(1);
  tkcntHistosSig2D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(2+n);
    tkcntHistosSig2D[n]->plot();
  }

  if (finalized) {
    for(int n=0; n < 2; n++) {
      canvas.Print(name + cName + ".ps");
      canvas.Clear();
      canvas.Divide(2,3);
      canvas.cd(1);
      effPurFromHistos[0+n]->discriminatorNoCutEffic()->plot();
      canvas.cd(2);
      effPurFromHistos[0+n]->discriminatorCutEfficScan()->plot();
      canvas.cd(3);
      effPurFromHistos[0+n]->plot();
      canvas.cd(4);
      effPurFromHistos[1+n]->discriminatorNoCutEffic()->plot();
      canvas.cd(5);
      effPurFromHistos[1+n]->discriminatorCutEfficScan()->plot();
      canvas.cd(6);
      effPurFromHistos[1+n]->plot();
    }
  }

  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void TrackProbabilityTagPlotter::write()
{
  TString dir= "TrackProbability"+theExtensionString;
  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);
  for(int n=0; n <= 4; n++) {
    tkcntHistosSig2D[n]->write();
    tkcntHistosSig3D[n]->write();
  }
  if (finalized) {
    for(int n=0; n < 4; n++) effPurFromHistos[n]->write();
  }
  gFile->cd();
}

void TrackProbabilityTagPlotter::epsPlot(const TString & name)
{
  for(int n=0; n <= 4; n++) {
    tkcntHistosSig2D[n]->epsPlot(name);
    tkcntHistosSig3D[n]->epsPlot(name);
  }
  if (finalized) {
    for(int n=0; n < 4; n++) effPurFromHistos[n]->epsPlot(name);
  }
}
