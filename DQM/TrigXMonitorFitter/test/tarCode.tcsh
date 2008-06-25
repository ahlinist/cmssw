#!/bin/tcsh

tar -czvf TXMonRootFits.tgz --exclude \*~ --exclude \*.o --exclude \*.exe  --exclude \*.d --exclude \*.dll --exclude \*.so --exclude \*.root --exclude \*.\*ps --exclude \*.gif --exclude singles --exclude L1 --exclude L2 --exclude L3 --exclude \*.tgz --exclude \*root.\? * 
