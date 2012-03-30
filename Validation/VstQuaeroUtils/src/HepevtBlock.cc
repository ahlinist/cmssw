#include "Validation/VstQuaeroUtils/interface/HepevtBlock.hh"
using namespace std;


istream& operator>>(istream& fin, HepevtParticle& p)
{
  fin >> p.isthep >> p.idhep;
  fin >> p.jmohep[0] >> p.jmohep[1];
  fin >> p.jdahep[0] >> p.jdahep[1];
  for(int i=0; i<5; i++)
    fin >> p.phep[i];
  for(int i=0; i<4; i++)
    fin >> p.vhep[i];
  return(fin);
}


ostream& operator<<(ostream& fout, const HepevtParticle& p)
{
  fout << p.isthep    << " " << p.idhep     << " ";
  fout << p.jmohep[0] << " " << p.jmohep[1] << " ";
  fout << p.jdahep[0] << " " << p.jdahep[1] << " ";
  for(int i=0; i<5; i++)
    fout << p.phep[i] << " ";
  for(int i=0; i<4; i++)
    fout << p.vhep[i] << " ";
  return(fout);
}


HepevtParticle::HepevtParticle():
  isthep(0), idhep(0), 
  jmohep(vector<int>(2)), jdahep(vector<int>(2)), 
  phep(vector<double>(5)), vhep(vector<double>(4))
{
}


CLHEP::HepLorentzVector HepevtParticle::getFourVector() const
{
  CLHEP::HepLorentzVector v = CLHEP::HepLorentzVector(phep[3],CLHEP::Hep3Vector(phep[0],phep[1],phep[2]));
  return(v);
}


istream& operator>>(istream& fin, HepevtEvent& e)
{
  fin >> e.nevhep;
  int nhep;
  fin >> nhep;
  HepevtParticle p;
  e.particles.clear();
  for(int i=0; i<nhep; i++)
    {
      fin >> p;
      e.particles.push_back(p);
    }
  return(fin);
}

ostream& operator<<(ostream& fout, const HepevtEvent& e)
{
  fout << e.nevhep << endl;
  fout << e.particles.size() << endl;
  for(size_t i=0; i<e.particles.size(); i++)
    {
      fout << e.particles[i] << endl;
    }
  return(fout);
}


HepevtEvent::HepevtEvent():
  nevhep(0), particles(vector<HepevtParticle>(0))
{

}

