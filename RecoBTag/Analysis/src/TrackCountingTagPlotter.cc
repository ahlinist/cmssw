#include "RecoBTag/Analysis/interface/TrackCountingTagPlotter.h"

TrackCountingTagPlotter::TrackCountingTagPlotter(JetTagPlotter *jetTagPlotter,
	bool update) :
    BaseBTagPlotter(jetTagPlotter->etaPtBin()), jetTagPlotter_(jetTagPlotter)
{
  finalized = false;
  if (update){
  TString dir= "TrackCounting"+theExtensionString;
  gFile->cd();
  gFile->cd(dir);
  }
  hSignificance = new FlavourHistorgrams<double>
       ("ips" + theExtensionString, "Significance of impact parameter",
	50, -10.0, 10.0, false, false, false, "b", update) ;

  tkcntHistosSig[0] = new FlavourHistorgrams<double>
       ("ips1" + theExtensionString, "Significance of impact parameter 1st trk",
	50, -10.0, 10.0, false, false, false, "b", update) ;

  tkcntHistosSig[1] = new FlavourHistorgrams<double>
       ("ips2" + theExtensionString, "Significance of impact parameter 2nd trk",
	50, -10.0, 10.0, false, false, false, "b", update) ;

  tkcntHistosSig[2] = new FlavourHistorgrams<double>
       ("ips3" + theExtensionString, "Significance of impact parameter 3rd trk",
	50, -10.0, 10.0, false, false, false, "b", update) ;

  tkcntHistosSig[3] = new FlavourHistorgrams<double>
       ("ips4" + theExtensionString, "Significance of impact parameter 4th trk",
	50, -10.0, 10.0, false, false, false, "b", update) ;

}


TrackCountingTagPlotter::~TrackCountingTagPlotter ()
{

  delete jetTagPlotter_;

  for(int n=0; n < 4; n++) delete tkcntHistosSig[n];
  delete hSignificance;
  if (finalized) {
    delete effPurFromHistos2;
    delete effPurFromHistos3;
  }
}


void TrackCountingTagPlotter::analyzeTag (const reco::TrackCountingTagInfo & tagInfo,
	const reco::JetTag & jetTag)
{

  //fixme:
  int jetFlav = 5;

  jetTagPlotter_->analyzeJetTag(jetTag);

  // check if properly initialized

    //fixme: method to set 2d or 3d:

  int numberOfTracks = jetTag.tracks().size();

  for(int n=0; n < numberOfTracks && n < 4; n++)
    tkcntHistosSig[n]->fill(jetFlav, tagInfo.significance(n,0));
  for(int n=0; n < numberOfTracks; n++)
      hSignificance->fill(jetFlav, tagInfo.significance(n,0));
}

void TrackCountingTagPlotter::finalize ()
{
  jetTagPlotter_->finalize();
  //
  // final processing:
  // produce the misid. vs. eff histograms
  //
  effPurFromHistos2 = new EffPurFromHistos (tkcntHistosSig[1],
		jetTagPlotter_->nBinEffPur(), jetTagPlotter_->startEffPur(),
		jetTagPlotter_->endEffPur());
  effPurFromHistos2->compute();
  effPurFromHistos3 = new EffPurFromHistos (tkcntHistosSig[2],
		jetTagPlotter_->nBinEffPur(), jetTagPlotter_->startEffPur(),
		jetTagPlotter_->endEffPur());
  effPurFromHistos3->compute();
  finalized = true;
}

void TrackCountingTagPlotter::psPlot(const TString & name)
{
  jetTagPlotter_->psPlot(name);

  TString cName = "TrackCountingPlots"+ theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "TrackCountingPlots"+ theExtensionString, 600, 900);
  canvas.UseCurrentStyle();
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  hSignificance->plot((TPad*) canvas.GetPrimitive(cName+"_1"));

  for(int n=0; n < 4; n++) {
    canvas.cd(2+n);
    tkcntHistosSig[n]->plot((TPad*) canvas.GetPrimitive(cName+"_"+itos(n+2)));
  }
  if (finalized) {
    canvas.Print(name + cName + ".ps");
    canvas.Clear();
    canvas.Divide(2,3);
    canvas.cd(1);
    effPurFromHistos2->discriminatorNoCutEffic()->plot((TPad*) canvas.GetPrimitive(cName+"_1"));
    canvas.cd(2);
    effPurFromHistos2->discriminatorCutEfficScan()->plot((TPad*) canvas.GetPrimitive(cName+"_2"));
    canvas.cd(3);
    effPurFromHistos2->plot((TPad*) canvas.GetPrimitive(cName+"_3"));
    canvas.cd(4);
    effPurFromHistos3->discriminatorNoCutEffic()->plot((TPad*) canvas.GetPrimitive(cName+"_4"));
    canvas.cd(5);
    effPurFromHistos3->discriminatorCutEfficScan()->plot((TPad*) canvas.GetPrimitive(cName+"_5"));
    canvas.cd(6);
    effPurFromHistos3->plot((TPad*) canvas.GetPrimitive(cName+"_6"));
  }

  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void TrackCountingTagPlotter::write()
{
  jetTagPlotter_->write();

  TString dir= "TrackCounting"+theExtensionString;
  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);
  for(int n=0; n < 4; n++) tkcntHistosSig[n]->write();
  hSignificance->write();
  if (finalized) {
    effPurFromHistos2->write();
    effPurFromHistos3->write();
  }
  gFile->cd();
}

void TrackCountingTagPlotter::epsPlot(const TString & name)
{
  jetTagPlotter_->epsPlot(name);
  for(int n=0; n < 4; n++) tkcntHistosSig[n]->epsPlot(name);
  hSignificance->epsPlot(name);
  if (finalized) {
    effPurFromHistos2->epsPlot(name);
    effPurFromHistos3->epsPlot(name);
  }
}
