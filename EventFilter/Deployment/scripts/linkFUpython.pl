#!/usr/bin/env perl
use DirHandle;
$oldfile = @ARGV[0];
$destination = @ARGV[1];
print "here $oldfile $destination \n";
open( INFILE, "$oldfile") || die "ERROR::could not open input file $oldfile";
print "$INFILE\n";
while( $record = <INFILE> )
{

    if($record =~ /PYTHONPATH=/){
	print "found PYTHONPATH = $record\n";
	@tok1 = split(/=\"/, $record);
#	print "@tok1\n";
	#pop(@tok1);
	#pop(@tok1);
	print "this is tok1\n";
	print "@tok1[1]\n";
	mkdir ($destination);
	opendir(LIBS,$destination);
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
			$foundlink = readlink "$destination\/$fname";
			print "replacing previous link $foundlink\n" if ($foundlink);
			system `rm $destination\/$fname` if $foundlink;
			print " linking $ffname\n";
			symlink("$ffname","$destination\/$fname");
		    }
		    if(($fname ne '.') and ($fname ne "..") and -d $ffname)
		    {
			print "verifying directory $ffname\n";
# make the dir if it does not exist
			print $destination."/".$fname."\n";
			$ddest = $destination."/".$fname;
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
			
			    symlink("$ddname","$destination\/$fname\/$dname") if -d $ddname;
			    print " linking directory $ddname\n" if -d $ddname; 
			    symlink("$ddname","$destination\/$fname\/$dname") if (grep {(/\.py/ || /\.pyc/)} $dname);
			    print " linking file $dname\n" if (grep {(/\.py/ || /\.pyc/)} $dname);
			}
		    }
		}
		pop(@tokens);
		
	    }
	    
	}
    }
}
