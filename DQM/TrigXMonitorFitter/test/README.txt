2008 June 25th
Trigger Cross Section Monitor Fitter (TXMon) Relase Note


1. To Download TXMon Fitter Package:

source /uscmst1/prod/sw/cms/cshrc uaf
scramv1 project CMSSW CMSSW_2_0_9 (or the release of your choice)
cd CMSSW_2_0_9/src/
eval `scramv1 runtime -csh`
project CMSSW
kserver_init

cvs co DQM/TrigXMonitorFitter
cd DQM/TrigXMonitorFitter/test/


2. To Setup Workspace

source ./FITSETUP.tcsh

3a. To run fitAllTXMonUsingRuns.pl normally

./fitAllTXMonUsingRuns.pl -f runnumbers.txt

Please Note: There are several options through 'fitAllTXMonUsingRuns.pl.' Type '-h' instead of '-f' for available options. 'runnumber.txt' contains some run numbers associated with '.root' files located in the './../testdata' subdirectory.

The output will be .eps, and .jpg files in ./L1, ./L2, and ./L3 directories, and .eps, .jpg, .HTML, and .root files in the ./HTML/ subdirectories.


3.b To fitSingleTXMonUsingRuns.pl normally

 ./fitSingleTXMonUsingRuns.pl [flags] Triggername runnumber

Please Note: There are several options through 'fitSingleTXMonUsingRuns.pl.' Type '-h' for available options. One example of how to execute this script properly, and normally is:


 ./fitSingleTXMonUsingRuns.pl L1_CEM4_PT4_PS250 240250

The output will be .eps, .jpg, .HTML, and .root files in the ./Singles/ subdirctories. 

4. Enjoy


