#include "Validation/VstQuaero/interface/Discriminant.hh"
#include <vector>
#include <string>
#include <numeric>
using namespace std;

Discriminant::Discriminant()
{
  h = b = FewKDE();
  hWt = bWt = 0;
}

Discriminant::Discriminant(const FewKDE& _h, const FewKDE& _b, double _hWt, double _bWt):
  h(_h), b(_b)
{
  hWt = _hWt;
  bWt = _bWt;
}


double Discriminant::compute(const vector<double>& x) const
{
  double ph = h.evaluate(x);
  double pb = b.evaluate(x);
  if((ph<1e-50)&&(pb<1e-50))
    ph = pb = 1;
  if((hWt==0)&&(bWt==0))
    return(0);
  double ans = hWt*ph / (hWt*ph + bWt*pb);
  return(ans);
}

Discriminant::Discriminant(const std::vector<std::vector<double> >& hPoints, 
				const std::vector<std::vector<double> >& bPoints, 
				const std::vector<double>& _hWt, 
				const std::vector<double>& _bWt)
{
  h.derive(hPoints,_hWt);
  b.derive(bPoints,_bWt);
  hWt = accumulate(_hWt.begin(), _hWt.end(), 0.);
  bWt = accumulate(_bWt.begin(), _bWt.end(), 0.);
}



std::istream &operator>>(std::istream& fin, Discriminant & discriminant)
{
  fin >> discriminant.hWt;
  fin >> discriminant.bWt;
  fin >> discriminant.h;
  fin >> discriminant.b;
  return(fin);
}

std::ostream &operator<<(std::ostream& fout, const Discriminant & discriminant)
{
  fout << discriminant.hWt << endl;
  fout << discriminant.bWt << endl;
  fout << discriminant.h << endl;
  fout << discriminant.b << endl;
  return(fout);
}


