#!/bin/tcsh

setenv h `hostname`
if ( `expr $h : "cdf[0-9].*"` )              echo 'uchicago'
if ( `expr $h : "b0pcmit[0-9][0-9].*"` )     echo 'mitcdf'
if ( `expr $h : "fcdf.*"` )                  echo 'fcdflnx'
if ( `expr $h : ".*-clued0.*"` )             echo 'clued0'
if ( `expr $h : "d0mino.*"` )                echo 'clued0'
if ( `expr $h : ".*[.]rtc"` )                echo 'rtc'
if ( `expr $h : "lxplus.*"` )                echo 'lxplus'
if ( `expr $h : "h1.*"` )                    echo 'desy'
if ( `expr $h : "fnpcsrv*"`)                 echo 'fixedtarget'
if ( `expr $h : "fngp*"`)                    echo 'fixedtarget'
if ( `expr $h : "fnpc1.*"` )                 echo 'fnpc1'
if ( `expr $h : "pat.*"` )                   echo 'MrennaDesktop'
if ( `expr $h : "d-.*.dhcp.fnal.gov"` )      echo 'laptop'
if ( `expr $h : "bruce-knutesons-computer.local"` )  echo 'laptop'
if ( `expr $h : "iodine.fnal.gov"` )         echo 'atom'
if ( `expr $h : "fluorine.fnal.gov"` )       echo 'atom'
if ( `expr $h : ".*ium.fnal.gov"` )          echo 'atom' 
if ( `expr $h : "cmswn.*.fnal.gov"` )          echo 'cmsuaf'
if ( `expr $h : "cmslpc.*.fnal.gov"` )          echo 'cmslpc'
if ( `expr $h : "fcdfcaf.*.fnal.gov"` )      echo 'cdfcaf'
if ( `expr $h : "Mephisto.*"` )              echo 'harvard'
if ( `expr $h : ".*cl.*.harvard.edu"` )      echo 'harv-heplcl'
if ( `expr $h : "node-[0-9][0-9]"` )         echo 'DEAS'



