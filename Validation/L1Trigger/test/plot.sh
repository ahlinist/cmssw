#! /bin/sh

cd $1
L1RPCAddDQMHistos L1_RPC *root
L1RPCPlotHistos histos.root
rm *pt9d* *pt980_*
