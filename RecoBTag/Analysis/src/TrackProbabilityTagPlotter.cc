#include "RecoBTag/Analysis/interface/TrackProbabilityTagPlotter.h"


TrackProbabilityTagPlotter::TrackProbabilityTagPlotter(JetTagPlotter *jetTagPlotter,
	bool update) :
    BaseBTagPlotter(jetTagPlotter->etaPtBin()), jetTagPlotter_(jetTagPlotter)
{

  if (update){
  TString dir= "TrackProbability"+theExtensionString;
  gFile->cd();
  gFile->cd(dir);
  }
  hSignificance = new FlavourHistorgrams<double>
	("jet probability", "Jet probability",
	 50, 0.0, 1.0, false, false, true, "b", update);

  tkprobHistosSig[0] = new FlavourHistorgrams<double>
	("1st track probability", "Significance of impact parameter",
	 50, 0.0, 1.0, false, false, true, "b", update);

  tkprobHistosSig[1] = new FlavourHistorgrams<double>
	("2nd track probability", "Significance of impact parameter",
	 50, 0.0, 1.0, false, false, true, "b", update);

  tkprobHistosSig[2] = new FlavourHistorgrams<double>
	("3rd track probability", "Significance of impact parameter",
	 50, 0.0, 1.0, false, false, true, "b", update);

  tkprobHistosSig[3] = new FlavourHistorgrams<double>
	("4th track probability", "Significance of impact parameter",
	 50, 0.0, 1.0, false, false, true, "b", update);

}


TrackProbabilityTagPlotter::~TrackProbabilityTagPlotter ()
{

  delete jetTagPlotter_;

  for(int n=0; n < 4; n++) delete tkprobHistosSig[n];
  delete hSignificance;
}


void TrackProbabilityTagPlotter::analyzeTag (const reco::TrackCountingTagInfo & tagInfo,
	const reco::JetTag & jetTag, const JetFlavour & jetFlavour)
{

  int jetFlav = jetFlavour.flavour();
  jetTagPlotter_->analyzeJetTag(jetTag, jetFlavour);

  // check if properly initialized

    //fixme: method to set 2d or 3d:

  int numberOfTracks = jetTag.tracks().size();

  for(int n=0; n < numberOfTracks && n < 4; n++)
    tkprobHistosSig[n]->fill(jetFlav, tagInfo.significance(n,0));
  for(int n=0; n < numberOfTracks; n++)
      hSignificance->fill(jetFlav, tagInfo.significance(n,0));
}

void TrackProbabilityTagPlotter::psPlot(const TString & name)
{
  jetTagPlotter_->psPlot(name);

  TString cName = "TrackProbabilityPlots"+ theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "TrackProbabilityPlots"+ theExtensionString, 600, 900);
  canvas.UseCurrentStyle();
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  hSignificance->plot(0);

  for(int n=0; n < 4; n++) {
    canvas.cd(2+n);
    tkprobHistosSig[n]->plot((TPad*) canvas.GetPrimitive(cName+"_"+itos(n+2)));
  }

  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void TrackProbabilityTagPlotter::write()
{
  jetTagPlotter_->write();

  TString dir= "TrackProbability"+theExtensionString;
  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);
  for(int n=0; n < 4; n++) tkprobHistosSig[n]->write();
  hSignificance->write();
  gFile->cd();
}

void TrackProbabilityTagPlotter::epsPlot(const TString & name)
{
  jetTagPlotter_->epsPlot(name);
  for(int n=0; n < 4; n++) tkprobHistosSig[n]->epsPlot(name);
  hSignificance->epsPlot(name);
}
