#!/bin/bash

function die { echo $1: status $2 ;  exit $2; }

randomPoisson 3 || die 'randomPoisson failed' $?
