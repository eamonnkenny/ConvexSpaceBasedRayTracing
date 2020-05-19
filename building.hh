//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Project Manager:       Dr. Peter J. Cullen
// Last Modification:     November 2001
// Software Tool Version: 1.0
//=========================================================================

#ifndef BUILDING_HH
#define BUILDING_HH

#include <iostream>
#include <iomanip>

#include "convex.hh"
#include "room.hh"
#include "story.hh"
#include "floor.hh"
#include "edgelist.hh"
#include "list.hh"
#include "box.hh"

#include "intrinsic.hh"
#include "storage.hh"

class CBuilding
//==========================================================================
// = FUNCTION 
// 	building.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      The definition of a building, containing floors, stories and a roof.
//      Also contains a list of all rooms, a list of all convex spaces.
//      Arrays of indexs into adjacent convex spaces
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:

      int         numberOfBuildings_;  // not used in first instance (value=1)

      CStory      *storyArray_;
      int         numberOfStories_;

      CFloor      *floorArray_;
      int         numberOfFloors_;

      CFloor      roof;
      //
      // define the stories, floors, and roof for a building
      // each array is dynamically allocated
      //

      CConvex     *convexArray_;
      int         numberOfConvexs_;

      CRoom       *roomArray_;
      int         numberOfRooms_;
      //
      // define arrays of all convexs and rooms for the current building
      // each array is dynamically allocated
      //

      CBoundingBox boundingBox_;
      //
      // The minimum bounding box of the building is stored as six numbers
      //

      CBuilding();
      //
      // default constructor with no objects in it
      //

      void connectConvexsAndRooms( string buildingFileName, string roomFile );
      //
      // read the contents of a building from a file in a database
      // given that we know the number of convex spaces and rooms in a
      // building it is now possible to define them. And then a call
      // is made to the link routines (see below) to link convexs, rooms,
      // floors, etc.
      //

      void getNumberOfConvexs( const string& buildingFileName );
      void getNumberOfRooms(   const string& buildingFileName );
      // 
      // reads the building database to see how many convexs and rooms are
      // stored within it.
      //

      void linkToConvexsInAllRooms( const string& buildingFileName );
      void linkToConvexsInAllFloors( const string& buildingFileName );
      void linkToConvexsInAllStories( const string& buildingFileName );
      void linkToRoomsInAllStories( const string& buildingFileName );
      void linkAllConvexsToOtherConvexs();
      void linkAllRoomsToOtherRooms( const string& buildingFileName );
      //
      // make the links between rooms, floors, stories with convexs, and
      // with convexs to other convexs.
      //

      char readABuildingMaterialType( ifstream& filePtr );
      //
      // from the database read in a building material type and 
      // the convex information that follows it, or the number that follows
      // it
      //

      boolean locateConvexNumber( const CPoint3d& pt, int& convexNumber ) const;
      boolean locateConvexNumber2( const CPoint3d& pt, int& convexNumber ) const;
      //
      // if a point pt is in a particular convex space in the building
      // return true and the index of that convex space in the convex array
      // otherwise return false.
      // Last edited: 20/11/01
      //

      void locateConvexNumber( List<CEdge>& diffractionEdges,
                               int       *diffractionEdgeConvexNumbers ) const;
      void locateConvexNumber2( List<CEdge>& diffractionEdges,
                                int       *diffractionEdgeConvexNumbers ) const;
      //
      // Added: 10/12/01
      // locate the convex in which each diffraction edge exists.
      //

      //List<CPlane>& calculateReflectionPlanes() const;
      void calculateReflectionPlanes( List<CPlane>& planeList ) const;
      //
      // calculate all possible reflection planes in the current building
      // using only coplanar boundaries of filled convex spaces to do this.
      // Added: 26/11/01 but moved from reflection.hh to here 04/12/01
      // because it is needed in diffraction.hh and reflection.hh
      //

      void updateBoundingBox( const CConvex& convexObject );
      //
      // update the dimensions of a bounding box surrounding the building
      //
};

#endif
