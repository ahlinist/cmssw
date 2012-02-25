#!/usr/bin/env perl
use DirHandle;
$oldfile = @ARGV[0];
$destination = @ARGV[1];
print "here $oldfile $destination \n";
open( INFILE, "$oldfile") || die "ERROR::could not open input file $oldfile";
print "$INFILE\n";
while( $record = <INFILE> )
{
    if($record =~ /PYTHONBASE=/){
	print "found PYTHONBASE = $record\n";
	@tok1 = split(/=\"/, $record);
	print "this is tok1\n";
	print "@tok1[1]\n";
	mkdir ($destination);
	mkdir ("$destination\/base");
	opendir(LIBS,"$destination\/base");
	@tokens = split(/\:/,@tok1[1]);
	while($#tokens != -1)
	{
	    if(@tokens[$#tokens] =~ /\$/)
	    {}
	    else
	    {
		print "token $#tokens @tokens[$#tokens]\n";
		opendir( DIR, @tokens[$#tokens]);
		@dots = readdir(DIR);
		foreach $fname (@dots)
		{
		    my $ffname = @tokens[$#tokens].'/'.$fname;
		    if(grep {(/\.py/ || /\.pyc/)} $fname)
		    {
			$foundlink = readlink "$destination\/base\/$fname";
			print "replacing previous link $foundlink\n" if ($foundlink);
			system `rm $destination\/base\/$fname` if $foundlink;
			print " linking $ffname\n";
			symlink("$ffname","$destination\/base\/$fname");
		    }
		    if(($fname ne '.') and ($fname ne "..") and -d $ffname)
		    {
			print "verifying directory $ffname\n";
                        # make the dir if it does not exist
			print $destination."/base/".$fname."\n";
			$ddest = $destination."/base/".$fname;
			print "check existence of $ddest\n";
			mkdir ($ddest) unless -e $ddest;
			print "checking subdirs of $fname\n";
                        # go one level down and readdir
			opendir( SDIR, $ffname);
			@subdirs = readdir(SDIR);
			foreach $dname (@subdirs)
			{
			    my $ddname = $ffname."/".$dname;
			    $foundlink = readlink "$ddest\/$dname";
			    print "replacing previous link $foundlink\n" if ($foundlink);
			    system `rm -r $ddest\/$dname` if $foundlink;
			
			    symlink("$ddname","$destination\/base\/$fname\/$dname") if -d $ddname;
			    print " linking directory $ddname\n" if -d $ddname; 
			    symlink("$ddname","$destination\/base\/$fname\/$dname") if (grep {(/\.py/ || /\.pyc/)} $dname);
			    print " linking file $dname\n" if (grep {(/\.py/ || /\.pyc/)} $dname);
			}
		    }
		}
		pop(@tokens);
		
	    }
	    
	}
    }

    if($record =~ /PYTHONPATH=/){
	print "found PYTHONPATH = $record\n";
	@tok1 = split(/=\"/, $record);
	print "this is tok1\n";
	print "@tok1[1]\n";
	mkdir ($destination);
	mkdir ("$destination\/path");
	opendir(LIBS,"$destination\/path");
	@tokens = split(/\:/,@tok1[1]);
	while($#tokens != -1)
	{
	    if(@tokens[$#tokens] =~ /\$/)
	    {}
	    else
	    {
		print "token $#tokens @tokens[$#tokens]\n";
		opendir( DIR, @tokens[$#tokens]);
		@dots = readdir(DIR);
		foreach $fname (@dots)
		{
		    my $ffname = @tokens[$#tokens].'/'.$fname;
		    if(grep {(/\.py/ || /\.pyc/)} $fname)
		    {
			$foundlink = readlink "$destination\/path\/$fname";
			print "replacing previous link $foundlink\n" if ($foundlink);
			system `rm $destination\/path\/$fname` if $foundlink;
			print " linking $ffname\n";
			symlink("$ffname","$destination\/path\/$fname");
		    }
		    if(($fname ne '.') and ($fname ne "..") and -d $ffname)
		    {
			print "verifying directory $ffname\n";
                        # make the dir if it does not exist
			print $destination."/path/".$fname."\n";
			$ddest = $destination."/path/".$fname;
			print "check existence of $ddest\n";
			mkdir ($ddest) unless -e $ddest;
			print "checking subdirs of $fname\n";
                        # go one level down and readdir
			opendir( SDIR, $ffname);
			@subdirs = readdir(SDIR);
			foreach $dname (@subdirs)
			{
			    my $ddname = $ffname."/".$dname;
			    $foundlink = readlink "$ddest\/$dname";
			    print "replacing previous link $foundlink\n" if ($foundlink);
			    system `rm -r $ddest\/$dname` if $foundlink;
			
			    symlink("$ddname","$destination\/path\/$fname\/$dname") if -d $ddname;
			    print " linking directory $ddname\n" if -d $ddname; 
			    symlink("$ddname","$destination\/path\/$fname\/$dname") if (grep {(/\.py/ || /\.pyc/)} $dname);
			    print " linking file $dname\n" if (grep {(/\.py/ || /\.pyc/)} $dname);
			}
		    }
		}
		pop(@tokens);
		
	    }
	    
	}
    }

}
