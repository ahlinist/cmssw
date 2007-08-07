#! /usr/bin/env bash

# Author : Samvel Khalatian (samvel at fnal dot gov)
# Created: 07/30/07
# License: GPL

# Include function
. ./scripts/print_note.sh
. ./scripts/print_usage.sh
. ./scripts/print_error.sh
. ./scripts/string_match.sh
. ./scripts/path.sh

# Declare constants
declare -r cUSAGE="`basename $0` [<DIR_TO_INSTALL_AT>]"

declare -r nERR_DIR_TO_INSTALL_DOES_NO_EXIST=65

cDirToInstall="`echo ${1:-'.'} | sed 's_/*$__'`/"

if [[ ! -d "$cDirToInstall" ]]; then
  printError "Specified folder to install software at does not exist: \
'$cDirToInstall'.\n"
  printUsage
  exit $nERR_DIR_TO_INSTALL_DOES_NO_EXIST
fi

cScriptsDir="./"
if [[ -z `echo "$cDirToInstall" | sed -n -e '\_^\./$_ p'` ]]; then
  cScriptsDir="${PWD}/"
  getAbsolutePath cDirToInstall
  cDirToInstall="${cDirToInstall}/"
fi

ln -s "${cScriptsDir}scripts/print_error.sh" \
      "${cDirToInstall}bash_print_error.sh"
ln -s "${cScriptsDir}scripts/print_usage.sh" \
      "${cDirToInstall}bash_print_usage.sh"
ln -s "${cScriptsDir}scripts/print_note.sh" \
      "${cDirToInstall}bash_print_note.sh"
ln -s "${cScriptsDir}scripts/find.sh" \
      "${cDirToInstall}find"
ln -s "${cScriptsDir}scripts/create_flag.sh" \
      "${cDirToInstall}create_flag"
ln -s "${cScriptsDir}scripts/includes_to_cmssw.sh" \
      "${cDirToInstall}cmssw_fix_includes"

for cMakefile in `ls -1 ./Makefile_*`
do
  cSuffix=${cMakefile#*_}
  ln -s "${cScriptsDir}scripts/make" "${cDirToInstall}make_${cSuffix}"
done

printNote "Thank you for using install.sh script. Now you are ready to start \
using RunSummaryFlags project. Start with README file /* it contains a lot of \
interesting information */"
echo
echo -e "Links were installed into folder: \033[1m${cDirToInstall}\033[0m"

exit 0
