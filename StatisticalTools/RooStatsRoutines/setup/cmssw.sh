#!/bin/bash

echo
echo Setting up additional RooFit environment...
echo

export PATH=${PATH}:${ROOFITSYS}/bin:${ROOFITSYS}/config

echo Remember that ROOT in CMSSW is nonstandard in some aspects.
echo For one, RooFit and RooStats headers are in an unusial location
echo and will not be found by compiler by default. To amend that,
echo issue the following command at ROOT command prompt
echo
echo '  gSystem -> SetIncludePath( "-I$ROOFITSYS/include" );'
echo
