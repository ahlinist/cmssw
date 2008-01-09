#!/bin/bash

# Move into config directory (quietly)
pushd $BUILD_HOME/TriDAS/RecoLuminosity/HLXReadOut/Config &> /dev/null

# define the project
Project=RecoLuminosity/HLXReadOut

# BaseSetup *MUST* come fist
source BaseSetup_slc3
source DIPSetup
source OracleSetup_slc3

popd &> /dev/null
