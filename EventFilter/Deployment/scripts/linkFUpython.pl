#!/usr/bin/env perl
use DirHandle;
$oldfile = @ARGV[0];
$destination = @ARGV[1];
print "here $oldfile $destination \n";
open( INFILE, "$oldfile") || die "ERROR::could not open input file $oldfile";
print "$INFILE\n";
while( $record = <INFILE> )
{
    if($record =~ /PYTHONHOME=/){
	print "found PYTHONHOME = $record\n";
	@tok1 = split(/=\"/, $record);
	print "this is tok1\n";
	print "@tok1[1]\n";
	mkdir ($destination);
	mkdir ("$destination\/home");
	opendir(LIBS,"$destination\/home");
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
			$foundlink = readlink "$destination\/home\/$fname";
			print "replacing previous link $foundlink\n" if ($foundlink);
			system `rm $destination\/home\/$fname` if $foundlink;
			print " linking $ffname\n";
			symlink("$ffname","$destination\/home\/$fname");
		    }
		    if(($fname ne '.') and ($fname ne "..") and -d $ffname)
		    {
			print "verifying directory $ffname\n";
                        # make the dir if it does not exist
			print $destination."/home/".$fname."\n";
			$ddest = $destination."/home/".$fname;
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
			
			    symlink("$ddname","$destination\/home\/$fname\/$dname") if -d $ddname;
			    print " linking directory $ddname\n" if -d $ddname; 
			    symlink("$ddname","$destination\/home\/$fname\/$dname") if (grep {(/\.py/ || /\.pyc/)} $dname);
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
