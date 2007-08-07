#! /usr/bin/env bash

# Author : Samvel Khalatian (samvel at fnal dot gov)
# Created: 07/30/07
# License: GPL

# Include function
. bash_print_error.sh
. bash_print_usage.sh

# Declare constants
declare -r cUSAGE="`basename $0` <PATTERN_OF_FILES_TO_FIX>"

declare -r nERR_NO_PATTERN=65

cFiles=${1-""}

function correctInclude() {
  local _cFile=$1

  sed -e '\_^#include_ s_^#include "interface/_#include "CalibTracker/SiStripRunSummary/interface/_' < "${_cFile}" > "${_cFile}.tmp"
  mv "${_cFile}.tmp" "${_cFile}"
  echo "[done] ${_cFile}"
}

if [[ -z "${cFiles}" ]]; then
  printError "Pattern of files to be fixed is not specified.\n"
  printUsage
  exit $nERR_NO_PATTERN
fi

# Might be pattern or folder
cDir=`dirname "${cFiles}"`
if [[ -d "${cFiles}" ]]; then
  cDir="${cFiles}"
fi

for cFile in `ls -1 ${cFiles}`
do
  correctInclude "${cDir}/`basename ${cFile}`"
done

exit 0
