/***********************************************************
 Usage:    checkSomething <quaeroTextFile>
 Purpose:  run over a file to check something
***********************************************************/


#include <vector>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/RelevantVariables.hh"
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"
using namespace std;

int main(int argc, char* argv[])
{

  if(argc==1)
    {
      cout << "Usage:  checkSomething <files>" << endl;
      exit(1);
    }
  assert(argc==2);
  string fileName = argv[1];
  QuaeroItemFile<QuaeroEvent> ef(fileName);
  QuaeroEvent e;
  RelevantVariables relevantVariables;
  PartitionRule partitionRule("hera-Vista");
  int n1=0, n2=0;
  while(ef.nextEvent(e))
    {

      if(false)
	{
	  double sumPt = ((int)(e.getGeneratedSumPt()));
	  double m_all = (e.getGeneratedSumPt()-sumPt)*1000;
	  cout << sumPt << "  " << m_all << endl;
	}
      if(false)
	{
	  //if(e.getRunNumber()=="113404.43891")
	  {
	    vector<double> v = relevantVariables.getAllVariableValuesForThisEvent(partitionRule.getFinalState(e).getTextString(), e, false);
	    assert(v.size()==2);
	    cout << e.getRunNumber() << "   " << v[1] << "   " << v[0] << endl;  
	    
	  }
	}


      if(false)
      {
	HepLorentzVector j1 = e.getThisObject("j",1)->getFourVector();
	  HepLorentzVector j2 = e.getThisObject("j",2)->getFourVector();
	  HepLorentzVector j3 = e.getThisObject("j",3)->getFourVector();      
	  
	  double dR_j2j3 = Math::deltaR(j2.phi(),j2.pseudoRapidity(),j3.phi(),j3.pseudoRapidity());
	  double m_j2j3 = (j2+j3).m();
	  
	  Hep3Vector proton(0,0,1);
	  Hep3Vector nhat = proton.cross(j1.vect()).unit();
	  double j2_ptOut = fabs(j2.vect().dot(nhat));
	  double j3_ptOut = fabs(j3.vect().dot(nhat));
	  
	  HepLorentzVector j23 = j2+j3;
	  double pt123 = j1.perp()-j23.perp();
	  double deltaPhi123 = Math::deltaphi(j1.phi(),j23.phi());
	  double j23_ptOut = fabs(j23.vect().dot(nhat));

	  cout << j23_ptOut << endl;
	}
    }
  return(0);
}
