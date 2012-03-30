/**********************************************
Implementation of GraphicalOutputSimple_root class
COnor Henderson 2006
**********************************************/


#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/GraphicalOutputSimple_root.hh"
using namespace std;



/******   Constructor   ******/

GraphicalOutputSimple_root::GraphicalOutputSimple_root(string _pawCommand, int NBkgContributions)
{
  pawCommand = _pawCommand;
  paneNumber = 0; // Here paneNumber is effectively pageNumber
  //Keep count of all distrbns plotted, so we can give them unique names
  distributionNumber = 0; 

  // number of bkg sources to show ('other' counts as 1)
  fNBkgContributions = NBkgContributions; // defaults to 5
  if(fNBkgContributions>13) fNBkgContributions=13;
}


/*****  Utility Methods  *****/

void GraphicalOutputSimple_root::addUnits(string& s)
{
  //add units
  string units = "";
  if(s=="zvtx")
    units = " (cm)";
  else if(s=="runHistory")
    units = " (1/pb)";
  else if(s.find("/")!=string::npos)
    units = "";
  else if((s.find("mass")!=string::npos)||
	  (s.find("Mass")!=string::npos))	  
    units = " (GeV)";
  else if((s.find("_eta")!=string::npos)||
	  (s.find("_detEta")!=string::npos)||
	  (s.find(" eta")!=string::npos)||
	  (s.find(" detEta")!=string::npos)||
	  (s.find("eltaEta")!=string::npos)||
	  (s.find("eltaR")!=string::npos)
	  )
    units = "";  // no units for pseudorapidity
  else if((s.find("delta")!=string::npos)||(s.find("Delta")!=string::npos)||
	  (s.find("theta")!=string::npos)||
	  (s.find("angle")!=string::npos)||
	  (s.find("phi")!=string::npos))
    units = " (radians)";
  else
    units = " (GeV)";

  s+=units;
}

void GraphicalOutputSimple_root::reformatDistributionName(string& distributionName)
{
  reformatStringForRoot(distributionName);
  addUnits(distributionName);
}


void GraphicalOutputSimple_root::reformatStringForRoot(string& s)
{
  // take the s which is paw-formatted
  // and make it root-formatted.
  // This entails:
  // replace #_# with _
  // replace ^+! with ^{+}
  // replace ^-! with ^{-}
  // replace p&/ with #slash{p}
  // replace [m] with #mu
  // replace [t] with #tau
  // replace [g] with #gamma
  // keep greek letters as words
  // escape () so as not to interpret them as functions

  // try using std::string function find and replace
  //  s = Math::replaceSubString(s,"#-#","_");
  s = Math::replaceSubString(s,"#-#"," "); // eg 'j1_pt' becomes 'j1 pt'
  s = Math::replaceSubString(s,"^+!","^{+}");
  s = Math::replaceSubString(s,"^-!","^{-}");
  s = Math::replaceSubString(s,"[m]","#mu");
  s = Math::replaceSubString(s,"[t]","#tau");
  s = Math::replaceSubString(s,"[g]","#gamma");
  s = Math::replaceSubString(s," gamma"," #gamma");
  s = Math::replaceSubString(s,"mu v","#mu#nu");
  s = Math::replaceSubString(s,"muv","#mu#nu");
  s = Math::replaceSubString(s,"munu","#mu#nu");
  s = Math::replaceSubString(s,"enu","e#nu");
  s = Math::replaceSubString(s,"mu mu","#mu#mu");
  s = Math::replaceSubString(s,"mumu","#mu#mu");
  s = Math::replaceSubString(s,"tautau","#tau#tau");
  s = Math::replaceSubString(s,"taunu","#tau#nu");
  s = Math::replaceSubString(s," mu"," #mu");
  // specifically to replace 'pmiss pt' with slash{p}_{T}
  s = Math::replaceSubString(s,"p&/ pt","#slash{p}_{T}");
  s = Math::replaceSubString(s,"p&/ pt","#slash{p}_{T}");
  s = Math::replaceSubString(s,"p&/","pmiss");
  s = Math::replaceSubString(s,"/&p","pmiss");

  // for delta and phi
  s = Math::replaceSubString(s,"Delta","#Delta");
  s = Math::replaceSubString(s,"delta","#Delta");
  s = Math::replaceSubString(s,"phi","#phi");
  s = Math::replaceSubString(s,"cos(theta","cos(#theta");
  //  s = Math::replaceSubString(s,"eta","#eta");
  // mass and transversemass
  s = Math::replaceSubString(s,"transversemass","Mt");
  s = Math::replaceSubString(s,"mass","M");
  s = Math::replaceSubString(s," pt"," p_{T}");
  s = Math::replaceSubString(s,"'generated sumPt'","Generated #sump_{T}");
  //  s = Math::replaceSubString(s,"sumPt","sum pT");
  s = Math::replaceSubString(s,"->","#rightarrow ");
  // make particle labels subscripts?
  //  s = Math::replaceSubString(s,"j1","j_{1}");
  //  s = Math::replaceSubString(s,"j2","j_{2}");
  //  s = Math::replaceSubString(s,"j3","j_{3}");
  //  s = Math::replaceSubString(s,"j4","j_{4}");
  //  s = Math::replaceSubString(s,"j5","j_{5}");
  //  s = Math::replaceSubString(s,"j6","j_{6}");
}

