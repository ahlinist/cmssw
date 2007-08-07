#! /usr/bin/env bash

# Find arbitrary RegExp pattern in one of files:
#   ./interface/*.h
#   ./src/*.cc
#   ./bin/*.cc
#
# Usage:
#
#   find_include.sh <dir_or_file> <pattern>

# Author : Samvel Khalatian (samvel at fnal dot gov)
# Created: 07/30/07
# License: GPL

# Include function
. bash_print_error.sh
. bash_print_usage.sh

# Declare constants
declare -r     cDIR=${1:-""}
declare -r cPATTERN=${2:-""}

declare -r cUSAGE="`basename $0` <DIR_OR_FILE> <NEW_FLAG_CLASS_NAME>"

declare -r nERR_NOT_SYMLINK=65
declare -r  nERR_NO_PATTERN=66

function findFiles() {
  local     _cDir=$1
  local _cPattern=$2

  for cFile in `ls "$_cDir"`
  do
    cLine=`sed -n -e "\_${_cPattern}_ p" < "${_cDir}/${cFile}"`
    if [[ -nz "${cLine}" ]]; then
      echo -e "[\033[1m${_cDir}/${cFile}\033[0m]"
      sed -n -e "\_${_cPattern}_ p" < "${_cDir}/${cFile}"
      echo
    fi
  done
}

if [[ ! -h `basename $0` ]]; then
  printError "Script should be invoked via symbolic script.\n"
  printUsage
  exit $nERR_NOT_SYMLINK
fi

if [[ -z "${cDIR}" ]]; then
  printError "Directory/file is not specified.\n"
  printUsage
  exit $nERR_NO_PATTERN
fi

if [[ -z "${cPATTERN}" ]]; then
  printError "Pattern is not specified.\n"
  printUsage
  exit $nERR_NO_PATTERN
fi

findFiles "${cDIR}" "${cPATTERN}"

exit 0
