#! /usr/bin/env bash

cPath=${1-""}

function correctInclude() {
  local _cFile=$1

  sed -e '\_^#include_ s_^#include "interface/_#include "CalibTracker/SiStripRunSummary/interface/_' < "${_cFile}" > "${_cFile}.tmp"
  mv "${_cFile}.tmp" "${_cFile}"
  echo "[done] ${_cFile}"
}

if [[ -nz "${cPath}" ]]; then
  if [[ -d "${cPath}" ]]; then
    # Path specified
    for cFile in `ls -1 ${cPath}`
    do
      correctInclude "${cPath}/${cFile}"
    done
  else
    # File specified
    correctInclude "${cPath}"
  fi
else
  # Path is not specified
  echo "[error] Input is not specified"
fi

exit 0
