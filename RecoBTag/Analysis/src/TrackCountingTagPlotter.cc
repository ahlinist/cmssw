#include "RecoBTag/Analysis/interface/TrackCountingTagPlotter.h"

TrackCountingTagPlotter::TrackCountingTagPlotter(JetTagPlotter *jetTagPlotter,
	bool update) :
    BaseBTagPlotter(jetTagPlotter->etaPtBin()), jetTagPlotter_(jetTagPlotter)
{
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

void TrackCountingTagPlotter::psPlot(const TString & name)
{
  jetTagPlotter_->psPlot(name);

  TString cName = "TrackCountingPlots"+ theExtensionString;
  TCanvas canvas(cName,
  	"TrackCountingPlots"+ theExtensionString, 600, 900);
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");
  canvas.cd(1);
  hSignificance->plot(0);

  for(int n=0; n < 4; n++) {
    canvas.cd(1+n);
    tkcntHistosSig[n]->plot(0);
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
  gFile->cd();
}

void TrackCountingTagPlotter::epsPlot(const TString & name)
{
  jetTagPlotter_->epsPlot(name);
  for(int n=0; n < 4; n++) tkcntHistosSig[n]->epsPlot(name);
  hSignificance->epsPlot(name);
}
