/***********************************
Implementation of QuaeroIO namespace
Bruce Knuteson 2003
***********************************/


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/getEnvVariable.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaeroUtils/interface/generate.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
using namespace std;


/*****  Midgame:  File manipulations  *****/



void QuaeroIO::generateModelSuspect(string colliderRun, string suspectFilename, string sigFilename)
{
  cout << "Using Suspect" << endl;
  if(suspectFilename=="")
    suspectFilename = getFilename("suspect",colliderRun);
  system(("cp -p "+suspectFilename+" "+suspectFilename+".suspect0").c_str());
  system(("grep -v kfactor "+suspectFilename+" > "+suspectFilename+".suspect").c_str());
  string pythiaFilename = getFilename("pythia",colliderRun);
  ofstream fpythia(pythiaFilename.c_str());
  fpythia << "imss(1)=13" << endl;
  fpythia << "suspect=" << suspectFilename+".suspect" << endl;  
  fpythia << "msel=39" << endl;
  /*
  fpythia << "! set allowed W decay modes" << endl
	  << "! turn some decays off for speed" << endl
	  << "mdme(190,1)=0  ! W -> qq" << endl
	  << "mdme(191,1)=0  ! W -> qq" << endl
	  << "mdme(194,1)=0  ! W -> qq" << endl
	  << "mdme(195,1)=0  ! W -> qq" << endl
	  << "mdme(198,1)=0  ! W -> qq" << endl
	  << "mdme(199,1)=0  ! W -> qq" << endl
	  << "mdme(206,1)=1  ! W -> e nu" << endl
	  << "mdme(207,1)=1  ! W -> mu nu" << endl
	  << "mdme(208,1)=1  ! W -> tau nu" << endl
	  << "" << endl
	  << "! set allowed Z decay modes" << endl
	  << "! turn some decays off for speed" << endl
	  << "mdme(174,1)=0  ! Z -> d dbar" << endl
	  << "mdme(175,1)=0  ! Z -> u ubar" << endl
	  << "mdme(176,1)=0  ! Z -> s sbar" << endl
	  << "mdme(177,1)=0  ! Z -> c cbar" << endl
	  << "mdme(178,1)=0  ! Z -> b bbar" << endl
	  << "mdme(182,1)=1  ! Z -> e+ e-" << endl
	  << "mdme(183,1)=0  ! Z -> nu_e nu_ebar" << endl
	  << "mdme(184,1)=1  ! Z -> mu+ mu- " << endl
	  << "mdme(185,1)=0  ! Z -> nu_mu nu_mubar " << endl
	  << "mdme(186,1)=1  ! Z -> tau+ tau- " << endl
	  << "mdme(187,1)=0  ! Z -> nu_tau nu_taubar " << endl;    
  */
  //fpythia << "msub(3)=1 \n msub(24)=1 \n msub(26)=1 \n msub(102)=1 \n msub(103)=1 \n msub(123)=1 \n msub(124)=1 \n msub(153)=1 \n msub(158)=1 \n msub(171)=1 \n msub(173)=1 \n msub(174)=1 \n msub(176)=1 \n msub(178)=1 \n msub(179)=1 \n msub(143)=1 \n "; // generate Higgs
  //fpythia << "msub(201)=1 \n msub(202)=1 \n msub(203)=1 \n msub(204)=1 \n msub(205)=1 \n msub(206)=1 \n msub(207)=1 \n msub(208)=1 \n msub(209)=1 \n msub(210)=1 \n msub(211)=1 \n msub(212)=1 \n msub(213)=1 \n msub(214)=1 \n msub(216)=1 \n msub(217)=1 \n msub(218)=1 \n msub(219)=1 \n msub(220)=1 \n msub(221)=1 \n msub(222)=1 \n msub(223)=1 \n msub(224)=1 \n msub(225)=1 \n msub(226)=1 \n msub(227)=1 \n msub(228)=1 \n msub(229)=1 \n msub(230)=1 \n msub(231)=1 \n msub(232)=1 \n msub(233)=1 \n msub(234)=1 \n msub(235)=1 \n msub(236)=1 \n msub(237)=1 \n msub(238)=1 \n msub(239)=1 \n msub(240)=1 \n msub(241)=1 \n msub(242)=1 \n msub(243)=1 \n msub(244)=1 \n msub(246)=1 \n msub(247)=1 \n msub(248)=1 \n msub(249)=1 \n msub(250)=1 \n msub(251)=1 \n msub(252)=1 \n msub(253)=1 \n msub(254)=1 \n msub(256)=1 \n msub(258)=1 \n msub(259)=1 \n msub(261)=1 \n msub(262)=1 \n msub(263)=1 \n msub(264)=1 \n msub(265)=1 \n msub(271)=1 \n msub(272)=1 \n msub(273)=1 \n msub(274)=1 \n msub(275)=1 \n msub(276)=1 \n msub(277)=1 \n msub(278)=1 \n msub(279)=1 \n msub(280)=1 \n msub(281)=1 \n msub(282)=1 \n msub(283)=1 \n msub(284)=1 \n msub(285)=1 \n msub(286)=1 \n msub(287)=1 \n msub(288)=1 \n msub(289)=1 \n msub(290)=1 \n msub(291)=1 \n msub(292)=1 \n msub(293)=1 \n msub(294)=1 \n msub(295)=1 \n msub(296)=1 \n "; // generate Susy
  fpythia.close();
  system(("cat "+suspectFilename+".suspect0 | grep -i 'kfactor' | grep -v '*' >> "+pythiaFilename).c_str());
  vector<double> resonanceMasses = vector<double>(0);
  generateModelPythia(colliderRun,resonanceMasses,pythiaFilename,sigFilename);
  system(("mv "+suspectFilename+".suspect0 "+suspectFilename).c_str());
  system(("rm "+suspectFilename+".suspect").c_str());

  return;
}


