
/* Routine for the conversion of starlight data format
 * into something readable by CMSSW_1_4_5
 *
 * Modification by X. Rouby on a routine kindly offered by J. Hollar
 * Sept. 28, 2007
*/ 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>                         
#include <cmath>

#include "TFile.h"
#include "TTree.h"

using namespace std;

//void makeEventsFile(int num=11)

void StarToHepmc( int num=0 )


{

  string filename = "HeavyIonsUltraperipheral/ExclUpsilonAndDileptonContinuum/InputMC/starlight_pbpb_up3s_xn_ee.out"; //input
  //"castor:/castor/cern.ch/user/k/kumarv/MuBkg/mumucont.reco50-01.root")
  string filename = "castor:/castor/cern.ch/user/k/kumarv/MuBkg/starlight_pbpb_mumucont_m6.out"; //input
  // string filename = "/afs/cern.ch/user/k/kumarv/scratch0/CMSSW_1_8_0/src/Input/starlight_pbpb_up1s_xn_mumu.out"; //input


  ifstream infile(filename.c_str());
  char outfilename[100];
  // sprintf(outfilename,"u3s_pbpb_ee-0%d.dat",num);
  sprintf(outfilename,"starlight_pbpb_mumucont.hepmc-0%d.dat",num);
  ofstream output(outfilename);
  if (! infile.is_open()) { cout << "\t ERROR: I can not open \"" << filename << "\"" << endl; return; }
  
  string temp_string, temp;
  istringstream curstring;

  const unsigned int N = 2; // N_particles
  const unsigned int M = 50000; // N_events
  const unsigned int K = num*M+1;  // first event 

  const double MU = 0.105658369; // muon mass [GeV]
  // const double MU = 0.0000;//elec mass
  
  int evt_n=0; // event_number, read from the input-file
  int nn=0; // event_counter, in the output file
  
  // GENER: STL  1.0            1           1           1           1  200.00     999.999     CMS
  getline(infile,temp_string); // The very first line is useless
  output <<  "HepMC::IO_Ascii-START_EVENT_LISTING" << endl;
  
  while (getline(infile,temp_string)) {
    
	curstring.clear(); // needed when using several tims istringstream::str(string)
	curstring.str(temp_string);

	if(strstr(temp_string.c_str(),"EVENT"))	{
		curstring >> temp >> evt_n;
		// EVENT:          1       2       1
      		if(evt_n >=K && evt_n < K+M)  {
			output << "E " << evt_n << " -1.0000000000000000e+00 -1.0000000000000000e+00 -1.0000000000000000e+00 20 " << evt_n << " 1 0 0" << endl;
			nn++;
		}

	} else if(strstr(temp_string.c_str(),"VERTEX"))	{
		float x,y,z,t;
		curstring >> temp >> x >> y >> z >> t; 
		// VERTEX:   0.0000       0.0000       0.0000       0.0000          1      0      0      2
      		if(evt_n >=K && evt_n < K+M) output << "V " << evt_n << " 0 " << x << " " << y << " " << z << " " << t << " 0 " << N <<  " 0" << endl;

	} else if(strstr(temp_string.c_str(),"TRACK"))	{
		int useless, part_n, pdg_id;
		float px, py, pz;
		//TRACK:      6   2.9797       3.1399       84.461          1      1      0    -13
		curstring >> temp >> useless >> px >> py >> pz >> part_n >> useless >> useless >> pdg_id;
 		//P 5 2212 -2.0 1.2 8.1 5.2 1 0 0 0 0   
 		if(evt_n >=K && evt_n < K+M) output << "P " << part_n+(evt_n-1)*N << " " << pdg_id << " " << px << " " << py << " " << pz << " " << sqrt(MU*MU + px*px + py*py + pz*pz) << " 1 0 0 0 0\n";
	}

  } // reading loop of the input file
  output << "HepMC::IO_Ascii-END_EVENT_LISTING" << endl;
  infile.close();
  output.close();
  cout << nn << " events written in " << outfilename << endl;
  return;
}

/*
void makefiles(int number_of_files=5) {
        for(int i=0; i<number_of_files; i++)
                makeEventsFile(i);
}
*/
/* Explaination of the format :
 * +++ Event +++
 * E 1 -1.0000000000000000e+00 -1.0000000000000000e+00 -1.0000000000000000e+00 20 0 1 0 0
 *   1 : event number  			<-------
 *   -1 : event scale
 *   -1 : alpha_QCD
 *   -1 : alpha_QED
 *   20 : signal process ID
 *   0 : signal process vertex barcode
 *   1 : number of vertices 		<-------
 *   0 : list of vertices
 *   0 : ?
 *
 * +++ Vertex +++
 * V -1 0 0 0 0 0 0 4 0
 *   -1 : vertex barcode (unique)       <-------
 *    0 : vertex id
 *    0 0 0 0 : vertex x,y,z,t
 *    0 : number of orphans
 *    4 : number of out particles       <-------
 *    0 : weights
 *
 * +++ Particle +++
 * P 5 2212 -2.0 1.2 8.1 5.2 1 0 0 0 0   
 *    5 : barcode			<-------
 *    0 : pdg_id			<-------
 *   -2.0 : px				<-------
 *    1.2 : py				<-------
 *    8.1 : pz				<-------
 *    5.2 : e				<-------
 *    1 : status			<-------
 *    0 0  : polarization eta , phi
 *    0 0  : vertex and ?
 */
