#!/bin/sh

# XDAQ basics
export XDAQ_ROOT=/opt/xdaq
export XDAQ_OS=linux
export XDAQ_PLATFORM=x86_slc4

export LD_LIBRARY_PATH
export LD_LIBRARY_PATH="/opt/xdaq/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="/opt/TriDAS/RecoLuminosity/HLXReadOut/CoreUtils/lib/${XDAQ_OS}/${XDAQ_PLATFORM}/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="/opt/TriDAS/RecoLuminosity/HLXReadOut/HLXCoreLibs/lib/${XDAQ_OS}/${XDAQ_PLATFORM}/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="/opt/TriDAS/hcal/hcalBase/lib/${XDAQ_OS}/${XDAQ_PLATFORM}:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="${ORACLE_HOME}/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="${ROOTSYS}/lib:$LD_LIBRARY_PATH"

echo $LD_LIBRARY_PATH
