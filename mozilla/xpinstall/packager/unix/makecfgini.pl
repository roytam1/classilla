#!perl
# 
# The contents of this file are subject to the Netscape Public
# License Version 1.1 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://www.mozilla.org/NPL/
#  
# Software distributed under the License is distributed on an "AS
# IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
# implied. See the License for the specific language governing
# rights and limitations under the License.
#  
# The Original Code is Mozilla Communicator client code, released
# March 31, 1998.
# 
# The Initial Developer of the Original Code is Netscape
# Communications Corporation. Portions created by Netscape are
# Copyright (C) 1998-1999 Netscape Communications Corporation. All
# Rights Reserved.
# 
# Contributor(s): 
# Sean Su <ssu@netscape.com>
# Samir Gehani <sgehani@netscape.com>
# 

#
# This perl script parses the input file for special variables
# in the format of $Variable$ and replace it with the appropriate
# value(s).
#
# Input: .it file
#             - which is a .ini template
#        version
#             - version to display on the blue background
#        Path to staging area
#             - path on where the seamonkey built bits are staged to
#        xpi path
#             - path on where xpi files will be located at
#        URL path
#             - path to where the .xpi files are staged.  can be
#               either ftp:// or http://
#
#   ie: perl makecfgini.pl config.it 5.0.0.1999120608 z:\exposed\windows\32bit\en\5.0 d:\builds\mozilla\dist\win32_0.obj\install\xpi ftp://sweetlou/products/client/seamonkey/windows/32bit/x86/1999-09-13-10-M10
#

# Make sure there are at least two arguments
if($#ARGV < 4)
{
  die "usage: $0 <.it file> <version> <staging path> <.xpi path> <URL path>

       .it file     : input ini template file
       version      : version to be shown in setup.  Typically the same version
                      as show in mozilla.exe.
       staging path : path to where the components are staged at
       .xpi path    : path to where the .xpi files have been built to
       URL path     : URL path to where the .xpi files will be staged at.
                      Either ftp:// or http:// can be used
       \n";
}

$inItFile         = $ARGV[0];
$inVersion        = $ARGV[1];
$inStagePath      = $ARGV[2];
$inXpiPath        = $ARGV[3];
$inURLPath        = $ARGV[4];

# Get the name of the file replacing the .it extension with a .ini extension
@inItFileSplit    = split(/\./,$inItFile);
$outIniFile       = $inItFileSplit[0];
$outIniFile      .= ".ini";

# Open the input file
open(fpInIt, $inItFile) || die "\ncould not open $ARGV[0]: $!\n";

# Open the output file
open(fpOutIni, ">$outIniFile") || die "\nCould not open $outIniFile: $!\n";

print "\n Making $outIniFile...\n";

# While loop to read each line from input file
while($line = <fpInIt>)
{
  # For each line read, search and replace $InstallSize$ with the calculated size
  if($line =~ /\$InstallSize\$/i)
  {
    $installSize          = 0;

    # split read line by ":" deliminator
    @colonSplit = split(/:/, $line);
    if($#colonSplit >= 0)
    {
      $componentName    = $colonSplit[1];
      chop($componentName);

      $installSize      = OutputInstallSize("$inStagePath/$componentName");
    }

    print fpOutIni "Install Size=$installSize\n";
  }
  elsif($line =~ /\$InstallSizeArchive\$/i)
  {
    $installSizeArchive = 0;

    # split read line by ":" deliminator
    @colonSplit = split(/:/, $line);
    if($#colonSplit >= 0)
    {
      $componentName = $colonSplit[1];
      chop($componentName);

      $installSizeArchive = OutputInstallSizeArchive("$inXpiPath/$componentName");
    }

    print fpOutIni "Archive Size=$installSizeArchive\n";
  }
  elsif($line =~ /\$Version\$/i)
  {
    # For each line read, search and replace $Version$ with the version passed in
    $line =~ s/\$Version\$/$inVersion/i;
    print fpOutIni $line;
  }
  else
  {
    # For each line read, search and replace $URLPath$ with the supplied URL
    $line =~ s/\$URLPath\$/$inURLPath/i;
    print fpOutIni $line;
  }
}

print " done!\n";

# end of script
exit(0);

sub OutputInstallSize()
{
  my($inPath) = @_;
  my($installSize);

  print "   calculating size for $inPath\n";
  $installSize    = GetSpaceRequired($inPath);
  $installSize   += 32768; # take into account install.js
  $installSize    = int($installSize / 1024);
  $installSize   += 1;
  return($installSize);
}

sub OutputInstallSizeArchive()
{
  my($inPath) = @_;
  my($installSizeArchive);
  my($dev, $ino, $mode, $nlink, $uid, $gui, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks);

  print "   calculating size for $inPath\n";
  ($dev, $ino, $mode, $nlink, $uid, $gui, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks) = stat $inPath;
  $installSizeArchive    = int($size / 1024);
  $installSizeArchive   += 1;
  return($installSizeArchive);
}

##
# GetSpaceRequired
#
# Finds the space used by the contents of a dir by recursively
# traversing the subdir hierarchy and counting individual file
# sizes
#
# @param   targetDir  the directory whose space usage to find
# @return  spaceUsed  the number of bytes used by the dir contents
# @author  sgehani@netscape.com
##
sub GetSpaceRequired()
{
    my($targetDir) = $_[0];
    my($spaceUsed) = 0;
    my(@dirEntries) = ();
    my($item) = "";

    @dirEntries = <$targetDir/*>;

    # iterate over all dir entries 
    foreach $item ( @dirEntries ) 
    {
        # if dir entry is dir
        if (-d $item)
        {       
            # add GetSpaceRequired(<dirEntry>) to space used
            $spaceUsed += GetSpaceRequired($item);
        }
        # else if dir entry is file
        elsif (-e $item)
        {
            # add size of file to space used
            $spaceUsed += (-s $item);
        }
    }

    return $spaceUsed;
}