string GraphicalOutputSimple_root::addPreliminaryLabel(string colliderRun, string experiment)
{
  string labelCommand = "";
  string preliminaryLabel = "";
  if ( colliderRun == "tev2" ){
    if ( experiment == "cdf" )
      // preliminaryLabel = "CDF Run II Preliminary (927 pb^{-1})";
      preliminaryLabel = "CDF Run II Preliminary (2.0 fb^{-1})";
    if ( experiment == "d0" )
      preliminaryLabel = "D0 Run II Preliminary (90 pb^{-1})";
  }
  if ( colliderRun == "lep2" ){
    if ( experiment == "aleph" )
      preliminaryLabel = "";
    if ( experiment == "l3" )
      preliminaryLabel = "";
    if ( experiment == "l3+aleph" || experiment == "aleph+l3" )
      preliminaryLabel = "";
  }
  if ( colliderRun == "hera" ){
    if ( experiment == "h1" )
      preliminaryLabel = "";
  }
  if(getenv("noPreliminaryLabel")!=NULL)
    preliminaryLabel = "";

  labelCommand += "TLatex *cdf_label = new TLatex(0.165,0.85,\""+preliminaryLabel+"\");\n";
  labelCommand += "cdf_label->SetNDC();\n";
  labelCommand += "cdf_label->SetTextSize(0.04);\n";
  labelCommand += "cdf_label->Draw();\n";
  return labelCommand;
}

const int nLegendEntriesPerColumn = 6;
const int nLegendColumns = 6/nLegendEntriesPerColumn;

string GraphicalOutputSimple_root::addLegendEntry(string legendName, string histName, string text, string option, int n)
{
  //option = F, L or P
  string legendCommand = "";
  reformatStringForRoot(text);
//   if(n%nLegendEntriesPerColumn==0)
//     legendCommand += createLegend(legendName+"_"+Math::ftoa(n/nLegendEntriesPerColumn),n/nLegendEntriesPerColumn+1);
//   legendCommand += legendName+"_"+Math::ftoa(n/nLegendEntriesPerColumn)+"->AddEntry("+histName+",\""+text+"\",\""+option+"\");\n";
  legendCommand += legendName+"->AddEntry("+histName+",\""+text+"\",\""+option+"\");\n";
  return legendCommand;
}


string GraphicalOutputSimple_root::createLegend(string legendName, int n)
{
  string legendCommand = "";
  //  double xmin=((n-1.)/nLegendColumns)*0+0.6;
  double xmin=0.6;
  double ymin=0.75;
  //  double xmax=((n-0.)/nLegendColumns)*0+0.94;
  double xmax=0.95;
  double ymax=1.0;
  legendCommand += "TLegend *"+legendName+" = new TLegend("+Math::ftoa(xmin)+","+
    Math::ftoa(ymin)+","+Math::ftoa(xmax)+","+Math::ftoa(ymax)+");\n";
  legendCommand += legendName+"->SetBorderSize(1);\n";
  legendCommand += legendName+"->GetEntrySeparation();\n";
  legendCommand += legendName+"->SetEntrySeparation(0.01);\n";
  //mrenna to remove histo blotting out data pts
  //  legendCommand += legendName+"->SetFillStyle(1001);\n";
  //  legendCommand += legendName+"->SetFillColor(10);\n";
  return legendCommand;
}

string GraphicalOutputSimple_root::setGlobalStyle()
{
  string styleCommand = "";
  styleCommand += "gROOT->SetStyle(\"Plain\"); \n";
  styleCommand += "gStyle->SetOptStat(0);\n";
  styleCommand += "gStyle->SetOptTitle(0);\n";
  //styleCommand += "gStyle->SetTitleBorderSize(0);\n";
  styleCommand += "gStyle->SetTitleX(0.15);\n";
  styleCommand += "gStyle->SetTitleY(0.965);\n";
  //  styleCommand += "gStyle->SetTitleFontSize(0.052);\n";
  styleCommand += "gStyle->SetNdivisions(505,\"x\");\n";
  styleCommand += "gStyle->SetNdivisions(505,\"y\");\n";
  styleCommand += "gStyle->SetPadLeftMargin(0.15);\n";
  styleCommand += "gStyle->SetPadRightMargin(0.05);\n";
  styleCommand += "gStyle->SetPadBottomMargin(0.15);\n";
  styleCommand += "gStyle->SetErrorX(0);\n";   // dont plot error bars along x-axis
  styleCommand += "  gStyle->SetTitleYOffset(1.3);\n";
  styleCommand += "  gStyle->SetTitleXOffset(1.02);\n";
  styleCommand += "  gStyle->SetTitleYSize(0.06);\n";
  styleCommand += "  gStyle->SetTitleXSize(0.06);\n";
  styleCommand += "  gStyle->SetLabelSize(0.06,\"x\");\n";
  styleCommand += "  gStyle->SetLabelSize(0.06,\"y\");\n";
  

  //define colors for bkg hists
  styleCommand += defineBkgColors();
  return styleCommand;
}

string GraphicalOutputSimple_root::createHistogramTitle(string histTitle)
{
  // ROOT doesnt provide much functionality to set the location of the 
  // automatic histogram title
  // so we remove it (with gStyle->SetOptTitle(0) in the global style part)
  // and here create our own TLatex object to replace it
  string titleCommand="";
  titleCommand += "TLatex *hist_title = new TLatex(0.165,0.93,\""+histTitle+"\");\n";
  titleCommand += "hist_title->SetNDC();\n";
  titleCommand += "hist_title->SetTextSize(0.05);\n";
  titleCommand += "hist_title->Draw();\n";
  return titleCommand;
}

