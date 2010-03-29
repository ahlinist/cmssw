#! /bin/sh

cd $1
L1RPCAddDQMHistos L1_RPC/Resolution *root
L1RPCPlotHistos 4 histos.root
rm *pt9d* *pt980_*
