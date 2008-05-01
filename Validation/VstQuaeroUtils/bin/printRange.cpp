// g++ -o $RELEASE_DIR/printRange printRange.cc -lm

#include <string>
#include <iostream>
#include <cassert>
using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=3)
    {
      cout << "Usage:  printRange <lo> <hi>" << endl;
      exit(1);
    }
  assert(argc==3);
  int lo = atoi(argv[1]);
  int hi = atoi(argv[2]);
  for(int i=lo; i<=hi; i++)
    cout << i << " ";
  cout << endl;
  return(0);
}
