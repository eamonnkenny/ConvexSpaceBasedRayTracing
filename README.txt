//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    October 2001
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

Compile code g++ in cygwin
--------------------------

- g++ set up for release directory

Runtime (Cygwin):
----------------
In main directory use sequence of commands: 

  make multi (make) or    make single
  cd runtime
  ./run.bat

There are 230 convexs in the database.
The software returns the ray path information, stories and some images.


Compile code under VC++6.0
--------------------------

Open the runtime.dsw file in STIWRO-C++\runtime (Updated 12/12/01)

- Visual C++ 6.0 set up for Debug or Release mode (can be changed by user)

Runtime (VC++6.0): 
----------------

Type CTRL-F5 or execute under build
Type F5 to debug under build


Visualisation (Matlab)
-------------

Refer to Software Manual in Final Report 

Zipping directory
-----------------

Command to type if you agree with statement below:     make clean

if you want to remove all files in the runtime\Debug directory, all result 
files named *.txt in the runtime directory and all *.o files and executables in
the runtime\Release directory, as well as all VC++ files except *.dsw and *.dsp 
then run above command. 

Then zip the program using winzip or:  zip <filename.zip> -r <directory_name>

