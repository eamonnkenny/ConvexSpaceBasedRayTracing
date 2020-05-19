//=========================================================================
// COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    20th May 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef CAPACITY_HH
#define CAPACITY_HH

#include <iostream>
#include <cmath>
#include <string>
#include "userconst.hh"
#include "point3d.hh"
#include "box.hh"
#include "building.hh"


#ifdef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class 
#else
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport)
#endif

EXPORT_CLASS_DECLSPEC CCapacity
//=========================================================================
// = FUNCTION 
//     capacity.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Grid of capacity requirements for a cross-section of z-axis
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2001-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  private:
    double  xSouthWest_;
    double  ySouthWest_;
    double  zSouthWest_;
    double  xNorthEast_;
    double  yNorthEast_;
    double  xStep_;
    double  yStep_;
    int     xNumberOfPts_;
    int     yNumberOfPts_;
    double* capacityVector_;
    double  totalCapacity_;
    int     numberOfServers_;

  public:
    CCapacity();
    CCapacity( char* capacityGridFileName, const CBaseTransceiver& btsObject );
    void setValues( double xSouthWest, double ySouthWest, double zSouthWest,
                    double xStep,      double yStep,      int    xNumberOfPts, 
                    int    yNumberOfPts );
    //
    // Default constructors to define the grid boundaries 
    //

    CCapacity( double xSouthWest, double ySouthWest, double zSouthWest,
               double xStep,      double yStep,      int    xNumberOfPts, 
               int    yNumberOfPts, double* capacityVector );
    //
    // Constructors to define the grid boundaries
    // This can be used for portability to other packages
    //

    void setZeros();
    //
    // Set the contents of the path-loss grid to have value zero everywhere
    // 

    void   setCapacity( double height, int i, int j );
    //
    // set the path loss at a grid reference point
    //

    double getCapacity( int i, int j ) const;
    double getCapacity( long x, long y ) const;
    //
    // return the path loss at a grid reference point or by interpolation.
    //

    CPoint3d getPoint( int i, int j ) const;
    CPoint3d getPoint( int i, double z ) const;
    //
    // Added 29/05/02
    // return the point for the index (i,j) on the grid
    //

    double* getCapacityVector();
    //
    // return the fully memory allocated path loss vector
    //

    void setSouthWest( double x, double y ) 
    { xSouthWest_ = x; ySouthWest_ = y; }
    void setNorthEast( double x, double y )
    { xNorthEast_ = x; yNorthEast_ = y; }
    void setStepSize( double x, double y )
    { xStep_ = x; yStep_ = y; }
    void setNumberOfPoints( int x, int y )
    { xNumberOfPts_ = x; yNumberOfPts_ = y; }
    //
    // set the start up values 
    //

    double length() const { return (xNorthEast_ - xSouthWest_); }
    double height() const { return (yNorthEast_ - ySouthWest_); }
    double xSouthWest() const { return xSouthWest_; }
    double ySouthWest() const { return ySouthWest_; }
    double zSouthWest() const { return zSouthWest_; }
    double xNorthEast() const { return xNorthEast_; }
    double yNorthEast() const { return yNorthEast_; }
    double xStepSize()  const { return xStep_; }
    double yStepSize()  const { return yStep_; }
    int    xNumberOfPoints() const { return xNumberOfPts_; }
    int    yNumberOfPoints() const { return yNumberOfPts_; }
    int    numberOfPoints() const { return xNumberOfPts_ * yNumberOfPts_; }
    //
    // return vertices of the path-loss grid, or step-sizes, or number of Pts
    //

    CCapacity& operator=( const CCapacity& otherCapacity );
    //
    // aisignment operator overload
    //

    double operator[]( int i ) const { return capacityVector_[i]; }
    //
    // aisignment operator overload
    //

    CPoint3d centreOfCapacity( const CBoundingBox& box, double& total ) const;
    //
    // centre of capacity
    //

    List<CBoundingBox>& splitIntoCells( const CBuilding&        buildingObject,
                                        const CBaseTransceiver& btsObject,
					      List<CPoint3d>&   centres );
    //
    // cell division counter
    //

    double totalCapacity( const CBoundingBox& box ) const;
    //
    // total Capacity requirement inside a particular box
    //

    void clear()
    {
      if ( xNumberOfPts_ > 0 && xNumberOfPts_ < 10000 
        && yNumberOfPts_ > 0 && yNumberOfPts_ < 10000 )
      {
        delete [] capacityVector_;
        xNumberOfPts_ = 0;
        yNumberOfPts_ = 0;
      }
    }

    ~CCapacity()
    {
      //clear();
    }
};

#endif
