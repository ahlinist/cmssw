#!/usr/bin/perl -w

$command = join (" ", @ARGV);
if ($pid = fork)
  {
    #parent process
  }

elsif (defined $pid)
  {
    #child process
    open(STDOUT,">/dev/null");
    open(STDERR,">&STDOUT");
    exec $command;
  }

elsif ($! =~ /No more process/)
  {
    die ("Too many processes\n");
  }

else
  {
    die ("Weird fork error\n");
  }	
