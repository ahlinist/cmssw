#include "JetMETCorrections/GammaJet/interface/GammaJetCorrector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "DataFormats/JetReco/interface/Jet.h"

#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
namespace {
class ParametrizationGammaJet{

 public:
  
  ParametrizationGammaJet(int ptype, vector<double> parameters):type(ptype),p(parameters){};
  
  double value(double arg) const;

 private:

  int type;
  std::vector<double> p;
};

double ParametrizationGammaJet::value(double e)const{

  double enew(e);

  switch(type){

  case 1:
    {
      double x=e;
      if(x<20) x=20;
      double kjet= p[0]+p[1]/(x+p[2]);
      if(p[3]) kjet+=p[3]*exp(-(x-p[4])*(x-p[4])/p[5]*log(2.));
      if(p[6]) kjet+=p[6]*exp(-(x-p[7])*(x-p[7])/p[8]*log(2.));
      if(p[9]) kjet+=p[9]*exp(-(x-p[10])*(x-p[10])/p[11]*log(2.));
      if(kjet<.2) kjet=.2;
      enew=e/kjet;
      break;
    }
  case 2:
    { 
      double x=e;
      if(x<p[0]) x=p[0];
      else if(x>p[8]) x=p[8];
      double kjet;
      if(x<p[4]) {
	kjet=p[1]+15*exp(-(x+p[2])/p[3]);
      }
      else {
	if(x+p[7]<.01) x=0.01-p[7]; //protection from bad parameters in the parameter file
	kjet=p[5] + p[6] * log(x+p[7]);
      }
      if(kjet<.1)kjet=.1; //protection from bad parameters in the parameter file 
      enew=e/kjet;
      break;
    }
  case 3:
    { 
      double x=e;
      if(x<p[0]) x=p[0];
      else if(x>p[6]) x=p[6];
      double kjet;
      if(x+p[3]<.01) x=0.01-p[3]; //protection from bad parameters in the parameter file
      kjet=p[1] + p[2] * log(x+p[3]) - p[4]/(x+p[5]);
      if(kjet<.1)kjet=.1; //protection from bad parameters in the parameter file 
      enew=e/kjet;
      break;
    }


  default:
    cerr<<"GammaJetCorrector: Error: unknown parametrization type '"<<type<<"' in GammaJetCorrector. No correction applied"<<endl;
    break;
  }
  return enew;
}

class   JetCalibrationParameterSetGammaJet{
public:
  JetCalibrationParameterSetGammaJet(string tag);
  int neta(){return etavector.size();}
  double eta(int ieta){return etavector[ieta];}
  int type(int ieta){return typevector[ieta];}
  const vector<double>& parameters(int ieta){return pars[ieta];}
  bool valid(){return etavector.size();}

private:

  vector<double> etavector;
  vector<int> typevector;
  vector< vector<double> > pars;
};
JetCalibrationParameterSetGammaJet::JetCalibrationParameterSetGammaJet(string tag){

  //string path(getenv("CMSSW_SEARCH_PATH"));
  //string file="JetCalibrationData/GammaJet/"+tag+".txt";
  std::string file="JetMETCorrections/GammaJet/data/"+tag+".txt";
  
  edm::FileInPath f1(file);
  
  std::ifstream in( (f1.fullPath()).c_str() );
  
  //  if ( f1.isLocal() ){
    cout << " Start to read file "<<file<<endl;
    string line;
    while( std::getline( in, line)){
      if(!line.size() || line[0]=='#') continue;
      istringstream linestream(line);
      double par;
      int type;
      linestream>>par>>type;
      
      cout<<" Parameter eta = "<<par<<" Type= "<<type<<endl;
      
      etavector.push_back(par);
      typevector.push_back(type);
      pars.push_back(vector<double>());
      while(linestream>>par)pars.back().push_back(par);
    }
    //  }
    //  else
    //    cout<<"The file \""<<file<<"\" was not found in path \""<<f1.fullPath()<<"\"."<<endl;
}
} // namespace

GammaJetCorrector::GammaJetCorrector (const edm::ParameterSet& fConfig) {
  setParameters (fConfig.getParameter <std::string> ("tagName"));
}


GammaJetCorrector::~GammaJetCorrector()
{
  for(ParametersMap::iterator ip=parametrization.begin();ip!=parametrization.end();ip++) delete ip->second;  
}

void GammaJetCorrector::setParameters(std::string fType)
{ 

  JetCalibrationParameterSetGammaJet pset (fType);
  if(pset.valid()){
    for (int ieta=0; ieta < pset.neta(); ieta++) {
      parametrization [pset.eta(ieta)]= new ParametrizationGammaJet (pset.type(ieta), pset.parameters(ieta));
    }
  }
  else {
    edm::LogError ("Jet Corrections not found") << "GammaJetCorrector: calibration = " << fType 
					     << " not found! Cannot apply any correction ..." << std::endl;
  }

}

double GammaJetCorrector::correction( const LorentzVector& fJet) const
{
  if(parametrization.empty()) { return 1.; }
  
    double et=fJet.Et();
    double eta=fabs(fJet.Eta());
    

    //if(eta<10) { eta=fabs(fJet.getY()); }
    
    cout<<" Et and eta of jet "<<et<<" "<<eta<<endl;

    double etnew;
    std::map<double,ParametrizationGammaJet*>::const_iterator ip=parametrization.upper_bound(eta);
    if(ip==parametrization.begin()) 
    { 
        etnew=ip->second->value(et); 
    }
      else if(ip==parametrization.end()) 
      {
          etnew=(--ip)->second->value(et);
      }
       else
          {
             double eta2=ip->first;
             double et2=ip->second->value(et);
             ip--;
             double eta1=ip->first;
             double et1=ip->second->value(et);
             etnew=et1+(eta-eta1)*(et2-et1)/(eta2-eta1);
          }
	 //theJet*=etnew/et;
	 
	 cout<<" The new energy found "<<etnew<<" "<<et<<endl;

     return etnew/et;
}
