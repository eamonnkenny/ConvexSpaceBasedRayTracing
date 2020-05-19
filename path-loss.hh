//=========================================================================
// COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    4th January 2002
// Last Modification:     Dated Below "Added dd/mm/yy"
// Previous Projects:     Storms, FMW/ARG
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================
#ifndef TCD_PATHLOSS_HH
#define TCD_PATHLOSS_HH

#include <string>
#include "userconst.hh"
#include "point3d.hh"
#include <cmath>
#include <iostream>


#ifdef __GNUG__
  #define EXPORT_CLASS_DECLSPEC class 
#else
  #define EXPORT_CLASS_DECLSPEC class __declspec(dllexport)
#endif

EXPORT_CLASS_DECLSPEC CPathLoss
//=========================================================================
// = FUNCTION 
//     path-loss.hh
//
// = AUTHOR
//     Eamonn M. Kenny
//
// = DESCRIPTION
//     Generate a class which will deal with Path Loss on a regular grid
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 1995-2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//=========================================================================
{
  private:
    double  xSouthWest_;
    double  ySouthWest_;
    double  xNorthEast_;
    double  yNorthEast_;
    double  xStep_;
    double  yStep_;
    int     xNumberOfPts_;
    int     yNumberOfPts_;
    double* pathLossVector_;

  public:
    CPathLoss();
    CPathLoss( double xSouthWest, double ySouthWest, double     xStep,
               double yStep,      int  xNumberOfPts, int yNumberOfPts );
    //
    // Default constructors to define the grid boundaries 
    //

    CPathLoss( double  xSouthWest, double ySouthWest, double     xStep,
               double  yStep,      int  xNumberOfPts, int yNumberOfPts,
               double* pathLossVector );
    //
    // Constructors to define the grid boundaries
    // This can be used for portability to other packages
    //

    void setZeros();
    //
    // Set the contents of the path-loss grid to have value zero everywhere
    // 

    void   setPathLoss( double height, int i, int j );
    //
    // set the path loss at a grid reference point
    //

    double getPathLoss( int i, int j ) const;
    double getPathLoss( long x, long y ) const;
    //
    // return the path loss at a grid reference point or by interpolation.
    //

    CPoint3d getPoint( int i, double z ) const;
    //
    // return the location of a point on the grid
    //

    double* getPathLossVector();
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

    CPathLoss& operator=( const CPathLoss& otherPathLoss );
    //
    // aisignment operator overload
    //

    void clear()
    {
      if ( xNumberOfPts_ > 0 && xNumberOfPts_ < 10000 
        && yNumberOfPts_ > 0 && yNumberOfPts_ < 10000 )
      {
        delete [] pathLossVector_;
        xNumberOfPts_ = 0;
        yNumberOfPts_ = 0;
      }
    }

    ~CPathLoss()
    {
      clear();
    }
};

#endif
