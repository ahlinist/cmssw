#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoBTag/Analysis/interface/TrackIPTagPlotter.h"

TrackIPTagPlotter::TrackIPTagPlotter(const TString & tagName,
	const EtaPtBin & etaPtBin, const edm::ParameterSet& pSet, bool update) :
	BaseTagInfoPlotter(tagName, etaPtBin)
{
  nBinEffPur_  = pSet.getParameter<int>("nBinEffPur");
  startEffPur_ = pSet.getParameter<double>("startEffPur");
  endEffPur_   = pSet.getParameter<double>("endEffPur");

  finalized = false;
  if (update){
  TString dir= "TrackIP"+theExtensionString;
  gFile->cd();
  gFile->cd(dir);
  }

  trkNbr3D = new FlavourHistograms<int>
	("selTrksNbr_3D" + theExtensionString, "Number of selected tracks for 3D IPS", 31, -0.5, 30.5,
	false, true, true, "b", update);

  trkNbr2D = new FlavourHistograms<int>
	("selTrksNbr_2D" + theExtensionString, "Number of selected tracks for 2D IPS", 31, -0.5, 30.5,
	false, true, true, "b", update);

  lowerIPSBound = -35.0;
  upperIPSBound = +35.0;

  tkcntHistosSig3D[4] = new FlavourHistograms<double>
       ("ips_3D" + theExtensionString, "3D IP significance",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig3D[0] = new FlavourHistograms<double>
       ("ips1_3D" + theExtensionString, "3D IP significance 1.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig3D[1] = new FlavourHistograms<double>
       ("ips2_3D" + theExtensionString, "3D IP significance 2.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig3D[2] = new FlavourHistograms<double>
       ("ips3_3D" + theExtensionString, "3D IP significance 3.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig3D[3] = new FlavourHistograms<double>
       ("ips4_3D" + theExtensionString, "3D IP significance 4.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig2D[4] = new FlavourHistograms<double>
       ("ips_2D" + theExtensionString, "2D IP significance",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig2D[0] = new FlavourHistograms<double>
       ("ips1_2D" + theExtensionString, "2D IP significance 1.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig2D[1] = new FlavourHistograms<double>
       ("ips2_2D" + theExtensionString, "2D IP significance 2.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig2D[2] = new FlavourHistograms<double>
       ("ips3_2D" + theExtensionString, "2D IP significance 3.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosSig2D[3] = new FlavourHistograms<double>
       ("ips4" + theExtensionString, "2D IP significance 4.trk",
	50, lowerIPSBound, upperIPSBound, false, true, true, "b", update) ;

  tkcntHistosProb3D[4] = new FlavourHistograms<float>
       ("prob_3D" + theExtensionString, "3D IP probability",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb3D[0] = new FlavourHistograms<float>
       ("prob1_3D" + theExtensionString, "3D IP probability 1.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb3D[1] = new FlavourHistograms<float>
       ("prob2_3D" + theExtensionString, "3D IP probability 2.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb3D[2] = new FlavourHistograms<float>
       ("prob3_3D" + theExtensionString, "3D IP probability 3.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb3D[3] = new FlavourHistograms<float>
       ("prob4_3D" + theExtensionString, "3D IP probability 4.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb2D[4] = new FlavourHistograms<float>
       ("prob_2D" + theExtensionString, "2D IP probability",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb2D[0] = new FlavourHistograms<float>
       ("prob1_2D" + theExtensionString, "2D IP probability 1.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb2D[1] = new FlavourHistograms<float>
       ("prob2_2D" + theExtensionString, "2D IP probability 2.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb2D[2] = new FlavourHistograms<float>
       ("prob3_2D" + theExtensionString, "2D IP probability 3.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  tkcntHistosProb2D[3] = new FlavourHistograms<float>
       ("prob4" + theExtensionString, "2D IP probability 4.trk",
	50, -1.1, 1.1, false, true, true, "b", update) ;

  decayLengthValuHisto = new FlavourHistograms<double>
       ("decLen" + theExtensionString, "Decay Length",
	50, -5.0, 5.0, false, true, true, "b", update);
  jetDistanceValuHisto = new FlavourHistograms<double>
       ("jetDist" + theExtensionString, "JetDistance",
	50, -0.1, 0.1, false, true, true, "b", update);
  jetDistanceSignHisto = new FlavourHistograms<double>
       ("jetDistSign" + theExtensionString, "JetDistance significance",
	50, -100.0, 100.0, false, true, true, "b", update);

}


TrackIPTagPlotter::~TrackIPTagPlotter ()
{

  delete trkNbr3D;
  delete trkNbr2D;
  delete decayLengthValuHisto;
  delete jetDistanceValuHisto;
  delete jetDistanceSignHisto;

  for(int n=0; n <= 4; n++) {
    delete tkcntHistosSig2D[n];
    delete tkcntHistosSig3D[n];
    delete tkcntHistosProb2D[n];
    delete tkcntHistosProb3D[n];
  }
  if (finalized) {
    for(int n=0; n < 4; n++) delete effPurFromHistos[n];
  }
}


void TrackIPTagPlotter::analyzeTag (const reco::BaseTagInfo * baseTagInfo,
	const BTagMCTools::JetFlavour & jetFlavour)
{

  const reco::TrackIPTagInfo * tagInfo = 
	dynamic_cast<const reco::TrackIPTagInfo *>(baseTagInfo);

  if (!tagInfo) {
    throw cms::Exception("Configuration")
      << "BTagPerformanceAnalyzer: Extended TagInfo not of type TrackIPTagInfo. " << endl;
  }

  int jetFlav = jetFlavour.flavour();

  vector<reco::TrackIPTagInfo::TrackIPData> ip = tagInfo->impactParameterData();

  vector<float> prob2d, prob3d;
  if (tagInfo->hasProbabilities()) {
    prob2d = tagInfo->probabilities(0);	
    prob3d = tagInfo->probabilities(1);	
  }

  trkNbr3D->fill(jetFlav, ip.size());
  trkNbr2D->fill(jetFlav, ip.size());

  vector<unsigned int> sortedIndices = tagInfo->sortedIndexes(reco::TrackIPTagInfo::IP2DSig);
  for(unsigned int n=0; n < sortedIndices.size() && n < 4; n++) {
    tkcntHistosSig2D[n]->fill(jetFlav, ip[sortedIndices[n]].ip2d.significance());
  }
  sortedIndices = tagInfo->sortedIndexes(reco::TrackIPTagInfo::Prob2D);
  for(unsigned int n=0; n < sortedIndices.size() && n < 4; n++) {
    tkcntHistosProb2D[n]->fill(jetFlav, prob2d[sortedIndices[n]]);
  }
  for(unsigned int n=sortedIndices.size(); n < 4; n++)
    tkcntHistosSig2D[n]->fill(jetFlav, lowerIPSBound-1.0);

  sortedIndices = tagInfo->sortedIndexes(reco::TrackIPTagInfo::IP3DSig);
  for(unsigned int n=0; n < sortedIndices.size() && n < 4; n++) {
    tkcntHistosSig3D[n]->fill(jetFlav, ip[sortedIndices[n]].ip3d.significance());
  }
  sortedIndices = tagInfo->sortedIndexes(reco::TrackIPTagInfo::Prob3D);
  for(unsigned int n=0; n < sortedIndices.size() && n < 4; n++) {
    tkcntHistosProb3D[n]->fill(jetFlav, prob3d[sortedIndices[n]]);
  }
  for(unsigned int n=sortedIndices.size(); n < 4; n++)
    tkcntHistosSig3D[n]->fill(jetFlav, lowerIPSBound-1.0);

  for(unsigned int n=0; n < ip.size(); n++) {
    tkcntHistosSig2D[4]->fill(jetFlav, ip[n].ip2d.significance());
    tkcntHistosProb2D[4]->fill(jetFlav, prob2d[n]);
  }
  for(unsigned int n=0; n < ip.size(); n++) {
    tkcntHistosSig3D[4]->fill(jetFlav, ip[n].ip3d.significance());
    tkcntHistosProb3D[4]->fill(jetFlav, prob3d[n]);
  }
  GlobalPoint pv(tagInfo->primaryVertex()->position().x(),
                 tagInfo->primaryVertex()->position().y(),
                 tagInfo->primaryVertex()->position().z());
  for(unsigned int n=0; n < ip.size(); n++) {
    double decayLen = (ip[n].closestToJetAxis - pv).mag();
    decayLengthValuHisto->fill(jetFlav, decayLen);
  }
  for(unsigned int n=0; n < ip.size(); n++) {
    jetDistanceValuHisto->fill(jetFlav, ip[n].distanceToJetAxis);
  }

}

void TrackIPTagPlotter::finalize ()
{
  //
  // final processing:
  // produce the misid. vs. eff histograms
  //
  effPurFromHistos[0] = new EffPurFromHistos (tkcntHistosSig3D[1],
		nBinEffPur_, startEffPur_,
		endEffPur_);
  effPurFromHistos[1] = new EffPurFromHistos (tkcntHistosSig3D[2],
		nBinEffPur_, startEffPur_,
		endEffPur_);
  effPurFromHistos[2] = new EffPurFromHistos (tkcntHistosSig2D[1],
		nBinEffPur_, startEffPur_,
		endEffPur_);
  effPurFromHistos[3] = new EffPurFromHistos (tkcntHistosSig2D[2],
		nBinEffPur_, startEffPur_,
		endEffPur_);
  for(int n=0; n < 4; n++) effPurFromHistos[n]->compute();
  finalized = true;
}

void TrackIPTagPlotter::psPlot(const TString & name)
{
  TString cName = "TrackIPPlots"+ theExtensionString;
  setTDRStyle()->cd();
  TCanvas canvas(cName, "TrackIPPlots"+ theExtensionString, 600, 900);
  canvas.UseCurrentStyle();
  canvas.Divide(2,3);
  canvas.Print(name + cName + ".ps[");

  canvas.cd(1);
  trkNbr3D->plot();
  canvas.cd(2);
  tkcntHistosSig3D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(3+n);
    tkcntHistosSig3D[n]->plot();
  }

  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);

  canvas.cd(1);
  trkNbr3D->plot();
  canvas.cd(2);
  tkcntHistosProb3D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(3+n);
    tkcntHistosProb3D[n]->plot();
  }

  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);
  canvas.cd(1);
  trkNbr2D->plot();
  canvas.cd(2);
  tkcntHistosSig2D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(3+n);
    tkcntHistosSig2D[n]->plot();
  }

  canvas.Print(name + cName + ".ps");
  canvas.Clear();
  canvas.Divide(2,3);
  canvas.cd(1);
  trkNbr2D->plot();
  canvas.cd(2);
  tkcntHistosProb2D[4]->plot();
  for(int n=0; n < 4; n++) {
    canvas.cd(3+n);
    tkcntHistosProb2D[n]->plot();
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
  canvas.Clear();
  canvas.Divide(1,3);
  canvas.cd(1);
  jetDistanceValuHisto->plot();
  canvas.cd(2);
  jetDistanceSignHisto->plot();
  canvas.cd(3);
  decayLengthValuHisto->plot();


  canvas.Print(name + cName + ".ps");
  canvas.Print(name + cName + ".ps]");
}

void TrackIPTagPlotter::write(const bool allHisto)
{
  TString dir= "TrackIP"+theExtensionString;
  gFile->cd();
  gFile->mkdir(dir);
  gFile->cd(dir);
  trkNbr2D->write(allHisto);
  trkNbr3D->write(allHisto);
  decayLengthValuHisto->write(allHisto);
  jetDistanceValuHisto->write(allHisto);
  jetDistanceSignHisto->write(allHisto);
  for(int n=0; n <= 4; n++) {
    tkcntHistosSig2D[n]->write(allHisto);
    tkcntHistosSig3D[n]->write(allHisto);
    tkcntHistosProb2D[n]->write(allHisto);
    tkcntHistosProb3D[n]->write(allHisto);
  }
  if (finalized) {
    for(int n=0; n < 4; n++) effPurFromHistos[n]->write(allHisto);
  }
  gFile->cd();
}

void TrackIPTagPlotter::epsPlot(const TString & name)
{
  trkNbr2D->epsPlot(name);
  trkNbr3D->epsPlot(name);
  decayLengthValuHisto->epsPlot(name);
  decayLengthSignHisto->epsPlot(name);
  jetDistanceValuHisto->epsPlot(name);
  jetDistanceSignHisto->epsPlot(name);
  for(int n=0; n <= 4; n++) {
    tkcntHistosSig2D[n]->epsPlot(name);
    tkcntHistosSig3D[n]->epsPlot(name);
    tkcntHistosProb2D[n]->epsPlot(name);
    tkcntHistosProb3D[n]->epsPlot(name);
  }
  if (finalized) {
    for(int n=0; n < 4; n++) effPurFromHistos[n]->epsPlot(name);
  }
}
