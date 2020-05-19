//=========================================================================
// COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
// All rights reserved  
//=========================================================================
// Author:                Eamonn M. Kenny
// Principal Researcher:  Dr. Peter J. Cullen
// First Modification:    November 26th 2001
// Previous Projects:     none
// Current  Project:      IP2000 STIWRO
// Software Tool Version: 1.0
//=========================================================================

#ifndef PLANES_HH
#define PLANES_HH

#include <iostream>
#include "point3d.hh"
#include "segment3d.hh"

class CPlane
//==========================================================================
// = FUNCTION 
// 	plane.hh
//
// = AUTHOR
//      Eamonn M. Kenny
//
// = DESCRIPTION
//      Calculate equation of a plane is given by Ax + By + Cz = D
//      Storing it as a unit outward normal (A,B,C) and D is shown
//      in Thomas Calculus (10th edition).
//
// = VERSION
//     1.0
//
// = COPYRIGHT
//     COPYRIGHT (c) 2003 by Trinity College Dublin, Dublin 2, IRELAND
//     All rights reserved 
//==========================================================================
{
  public:
     CPoint3d outwardNormal_;
     double   D_;

     CPlane()
     {
       outwardNormal_ = CPoint3d( 0.0, 0.0, 0.0 );
       D_             = -1;
     }

     CPlane( const CPoint3d& normal, double rhs_constant );
     CPlane( const CPoint3d& p1, const CPoint3d& p2 ); // Added 27/11/01
     //
     // construct a plane given a normal and the constant 
     //

     CPlane& operator=( const CPlane& plane2 );
     //
     // return the copied plane
     //

     boolean operator==( const CPlane& plane2 ); // Added 27/11/01
     boolean operator!=( const CPlane& plane2 ); // Added 27/11/01
     //
     // return true if planes are equal
     //

     friend ostream& operator<< ( ostream& s, const CPlane& plane2 );
     //
     // output plane information to a file buffer, or screen.
     //

     double  sign( const CPoint3d& p ); // Added: 27/11/01
     int    isign( const CPoint3d& p );
     //
     // Calculate whether p is on the positive side of the plane or not
     // returns -1 for negative side, 1 for positive, 0 for on the plane.
     //

     double   distance( const CPoint3d& aPoint );
     //
     // distance from a point to the current plane
     //

     double   shortestDistance( const CPoint3d& aPoint );
     //
     // absolute valid of shortest distance from a point to the current plane
     //

     double   signedShortestDistance( const CPoint3d& aPoint );
     //
     // signed shortest distance from a point to the current plane
     //

     CPoint3d image( const CPoint3d& aPoint );
     //
     // image of a point in the plane
     //

     boolean  validImage( const CPoint3d& aPoint, CPoint3d& newPoint );
     //
     // image of a point in the plane to create a new point, if this is
     // possible.
     //

     boolean  validImage( const CSegment3d& aSegment, CSegment3d& newSegment );
     //
     // calculate the image of a segment in the plane to create a new
     // segment, if this is possible. Added: 04/12/01
     //
};

#endif
