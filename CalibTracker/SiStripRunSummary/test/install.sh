#! /usr/bin/env bash

# Author : Samvel Khalatian (samvel at fnal dot gov)
# Created: 07/30/07
# License: GPL

# Include function
. ./scripts/print_note.sh

ln -s ./scripts/print_error.sh bash_print_error.sh
ln -s ./scripts/print_usage.sh bash_print_usage.sh
ln -s ./scripts/print_note.sh  bash_print_note.sh
ln -s ./scripts/find.sh        find
ln -s ./scripts/create_flag.sh create_flag

for cMakefile in `ls -1 ./Makefile_*`
do
  cSuffix=${cMakefile#*_}
  ln -s ./scripts/make           make_${cSuffix}
done

printNote "Thank you for using install.sh script. Now you are ready to start \
using RunSummaryFlags project. Start with README file /* it contains a lot of \
interesting information */"

exit 0
