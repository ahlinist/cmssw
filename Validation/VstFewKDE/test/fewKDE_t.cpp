#include "Validation/VstFewKDE/interface/FewKDE.hh"

int main(int argc, char* argv[])
{
  FewKDE::FewKDE fewKDE;

  int N = 10;
  int d = 1;
  std::vector<std::vector<double> > data(N, std::vector<double>(d, 0));
  std::vector<double> wt(N, 1);
  for(int i=0; i<N; i++)
    for(int j=0; j<d; j++)
      data[i][j] = drand48();
  for(int i=0; i<N; i++)
    wt[i] = drand48();

  // Set up the region on which the kernel estimate will be defined
  fewKDE.enclose(data);

  // Derive the kernel estimate
  fewKDE.derive(data, wt);

  std::cout << "FewKDE estimate is: " << std::endl << std::endl;
  std::cout << fewKDE << std::endl;

  std::cout << std::endl << "done!" << std::endl;

  return 0;
}