string GraphicalOutputSimple_root::defineBkgColors()
{
  // bkg colors are shades of pink
  // we will use up to five different shades
  //  TColor pink0(500,1.0,1.00,1.00,"pink0");
  // we later refer to them by number: 500+i
  string colorCommand="";
  if(fNBkgContributions<=5) {
    colorCommand += "TColor white(500,1.0,1.00,1.00,\"white\");\n";
    colorCommand += "TColor pink1(501,1.0,0.85,0.85,\"pink1\");\n";
    colorCommand += "TColor pink2(502,1.0,0.70,0.70,\"pink2\");\n";
    colorCommand += "TColor pink3(503,1.0,0.50,0.50,\"pink3\");\n";
    //  colorCommand += "TColor pink4(504,1.0,0.10,0.10,\"pink4\");\n";
    // make the last one completely red
    colorCommand += "TColor pink4(504,1.0,0.00,0.00,\"pink4\");\n";
    colorCommand += "TColor blue1(509,0.85,0.85,1.00,\"blue1\");\n";
  }
  else {
    // now we add more colors
    colorCommand += "TColor white(500,1.0,1.00,1.00,\"white\");\n";
    colorCommand += "TColor pink1(501,1.0,0.85,0.85,\"pink1\");\n";
    colorCommand += "TColor pink2(502,1.0,0.70,0.70,\"pink2\");\n";
    colorCommand += "TColor pink3(503,1.0,0.50,0.50,\"pink3\");\n";
    colorCommand += "TColor pink4(504,1.0,0.00,0.00,\"pink4\");\n";

    colorCommand += "TColor green1(505,0.85,1.0,0.85,\"green1\");\n";
    colorCommand += "TColor green2(506,0.70,1.0,0.70,\"green2\");\n";
    colorCommand += "TColor green3(507,0.50,1.0,0.50,\"green3\");\n";
    colorCommand += "TColor green4(508,0.00,1.0,0.00,\"green4\");\n";
    colorCommand += "TColor blue1(509,0.85,0.85,1.00,\"blue1\");\n";
    colorCommand += "TColor blue2(510,0.70,0.70,1.00,\"blue2\");\n";
    colorCommand += "TColor blue3(511,0.50,0.50,1.00,\"blue3\");\n";
    colorCommand += "TColor blue4(512,0.00,0.00,1.00,\"blue4\");\n";

  }
  return colorCommand;
}

string GraphicalOutputSimple_root::labelHistogramAxis(string histName, string axisLabel, string whichAxis)
{
  string axisLabelCommand = "";
  axisLabelCommand += histName+"->Set"+whichAxis+"Title(\""+axisLabel+"\");\n";
  axisLabelCommand += histName+"->Get"+whichAxis+"axis()->CenterTitle();\n"; // center title    
  return axisLabelCommand;
}

string GraphicalOutputSimple_root::createRootHistogram(string histName, string binEdgesArrayName)
{
  // create a histogram, given name of array containing binEdges
  // that array is presumed to have already been defined
  // TH1F *h1 = new TH1F("h1","h1",array.GetSize()-1,array.GetArray());
  string ans = "";
  ans += "TH1F *"+histName+" = new TH1F(\""+histName+"\",\""+histName+"\",";
  ans += binEdgesArrayName+".GetSize()-1,"+binEdgesArrayName+".GetArray()); \n";
  ans += histName+"->SetTitleSize(0.06);\n";
  return(ans);
}

string GraphicalOutputSimple_root::fillRootHistogram(string histName, string contentsArrayName)
{
  // fill histogram from contentsArray
  // both histogram and array presumed to have already been defined
  // contentsArray defines the contents _in each bin_ of the histogram
  string ans = "";
  ans += "for(Int_t i=0;i<"+contentsArrayName+".GetSize();i++) {\n";
  ans += histName+"->SetBinContent(i+1,"+contentsArrayName+"[i]);\n";
  ans += "}\n";
  return(ans);
}

string GraphicalOutputSimple_root::fillRootArray(string arrayName, const vector<double>& values)
{
  // code to create a ROOT array 
  //TArrayD arrayName(arraySize);
  // then fill
  string ans = "";
  size_t n = values.size();
  ans += "TArrayD "+arrayName+"("+Math::ftoa(n)+");\n"; 
  for(size_t i=0;i<n;i++) {
    ans += arrayName+"["+Math::ftoa(i)+"] = "+Math::ftoa(values[i])+"; \n";
  }
  return(ans);
}

string GraphicalOutputSimple_root::fillRootArray(string arrayName, const vector<int>& values)
{
  // code to create a ROOT array 
  //TArrayI arrayName(arraySize);
  // then fill
  string ans = "";
  size_t n = values.size();
  ans += "TArrayI "+arrayName+"("+Math::ftoa(n)+");\n"; 
  for(size_t i=0;i<n;i++) {
    ans += arrayName+"["+Math::ftoa(i)+"] = "+Math::ftoa(values[i])+"; \n";
  }
  return(ans);
}


string GraphicalOutputSimple_root::markRegionOnAxis(string distributionCode, double limitLeft, double limitRight)
{
  // Place a mark along the horizontal axis.
  string ans = "";
  ans += "TBox box_"+distributionCode+";\n";
  ans += "box_"+distributionCode+".SetFillStyle(1001);\n";
  ans += "box_"+distributionCode+".SetFillColor(4);\n";
  ans += "box_"+distributionCode+".DrawBox("+Math::ftoa(limitLeft)+",-0.002*dataHist_"+distributionCode+"->GetMaximum(),"+Math::ftoa(limitRight)+",-0.01*dataHist_"+distributionCode+"->GetMaximum());\n";
  ans += "TLine line_"+distributionCode+";\n";
  ans += "line_"+distributionCode+".SetLineColor(4);\n";
  ans += "line_"+distributionCode+".SetLineStyle(2);\n";
  ans += "line_"+distributionCode+".DrawLine("+Math::ftoa(limitLeft)+",-0.002*dataHist_"+distributionCode+"->GetMaximum(),"+Math::ftoa(limitLeft)+",0.5*dataHist_"+distributionCode+"->GetMaximum());";
  ans += "line_"+distributionCode+".DrawLine("+Math::ftoa(limitRight)+",-0.002*dataHist_"+distributionCode+"->GetMaximum(),"+Math::ftoa(limitRight)+",0.5*dataHist_"+distributionCode+"->GetMaximum());";

  return(ans);
}

//Add An Arrow on the x axis to indicate prescale divisions
void GraphicalOutputSimple_root::addPrescaleArrow(string label,double x,double YMAX) {
  pawCommand += "\nTArrow *arw" + label + " = new TArrow(" + Math::ftoa(x) + "," + Math::ftoa(YMAX) + "*0.06," + Math::ftoa(x) + ",0,0.01,\">\");\n";
  pawCommand += "arw" + label + "->SetLineWidth(2);\n";
  pawCommand += "arw" + label + "->Draw();\n";
  return;
}



/*****  Adornment Methods  *****/