void QuaeroIO::generateModelPythia(string colliderRun, vector<double>& resonanceMasses, string pythiaFilename, string sigFilename)
{
  cout << GlobalVariables::chatterTab.tab() << "Generating model" << endl;

  if(pythiaFilename=="")
    pythiaFilename = getFilename("pythia",colliderRun);
  if(sigFilename=="")
    sigFilename = getFilename("hepg",colliderRun);

  string tmpFileName = Math::getTmpFilename();
  string pythiaFileName_NoSpaces = Math::getTmpFilename();
  system(("cat "+pythiaFilename+" | sed -e 's/\t//g' | sed -e 's/ //g' > "+pythiaFileName_NoSpaces).c_str());
  system(("cat "+pythiaFilename+" | grep -i 'pmas([ 0-9]*, *1) *= *' | sed -e 's/\\!.*//' | sed -e 's/pmas([ 0-9]*, *1) *= *//' > "+tmpFileName).c_str());
  ifstream finResonanceMasses(tmpFileName.c_str());
  double m=0;
  while(finResonanceMasses >> m)
    {
      if( (resonanceMasses.empty()) ||
	  (find(resonanceMasses.begin(), resonanceMasses.end(), m) == resonanceMasses.end()) )
	if(m>20)
	  resonanceMasses.push_back(m);
    }
  finResonanceMasses.close();
  system(("rm "+tmpFileName).c_str());


  char * pythiaFilenameC = (char*)pythiaFileName_NoSpaces.c_str();
  char * sigFilenameC = (char*)sigFilename.c_str();
  char* colliderRunC = (char*)colliderRun.c_str();
  //double pythiaXsec=0;
  vector<double> lumi = RCPParameters::desiredGeneratedLuminosity(colliderRun);
  int nmax = RCPParameters::nSignalEvents;

  if(colliderRun=="hera")
    {
      assert(lumi.size()==3);
      double lumi1 = lumi[0], lumi2 = lumi[1], lumi3 = lumi[2];  // units are pb^-1
      double xsec1=0, xsec2=0, xsec3=0; // units are pb
      generatehera_(sigFilenameC, pythiaFilenameC, &lumi1, &lumi2, &lumi3, &xsec1, &xsec2, &xsec3, &nmax,
		    sigFilename.length(), pythiaFileName_NoSpaces.length());
      assert(RCPParameters::actualGeneratedLuminosity[colliderRun].size()==3);
      RCPParameters::actualGeneratedLuminosity[colliderRun][0]=lumi1;
      RCPParameters::actualGeneratedLuminosity[colliderRun][1]=lumi2;
      RCPParameters::actualGeneratedLuminosity[colliderRun][2]=lumi3;
      system(("echo '  Pythia cross section at "+colliderRun+" = "
	      +Math::ftoa(Math::nice(xsec1,+1))+" pb, "
	      +Math::ftoa(Math::nice(xsec2,+1))+" pb, "+"and "
	      +Math::ftoa(Math::nice(xsec3,+1))+" pb, \n"+
	      "    for e+p at 301 GeV, e+p at 319 GeV, and e-p at 319 GeV, respectively.' >> "+getFilename("answer")).c_str());
    }
  else
    {
      assert(lumi.size()>=1);
      for(size_t j=1; j<lumi.size(); ++j)
	assert(lumi[j]==lumi[0]);
      double lumi1 = lumi[0]*lumi.size();  // units are pb^-1
      double xsec1=0;
      generate_(sigFilenameC, pythiaFilenameC, &lumi1, &xsec1, colliderRunC, &nmax,
		    sigFilename.length(), pythiaFileName_NoSpaces.length(), colliderRun.length());
      RCPParameters::actualGeneratedLuminosity[colliderRun] = vector<double>(lumi.size(),lumi1/lumi.size());
      system(("echo '  Pythia cross section at "+colliderRun+" = "
	      +Math::ftoa(Math::nice(xsec1,+1))+" pb.' >> "+getFilename("answer")).c_str());
    }
  return;
}     


