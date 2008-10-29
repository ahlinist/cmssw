#!/bin/bash

# Move into config directory (quietly)
pushd $BUILD_HOME/RecoLuminosity/HLXReadOut/Config &> /dev/null

# define the project
Project=RecoLuminosity/HLXReadOut

# BaseSetup *MUST* come first
source BaseSetup_slc4
source DIPSetup
source OracleSetup_slc4

popd &> /dev/null
