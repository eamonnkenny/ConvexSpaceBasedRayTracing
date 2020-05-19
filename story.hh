//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     Oct 2001
// Software Tool Version: 1.0
//=========================================================================

#ifndef STORY_HH
#define STORY_HH

#include <iostream>
#include "room.hh"
#include "convex.hh"

class CStory
//==========================================================================
// = FUNCTION 
//     story.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of a story, containing convex spaces and rooms.
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
      int  *roomIndex_;
      int  numberOfRooms_;
      int  *convexSpaceIndex_;
      int  numberOfConvexs_;

      CStory()
      {
        numberOfRooms_   = 0;
        numberOfConvexs_ = 0;
        // default constructor
      }

      //void read( istream& filePointer );
      //
      // read in the stories convexs and rooms
      // 

      ~CStory()
      {
        if ( numberOfRooms_ > 0 && numberOfConvexs_ < MAX_CONVEX )
          delete [] roomIndex_;
        if ( numberOfConvexs_ > 0 && numberOfConvexs_ < MAX_CONVEX )
          delete [] convexSpaceIndex_;
      }
};

#endif
