#!/usr/bin/perl

use warnings;
use strict;


my $makegifpower = "true";
my $makepngpower = "false";

my $help = "Usage: $0 -args (file1.eps)
-c       => use 'convert' instead of 'ps2img.sh'
-f       => Force the creation even if the gif or png files already exist
-h       => This help screen
-m match => Select files that start with pattern 'match'
-n       => Only files that do not have any gif files are converted
            (i.e. does not convert files where eps file is Newer than gif file)
-o       => convert Only the files explicitly listed
-r       => rest of arguments are meant for ps2img.sh
-v       => verbose (says which files are NOT converted)

By using the file name *.eps, all .eps in your current directory
will be converted to .gif.\n";



# Line 26
my ($force, $verbose, $newOnly, $only, @masks, @addfiles);
my $convertCommand = "./ps2img.sh";
while (@ARGV)
{
    my $arg = shift @ARGV;

    if ($arg =~ /-h/i)
    {
        print "$help\n";
        exit;
    }
    if ($arg =~ /-f/i)
    {
        $newOnly = "";
        $force = "true";
        next;
    }
    if ($arg =~ /-n/i)
    {
        $newOnly = "true";
        $force = "";
        next;
    }   
    if ($arg =~ /-v/i)
    {
        $verbose = "true";
        next;
    }
    if ($arg =~ /-c/i)
    {
	$convertCommand = "convert";
        next;
    }
    if ($arg =~ /-o/i)
    {
        $only = "true";
        next;
    }
    # Line 65
    if ($arg =~ /-m/i)
    {
        my $mask = shift @ARGV;
        push @masks, $mask;
        next;
      }
    if ($arg =~ /-r/i)
      {
        last;
      }
    if ( -e $arg)
      {
        push @addfiles, $arg;
        next;
      }
  }

if (! @masks)
  {
    push @masks, "";
  }

# all left over arguments are for ps2img.sh
my $arg = "@ARGV";


my @epsFiles;
if (! $only)
  {
    foreach my $mask (@masks)
      {
        push @epsFiles, glob ("$mask*.eps");
      }
  }

push @epsFiles, @addfiles;

#print "epsFiles @epsFiles\n";


if ($makegifpower)
  {
    foreach my $eps (@epsFiles)
      {
	(my $gif = $eps) =~ s|\.eps$|.gif|i;
	next if ($gif eq $eps);
	if (! -e $gif || ((-M $eps < -M $gif) && !$newOnly) || $force)
	  {
	    if ($newOnly && -e $gif)
	      {
		warn "Danger: $gif\n";
		next;
	      }
	    my $command = "$convertCommand $arg $eps $gif";
	    print "$command\n";
	    system $command;
	  }
	else
	  {
	    print "$gif is newer than $eps\n" if $verbose;
	  }
      }
  }


if ($makepngpower)
  {
    foreach my $eps (@epsFiles)
      {
	(my $png = $eps) =~ s|\.eps$|.png|i;
	next if ($png eq $eps);
	if (! -e $png || ((-M $eps < -M $png) && !$newOnly) || $force)
	  {
	    if ($newOnly && -e $png)
	      {
		warn "Danger: $png\n";
		next;
	      }
	    my $command = "$convertCommand $arg $eps $png";
	    print "$command\n";
	    system $command;
	  }
	else
	  {
	    print "$png is newer than $eps\n" if $verbose;
	  }
      }
    
  }
