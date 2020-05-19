//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  
// Last Modification:     November 2001
// Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
// Current  Project:      
// Software Tool Version: 2.0
//=========================================================================
#ifndef FILE_CONFIGURATIONS_HH
#define FILE_CONFIGURATIONS_HH

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#define BOOST_NO_CXX11_SCOPED_NUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_NUMS

using std::ostream;
using namespace std;

class CProperties
//==========================================================================
// = FUNCTION 
// 	configuration.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Define configurations for all the directory structures of the project
//
// = VERSION
//     1.2
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  //private:
  public:
      void read( const string& xmlInputFileName );
      static string btsDirectory;
      static string diagramDirectory;
      static string buildingDirectory;
      static string windowsDirectory;
      static string linuxDirectory;
      static string matlabDirectory;
      static string matlabOptimisationDirectory;
      static string matlabRayPathsDirectory;
      static string matlabCoverageDirectory;
      static string matlabBuildingDirectory;
      static string matlabThesisDirectory;
      static string measurementDirectory;
      static string baseDirectory;
};

#endif
