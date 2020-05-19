//=========================================================================
// COPYRIGHT (c) 2000-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     Oct 2001
// Software Tool Version: 1.0
//=========================================================================

#ifndef ROOM_HH
#define ROOM_HH

#include <iostream>
#include "point3d.hh"
#include "convex.hh"

class CRoom
//==========================================================================
// = FUNCTION 
// 	room.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      A room contains a list of convex spaces, and connections to adjacent
//      rooms.
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
      int          *convexSpaceIndex_;
      int          numberOfConvexs_;
      //
      // define all convexes in the current room
      //

      int          *roomsBelow_, *roomsAdjacent_, *roomsAbove_;
      int          numberOfRoomsBelow_;
      int          numberOfRoomsAdjacent_;
      int          numberOfRoomsAbove_;
      //
      // define all rooms connected to the current room
      //
      CRoom()
      {
        numberOfConvexs_ = 0;
        numberOfRoomsAdjacent_ = 0;
        numberOfRoomsBelow_ = 0;
        numberOfRoomsAbove_ = 0;
		centroid_ = CPoint3d( 0.0, 0.0, 0.0 );
      }

      CPoint3d     centroid_;
      //
      // centroid of the room 
      //

      void         read( istream& filePointer );
      //
      // method to read the room from a file
      //

      void clear();
      //
      // destructor for rooms Added 09/01/02
      //

      CRoom& operator=( const CRoom& otherRoom );
      //
      // assignment operator overload Added 09/01/02
      //

      ~CRoom()
      {
        clear(); // Added 09/01/02
      }
};

#endif