void GraphicalOutputSimple_root::addComment(string comment)
{
  pawCommand += "//"+comment;
}


/**  New Page  **/

void GraphicalOutputSimple_root::newPage(int numberOfPlotsOnPage)
{
  if(numberOfPlotsOnPage!=1) 
    cout << "GraphicalOutputSimple_root only does one plot per page" <<endl;
  return;
}




/*****  Plot Methods  *****/

/**  1d  **/



void GraphicalOutputSimple_root::add1dPlot(string distributionName,
				      vector<double> binEdges,
				      vector<double> fs, vector<double> fb, vector<int> fd,
					   vector<bool> showBkgSigData)				   
{
  vector<string> sources;
  vector<double> sourceWeights;
  add1dPlot(distributionName, binEdges,
	    vector<vector<double> >(1,fs), 
	    vector<vector<double> >(1,fb), 
	    fd, showBkgSigData, sources, sourceWeights);
}


void GraphicalOutputSimple_root::add1dPlot(
		string distributionName,
		vector<double> binEdges,
		vector<vector<double> > fs, vector<vector<double> > fb, vector<int> fd,
		vector<bool> showBkgSigData, vector<string> sources, vector<double> sourceWeights,
		string colliderRun, string experiment,
		string finalState, vector<double> windowLimitsLeft, vector<double> windowLimitsRight
		)
{
  // this plots all data and bkg contributions for one distribution (eg object pt) for a final state
  string plotCommand = "";
  plotCommand += "// Begin distribution "+distributionName+"\n";
  string distributionName1 = distributionName;
  size_t nbins = binEdges.size()-1;
  assert(fb.size()<=13); // less than 13 bkg components
  assert(fs.size()<=13);

  for(size_t i=0; i<fb.size(); i++) {
    if(fb[i].empty()) {
      fb[i] = vector<double>(nbins,0);
    }
  }
  for(size_t i=0; i<fs.size(); i++){
    if(fs[i].empty()) {
      fs[i] = vector<double>(nbins,0);
    }
  }
  if(fd.empty()) {
    fd = vector<int>(nbins,0);
  }

  for(size_t i=1; i<fb.size(); i++)
    {
      // each bkg component is a vector where each element is the 
      // bkg contrbn for that bin of the distribution

      assert(fb[i].size()==nbins); 
      for(size_t j=0; j<nbins; j++){
	//keep building up the bkg components
	// so that each histogram can be plotted on top of the others
	fb[i][j] += fb[i-1][j]; 
      }
    }
  for(size_t i=1; i<fs.size(); i++)
    {
      assert(fs[i].size()==nbins);
      for(size_t j=0; j<nbins; j++)
	fs[i][j] += fs[i-1][j];
    }

  double histoYMAX = 0.;
  //int nx = nbins;
 
  assert(showBkgSigData.size()==3);
  if( (showBkgSigData[0]) &&
      (!fb.empty()) &&
      (histoYMAX < *max_element(fb.back().begin(), fb.back().end())))
    histoYMAX = *max_element(fb.back().begin(), fb.back().end());
  if( (showBkgSigData[1]) &&
      (!fs.empty()) &&
      (histoYMAX < *max_element(fs.back().begin(), fs.back().end())))
    histoYMAX = *max_element(fs.back().begin(), fs.back().end());
  if( showBkgSigData[2] &&
      (!fd.empty()) &&
      (histoYMAX < *max_element(fd.begin(), fd.end())))
    histoYMAX = *max_element(fd.begin(), fd.end());
  
  histoYMAX *= 1.05;
  // make it bigger to allow legend to fit
  histoYMAX *= 1.25;

  //add up all sourceWeights so they can be expressed as percentage
  double totalSourceWeight = 0;
  for(size_t i=0;i<sourceWeights.size();i++) {
    totalSourceWeight += sourceWeights[i];
  }
  if(totalSourceWeight==0)
    totalSourceWeight=1; // avoid divide by zero

  // make a root-formatted name
  reformatStringForRoot(distributionName);
  addUnits(distributionName);

  string distributionCode = "distrbn"+Math::ftoa(distributionNumber);
 
  Math::makeDifferent(binEdges);
  string binEdgesArrayName = "binEdgesArray_"+distributionCode;
  plotCommand += fillRootArray(binEdgesArrayName,binEdges);
 
  string dataArrayName = "dataArray_"+distributionCode;
  string dataHistName = "dataHist_"+distributionCode;
  string bkgHistNameBase = "bkgHist_"+distributionCode;
  string sigHistNameBase = "sigHist_"+distributionCode;

  if( showBkgSigData[2]) plotCommand += fillRootArray(dataArrayName, fd);

  // create histogram with from bin edges and fill contents from data
  plotCommand += createRootHistogram(dataHistName,binEdgesArrayName);
  if( showBkgSigData[2]) plotCommand += fillRootHistogram(dataHistName,dataArrayName);
  plotCommand += dataHistName+"->Draw(\"e1p\");\n";
  plotCommand += dataHistName+"->SetMaximum("+Math::ftoa(histoYMAX)+");\n";
  plotCommand += dataHistName+"->SetMinimum(0);\n";



  // at this point, if histogram has large number of entries
  // then have to move the Y title offset a little more
  plotCommand += "if("+dataHistName+"->GetMaximum()>9999)\n";
  plotCommand += dataHistName+"->SetTitleOffset(1.5,\"Y\");\n";
  plotCommand += dataHistName+"->SetMarkerStyle(20);\n";
  plotCommand += dataHistName+"->SetMarkerSize(1.0);\n";

  plotCommand += labelHistogramAxis(dataHistName,distributionName,"X");
  plotCommand += labelHistogramAxis(dataHistName,"Number of Events","Y");
  // no longer do automatic title - now create a TLatex object ourselves
  // we label histogram by final state
  plotCommand += createHistogramTitle(finalState);

  string legendName = "legend"+Math::ftoa(distributionNumber);
  plotCommand += createLegend(legendName);
  int nLegendEntry = 0;

  if(getenv("vistaLegendDataEntry")!=NULL)
    plotCommand += addLegendEntry(legendName,dataHistName,getenv("vistaLegendDataEntry"),"P", nLegendEntry++);
  else if((colliderRun == "tev2" )&&(experiment == "cdf"))
    plotCommand += addLegendEntry(legendName,dataHistName,"CDF Run II Data","P", nLegendEntry++);
  else if((colliderRun == "tev2" )&&(experiment == "d0"))
      plotCommand += addLegendEntry(legendName,dataHistName,"D0 Run II Data","P", nLegendEntry++);
  else if((colliderRun == "lep2" )&&(experiment == "aleph"))
    plotCommand += addLegendEntry(legendName,dataHistName,"ALEPH LEP 2 Data","P", nLegendEntry++);
  else if((colliderRun == "lep2" )&&(experiment == "l3"))
    plotCommand += addLegendEntry(legendName,dataHistName,"L3 LEP 2 Data","P", nLegendEntry++);
  else if((colliderRun == "lep2" )&&((experiment == "l3+aleph" || experiment == "aleph+l3")))
    plotCommand += addLegendEntry(legendName,dataHistName,"L3 plus ALEPH LEP 2 Data","P", nLegendEntry++);
  else if((colliderRun == "hera")&&(experiment == "h1"))
    plotCommand += addLegendEntry(legendName,dataHistName,"H1 Data","P", nLegendEntry++);
  else if(getenv("forceLegendDataEntry")!=NULL)
    plotCommand += addLegendEntry(legendName,dataHistName,getenv("forceLegendDataEntry"),"P", nLegendEntry++);
  else
    plotCommand += addLegendEntry(legendName,dataHistName,"Data","P", nLegendEntry++);

  plotCommand += addPreliminaryLabel(colliderRun, experiment);

  if(showBkgSigData[0]) // if showBkg
    {
      // fb.size() is the number of bkg components - make a histo for each
      for(int j=fb.size()-1; j>=0; j--)
	{
	  string bkgHistName = bkgHistNameBase+"_"+Math::ftoa(j);
	  string bkgArrayName = "bkgArray_"+distributionCode+"_"+Math::ftoa(j);
	  plotCommand += fillRootArray(bkgArrayName,fb[j]);
	  plotCommand += createRootHistogram(bkgHistName,binEdgesArrayName);
	  plotCommand += fillRootHistogram(bkgHistName,bkgArrayName);

	  int bkgColor=500;
	  if(j>=fNBkgContributions) bkgColor = 500+fNBkgContributions-1;
	  else bkgColor = 500+j;
	  //mrenna  to prevent histo from blotting out pts
	  //	  plotCommand += bkgHistName+"->SetFillStyle(0);\n";
	  plotCommand += bkgHistName+"->SetFillColor("+Math::ftoa(bkgColor)+");\n";

	  //set line color - red or else same as the fill
	  int lineColor=500;
	  lineColor = bkgColor;
	  if(lineColor>=500 && lineColor<=504) lineColor = 504;
	  else if(lineColor>=505 && lineColor<=508) lineColor = 508;
	  else if(lineColor>=509 && lineColor<=512) lineColor = 512;
	  plotCommand += bkgHistName+"->SetLineColor("+Math::ftoa(lineColor)+");\n";
	  

	  // add appropriate legend entry

	  //Georgios: If the source type is "mock" it should not be included in the legend.
	  // That could happen if we have for example bkg=0 and data>0, in which case we create 
	  // "mock" bkg mimicking the data, just for the purpose of being able to plot the data.

	  if ( sources[j] == "mock" ) continue;

	  if(bkgColor<500+fNBkgContributions-1) {
	    string legendEntry = sources[j]+" : "+Math::ftoa(Math::toleranceRound(100.0*sourceWeights[j]/totalSourceWeight,0.1))+"%";
	    plotCommand += addLegendEntry(legendName,bkgHistName,legendEntry,"F",nLegendEntry++);
	  }
	  else { // then add the 'Others' entry for the red (in case of 5 colors) histogram
	    plotCommand += addLegendEntry(legendName,bkgHistName,"Other","F",nLegendEntry++);
	  }
	  
	}
    }
  if(showBkgSigData[1]) // if showSig (used in Quaero, not Vista)
    // ignore this section for now
    {
      // fb.size() is the number of bkg components - make a histo for each
      for(int j=fs.size()-1; j>=0; j--)
	{
	  string sigHistName = sigHistNameBase+"_"+Math::ftoa(j);
	  string sigArrayName = "sigArray_"+distributionCode+"_"+Math::ftoa(j);
	  plotCommand += fillRootArray(sigArrayName,fs[j]);
	  plotCommand += createRootHistogram(sigHistName,binEdgesArrayName);
	  plotCommand += fillRootHistogram(sigHistName,sigArrayName);

	  int bkgColor=500;
	  if(j>=fNBkgContributions) bkgColor = 500+fNBkgContributions-1;
	  else bkgColor = 500+j;
	  //mrenna  to prevent histo from blotting out pts
	  //plotCommand += sigHistName+"->SetFillStyle(0);\n";
          //mrenna no fill
	  //plotCommand += sigHistName+"->SetFillColor("+Math::ftoa(bkgColor)+");\n";

	  //set line color - red or else same as the fill
	  int lineColor=500;
	  lineColor = bkgColor;
	  if(lineColor>=500 && lineColor<=504) lineColor = 504;
	  else if(lineColor>=505 && lineColor<=508) lineColor = 508;
	  else if(lineColor>=509 && lineColor<=512) lineColor = 512;
          //mrenna
          lineColor = 1;
	  plotCommand += sigHistName+"->SetLineColor("+Math::ftoa(lineColor)+");\n";
	  

	  // add appropriate legend entry

	  //Georgios: If the source type is "mock" it should not be included in the legend.
	  // That could happen if we have for example bkg=0 and data>0, in which case we create 
	  // "mock" bkg mimicking the data, just for the purpose of being able to plot the data.
	  if ( sources[j] == "mock" ) continue;

	  /*	  if(bkgColor<500+fNBkgContributions-1) {
	    string legendEntry = sources[j]+" : "+Math::ftoa(Math::toleranceRound(100.0*sourceWeights[j]/totalSourceWeight,0.1))+"%";
	    plotCommand += addLegendEntry(legendName,sigHistName,legendEntry,"F",nLegendEntry++);
	  }
	  else { // then add the 'Others' entry for the red (in case of 5 colors) histogram
	    plotCommand += addLegendEntry(legendName,sigHistName,"Other","F",nLegendEntry++);
	    }*/
	}
      //      cout << "ShowSignal not yet implemented in ROOT..." <<endl;
      // plotCommand += "// ShowSignal not yet implemented in ROOT... \n";
    }
  if(showBkgSigData[0]) // if showBkg
    {
      // plot the bkg hists
      for(int j=fb.size()-1; j>=0; j--)	{
	string bkgHistName = bkgHistNameBase+"_"+Math::ftoa(j);
	plotCommand += bkgHistName+"->Draw(\"same\");\n";
      }
    }

  if(showBkgSigData[1]) // if showSig
    {
      // plot the bkg hists
      for(int j=fs.size()-1; j>=0; j--)	{
	string sigHistName = sigHistNameBase+"_"+Math::ftoa(j);
        // mrenna line and points
	plotCommand += sigHistName+"->Draw(\"same\");\n";
	plotCommand += sigHistName+"->SetMarkerStyle(20);\n";
	plotCommand += sigHistName+"->SetMarkerSize(1.0);\n";
	plotCommand += sigHistName+"->Draw(\"e1psame\");\n";
      }
    }


  if(showBkgSigData[2]) // if showData
    {
      //      plotCommand += dataHistName+"->SetMarkerSize(0.1);\n";//Georgios temporary for finer binning
      plotCommand += dataHistName+"->Draw(\"e1psame\");\n";
    }

  if( distributionNumber < windowLimitsLeft.size() ) {


  if ( distributionName.find("bump in",0) == string::npos && windowLimitsLeft[distributionNumber] < windowLimitsRight[distributionNumber]) // this is for the page where we show the mass variable, not the zoom in.
    plotCommand += markRegionOnAxis(distributionCode, windowLimitsLeft[distributionNumber], windowLimitsRight[distributionNumber]);
  if (distributionName.find("bump in",0) != string::npos && windowLimitsLeft[distributionNumber-1] < windowLimitsRight[distributionNumber-1]) // this is for the page where we zoom in at the bump. The window limits are found in the previous variable, which was the whole mass variable, not just the bump.
    plotCommand += markRegionOnAxis(distributionCode, windowLimitsLeft[distributionNumber-1], windowLimitsRight[distributionNumber-1]);


  }

  plotCommand += legendName+"->Draw();\n";
  plotCommand += "gPad->RedrawAxis();\n"; // to redraw ticks
  plotCommand += "// End distribution "+distributionName1+"\n";

  // increment distrbn counter
  // Georgios: if it was a bump variable, don't increase it. The bump was coming like a package, together with the mass distribution.
  if ( distributionName.find("bump in",0) == string::npos ) //if you didn't find "bump in" in the name then
    distributionNumber++; 


    
   
   pawCommand += plotCommand;
}

