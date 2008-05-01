// Reads events from a file, then writes to STDOUT those that pass a cut
// Changing the cut requires recompiling, just like checkDistributionsWithCuts
// Filename is passed as argument
// command option --preserveComments means that the original line is not stripped of comments

// Conor Henderson, 28 June 2007
// $Id: filterEventsFromFile.cc,v 1.1 2007/07/16 22:01:31 conor Exp $


#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>

#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"

using namespace std;


/*******************************************************
                Impose your cut here
*******************************************************/

bool passesMyCutQ(QuaeroEvent& e)
{
  bool ans = true;

  //  ans = false;
//   HepLorentzVector mu;
//   if(e.getThisObject("mu-",1)) {
//     mu = e.getThisObject("mu-",1)->getFourVector();  
//     cout << "mu- " << mu.m() <<", " << mu.perp() <<", "<<mu.eta()<<", "<<mu.phi() <<endl;
//   }
//  HepLorentzVector v1 = e.getPmiss();  
//   cout << "Pmiss = "<<v1.perp() <<endl;
//   if(v1.perp()>20) {
//     ans = true;
//   }

  return ans;
}


int main(int argc, char* argv[])
{
  if(argc<=1) {
    cout << "Usage: "<<argv[0]<<" [--preserveComments] <inputFile>"<<endl;
    cout << "Events passing the cut will be written to STDOUT" <<endl;    
    cout << "With option --preserveComments  comment fields will also be passed to the output" <<endl;
    return 1;
  }
  //  assert(argc==2);


  // set this flag based on option
  int preserveComments = 0;


  // standard struct for options processing
  // must be terminated by  entry { 0, 0, 0, 0 }
  // options can be setup to automatically set flags
  // that's what I do for --preserveComments
  // in comments is a help option which does not set a flag
  struct option long_options[] = {
    { "preserveComments", no_argument, &preserveComments, 1},
    //    { "help", no_argument, NULL, 'h' },
    { 0, 0, 0, 0 }
  };
  int c;
 

  /* getopt_long stores the option index here. */
  int option_index = 0;

  // loop to be sure to get all options
  while(1) {
    // there are no short options  - that the empty string here
    c = getopt_long(argc, argv, "",long_options,&option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {

      // example of an option that does not set a flag
//        case 'h' :
// 	 // this is for --help
// 	 cout << "Usage: "<<argv[0]<<" [--preserveComments] <inputFile>"<<endl;
// 	 cout << "Events passing the cut will be written to STDOUT" <<endl;    
// 	 cout << "With option --preserveComments  comment fields will also be passed to the output" <<endl;
// 	 return 1;
// 	 break;

      case 0 :
	// in this case, getopt_long returning 0 means that it found an option
	// with a valid associated int flag, and so it wrote the value to this flag
	// as set out in the long_options

	// since my option is constructed to set the flag for preserving comments
	// then that's all I need to do
	break;
	   
	
    default:
      // an error message is already printed by the getopt_long function in this case,
      // so just exit
      return 1;
    }
   

  }





  string file_name = "";
  // get input file name after command-line options have been processed
  // getopt.h has given us a global int optind, which now contains the index of argv
  // that is left after processing all options
  //  cout << "Index = " << optind << endl;
  if (optind < argc) {
    file_name = argv[optind];    
  }
  //  cout << file_name <<endl;


  //  if(preserveComments)
  //     cout << "Try to preserve comments" <<endl;


  // will read this file simultaneously as regular file and Quaero file
  ifstream file_in(file_name.data());
  if(!file_in.is_open()) {
    cout << "Cant open file " << file_name <<endl;
    return 1;
  }
  string original_line;

  QuaeroItemFile<QuaeroEvent> quaero_infile(file_name);
  QuaeroEvent event;
  while(quaero_infile.nextEvent(event)) {

    //also read from std stream at same time
    getline(file_in,original_line);

    //apply the user cut
    if(passesMyCutQ(event)) {

      if(preserveComments)
	// then print original line
	cout << original_line <<endl;
      else 
	// print the Quaero event, from which comments have been removed
	cout << event <<endl;
    }

  }
  file_in.close();

  
}
