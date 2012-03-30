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
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaero/interface/QuaeroIO.hh"
#include "Validation/VstQuaeroUtils/interface/generate.hh"
#include "Validation/VstQuaero/interface/QuaeroSysObject.hh"
#include "Validation/VstQuaero/interface/RCPParameters.hh"
using namespace std;


/*****  Midgame:  File manipulations  *****/



void QuaeroIO::generateModelMadEvent(string colliderRun, vector<double>& resonanceMasses, 
				     string MadEventFilename, string sigFilename)
{

  if(MadEventFilename=="")
    MadEventFilename = getFilename("MadEvent",colliderRun);
  if(sigFilename=="")
    sigFilename = getFilename("hepg",colliderRun);

  map<int,string> colliderSubRuns;
  if(colliderRun=="lep2")
    for(int i=-18; i<=-11; i++)
      colliderSubRuns[i] = "e-e+";
  if(colliderRun=="hera")
    {
      colliderSubRuns[-21] = "paae+";
      colliderSubRuns[-22] = "paae+";
      colliderSubRuns[-23] = "paae-";
    }
  if(colliderRun=="tev1")
    colliderSubRuns[31] = "paapab";     // proton lite vs. anti-proton lite
  if(colliderRun=="tev2")
    colliderSubRuns[41] = "paapab";     // proton lite vs. anti-proton lite
  if(colliderRun=="lhc")
    colliderSubRuns[51] = "pp";         // proton vs. proton
  vector<double> lumi = RCPParameters::desiredGeneratedLuminosity(colliderRun);
  if(colliderRun=="hera")
    assert(lumi.size()==3);
  if(colliderRun=="lep2")
    assert(lumi.size()==8);  
  if(colliderRun=="tev1")
    assert(lumi.size()==1);
  if(colliderRun=="tev2")
    assert(lumi.size()==1);
  if(colliderRun=="lhc")
    assert(lumi.size()==1);
  double totalDesiredLuminosityForThisColliderRun = accumulate(lumi.begin(),lumi.end(),0.); // units are pb^-1
  vector<double> xsec = vector<double>(lumi.size(),0);

  //  double weightedCrossSection = 0;
  //double totalLumi = 0;

  system(("mkdir "+getFilename("tmpMadEventDirectory")).c_str());
  system(("ln -sf "+getFilename("MadEventDirectory")+"/MadGraphII "+getFilename("tmpMadEventDirectory")).c_str());
  system(("cp -pr "+getFilename("MadEventDirectory")+"/Template "+getFilename("tmpMadEventDirectory")).c_str());
  
  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/interactions.dat "+
	  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.interactions.dat").c_str());
  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/particles.dat "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.particles.dat").c_str());

  system(("rm  "+sigFilename+".xsec "+GlobalVariables::silencerString).c_str());
  for(map<int,string>::iterator iColliderSubRun=colliderSubRuns.begin(); 
      iColliderSubRun!=colliderSubRuns.end(); iColliderSubRun++)
    {
      cout << endl;
      cout << "Collider sub run #" << abs(iColliderSubRun->first) << Math::spacePad(55) << flush;  system("date");
      int iColliderSubRunCount = abs(iColliderSubRun->first) % 10;
      int maxParticlesInFinalState;
      string tmpFileName = Math::getTmpFilename();
      system(("cat "+MadEventFilename+" | grep -e 'MAXFSLEGS' | sed -e 's/MAXFSLEGS//' > "+tmpFileName).c_str());
      ifstream ftmp(tmpFileName.c_str()); ftmp >> maxParticlesInFinalState;
      if(!ftmp)
	maxParticlesInFinalState=4;
      ftmp.close(); system(("rm "+tmpFileName).c_str());
      double signalCrossSectionForThisColliderSubRun = 0;
      double signalCrossSectionForThisColliderSubRun_forced = 0;
      int totalNumberOfEventsGeneratedForThisColliderSubRun = 0;
      map<pair<int,int>,int> numberOfEventsGeneratedForThisColliderSubRunAndNumberOfParticlesInTheFinalState;
      map<pair<int,int>,double> madEventCrossSectionForThisColliderSubRunAndNumberOfParticlesInTheFinalState;

      for(int iParticlesInFinalState=2; iParticlesInFinalState<=maxParticlesInFinalState; iParticlesInFinalState++)
	{
	  if(iParticlesInFinalState>2)
	    cout << endl;
	  cout << "  " << iParticlesInFinalState << " particles " << flush;

	  // Create necessary directories
	  // bktemp if((iColliderSubRun != colliderSubRuns.begin())||(iParticlesInFinalState>2))
	  // system(("rm -r "+getFilename("tmpMadEventDirectory")+" "+GlobalVariables::silencerString).c_str());
	  system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; rm -r SubProcesses/P*_* "+GlobalVariables::silencerString).c_str());
	  
	  // Get relevant information from MadEventFilename
	  string tmpFileName1 = Math::getTmpFilename();
	  system(("cat "+MadEventFilename+" | grep -e 'INTERACTION' | sed -e 's/INTERACTION//' | sed -e 's/QNP//' | sed -e 's/np_coupl.*)//' >> "+tmpFileName1).c_str());
	  map<string,int> xanyParticles;
	  ifstream ftmp1(tmpFileName1.c_str());
	  string s;
	  while(ftmp1 >> s)
	    {
	      for(size_t i=0; i<s.length(); i++)
		if(isupper(s[i]))
		  s[i] = tolower(s[i]);
	      if((s=="d") || (s=="d~"))
		xanyParticles["d"] = xanyParticles["d~"] = 1;
	      if((s=="u") || (s=="u~"))
		xanyParticles["u"] = xanyParticles["u~"] = 1;
	      if((s=="s") || (s=="s~"))
		xanyParticles["s"] = xanyParticles["s~"] = 1;
	      if((s=="c") || (s=="c~"))
		xanyParticles["c"] = xanyParticles["c~"] = 1;
	      if((s=="b") || (s=="b~"))
		xanyParticles["b"] = xanyParticles["b~"] = 1;
	      if((s=="t") || (s=="t~"))
		xanyParticles["t"] = xanyParticles["t~"] = 1;
	      if((s=="e+") || (s=="e-"))
		xanyParticles["e+"] = xanyParticles["e-"] = 1;
	      if((s=="mu+") || (s=="mu-"))
		xanyParticles["mu+"] = xanyParticles["mu-"] = 1;
	      if((s=="ta+") || (s=="ta-"))
		xanyParticles["ta+"] = xanyParticles["ta-"] = 1;
	      if((s=="ve") || (s=="ve~"))
		xanyParticles["ve"] = xanyParticles["ve~"] = 1;
	      if((s=="vm") || (s=="vm~"))
		xanyParticles["vm"] = xanyParticles["vm~"] = 1;
	      if((s=="vt") || (s=="vt~"))
		xanyParticles["vt"] = xanyParticles["vt~"] = 1;
	      if((s=="w+") || (s=="w-"))
		xanyParticles["w+"] = xanyParticles["w-"] = 1;
	      if((s=="g") || (s=="a") || (s=="z") || (s=="h"))
		xanyParticles[s] = 1;
	      if((s=="lls"))
		xanyParticles["lls"] = 1;
	      if((s=="llf") || (s=="llf~"))
		xanyParticles["llf"] = xanyParticles["llf~"] = 1;
	      if((s=="llv"))
		xanyParticles["llv"] = 1;		
	    }
	  ftmp1.close();
	  if(colliderRun=="hera")
	    xanyParticles["e+"] = xanyParticles["e-"] = 1;	    
	  ofstream fxany((getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/xany.dat").c_str());
	  for(map<string,int>::iterator i=xanyParticles.begin(); i!=xanyParticles.end(); i++)
	    fxany << i->first;
	  fxany << endl;
	  fxany.close();

	  //	  system(("echo 'xany = ' `cat "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/xany.dat`").c_str());
	  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.interactions.dat "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/interactions.dat").c_str());
	  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.particles.dat "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/particles.dat").c_str());

	  // system(("echo 'xany = ' `cat "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/xany.dat`").c_str());
	  system(("cat "+MadEventFilename+" | grep -e 'INTERACTION' | sed -e 's/INTERACTION//' | sed -e 's/np_coupl_cLR(/np_coupl_cLR(1,/' | sed -e 's/np_coupl_rLR(/np_coupl_rLR(1,/' | sed -e 's/np_coupl_cLREM(/np_coupl_cLREM(1,1,/' >> "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/interactions.dat").c_str());
	  system(("cat "+MadEventFilename+" | grep -e 'PARTICLE' | sed -e 's/PARTICLE//' >> "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/particles.dat").c_str());
	  system(("echo '0' > "+getFilename("tmpMadEventDirectory")+"/Template/diagramNumbersToKeep.txt").c_str());
	  string initialState = iColliderSubRun->second;
	  string finalState = "";
	  string madEventInputFileName = getFilename("tmpMadEventDirectory")+"/Template/Process/input.dat";
	  system(("cat "+MadEventFilename+" | grep -e 'FINALSTATE' | sed -e 's/FINALSTATE//' > "+madEventInputFileName).c_str());
	  ifstream fFinalState(madEventInputFileName.c_str());
	  string finalStateParticle="";
	  int iSpecifiedFinalStateParticles=0;
	  if(fFinalState)
	    while(fFinalState >> finalStateParticle)
	      {
		iSpecifiedFinalStateParticles++;
		finalState += finalStateParticle;
	      }
	  fFinalState.close();
	  if((iSpecifiedFinalStateParticles>0)&&
	     (iSpecifiedFinalStateParticles!=iParticlesInFinalState))
	    continue;
	  string particlesThatCannotBeInternalFeynmanDiagramLinesUnlessTheyShareAVertexWithOneOfTheIncomingParticles = " ";
	  if(finalState=="")   // if no final state is explicitly specified
	    {
	      for(int i=0; i<iParticlesInFinalState; i++)
		if((i==iParticlesInFinalState-1)&&(colliderRun=="tev1"))
		  finalState += "xanl "; // "xanl" stands for "any lepton"
		else
		  finalState += "xany ";
	      particlesThatCannotBeInternalFeynmanDiagramLinesUnlessTheyShareAVertexWithOneOfTheIncomingParticles = " ,w+w-zag ";
	    }
	  string particlesThatCannotBeInternalFeynmanDiagramLines = " /e+e-mu+mu-ta+ta-veve~vmvm~vtvt~ ";
	  // bktemp -- I am commenting this out just for testing the crypton 
	  // string particlesThatCannotBeInternalFeynmanDiagramLines = " /e+e-mu+mu-ta+ta-veve~vmvm~vtvt~uu~dd~ss~cc~bb~tt~ ";
	  if(iSpecifiedFinalStateParticles>0)
	    particlesThatCannotBeInternalFeynmanDiagramLines = Math::replaceSubString(particlesThatCannotBeInternalFeynmanDiagramLines,"tt~","");

	  string theProcess = initialState+" > "+finalState+" "+particlesThatCannotBeInternalFeynmanDiagramLines+particlesThatCannotBeInternalFeynmanDiagramLinesUnlessTheyShareAVertexWithOneOfTheIncomingParticles;
	  system(("echo '"+theProcess+"' > "+getFilename("tmpMadEventDirectory")+"/Template/Process/input.dat").c_str());
	  // cout << theProcess << endl; 
	  system(("echo '99\n99\n99\n99\n\n\n' >> "+getFilename("tmpMadEventDirectory")+"/Template/Process/input.dat").c_str());
	  
	  string silencerString = " >> "+getFilename(".logfile");  //(RCPParameters::debugLevel >1 ? "" : GlobalVariables::silencerString); 
	  // Run MadGraph
	  cout << "mg2 " << flush;
	  system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/; "+getFilename("tmpMadEventDirectory")+"/MadGraphII/mg2 < ../Process/input.dat "+silencerString).c_str());

	  ifstream fmatrix((getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/matrix.ps").c_str());
	  if(!fmatrix)
	    {
	      fmatrix.close();
	      continue;
	    }
	  fmatrix.close();
	  cout << "newprocess " << flush;
	  // system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses; cp run_cuts_inclusive.dat run_cuts.dat").c_str());
	  system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; ./bin/newprocess "+GlobalVariables::silencerString).c_str()); //> out_newprocess.log 
	  system(("gs -q -dNOPAUSE -dBATCH -sDEVICE=pswrite -sOutputFile="+
		  getFilename("MadEventDiagrams",colliderRun,iColliderSubRun->second,Math::ftoa(iParticlesInFinalState))+
		  " "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/P*/matrix.ps").c_str());
	  system(("gzip -f "+
		  getFilename("MadEventDiagrams",colliderRun,iColliderSubRun->second,Math::ftoa(iParticlesInFinalState))).c_str());

	  system(("cat "+MadEventFilename+" | grep -e 'PARAMETER' | sed -e 's/PARAMETER//' | grep -i 'npm(' | sed -e 's/[nN][pP][mM]([0-9]*) *=//' > "+getFilename("tmpMadEventDirectory")+"/Template/tmp0.in").c_str());
	  ifstream finResonanceMassesIn((getFilename("tmpMadEventDirectory")+"/Template/tmp0.in").c_str());
	  double m=0;
	  while(finResonanceMassesIn >> m)
	    if( (resonanceMasses.empty()) ||
		(find(resonanceMasses.begin(), resonanceMasses.end(), m)==resonanceMasses.end()) )
	      if(m>20)
		resonanceMasses.push_back(m);
	  finResonanceMassesIn.close();

	  string bardIn = "1 ; 0.001 ; "+Math::ftoa(abs(iColliderSubRun->first))+" ";
	  s="";
	  char* c = new char[500];
	  system(("cat "+MadEventFilename+" | grep -e 'PARAMETER' | sed -e 's/PARAMETER//' > "+getFilename("tmpMadEventDirectory")+"/Template/tmp1.in").c_str());
	  ifstream finBardIn((getFilename("tmpMadEventDirectory")+"/Template/tmp1.in").c_str());
	  while(finBardIn.getline(c,500))
	    {
	      s = c;
	      for(size_t l=0; l<s.length()-3; l++)
		{
		  if(s.substr(l,3)=="(L,")
		    s = s.substr(0,l) + "(1," + s.substr(l+3);
		  if(s.substr(l,3)=="(R,")
		    s = s.substr(0,l) + "(2," + s.substr(l+3);
		  if(s.substr(l,3)=="EDM")
		    s = s.substr(0,l) + " 1 " + s.substr(l+3);
		  if(s.substr(l,3)=="MDM")
		    s = s.substr(0,l) + " 2 " + s.substr(l+3);
		}

	      replace(s.begin(),find(s.begin(),s.end(),'='),'(',':');
	      replace(s.begin(),find(s.begin(),s.end(),'='),')',':');
	      replace(s.begin(),find(s.begin(),s.end(),'='),',',':');
	      replace(s.begin(),s.end(),'=',' ');
	      if((!s.empty())&&(find(s.begin(), s.end(), ':')!=s.end()))
		bardIn += " ; " + s ;
	    }

	  finBardIn.close();
	  ofstream foutBardIn((getFilename("tmpMadEventDirectory")+"/Template/bard.in").c_str());
	  // cout << "bard.in = " << bardIn << endl;
	  foutBardIn << bardIn << endl;
	  foutBardIn.close();
	  system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses; ln -sf ../bard.in .").c_str());
	  vector<string> subProcesses = Math::getFilesInDirectory(getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/","P*_*");
	  for(size_t i=0; i<subProcesses.size(); i++)
	    system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/"+subProcesses[i]+"/; ln -sf ../bard.in .").c_str());
	  ofstream fPmassInc((getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/pmass.inc").c_str());
	  fPmassInc << "        pmass(1)=0" << endl;
	  fPmassInc << "        pmass(2)=0" << endl;
	  fPmassInc << "        do i=3,nexternal" << endl;
	  fPmassInc << "          pmass(i)=-1" << endl;
	  fPmassInc << "        enddo" << endl;
	  fPmassInc.close();
	  system(("cp "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/pmass.inc "+
		  getFilename("tmpMadEventDirectory")+"/Template/Source/pmass.inc").c_str());
	  
	  system(("cat "+MadEventFilename+" | grep -e 'XSEC' | sed -e 's/XSEC//' > "+getFilename("tmpMadEventDirectory")+"/Template/tmp2.in").c_str());
	  ifstream ftmp2((getFilename("tmpMadEventDirectory")+"/Template/tmp2.in").c_str());
	  double signalCrossSection=-1;
	  if(ftmp2)
	    {
	      int iCR=0;
	      while(ftmp2 >> iCR)
		{
		  double x=0;
		  if(!(ftmp2 >> x))
		    break;
		  if(iCR==abs(iColliderSubRun->first))
		    {
		      signalCrossSection = x;
		      signalCrossSectionForThisColliderSubRun_forced = x;
		      // cout << "setting signalCrossSection=" << x << " " << flush;
		    }
		}
	    }
	  ftmp2.close();
	  system(("cat "+MadEventFilename+" | grep -e 'KFACT' | sed -e 's/KFACT//' > "+getFilename("tmpMadEventDirectory")+"/Template/tmp3.in").c_str());
	  ifstream ftmp3((getFilename("tmpMadEventDirectory")+"/Template/tmp3.in").c_str());
	  double kFactor = 1;
	  if(ftmp3)
	    {
	      int iCR=0;
	      while(ftmp3 >> iCR)
		{
		  double x=0;
		  if(!(ftmp3 >> x))
		    break;
		  if(iCR==abs(iColliderSubRun->first))
		    kFactor = x;
		}
	    }
	  ftmp3.close();

	  if(signalCrossSection!=0)
	    {
	      signalCrossSection=0;
	      cout << "survey " << flush;
	      system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; ./bin/survey 0 "+silencerString).c_str()); //
	      ifstream fxsec((getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/results.dat").c_str());
	      if(fxsec)
		fxsec >> signalCrossSection;
	      fxsec.close();
	    }

	  if(kFactor!=1)
	    signalCrossSection *= kFactor;

	  double myLumi = 0;
	  assert((size_t) iColliderSubRunCount<=lumi.size());
	  myLumi = lumi[iColliderSubRunCount-1];
	  cout << "(" << myLumi/1000 << "fb^-1) " << flush;
	  int numberOfEventsToGenerate = (int)(signalCrossSection*myLumi);
	  if(signalCrossSection*myLumi>RCPParameters::nSignalEvents)
	    numberOfEventsToGenerate = (int)(RCPParameters::nSignalEvents*(myLumi/totalDesiredLuminosityForThisColliderRun));

	  // Invest additional time to generate events if these events will be put into signalFileStorage
	  if((atoi(RCPParameters::signalFileStorage.id.c_str())>0)&&
	     (GlobalVariables::id==RCPParameters::signalFileStorage.id)&&
	     (RCPParameters::signalFileStorage.scaleFactor==1))
	    numberOfEventsToGenerate *= 10;

	  int numberOfEventsThatWereGenerated = 0;

	  if(numberOfEventsToGenerate>0)
	    {
	      cout << "(" << numberOfEventsToGenerate << " events) " << flush;
	      cout << "refine " << flush;
	      
	      // silencerString = ""; //bktemp

	      system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; ./bin/refine "+Math::ftoa(numberOfEventsToGenerate*3)+" 0 "+silencerString).c_str());  // *3 is a hack suggested by Stelzer, in order to ensure we end up with enough events after unweight_events
	      // do this twice
	      system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; ./bin/refine "+Math::ftoa(numberOfEventsToGenerate*3)+" 0 "+silencerString).c_str());  // *3 is a hack suggested by Stelzer, in order to ensure we end up with enough events after unweight_events

	      cout << "unweight " << flush;
	      system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/; ./bin/unweight_events "+Math::ftoa(numberOfEventsToGenerate)+" "+silencerString).c_str());
	      cout << "mlm " << flush;
	      system(("cd "+getFilename("tmpMadEventDirectory")+"/Template/Events/; cp ../cuts.dat .; echo '-1' "+Math::ftoa(abs(iColliderSubRun->first))+" > nupmax.txt; date >> "+getFilename(".logfile")+"; "+getFilename("mlm")+" 2> /dev/null >> "+getFilename(".logfile")).c_str());
	      if(RCPParameters::debugLevel>=4)
		system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/Events/unweighted_events.dat "+getFilename("MadEventLesHouchesEventFile",colliderRun,iColliderSubRun->second,Math::ftoa(iParticlesInFinalState))).c_str());
	      vector<string> outStdhepFiles = Math::getFilesInDirectory(getFilename("tmpMadEventDirectory")+"/Template/Events/","out.stdhep_*");
	      for(size_t i=0; i<outStdhepFiles.size(); i++)
		{
		  if(outStdhepFiles[i].substr(outStdhepFiles[i].length()-1)!="*")
		    {
		      string stdhepFileName = getFilename("tmpMadEventDirectory")+"/Template/Events/"+outStdhepFiles[i];
		      char* stdhepFileNameC = (char*)stdhepFileName.c_str();


//		      numberOfEventsThatWereGenerated += countstdhepeventsinthisfile_(stdhepFileNameC,stdhepFileName.length());
//		      numberOfEventsThatWereGenerated += countstdhepeventsinthisfile_(stdhepFileName);
// must handle something besides STDHEP files
		      numberOfEventsThatWereGenerated += -1;
		      system(("mv "+stdhepFileName+" "+sigFilename+"_"+Math::ftoa(iColliderSubRunCount)+"_"+Math::ftoa(iParticlesInFinalState)+"_"+Math::ftoa(i)+" "+GlobalVariables::silencerString).c_str());
		    }
		}
	      cout << "(" << numberOfEventsThatWereGenerated << " events) " << flush;
	      totalNumberOfEventsGeneratedForThisColliderSubRun += numberOfEventsThatWereGenerated;
	      
	      // exit(1); // bktemp
	      
	    }

	  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.interactions.dat "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/interactions.dat").c_str());
	  system(("cp "+getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/.particles.dat "+
		  getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/particles.dat").c_str());

	  ifstream fxsec((getFilename("tmpMadEventDirectory")+"/Template/SubProcesses/results.dat").c_str());
	  if(fxsec)
	    fxsec >> signalCrossSection;
	  fxsec.close();
	  if(kFactor!=1)
	    signalCrossSection *= kFactor;
	  
	  signalCrossSectionForThisColliderSubRun += signalCrossSection;
	  cout << signalCrossSection << "pb " << flush;

	  if(numberOfEventsThatWereGenerated>0)
	    {
	      numberOfEventsGeneratedForThisColliderSubRunAndNumberOfParticlesInTheFinalState[pair<int,int>(iColliderSubRunCount,iParticlesInFinalState)]=numberOfEventsThatWereGenerated;
	      madEventCrossSectionForThisColliderSubRunAndNumberOfParticlesInTheFinalState[pair<int,int>(iColliderSubRunCount,iParticlesInFinalState)]=signalCrossSection;
	    }
	}

      if(signalCrossSectionForThisColliderSubRun_forced!=0)
	lumi[iColliderSubRunCount-1] = totalNumberOfEventsGeneratedForThisColliderSubRun/signalCrossSectionForThisColliderSubRun_forced;
      else if(signalCrossSectionForThisColliderSubRun!=0)
	lumi[iColliderSubRunCount-1] = totalNumberOfEventsGeneratedForThisColliderSubRun/signalCrossSectionForThisColliderSubRun;
      xsec[iColliderSubRunCount-1] = signalCrossSectionForThisColliderSubRun;

      for(map<pair<int,int>,int>::iterator j1 = numberOfEventsGeneratedForThisColliderSubRunAndNumberOfParticlesInTheFinalState.begin(); 
	  j1 != numberOfEventsGeneratedForThisColliderSubRunAndNumberOfParticlesInTheFinalState.end(); j1++)
	{
	  if(signalCrossSectionForThisColliderSubRun>0)
	    {
	      double weightFactor = (madEventCrossSectionForThisColliderSubRunAndNumberOfParticlesInTheFinalState[j1->first]/signalCrossSectionForThisColliderSubRun)*(1.*j1->second/totalNumberOfEventsGeneratedForThisColliderSubRun);
	      system(((string)"echo "+Math::ftoa(j1->first.first)+" "+
		      Math::ftoa(j1->first.second)+" "+
		      Math::ftoa(weightFactor)+
		      " >> "+sigFilename+".xsec").c_str());
	    }
	}


      /* No need to run at lower LEP energies in e+e- collisions,
	 or at lower HERA energies in pe+ collisions, if no signal
	 events were produced in this attempt. */

      if((totalNumberOfEventsGeneratedForThisColliderSubRun<10)&&
	 ((colliderRun=="lep2")||
	  ((colliderRun=="hera")&&
	   (iColliderSubRun->second=="pe+"))))
	{
	  for(int ii = iColliderSubRunCount-2; ii>=0; ii--)
	    lumi[ii] = xsec[ii] = 0;
	  break;
	}
    }
  cout << endl;
  RCPParameters::signalXsec[colliderRun] = xsec;
  RCPParameters::actualGeneratedLuminosity[colliderRun] = lumi;
  if(colliderRun=="hera")
    {
      assert(xsec.size()==3);
      system(("echo '  MadEvent cross section at "+colliderRun+" = "
	      +Math::ftoa(Math::nice(xsec[0],+1))+" pb, "
	      +Math::ftoa(Math::nice(xsec[1],+1))+" pb, "+"and "
	      +Math::ftoa(Math::nice(xsec[2],+1))+" pb, \n"+
	      "    for e+p at 301 GeV, e+p at 319 GeV, and e-p at 319 GeV, respectively.' >> "+getFilename("answer")).c_str());
    }
  else
    {
      string xsecString = "";
      for(size_t i=0; i<xsec.size(); i++)
	xsecString += Math::ftoa(Math::nice(xsec[i],+1))+" pb" + (i<xsec.size()-1 ? ", " : " ");
      system(("echo '  MadEvent cross section at "+colliderRun+" = "
	      +xsecString+"' >> "+getFilename("answer")).c_str());     
    }
  
  system(("rm -r "+getFilename("tmpMadEventDirectory")+" "+GlobalVariables::silencerString).c_str());

  return;
}
