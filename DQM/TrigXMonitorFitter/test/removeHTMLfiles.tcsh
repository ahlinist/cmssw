#!/bin/tcsh


# begin program

echo "Beginning RemoveHtmlfiles.sh"

echo "Removing ./HTML/L1 files"
rm -r ./HTML/L1/*
echo "./HTML/L1:"
ls ./HTML/L1
echo 

echo "Removing ./HTML/L2 files"
rm -r ./HTML/L2/*
echo "./HTML/L2:"
ls ./HTML/L2
echo

echo "Removing ./HTML/L3 files"
rm -r ./HTML/L3/*
echo "./HTML/L3:"
ls ./HTML/L3
echo

echo "Removing ./HTML/root files"
rm -r ./HTML/root/*
echo "./HTML/root:"
ls ./HTML/root
echo


echo "Done removing files"
echo "Now exiting"
echo

# end program
