//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Derek Bell, Update by: Eamonn Kenny
// Project Manager:       Dr. Peter J. Cullen
// First Modification:    January 2002
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef CRELOADBUILDING_HH
#define CRELOADBUILDING_HH

#include <iostream>
#include <iomanip>

#include "convex.hh"
#include "room.hh"
#include "story.hh"
#include "floor.hh"
#include "edgelist.hh"
#include "list.hh"
#include "point3d.hh"
#include "building.hh"

#include "intrinsic.hh"
#include "storage.hh"

class CReloadBuilding
//==========================================================================
// = FUNCTION 
//    reloader.hh
//
// = AUTHOR
//    Derek Bell
//
// = DESCRIPTION
//    Stores the transmitter location and reloads it if the stored
//    transmitter location is different from the default
//
// = VERSION
//    1.0
//
// = COPYRIGHT
//    COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//    All rights reserved 
//==========================================================================
{
  public:
    double         totalConvexes_;
    CBuilding      building_;
    CMemoryStorage objectStorage_;

    string fullDirectoryPath( const string& fileName );
    //
    // Obtain full directory from CProperties
    //

    CReloadBuilding();
    CReloadBuilding( const double& total, const CBuilding& b);
    //
    // default constructors
    //

    CReloadBuilding( const string& buildingFileName );
    CReloadBuilding( char* buildingFileName, char* buildingSaveFileName );
    //
    // using the building file name generate a saved file of connections
    // within that building, or read from it if the saved file already
    // exists assuming the transmitter location has not changed since last 
    // run.
    //

    void setTotalConvexes( const double& total);
    // 
    // set the transmitter location
    // 

    double getTotalConvexes();
    // 
    // get the transmitter location
    // 

    void setBuilding( const CBuilding& b );
    //
    // set the building data to be saved
    //

    CBuilding getBuilding();

    void writeTotalConvexes(ofstream& ostr);
    // 
    // save the transmitter location to the stream
    // 

    void readTotalConvexes(ifstream& ifstr);
    //
    // read in the total number of convexes in the building
    //

    void readBoundingBox( ifstream& ifstr );
    //
    // read in the bounding box for the building
    //

    void writeCBoundary(ostream& ostr, int convex, int facet);
    // Save a boundary to the stream

    void readCBoundary(ifstream& ifstr,int convex, int facet);

    void writeCPlane(ostream& ostr, const CPlane& p);
    // Save the an oriented plane to the stream

    void readCPlane(ifstream&, CPlane& CP );

    void writeCRoom(ostream& ostr, int roomnum);
    // Save a room to the stream

    void writeBoundingBox( ofstream& ofstr );
    //
    // write out the bounding box for the building
    //

    void readBuilding(const string& inputFileName);
    void readCRoom(ifstream& ifstr,int room);
    void readCStory(ifstream& ifstr,int);
    void readCFloor(ifstream& ifstr,int storey);
    void readRoof(ifstream& ifstr);
    void readCConvex(ifstream& ifstr,int storey);
    void readCPoint3d(ifstream& ifstr, CPoint3d& p);
    //
    // read functions for reading different parts of building connectivity
    // from a file stream
    //

    void writeBuilding(string outputFileName);
    void writeCStory(ostream& ostr, int story);
    void writeCFloor(ostream& ostr, int floor);
    void writeRoof(ostream& ostr);
    void writeCConvex(ostream&, int convex);
    void writeCPoint3d(ostream& ostr, const CPoint3d& p);
    //
    // write functions for sending different parts of building connectivity
    // to a file stream
    //

    void compareBuildings( const CBuilding& b1, const CBuilding& b2);
    //
    // compare the connectivity of a building to another.
    //
};
#endif
