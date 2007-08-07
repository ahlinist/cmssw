#!/bin/bash

# Path function

# Author  : Samvel Khalatian (ksam@mail.ru)
# Date    : 01/12/2006
# Modified: 01/12/2006

if [[ -z "${_GET_ABSOLUTE_PATH}" ]]; then
  _GET_ABSOLUTE_PATH=1

  #  Get Absolute Path to any existing(!) folder
  #  Arguments
  #    1  Variable Name to reference
  #    2  Path to folder
  #  Return
  #    Path is written to variable passed as name. It is empty on error
  getAbsolutePath() {
      local _cVarRef=${1:-""}

      [[ -n ${_cVarRef} ]] && {
    pushd ${!_cVarRef} &> /dev/null
    eval "$_cVarRef=`pwd`"
    popd &> /dev/null
      } || eval "$_cVarRef="
  }
fi # End _GET_ABSOLUTE_PATH
