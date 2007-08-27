#!/usr/bin/env perl

use strict;

my $current = "./";

my @subdirs = qw (L1 L2 L3 Ln); #N-trigger template

my $maxAge = shift || 1; # 1 day default

foreach my $sub (@subdirs)
	{
	    chdir "$current/../$sub";
	    my @files = glob ("*");
	    foreach my $file (@files)
		{
	        	if (-M $file > $maxAge)
			{
		            #print "removing $file\n";
		            #system "ls -l $file";
		            unlink $file;
			}
		}
	}
