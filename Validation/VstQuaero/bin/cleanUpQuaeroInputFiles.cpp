// g++ -o cleanUpQuaeroInputFiles cleanUpQuaeroInputFiles.cpp 

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
  for(int i=1; i<argc; i++)
    {
      string filename = argv[i];
      cout << filename << endl;
      string tmp = "tmp.txt";
      ifstream fin(filename.c_str());
      ofstream fout(tmp.c_str());
      double wt, px, py, pz;
      string objectType="";
      while(fin >> wt)
	{
	  fout << wt << " ";
	  while(true)
	    {
	      fin >> objectType;
	      if(objectType==";")
		{
		  fout << " ; " << endl;
		  break;
		}
	      else
		{
		  fout << " " << objectType << " ";
		  fin >> px >> py >> pz;
		  fout << px << " " << py << " " << pz << " ";
		}
	    }
	}
      system(("mv "+tmp+" "+filename).c_str());
    }
  return(0);
}
