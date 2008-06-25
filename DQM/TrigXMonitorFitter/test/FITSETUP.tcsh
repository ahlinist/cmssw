#!/bin/tcsh

echo "Setting up supplemental fit environment."

set Workspace = "./../TXMonFitterWorkspace"
set DataSource1 =  "./../testdata"
set DataSource2 =  "./../testdata"


	mkdir $Workspace
	cp ./* $Workspace
	cd $Workspace


	source /uscmst1/prod/sw/cms/cshrc prod
	eval `scramv1 runtime -csh`

	
	mkdir ./L1
	mkdir ./L2
	mkdir ./L3

	mkdir ./HTML
	mkdir ./HTML/L1
	mkdir ./HTML/L2
	mkdir ./HTML/L3

	mkdir ./HTML/root

	mkdir ./Singles
	mkdir ./Singles/L1
	mkdir ./Singles/L2
	mkdir ./Singles/L3

	mkdir ./Singles/root

	ln -s $DataSource1 ./txmon_new
	ln -s $DataSource2 ./txmon_old


        gmake

echo "Done!"
