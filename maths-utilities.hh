//=========================================================================
// COPYRIGHT (c) 2015-2017 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Last Modification:     August 2015
// Previous Projects:     Storms, FMW/ARG, IP2000 STIWRO
// Current  Project:      None
// Software Tool Version: 2.0
//=========================================================================
#ifndef MATHS_UTILITIES_HH
#define MATHS_UTILITIES_HH

using namespace std;
#include "complex.hh"

class CMaths
//==========================================================================
// = FUNCTION 
// 	maths-utilities.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Definition of a set of double valued and complex number mathematical 
//      functions that don't sit well inside the object oriented programming
//      paradigm of modern C++. 
//      e.g:    expj(z) as opposed to z.expj() which is not intuitive
//
// = VERSION
//     2.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2015-2017 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      static complex  expj( const double& d );
      static complex  expj( const complex& v );
      static complex  expminusj( const double& d );
      static complex  expminusj( const complex& v );
      static complex  exp( const complex& v );
      static complex  csqrt( const complex& v );
};

#endif