// void GraphicalOutputSimple_root::add1dPlot(
// 		string distributionName,
// 		vector<double> binEdges,
// 		vector<vector<double> > fs, vector<vector<double> > fb, vector<int> fd,
// 		vector<bool> showBkgSigData, vector<string> sources, vector<double> sourceWeights,
// 		string colliderRun, string experiment,
// 		string finalState
// 		)
// {
//   add1dPlot( distributionName, binEdges, fs, fb, fd, showBkgSigData, sources, sourceWeights, colliderRun, experiment, finalState, pair<double,double>(-1,-1) );
// }

void GraphicalOutputSimple_root::add1dPlots(vector<string> distributionNames, 
		  vector<bool> showBkgSigData,
		  string colliderRun,
		  string experiment, 
		  string finalstate,
		  double logLikelihood
		  )
{
  cout << "This is not yet implemented..." <<endl;
  return;
}


void GraphicalOutputSimple_root::add1dPlots(vector<string> distributionNames, 
		const vector<double>& weightsBkgSM,
		const vector<double>& weightsSigBkg,
		const vector<vector<double> >& variableValuesBkgSM, 
		const vector<vector<double> >& variableValuesSigBkg, 
		const vector<vector<double> >& variableValuesData,
		vector<bool> showBkgSigData,
		string colliderRun, 
		string experiment, 
		string finalstate, 
		double logLikelihood,
		bool justShowOne)
{
  vector<string> sources;
  vector<double> sourceWeights;
  add1dPlots(distributionNames, 
	     vector<vector<double> >(1,weightsBkgSM),
	     vector<vector<double> >(1,weightsSigBkg),
	     vector<vector<vector<double> > >(1,variableValuesBkgSM), 
	     vector<vector<vector<double> > >(1,variableValuesSigBkg), 
	     variableValuesData, sources, sourceWeights,
	     showBkgSigData, colliderRun, experiment, finalstate, logLikelihood, justShowOne);
}



