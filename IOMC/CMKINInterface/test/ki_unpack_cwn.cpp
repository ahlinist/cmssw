//***************************************************************
//* First test of CMKIN++ with the Pythia interface             *
//* using the GENSER framework.                                 *
//*                                                             *
//***************************************************************
//* author: Hector Naves Sordo -- Universidad de Oviedo / IFCA  *
//* email:  Hector.Naves.Sordo@cern.ch                          *
//* First Release: 30/05/05                                     *
//*                                                             *
//***************************************************************

#include <iostream>
#include <memory>
#include <iomanip>
#include "interface/Kikeys.h"
#include "interface/Kiffread.h"
#include "interface/Util.h"
#include "interface/Kiutil.h"
#include "interface/Kigeneral.h"
#include "interface/Kiunit.h"
#include "interface/Packlib.h"
#include "interface/Kicwn.h"

using namespace std;

int main()
{

  //********************************************
  //* Initialise FFREAD and CMSIM IODB package *
  //********************************************

  const char* limp="LIMP";
  const char* lout="LOUT";
  const char* size="SIZE";

  int* nffword = _nffword;
  int* lun_ffin = _lun_ffin;
  int* lun_ffout = _lun_ffout;
  int* ncharPerKey = _ncharPerKey;


  ffinit(nffword);
  ffset(limp, lun_ffin);
  cout << "Hola" << endl;

  ffset(lout, lun_ffout);
  ffset(size, ncharPerKey);



  cout << endl;
  cout << "** Initialising FFKEY data cards..." << endl;

  // CMKIN data card definitions
  ki_def_keys();

  // CMSIM IODB data card definitions
  io_ffkey();

  cout << endl;
  cout << "** Calling KI_FFGO..." << endl;


}

