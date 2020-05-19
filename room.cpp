//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    9th January 2002 
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#include "room.hh"

void CRoom::clear()
{
  if ( numberOfConvexs_ > 0 && numberOfConvexs_ < MAX_CONVEX )
  {
    delete [] convexSpaceIndex_;
    numberOfConvexs_ = 0;
  }
  if ( numberOfRoomsBelow_ > 0 && numberOfRoomsBelow_ < MAX_CONVEX )
  {
    delete [] roomsBelow_;
    numberOfRoomsBelow_ = 0;
  }
  if ( numberOfRoomsAdjacent_ > 0 && numberOfRoomsAdjacent_ < MAX_CONVEX )
  {
    delete [] roomsAdjacent_;
    numberOfRoomsAdjacent_ = 0;
  }
  if ( numberOfRoomsAbove_ > 0 && numberOfRoomsAbove_ < MAX_CONVEX )
  {
    delete [] roomsAbove_;
    numberOfRoomsAbove_ = 0;
  }
}

CRoom& CRoom::operator=( const CRoom& otherRoom )
{
  int i;

  clear();

  if ( otherRoom.numberOfConvexs_ > 0 
    && otherRoom.numberOfConvexs_ < MAX_CONVEX )
  {
    numberOfConvexs_ = otherRoom.numberOfConvexs_;
    convexSpaceIndex_ = new int [numberOfConvexs_];
    for ( i = 0; i < numberOfConvexs_; i++ )
      convexSpaceIndex_[i] = otherRoom.convexSpaceIndex_[i];
      
  }

  if ( otherRoom.numberOfRoomsBelow_ > 0
    && otherRoom.numberOfRoomsBelow_ < MAX_CONVEX )
  {
    numberOfRoomsBelow_ = otherRoom.numberOfRoomsBelow_;
    roomsBelow_ = new int [numberOfRoomsBelow_];
    for ( i = 0; i < numberOfRoomsBelow_; i++ )
      roomsBelow_[i] = otherRoom.roomsBelow_[i];
  }

  if ( otherRoom.numberOfRoomsAdjacent_ > 0
    && otherRoom.numberOfRoomsAdjacent_ < MAX_CONVEX )
  {
    numberOfRoomsAdjacent_ = otherRoom.numberOfRoomsAdjacent_;
    roomsAdjacent_ = new int [numberOfRoomsAdjacent_];
    for ( i = 0; i < numberOfRoomsAdjacent_; i++ )
      roomsAdjacent_[i] = otherRoom.roomsAdjacent_[i];
  }

  if ( otherRoom.numberOfRoomsAbove_ > 0 
    && otherRoom.numberOfRoomsAbove_ < MAX_CONVEX )
  {
    numberOfRoomsAbove_ = otherRoom.numberOfRoomsAbove_;
    roomsAbove_ = new int [numberOfRoomsAbove_];
    for ( i = 0; i < numberOfRoomsAbove_; i++ )
      roomsAbove_[i] = otherRoom.roomsAbove_[i];
  }

  return *this;
}