void GraphicalOutputSimple_root::add1dPlots(
		vector<string> distributionNames,
		const vector<vector<double> >& bkgWeights,  // [iSubComponent][iEvent]
		const vector<vector<double> >& sigWeights,  // [iSubComponent][iEvent]
		const vector<vector<vector<double> > >& bkgEvents, // [iSubComponent][iEvent][iVariable]
		const vector<vector<vector<double> > >& sigEvents, // [iSubComponent][iEvent][iVariable]
		const vector<vector<double> >& dataEvents, // [iEvent][iVariable]
		vector<string> sources, vector<double> sourceWeights,
		vector<bool> showBkgSigData,
		string colliderRun, 
		string experiment, 
		string finalState, 
		double logLikelihood,
		bool justShowOne,
		string specificDistribution,
		vector<double> windowLimitsLeft,
		vector<double> windowLimitsRight
		)
{
  logLikelihood = max(-100.,logLikelihood);
  double s=0, b=0;
  for(size_t i=0; i<sigWeights.size(); i++)
    s += accumulate(sigWeights[i].begin(),sigWeights[i].end(),0.);
  for(size_t i=0; i<bkgWeights.size(); i++)    
    b += accumulate(bkgWeights[i].begin(),bkgWeights[i].end(),0.);
  //  int N = dataEvents.size();

  vector<double> combinedBkgWeights, combinedSigWeights;
  vector<vector<double> > combinedBkgEvents, combinedSigEvents;
  for(size_t i=0; i<sigWeights.size(); i++)
    combinedSigWeights.insert(combinedSigWeights.end(),sigWeights[i].begin(),sigWeights[i].end());
  for(size_t i=0; i<bkgWeights.size(); i++)
    combinedBkgWeights.insert(combinedBkgWeights.end(),bkgWeights[i].begin(),bkgWeights[i].end());
  for(size_t i=0; i<sigEvents.size(); i++)
    combinedSigEvents.insert(combinedSigEvents.end(),sigEvents[i].begin(),sigEvents[i].end());
  for(size_t i=0; i<bkgEvents.size(); i++)
    combinedBkgEvents.insert(combinedBkgEvents.end(),bkgEvents[i].begin(),bkgEvents[i].end());


  //assert(comparableQ(sigEvents,dataEvents));
  //assert(comparableQ(sigEvents,bkgEvents));

  double sigMultiplicativeFactor = 1.;
  //sigMultiplicativeFactor = (int)pow(10.,(int)log10(b/s));

  size_t d = distributionNames.size();
  for(size_t j=0; j<dataEvents.size(); j++)
    assert(dataEvents[j].size()==d);
  for(size_t i=0; i<sigEvents.size(); i++)
    for(size_t j=0; j<sigEvents[i].size(); j++)
    assert(sigEvents[i][j].size()==d);
  for(size_t i=0; i<bkgEvents.size(); i++)
    for(size_t j=0; j<bkgEvents[i].size(); j++)
      assert(bkgEvents[i][j].size()==d);
  if(justShowOne)
    d = min(1,(int) d);
	    
  pawCommand += "// begin finalState "+finalState+"\n";


  if(d==0)
    {
      cout << "Should not have 'd==0' in Vista..." <<endl;
      pawCommand += "//Should not have 'd==0' in Vista.. \n";
    }  
  else
    {
      // loop over variables
      vector<int> nbins;
      Math::makeNiceHistogramRange(combinedBkgWeights, combinedSigWeights, 
				   combinedBkgEvents, combinedSigEvents, 
				   dataEvents, range, nbins);
      
      if(nbins!=vector<int>(0))
	{
	  for(size_t k=0; k<d; k++)
	    {
	      if (specificDistribution!="all") { //Georgios: Only plot a specific distribution if given
		string distributionName = distributionNames[k];
		distributionName = Math::replaceSubString(distributionName, "^+!", "+");
		distributionName = Math::replaceSubString(distributionName, "^-!", "-");
		distributionName = Math::replaceSubString(distributionName, "#-#", "_");
		distributionName = Math::replaceSubString(distributionName, "[m]", "mu");
		distributionName = Math::replaceSubString(distributionName, "[t]", "tau");
		distributionName = Math::replaceSubString(distributionName, "[g]", "ph");
		distributionName = Math::replaceSubString(distributionName, "p&/", "pmiss");
		cout << "Distribution : " << distributionName << " ";
		if (distributionName!=specificDistribution) {
		  cout << " SKIP" << endl;
		  distributionNumber++; 
		  continue;
		}
		cout << endl;
	      }
	      vector<double> xrange = range[k];
	      int nx = nbins[k];
	      
	      vector<double> v(1);
	      double dx = (xrange[1]-xrange[0])/nx;
	      string vv1="", vfs="", vfb="", vfd="";
	      vector<vector<double> > fs(sigEvents.size(),vector<double>(nx));
	      vector<vector<double> > fb(bkgEvents.size(),vector<double>(nx));
	      vector<int> fd(nx);
	      for(size_t i=0; i<sigEvents.size(); i++)
		{
		  for(size_t j=0; j<sigEvents[i].size(); j++)
		    {
		      int bin = (int)((sigEvents[i][j][k]-xrange[0])/dx);
		      if((bin>=0)&&(bin<nx))
			fs[i][bin] += sigMultiplicativeFactor*sigWeights[i][j];
		    }
		}
	      for(size_t i=0; i<bkgEvents.size(); i++)
		{
		  for(size_t j=0; j<bkgEvents[i].size(); j++)
		    {
		      int bin = (int)((bkgEvents[i][j][k]-xrange[0])/dx);
		      if((bin>=0)&&(bin<nx))
			fb[i][bin] += bkgWeights[i][j];
		    }
		}
	      for(size_t i=0; i<dataEvents.size(); i++)
		{
		  int bin = (int)((dataEvents[i][k]-xrange[0])/dx);
		  if((bin>=0)&&(bin<nx))
		    fd[bin]++;
		}
	      //TCanvas *c1 = new TCanvas("c1","c1",800,600);
	      string canvasName = "canv"+Math::ftoa(paneNumber);
	      pawCommand += "TCanvas *"+canvasName+" = new TCanvas(\"";
	      pawCommand += canvasName+"\",\""+canvasName+"\",800,600); \n";
	      paneNumber++;
	      add1dPlot(distributionNames[k], 
			deriveEvenlySpacedBinEdges(xrange[0], xrange[1], nx), 
			fs, fb, fd, showBkgSigData,
			sources, sourceWeights,colliderRun,experiment,finalState,windowLimitsLeft,windowLimitsRight);
	      pawCommand += canvasName+"->Print(\"PLOTSFILE\");\n";

	      //Georgios: make additional page with bump plot, if needed.
	      if (k < windowLimitsLeft.size() && (windowLimitsLeft[k] < windowLimitsRight[k])) {

		std::cout << windowLimitsLeft[k] << " " << windowLimitsRight[k] << " " << k << " " <<
		  windowLimitsLeft.size() << std::endl;
		double sizeOfBump=(windowLimitsRight[k] - windowLimitsLeft[k]);
		double sizeOfPaddingOnEachSide=1;
		if ( getenv("graphicalSizeOfPaddingOnEachSide")!=NULL)
		  sizeOfPaddingOnEachSide = atof(getenv("graphicalSizeOfPaddingOnEachSide"));
		xrange[0] = windowLimitsLeft[k] - sizeOfPaddingOnEachSide*sizeOfBump;
		xrange[1] = windowLimitsRight[k] + sizeOfPaddingOnEachSide*sizeOfBump;
		double nx = 60.0;
		double dx = (xrange[1]-xrange[0])/nx;
		vector<vector<double> > fs(sigEvents.size(),vector<double>((int)nx));
		vector<vector<double> > fb(bkgEvents.size(),vector<double>((int)nx));
		vector<int> fd((int)nx);
		for(size_t i=0; i<sigEvents.size(); i++)  {
		  for(size_t j=0; j<sigEvents[i].size(); j++)  {
		    if(sigEvents[i][j][k]<xrange[0]) continue;
		    int bin = (int)((sigEvents[i][j][k]-xrange[0])/dx);
		    if((bin>=0)&&(bin<nx))
		      fs[i][bin] += sigMultiplicativeFactor*sigWeights[i][j];
		  }
		}
		for(size_t i=0; i<bkgEvents.size(); i++)  {
		  for(size_t j=0; j<bkgEvents[i].size(); j++)  {
		    if(bkgEvents[i][j][k]<xrange[0]) continue;
		    int bin = (int)((bkgEvents[i][j][k]-xrange[0])/dx);
		    if((bin>=0)&&(bin<nx))
		      fb[i][bin] += bkgWeights[i][j];
		  }
		}
		for(size_t i=0; i<dataEvents.size(); i++)   {
		  if(dataEvents[i][k]<xrange[0]) continue;
		  int bin = (int)((dataEvents[i][k]-xrange[0])/dx);
		  if((bin>=0)&&(bin<nx))
		    fd[bin]++;
		}
		string canvasName = "canv"+Math::ftoa(paneNumber);
		pawCommand += "TCanvas *"+canvasName+" = new TCanvas(\"";
		pawCommand += canvasName+"\",\""+canvasName+"\",800,600); \n";
		paneNumber++;
		add1dPlot("bump in "+distributionNames[k], 
			  deriveEvenlySpacedBinEdges(xrange[0], xrange[1], (int)nx), 
			  fs, fb, fd, showBkgSigData,
			  sources, sourceWeights,colliderRun,experiment,finalState,windowLimitsLeft,windowLimitsRight);
		pawCommand += canvasName+"->Print(\"PLOTSFILE\");\n";
	      } // end of plotting just bump
	      
	    }
	}      
    }
  pawCommand += "// end finalState "+finalState+"\n";
}


