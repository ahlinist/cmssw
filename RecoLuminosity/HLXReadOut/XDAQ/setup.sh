#!/bin/sh

# XDAQ basics
export XDAQ_ROOT=$HOME/TriDAS
export XDAQ_OS=linux
export XDAQ_PLATFORM=x86

export LD_LIBRARY_PATH
export LD_LIBRARY_PATH="./:$XDAQ_ROOT/daq/xdaq/lib/linux/x86/:$XDAQ_ROOT/daq/extern/xerces/linuxx86/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xtuple/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/asyncresolv/linuxx86/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/toolbox/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xoap/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/pt/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xdaq/executive/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xdaq/hyperdaq/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/pt/fifo/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/pt/http/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/log4cplus/linuxx86/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/mimetic/linuxx86/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/log4cplus/udpappender/lib/linux/x86:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/log4cplus/xmlappender/lib/linux/x86:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/extern/cgicc/linuxx86/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xdata/lib/linux/x86/:$LD_LIBRARY_PATH" 
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xcept/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xgi/lib/linux/x86/:$LD_LIBRARY_PATH" 
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/xrelay/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/daq/config/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/lumi/HF/CoreUtils/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/lumi/HF/HLXReadout/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$XDAQ_ROOT/lumi/HF/HLXReadout/lib/linux/x86/:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="${ORACLE_HOME}:${ORACLE_HOME}/lib:${ORACLE_HOME}/rdbms/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="${ROOTSYS}/lib:$LD_LIBRARY_PATH"

echo $LD_LIBRARY_PATH
