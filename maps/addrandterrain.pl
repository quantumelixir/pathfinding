#!/opt/local/bin/perl
#open(MAP, $ARGV[0]) or die "could not locate specified map file";
opendir(MYDIR, ".") or die "dir open failed: $!\n";
foreach(readdir(MYDIR))
{
	next if ($_ eq "." || $_ eq ".." || not $_ =~ m/.map/ || $_ =~ m/new/ );
	open(MAP, $_);
	local $. = 0;
	$targetline = 4;
	#dddo { $LINE = <MAP> } until $. == $targetline || eof;
	@lines = <MAP>;
	if(not $lines[3] eq "map\n") 
	{
		die "invalid map file. $!\n";
	}

	open(NEWMAP, ">new".$_); 
	for $linenum (0..3)
	{
		print NEWMAP $lines[$linenum];
	}
	close MAP;
	for $linenum(4..$#lines)
	{ 
		$LINE = $lines[$linenum]; 
		#print $LINE;
		@tiles = split(//, $LINE);
		for $tile(@tiles) 
		{ 
			#print $tile; 
			if(not $tile eq "@" and  not $tile eq "\n") 
			{
				if(rand 1 < 0.25) { $tile = "T"; }
				else { $tile = "."; }
			}
		}
		$newline = join('', @tiles);
		print NEWMAP $newline;
		
	}
	close NEWMAP;
}
closedir MYDIR;