string QuaeroIO::getFilename(string type, string colliderRun, string experiment, string finalstate)
{
  string tmpSysDir = GlobalVariables::localDir+"tmp/"+GlobalVariables::id+"/"+Math::ftoa(GlobalVariables::iSystematicShift)+"/"+colliderRun+"/"+experiment+"/";
  string tmpDir = GlobalVariables::localDir+"tmp/"+GlobalVariables::id+"/";
  if((getEnvVariable("QUAERO_TMP_DIR")!="_unknown_")&&
     (RCPParameters::debugLevel<=1))
    tmpDir = ((string)getEnvVariable("QUAERO_TMP_DIR"))+"/"+GlobalVariables::id+"/";
  // assert(getenv("QUAERO_CC_DIR")!=NULL);
  //  assert(getenv("QUAERO_EDITION")!=NULL);
  string expDir = (string)getEnvVariable("QUAERO_CC_DIR")+"/"+getEnvVariable("QUAERO_EDITION")+"/experiments/"+colliderRun+"/"+experiment+"/";
  string sysDir = (string)getEnvVariable("QUAERO_CC_DIR")+"/"+getEnvVariable("QUAERO_EDITION")+"/experiments/sys/";
  //string sysDir = "/experiments/sys/";
  string requestDir = GlobalVariables::localDir+"requests/"+GlobalVariables::id+"/";


  // experiment-specific executables
  if(type=="simulate_directory")
    return(expDir+"simulate/");
  if(type=="refine_directory")
    return(expDir+"refine/");
  if(type=="simulate_executable")
    return(expDir+"simulate/simulate");
  if(type=="simulate0_executable")
    return(expDir+"simulate/simulate0");
  if(type=="refine_executable")
    return(expDir+"refine/refine");
  if(type=="hint_specs")
    {
      assert(experiment=="hint");
      return(expDir+"hint_specs.txt");
    }
  if(type=="signalFileStorage")
    return(GlobalVariables::localDir+"signalFileStorage/"+RCPParameters::signalFileStorage.id+"/");

  // systematic errors
  if(type=="sysSources")
    return(sysDir+"/systematicSources.txt");
  if(type=="sysCorrelations")
    return(sysDir+"/systematicCorrelations.txt");

  // signal files
  if(type=="signalfiles")
    return(requestDir+"/signalfiles/");
  if(type=="hepg")
    return(requestDir+"/signalfiles/hepg/"+colliderRun); 
  if(type=="metaSig")
    return(requestDir+"/signalfiles/"+colliderRun+"/"+experiment);  
  if(type=="shiftedSig")
    return(tmpSysDir+"sig/shiftedSig");
  if(type=="sigListing")
    return(tmpSysDir+"sig/sigListing.dat");
  if(type=="sig")
    if(finalstate=="")
      return(tmpSysDir+"sig/sig");
    else
      return(tmpSysDir+"sig/sig_"+finalstate+".txt");

  // background files
  if(type=="metaBkg")
    return(GlobalVariables::localDir+"/tmp/"+GlobalVariables::id+"/relevantBkg/metaBkg_"+colliderRun+"-"+experiment+".txt");
    //return(expDir+"metaBkg.txt");
  if(type=="bkgListing")
    return(expDir+"/bkg/bkgListing.dat");
  if(type=="relevantBkg")
    return(GlobalVariables::localDir+"/tmp/"+GlobalVariables::id+"/relevantBkg/relevantBkg_"+colliderRun+"-"+experiment+".txt");
  if(type=="shiftedBkg")
    return(tmpSysDir+"bkg/"+"shiftedBkg");
  if(type=="bkgInitial")
    {
      if(finalstate=="")
	return(expDir+"/bkg/bkg_*.txt");
      else
	return(expDir+"/bkg/bkg_"+finalstate+".txt");
    }
  if(type=="bkg")
    if(finalstate=="directory")
      return(tmpSysDir+"bkg/");
    else if(finalstate=="")
      return(tmpSysDir+"bkg/"+"bkg");
    else
      return(tmpSysDir+"bkg/"+"bkg_"+finalstate+".txt");

  // data files
  if(type=="metaData")
    return(expDir+"metaData.txt");
  if(type=="data")
    if(finalstate=="")
      return(expDir+"data/"+"data");
    else
      return(expDir+"data/data_"+finalstate+".txt");

  // temporary files
  if(type=="tmpDirectoryForSort")
    return(tmpSysDir);

  // begingame files
  if(type=="pythia")
    return(requestDir+"/generatorfiles/input");
  if(type=="suspect")
    {
      string ans = requestDir+"/generatorfiles/input";
      for(size_t i=0; i<ans.length()-1; i++)
	if(ans.substr(i,2)=="//")
	  ans = ans.substr(0,i)+ans.substr(i+1);
      return(ans);
    }
  if(type=="MadEvent")
    return(requestDir+"/generatorfiles/input");

  if((type=="MadEventDiagrams")||
     (type=="MadEventLesHouchesEventFile"))
    {
      if(experiment=="paapab")
	experiment = "ppbar";
      else if(experiment=="paae+")
	experiment = "pe+";
      else if(experiment=="paae-")
	experiment = "pe-";
    }
  if(type=="MadEventDiagrams")
    return(requestDir+"/generatorfiles/diagrams_"+colliderRun+"_"+experiment+"_"+finalstate+"particles.ps");
  if(type=="MadEventLesHouchesEventFile")
    return(requestDir+"/generatorfiles/events_"+colliderRun+"_"+experiment+"_"+finalstate+"particles.dat");
  if(type=="MadEventDirectory")
    return(GlobalVariables::localDir+"/MadEvent/");
  if(type=="mlm")
    return(GlobalVariables::localDir+"/mlm");
  if(type=="tmpMadEventDirectory")
    return(tmpDir+"/MadEvent/");
  if(type=="request")
    return(requestDir+"/request.txt");
  if(type==".request")
    return(requestDir+"/.request.txt");
  if(type=="index.html")
    return(requestDir+"/index.html");
  if(type=="answer")
    return(requestDir+"/answer.txt");
  if(type==".answer")
    return(requestDir+"/.answer.txt");

  // endgame files
  if(type=="ans")
    return(tmpDir+"ans.txt");
  if(type=="error")
    return(requestDir+"/error.txt");
  if(type=="paw")
    return(tmpDir+"paw");
  if(type=="plots")
    return(requestDir+"plots.ps");
  if(type==".plots")
    return(requestDir+".plots.ps");
  if(type=="logfile")
    return(requestDir+"logfile.txt");
  if(type==".logfile")
    return(requestDir+".logfile.txt");

  //  assert(getenv("QUAERO_HTML_URL")!=NULL);
  if(type=="plotsURL")
    return((string)getEnvVariable("QUAERO_HTML_URL")+getEnvVariable("QUAERO_EDITION")+"/requests/"+GlobalVariables::id+"/plots.ps.gz");
  if(type=="resultURL")
    return((string)getEnvVariable("QUAERO_HTML_URL")+getEnvVariable("QUAERO_EDITION")+"/requests/"+GlobalVariables::id+"/");
  if(type=="tmp")
    {
      string f = tmpDir+"tmp_"+Math::ftoa(((int)(drand48()*1e6)));
      while(ifstream(f.c_str()))
	f = tmpDir+"tmp_"+Math::ftoa(((int)(drand48()*1e6)));
      return(f);
    }
  if(type=="tmpDir")
    return(tmpDir);
    
  if(type=="rcpParameters")
    return(tmpDir+"rcpParameters.txt");
  if(type=="relevantVariables")
    return(tmpDir+"relevantVariables.txt");
  if(type=="systematicShiftsToCompute")
    return(GlobalVariables::localDir+"batchtmp/systematicShiftsToCompute.txt");
  if(type=="computeLikelihoodForSystematicShift")
    return(GlobalVariables::localDir+"batchtmp/computeLikelihoodForSystematicShift");
  if(type=="likelihoods")
    return(tmpDir+"likelihoods.txt");
  cout << type << endl;
  assert(false);
  return("");
}