/**  2d  **/

void GraphicalOutputSimple_root::add2dPlot(string distributionName1, string distributionName2, 
				vector<double> range1, vector<double> range2, 
				int nbins1, int nbins2, 
				vector<vector<double> > fs, vector<vector<double> > fb, vector<vector<int> > fd, 
				vector<bool> showBkgSigData)
{
  cout << "add2dPlot() not yet implemented in ROOT..." <<endl;
  pawCommand += "// add2dPlot() not yet implemented in ROOT... \n";

}


/*****  Draw  *****/

void GraphicalOutputSimple_root::draw(string plotFileName, string mainCommandFileName, string localDir)
{
  // this is where the mainCommand, that has been constructed from all the other member functions,
  // gets wrapped into a (temporary?) ROOT macro and then executed
  // arguments can give the names for the files involved: default args are defined for last two

  // root macro header and footers
  // define the style here
  string rootMacroHeader = "";
  string rootMacroFooter = "";
  rootMacroHeader += "{ \n";
  rootMacroHeader += "gROOT->Reset(); \n";
  // set the style attributes
  rootMacroHeader += setGlobalStyle();
  rootMacroHeader += "TCanvas* canv = new TCanvas(\"canv\",\"canv\",800,600);\n";
  rootMacroHeader += "canv->Print(\""+plotFileName+"[\");\n";

  pawCommand = Math::replaceSubString(pawCommand,"PLOTSFILE",plotFileName);

  rootMacroFooter += "canv->Print(\""+plotFileName+"]\");\n";
  rootMacroFooter += "\n } \n";

  if(mainCommandFileName=="")
    mainCommandFileName = Math::getTmpFilename();

  ofstream fRootFile(mainCommandFileName.data());
  fRootFile << rootMacroHeader <<endl;
  fRootFile << pawCommand <<endl;
  fRootFile << rootMacroFooter <<endl;
  fRootFile.close();

  string executionCommand = "root -q -b "+mainCommandFileName;
  //  string executionCommand = "source ~cdfsoft/cdf2.shrc; setup cdfsoft2 6.1.2; root -q -b "+mainCommandFileName;
#ifdef QUAERO_CC_PLATFORM_clued0
  executionCommand = "source ~/.login; setup D0RunII p18.08.00; root -q -b "+mainCommandFileName;
#endif
#ifdef QUAERO_CC_PLATFORM_cmsuaf
  executionCommand = "root -q -b "+mainCommandFileName;
#endif
  // direct stdout and stderr?
  executionCommand += ">& /dev/null";
  system(executionCommand.data());
  
  return;
}


/*****  Addition operator  *****/

GraphicalOutputSimple_root GraphicalOutputSimple_root::operator+(const GraphicalOutputSimple_root& rhs) const
{
  return(GraphicalOutputSimple_root(this->pawCommand+rhs.pawCommand));
}




