//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     Oct 2001
// Software Tool Version: 1.0
//=========================================================================

#ifndef FLOOR_HH
#define FLOOR_HH

#include <iostream>
#include "convex.hh"

class CFloor
//==========================================================================
// = FUNCTION 
// 	story.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of a floor, containing convex spaces.
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2 IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
      int  *convexSpaceIndex_;
      int  numberOfConvexs_;
      
      CFloor()
      {
        numberOfConvexs_ = 0;
	// default constructor
      }

      void     read( istream& filePointer );
      //
      // read the pointers to convexs in a floor
      //
};

#endif
