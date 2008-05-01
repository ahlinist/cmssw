
// To run code in this directory, simply set this one environmental
// variable to point to the directory where the the Stntuple package
// was built.  This would be the where the stnmaker.exe is that made 
// the ntuple file you are trying to read.
// that area and it's subdirectories will contain shared object libraries
// that you need to load (will be done automatically below)
// in order to unpack the objects sotored in the file.
// it will also allow you to compile code in this directory
// picking up the correct include files.
//
// If you switch between versions of files you are reading, 
// (reset this envoronmental) you should
// get rid of shared object libraries you made in this directory
// (rm *.so)
// 
// make this point to the release area where Stntuple is built
// setenv BASERELEASE /home/cdf/carla/stntuple/4.2.0
//
// now start root while in this directory.
// compile local code (.L TEleAna.cc+) and read stntuple etc.


{

  // pick up includes locally first
#pragma includepath ./
  // convenient if you are in a subdir of a release that contains Stntuple
#pragma includepath ../include
#pragma includepath ../../include
  // takes Stntuple includes from the file's original release area
  // since the files was create using these includes, these are the best
  // to use for local compliation
#pragma includepath $BASERELEASE/include
  // get the rest of the includes from the cdfsoft area
#pragma includepath $CDFSOFT2_DIR/include
  //#pragma includepath /cdf/code/cdfsoft/dist/releases/development/include

  // the resulting path can be seen with 
  // gSystem->GetIncludePath()

  // I think lines like this bring the includes
  // automatically into every compilation
  // but is not necessary
  //#include <iomanip.h>
  //#include <time.h>

  // load in ROOT physics vectors and event
  // generator libraries

  gSystem->Load("$ROOTSYS/lib/libPhysics.so");
  gSystem->Load("$ROOTSYS/lib/libEG.so");

  TAuthenticate::SetGlobalUser("knuteson");

  // my personal style settings
{
   gStyle->SetOptStat(111111);
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);
   gStyle->SetOptDate(1);
   gStyle->SetLabelSize(0.05);
   gStyle->SetTitleYOffset(0.98);
   gStyle->SetLabelOffset(0.005);
   gStyle->SetTitleOffset(1.1);
   gStyle->SetPadLeftMargin(0.1);
   gStyle->SetHistLineWidth(4);
   gStyle->SetHistLineColor(1);
   gStyle->SetMarkerStyle(21);
}

  // here is how to execute your default rootlogon.C
  // which may set styles parameters etc asthis one does
  gInterpreter->ExecuteMacro("rootlogon_default.C");


}
